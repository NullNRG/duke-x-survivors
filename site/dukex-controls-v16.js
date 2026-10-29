// DUKE_X_MOBILE_V32 — native enemy HP/status telemetry; V27 touch retained unchanged.
// DUKE_X_MOBILE_V31 — GRP-aware 70-skill native weapon simulator and diagnostics.
// DUKE_X_MOBILE_V28: touch bridge retained; live native power draws and no choice flash.
/* DUKE_X_MOBILE_V27 — staged-page-independent visible MOVE + independent LOOK native controls. */
(function(){'use strict';
// DXV21: the V20 installer changed a script URL but could leave NO mobile HTML.
// Recover the entire native UI from this self-contained script on every page.
 // Does not touch the Emscripten Module, renderer, GRP, WASM or original menu.
// DXV32.2: remove manual blast cheat, keep native auto firing.
const dxV21Required=["dx-move-pad", "dx-move-zone", "dx-aim-pad", "dx-aim-zone", "dx-field-launch", "dx-field-sheet", "dx-field-close", "dx-level-mask", "dx-level-card", "dx-cards", "dx-level-sub", "dx-reroll", "dx-banish-toggle", "dx-input-monitor", "dx-respawn"];
function dxV21EnsureShell(){
 if(!document.body)throw Error('DXV21: control script loaded before document.body');
 if(!document.getElementById('dx-v21-ui-style')){
  const style=document.createElement('style');style.id='dx-v21-ui-style';
  style.textContent="#duke-x-controls{display:none;position:fixed;inset:0;z-index:1050;pointer-events:none;color:#f2dfb8;font:700 13px system-ui,sans-serif;user-select:none;-webkit-user-select:none;touch-action:pan-x pan-y}\n#duke-x-controls *{box-sizing:border-box;-webkit-user-select:none;user-select:none}\n#duke-x-controls button{cursor:pointer;touch-action:manipulation;pointer-events:auto;color:#f4e8c7;font:800 12px system-ui,sans-serif;letter-spacing:.5px;border:1px solid #9e753e;background:linear-gradient(165deg,#48372d,#191b20);border-radius:7px;box-shadow:inset 0 1px #fff2,0 3px 8px #0009}\n#duke-x-controls button:active{filter:brightness(1.35)}\n#dx-move-zone,#dx-aim-zone{display:none;position:absolute;bottom:0;height:57%;width:43%;pointer-events:auto;touch-action:none}\n#dx-move-zone{left:0}#dx-aim-zone{right:0;bottom:calc(158px + env(safe-area-inset-bottom));height:42%;}\n#duke-x-controls.dx-in-game:not(.dx-overlay-open) #dx-move-zone,#duke-x-controls.dx-in-game:not(.dx-overlay-open) #dx-aim-zone{display:block}\n.dx-pad{display:none;position:absolute;width:116px;height:116px;left:90px;top:calc(100% - 107px - env(safe-area-inset-bottom));margin:-58px 0 0 -58px;border-radius:100%;border:2px solid #ebb454a6;background:radial-gradient(circle,#ebae2e22,#0d1b2c88);box-shadow:inset 0 0 18px #000b,0 0 14px #0009;transform:none;pointer-events:none;opacity:.40;transition:opacity .10s}\n#dx-aim-pad{left:auto;right:calc(35px + env(safe-area-inset-right));top:calc(100% - 245px - env(safe-area-inset-bottom));margin:-58px 0 0 0}\n#duke-x-controls.dx-in-game:not(.dx-overlay-open):not(.dx-player-dead) .dx-pad{display:block}\n#duke-x-controls.dx-player-dead #dx-move-zone,#duke-x-controls.dx-player-dead #dx-aim-zone,#duke-x-controls.dx-player-dead #dx-field-tools{display:none!important}\n#dx-respawn{display:none!important;position:absolute;bottom:calc(70px + env(safe-area-inset-bottom));left:50%;transform:translateX(-50%);width:min(300px,80vw);height:60px;border-color:#ffd378!important;background:linear-gradient(#9e431c,#451919)!important;font-size:19px!important;z-index:1052}\n#duke-x-controls.dx-player-dead:not(.dx-overlay-open) #dx-respawn{display:block!important}\n\n.dx-pad.active{opacity:1}.dx-pad .knob{position:absolute;left:38px;top:38px;width:40px;height:40px;border-radius:50%;background:radial-gradient(circle at 32% 20%,#ffe4a2,#a85c26 75%,#3d2019);box-shadow:0 2px 9px #000b}\n#dx-aim-pad{border-color:#cb5233bb}#dx-aim-pad .knob{background:radial-gradient(circle at 35% 25%,#ffd0b0,#ac3b27)}\n#dx-field-tools{display:none;position:absolute;right:8px;bottom:calc(13px + env(safe-area-inset-bottom));width:120px;gap:5px;grid-template-columns:repeat(2,1fr)}\n#duke-x-controls.dx-in-game:not(.dx-overlay-open) #dx-field-tools{display:grid}\n#dx-field-tools button{height:41px;min-width:0;font-size:10px}\n#dx-field-tools #dx-blast{grid-column:span 2;background:linear-gradient(#ad4b25,#5b1d18);border-color:#eebd63;font-size:14px;height:48px}\n#dx-field-launch{pointer-events:auto;position:absolute;right:calc(8px + env(safe-area-inset-right));top:calc(8px + env(safe-area-inset-top));width:42px;height:37px;font-size:21px!important;z-index:1060}\n#duke-x-controls:not(.dx-in-game) #dx-field-launch{top:auto;bottom:calc(12px + env(safe-area-inset-bottom));width:46px;height:46px;transform:none}\n#dx-field-sheet{display:none;pointer-events:auto;position:absolute;inset:0;background:#070a11de;padding:calc(15px + env(safe-area-inset-top)) 12px calc(15px + env(safe-area-inset-bottom));overflow-y:auto;touch-action:pan-y;z-index:1058}\n#duke-x-controls.dx-menu-open #dx-field-sheet{display:flex;align-items:center;justify-content:center}\n.dx-panel{width:min(650px,100%);max-height:calc(100dvh - 34px);overflow:auto;background:linear-gradient(150deg,#372c25,#13191f 40%,#261d1b);border:2px solid #97733e;box-shadow:0 0 0 4px #08080e,0 12px 44px #000e;border-radius:4px;touch-action:pan-y;padding:12px}\n.dx-header{display:flex;align-items:center;justify-content:space-between;gap:8px;font:900 19px Georgia,serif;color:#ffc968;letter-spacing:1px;text-shadow:0 2px #411d13}\n.dx-header button{min-width:38px;min-height:34px}\n.dx-sub{color:#bcae97;font:600 11px system-ui;margin:6px 0 12px;line-height:1.5}\n.dx-tabs{display:flex;gap:5px;flex-wrap:wrap;margin:9px 0}.dx-tabs button{padding:9px 10px;flex:1}.dx-tabs button[aria-selected=\"true\"]{color:#211a15;background:linear-gradient(#ecca76,#a76a31)}\n.dx-menu-section{display:none}.dx-menu-section.active{display:block}\n.dx-menu-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(135px,1fr));gap:6px}.dx-inventory-item{border:1px solid #68533b;border-radius:5px;padding:9px;background:#0a0f17bb}.dx-inventory-item b{display:block;font-size:11px}.dx-inventory-item small{font-weight:500;color:#d5bc85}\n.dx-profile{display:block;width:100%;text-align:left;padding:10px;margin:5px 0}.dx-profile.selected{border-color:#ffd379!important;box-shadow:0 0 0 1px #f0b63f}\n.dx-actionline{display:flex;gap:6px;flex-wrap:wrap;margin:8px 0}.dx-actionline button{padding:10px 13px}\n#dx-input-monitor{white-space:pre-line;font:600 11px ui-monospace,monospace;line-height:1.65;border:1px solid #87683c;padding:9px;border-radius:5px;overflow-wrap:anywhere;background:#111827}.dx-hint{font-size:11px;color:#aaa89b;font-weight:500;line-height:1.5}\n#dx-level-mask{display:none;position:absolute;inset:0;z-index:1100;pointer-events:auto;align-items:center;justify-content:center;background:radial-gradient(ellipse at center,#1b1e25ee,#04050afb);padding:calc(25px + env(safe-area-inset-top)) 10px calc(14px + env(safe-area-inset-bottom));overflow:auto;touch-action:pan-y}\n#dx-level-card{width:min(700px,100%);max-height:95dvh;overflow-y:auto;border:3px ridge #ad843b;background:linear-gradient(165deg,#402921,#101723 55%,#2a2319);box-shadow:0 0 0 4px #0c0807,0 18px 55px #000f;padding:12px;touch-action:pan-y}\n#dx-level-title{text-align:center;font:900 clamp(21px,4.3vw,31px) Georgia,serif;letter-spacing:2px;color:#ffd178;text-shadow:0 2px #522316}\n#dx-level-sub{text-align:center;color:#e8d6b7;margin:4px 0 10px;font-size:11px}\n#dx-cards{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:7px}\n#duke-x-controls .dx-upgrade{display:flex;flex-direction:column;align-items:center;padding:10px 6px;min-height:168px;justify-content:space-between;border:1px solid #a17d48;background:linear-gradient(150deg,#5c4430,#151b28 56%,#2a2520);border-radius:4px;min-width:0}\n.dx-icon{display:flex;justify-content:center;align-items:center;width:55px;height:55px;border:1px solid #8f6f48;border-radius:5px;background:#0008}\n.dx-icon img{max-width:49px;max-height:49px;object-fit:contain;image-rendering:pixelated;filter:drop-shadow(0 0 4px #f3af3770)}\n.dx-symbol{font:900 28px system-ui;color:#eab952}.dx-type{font-size:9px;color:#9acde5;margin-top:5px}\n.dx-power{font-size:clamp(10px,2.3vw,14px);color:#ffda8d;text-align:center;margin:6px 0 2px;overflow-wrap:anywhere}\n.dx-desc{font-size:10px;font-weight:500;color:#cfc0af;text-align:center;line-height:1.3;min-height:24px}\n.dx-tier{font-size:10px;color:#e9bf73;margin-top:7px}.dx-evo{font-size:10px;color:#ff8f63}\n#dx-level-actions{display:flex;align-items:center;justify-content:center;gap:8px;margin-top:10px}#dx-level-actions button{padding:9px 12px}\n#dx-level-foot{text-align:center;font-size:10px;color:#b6a48c;margin-top:8px}\nbody:not(.dx-show-debug-log) #dukex-diag,body:not(.dx-show-debug-video) #dx-video-probe{display:none!important}\nbody.dx-show-debug-log #dukex-diag{z-index:1030!important;left:6px!important;top:52px!important;max-height:68dvh!important;overflow:auto}\nbody.dx-show-debug-video #dx-video-probe{z-index:1030!important;bottom:calc(63px + env(safe-area-inset-bottom))!important;max-height:65dvh!important;overflow:auto}\n@media(max-width:420px){#dx-cards{gap:4px}#duke-x-controls .dx-upgrade{padding:7px 3px;min-height:156px}.dx-icon{width:43px;height:43px}.dx-icon img{max-width:40px;max-height:40px}#dx-field-tools{width:106px}}\n/* DXV19: Android desktop-site, stylus, foldables may not match pointer:coarse. */\n#duke-x-controls.dx-v19-touch{display:block!important}\n#duke-x-controls.dx-v19-touch:not(.dx-in-game) #dx-move-pad{display:none}\n/* MOVE stays anchored on screen and receives a real visible, labeled pad. */\n#duke-x-controls #dx-move-zone{left:0;bottom:0;width:min(48vw,360px);height:48dvh;z-index:1051}\n#duke-x-controls #dx-move-pad{left:calc(17px + env(safe-area-inset-left));right:auto;top:auto;bottom:calc(16px + env(safe-area-inset-bottom));margin:0;width:132px;height:132px;z-index:1053;opacity:.85;background:radial-gradient(circle at 40% 30%,#dba54337,#192b458a 75%);border:3px solid #ffd178;box-shadow:0 0 0 5px #0005,0 0 26px #e5a74277}\n#duke-x-controls #dx-move-pad .knob{left:43px;top:43px}\n#duke-x-controls #dx-move-pad::after{content:'MOVE';position:absolute;bottom:6px;left:0;width:100%;color:#ffe3a0;font:900 11px system-ui,sans-serif;letter-spacing:2px;text-align:center;pointer-events:none;text-shadow:0 2px 3px #000}\n#duke-x-controls.dx-in-game:not(.dx-overlay-open):not(.dx-player-dead) #dx-move-pad{display:block!important}\n#duke-x-controls.dx-player-dead #dx-move-pad{display:none!important}\n@media(hover:none) and (pointer:coarse){#duke-x-controls{display:block}}";
  (document.head||document.documentElement).appendChild(style);
 }
 const previous=document.getElementById('duke-x-controls');
 // Always replace the previous *Duke X owned* UI subtree, not the game canvas.
 // This also clears stale inline styles after a death or display-mode change.
 const template=document.createElement('template');
 template.innerHTML="<div id=\"duke-x-controls\" aria-label=\"Duke X game controls\">\n <div id=\"dx-move-zone\" aria-label=\"MOVE: walk forward backward and strafe\"></div><div id=\"dx-aim-zone\" aria-label=\"Floating aim joystick\"></div>\n <div class=\"dx-pad\" id=\"dx-move-pad\"><div class=\"knob\"></div></div><div class=\"dx-pad\" id=\"dx-aim-pad\"><div class=\"knob\"></div></div>\n <button id=\"dx-field-launch\" aria-label=\"Duke X field menu\">\u2630</button>\n <button id=\"dx-respawn\" aria-label=\"Restart Duke level\">RESTART RUN \u21bb</button>\n <div id=\"dx-field-tools\"><button data-dx-key=\"KeyE\">USE</button><button data-dx-key=\"Space\">JUMP</button><button data-dx-key=\"KeyC\">DUCK</button><button data-dx-key=\"KeyJ\">KICK</button></div>\n <section id=\"dx-field-sheet\" aria-label=\"Field computer menu\"><div class=\"dx-panel\">\n  <div class=\"dx-header\">DUKE X // FIELD COMPUTER <button id=\"dx-field-close\" aria-label=\"Close menu\">\u2715</button></div>\n  <div class=\"dx-sub\">Original Duke 3D \u00b7 Multiple simultaneous weapons \u00b7 Build your own arsenal</div>\n  <div class=\"dx-tabs\" role=\"tablist\"><button data-dx-tab=\"armory\" aria-selected=\"true\">ARMORY</button><button data-dx-tab=\"character\">CHARACTER</button><button data-dx-tab=\"operations\">OPS</button><button data-dx-tab=\"diagnostics\">TOOLS</button></div>\n  <div class=\"dx-menu-section active\" data-dx-section=\"armory\"><div class=\"dx-sub\">All equipped weapons and passive ranks operate together.</div><div id=\"dx-armory-list\" class=\"dx-menu-grid\"></div><h4 class=\"dx-sub\">EVOLUTION RESEARCH</h4><div id=\"dx-fusion-guide\" class=\"dx-menu-grid\"></div></div>\n  <div class=\"dx-menu-section\" data-dx-section=\"character\"><div class=\"dx-sub\">Starter changes take effect on the next run, not during combat.</div><div id=\"dx-character-list\"></div></div>\n  <div class=\"dx-menu-section\" data-dx-section=\"operations\"><div class=\"dx-actionline\"><button id=\"dx-native-menu\">DUKE MENU</button><button id=\"dx-show-map\">MAP</button><button id=\"dx-ops-restart\">RESTART RUN</button><button id=\"dx-career\">BUY PERMANENT RANK</button></div><div id=\"dx-ops-status\" class=\"dx-hint\">Earn Duke tokens through alien kills.</div></div>\n  <div class=\"dx-menu-section\" data-dx-section=\"diagnostics\"><p class=\"dx-hint\">Debug panels stay hidden unless explicitly enabled. Reports remain available.</p><div class=\"dx-actionline\"><button data-dx-debug=\"log\">RUNTIME LOG</button><button data-dx-debug=\"video\">VIDEO STATS</button><button data-dx-debug=\"off\">HIDE DIAGNOSTICS</button></div><div class=\"dx-sub\">LIVE MOBILE INPUT / NATIVE ENGINE</div><div id=\"dx-input-monitor\" aria-live=\"off\">Touch input values appear here after engine initialization.</div></div>\n </div></section>\n <section id=\"dx-level-mask\" aria-label=\"Choose power upgrade\"><div id=\"dx-level-card\">\n <div id=\"dx-level-title\">DUKE X \u2014 LEVEL UP</div><div id=\"dx-level-sub\">CHOOSE YOUR NEXT POWER</div>\n <div id=\"dx-cards\"><button class=\"dx-upgrade\" data-dx-pick=\"1\"><span class=\"dx-icon\"><span class=\"dx-symbol\">I</span></span><span class=\"dx-type\"></span><span class=\"dx-power\"></span><span class=\"dx-desc\"></span><span class=\"dx-tier\"></span><span class=\"dx-evo\"></span></button>\n <button class=\"dx-upgrade\" data-dx-pick=\"2\"><span class=\"dx-icon\"><span class=\"dx-symbol\">II</span></span><span class=\"dx-type\"></span><span class=\"dx-power\"></span><span class=\"dx-desc\"></span><span class=\"dx-tier\"></span><span class=\"dx-evo\"></span></button>\n <button class=\"dx-upgrade\" data-dx-pick=\"3\"><span class=\"dx-icon\"><span class=\"dx-symbol\">III</span></span><span class=\"dx-type\"></span><span class=\"dx-power\"></span><span class=\"dx-desc\"></span><span class=\"dx-tier\"></span><span class=\"dx-evo\"></span></button></div>\n <div id=\"dx-level-actions\"><button id=\"dx-reroll\">REROLL</button><button id=\"dx-banish-toggle\">BANISH</button></div>\n <div id=\"dx-level-foot\">Weapons equip automatically and fire concurrently. Level-up pauses solo runs.</div>\n </div></section>\n</div>";
 const restored=template.content.querySelector('#duke-x-controls');
 if(!restored)throw Error('DXV21: mobile UI template invalid');
 restored.dataset.dxUi='21';
 if(previous)previous.replaceWith(restored);else document.body.appendChild(restored);
 for(const id of dxV21Required)if(!restored.querySelector('#'+id))throw Error('DXV21: missing UI node '+id);
 console.info('[DXV21] Native mobile HUD restored: MOVE, LOOK, field computer, power choices');
 return restored;
}
const root=dxV21EnsureShell();if(root.dataset.dx25)return;root.dataset.dx25='yes';
// DXV31: input adapters only. Duke's original synchronized SK_* bits perform
// jetpack toggling, inventory cycling, healing, steroids and vision.
const dx31Tools=root.querySelector('#dx-field-tools');
if(dx31Tools){
 dx31Tools.innerHTML='<button data-dx-key="KeyE">USE</button><button data-dx-key="Space">JUMP</button>'+ 
 '<button data-dx-key="KeyC">DESCEND</button><button data-dx-inv="4">JETPACK</button>'+ 
 '<button data-dx-inv="5">MEDKIT</button><button data-dx-inv="2">ITEM ▶</button>'+ 
 '';
}
// DXV33 native autonomy and GPU bright-pass options. No free BLAST action.
let dx33Auto=false,dx33GlowLevel=1,dx33AppliedModule=null;
try{dx33Auto=localStorage.getItem('dukex.auto.v33')==='true';dx33GlowLevel=Number(localStorage.getItem('dukex.glow.v33')||'1');}catch(_){}
const dx33Ops=root.querySelector('[data-dx-section="operations"] .dx-actionline');
if(dx33Ops){
 const auto=document.createElement('button');auto.id='dx33-auto';auto.textContent='AUTOPILOT '+(dx33Auto?'ON':'OFF');dx33Ops.appendChild(auto);
 auto.onclick=()=>{const m=window.Module; if(typeof m?._DukeX_SetAutoMode!=='function'){
    const st=root.querySelector('#dx-ops-status');if(st)st.textContent='Autopilot requires rebuilt V33 native WASM';return;
  }
  dx33Auto=!dx33Auto;try{localStorage.setItem('dukex.auto.v33',String(dx33Auto));}catch(_){}
  dx33AppliedModule=null;m._DukeX_SetAutoMode(dx33Auto?1:0);dx33AppliedModule=m;
  auto.textContent='AUTOPILOT '+(dx33Auto?'ON':'OFF');};
 const glow=document.createElement('button');glow.id='dx33-glow';dx33Ops.appendChild(glow);
 const levels=['OFF','LOW','HIGH'];dx33GlowLevel=Math.max(0,Math.min(2,dx33GlowLevel||0));
 function applyGlow(){glow.textContent='GPU BLOOM '+levels[dx33GlowLevel];window.DX33Glow?.setLevel(dx33GlowLevel)}
 glow.onclick=()=>{dx33GlowLevel=(dx33GlowLevel+1)%3;try{localStorage.setItem('dukex.glow.v33',String(dx33GlowLevel));}catch(_){}applyGlow();};
 applyGlow();
 setInterval(()=>{const m=window.Module;if(m===dx33AppliedModule || typeof m?._DukeX_SetAutoMode!=='function')return;
   m._DukeX_SetAutoMode(dx33Auto?1:0);dx33AppliedModule=m;},750);
}
const dx31Ops=root.querySelector('[data-dx-section="operations"] .dx-actionline');
if(dx31Ops){
 for(const [n,title] of [[1,'ITEM ◀'],[3,'USE ITEM'],[4,'TOGGLE JETPACK'],[5,'USE MEDKIT'],[6,'STEROIDS'],[7,'NIGHT VISION'],[8,'HOLODUKE']]){
  const b=document.createElement('button');b.dataset.dxInv=String(n);b.textContent=title;dx31Ops.appendChild(b);
 }
 const status=document.createElement('p');status.id='dx31-inventory-status';status.className='dx-hint';
 status.textContent='Native inventory bridge waiting for EDuke32';dx31Ops.after(status);
}
function dx31Inventory(n){
 const m=window.Module,fn=m&&m._DukeX_QueueInventoryAction;
 if(typeof fn!=='function'){
  const status=root.querySelector('#dx31-inventory-status');
  if(status)status.textContent='NO NATIVE INVENTORY BRIDGE — V31 engine rebuild required';
  return false;
 }
 try{return fn(n)===1}catch(e){console.warn('DukeX inventory dispatch',e);return false}
}
for(const b of root.querySelectorAll('[data-dx-inv]'))b.addEventListener('click',e=>{
 e.preventDefault();dx31Inventory(Number(b.dataset.dxInv));
});
setInterval(()=>{
 const m=window.Module,probe=m&&m._DukeX_InventoryAmount;
 if(typeof probe!=='function')return;
 const fuel=probe(1),med=probe(2),armor=probe(3),boost=probe(4);
 const jet=root.querySelector('#dx-field-tools [data-dx-inv="4"]');
 if(jet)jet.textContent='JET '+Math.round(fuel*100/1600)+'%';
 const status=root.querySelector('#dx31-inventory-status');
 if(status)status.textContent='JET '+fuel+'/1600 · MED '+med+' · ARMOR '+armor+' · STEROIDS '+boost;
},900);
// V32: distinguish rendering a particle from applying an actual health hit.
// Data is sourced ONLY from the compiled engine's native combat hooks.
window.DX32_CombatState=()=>{
 const m=window.Module||{},read=(n,arg)=>{const f=m['_'+n];try{return typeof f==='function'?(arg===undefined?f():f(arg)):null}catch(_){return null}};
 return {native:typeof m._DukeX_V32Marker==='function',hits:read('DukeX_CombatHitsApplied'),
 damage:read('DukeX_CombatDamageApplied'),blocked:read('DukeX_CombatHitsBlocked'),
 victim:read('DukeX_CombatLastVictim'),before:read('DukeX_CombatLastHPBefore'),
 after:read('DukeX_CombatLastHPAfter'),frozen:read('DukeX_CombatStatusCount',1),
 shrunk:read('DukeX_CombatStatusCount',2),expanded:read('DukeX_CombatStatusCount',3),
 stunned:read('DukeX_CombatStatusCount',4),burning:read('DukeX_CombatStatusCount',5),
 fusions:read('DukeX_FusionProcCount')};
};
const dx32Diag=root.querySelector('[data-dx-section="diagnostics"]');
if(dx32Diag && !root.querySelector('#dx32-combat-stats')){
 const el=document.createElement('pre');el.id='dx32-combat-stats';
 el.style.cssText='white-space:pre-wrap;font:600 11px monospace;color:#8de9e4;border:1px solid #7d8268;padding:8px;border-radius:4px';
 el.textContent='NATIVE COMBAT: waiting for V32 WASM';dx32Diag.appendChild(el);
 setInterval(()=>{
  if(!root.classList.contains('dx-menu-open'))return;
  const d=window.DX32_CombatState();
  el.textContent=d.native?`NATIVE DAMAGE VERIFIED: ${d.damage} HP / ${d.hits} hits\nLast enemy #${d.victim}: ${d.before} -> ${d.after} HP\nFreeze ${d.frozen} · Shrink ${d.shrunk} · Grow ${d.expanded} · Stun ${d.stunned} · Burn ${d.burning}\nFusion triggers ${d.fusions} · Blocked ${d.blocked}`:'V32 COMBAT WASM NOT INSTALLED — previous engine still running';
 },700);
}
window.DX31_InventoryState=()=>({bridge:typeof window.Module?._DukeX_QueueInventoryAction==='function',
 fuel:window.Module?._DukeX_InventoryAmount?.(1),
 medkit:window.Module?._DukeX_InventoryAmount?.(2),
 beams:window.Module?._DukeX_V31BeamSegments?.(),
 jumps:window.Module?._DukeX_V31AirJumps?.()});
// DUKE_X_MOBILE_V31: distinct native inventory SK_* actions + real beam HUD.

// DXV29: user-selectable, *native* Build velocity scaling. Not CSS joystick speed.
let dx29RunPercent=440,dx29LastEngine=null;
try{const saved=Number(localStorage.getItem('dukex.run.v29'));if([320,440,560].includes(saved))dx29RunPercent=saved}catch(_){}
const dx29Ops=root.querySelector('[data-dx-section="operations"] .dx-actionline');
let dx29RunBtn=null;
if(dx29Ops){
 dx29RunBtn=document.createElement('button');dx29RunBtn.id='dx29-run';
 dx29RunBtn.textContent='RUN TURBO '+dx29RunPercent+'%';
 dx29Ops.appendChild(dx29RunBtn);
 dx29RunBtn.onclick=()=>{dx29RunPercent=dx29RunPercent===320?440:dx29RunPercent===440?560:320;
  try{localStorage.setItem('dukex.run.v29',String(dx29RunPercent))}catch(_){}
  dx29RunBtn.textContent='RUN '+(dx29RunPercent===320?'FAST':dx29RunPercent===440?'TURBO':'INSANE')+' '+dx29RunPercent+'%';
  dx29LastEngine=null;
  dx29ApplyRun();};
}
function dx29ApplyRun(){const m=window.Module;
 if(!m||typeof m._DukeX_SetRunScale!=='function')return false;
 if(m===dx29LastEngine)return true;
 try{m._DukeX_SetRunScale(dx29RunPercent);dx29LastEngine=m;return true}catch(_){return false}
}
setInterval(dx29ApplyRun,450);
window.DX29_Status=()=>({runSelected:dx29RunPercent,runNative:!!window.Module?._DukeX_SetRunScale,
 visuals:callNumber('DukeX_ProbeVisuals'),traps:callNumber('DukeX_ProbeTripwires'),
 napalmFields:callNumber('DukeX_ProbeNapalmFields'),actualRun:callNumber('DukeX_ProbeRunScale'),
 ordnance:callNumber('DukeX_V30ActiveOrdnance'),impacts:callNumber('DukeX_V30ImpactCount'),
 bounces:callNumber('DukeX_V30RicochetCount'),expanderAvailable:callNumber('DukeX_V30EditionHasExpander')});

// DXV26: old V18/V22 rings were NEVER unmounted. Their more-specific CSS
// overrides the V23 display:none and paints a doubled MOVE control.
// Remove the obsolete DOM nodes entirely; only DX23 may own the two thumbs.
for(const id of ['dx-move-pad','dx-aim-pad','dx-move-zone','dx-aim-zone','dx22-traverse']){
 const obsolete=document.getElementById(id);if(obsolete)obsolete.remove();
}

// DXV20: inject the movement-pad CSS at runtime. This avoids relying on a
// specific prior generated HTML block or even the presence of a version comment.
if(!document.getElementById('dx-v20-move-style')){
 const st=document.createElement('style');st.id='dx-v20-move-style';
 st.textContent="/* DXV19: Android desktop-site, stylus, foldables may not match pointer:coarse. */\n#duke-x-controls.dx-v19-touch{display:block!important}\n#duke-x-controls.dx-v19-touch:not(.dx-in-game) #dx-move-pad{display:none}\n/* MOVE stays anchored on screen and receives a real visible, labeled pad. */\n#duke-x-controls #dx-move-zone{left:0;bottom:0;width:min(48vw,360px);height:48dvh;z-index:1051}\n#duke-x-controls #dx-move-pad{left:calc(17px + env(safe-area-inset-left));right:auto;top:auto;bottom:calc(16px + env(safe-area-inset-bottom));margin:0;width:132px;height:132px;z-index:1053;opacity:.85;background:radial-gradient(circle at 40% 30%,#dba54337,#192b458a 75%);border:3px solid #ffd178;box-shadow:0 0 0 5px #0005,0 0 26px #e5a74277}\n#duke-x-controls #dx-move-pad .knob{left:43px;top:43px}\n#duke-x-controls #dx-move-pad::after{content:'MOVE';position:absolute;bottom:6px;left:0;width:100%;color:#ffe3a0;font:900 11px system-ui,sans-serif;letter-spacing:2px;text-align:center;pointer-events:none;text-shadow:0 2px 3px #000}\n#duke-x-controls.dx-in-game:not(.dx-overlay-open):not(.dx-player-dead) #dx-move-pad{display:block!important}\n#duke-x-controls.dx-player-dead #dx-move-pad{display:none!important}\n@media(hover:none) and (pointer:coarse){#duke-x-controls{display:block}}";
 (document.head||document.documentElement).appendChild(st);
}

// DXV19: Some Android/desktop-site configurations report a fine pointer even
// though the device has touch. The V18 CSS hid the entire HUD in that case.
const touchCapable=(navigator.maxTouchPoints||0)>0 ||
  matchMedia('(any-pointer:coarse)').matches || /Android|iPhone|iPad/i.test(navigator.userAgent);
if(touchCapable)root.classList.add('dx-v19-touch');
const codes={KeyW:[87,'w'],KeyE:[69,'e'],KeyC:[67,'c'],KeyA:[65,'a'],KeyS:[83,'s'],KeyD:[68,'d'],Space:[32,' '],KeyJ:[74,'j'],Escape:[27,'Escape'],Tab:[9,'Tab'],Digit1:[49,'1'],Digit2:[50,'2'],Digit3:[51,'3']};
const held=new Set();
// Hardware keyboard fallback remains available; touch buttons use native synced
// input bits because synthetic key events are not reliably recognized by SDL.
const touchMasks={KeyE:4,Space:1,KeyC:2,KeyJ:8};
const pressedTouchKeys=new Set();
function applyTouchButtons(){const m=window.Module;if(m&&typeof m._DukeX_SetTouchButtons==='function'){
 let mask=0;for(const k of pressedTouchKeys)mask|=touchMasks[k]||0;
 m._DukeX_SetTouchButtons(mask);return true;
}return false;}
function buttonHold(code,on){
 if(touchMasks[code]){on?pressedTouchKeys.add(code):pressedTouchKeys.delete(code);
  if(applyTouchButtons()){hold(code,false);return;}}
 hold(code,on);
}

function send(code,on){const k=codes[code];if(!k)return;
 const e=new KeyboardEvent(on?'keydown':'keyup',{key:k[1],code,bubbles:true,cancelable:true});
 try{Object.defineProperty(e,'keyCode',{get:()=>k[0]});Object.defineProperty(e,'which',{get:()=>k[0]});}catch(_){}window.dispatchEvent(e);
}
function hold(code,on){if(held.has(code)===on)return;on?held.add(code):held.delete(code);send(code,on)}
function press(code){send(code,true);setTimeout(()=>send(code,false),80)}
function callNumber(name,args=[]){try{const m=window.Module;const f=m&&m['_'+name];return typeof f==='function'?f(...args):0}catch(e){return 0}}
function callString(name,args=[]){try{const m=window.Module;if(!m)return '';if(typeof m.ccall==='function')return m.ccall(name,'string',Array(args.length).fill('number'),args)||'';
 const p=callNumber(name,args),buf=m.HEAPU8||(m.wasmMemory&&new Uint8Array(m.wasmMemory.buffer));if(!p||!buf)return '';
 let end=p;while(end<buf.length&&buf[end]&&end<p+190)end++;return new TextDecoder().decode(buf.slice(p,end));}catch(e){return ''}}
function notify(text){const out=root.querySelector('#dx-ops-status');out.textContent=text}
// DXV27: Touch-first, two-stick native controller.
// Fixes V23-26: window-only pointermove + mode/viewport resets discarded actual
// Android TouchEvent drags. Each thumb has one owner and one native axis pair.
const axes={move:{pid:null,x:0,y:0},aim:{pid:null,x:0,y:0}};
let prevGameplay=false,prevDead=false,prevRunGeneration=-1;
const joyMap=new Map();
const dx27={touchStarts:0,touchMoves:0,touchEnds:0,pointerStarts:0,pointerMoves:0,
 nativeSends:0,nativeFailures:0,lastError:'',lastGesture:'none',lastNativeSamples:null};
function bridge(){return window.Module||null}
function nativeReady(){const m=bridge();return !!(m&&typeof m._DukeX_SetTouchAxes==='function'&&typeof m._DukeX_SetAimAxes==='function')}
function joystickEnabled(){
 const m=bridge();
 return !!m && !document.hidden && !root.classList.contains('dx-overlay-open') &&
   !root.classList.contains('dx-player-dead') &&
   typeof m._DukeX_SetTouchAxes==='function' && typeof m._DukeX_SetAimAxes==='function';
}
function nativeAxes(){const m=bridge();if(!m)return false;
 const tx=Math.round(axes.move.x*1000),ty=Math.round(axes.move.y*1000);
 const ax=Math.round(axes.aim.x*1000),ay=Math.round(axes.aim.y*1000);
 try{
  if(typeof m._DukeX_SetTouchAxes!=='function'||typeof m._DukeX_SetAimAxes!=='function')return false;
  m._DukeX_SetTouchAxes(tx,ty);m._DukeX_SetAimAxes(ax,ay);
  ++dx27.nativeSends;dx27.lastError='';return true;
 }catch(e){++dx27.nativeFailures;dx27.lastError=String(e);return false}
}
function axis(which,x,y){
 axes[which].x=Math.max(-1,Math.min(1,x));axes[which].y=Math.max(-1,Math.min(1,y));
 // Never synthesize keyboard events on Android: SDL won't consume them.
 nativeAxes();
}
function padHome(which){const j=joyMap.get(which);if(!j)return;
 j.knob.style.transform='translate3d(0,0,0)';j.node.classList.remove('dx27-active');
 j.readout.textContent=which==='move'?'MOVE':'LOOK';
}
function releaseOne(which,id){const j=joyMap.get(which);if(!j||j.pid===null)return;
 if(id!=null&&j.pid!==id)return;
 j.pid=null;axes[which].pid=null;axis(which,0,0);padHome(which);
}
function release(e){const id=e&&e.pointerId!=null?'p:'+e.pointerId:null;
 releaseOne('move',id);releaseOne('aim',id);
}
function resetMobileInput(){releaseOne('move',null);releaseOne('aim',null);
 axis('move',0,0);axis('aim',0,0);
 pressedTouchKeys.clear();applyTouchButtons();for(const k of [...held])hold(k,false);
}
const style27=document.createElement('style');style27.id='dx27-two-stick-style';style27.textContent=`
#duke-x-controls #dx-move-zone,#duke-x-controls #dx-aim-zone,
#duke-x-controls #dx-move-pad,#duke-x-controls #dx-aim-pad,#dx22-traverse{display:none!important;pointer-events:none!important}
.dx27-stick{position:fixed!important;z-index:2147482000!important;display:block!important;
 width:138px;height:138px;touch-action:none!important;overscroll-behavior:contain;
 user-select:none;-webkit-user-select:none;pointer-events:auto!important;-webkit-tap-highlight-color:transparent}
#dx23-move{left:calc(9px + env(safe-area-inset-left));bottom:calc(23px + env(safe-area-inset-bottom))}
#dx23-aim{right:calc(9px + env(safe-area-inset-right));bottom:calc(192px + env(safe-area-inset-bottom))}
.dx27-stick.dx27-off{opacity:.55}
.dx27-ring{position:absolute;inset:5px;border:3px solid #f2b953;border-radius:50%;
 background:radial-gradient(circle,#b47e423d,#121c29dc 73%);box-shadow:0 0 0 4px #0007,0 0 20px #000a}
#dx23-aim .dx27-ring{border-color:#dd765e}
.dx27-knob{position:absolute;left:48px;top:48px;width:42px;height:42px;pointer-events:none;
 border-radius:50%;background:radial-gradient(circle at 30% 20%,#ffe3a4,#bd6d30 75%);
 box-shadow:0 2px 10px #000c;will-change:transform}
#dx23-aim .dx27-knob{background:radial-gradient(circle at 30% 20%,#ffdcc9,#b95548 75%)}
.dx27-readout{position:absolute;bottom:9px;width:100%;text-align:center;color:#ffdd9b;
 font:800 10px/1.2 ui-monospace,monospace;letter-spacing:1px;pointer-events:none;text-shadow:0 2px 4px black}
.dx27-active .dx27-ring{box-shadow:0 0 0 4px #0007,0 0 20px #ffc477aa}
.dx27-stick.dx27-disconnected .dx27-ring{border-color:#e65b50}
@media(max-height:500px){.dx27-stick{transform:scale(.82);transform-origin:bottom left}#dx23-aim{transform-origin:bottom right;bottom:105px}}
`;document.head.appendChild(style27);
for(const old of document.querySelectorAll('#dx23-move,#dx23-aim,#dx22-traverse'))old.remove();
function mountPad(which){
 const node=document.createElement('div');node.id='dx23-'+which;node.className='dx27-stick';
 node.setAttribute('role','group');node.setAttribute('aria-label',which==='move'?'MOVE walk run and strafe':'LOOK turn and pan');
 node.innerHTML='<div class="dx27-ring"></div><div class="dx27-knob"></div><div class="dx27-readout">'+(which==='move'?'MOVE':'LOOK')+'</div>';
 document.body.appendChild(node);
 const j={node,knob:node.querySelector('.dx27-knob'),readout:node.querySelector('.dx27-readout'),pid:null,cx:0,cy:0};
 joyMap.set(which,j);return j;
}
mountPad('move');mountPad('aim');
function padAt(x,y){for(const which of ['move','aim']){
 const j=joyMap.get(which),r=j.node.getBoundingClientRect();
 if(x>=r.left&&x<=r.right&&y>=r.top&&y<=r.bottom)return which;
 }return null}
function claim(which,id,x,y){const j=joyMap.get(which);
 if(!j||j.pid!==null||!joystickEnabled())return false;
 j.pid=id;axes[which].pid=id;j.cx=x;j.cy=y;j.node.classList.add('dx27-active');
 axis(which,0,0);dx27.lastGesture=which+' started '+id;return true;
}
function travel(which,id,x,y){const j=joyMap.get(which);if(!j||j.pid!==id)return false;
 const dx=x-j.cx,dy=y-j.cy,dist=Math.hypot(dx,dy),radius=52;
 const n=Math.min(1,dist/radius),mag=n<=.09?0:(n-.09)/.91;
 const vx=dist<.001?0:dx/dist*mag,vy=dist<.001?0:dy/dist*mag;
 axis(which,vx,vy);
 j.knob.style.transform=`translate3d(${Math.round(vx*40)}px,${Math.round(vy*40)}px,0)`;
 j.readout.textContent=(which==='move'?'M':'L')+' '+Math.round(vx*100)+' '+Math.round(vy*100);
 dx27.lastGesture=which+' moved '+id;return true;
}
// Android Chrome primarily delivers TouchEvents. Capture at document level
// so movement continues after the finger leaves the 138px art and even when
// the underlying canvas steals pointer capture.
document.addEventListener('touchstart',e=>{
 let owned=false;for(const t of e.changedTouches){const which=padAt(t.clientX,t.clientY);
  if(which&&claim(which,'t:'+t.identifier,t.clientX,t.clientY)){++dx27.touchStarts;owned=true}
 }if(owned&&e.cancelable)e.preventDefault();
},{capture:true,passive:false});
document.addEventListener('touchmove',e=>{
 let owned=false;for(const t of e.changedTouches)for(const which of ['move','aim']){
  if(travel(which,'t:'+t.identifier,t.clientX,t.clientY)){++dx27.touchMoves;owned=true;break}
 }if(owned&&e.cancelable)e.preventDefault();
},{capture:true,passive:false});
function endTouches(e){let owned=false;
 for(const t of e.changedTouches)for(const which of ['move','aim']){
  const j=joyMap.get(which);if(j.pid==='t:'+t.identifier){releaseOne(which,'t:'+t.identifier);++dx27.touchEnds;owned=true}
 }if(owned&&e.cancelable)e.preventDefault();
}
document.addEventListener('touchend',endTouches,{capture:true,passive:false});
document.addEventListener('touchcancel',endTouches,{capture:true,passive:false});
// Mouse/stylus fallback; touchscreen events are owned only by the handlers above.
for(const which of ['move','aim'])joyMap.get(which).node.addEventListener('pointerdown',e=>{
 if(e.pointerType==='touch'&&('ontouchstart' in window))return;
 if(claim(which,'p:'+e.pointerId,e.clientX,e.clientY)){++dx27.pointerStarts;e.preventDefault()}
},{passive:false});
document.addEventListener('pointermove',e=>{
 if(e.pointerType==='touch'&&('ontouchstart' in window))return;
 for(const which of ['move','aim'])if(travel(which,'p:'+e.pointerId,e.clientX,e.clientY)){++dx27.pointerMoves;if(e.cancelable)e.preventDefault();break}
},{capture:true,passive:false});
document.addEventListener('pointerup',release,true);
document.addEventListener('pointercancel',release,true);
window.addEventListener('blur',resetMobileInput);
window.addEventListener('pagehide',resetMobileInput);
document.addEventListener('visibilitychange',()=>{if(document.hidden)resetMobileInput()});
// DO NOT reset on visualViewport.resize: Android collapses the address bar
// during touches and can fire continuous viewport resizing during a drag.
let dx27LastMode=-1;
setInterval(()=>{
 const connected=nativeReady(),ok=joystickEnabled();
 for(const which of ['move','aim']){
  const j=joyMap.get(which);j.node.classList.toggle('dx27-off',!ok);
  j.node.classList.toggle('dx27-disconnected',!connected);
  if(!connected)j.readout.textContent='NATIVE OFF';
 }
 // Controls never disappear. Only death/modal releases a held stick.
 if(!ok&&(axes.move.pid!==null||axes.aim.pid!==null))resetMobileInput();
 nativeAxes();
 const m=bridge();if(m&&typeof m._DukeX_ProbeTouchSamples==='function'){
  try{dx27.lastNativeSamples=m._DukeX_ProbeTouchSamples()}catch(e){dx27.lastError=String(e)}
 }
},90);
window.DX23_InputState=()=>({move:[axes.move.x,axes.move.y],look:[axes.aim.x,axes.aim.y],
 movePointer:axes.move.pid,lookPointer:axes.aim.pid,enabled:joystickEnabled(),version:27});
window.DX27_TouchReport=()=>({version:27,connected:nativeReady(),gameMode:callNumber('DukeX_ProbeGameMode'),
 axes:window.DX23_InputState(),touchStarts:dx27.touchStarts,touchMoves:dx27.touchMoves,
 touchEnds:dx27.touchEnds,pointerStarts:dx27.pointerStarts,pointerMoves:dx27.pointerMoves,
 nativeSends:dx27.nativeSends,nativeFailures:dx27.nativeFailures,lastError:dx27.lastError,
 lastNativeSamples:dx27.lastNativeSamples,lastGesture:dx27.lastGesture});
for(const b of root.querySelectorAll('[data-dx-key]')){
 const code=b.dataset.dxKey;b.addEventListener('pointerdown',e=>{e.preventDefault();buttonHold(code,true)});
 for(const event of ['pointerup','pointercancel','pointerleave'])b.addEventListener(event,()=>buttonHold(code,false));
}

let nextOfferTime=0;
let menu=false,selectedTab='armory',banishMode=false,pendingBefore=0,lastLoadout=-2;
const sheet=root.querySelector('#dx-field-sheet'),mask=root.querySelector('#dx-level-mask');
function modalState(){root.classList.toggle('dx-overlay-open',menu||mask.style.display==='flex');}
function openMenu(on){if(on)document.body.classList.remove('dx-show-debug-log','dx-show-debug-video');menu=on;root.classList.toggle('dx-menu-open',on);modalState();release(null);if(on){refreshMenu();}}
root.querySelector('#dx-field-launch').addEventListener('click',()=>openMenu(!menu));
root.querySelector('#dx-field-close').addEventListener('click',()=>openMenu(false));
sheet.addEventListener('pointerdown',e=>{if(e.target===sheet)openMenu(false)});
for(const b of root.querySelectorAll('[data-dx-tab]'))b.addEventListener('click',()=>{selectedTab=b.dataset.dxTab;refreshMenu()});
function refreshMenu(){
 for(const b of root.querySelectorAll('[data-dx-tab]'))b.setAttribute('aria-selected',String(b.dataset.dxTab===selectedTab));
 for(const b of root.querySelectorAll('[data-dx-section]'))b.classList.toggle('active',b.dataset.dxSection===selectedTab);
 if(selectedTab==='armory')redrawArmory();if(selectedTab==='character')redrawCharacters();
 if(selectedTab==='diagnostics')updateTouchMonitor();
 if(selectedTab==='operations')notify(`TOKENS ${callNumber('DukeX_CareerCredits')} • META RANK ${callNumber('DukeX_CareerRank')} • Next ${20*(1+callNumber('DukeX_CareerRank'))} tokens`);
}
function redrawArmory(){const list=root.querySelector('#dx-armory-list');list.replaceChildren();let found=0;
 const n=callNumber('DukeX_ItemCount');for(let id=0;id<n&&id<96;id++){
  const rank=callNumber('DukeX_ItemRank',[id]);if(rank===0)continue;
  const item=document.createElement('div');item.className='dx-inventory-item';
  const title=document.createElement('b');title.textContent=callString('DukeX_ItemName',[id])||`POWER ${id}`;
  const desc=document.createElement('small');desc.textContent=`${callNumber('DukeX_ItemIsWeapon',[id])?'WEAPON':'PASSIVE'} · RANK ${rank}`;
  item.append(title,desc);list.append(item);found++;
 }
 if(!found){const p=document.createElement('p');p.className='dx-hint';p.textContent='Enter a level and collect XP crystals to build your arsenal.';list.append(p);}
 const guide=root.querySelector('#dx-fusion-guide');guide.replaceChildren();let displayed=0;
 const recipeCount=callNumber('DukeX_RecipeCount');
 for(let r=0;r<recipeCount&&r<40;r++){
  const weapon=callNumber('DukeX_RecipeWeapon',[r]),passive=callNumber('DukeX_RecipePassive',[r]);
  const wr=callNumber('DukeX_ItemRank',[weapon]),pr=callNumber('DukeX_ItemRank',[passive]);
  if(wr===0 && displayed>=3)continue;
  const name=callString('DukeX_RecipeName',[r]),active=wr>=5&&pr>=(weapon<7?3:2);
  const title=document.createElement('b');title.textContent=name || 'UNKNOWN EVOLUTION';
  const status=document.createElement('small');
  status.textContent=active?'FUSED · ACTIVE':`${callString('DukeX_ItemName',[weapon])} ${wr}/5 + ${callString('DukeX_ItemName',[passive])} ${pr}/${weapon<7?3:2}`;
  const item=document.createElement('div');item.className='dx-inventory-item';item.append(title,status);guide.append(item);displayed++;
 }
}
const loadoutDetails=['Automatic pistol / classic all-rounder','RPG storm + explosive trap field','Laser grid + guided seekers','Radioactive aura + armor; close-range survival','Freezeblast + shatter barrage'];
const starterKey='dukex.starter.v16';let selectedLoadout=0;
try{selectedLoadout=Math.max(0,Math.min(4,+localStorage.getItem(starterKey)||0));}catch(_){}
function applyStarter(){const m=window.Module;if(m&&typeof m._DukeX_SelectLoadout==='function')m._DukeX_SelectLoadout(selectedLoadout)}
function redrawCharacters(){const host=root.querySelector('#dx-character-list');host.replaceChildren();
 for(let i=0;i<5;i++){const b=document.createElement('button');b.className='dx-profile'+(selectedLoadout===i?' selected':'');
 const name=callString('DukeX_LoadoutName',[i])||['DUKE CLASSIC','DEMOLITION EXPERT','CYBER COMMANDO','HAZMAT SPECIALIST','CRYO MARINE'][i];
 b.textContent=`${selectedLoadout===i?'◆ ':'◇ '}${name} — ${loadoutDetails[i]}`;
 b.onclick=()=>{selectedLoadout=i;try{localStorage.setItem(starterKey,String(i))}catch(_){}applyStarter();redrawCharacters()};host.append(b);}
 const p=document.createElement('p');p.className='dx-hint';p.textContent='Selected starter is applied when your NEXT level begins. All other weapons can still be found as upgrades.';host.append(p);
}
root.querySelector('#dx-native-menu').onclick=()=>{openMenu(false);press('Escape')};
root.querySelector('#dx-show-map').onclick=()=>{openMenu(false);press('Tab')};
function restartRun(){const result=callNumber('DukeX_RequestRestart');if(result){openMenu(false);resetMobileInput();notify('Restart requested through native EDuke32.')}else notify('Native restart unavailable. Rebuild V18 first.')}
root.querySelector('#dx-respawn').onclick=restartRun;
root.querySelector('#dx-ops-restart').onclick=restartRun;
root.querySelector('#dx-career').onclick=()=>{if(callNumber('DukeX_BuyCareerRank')){saveCareer();refreshMenu()}else notify('Not enough tokens, or already at the maximum rank.')};
for(const b of root.querySelectorAll('[data-dx-debug]'))b.onclick=()=>{
 document.body.classList.toggle('dx-show-debug-log',b.dataset.dxDebug==='log');
 document.body.classList.toggle('dx-show-debug-video',b.dataset.dxDebug==='video');openMenu(false);
 // Explicitly opt-in only; both diagnostic scripts still collect logs silently.
};
let lastActiveTouch='none';
function snapshotTouch(){
 const move=axes.move, aim=axes.aim;
 if(Math.abs(move.x)+Math.abs(move.y)+Math.abs(aim.x)+Math.abs(aim.y)<.12)return;
 const probe=(n)=>callNumber('DukeX_ProbeTouch'+n);
 lastActiveTouch=`L(${Math.round(move.x*1000)},${Math.round(move.y*1000)}) R(${Math.round(aim.x*1000)},${Math.round(aim.y*1000)})
 | native F/S/T ${probe('Forward')}/${probe('Strafe')}/${probe('Turn')}
 | world ${probe('WorldX')},${probe('WorldY')} facing ${probe('Angle')}`;
}
setInterval(snapshotTouch,250);
function updateTouchMonitor(){
 const target=root.querySelector('#dx-input-monitor');if(!target)return;
 const m=window.Module, has=m&&typeof m._DukeX_SetTouchAxes==='function', probes=m&&typeof m._DukeX_ProbeTouchSamples==='function';
 const sv=(name)=>callNumber('DukeX_ProbeTouch'+name);
 const fmt=a=>Math.round(a.x*1000)+','+Math.round(a.y*1000);
 const pendingExport=!!(m&&typeof m._DukeX_PendingUpgrades==='function');
 target.textContent= 'HUD V26 | upgrade engine: '+(pendingExport?'CONNECTED':'NOT EXPORTED')+' | waiting power choices: '+(pendingExport?callNumber('DukeX_PendingUpgrades'):'?')+' | gamemode '+callNumber('DukeX_ProbeGameMode')+'\n'
 + 'Native bridge: '+(has?'CONNECTED':'NOT EXPORTED — NATIVE REBUILD REQUIRED')+' | engine input: '+(probes?'MEASURED':'NOT YET BUILT')+ '\n'
 +'MOVE stick '+fmt(axes.move)+' | LOOK stick '+fmt(axes.aim)+'\n'
 +(probes?('Engine P_GetInput calls '+sv('Samples')+' | forward '+sv('Forward')+' | strafe '+sv('Strafe')+' | turn '+sv('Turn')+' | pitch '+sv('Pitch')+' | buttons '+sv('Buttons')+'\nLast active: '+lastActiveTouch)
 :'Run the V17 native rebuild to install the input counters.');
}
setInterval(()=>{if(menu&&selectedTab==='diagnostics')updateTouchMonitor()},250);
// DXV25: exported truth at EVERY layer. Never equate pad motion with game movement.
let dx25TouchHistory=[];
function dx25Sample(){
 const m=window.Module||{}, p=(n)=>callNumber('DukeX_ProbeTouch'+n);
 const rec={time:Date.now(),move:[Math.round(axes.move.x*1000),Math.round(axes.move.y*1000)],
  look:[Math.round(axes.aim.x*1000),Math.round(axes.aim.y*1000)],
  bridge:typeof m._DukeX_SetTouchAxes==='function'&&typeof m._DukeX_SetAimAxes==='function',
  inputCalls:typeof m._DukeX_ProbeTouchSamples==='function'?p('Samples'):null,
  forward:p('Forward'),strafe:p('Strafe'),turn:p('Turn'),pitch:p('Pitch'),
  x:p('WorldX'),y:p('WorldY'),angle:p('Angle'),
  gm:callNumber('DukeX_ProbeGameMode'),health:callNumber('DukeX_ProbeRespawnHealth'),
  sector:callNumber('DukeX_ProbeSpawnSector'),spriteSector:callNumber('DukeX_ProbeSpriteSector'),
  z:callNumber('DukeX_ProbePlayerZ'),cameraDistance:callNumber('DukeX_ProbeCameraDistance'),
  offerSeed:callNumber('DukeX_ProbeOfferSeed'),offerGeneration:callNumber('DukeX_ProbeOfferGeneration'),
  activeNativeVisuals:callNumber('DukeX_ProbeVisuals'),activeTripwires:callNumber('DukeX_ProbeTripwires'),
  persistentNapalm:callNumber('DukeX_ProbeNapalmFields'),runPercent:callNumber('DukeX_ProbeRunScale')};
 dx25TouchHistory.push(rec);if(dx25TouchHistory.length>200)dx25TouchHistory.shift();
}
setInterval(dx25Sample,200);
window.DX25_ControlReport=()=>({version:29,url:location.href,userAgent:navigator.userAgent,
 time:new Date().toISOString(),wasmReady:!!window.Module, touchEvents:window.DX27_TouchReport(),
 nativeFunctions:Object.fromEntries(['DukeX_SetTouchAxes','DukeX_SetAimAxes','DukeX_ProbeTouchSamples',
 'DukeX_ProbeTouchWorldX','DukeX_ProbeGameMode','DukeX_RequestRestart','DukeX_PendingUpgrades'].map(k=>[k,typeof window.Module?.['_'+k]==='function'])),
 history:dx25TouchHistory.slice()});
// DUKE_X_MOBILE_V29: preserves V27 touch bridge, adds real native running speed and effect telemetry.
const dx25Save=document.createElement('button');dx25Save.id='dx25-save-report';
dx25Save.textContent='SAVE INPUT/RESPAWN REPORT';
dx25Save.style.cssText='min-height:42px;padding:8px;margin:10px 0;';
const dx25Monitor=root.querySelector('#dx-input-monitor');
if(dx25Monitor){dx25Monitor.parentNode.insertBefore(dx25Save,dx25Monitor.nextSibling)}
dx25Save.addEventListener('click',()=>{const blob=new Blob([JSON.stringify(window.DX25_ControlReport(),null,2)],{type:'application/json'});
 const link=document.createElement('a');link.href=URL.createObjectURL(blob);link.download='dukex-v26-control-report.json';
 document.body.appendChild(link);link.click();link.remove();setTimeout(()=>URL.revokeObjectURL(link.href),1500);});

let art=new Map(),artReady=false;
async function assetLoad(){if(artReady)return;const m=window.Module;if(!m||!m.FS||!m.FS.analyzePath('/DUKE3D.GRP').exists)return;
 artReady=true;try{art=await getDukeArt(m.FS,[46,2595,2605,1641,2609,1625])}catch(e){console.warn('[Duke X] original GRP icon decode unavailable',e)}
}
function redrawCards(pending){root.querySelector('#dx-level-sub').textContent=`${pending} POWER CHOICE${pending===1?'':'S'} AVAILABLE · ${callNumber('DukeX_ItemCount')||62} WEAPON/PERK PATHS`;
 for(const b of root.querySelectorAll('[data-dx-pick]')){
  const i=+b.dataset.dxPick,name=callString('DukeX_OfferName',[i]),desc=callString('DukeX_OfferDescription',[i]);
  const rank=callNumber('DukeX_OfferRank',[i]),tile=callNumber('DukeX_OfferIcon',[i]),evo=callNumber('DukeX_OfferEvolution',[i]);
  b.querySelector('.dx-power').textContent=name||'SUPPLY DROP';b.querySelector('.dx-desc').textContent=desc||'LEVEL UP YOUR ARSENAL';
  b.querySelector('.dx-type').textContent=`${callNumber('DukeX_OfferIsWeapon',[i])?'WEAPON':'PASSIVE'} · ${rank===0?'NEW':'UPGRADE'}`;
  b.querySelector('.dx-tier').textContent=`${i} · RANK ${rank+1}`;
  b.querySelector('.dx-evo').textContent=evo?'EVOLUTION / FUSION READY':'';
  const icon=b.querySelector('.dx-icon');let im=icon.querySelector('img');
  if(art.has(tile)){if(!im){icon.replaceChildren();im=document.createElement('img');im.alt='Original Duke ART';icon.append(im)}if(im.src!==art.get(tile))im.src=art.get(tile);}
  else{icon.replaceChildren();const symbol=document.createElement('span');symbol.className='dx-symbol';symbol.textContent=tile===2605?'✹':tile===1625?'⚡':'◆';icon.append(symbol)}
 }
 const rerolls=callNumber('DukeX_OfferRerolls'),banishes=callNumber('DukeX_OfferBanishes');
 const reroll=root.querySelector('#dx-reroll'),banish=root.querySelector('#dx-banish-toggle');
 reroll.textContent=`REROLL (${rerolls})`;reroll.disabled=rerolls===0;
 banish.textContent=banishMode?'SELECT CARD TO BANISH':`BANISH (${banishes})`;banish.disabled=banishes===0;
}
for(const b of root.querySelectorAll('[data-dx-pick]'))b.onclick=()=>{
 const i=Number(b.dataset.dxPick);
 if(banishMode){if(callNumber('DukeX_BanishOffer',[i])){banishMode=false;redrawCards(callNumber('DukeX_PendingUpgrades'))}return;}
 if(callNumber('DukeX_ChooseUpgrade',[i])){
   // The next random roll MUST NOT flash beneath the current selection.
   mask.style.display='none';delete mask.dataset.lastDrawn;
   nextOfferTime=performance.now()+260;pendingBefore=-1;modalState();
  }
 else{send('Digit'+i,true);send('Digit'+i,false)};
};
root.querySelector('#dx-reroll').onclick=()=>{if(callNumber('DukeX_RerollOffers'))redrawCards(callNumber('DukeX_PendingUpgrades'))};
root.querySelector('#dx-banish-toggle').onclick=()=>{banishMode=!banishMode;redrawCards(callNumber('DukeX_PendingUpgrades'))};
window.addEventListener('keydown',e=>{if(mask.style.display==='flex'&&/^Digit[123]$/.test(e.code)){e.preventDefault();const card=root.querySelector('[data-dx-pick=\"'+e.code.slice(-1)+'\"]');if(card)card.click();}},{capture:true});
const META_KEY='dukex.career.v1';let careerRestored=false;
function saveCareer(){if(!careerRestored)return;try{localStorage.setItem(META_KEY,JSON.stringify({v:1,credits:callNumber('DukeX_CareerCredits'),rank:callNumber('DukeX_CareerRank')}))}catch(_){}}
const startup=setInterval(()=>{const m=window.Module;if(!m||typeof m._DukeX_SelectLoadout!=='function')return;
 clearInterval(startup);applyStarter();careerRestored=true;
 try{const c=JSON.parse(localStorage.getItem(META_KEY)||'null');if(c?.v===1&&typeof m._DukeX_LoadCareer==='function')m._DukeX_LoadCareer(c.credits|0,c.rank|0)}catch(_){}
},90);
setInterval(saveCareer,4500);window.addEventListener('pagehide',saveCareer);
setInterval(()=>{
 const gm=callNumber('DukeX_ProbeGameMode'),gmAvailable=!!window.Module&&typeof window.Module._DukeX_ProbeGameMode==='function',
 inGame=gmAvailable?(gm&4)!==0:!!(window.Module&&typeof window.Module._DukeX_SetTouchAxes==='function');
 const dead=callNumber('DukeX_ProbeDeathState')===1;
 const generation=callNumber('DukeX_ProbeRunGeneration');
 if(generation!==prevRunGeneration || dead!==prevDead || inGame!==prevGameplay){
  resetMobileInput();
  if(generation!==prevRunGeneration){mask.style.display='none';pendingBefore=0;banishMode=false;openMenu(false);}
 }
 prevRunGeneration=generation;prevDead=dead;prevGameplay=inGame;
 root.classList.toggle('dx-in-game',inGame);
 root.classList.toggle('dx-player-dead',dead);
 const pending=callNumber('DukeX_PendingUpgrades');
 // Level-up cards have precedence over the field computer. Never leave an
 // unspent power choice hidden under a menu while the native solo sim pauses.
 const visible=pending>0&&!dead&&performance.now()>=nextOfferTime&&
   (inGame || typeof window.Module?._DukeX_ProbeGameMode!=='function');
 if(visible&&menu)openMenu(false);
 // NO PRE-ROLL: draw fresh weighted choices inside the native engine only when
 // the selection modal actually becomes visible, never on XP-gain tick.
 let openSucceeded=true;
 if(visible && typeof window.Module?._DukeX_BeginOffer==='function')
   openSucceeded=callNumber('DukeX_BeginOffer')===1;
 mask.style.display=(visible&&openSucceeded)?'flex':'none';modalState();
 if(visible&&openSucceeded){
   if(!artReady)assetLoad();
   if(pending!==pendingBefore || !mask.dataset.lastDrawn)redrawCards(pending);
   mask.dataset.lastDrawn='yes';
 }else delete mask.dataset.lastDrawn;
 pendingBefore=pending;
},200);
// Extract thumbnail pixels from the legally loaded, original DUKE3D.GRP in
// Emscripten MEMFS. ART tiles are column-major indexed data, PAL is 6-bit RGB.
// No assets are downloaded, recreated, or falsely labeled as Duke artwork.
async function getDukeArt(fs,tiles){
 const s=fs.open('/DUKE3D.GRP','r'), dvbuf=(pos,size)=>{const b=new Uint8Array(size);fs.read(s,b,0,size,pos);return b};
 try{
  const h=dvbuf(0,16);const head=new TextDecoder().decode(h.slice(0,12));if(!head.startsWith('KenSilverman'))throw Error('not a Build GRP');
  const dv=new DataView(h.buffer);const n=dv.getInt32(12,true);if(n<1||n>4096)throw Error('GRP count');
  const dir=dvbuf(16,n*16);const entries=new Map();let off=16+n*16;
  for(let i=0;i<n;i++){
   const item=dir.subarray(i*16,i*16+16);let name=new TextDecoder().decode(item.subarray(0,12)).replace(/\0.*$/,'').toUpperCase();
   const len=new DataView(dir.buffer,dir.byteOffset+i*16+12,4).getUint32(0,true);
   if(off+len>fs.stat('/DUKE3D.GRP').size)throw Error('invalid GRP entry');
   entries.set(name,{offset:off,len});off+=len;
  }
  const paletteFile=entries.get('PALETTE.DAT');if(!paletteFile||paletteFile.len<768)return new Map();
  const pal=dvbuf(paletteFile.offset,768);const out=new Map();
  for(const [name,file] of entries){if(!/^TILES\d{3}\.ART$/.test(name))continue;
   const header=dvbuf(file.offset,16);const meta=new DataView(header.buffer);const begin=meta.getInt32(8,true),end=meta.getInt32(12,true);
   const count=end-begin+1;if(count<1||count>1024)continue;
   const wanted=tiles.filter(id=>id>=begin&&id<=end&&!out.has(id));if(!wanted.length)continue;
   const m=dvbuf(file.offset+16,count*8);const metaDv=new DataView(m.buffer);let pixelPos=file.offset+16+count*8;
   for(let i=0;i<count;i++){
    const w=metaDv.getUint16(i*2,true),h=metaDv.getUint16(count*2+i*2,true);const len=w*h;const tile=begin+i;
    if(wanted.includes(tile)&&w>0&&h>0&&w<=512&&h<=512&&pixelPos+len<=file.offset+file.len){
     const pixels=dvbuf(pixelPos,len);const c=document.createElement('canvas');c.width=w;c.height=h;
     const ctx=c.getContext('2d'),im=ctx.createImageData(w,h);
     for(let x=0;x<w;x++)for(let y=0;y<h;y++){
      const ix=pixels[x*h+y];const at=(y*w+x)*4;
      for(let k=0;k<3;k++)im.data[at+k]=Math.min(255,pal[ix*3+k]*4);
      im.data[at+3]=ix===255?0:255;
     }
     ctx.putImageData(im,0,0);out.set(tile,c.toDataURL('image/png'));
    }
    pixelPos+=len;
   }
  }
  return out;
 }finally{fs.close(s)}
}
})();

window.DX33_State=()=>({auto:!!window.Module?._DukeX_GetAutoMode?.(),native:!!window.Module?._DukeX_V33Marker,bolts:window.Module?._DukeX_V33BoltCount?.()??null,shots:window.Module?._DukeX_V33BoltsFired?.()??null,hits:window.Module?._DukeX_V33BoltDamageHits?.()??null,bloom:window.DX33Glow?.getStatus()});
