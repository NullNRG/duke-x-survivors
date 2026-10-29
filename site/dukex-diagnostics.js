/* Duke X diagnostics for REAL EDuke32 WASM. Observes the engine; no second renderer.
   Self-contained, no remote telemetry, no game state modifications. */
(function (global) {
  'use strict';
  if (global.DukeXDiag) return;
  const startedAt = Date.now();
  const entries = [];
  const MAX_LOG = 160;
  const state = {runtime: false, status: 'initializing', deps: '?', wasm: 'unknown', data: 'unknown', net: 'unknown', grp: 'unknown', canvas: 'unknown', errors: 0};
  let root, body, summary, toggle, requestSeen = {};
  const clean = v => { try {return typeof v === 'string' ? v : v instanceof Error ? (v.stack || v.message) : JSON.stringify(v);} catch(e) {return String(v);} };
  function add(level, msg) {
    const time = ((Date.now()-startedAt)/1000).toFixed(1).padStart(6, ' ');
    const line = time+'s ['+level+'] '+String(msg).slice(0, 450);
    entries.push(line); if (entries.length > MAX_LOG) entries.shift();
    if (level === 'ERR') state.errors++;
    if (body) { body.textContent = entries.join('\n'); body.scrollTop=body.scrollHeight; }
    render();
  }
  function render() {
    if (!summary) return;
    summary.textContent = 'EDuke32: '+(state.runtime?'RUNTIME READY':'NOT READY')+' · errors '+state.errors+'\n'+
      'WASM '+state.wasm+' · data '+state.data+' · net '+state.net+' · GRP '+state.grp+'\n'+
      'Canvas '+state.canvas+' · deps '+state.deps+' · '+state.status;
  }
  const savedError = console.error.bind(console), savedWarn=console.warn.bind(console);
  console.error = function(...args){add('ERR',args.map(clean).join(' ')); savedError(...args);};
  console.warn = function(...args){add('WRN',args.map(clean).join(' ')); savedWarn(...args);};
  global.addEventListener('error', e=>{
    if (e.target && e.target !== global && (e.target.src || e.target.href))
      add('ERR', 'Resource load error: '+(e.target.src||e.target.href));
    else add('ERR','JS: '+(e.message||'unknown')+' at '+(e.filename||'?')+':'+e.lineno+':'+e.colno);
  },true);
  global.addEventListener('unhandledrejection', e=>add('ERR','Promise rejected: '+clean(e.reason)));
  function report() {
    const m=global.Module;
    const details={url:location.href, userAgent:navigator.userAgent, elapsedSeconds:(Date.now()-startedAt)/1000,
      state, module:{present:!!m, asm:!!(m&&m.asm), calledRun:!!(m&&m.calledRun), exports:!!(m&&m._main),
        heapBytes:m&&m.HEAPU8?m.HEAPU8.byteLength:0},
      canvas:{width:document.querySelector('canvas')?.width||0,height:document.querySelector('canvas')?.height||0},loader:document.getElementById('loader')?.className||'',engineMode:global.__e32menu||null,
      assetLoads:requestSeen, log:entries};
    return JSON.stringify(details,null,2);
  }
  function initUI() {
    if (root||!document.body) return;
    root=document.createElement('div');root.id='dukex-diag';
    root.style.cssText='position:fixed;left:6px;top:6px;z-index:2147483647;max-width:min(96vw,680px);color:#eaf5e2;font:12px/1.35 ui-monospace,monospace;pointer-events:auto;';
    root.innerHTML='<button id="dx-diag-toggle" type="button">DX DIAG</button><div id="dx-diag-panel" style="background:#0b1623f4;border:1px solid #fbce53;border-radius:7px;padding:8px;max-height:55vh;overflow:auto;"><div id="dx-diag-summary" style="white-space:pre-wrap;margin-bottom:7px;">Inspecting...</div><div style="display:flex;gap:7px;margin-bottom:6px"><button id="dx-diag-copy" type="button">Copy report</button><button id="dx-diag-save" type="button">Save report</button><button id="dx-diag-clear" type="button">Clear log</button></div><pre id="dx-diag-log" style="margin:0;white-space:pre-wrap;overflow:auto;max-height:28vh;word-break:break-word"></pre></div>';
    document.body.appendChild(root);
    summary=root.querySelector('#dx-diag-summary');body=root.querySelector('#dx-diag-log');toggle=root.querySelector('#dx-diag-toggle');
    root.querySelectorAll('button').forEach(b=>b.style.cssText='background:#303947;color:#fff;border:1px solid #7a7b7f;border-radius:5px;padding:7px;font:12px ui-monospace,monospace;touch-action:manipulation');
    toggle.addEventListener('click',()=>{const panel=root.querySelector('#dx-diag-panel');panel.hidden=!panel.hidden;});
    root.querySelector('#dx-diag-copy').addEventListener('click',async()=>{try{await navigator.clipboard.writeText(report());add('OK','Report copied to clipboard.');}catch(e){add('WRN','Clipboard not available: '+clean(e));}});
    root.querySelector('#dx-diag-save').addEventListener('click',()=>{
      const url=URL.createObjectURL(new Blob([report()],{type:'application/json'}));const a=document.createElement('a');a.href=url;a.download='dukex-runtime-diagnostics.json';a.click();setTimeout(()=>URL.revokeObjectURL(url),3000);
    });
    root.querySelector('#dx-diag-clear').addEventListener('click',()=>{entries.length=0;body.textContent='';render();});
    body.textContent=entries.join('\n');render();
    add('OK','Diagnostic overlay installed. This uses the original EDuke32 engine.');
  }
  if (document.readyState === 'loading')document.addEventListener('DOMContentLoaded',initUI);
  else initUI();
  function wrap(m,key,onCall) {
    if (!m || typeof m[key]!=='function') return;
    const old=m[key]; m[key]=function(...a){onCall(...a);return old.apply(this,a);};
  }
  function hookModule(){
    const m=global.Module;
    if (!m){add('ERR','window.Module missing before eduke32.js');return;}
    wrap(m,'setStatus',text=>{state.status=String(text||'');add('STS','Status: '+state.status);});
    wrap(m,'monitorRunDependencies',count=>{state.deps=count;add('DEP','Run dependencies remaining: '+count);});
    wrap(m,'printErr',str=>add('STDERR',str));
    wrap(m,'onRuntimeInitialized',()=>{state.runtime=true;add('OK','Emscripten onRuntimeInitialized fired');});
    wrap(m,'onAbort',reason=>add('ERR','WASM abort: '+clean(reason)));
    wrap(m,'onExit',code=>add('WRN','WASM process exit: '+code));
    add('OK','Module hooks attached; watching engine initialization.');
  }
  // Called after the page defines Module, immediately before eduke32.js.
  function inspect(){
    try{
      const m=global.Module;
      if(m){state.runtime=state.runtime||!!m.calledRun;
        try {let files=m.FS&&m.FS.readdir('/');state.grp=files?files.filter(x=>/\.grp$/i.test(x)).join(',')||'none in MEMFS':'FS not ready';}catch(e){state.grp='FS error: '+clean(e).slice(0,90);}
      }
      const c=document.getElementById('canvas');state.canvas=c?(c.width+'x'+c.height):'not found';
      const loader=document.getElementById('loader');if(loader && !state.runtime)state.status='loader='+loader.className+', status='+(document.getElementById('status')?.textContent||state.status);
      if(m && state.runtime && !m._DukeX_PendingUpgrades) state.status='runtime ready; DukeX exports unavailable (check native link)';
      if (performance.getEntriesByType) {
        for(const e of performance.getEntriesByType('resource')){
          const name=e.name.split('?')[0].split('/').pop();
          if(/^eduke32\.(js|wasm|data)$/.test(name)||name==='eduke32-net.js'||name==='engine-boot.js'){
            requestSeen[name]={bytes:e.transferSize??null, durationMs:Math.round(e.duration)};
            if(name==='eduke32.wasm')state.wasm='requested';
            if(name==='eduke32.data')state.data='requested';
            if(name==='eduke32-net.js')state.net='requested';
          }
        }
      }
    }catch(e){add('WRN','Inspector: '+clean(e));}
    render();
  }
  global.DukeXDiag={hookModule,add,report,inspect};
  setInterval(inspect,2000);
  setTimeout(()=>{if(!state.runtime)add('WRN','Still no runtime at 12s; inspect first error/DEP or missing GRP.');},12000);
  setTimeout(()=>{if(!state.runtime)add('WRN','Still no runtime at 30s. Export report with Save report.');},30000);
})(window);
