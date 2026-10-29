/* Duke X V47 original indexed ART loader. File is local and contains no Duke art. */
(function(g){'use strict';
 const name='tiles051.art';
 function preRun(Module){
  const dependency='dukex-v47-original-sprites';
  if(!Module || !Module.FS || !Module.addRunDependency || !Module.removeRunDependency){
    console.error('[Duke X V47] Emscripten FS/run dependency unavailable; cannot preload custom ART');
    return;
  }
  Module.addRunDependency(dependency);
  let settled=false;
  const complete=()=>{if(!settled){settled=true;Module.removeRunDependency(dependency)}};
  const deadline=setTimeout(()=>{console.warn('[Duke X V47] ART fetch timed out; continue using fallback sprites');complete()},8000);
  fetch('./'+name+'?v=47',{cache:'no-store'}).then(r=>{
   if(!r.ok)throw Error('ART unavailable (HTTP '+r.status+')');
   return r.arrayBuffer();
  }).then(buf=>{
   const data=new Uint8Array(buf),head=new DataView(buf);
   if(data.length<50000 || head.getInt32(0,true)!==1 ||
       head.getInt32(4,true)!==13 || head.getInt32(8,true)!==13056 ||
       head.getInt32(12,true)!==13068)
     throw Error('Incorrect V47 ART header');
   Module.FS.writeFile('/'+name,data);
   console.info('[Duke X V47] Original transparent chest/rat/selection art ready');
  }).catch(error=>{
    console.error('[Duke X V47] Missing custom art; native renderer will use fallback tiles',error);
  }).finally(()=>{clearTimeout(deadline);complete()});
 }
 g.DukeXV47Art={preRun};
})(window);
