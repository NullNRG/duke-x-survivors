// DXV41 browser primary renderer.  EDuke32 owns gameplay; Three.js owns ALL 3D pixels.
// No FX overlay mode, no colored placeholder world, no spheres in place of actors.
import * as THREE from 'three';
import {flatUV,wallUV,worldSig,preferThree,sectorRings,ringLayers,buildHorizonPitch,oneWayVisible} from './dukex-v43-mapping.mjs';

const STRIDE=20, XY=1/1024, Z=1/16384, ITEM_LIMIT=65536;
const V=(x,y,z)=>new THREE.Vector3(x*XY,-z*Z,y*XY);
const clamp=(x,lo,hi)=>Math.max(lo,Math.min(hi,x));
const toNum=x=>Number.isFinite(x)?x:0;
const read=(mod,prepare,ptr,limit,stride)=>{
  if(typeof mod?.[prepare]!=='function'||typeof mod?.[ptr]!=='function')return null;
  const count=mod[prepare]();if(!Number.isInteger(count)||count<=0||count>limit)return null;
  const offset=mod[ptr]()>>>0;if(!offset||(offset&3))return null;
  const base=offset>>>2,length=count*stride;
  if(!mod.HEAPF32||base+length>mod.HEAPF32.length)return null;
  return mod.HEAPF32.slice(base,base+length);
};
function geometry(vertices,indices,uv){
  const position=[],tex=[],normal=[];
  for(const ids of indices)for(const i of ids){const p=vertices[i],t=uv[i];
    position.push(p.x,p.y,p.z);tex.push(t[0],t[1]);}
  if(!position.length)return null;
  const g=new THREE.BufferGeometry();g.setAttribute('position',new THREE.Float32BufferAttribute(position,3));
  g.setAttribute('uv',new THREE.Float32BufferAttribute(tex,2));g.computeVertexNormals();return g;
}
function disposeTree(root){root?.traverse(node=>{node.geometry?.dispose();});}
class TileBank {
  constructor(mod){this.mod=mod;this.textures=new Map();this.materials=new Map();this.max=2048;this.errors=0;this.mapEpoch=0;this.animated=new Map();this.animationLast=0;}
  get(tile,pal=0,shade=0){
    tile=Math.trunc(tile);pal=clamp(Math.trunc(pal)||0,0,255);
    shade=clamp(Math.trunc(shade)||0,0,31);
    const key=tile+':'+pal+':'+shade;if(this.textures.has(key))return this.textures.get(key);
    if(this.textures.size>=this.max){this.errors++;if(this.errors===1)console.warn("Duke X: tile GPU cache capacity reached; some art may not render");return null;}
    const m=this.mod();if(!m?._DukeX_V41_TilePrepare)return null;
    const n=m._DukeX_V43_TilePrepare?.(tile,pal,shade) ?? m._DukeX_V41_TilePrepare(tile,pal);
    const w=m._DukeX_V41_TileWidth?.(),h=m._DukeX_V41_TileHeight?.(),ptr=m._DukeX_V41_TilePtr?.();
    if(!n||!w||!h||w>512||h>512||n!==w*h*4||!(ptr>>>0)||(ptr>>>0)+n>m.HEAPU8.length){
      this.errors++;return null;
    }
    // Native tile buffer is row-major RGBA produced from original Build ART and palette.
    const data=new Uint8Array(m.HEAPU8.slice(ptr>>>0,(ptr>>>0)+n));
    const texture=new THREE.DataTexture(data,w,h,THREE.RGBAFormat);
    texture.magFilter=THREE.NearestFilter;texture.minFilter=THREE.NearestFilter;
    texture.generateMipmaps=false;texture.colorSpace=THREE.SRGBColorSpace;
    texture.wrapS=THREE.RepeatWrapping;texture.wrapT=THREE.RepeatWrapping;
    texture.flipY=true;texture.needsUpdate=true;
    // Tile cache is bounded by material reuse and explicit resource eviction on map changes.
    this.textures.set(key,{texture,w,h});return {texture,w,h};
  }
  mat(tile,pal=0,shade=0,masked=false,translucence=0){
    shade=clamp(Math.trunc(shade)||0,0,31);
    const key=`${tile}/${pal}/${shade}/${masked?1:0}/${translucence}`;
    if(this.materials.has(key))return this.materials.get(key);
    const data=this.get(tile,pal,shade);if(!data)return null;
    // Indexed palette RGB is fully baked in C++. Alpha-tested pixels must WRITE
    // depth or overlapping fences/sprites will draw through one another.
    const material=new THREE.MeshBasicMaterial({map:data.texture,side:THREE.DoubleSide,
      alphaTest:masked?.49:0,transparent:translucence>0,
      opacity:translucence===2?.33:translucence===1?.66:1,
      depthWrite:translucence===0,toneMapped:false});
    this.materials.set(key,material);
    const m=this.mod();
    if(m?._DukeX_V44_TileAnimationFrames?.(tile)>0)this.animated.set(key,{tile,pal,shade,material,last:tile});
    return material;
  }
  updateAnimations(now){
    if(now-this.animationLast<110 || !this.animated.size)return;
    this.animationLast=now;
    const m=this.mod();if(!m?._DukeX_V44_TileAnimationOffset)return;
    for(const state of this.animated.values()){
      const current=state.tile+m._DukeX_V44_TileAnimationOffset(state.tile);
      if(current===state.last)continue;
      const data=this.get(current,state.pal,state.shade);
      if(!data)continue;
      state.material.map=data.texture;state.material.needsUpdate=true;state.last=current;
    }
  }
  clear(){for(const v of this.materials.values())v.dispose();this.materials.clear();
    for(const v of this.textures.values())v.texture.dispose();this.textures.clear();this.animated.clear();}
}
export class DukeXPrimaryRenderer {
  constructor(nativeCanvas){
    this.nativeCanvas=nativeCanvas;this.module=()=>window.Module;this.tiles=new TileBank(this.module);
    this.active=false;this.alive=true;this.wantsThree=false;this.world=null;this.sprites=new Map();this.freeSprites=[];this.sharedSpriteGeo=new THREE.PlaneGeometry(1,1);this.lastWorld=0;this.worldSignature="";this.worldFingerprint=0;this.worldSector=-1;this.oneWayWalls=[];this.skyTextures=[];this.lastWorldChange=0;this.diagnostics={missingTiles:0,hiddenSprites:0};
    this.lastFrame=0;this.frameCount=0;this.fps=0;this.lastSize='';this.missing=0;
    this.root=document.createElement('canvas');this.root.id='dukex-v41-primary';
    this.root.style.cssText='position:fixed;z-index:3;pointer-events:none;display:none';
    document.body.appendChild(this.root);
    this.renderer=new THREE.WebGLRenderer({canvas:this.root,alpha:false,antialias:false,powerPreference:'high-performance'});
    this.renderer.outputColorSpace=THREE.SRGBColorSpace;
    this.renderer.toneMapping=THREE.NoToneMapping;
    this.renderer.toneMappingExposure=1;
    this.pixelScale=Math.min(1.25,window.devicePixelRatio||1);this.lowFrames=0;
    this.scene=new THREE.Scene();this.scene.background=new THREE.Color(0x111521);
    this.scene.fog=null;
    // Sprite and wall colors are native palette values; do not add unrequested light tint.
    this.camera=new THREE.PerspectiveCamera(78,1,.01,210);
    this.weaponLights=[]; // indexed-color materials bypass physically based lights
    // Native V42 endpoints: GPU geometry, not a texture overlay or LASERLINE billboard.
    this.beamMeshes=[];this.beamAxis=new THREE.Vector3(0,1,0);
    const beamGeo=new THREE.CylinderGeometry(1,1,1,8,1,true);
    for(let i=0;i<112;i++)for(let layer=0;layer<2;layer++){
      const mat=new THREE.MeshBasicMaterial({color:layer?0xff2727:0xffeeee,
        transparent:!!layer,opacity:layer?.24:1,depthWrite:!layer,blending:layer?THREE.AdditiveBlending:THREE.NormalBlending});
      const mesh=new THREE.Mesh(beamGeo,mat);mesh.visible=false;
      mesh.frustumCulled=false;this.scene.add(mesh);this.beamMeshes.push(mesh);
    }
    // Postprocessing render targets were created even with postFX disabled.
    // The indexed ART renderer draws directly to the GPU canvas.
    this.hud=document.createElement('div');this.hud.id='dukex-v41-hud';
    this.hud.style.cssText='display:none;position:fixed;z-index:11;pointer-events:none;color:#fff;font:700 13px system-ui;text-shadow:0 2px 4px #000;max-width:400px';
    this.hud.innerHTML='<div style="display:flex;gap:12px"><span id="dx41hp"></span><span id="dx41armor"></span><span id="dx41wave"></span></div><div style="height:5px;background:#000b;margin-top:5px;border-radius:5px"><div id="dx41xp" style="height:100%;width:0;background:#7ef5ed;border-radius:5px"></div></div><small id="dx41stats"></small>';
    document.body.appendChild(this.hud);
    this.status=document.createElement('div');this.status.id='dukex-v42-render-status';
    this.status.style.cssText='display:none;position:fixed;top:48px;left:12px;z-index:1080;background:#3218;color:#fff;padding:8px;font:12px system-ui';
    document.body.appendChild(this.status);
    this.onContextLost=e=>{e.preventDefault();this.wantsThree=false;this.deactivate('WebGL context lost');};
    this.root.addEventListener('webglcontextlost',this.onContextLost);
    // Renderer selection is controlled only by the web Settings panel.
    this.toggle=null;this.onKey=null;
    
    this.frame=this.frame.bind(this);requestAnimationFrame(this.frame);
  }
  ready(){const m=this.module();return !!(m?.HEAPF32&&m?.HEAPU8&&m._DukeX_V41_WorldPrepare&&
    m._DukeX_V41_FramePrepare&&m._DukeX_V41_TilePrepare&&m._DukeX_V41_SetPrimary&&m._DukeX_V43_TilePrepare&&m._DukeX_V44_Version?.()===45&&m._DukeX_V45_Version?.()===45&&m._DukeX_V44_WorldFingerprint);}
  setNativeVisibility(hidden){
    // Native canvas remains in document for Emscripten pointer lock/touch/game menus;
    // it never contributes a pixel to the Three scene and is not a shader input.
    this.nativeCanvas.style.opacity=hidden?'0':'';
    this.nativeCanvas.style.visibility='visible';
  }
  activate(){
    if(!this.ready())return false;
    const m=this.module();if(m._DukeX_V41_HudPrepare?.()!==12)return false;
    const hp=m._DukeX_V41_HudPtr?.();if(!(hp>>>0)||m.HEAPF32[(hp>>>2)+12]!==1)return false;
    const w=read(m,'_DukeX_V41_WorldPrepare','_DukeX_V41_WorldPtr',ITEM_LIMIT,STRIDE);
    const f=read(m,'_DukeX_V41_FramePrepare','_DukeX_V41_FramePtr',2048,STRIDE);
    if(!w||!f||f[0]!==0)return false;
    // Never hide the native renderer until a complete real geometry snapshot exists.
    if(!this.makeWorld(w))return false;
    this.updateFrame(f);this.resize();
    this.active=true;this.wantsThree=true;this.root.style.display='block';this.hud.style.display='block';
    this.setNativeVisibility(true);m._DukeX_V41_SetPrimary(1);
    this.status.style.display='none';this.lastWorld=performance.now();this.updateToggle();return true;
  }
  deactivate(reason=''){
    if(this.module()?._DukeX_V41_SetPrimary)this.module()._DukeX_V41_SetPrimary(0);
    if(reason){this.wantsThree=false;try{localStorage.setItem('dukex-renderer','classic');}catch{}}
    this.active=false;this.root.style.display='none';this.hud.style.display='none';
    this.setNativeVisibility(false);this.status.style.display=reason?'block':'none';this.status.textContent=reason?'GPU backend error — original engine restored: '+String(reason).slice(0,100):'';
    this.updateToggle();if(reason)console.error('[Duke X V43]',reason);
  }
  updateToggle(){ /* Settings manages selection. No gameplay overlay. */ }
  setMode(mode,persist=false){
    this.wantsThree=mode==='three';if(persist)try{localStorage.setItem('dukex-renderer',this.wantsThree?'three':'classic');}catch{}
    if(!this.wantsThree){this.deactivate();this.status.style.display='none';}
    else if(!this.active)this.activate();
    this.updateToggle();return this.wantsThree;
  }
  // Concatenate the actual Build PSKY tile sequence, preserving indexed ART
  // palette colors and nearest-neighbor sampling. One atlas per sky per map.
  skyAtlas(baseTile,pal=0){
    const m=this.module();
    const count=clamp(m?._DukeX_V45_SkyPanels?.(baseTile)||1,1,16);
    const panels=[];
    for(let i=0;i<count;i++){
      const tile=m?._DukeX_V45_SkyPanelTile?.(baseTile,i)??baseTile;
      const entry=this.tiles.get(tile,pal,0);
      if(entry)panels.push(entry);
    }
    if(!panels.length)return null;
    const w=panels[0].w,h=panels[0].h;
    const maxSide=Math.min(this.renderer.capabilities.maxTextureSize||4096,8192);
    const countUsed=Math.min(panels.length,Math.max(1,Math.floor(maxSide/w)));
    if(w*countUsed>maxSide||h>maxSide)return panels[0].texture;
    const rgba=new Uint8Array(w*countUsed*h*4);
    for(let i=0;i<countUsed;i++){
      const panel=panels[i];const bytes=panel.texture.image.data;
      for(let y=0;y<h;y++)for(let x=0;x<w;x++){
        const ox=(y*w*countUsed+i*w+x)*4;
        const sx=(Math.min(y,panel.h-1)*panel.w+Math.min(x,panel.w-1))*4;
        rgba.set(bytes.subarray(sx,sx+4),ox);
      }
    }
    const atlas=new THREE.DataTexture(rgba,w*countUsed,h,THREE.RGBAFormat);
    atlas.magFilter=THREE.NearestFilter;atlas.minFilter=THREE.NearestFilter;
    atlas.wrapS=atlas.wrapT=THREE.RepeatWrapping;
    atlas.colorSpace=THREE.SRGBColorSpace;atlas.generateMipmaps=false;
    atlas.flipY=true;atlas.needsUpdate=true;
    this.skyTextures.push(atlas);return atlas;
  }
  makeWorld(data){
    const bySector=new Map(),anchors=new Map(),walls=[],wallSector=new Map(),oneWaySign=new Map();
    for(let i=0;i<data.length;i+=STRIDE){const r=data.subarray(i,i+STRIDE);
      if(r[0]===7)anchors.set(r[1],r);
      else if(r[0]===1){let sec=bySector.get(r[1]);if(!sec){sec=[];bySector.set(r[1],sec);}sec.push(r);wallSector.set(r[18]|0,r[1]|0);}
      else if(r[0]===2||r[0]===4||r[0]===5||r[0]===6)walls.push(r);
    }
    if(!bySector.size)return false;
    const next=new THREE.Group();next.name='EDuke32-native-primary-world';
    const groups=new Map();const oneWayGroups=[];let surfaces=0,triCount=0;
    const add=(sector,tile,pal,shade,masked,vertices,triangles,uv,wallRecord=null,translucence=0)=>{
      const mat=this.tiles.mat(tile,pal,shade,masked,translucence);if(!mat){this.diagnostics.missingTiles++;return;}
      // Material batches are SECTOR-local so Three.js can cull off-screen rooms.
      const key=`${sector}/${tile}/${pal}/${shade}/${masked?1:0}/${translucence}/${wallRecord?wallRecord[1]:"static"}`;
      let group=groups.get(key);if(!group){group={mat,pos:[],uv:[],wall:wallRecord};groups.set(key,group);}
      for(const tri of triangles)for(const idx of tri){let v=vertices[idx],u=uv[idx];
        group.pos.push(v.x,v.y,v.z);group.uv.push(u[0],u[1]);}
      triCount+=triangles.length;surfaces++;
    };
    this.nextSky=null;
    for(const oldSky of this.skyTextures)oldSky.dispose();this.skyTextures=[];
    for(const [id,records] of bySector){
      if(records.length<3||records.length>4096)continue;
      const anchor=anchors.get(id);
      if((records[0][9]&1)&&!this.nextSky){
        const skyTexture=this.skyAtlas(records[0][7],records[0][13]);
        if(skyTexture){const dome=new THREE.Mesh(new THREE.SphereGeometry(110,48,16),
          new THREE.MeshBasicMaterial({map:skyTexture,side:THREE.BackSide,depthWrite:false,fog:false,toneMapped:false}));
          const uv=dome.geometry.getAttribute('uv');for(let j=0;j<uv.count;j++)uv.setXY(j,uv.getX(j),uv.getY(j)*2);
          uv.needsUpdate=true;dome.renderOrder=-100;next.add(dome);this.nextSky=dome;}
      }
      const rings=sectorRings(records);
      for(const ring of rings){let signed=0;
        for(let k=0;k<ring.length;k++){const a=ring[k],b=ring[(k+1)%ring.length];
          signed+=a[2]*b[3]-b[2]*a[3];}
        for(const r of ring)oneWaySign.set(r[18]|0,signed>=0?1:-1);
      }
      for(const layer of ringLayers(rings)){
        // Build sectors can contain multiple wall loops and nested holes.
        const all=[...layer.outer,...layer.holes.flat()];
        const poly=layer.outer.map(r=>new THREE.Vector2(r[2]*XY,r[3]*XY));
        const holes=layer.holes.map(ring=>ring.map(r=>new THREE.Vector2(r[2]*XY,r[3]*XY)));
        let tris;try{tris=THREE.ShapeUtils.triangulateShape(poly,holes);}catch{continue;}
        if(!tris.length)continue;
        for(const ceiling of [false,true]){
          const base=records[0],stat=base[ceiling?9:8];if(stat&1)continue;
          const tile=base[ceiling?7:6],pal=base[ceiling?13:12],shade=base[ceiling?11:10];
          const tex=this.tiles.get(tile,pal,shade),verts=all.map(r=>V(r[2],r[3],r[ceiling?5:4]));
          const uv=all.map(r=>flatUV(r[2],r[3],base,anchor,ceiling,tex?.w||64,tex?.h||64));
          add(id,tile,pal,shade,false,verts,ceiling?tris.map(t=>[...t].reverse()):tris,uv);
        }
      }
    }
    for(const r of walls){
      // Native wall record packs endpoints and corresponding sloped Z heights.
      const vertices=[V(r[2],r[3],r[4]),V(r[2],r[3],r[5]),V(r[6],r[7],r[9]),V(r[6],r[7],r[8])];
      const tile=r[10],pal=r[11],shade=r[12],masked=r[0]===4||r[0]===8||(r[13]&128)!==0;
      const dims=this.tiles.get(tile,pal,shade);
      const isOneWay=r[0]===8;
      const translucent=(r[13]&128)?((r[13]&512)?2:1):0;
      // One-way walls are owned by their source side, not mirrored into a
      // double-sided opaque wall that closes the connected passage.
      add(wallSector.get(r[1]|0)??-1,tile,pal,shade,masked,vertices,
        [[0,1,2],[0,2,3]],wallUV(r,dims?.w||64,dims?.h||64),
        isOneWay?r:null,translucent);
    }
    if(!surfaces){disposeTree(next);return false;}
    for(const g of groups.values()){
      const geo=new THREE.BufferGeometry();geo.setAttribute('position',new THREE.Float32BufferAttribute(g.pos,3));
      geo.setAttribute('uv',new THREE.Float32BufferAttribute(g.uv,2));geo.computeVertexNormals();geo.computeBoundingSphere();
      const mesh=new THREE.Mesh(geo,g.mat);mesh.frustumCulled=true;next.add(mesh);
      if(g.wall)oneWayGroups.push({mesh,record:g.wall,sign:oneWaySign.get(g.wall[1]|0)??1});
    }
    this.scene.add(next);if(this.world){this.scene.remove(this.world);disposeTree(this.world);}
    this.world=next;this.oneWayWalls=oneWayGroups;this.skyMesh=this.nextSky;this.worldCount=surfaces;this.triCount=triCount;this.drawCalls=groups.size;
    this.worldSignature=worldSig(data);this.lastWorldChange=performance.now();this.worldFingerprint=this.module()._DukeX_V44_WorldFingerprint()>>>0;return true;
  }
  updateFrame(data){
    const camera=data.subarray(0,STRIDE);if(camera[0]!==0)return;
    this.camera.position.copy(V(camera[2],camera[3],camera[4]));
    if(camera[8]>40&&camera[8]<150&&this.camera.fov!==camera[8]){this.camera.fov=camera[8];this.camera.updateProjectionMatrix();}
    const angle=camera[5]*Math.PI/1024;
    const pixelWidth=camera[10]>0?camera[10]:this.renderer.domElement.width;
    const pixelHeight=camera[11]>0?camera[11]:this.renderer.domElement.height;
    const pitch=buildHorizonPitch(camera[6],camera[8]||90,pixelWidth,pixelHeight);
    // World-record-based one-way walls are sector-facing masks; checking the
    // camera against each wall prevents them appearing from their back side.
    for(const w of this.oneWayWalls)w.mesh.visible=oneWayVisible(w.record,camera[2],camera[3],w.sign);
    const forward=new THREE.Vector3(Math.cos(angle)*Math.cos(pitch),Math.sin(pitch),Math.sin(angle)*Math.cos(pitch));
    this.camera.up.set(0,1,0);this.camera.lookAt(this.camera.position.clone().add(forward));
    if(camera[12])this.camera.rotateZ(camera[12]*Math.PI/1024);
    if(this.skyMesh)this.skyMesh.position.copy(this.camera.position);
    const visible=new Set();const wallSprites=new Set();let active=0;
    for(let i=STRIDE;i<data.length;i+=STRIDE){
      const r=data.subarray(i,i+STRIDE);if(r[0]!==3||active>=512)continue;
      const id=r[1],tile=r[5],pal=r[8],shade=r[9],cstat=r[10];
      if(tile===2567)continue; // LASERLINE is rendered from native 3D beam endpoints instead
      const tex=this.tiles.get(tile,pal,shade);if(!tex){this.diagnostics.missingTiles++;continue;}
      // Nonmoving wall decoration can be inserted twice by Build sector
      // portals; dedupe only true tile/position/orientation matches.
      if((cstat&16)&&r[14]>=0){const signature=[tile,Math.round(r[2]/16),Math.round(r[3]/16),Math.round(r[4]/256),r[11]|0].join(':');
        if(wallSprites.has(signature))continue;wallSprites.add(signature);}
      const mat=this.tiles.mat(tile,pal,shade,true,(cstat&2)?((cstat&512)?2:1):0);if(!mat)continue;
      let state=this.sprites.get(id);
      if(!state){const mesh=this.freeSprites.pop()||new THREE.Mesh(this.sharedSpriteGeo,mat);
        mesh.material=mat;this.scene.add(mesh);state={mesh,material:mat};this.sprites.set(id,state);}
      const mesh=state.mesh;mesh.visible=true;mesh.material=mat;
      const w=clamp(tex.w*r[6]/4096,0.02,32),h=clamp(tex.h*r[7]/4096,0.02,32);
      mesh.scale.set(w,h,1);mesh.position.copy(V(r[2],r[3],r[4]));
      const ang=r[11]*Math.PI/1024;
      // Build center flag, and x/y offsets are specified in tile-space pixels.
      if(!(cstat&128))mesh.position.y+=h/2;
      if(cstat&32){mesh.rotation.set(-Math.PI/2,Math.PI/2-ang,0,'YXZ');} // horizontal floor decal
      else if(cstat&16){mesh.rotation.set(0,Math.PI/2-ang,0,'YXZ');} // wall normal follows native angle
      else {
        // Build face sprites stay vertical and only rotate around the world up axis.
        // Camera-facing billboards rotate around WORLD UP, never with pitch.
        // The texture animation and mirroring were already resolved natively.
        const dx=this.camera.position.x-mesh.position.x,dz=this.camera.position.z-mesh.position.z;
        mesh.rotation.set(0,Math.atan2(dx,dz),0,'YXZ');
      }
      if(cstat&4)mesh.scale.x*=-1;if(cstat&8)mesh.scale.y*=-1;
      // Native xoffset/yoffset shift the tile registration, not the actor world origin.
      if(r[16]||r[17]){
        const halfWidth=mesh.scale.x/Math.max(1,tex.w),halfHeight=mesh.scale.y/Math.max(1,tex.h);
        mesh.translateX(-r[16]*halfWidth);mesh.translateY(r[17]*halfHeight);
      }
      visible.add(id);active++;

    }
    // Reuse stable objects, but never leave dead sprites visible.
    for(const [id,s] of this.sprites){if(!visible.has(id)){
      this.scene.remove(s.mesh);s.mesh.visible=false;
      if(this.freeSprites.length<256)this.freeSprites.push(s.mesh);this.sprites.delete(id);
    }}

    this.spriteCount=active;
  }
  updateBeams(){
    const m=this.module(),count=m?._DukeX_V42_BeamPrepare?.(),ptr=m?._DukeX_V42_BeamPtr?.()>>>0;
    let n=0;
    if(Number.isInteger(count)&&count>=0&&count<=112&&ptr>0&&!(ptr&3)&&
       m.HEAPF32&&(ptr>>>2)+count*10<=m.HEAPF32.length){
      const f=m.HEAPF32;
      for(let i=0;i<count;i++){
        const o=(ptr>>>2)+i*10;
        const a=V(f[o+1],f[o+2],f[o+3]),b=V(f[o+4],f[o+5],f[o+6]);
        const delta=b.clone().sub(a),length=delta.length();
        if(length<.001||!Number.isFinite(length))continue;
        for(let layer=0;layer<2;layer++){
          const mesh=this.beamMeshes[n++];mesh.visible=true;
          mesh.position.copy(a).add(b).multiplyScalar(.5);
          mesh.quaternion.setFromUnitVectors(this.beamAxis,delta.normalize());
          const width=(layer?.045:.012)*Math.min(2,1+f[o+7]*.08);
          mesh.scale.set(width,length,width);
        }
      }
    }
    for(;n<this.beamMeshes.length;n++)this.beamMeshes[n].visible=false;
  }
  updateHUD(){const m=this.module(),n=m?._DukeX_V41_HudPrepare?.();if(n!==12)return false;
    const offset=m._DukeX_V41_HudPtr?.()>>>0;if(!offset||offset+52>m.HEAPU8.length)return false;
    const h=m.HEAPF32.subarray(offset>>>2,(offset>>>2)+12);
    const set=(id,value)=>{const el=this.hud.querySelector('#'+id);if(el)el.textContent=value;};
    set('dx41hp',`HP ${Math.max(0,h[0]|0)}`);set('dx41armor',`ARMOR ${h[1]|0}`);
    set('dx41wave',`WAVE ${h[5]|0} · LV ${h[2]|0}`);
    this.hud.querySelector('#dx41xp').style.width=(100*clamp(h[4]>0?h[3]/h[4]:0,0,1))+'%';
    set('dx41stats',`${h[6]|0} KILLS · ${h[11]|0} ENEMIES`);
    return m.HEAPF32[(offset>>>2)+12]===1;
  }
  resize(){const r=this.nativeCanvas.getBoundingClientRect();if(r.width<4||r.height<4)return false;
    const ratio=clamp(this.pixelScale,.7,Math.min(window.devicePixelRatio||1,1.8));
    const key=`${Math.round(r.width)}x${Math.round(r.height)}@${ratio.toFixed(2)}`;
    this.root.style.left=r.left+'px';this.root.style.top=r.top+'px';
    this.root.style.width=r.width+'px';this.root.style.height=r.height+'px';
    this.hud.style.left=(r.left+12)+'px';this.hud.style.top=(r.top+10)+'px';
    if(this.lastSize!==key){this.lastSize=key;this.renderer.setPixelRatio(ratio);
      this.renderer.setSize(r.width,r.height,false);
      this.camera.aspect=r.width/r.height;this.camera.updateProjectionMatrix();}
    return true;
  }
  frame(time){if(!this.alive)return;requestAnimationFrame(this.frame);
    if(!this.active)return;
    try{
      if(!this.ready()||!this.resize()){this.deactivate('Native memory bridge unavailable');return;}
      const m=this.module(),data=read(m,'_DukeX_V41_FramePrepare','_DukeX_V41_FramePtr',2048,STRIDE);
      if(!data||data[0]!==0){if(time-this.lastFrame>2000)this.deactivate('No active native camera');return;}
      this.updateFrame(data);this.updateBeams();
      if(!this.updateHUD()){this.deactivate();return;} // menus belong to native backend
      // Structural world refresh is infrequent; per-frame sprites and camera are live.
      if(time-this.lastWorld>160){
        const revision=m._DukeX_V44_WorldFingerprint()>>>0;
        if(revision!==this.worldFingerprint){
          const world=read(m,'_DukeX_V41_WorldPrepare','_DukeX_V41_WorldPtr',ITEM_LIMIT,STRIDE);
          if(world)this.makeWorld(world);
        }
        this.lastWorld=time;
      }
      this.tiles.updateAnimations(time);
      this.renderer.render(this.scene,this.camera);
      this.frameCount++;
      if(this.lastFrame){const elapsed=time-this.lastFrame;this.fps=this.fps*.92+(1000/Math.max(1,elapsed))*.08;
        if(this.frameCount%120===0){
        }
        if(this.frameCount%120===0){if(this.fps<35)this.pixelScale=Math.max(.7,this.pixelScale*.88);
          else if(this.fps>57)this.pixelScale=Math.min(Math.min(window.devicePixelRatio||1,1.8),this.pixelScale*1.08);}}
      this.lastFrame=time;
    }catch(error){this.deactivate(error?.stack||String(error));}
  }
  destroy(){this.deactivate();this.alive=false;clearInterval(this.resumeTimer);
    for(const s of this.sprites.values())this.scene.remove(s.mesh);
    this.sprites.clear();this.freeSprites.length=0;this.sharedSpriteGeo.dispose();
    
    disposeTree(this.world);for(const t of this.skyTextures)t.dispose();this.skyTextures=[];this.tiles.clear();this.renderer.dispose();
    this.root.remove();this.hud.remove();this.status.remove();}
}
export async function installDukeXPrimary(){
  const canvas=document.querySelector('#canvas');if(!canvas)throw Error('Native canvas unavailable');
  const renderer=new DukeXPrimaryRenderer(canvas);window.DukeXRenderer=renderer;
  let stored=null;try{stored=localStorage.getItem('dukex-renderer');}catch{}
  renderer.wantsThree=preferThree(location.search,stored);renderer.updateToggle();
  {
    const started=performance.now();
    const boot=()=>{
      if(!renderer.alive||!renderer.wantsThree||renderer.active)return;
      if(renderer.ready()){renderer.activate();return;}
      if(performance.now()-started>9000){
        renderer.status.style.display='block';
        const m=renderer.module();
        renderer.status.textContent='THREE PRIMARY NOT READY: '+
          (!m?.HEAPF32?'Emscripten memory bridge missing':
           !m?._DukeX_V41_SetPrimary?'Native V41 renderer exports missing':'Waiting for game initialization');
      }
    };setTimeout(boot,500);renderer.resumeTimer=setInterval(boot,950);
  }
  document.addEventListener('visibilitychange',()=>{if(document.hidden)renderer.lastFrame=0;});
  return renderer;
}
