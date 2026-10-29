/* DUKE_X_V49_RESPONSIVE: responsive EDuke32 browser presentation and shared HUD.
 * This is a frontend-only compatibility layer. Never changes the SDL canvas
 * backing store or native framebuffer while a game is running: that would
 * reset WebGL/SDL contexts and can destroy an active run.
 */
(function () {
  'use strict';
  if (window.DukeXResponsiveV49) return;
  const canvas = document.getElementById('canvas');
  const root = document.getElementById('duke-x-controls');
  if (!canvas || !root) return;

  const STYLE_ID = 'dukex-v49-responsive-css';
  const css = `
    /* Field computer and upgrade choices must exist on desktop too. */
    #duke-x-controls { display:block !important; pointer-events:none; }
    #duke-x-controls #dx-field-sheet,#duke-x-controls #dx-level-mask { box-sizing:border-box; }
    #duke-x-controls #dx-field-launch { display:block; }
    /* Touch-only gameplay controls, NEVER the menus they surround. */
    body.dx-v49-desktop #dx23-move,body.dx-v49-desktop #dx23-aim,
    body.dx-v49-desktop #dx22-traverse,
    body.dx-v49-desktop #duke-x-controls #dx-move-zone,
    body.dx-v49-desktop #duke-x-controls #dx-aim-zone,
    body.dx-v49-desktop #duke-x-controls #dx-move-pad,
    body.dx-v49-desktop #duke-x-controls #dx-aim-pad,
    body.dx-v49-desktop #duke-x-controls #dx-field-tools {
      display:none !important;pointer-events:none !important;
    }
    /* Desktop field launcher stays accessible outside combat and clears
       the game's Settings, audio and fullscreen buttons. */
    body.dx-v49-desktop #duke-x-controls #dx-field-launch {
      display:block !important;position:fixed !important;
      left:auto !important;right:calc(160px + env(safe-area-inset-right)) !important;
      top:calc(12px + env(safe-area-inset-top)) !important;
      bottom:auto !important;transform:none !important;
      width:42px;height:40px;opacity:.94;z-index:1100;
    }
    body.dx-v49-desktop #duke-x-controls.dx-menu-open #dx-field-launch {
      z-index:1105;
    }
    /* The field computer / upgrade chooser are independent from touch pads.
       Allow scroll and clicking by mouse, touchscreen, or keyboard. */
    #duke-x-controls.dx-menu-open #dx-field-sheet,
    #duke-x-controls #dx-level-mask[style*="flex"] {pointer-events:auto !important;}
    #duke-x-controls #dx-field-sheet .dx-panel,
    #duke-x-controls #dx-level-card {box-sizing:border-box;max-width:calc(100vw - 24px);}
    #settings .panel {box-sizing:border-box;max-width:calc(100vw - 20px);max-height:calc(100dvh - 24px);}
    #canvas { display:block;max-width:none;max-height:none;outline:none; }
    @media (max-width:640px) {
      body.dx-v49-desktop #duke-x-controls #dx-field-launch {
        right:calc(155px + env(safe-area-inset-right)) !important;
      }
      #duke-x-controls #dx-cards {grid-template-columns:repeat(3,minmax(0,1fr));}
      #duke-x-controls #dx-field-sheet {padding:calc(10px + env(safe-area-inset-top)) 8px calc(10px + env(safe-area-inset-bottom));}
    }
    @media (max-width:370px) {
      #duke-x-controls #dx-cards{grid-template-columns:1fr;}
      #duke-x-controls .dx-upgrade{min-height:110px !important;}
    }
    @media (max-height:470px) {
      #settings .panel,#duke-x-controls .dx-panel,#duke-x-controls #dx-level-card {
        max-height:calc(100dvh - 12px);overflow-y:auto;
      }
    }
  `;
  let style = document.getElementById(STYLE_ID);
  if (!style) {
    style = document.createElement('style');style.id=STYLE_ID;style.textContent=css;
    document.head.appendChild(style);
  }

  const MOBILE_KEY='dukex.mobile.mode.v49';
  const modeSelect=document.getElementById('dukex-mobile-control-mode');
  const modeStatus=document.getElementById('dukex-mobile-control-status');
  let mobileMode='auto',touchOn=null;
  try {
    mobileMode=localStorage.getItem(MOBILE_KEY) || 'auto';
    if (!['auto','on','off'].includes(mobileMode)) mobileMode='auto';
  } catch (_) {}
  function phoneOrTablet() {
    const ua=navigator.userAgent||'';
    const uaData=navigator.userAgentData;
    if (uaData && uaData.mobile) return true;
    if (/Android|iPhone|iPad|iPod|Mobile|Tablet|Silk|Kindle/i.test(ua)) return true;
    // iPadOS can masquerade as macOS with a desktop user agent.
    if (navigator.platform==='MacIntel' && (navigator.maxTouchPoints||0)>1) return true;
    return false;
  }
  function updateInputMode() {
    const enabled=mobileMode==='on'||(mobileMode==='auto'&&phoneOrTablet());
    if (touchOn!==enabled) {
      if (!enabled) window.DX46_ResetTouchInput?.();
      touchOn=enabled;
    }
    root.classList.toggle('dx-v19-touch',enabled);
    document.body.classList.toggle('dx-v49-touch',enabled);
    document.body.classList.toggle('dx-v49-desktop',!enabled);
    if (modeSelect && modeSelect.value!==mobileMode) modeSelect.value=mobileMode;
    if (modeStatus) modeStatus.textContent=enabled?'Touch gameplay controls enabled':'Mouse and keyboard; menus remain enabled';
    return enabled;
  }
  if(modeSelect) modeSelect.addEventListener('change',()=>{
    mobileMode=['auto','on','off'].includes(modeSelect.value)?modeSelect.value:'auto';
    try {localStorage.setItem(MOBILE_KEY,mobileMode);} catch (_) {}
    updateInputMode();
  });
  updateInputMode();

  // Override the old one-shot CSS sizing. Its render cap (480p/720p) limits
  // internal raster quality and must not shrink the DISPLAY box on desktop.
  // Keep aspect-correct letterboxing, with optional Fill, but do not touch
  // canvas.width/height or use Module.setCanvasSize during active gameplay.
  const previousFit=window.fitCanvas;
  if (typeof previousFit==='function')window.removeEventListener('resize',previousFit);
  const last={w:0,h:0,nativeW:0,nativeH:0,viewportW:0,viewportH:0};
  let scheduled=false,applying=false;
  function viewport() {
    const v=window.visualViewport;
    // visualViewport responds to address-bar and virtual keyboard changes.
    const w=Math.max(2,Math.floor(Math.min(window.innerWidth||document.documentElement.clientWidth||640,v?.width||Infinity)));
    const h=Math.max(2,Math.floor(Math.min(window.innerHeight||document.documentElement.clientHeight||480,v?.height||Infinity)));
    return {w,h};
  }
  function fit() {
    if (applying)return;
    applying=true;
    try {
      const v=viewport();
      let fill=false;
      try {fill=localStorage.getItem('e32_display')==='fill';}catch(_){}
      const aspect=4/3;
      let w=v.w,h=v.h;
      if(!fill){if(w/h>aspect)w=Math.floor(h*aspect);else h=Math.floor(w/aspect);}
      // Never write canvas backing width/height: resize of the software SDL
      // buffer has to happen through the native video-mode API, not DOM props.
      const width=Math.max(2,w),height=Math.max(2,h);
      if(canvas.style.width!==width+'px')canvas.style.width=width+'px';
      if(canvas.style.height!==height+'px')canvas.style.height=height+'px';
      // An explicit transform origin avoids stale positions on zoom/rotate.
      if(canvas.style.maxWidth!=='none')canvas.style.maxWidth='none';
      if(canvas.style.maxHeight!=='none')canvas.style.maxHeight='none';
      last.w=width;last.h=height;last.nativeW=canvas.width;last.nativeH=canvas.height;
      last.viewportW=v.w;last.viewportH=v.h;
      // Three.js asks for current canvas rectangle every frame; proactively
      // request an update too, when an experimental backend exists.
      try{window.DukeXRenderer?.resize?.();}catch(_){}
    } finally {applying=false;}
  }
  function schedule() {
    if(scheduled)return;
    scheduled=true;
    requestAnimationFrame(()=>{scheduled=false;fit();});
  }
  window.fitCanvas=fit;
  // The old callback was removed above; all subsequent calls to the global
  // function, including Settings Display, now use this viewport manager.
  window.addEventListener('resize',schedule,{passive:true});
  window.addEventListener('orientationchange',schedule,{passive:true});
  document.addEventListener('fullscreenchange',schedule,{passive:true});
  window.visualViewport?.addEventListener('resize',schedule,{passive:true});
  window.visualViewport?.addEventListener('scroll',schedule,{passive:true});
  // Some SDL video-mode transitions change the backing dimensions or style
  // asynchronously without dispatching resize. Refit its CSS on the next RAF.
  const observer=new MutationObserver(mutations=>{
    if(applying)return;
    if(mutations.some(m=>['width','height','style'].includes(m.attributeName)))schedule();
  });
  observer.observe(canvas,{attributes:true,attributeFilter:['width','height','style']});
  const rootObserver=typeof ResizeObserver==='function'?new ResizeObserver(schedule):null;
  rootObserver?.observe(document.documentElement);
  schedule();

  // Desktop-native menus remain keyboard friendly. A single shortcut for
  // Field Computer (Alt+I) avoids stealing EDuke's function/gameplay keys.
  document.addEventListener('keydown',event=>{
    const target=event.target;
    if(event.defaultPrevented || target?.matches?.('input,textarea,select,[contenteditable]'))return;
    const settings=document.getElementById('settings');
    if(settings?.classList.contains('open'))return;
    if(event.altKey && event.code==='KeyI' && !event.ctrlKey && !event.metaKey){
      const launch=document.getElementById('dx-field-launch');
      launch?.click();event.preventDefault();event.stopPropagation();
    }
  },true);

  window.DukeXResponsiveV49={
    version:'49.0',
    fit,schedule,
    getStatus:()=>({...last,mode:mobileMode,touchEnabled:!!touchOn,
      isDesktop:!touchOn,backingPreserved:true}),
    setTouchMode:mode=>{
      if(!['auto','on','off'].includes(mode))return false;
      mobileMode=mode;try{localStorage.setItem(MOBILE_KEY,mode);}catch(_){}
      updateInputMode();return true;
    },
    updateInputMode
  };
  console.info('[Duke X V49] Shared desktop/mobile menus and safe responsive canvas enabled');
})();
