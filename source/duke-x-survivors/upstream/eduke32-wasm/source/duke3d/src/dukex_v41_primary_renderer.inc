// DXV41_NATIVE_THREE_PRIMARY_RENDERER_TILE_WORLD_FRAME_HUD
// GPL-2.0-or-later. Read-only renderer contracts except explicit backend selection.
// Included after the V40 snapshot functions by duke_x_survivors.inc in game.cpp.
#if defined(__EMSCRIPTEN__) && defined(MAXWALLS) && defined(MAXSECTORS)
// game.cpp includes duke_x_survivors.inc before this engine function's
// definition. The forward declaration keeps the Classic sprite pipeline
// accessible from the native Three.js snapshot endpoint.
#ifndef DXV43_RENDER_TEST_MOCK
void G_DoSpriteAnimations(int32_t,int32_t,int32_t,int32_t,int32_t);
#endif
// DXV45_1_VIEWPORT_GLOBALS_COMPILER_REPAIR
// DXV45: PORTAL_MASK_ONEWAY_PARALLAX_VIEWPORT_PARITY
// DXV44: CLASSIC_RGB_CAMERA_SPRITES_ANIMATION_WORLD_PARITY
// DXV43_2: BUILD_ANIMATED_TILE_PUBLIC_PICANM_CLOCK
// Source-compatible implementation of the Build engine's private tile-animation function
// (source/build/src/engine.cpp). This extension cannot directly call that private
// function from source/duke3d/src/game.cpp. Preserve oscillating/forward/reverse
// animation with the *same* picanm flags and totalclocklock time base.
static inline int DXV43AnimatedOffset(int const tile) {
  if ((unsigned)tile >= (unsigned)MAXTILES) return 0;
  int const count = (int)picanm[tile].num;
  if (count <= 0) return 0;
  int const tick = (int)totalclocklock >> (picanm[tile].sf & PICANM_ANIMSPEED_MASK);
  switch (picanm[tile].sf & PICANM_ANIMTYPE_MASK) {
    case PICANM_ANIMTYPE_OSC: {
      int const period = count << 1;
      int const position = tick % period;
      return position < count ? position : period - position;
    }
    case PICANM_ANIMTYPE_FWD: return tick % (count + 1);
    case PICANM_ANIMTYPE_BACK: return -(tick % (count + 1));
    default: return 0;
  }
}

