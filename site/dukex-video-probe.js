/* DX VIDEO PROBE V10 - observes actual EDuke32/Emscripten canvas, never creates a renderer.
 * Install before eduke32.js to catch getContext and draw calls. Local reports only.
 */
(function(global){'use strict';
if(global.DukeXVideoProbe)return;
var start=Date.now(),counts={getContext:{},putImageData:0,drawImage:0,fillRect:0,drawArrays:0,drawElements:0,clear:0},errors=[],ctx2D=null,ctxGL=null,events=[],panel,summary;
function note(s){events.push(((Date.now()-start)/1000).toFixed(1)+'s '+s);if(events.length>80)events.shift();}
function wrap(obj,method,onCall){if(!obj||typeof obj[method]!=='function')return;
 var old=obj[method];try{obj[method]=function(){try{onCall.call(this,arguments);}catch(e){}return old.apply(this,arguments);};}catch(e){note('Cannot hook '+method+': '+e.message);}}
wrap(HTMLCanvasElement.prototype,'getContext',function(args){var t=String(args[0]);counts.getContext[t]=(counts.getContext[t]||0)+1;note('canvas '+(this.id||'?')+' getContext('+t+') '+this.width+'x'+this.height);});
if(global.CanvasRenderingContext2D){for(const method of ['putImageData','drawImage','fillRect'])wrap(CanvasRenderingContext2D.prototype,method,function(){counts[method]++;if(!ctx2D&&this.canvas&&this.canvas.id==='canvas')ctx2D=this;});}
for(const name of ['WebGLRenderingContext','WebGL2RenderingContext']){var ctor=global[name];if(!ctor)continue;for(const method of ['drawArrays','drawElements','clear'])wrap(ctor.prototype,method,function(){counts[method]++;if(!ctxGL&&this.canvas&&this.canvas.id==='canvas')ctxGL=this;});}
function native(m,n){try{var f=m&&m[n];return typeof f==='function'?f():null;}catch(e){return 'error:'+String(e).slice(0,80);}}
function info(){var c=document.getElementById('canvas'),m=global.Module,rect=c&&c.getBoundingClientRect(),cs=c&&global.getComputedStyle(c),all=Array.from(document.querySelectorAll('canvas')).map(x=>({id:x.id,width:x.width,height:x.height,cssWidth:x.style.width,cssHeight:x.style.height})),pixel=null;
 if(ctx2D&&c&&ctx2D.canvas===c){try{var p=ctx2D.getImageData(Math.floor(c.width/2),Math.floor(c.height/2),1,1).data;pixel=Array.from(p);}catch(e){pixel='unavailable: '+e.message;}}
 return {elapsedSeconds:Math.round((Date.now()-start)/100)/10, url:location.href, browser:navigator.userAgent, documentVisibility:document.visibilityState,
 video:{canvases:all,targetCanvas:c?{width:c.width,height:c.height,rect:rect?{width:rect.width,height:rect.height}:null,computedDisplay:cs&&cs.display,computedVisibility:cs&&cs.visibility}:null,context2DSeen:!!ctx2D,webGLSeen:!!ctxGL,pixelCenter:pixel,paintCalls:counts},
 engine:{module:!!m,runtimeInitialized:!!(global.DukeXDiag&&global.DukeXDiag.report&&JSON.parse(global.DukeXDiag.report()).state.runtime),
 exports:{main:!!(m&&m._main),DukeX_PendingUpgrades:!!(m&&m._DukeX_PendingUpgrades)},
 frameCount:native(m,'_DukeX_ProbeFrameCount'),width:native(m,'_DukeX_ProbeXres'),height:native(m,'_DukeX_ProbeYres'),bpp:native(m,'_DukeX_ProbeBpp'),renderMode:native(m,'_DukeX_ProbeRenderMode'),gameMode:native(m,'_DukeX_ProbeGameMode'),survivalTicks:native(m,'_DukeX_ProbeSurvivorTicks')}, events:events.slice()};}
function save(){var json=JSON.stringify(info(),null,2);var url=URL.createObjectURL(new Blob([json],{type:'application/json'})),a=document.createElement('a');a.href=url;a.download='dukex-video-diagnostic-v10.json';a.click();setTimeout(()=>URL.revokeObjectURL(url),5000);}
function make(){if(panel||!document.body)return;panel=document.createElement('section');panel.id='dx-video-probe';panel.style.cssText='position:fixed;bottom:4px;left:4px;z-index:2147483646;background:#101922ef;border:1px solid #ffaa55;border-radius:8px;color:#f7f2e6;padding:7px;max-width:96vw;font:12px monospace;pointer-events:auto';
 panel.innerHTML='<div style="font-weight:bold;color:#ffbf4b">DX VIDEO V10</div><pre id="dx-video-summary" style="white-space:pre-wrap;margin:4px 0"></pre><button id="dx-video-copy">Copy video report</button> <button id="dx-video-save">Save report</button>';document.body.appendChild(panel);summary=panel.querySelector('#dx-video-summary');panel.querySelectorAll('button').forEach(b=>b.style.cssText='padding:5px;color:white;background:#35465b;border:1px solid #8a8a92;border-radius:3px');
 panel.querySelector('#dx-video-copy').onclick=()=>{var s=JSON.stringify(info(),null,2);navigator.clipboard?.writeText(s).then(()=>note('Copied report'),()=>note('Clipboard unavailable'));};panel.querySelector('#dx-video-save').onclick=save;update();}
function update(){if(!summary)return;var v=info(),e=v.engine,p=v.video.paintCalls;summary.textContent='canvas: '+(v.video.targetCanvas?v.video.targetCanvas.width+'x'+v.video.targetCanvas.height:'MISSING')+' · contexts: '+JSON.stringify(p.getContext)+'\n2D presents '+p.putImageData+' / '+p.drawImage+' · WebGL draws '+(p.drawArrays+p.drawElements)+'\nEDuke: frames '+(e.frameCount??'?')+' · mode '+(e.width??'?')+'x'+(e.height??'?')+'x'+(e.bpp??'?')+' · gm '+(e.gameMode??'?')+'\nNative probe '+(e.frameCount===null?'not yet built':'available')+' · runtime '+(e.runtimeInitialized?'ready':'waiting');}
if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',make);else make();
var timer=setInterval(update,1500);note('DX V10 attached before engine load; passive presentation counters active.');global.DukeXVideoProbe={info,save,update};
})(window);
