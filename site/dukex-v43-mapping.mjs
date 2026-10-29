// DXV43 renderer mapping math, shared by browser renderer and dependency-free tests.
export const clamp=(n,lo,hi)=>Math.max(lo,Math.min(hi,n));
export const worldSig=(data)=>{let h=2166136261;for(let i=0;i<data.length;i++){
  const v=Math.round(data[i]*16)|0;h=Math.imul(h^(v&255),16777619);h=Math.imul(h^((v>>>8)&255),16777619);
}return (h>>>0).toString(16)+':'+data.length;};
export function flatUV(x,y,record,anchor,ceiling,texWidth=64,texHeight=texWidth){
  const stat=record[ceiling?9:8]|0;
  const px=record[ceiling?16:14],py=record[ceiling?17:15];
  let dx=x,dy=y;
  if((stat&64)&&anchor){
    dx-=anchor[2];dy-=anchor[3];const ax=anchor[4],ay=anchor[5];const len=Math.hypot(ax,ay)||1;
    const ux=ax/len,uy=ay/len;
    const along=dx*ux+dy*uy,across=dy*ux-dx*uy;
    // Build's slope-relative alignment adjusts the perpendicular texture axis
    // by sqrt(heinum² + 4096²)/4096, not by horizontal wall length alone.
    const heinum=anchor[ceiling?13:12]||0;
    dx=along;dy=across*Math.hypot(4096,heinum)/4096;
  }
  if(stat&4)[dx,dy]=[dy,dx];
  if(stat&16)dx=-dx;if(stat&32)dy=-dy;
  const zoom=(stat&8)?2:1;
  return [dx/(Math.max(8,texWidth)*16*zoom)+px/256,
          dy/(Math.max(8,texHeight)*16*zoom)+py/256];
}
// Build wall texture coordinates reference each ENDPOINT's own sector heights,
// including slopes. One shared min/max height distorts the texture diagonally.
// Engine prepwall() scales wall U to wall.xrepeat<<3 PIXELS across the
// entire segment. U repeat is independent of geometric wall length; the
// previous span*xrepeat/(width*128) squashed short segments and stretched
// long ones. Wall Y uses the native z-coordinate/yrepeat ratio.
export function wallUV(r,texWidth=64,texHeight=texWidth){
  const cstat=r[13]|0;
  const xrep=Math.max(1,r[14]),yrep=Math.max(1,r[15]);
  const uLen=(xrep*8)/Math.max(1,texWidth);
  const vScale=yrep/(2048*Math.max(1,texHeight));
  const vA=z=>((cstat&4)?z-r[5]:z-r[4])*vScale+r[17]/256;
  const vB=z=>((cstat&4)?z-r[9]:z-r[8])*vScale+r[17]/256;
  let u0=r[16]/256,u1=u0+uLen;
  if(cstat&8)[u0,u1]=[u1,u0];
  let v0=vA(r[4]),v1=vA(r[5]),v2=vB(r[9]),v3=vB(r[8]);
  if(cstat&256)[v0,v1,v2,v3]=[-v0,-v1,-v2,-v3];
  return [[u0,v0],[u0,v1],[u1,v2],[u1,v3]];
}
// The Build horizon is 100 at screen center. engine.cpp projects it using
// viewport width, vertical height and horizontal viewing range; the old
// atan((100-horizon)/128) ignored both FOV and aspect ratio.
export function buildHorizonPitch(horizon,fov=90,width=640,height=400){
  const f=Number.isFinite(fov)&&fov>35&&fov<170?fov:90;
  const w=Math.max(1,width),h=Math.max(1,height);
  const focal=w/(2*Math.tan(f*Math.PI/360));
  // Build's default 100 horizon maps to zero pitch regardless of viewport.
  const pixelShift=(100-horizon)*h/200;
  return Math.atan2(pixelShift,focal);
}
// Only the originating sector may see a one-way portal wall.
export function oneWayVisible(r,cameraX,cameraY,interiorSign=1){
  const cross=(r[6]-r[2])*(cameraY-r[3])-(r[7]-r[3])*(cameraX-r[2]);
  // The owning sector is inside the loop (left for CCW, right for CW).
  // Holes reverse their winding. Do not assume one global wall normal.
  return cross*interiorSign>=0;
}
export function spriteOrientation(cstat,angle,cameraAngle){
  if(cstat&32)return 'floor';if(cstat&16)return 'wall';return 'face';
}
export function preferThree(search,stored){
  // The URL does not opt visitors into experimental graphics. Settings only.
  return stored === 'three';
}

// The Build sector wall list can hold disjoint contours and holes. A single
// triangulated polygon incorrectly filled holes and produced invisible walls.
export function sectorRings(records){
  if(!records?.length)return [];
  const byId=new Map(records.map(r=>[r[18]|0,r])),visited=new Set(),rings=[];
  if(byId.size!==records.length)return [records]; // old native snapshot compatibility
  for(const record of records){
    const first=record[18]|0;if(visited.has(first))continue;
    let id=first,ring=[],complete=false;
    for(let step=0;step<records.length;step++){
      if(visited.has(id)){complete=id===first;break;}
      const r=byId.get(id);if(!r)break;
      visited.add(id);ring.push(r);id=r[19]|0;
      if(id===first){complete=true;break;}
    }
    if(complete&&ring.length>=3)rings.push(ring);
  }
  return rings.length?rings:[records];
}
function area(ring){let a=0;for(let i=0;i<ring.length;i++){
  const p=ring[i],q=ring[(i+1)%ring.length];a+=p[2]*q[3]-q[2]*p[3];
}return a/2;}
function contains(ring,x,y){let inside=false;
  for(let i=0,j=ring.length-1;i<ring.length;j=i++){
    const p=ring[i],q=ring[j];
    if(((p[3]>y)!==(q[3]>y))&&(x<(q[2]-p[2])*(y-p[3])/(q[3]-p[3])+p[2]))inside=!inside;
  }
  return inside;
}
export function ringLayers(rings){
  const nodes=rings.filter(r=>r.length>=3&&Math.abs(area(r))>1e-8)
    .map(r=>({ring:r,area:Math.abs(area(r)),parent:null,depth:0}));
  for(const node of nodes){
    const p=node.ring[0];
    for(const other of nodes){if(other===node||other.area<=node.area)continue;
      if(contains(other.ring,p[2],p[3])&&(!node.parent||other.area<node.parent.area))node.parent=other;
    }
  }
  for(const node of nodes){let p=node.parent;while(p){node.depth++;p=p.parent;}}
  return nodes.filter(n=>n.depth%2===0).map(n=>({outer:n.ring,
    holes:nodes.filter(h=>h.parent===n && h.depth%2===1).map(h=>h.ring)}));
}