namespace DukeXRender41 {
constexpr int stride=20, maxWorld=65536, maxFrame=2048, maxTile=512;
static float world[stride*maxWorld], frame[stride*maxFrame], hud[16];
static unsigned char rgba[maxTile*maxTile*4];
static int worldCount=0, frameCount=0, tileWidth=0, tileHeight=0;
static int enabled=0; // Browser owns primary scene ONLY after a verified initialized GPU backend.
static bool emit(float *buf,int &count,int limit,int type,int id,
                 float a=0,float b=0,float c=0,float d=0,float e=0,float f=0,
                 float g=0,float h=0,float i=0,float j=0,float k=0,float l=0,
                 float m=0,float n=0,float o=0,float p=0,float q=0,float r=0){
 if(count>=limit)return false;
 float *v=buf+stride*count++; v[0]=(float)type;v[1]=(float)id;
 v[2]=a;v[3]=b;v[4]=c;v[5]=d;v[6]=e;v[7]=f;v[8]=g;v[9]=h;
 v[10]=i;v[11]=j;v[12]=k;v[13]=l;v[14]=m;v[15]=n;
 v[16]=o;v[17]=p;v[18]=q;v[19]=r;return true;
}
static bool validWall(int i){return i>=0&&i<numwalls&&i<MAXWALLS;}
// Viewtype-5/7 CON actors have mirrored rear frames, not a single face-camera tile.
// Source-equivalent to G_DoSpriteAnimations: getofs_viewtype and viewtype_mirror.
static int viewOffset(int const rotations,int const angle){
  return (((((angle+3072)&2047)*rotations+1024)>>11)%rotations);
}
static int actionViewOffset(int viewtype,int spriteAngle,int viewAngle,int &cstat){
 const int n=viewtype<0?-viewtype:viewtype;
 const int a=viewtype<0?viewAngle-spriteAngle:spriteAngle-viewAngle;
 auto mirror=[&](int rotations,int raw){
  if(raw>rotations/2){cstat|=4;return rotations-raw;}
  cstat&=~4;return raw;
 };
 switch(n){
  case 2:return viewOffset(8,a)&1;
  case 3:case 4:return mirror(7,viewOffset(16,a)&7);
  case 5:return mirror(8,viewOffset(8,a));
  case 7:return mirror(12,viewOffset(12,a));
  case 8:cstat&=~4;return viewOffset(8,a);
  case 9:return mirror(16,viewOffset(16,a));
  case 12:cstat&=~4;return viewOffset(12,a);
  case 16:cstat&=~4;return viewOffset(16,a);
  default:return 0;
 }
}
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_SetPrimary(int on){
 DukeXRender41::enabled=(on!=0);return DukeXRender41::enabled;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_Primary(){return DukeXRender41::enabled;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_SnapshotStride(){return DukeXRender41::stride;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_WorldPtr(){return (int)(uintptr_t)DukeXRender41::world;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_FramePtr(){return (int)(uintptr_t)DukeXRender41::frame;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_HudPtr(){return (int)(uintptr_t)DukeXRender41::hud;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_TilePtr(){return (int)(uintptr_t)DukeXRender41::rgba;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_TileWidth(){return DukeXRender41::tileWidth;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_TileHeight(){return DukeXRender41::tileHeight;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_WorldPrepare(){
 using namespace DukeXRender41;worldCount=0;
 if(numsectors<=0||numsectors>MAXSECTORS)return 0;
 for(int s=0;s<numsectors;s++){
  const auto &sec=sector[s];int first=sec.wallptr,last=first+sec.wallnum;
  if(first<0||sec.wallnum<3||last>numwalls||last>MAXWALLS)continue;
  // Sector texture basis; record type 7 is metadata, never actual geometry.
  auto const &origin=wall[first];if(!validWall(origin.point2))continue;
  auto const &axis=wall[origin.point2];
  if(!emit(world,worldCount,maxWorld,7,s,origin.x,origin.y,axis.x-origin.x,axis.y-origin.y,
      sec.floorstat,sec.ceilingstat,sec.floorxpanning,sec.floorypanning,
      sec.ceilingxpanning,sec.ceilingypanning,
#ifndef DXV43_RENDER_TEST_MOCK
      sec.floorheinum,sec.ceilingheinum
#else
      0,0
#endif
      ))return worldCount;
  for(int w=first;w<last;w++){
   const auto &p=wall[w];if(!validWall(p.point2))continue;
   const auto &q=wall[p.point2];
   float fa=(float)getflorzofslope(s,p.x,p.y), fb=(float)getflorzofslope(s,q.x,q.y);
   float ca=(float)getceilzofslope(s,p.x,p.y), cb=(float)getceilzofslope(s,q.x,q.y);
   if(!emit(world,worldCount,maxWorld,1,s,p.x,p.y,fa,ca,sec.floorpicnum,
       sec.ceilingpicnum,
       sec.floorstat,sec.ceilingstat,sec.floorshade,sec.ceilingshade,
       sec.floorpal,sec.ceilingpal,sec.floorxpanning,sec.floorypanning,
       sec.ceilingxpanning,sec.ceilingypanning,w,p.point2))return worldCount;
   if(p.nextsector>=0&&p.nextsector<numsectors){
     const int n=p.nextsector;
     float nfa=(float)getflorzofslope(n,p.x,p.y),nfb=(float)getflorzofslope(n,q.x,q.y);
     float nca=(float)getceilzofslope(n,p.x,p.y),ncb=(float)getceilzofslope(n,q.x,q.y);
     // Upper band only where the adjacent ceiling hangs below our ceiling.
     const float upperA=nca>ca?nca:ca,upperB=ncb>cb?ncb:cb;
     if((!(sec.ceilingstat&1)||!(sector[n].ceilingstat&1)) && (upperA>ca||upperB>cb)){
       if(!emit(world,worldCount,maxWorld,5,w,p.x,p.y,ca,upperA,q.x,q.y,cb,upperB,
          p.picnum,p.pal,p.shade,p.cstat,p.xrepeat,p.yrepeat,p.xpanning,p.ypanning,p.overpicnum,p.nextsector))return worldCount;
     }
     const float lowerA=nfa<fa?nfa:fa,lowerB=nfb<fb?nfb:fb;
     if((!(sec.floorstat&1)||!(sector[n].floorstat&1)) && (lowerA<fa||lowerB<fb)){
       // Build lower-wall swap uses the facing wall's repeat/pan/tile attributes.
       const auto &lower=((p.cstat&2)&&validWall(p.nextwall))?wall[p.nextwall]:p;
       if(!emit(world,worldCount,maxWorld,6,w,p.x,p.y,lowerA,fa,q.x,q.y,lowerB,fb,
          lower.picnum,lower.pal,lower.shade,lower.cstat,lower.xrepeat,lower.yrepeat,
          lower.xpanning,lower.ypanning,lower.overpicnum,p.nextsector))return worldCount;
     }
     // The opening is the INTERSECTION of the two sectors, not the local
     // floor-to-ceiling slab. Otherwise masked fences cover walls and skies.
     const float openTopA=nca>ca?nca:ca,openTopB=ncb>cb?ncb:cb;
     const float openBotA=nfa<fa?nfa:fa,openBotB=nfb<fb?nfb:fb;
     if(openTopA<openBotA || openTopB<openBotB){
       // Clamp closed ends on crossing slopes to a zero-height edge.
       const float maskedBotA=openBotA<openTopA?openTopA:openBotA;
       const float maskedBotB=openBotB<openTopB?openTopB:openBotB;
       // CSTAT 32 is one-way: visible only from THIS wall's sector.
       // CSTAT 16 is a masked/translucent portal. Adjacent duplicates are
       // omitted unless the reverse side uses a different actual artwork.
       const bool oneway=(p.cstat&32)!=0;
       const bool masked=(p.cstat&16)!=0;
       bool duplicate=false;
       if(masked&&!oneway&&validWall(p.nextwall)){
         const auto &reverse=wall[p.nextwall];
         duplicate=(reverse.cstat&16)&&!(reverse.cstat&32)&&p.nextwall<w&&
           reverse.overpicnum==p.overpicnum&&reverse.pal==p.pal&&reverse.shade==p.shade;
       }
       if((oneway||masked)&&p.overpicnum>=0&&!duplicate){
         if(!emit(world,worldCount,maxWorld,oneway?8:4,w,p.x,p.y,openTopA,maskedBotA,
            q.x,q.y,openTopB,maskedBotB,p.overpicnum,p.pal,p.shade,p.cstat,
            p.xrepeat,p.yrepeat,p.xpanning,p.ypanning,p.overpicnum,p.nextsector))return worldCount;
       }
     }
   }else{
     if(!emit(world,worldCount,maxWorld,2,w,p.x,p.y,ca,fa,q.x,q.y,cb,fb,
        p.picnum,p.pal,p.shade,p.cstat,p.xrepeat,p.yrepeat,p.xpanning,p.ypanning,p.overpicnum,-1))return worldCount;
   }
  }
 }
 return worldCount;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_FramePrepare(){
 using namespace DukeXRender41;frameCount=0;
 if(numplayers<=0||myconnectindex<0||myconnectindex>=MAXPLAYERS||
    !g_player[myconnectindex].ps)return 0;
 auto const *p=g_player[myconnectindex].ps;
#ifndef DXV43_RENDER_TEST_MOCK
 // G_DrawRooms still computes this interpolated, clipped third-person camera
 // when GPU owns the world. Never substitute the player's feet/position.
 emit(frame,frameCount,maxFrame,0,myconnectindex,CAMERA(pos.x),CAMERA(pos.y),CAMERA(pos.z),
      fix16_to_int(CAMERA(q16ang)),fix16_to_int(CAMERA(q16horiz)),CAMERA(sect),ud.fov,p->over_shoulder_on,xdim,ydim,ud.screen_tilting?p->rotscrnang:0);
 const int viewX=CAMERA(pos.x),viewY=CAMERA(pos.y),viewAng=fix16_to_int(CAMERA(q16ang));
#else
 emit(frame,frameCount,maxFrame,0,myconnectindex,p->pos.x,p->pos.y,p->pos.z,
      fix16_to_int(p->q16ang),fix16_to_int(p->q16horiz),p->cursectnum);

#endif
#ifndef DXV43_RENDER_TEST_MOCK
 // DXV45: Use the EXACT Classic temporary-sprite transformation pass.
 // Raw sprite[] does not include Duke actor action frames, directional
 // rotations, sector/palette overrides, enemy effects or hidden-map props.
 // Native world drawing was disabled in GPU mode, so Classic no longer
 // populates this temporary list. Prepare it here without invoking drawrooms.
 if(enabled){
   spritesortcnt=0;
   const int camX=CAMERA(pos.x),camY=CAMERA(pos.y),camZ=CAMERA(pos.z);
   const int camAng=fix16_to_int(CAMERA(q16ang));
   // Walk Build's live sector sprite lists rather than scanning every
   // unused MAXSPRITES slot at 60-120 Hz. This is a renderer hot path.
   for(int sect=0;sect<numsectors&&spritesortcnt<maxFrame-128;sect++){
     int visits=0;
     for(int id=headspritesect[sect];id>=0&&id<MAXSPRITES&&
         visits++<MAXSPRITES&&spritesortcnt<maxFrame-128;id=nextspritesect[id]){
       auto const &sp=sprite[id];
       if(sp.statnum>=MAXSTATUS||sp.xrepeat==0||sp.yrepeat==0||
          (sp.cstat&32768)||(id==p->i&&!p->over_shoulder_on))continue;
       const int64_t dx=(int64_t)sp.x-camX,dy=(int64_t)sp.y-camY;
       if(dx*dx+dy*dy>(int64_t)49152*49152)continue;
       renderAddTSpriteFromSprite((uint16_t)id);
     }
   }
   // Same function used by G_DrawRooms, including CON and tile-specific
   // animation, masking and enemy decoration decisions.
   G_DoSpriteAnimations(camX,camY,camZ,camAng,65536);
   // Some actors legitimately generate extra tsprites (shadows, HUD effects).
   // A secondary instance gets its own renderer ID, never overwriting Duke.
   static uint32_t seen[MAXSPRITES]={};static uint32_t frameSerial=0;
   if(++frameSerial==0){Bmemset(seen,0,sizeof(seen));frameSerial=1;}
   for(int j=0;j<spritesortcnt && frameCount<maxFrame;j++){
     auto const &sp=tsprite[j];
     if(sp.owner<0||sp.owner>=MAXSPRITES||(unsigned)sp.picnum>=MAXTILES||
        sp.xrepeat==0||sp.yrepeat==0||(sp.cstat&32768))continue;
     const int tile=sp.picnum+DXV43AnimatedOffset(sp.picnum);
     if((unsigned)tile>=MAXTILES||tilesiz[tile].x<=0||tilesiz[tile].y<=0)continue;
     const int id=sp.owner;
     const int identity=(seen[id]==frameSerial)?MAXSPRITES+j:id;
     seen[id]=frameSerial;
     emit(frame,frameCount,maxFrame,3,identity,sp.x,sp.y,sp.z,tile,
          sp.xrepeat,sp.yrepeat,sp.pal,sp.shade,sp.cstat,sp.ang,
          sp.sectnum,sp.owner,sp.clipdist,sp.statnum,sp.xoffset,sp.yoffset);
   }
   return frameCount;
 }
#endif
 for(int id=0;id<MAXSPRITES && frameCount<maxFrame;id++){
  const auto &sp=sprite[id];
  if(sp.statnum>=MAXSTATUS||sp.xrepeat==0||sp.yrepeat==0||(sp.cstat&32768)||(id==p->i && !p->over_shoulder_on))continue;
  const int64_t dx=(int64_t)sp.x-p->pos.x,dy=(int64_t)sp.y-p->pos.y;
  if(dx*dx+dy*dy>(int64_t)24576*24576)continue;
  int displayedTile=(sp.picnum>=0&&sp.picnum<MAXTILES)
      ?sp.picnum+DXV43AnimatedOffset(sp.picnum):sp.picnum;
  int renderCstat=sp.cstat,renderPal=sp.pal,renderShade=sp.shade;
#ifndef DXV43_RENDER_TEST_MOCK
  if((unsigned)sp.sectnum<(unsigned)numsectors){
    // Same floor/ceiling shading source as G_DoSpriteAnimations for nonactors.
    if(!(sp.cstat&16) && !(sp.cstat&8192) && !G_TileHasActor(sp.picnum))
      renderShade=(sector[sp.sectnum].ceilingstat&1)?sector[sp.sectnum].ceilingshade:sector[sp.sectnum].floorshade;
    const int floorPal=sector[sp.sectnum].floorpal;
    if(floorPal>0 && floorPal<MAXPALOOKUPS && !g_noFloorPal[floorPal])renderPal=floorPal;
  }
  if(G_TileHasActor(sp.picnum)){
   const int action=AC_ACTION_ID(actor[id].t_data);
   if(action>=0 && (unsigned)action+ACTION_PARAM_COUNT<(unsigned)g_scriptSize &&
      apScript[action+ACTION_PARAM_COUNT]==CON_END){
     const int viewtype=apScript[action+ACTION_VIEWTYPE];
     const int frameIndex=AC_CURFRAME(actor[id].t_data);
     const int n=viewtype<0?-viewtype:viewtype;
     const int actionFlags=apScript[action+ACTION_FLAGS];
     const int viewAngle=((n>4 && n!=8)||(actionFlags&AF_VIEWPOINT))?
       getangle(sp.x-viewX,sp.y-viewY):viewAng;
     const int direction=actionViewOffset(viewtype,sp.ang,viewAngle,renderCstat);
     int candidate=sp.picnum+apScript[action+ACTION_STARTFRAME]+viewtype*frameIndex+direction;
     // Exactly as classic: walk backward to the last nonempty angle frame.
     if(viewtype>0)while(candidate>0 && candidate<MAXTILES && tilesiz[candidate].x==0)candidate-=n;
     if(candidate>=0&&candidate<MAXTILES&&tilesiz[candidate].x>0)displayedTile=candidate;
   }
  }
  // Classic third-person Duke receives the player's resolved color palette.
  if(id==p->i && p->over_shoulder_on)renderPal=p->palookup;
#endif
  emit(frame,frameCount,maxFrame,3,id,sp.x,sp.y,sp.z,displayedTile,sp.xrepeat,sp.yrepeat,
       renderPal,renderShade,renderCstat,sp.ang,sp.sectnum,sp.owner,sp.clipdist,sp.statnum,sp.xoffset,sp.yoffset);
 }
 return frameCount;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_HudPrepare(){
 using namespace DukeXRender41;
 for(float &v:hud)v=0;
 if(numplayers<=0||myconnectindex<0||myconnectindex>=MAXPLAYERS||!g_player[myconnectindex].ps)return 0;
 const auto *p=g_player[myconnectindex].ps;
 hud[0]=p->i>=0&&p->i<MAXSPRITES?sprite[p->i].extra:0;
 hud[1]=p->inv_amount[GET_SHIELD];
 if(DukeX::initialized && DukeX::Alive(myconnectindex)){
  const auto &f=DukeX::fighters[myconnectindex];
  hud[2]=f.level;hud[3]=f.xp;hud[4]=f.threshold;hud[5]=DukeX::wave;
  hud[6]=f.kills;hud[7]=f.unspent;hud[8]=DukeX::gameTicks;
 }
 hud[9]=p->gm;hud[10]=p->cursectnum;hud[11]=DukeX::activeEnemies;
 hud[12]=(p->gm & MODE_GAME)?1:0;
 return 12;
}
// Build ART pixels are COLUMN MAJOR; base palette is RGB with index 255 transparent.
// Only requested tiles enter the upload path. No GRP is copied to JS or a CDN.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V41_TilePrepare(int tile,int pal){
 using namespace DukeXRender41;tileWidth=tileHeight=0;
 if(tile<0||tile>=MAXTILES)return 0;
 int w=tilesiz[tile].x,h=tilesiz[tile].y;
 if(w<=0||h<=0||w>maxTile||h>maxTile)return 0;
 if(!waloff[tile]&&!tileLoad((int16_t)tile))return 0;
 if(!waloff[tile])return 0;
 if(pal<0||pal>=MAXPALOOKUPS||!palookup[pal])pal=0;
 auto const *colors=(unsigned char const *)(uintptr_t)waloff[tile];
 auto const *lookup=palookup[pal];
 for(int y=0;y<h;y++)for(int x=0;x<w;x++){
   unsigned char index=colors[x*h+y]; // Build original ART layout
   int out=(y*w+x)*4;
   if(index==255){rgba[out]=rgba[out+1]=rgba[out+2]=0;rgba[out+3]=0;continue;}
   int mapped=lookup?(unsigned char)lookup[index]:index;
   rgba[out]=palette[mapped*3];rgba[out+1]=palette[mapped*3+1];
   rgba[out+2]=palette[mapped*3+2];rgba[out+3]=255;
 }
 tileWidth=w;tileHeight=h;return w*h*4;
}
// V43: actual Build palookup shade rows; avoid multiplying a fake RGB brightness
// (which corrupts palette swaps, water palettes, enemies and fullbright tiles).
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V43_TilePrepare(int tile,int pal,int shade){
 using namespace DukeXRender41;tileWidth=tileHeight=0;
 if(tile<0||tile>=MAXTILES)return 0;
 const int w=tilesiz[tile].x,h=tilesiz[tile].y;
 if(w<=0||h<=0||w>maxTile||h>maxTile)return 0;
 if(!waloff[tile]&&!tileLoad((int16_t)tile))return 0;
 if(!waloff[tile])return 0;
 if(pal<0||pal>=MAXPALOOKUPS||!palookup[pal])pal=0;
 const int lookupShade=shade<0?0:shade>=numshades?numshades-1:shade;
 auto const *colors=(unsigned char const *)(uintptr_t)waloff[tile];
 const auto *lookup=(unsigned char const *)palookup[pal]+(lookupShade<<8);
 for(int y=0;y<h;y++)for(int x=0;x<w;x++){
  const unsigned char index=colors[x*h+y];const int o=(y*w+x)*4;
  if(index==255){rgba[o]=rgba[o+1]=rgba[o+2]=rgba[o+3]=0;continue;}
  const unsigned char mapped=lookup[index];
  rgba[o]=palette[mapped*3];rgba[o+1]=palette[mapped*3+1];
  rgba[o+2]=palette[mapped*3+2];rgba[o+3]=255;
 }
 tileWidth=w;tileHeight=h;return w*h*4;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char *DukeX_V43Marker(){
 return "DXV45_1_VIEWPORT_GLOBALS_COMPILER_REPAIR | DXV45: PORTAL_MASK_ONEWAY_PARALLAX_VIEWPORT_PARITY | DXV44: CLASSIC_RGB_CAMERA_SPRITES_ANIMATION_WORLD_PARITY | DXV43_2: BUILD_ANIMATED_TILE_PUBLIC_PICANM_CLOCK";
}
extern "C" EMSCRIPTEN_KEEPALIVE const char *DukeX_V41Marker(){
 return "DXV41: NATIVE_THREE_PRIMARY_RENDERER_TEXTURED_BUILD_SCENE_HUD";
}
#endif

#if defined(__EMSCRIPTEN__) && defined(MAXWALLS) && defined(MAXSECTORS)
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V44_TileAnimationFrames(int tile){
 return (unsigned)tile<MAXTILES ? picanm[tile].num : 0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V44_TileAnimationOffset(int tile){
 return DXV43AnimatedOffset(tile);
}
// Read-only structural fingerprint: replacing a 1 MB+ JS world snapshot poll.
// Animated ART tiles are handled separately by V44_TileAnimationOffset.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V44_WorldFingerprint(){
 uint32_t hash=2166136261u;
 auto mix=[&](uint32_t v){hash=(hash^v)*16777619u;};
 mix((uint32_t)numwalls);mix((uint32_t)numsectors);
 for(int i=0;i<numsectors;i++){
  const auto &s=sector[i];
#ifndef DXV43_RENDER_TEST_MOCK
  mix(s.wallptr);mix(s.wallnum);mix(s.floorz);mix(s.ceilingz);
  mix(s.floorheinum);mix(s.ceilingheinum);
  mix(s.floorpicnum);mix(s.ceilingpicnum);mix(s.floorstat);mix(s.ceilingstat);
  mix(s.floorshade);mix(s.ceilingshade);mix(s.floorpal);mix(s.ceilingpal);
#else
  mix(s.wallptr);mix(s.wallnum);mix(s.floorpicnum);mix(s.ceilingpicnum);
#endif
  mix(s.floorxpanning);mix(s.floorypanning);mix(s.ceilingxpanning);mix(s.ceilingypanning);
 }
 for(int i=0;i<numwalls;i++){
  const auto &w=wall[i];mix(w.x);mix(w.y);mix(w.point2);mix(w.nextsector);mix(w.nextwall);
  mix(w.picnum);mix(w.overpicnum);mix(w.cstat);mix(w.shade);mix(w.pal);
  mix(w.xrepeat);mix(w.yrepeat);mix(w.xpanning);mix(w.ypanning);
 }
 return (int)hash;
}
// Classic Build multi-tile parallax sky configuration, no guessed tile IDs.
// getpskyidx is defined in build.h and indexes the same psky_t table used by
// classic's parascan. Export the concrete tile mapping for the GPU panorama.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V45_SkyPanels(int baseTile){
#ifndef DXV43_RENDER_TEST_MOCK
 if((unsigned)baseTile>=MAXTILES||!multipsky)return 1;
 int const id=getpskyidx(baseTile);
 if(id<0||id>=pskynummultis)return 1;
 int const bits=multipsky[id].lognumtiles;
 return bits>=0&&bits<=4 ? (1<<bits) : 1;
#else
 (void)baseTile;return 1;
#endif
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V45_SkyPanelTile(int baseTile,int panel){
 if((unsigned)baseTile>=MAXTILES)return -1;
#ifndef DXV43_RENDER_TEST_MOCK
 if(!multipsky)return baseTile;
 int const id=getpskyidx(baseTile);
 if(id<0||id>=pskynummultis)return baseTile;
 int const bits=multipsky[id].lognumtiles;
 if(bits<0||bits>4)return baseTile;
 int const offset=(int)multipsky[id].tileofs[(unsigned)panel&((1u<<bits)-1)];
 int const result=baseTile+offset;
 return (unsigned)result<MAXTILES?result:-1;
#else
 (void)panel;return baseTile;
#endif
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V44_Version(){return 45;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V45_Version(){return 45;}
#endif
