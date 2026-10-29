// Duke X: public GitHub Pages build. Original copyrighted game data stays on the user's device.
(function () {
  'use strict';
  const css = document.createElement('style');
  css.textContent = `
    #dx-public-gate{position:fixed;inset:0;z-index:2147483000;display:flex;align-items:center;justify-content:center;
      overflow:auto;padding:24px;background:radial-gradient(ellipse at 50% 20%,#2b302e,#0d1015 72%);
      color:#f0e9d8;font:16px/1.55 system-ui,-apple-system,sans-serif}
    #dx-public-gate *{box-sizing:border-box}
    #dx-public-gate .panel{width:min(620px,100%);border:1px solid #ffffff30;background:#131720;
      box-shadow:0 22px 80px #000b;border-radius:18px;padding:clamp(20px,5vw,38px)}
    #dx-public-gate h1{font-size:clamp(26px,5vw,40px);margin:0 0 5px;letter-spacing:.01em}
    #dx-public-gate .eyebrow{color:#d7ac65;text-transform:uppercase;letter-spacing:.14em;font-size:12px;font-weight:750}
    #dx-public-gate p{color:#c9c7c1;margin:11px 0 16px}
    #dx-public-gate .button{display:block;width:100%;background:#e3ae58;color:#181410;border:0;border-radius:9px;
      padding:16px 20px;font:800 16px system-ui;cursor:pointer;text-align:center}
    #dx-public-gate .button:hover{background:#f9c878}
    #dx-public-gate .pill{display:inline-block;padding:5px 9px;border:1px solid #ffffff33;border-radius:100px;
      font-size:12px;color:#bbb;margin:5px 6px 6px 0}
    #dx-public-gate small{display:block;color:#a7a6a1;font-size:12px;margin-top:16px}
    #dx-public-gate a{color:#f2cb90}
    #dx-public-gate #dx-gate-status{color:#f3b2a5;min-height:24px;font-size:13px}
  `;
  document.head.appendChild(css);
  const overlay=document.createElement('div');overlay.id='dx-public-gate';
  overlay.innerHTML=`<div class="panel" role="dialog" aria-label="Load game data" aria-modal="true">
    <div class="eyebrow">Duke X · Browser edition</div>
    <h1>Your game. Your files.</h1>
    <p>This public preview runs the real EDuke32/WebAssembly game in your browser. To play, choose a
    <strong>DUKE3D.GRP</strong> file that you obtained legally. The file stays on your device; it is never uploaded to this website.</p>
    <div><span class="pill">Classic renderer by default</span><span class="pill">Three.js experimental in Settings</span><span class="pill">Mobile controls</span></div>
    <input id="dx-grp-file" type="file" aria-label="Choose Duke Nukem 3D GRP" style="display:none" />
    <button type="button" class="button" id="dx-choose-grp">Choose DUKE3D.GRP and Play</button>
    <div id="dx-gate-status" role="status" aria-live="polite"></div>
    <small>Requires legally obtained game data. Shareware users can get the original release from an authorized distributor.
    This fan project is not endorsed by the copyright holders. Multiplayer services are disabled in this public preview.
    <a href="./credits.html">Credits, data rights and source code</a>.</small>
  </div>`;
  document.body.appendChild(overlay);
  const fileInput=overlay.querySelector('#dx-grp-file');
  const button=overlay.querySelector('#dx-choose-grp');
  const status=overlay.querySelector('#dx-gate-status');
  button.addEventListener('click',()=>fileInput.click());
  fileInput.addEventListener('change',async()=>{
    const file=fileInput.files && fileInput.files[0];
    if (!file)return;
    button.disabled=true;
    try {
      status.textContent='Verifying local GRP file…';
      if (file.size<1000000 || file.size>250000000)throw new Error('Unexpected GRP size. Select a Duke Nukem 3D game data file.');
      const ab=await file.arrayBuffer();
      const bytes=new Uint8Array(ab);
      const sig='KenSilverman';
      for (let i=0;i<sig.length;i++)if(bytes[i]!==sig.charCodeAt(i))throw new Error('Invalid Build GRP signature; this is not a Duke 3D GRP.');
      // No retention, remote transfer, IndexedDB or cookies for the chosen bytes.
      window.__DUKEX_USER_GRP_BYTES=bytes;
      const mod=window.Module;
      if (!mod || !Array.isArray(mod.preRun))throw new Error('Engine boot configuration unavailable.');
      mod.preRun.unshift(function dukexInstallLocalGameData(){
        const data=window.__DUKEX_USER_GRP_BYTES;
        if (!data)throw new Error('Local GRP bytes unavailable.');
        // FS exists at preRun time; original Emscripten data preloader normally writes the exact same path.
        mod.FS.writeFile('/DUKE3D.GRP',data);
        window.__DUKEX_USER_GRP_BYTES=null;
      });
      const originalInit=mod.onRuntimeInitialized;
      mod.onRuntimeInitialized=function(){
        if (typeof originalInit==='function') originalInit.apply(this,arguments);
        overlay.remove();css.remove();
      };
      const oldAbort=mod.onAbort;
      mod.onAbort=function(reason){
        status.textContent='Engine could not start: '+String(reason||'unknown error');
        if (typeof oldAbort==='function')oldAbort.apply(this,arguments);
      };
      status.textContent='Starting WebAssembly engine…';
      const script=document.createElement('script');
      script.src='./eduke32.js?v=public-local-grp-1';
      script.async=false;
      script.onerror=()=>{status.textContent='Failed to load the WASM engine script.';button.disabled=false;};
      document.body.appendChild(script);
    } catch(e) {
      status.textContent=String(e && e.message||e);
      button.disabled=false;
      fileInput.value='';
    }
  });
})();
