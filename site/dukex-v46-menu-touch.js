/* Duke X V46.1: actual engine-drawn character cards are clickable in canvas.
   No fake/duplicate character-selection overlay. The engine owns selection,
   mode transitions, character stats and map loading. */
(()=>{'use strict';
 const get=(name,...args)=>{try{const f=window.Module?.['_'+name];return typeof f==='function'?f(...args):0}catch(_){return 0}};
 const root=document.getElementById('duke-x-controls');
 const canvas=document.getElementById('canvas');
 if(!canvas){console.warn('[DX46.1] Game canvas missing; native touch picker unavailable');return}
 const css=document.createElement('style');css.id='dx461-pregame-control-guard';
 // The older V27 pads are direct body children with display:block!important;
 // simply toggling dx-in-game on #duke-x-controls NEVER hid them.
 css.textContent=`#dx23-move,#dx23-aim{display:none!important;pointer-events:none!important}
 body.dx461-playing #dx23-move,body.dx461-playing #dx23-aim{display:block!important;pointer-events:auto!important}
 body:not(.dx461-playing) #duke-x-controls #dx-move-pad,
 body:not(.dx461-playing) #duke-x-controls #dx-aim-pad,
 body:not(.dx461-playing) #duke-x-controls #dx-move-zone,
 body:not(.dx461-playing) #duke-x-controls #dx-aim-zone,
 body:not(.dx461-playing) #duke-x-controls #dx-field-tools,
 body:not(.dx461-playing) #duke-x-controls #dx-field-launch{display:none!important;pointer-events:none!important}
 body.dx461-character-select #canvas{touch-action:none;cursor:pointer}
 body.dx461-character-select #dx46-native-picks{display:none!important}`;
 document.head.append(css);
 let premap=false,playing=false;
 function poll(){
   const m=window.Module,mode=get('DukeX_ProbeGameMode');
   const hasMode=typeof m?._DukeX_ProbeGameMode==='function';
   const picker=get('DukeX_V46PremapActive')===1;
   const game=hasMode && (mode&4)!==0 && (mode&1)===0 && (mode&8)===0 && !picker;
   const touchEnabled=root?.classList.contains('dx-v19-touch')===true;
   const modal=root?.classList.contains('dx-overlay-open')===true;
   const active=game && touchEnabled && !modal && !document.hidden && !document.body.classList.contains('dx-game-loading');
   if(playing&&!active)window.DX46_ResetTouchInput?.();
   premap=picker;playing=active;
   document.body.classList.toggle('dx461-character-select',picker);
   document.body.classList.toggle('dx461-playing',active);
   root?.classList.toggle('dx-in-game',game);
 }
 poll();setInterval(poll,75);
 // Pixel mapping is intentionally based on the actual displayed canvas,
 // not window.innerWidth/Height; supports letterbox, foldables, zoom and FOV.
 function toNative(e){
   const r=canvas.getBoundingClientRect();
   if(!(r.width>0&&r.height>0))return null;
   const nx=(e.clientX-r.left)/r.width,ny=(e.clientY-r.top)/r.height;
   if(nx<0||nx>=1||ny<0||ny>=1)return null;
   return [Math.floor(nx*320),Math.floor(ny*200)];
 }
 let down=null;
 canvas.addEventListener('pointerdown',e=>{
   if(!premap)return;
   down={id:e.pointerId,x:e.clientX,y:e.clientY};
   if(e.cancelable)e.preventDefault();
   e.stopImmediatePropagation();
 },{capture:true,passive:false});
 canvas.addEventListener('pointerup',e=>{
   if(!premap||!down||down.id!==e.pointerId)return;
   const dx=e.clientX-down.x,dy=e.clientY-down.y;down=null;
   if(dx*dx+dy*dy>400)return;
   const p=toNative(e);
   if(!p)return;
   const result=get('DukeX_V461TapCharacter',p[0],p[1]);
   if(result>0){
     if(e.cancelable)e.preventDefault();
     e.stopImmediatePropagation();
     if(result===2)document.body.classList.add('dx-game-loading');
     // Release previous joystick state immediately. Native controls return
     // automatically only when MODE_GAME is active and MODE_MENU is absent.
     window.DX46_ResetTouchInput?.();
   }
 },{capture:true,passive:false});
 canvas.addEventListener('pointercancel',()=>{down=null},true);
 // Do not request pointer lock from the engine canvas while a native menu is open.
 canvas.addEventListener('click',e=>{if(premap){e.preventDefault();e.stopImmediatePropagation()}},true);
 // No post-confirm spinner trapping game input; clear loading once the
 // engine reports active gameplay, or the picker is opened again.
 setInterval(()=>{
   const mode=get('DukeX_ProbeGameMode');
   if(((mode&4)!==0&&(mode&1)===0&&!premap) || premap)
     document.body.classList.remove('dx-game-loading');
 },125);
 // Rare boon/bane screen remains engine-owned. Small buttons are provided
 // only as a fallback when the native engine requests an active choice.
 const old=document.getElementById('dx46-native-picks');if(old)old.remove();
 const twistHost=document.createElement('section');twistHost.id='dx461-twists';
 twistHost.setAttribute('aria-label','Choose a run boon or bane');
 twistHost.style.cssText='display:none;position:fixed;left:4vw;right:4vw;bottom:env(safe-area-inset-bottom,10px);z-index:2147482200;background:#111b29f2;border:2px solid #ffc477;border-radius:8px;padding:12px;color:#f9d78e;touch-action:pan-y';
 twistHost.innerHTML='<div style="font:bold 14px sans-serif;margin-bottom:10px">RUN MUTATION — CHOOSE YOUR FATE</div><div style="display:flex;gap:8px" id="dx461-twist-buttons"></div>';
 document.body.append(twistHost);
 let signature='';
 setInterval(()=>{
   const pending=get('DukeX_V46TwistActive')!==0 && !premap;
   twistHost.style.display=pending?'block':'none';
   if(!pending){signature='';return}
   const sig=[1,2,3].map(i=>get('DukeX_V46TwistType',i)+':'+get('DukeX_V46TwistStrength',i)).join('/');
   if(sig===signature)return;signature=sig;
   const container=twistHost.querySelector('#dx461-twist-buttons');container.replaceChildren();
   for(let i=1;i<=3;i++){
     const b=document.createElement('button');const type=get('DukeX_V46TwistType',i);
     b.textContent=i+' · '+(type<0?'BANE':'BOON')+' '+Math.abs(type)+' / x'+get('DukeX_V46TwistStrength',i);
     b.style.cssText='flex:1;min-height:55px;background:#3b2930;color:#ffe0a8;border:1px solid #f4ad6b;border-radius:6px';
     b.addEventListener('click',()=>get('DukeX_V46ChooseTwist',i));container.append(b);
   }
 },200);
 document.addEventListener('keydown',e=>{
   if(get('DukeX_V46TwistActive') && /^Digit[123]$/.test(e.code)){
     if(get('DukeX_V46ChooseTwist',Number(e.code.slice(-1)))){e.preventDefault();e.stopImmediatePropagation()}
   }
 },true);
})();
