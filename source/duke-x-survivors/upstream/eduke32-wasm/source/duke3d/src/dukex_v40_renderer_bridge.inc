// Duke X V40: read-only snapshot endpoint for browser GPU renderers.
// This module never changes world state, damage, animation or network authority.
// SPDX-License-Identifier: GPL-2.0-or-later
// DXV40: THREE_SCENE_NATIVE_WORLD_CAMERA_SPRITES_RENDER_ENDPOINT
#if defined(__EMSCRIPTEN__) && defined(MAXWALLS) && defined(MAXSECTORS)
namespace DukeXRender40 {
static constexpr int kStride=12;
static constexpr int kMaxWorld=32768;
static constexpr int kMaxFrame=1024;
static float world[kStride*kMaxWorld];
static float frame[kStride*kMaxFrame];
static int worldCount=0,frameCount=0;
static bool emit(float *dst,int &count,int limit,int type,int id,
                 float a,float b,float c,float d,float e,float f,
                 float g,float h,float j,float k){
    if(count>=limit)return false;
    float *p=dst+kStride*count++;
    p[0]=(float)type;p[1]=(float)id;p[2]=a;p[3]=b;
    p[4]=c;p[5]=d;p[6]=e;p[7]=f;
    p[8]=g;p[9]=h;p[10]=j;p[11]=k;
    return true;
}
}
// No engine heap pointers escape: JS copies each bounded snapshot immediately.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V40_WorldPrepare(){
    using namespace DukeXRender40;
    worldCount=0;
    if(numsectors<=0 || numsectors>MAXSECTORS)return 0;
    for(int s=0;s<numsectors;s++){
        const int start=sector[s].wallptr, count=sector[s].wallnum;
        if(start<0 || count<3 || start+count>MAXWALLS)continue;
        for(int w=start;w<start+count;w++){
            auto const &p=wall[w];
            if(p.point2<0 || p.point2>=MAXWALLS)continue;
            auto const &q=wall[p.point2];
            const int floorA=getflorzofslope(s,p.x,p.y), floorB=getflorzofslope(s,q.x,q.y);
            const int ceilA=getceilzofslope(s,p.x,p.y),ceilB=getceilzofslope(s,q.x,q.y);
            if(!emit(world,worldCount,kMaxWorld,1,s,(float)p.x,(float)p.y,
               (float)floorA,(float)ceilA,(float)sector[s].floorpicnum,
               (float)sector[s].ceilingpicnum,(float)sector[s].floorstat,
               (float)sector[s].ceilingstat,(float)sector[s].floorshade,
               (float)sector[s].ceilingshade))break;
            if(p.nextsector<0 || p.nextsector>=numsectors){
                if(!emit(world,worldCount,kMaxWorld,2,w,(float)p.x,(float)p.y,
                   (float)floorA,(float)ceilA,(float)q.x,(float)q.y,
                   (float)floorB,(float)ceilB,(float)p.picnum,-1))break;
            }else{
                // A portal has OPEN air between sectors. Only emit the actual
                // upper and lower wall bands; never cover a door with a quad.
                const int neighbour=p.nextsector;
                const int nfA=getflorzofslope(neighbour,p.x,p.y);
                const int nfB=getflorzofslope(neighbour,q.x,q.y);
                const int ncA=getceilzofslope(neighbour,p.x,p.y);
                const int ncB=getceilzofslope(neighbour,q.x,q.y);
                if(ceilA<ncA || ceilB<ncB){
                    if(!emit(world,worldCount,kMaxWorld,2,w,(float)p.x,(float)p.y,
                       (float)ncA,(float)ceilA,(float)q.x,(float)q.y,
                       (float)ncB,(float)ceilB,(float)p.picnum,-1))break;
                }
                if(floorA>nfA || floorB>nfB){
                    if(!emit(world,worldCount,kMaxWorld,2,w,(float)p.x,(float)p.y,
                       (float)floorA,(float)nfA,(float)q.x,(float)q.y,
                       (float)floorB,(float)nfB,(float)p.picnum,-1))break;
                }
            }
        }
        if(worldCount+2>=kMaxWorld)break;
    }
    return worldCount;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V40_WorldPtr(){
    return (int)(uintptr_t)DukeXRender40::world;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V40_FramePrepare(){
    using namespace DukeXRender40;
    frameCount=0;
    if(numsectors<=0 || numplayers<=0 || myconnectindex<0 || myconnectindex>=MAXPLAYERS ||
       !g_player[myconnectindex].ps)return 0;
    auto const *p=g_player[myconnectindex].ps;
    emit(frame,frameCount,kMaxFrame,0,myconnectindex,
         (float)p->pos.x,(float)p->pos.y,(float)p->pos.z,
         (float)fix16_to_int(p->q16ang),(float)fix16_to_int(p->q16horiz),
         (float)p->cursectnum,0,0,0,0);
    // Keep the most relevant sprites and visual effects near the camera.
    for(int id=0;id<MAXSPRITES && frameCount<kMaxFrame;id++){
        auto const &sp=sprite[id];
        if(sp.statnum>=MAXSTATUS || sp.xrepeat==0 || sp.yrepeat==0 || (sp.cstat&32768))continue;
        if((int64_t)(sp.x-p->pos.x)*(sp.x-p->pos.x)+
           (int64_t)(sp.y-p->pos.y)*(sp.y-p->pos.y)>((int64_t)16384*16384))continue;
        emit(frame,frameCount,kMaxFrame,3,id,(float)sp.x,(float)sp.y,(float)sp.z,
             (float)sp.picnum,(float)sp.xrepeat,(float)sp.yrepeat,
             (float)sp.pal,(float)sp.shade,(float)sp.cstat,(float)sp.ang);
    }
    return frameCount;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V40_FramePtr(){
    return (int)(uintptr_t)DukeXRender40::frame;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V40_SnapshotStride(){return DukeXRender40::kStride;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V40Marker(){
    return "DXV40: THREE_SCENE_NATIVE_WORLD_CAMERA_SPRITES_RENDER_ENDPOINT";
}
#endif
