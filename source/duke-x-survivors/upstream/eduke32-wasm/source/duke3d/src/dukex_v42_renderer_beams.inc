// V42: authoritative source-to-target native 3D beam endpoint for primary GPU renderer.
// No simulated combat in JavaScript. All coordinates come from clipped native rays.
#if defined(__EMSCRIPTEN__) && defined(MAXWALLS) && defined(MAXSECTORS)
namespace DukeXRender42 {
static float segments[112*10];
static int count=0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V42_BeamPrepare(){
 using namespace DukeXRender42;count=0;
 for(const auto &mine:DukeX::tripwires){
  if(mine.id<0||mine.id>=MAXSPRITES||sprite[mine.id].statnum!=STAT_FX||
     sprite[mine.id].picnum!=TRIPBOMB||mine.armDelay>0)continue;
  if(mine.originX==mine.endX&&mine.originY==mine.endY&&mine.originZ==mine.endZ)continue;
  float *v=&segments[count*10];
  v[0]=(float)mine.id;v[1]=(float)mine.originX;v[2]=(float)mine.originY;v[3]=(float)mine.originZ;
  v[4]=(float)mine.endX;v[5]=(float)mine.endY;v[6]=(float)mine.endZ;
  v[7]=(float)mine.rank;v[8]=2;v[9]=1;
  if(++count>=16)break;
 }
 // Non-tripmine beams include rails, chain lightning, frost tethers, and arcs.
 // The original LASERLINE sprites are deliberately NOT rendered by Three.js.
 for(const auto &fx:DukeX::nativeBeamFx){
  if(fx.expiry<=DukeX::gameTicks||fx.expiry>DukeX::gameTicks+24)continue;
  if(fx.x0==fx.x1&&fx.y0==fx.y1&&fx.z0==fx.z1)continue;
  float *v=&segments[count*10];
  v[0]=-1;v[1]=(float)fx.x0;v[2]=(float)fx.y0;v[3]=(float)fx.z0;
  v[4]=(float)fx.x1;v[5]=(float)fx.y1;v[6]=(float)fx.z1;
  v[7]=(float)fx.thick/8.f;v[8]=1;v[9]=(float)fx.pal;
  if(++count>=112)break;
 }
 return count;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V42_BeamPtr(){return (int)(uintptr_t)DukeXRender42::segments;}
extern "C" EMSCRIPTEN_KEEPALIVE const char *DukeX_V42Marker(){
 return "DXV42: SOURCE_ALIGNED_PITCHED_TRIPWIRE_FRIENDLY_THREE_NATIVE_PCG";
}
#endif
