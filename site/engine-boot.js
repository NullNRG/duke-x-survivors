/* Actual Emscripten EDuke32 boot preRun helper. Installed into upstream engine/index.html. */
(function(global){'use strict';
 const KEY='dukeX.mainGroup.v1';
 const MAX_BYTES=650*1024*1024;
 function showFailure(why){
   console.error('[Duke X boot]',why);
   const div=document.createElement('div');div.id='dukex-boot-error';
   Object.assign(div.style,{position:'fixed',inset:'0',zIndex:'99999',display:'grid',placeItems:'center',background:'#120e17',color:'#f5ebdc',font:'16px system-ui',padding:'24px'});
   div.innerHTML='<div style="max-width:700px"><h1 style="color:#ffc342">DUKE X: GRP NOT AVAILABLE</h1><p id="dukex-boot-reason"></p><a href="../index.html" style="display:inline-block;background:#ffc342;color:#170f17;padding:14px;text-decoration:none;border-radius:5px">RETURN TO GRP LIBRARY</a></div>';
   document.body.append(div);div.querySelector('#dukex-boot-reason').textContent=why;
 }
 function call(module,action,params={}){
   if(!global.ErryThang?.nativeCall)throw new Error('ErryThang bridge unavailable; app-file GRP cannot be restored.');
   const r=JSON.parse(global.ErryThang.nativeCall(JSON.stringify({module,action,params,requestId:global.crypto?.randomUUID?.()||'dx-boot'})));
   if(!r?.ok)throw new Error(r?.error?.message||`${module}.${action} failed`);
   return r.data||{};
 }
 function dbGet(key){return new Promise((resolve,reject)=>{
   if(!global.indexedDB){reject(Error('IndexedDB unavailable'));return;}
   const req=indexedDB.open('duke-x-assets-v1',1);
   req.onupgradeneeded=()=>{if(!req.result.objectStoreNames.contains('grps'))req.result.createObjectStore('grps');};
   req.onerror=()=>reject(req.error);
   req.onsuccess=()=>{const db=req.result;const txn=db.transaction('grps','readonly');const get=txn.objectStore('grps').get(key);
     get.onsuccess=()=>{db.close();resolve(get.result)};get.onerror=()=>{db.close();reject(get.error)};};
 });}
 function checksum(bytes){const table=new Uint32Array(256);for(let i=0;i<256;i++){let c=i;for(let j=0;j<8;j++)c=(c>>>1)^((c&1)?0xedb88320:0);table[i]=c>>>0;}
   let c=0xffffffff;for(let i=0;i<bytes.length;i++)c=(c>>>8)^table[(c^bytes[i])&255];return ((c^0xffffffff)>>>0).toString(16).padStart(8,'0');}
 async function readStored(sel){
   if(sel.location==='indexedDB'){const data=await dbGet(sel.idbKey);if(!data)throw Error('Saved browser GRP was removed.');return new Uint8Array(await data.arrayBuffer());}
   if(sel.location==='appPath'){
     const source={type:'appPath',path:sel.path};
     const stat=call('files','stat',{source,scope:'app',path:sel.path});
     if(Number(stat.size??stat.length)!==sel.bytes)throw Error('Stored GRP size mismatch. Please reimport.');
     const preview=call('content','previewUrl',{source});
     const url=preview.url||preview.previewUrl||preview.contentUrl;
     if(typeof url!=='string'||!url)throw Error('ErryThang did not supply a game-data content URL.');
     const response=await fetch(url,{cache:'no-store'});
     if(!response.ok)throw Error('Could not read saved GRP from app storage: HTTP '+response.status);
     return new Uint8Array(await response.arrayBuffer());
   }
   throw Error('Unknown saved GRP location type.');
 }
 function preRun(Module){
   let sel;try{sel=JSON.parse(localStorage.getItem(KEY)||'null');}catch{}
   if(!sel)return; // upstream shareware packaging remains available if no selection.
   // Multiplayer join/download selection is owned by upstream DukeNet; don't override host GRP.
   const qs=new URLSearchParams(global.location?.search||'');
   if(qs.has('join'))return;
   if(!Number.isSafeInteger(sel.bytes)||sel.bytes<16||sel.bytes>MAX_BYTES){showFailure('Saved GRP metadata is invalid.');throw Error('Bad GRP size');}
   Module.addRunDependency('dukex-saved-grp');
   // The engine reads the exact original GRP bytes, not a reconstruction or tiles conversion.
   (async()=>{
     const bytes=await readStored(sel);
     if(bytes.byteLength!==sel.bytes)throw Error('GRP size changed.');
     if(String.fromCharCode(...bytes.subarray(0,12))!=='KenSilverman')throw Error('Saved file is not a GRP.');
     if(checksum(bytes)!==sel.crc32)throw Error('GRP CRC32 mismatch. Reimport the original file.');
     Module.FS.writeFile('/DUKEX_MAIN.GRP',bytes);
     Module.arguments=Module.arguments||[];
     Module.arguments.push('-gamegrp','/DUKEX_MAIN.GRP');
     Module.removeRunDependency('dukex-saved-grp');
   })().catch(err=>{
     // Important: never silently boot a different Duke edition on asset failure.
     showFailure(err.message||String(err));
     // Keep the run dependency unresolved so game main() does not silently start shareware.
     // User can return to the GRP library and re-authorize/reimport.
   });
 }
 global.DukeXErry={preRun};
})(window);
