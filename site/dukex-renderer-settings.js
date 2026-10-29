// Duke X public website settings: Classic by default, experimental Three.js by opt-in.
(function () {
  'use strict';
  const input = document.getElementById('dukex-experimental-renderer');
  const info = document.getElementById('dukex-renderer-setting-status');
  if (!input) return;
  function message(t) { if (info) info.textContent = t; }
  let stored = 'classic';
  try { stored = localStorage.getItem('dukex-renderer') || 'classic'; } catch (_) {}
  input.checked = stored === 'three';
  if (!window.WebGL2RenderingContext) {
    input.checked = false;
    input.disabled = true;
    message('WebGL2 unavailable: Classic renderer only.');
    return;
  }
  message(input.checked ? 'Experimental GPU renderer selected.' : 'Classic (recommended) is active.');
  input.addEventListener('change', async function () {
    const experimental = Boolean(input.checked);
    try { localStorage.setItem('dukex-renderer', experimental ? 'three' : 'classic'); } catch (_) {}
    try {
      if (typeof window.DukeXRequestExperimentalRenderer === 'function') {
        await window.DukeXRequestExperimentalRenderer(experimental);
      } else if (window.DukeXRenderer && typeof window.DukeXRenderer.setMode === 'function') {
        window.DukeXRenderer.setMode(experimental ? 'three' : 'classic', true);
      } else if (experimental) {
        throw Error('Renderer loader is unavailable in this build');
      }
      message(experimental ? 'Experimental Three.js selected. The engine switches when gameplay is ready.' : 'Classic renderer selected.');
    } catch (e) {
      input.checked = false;
      try { localStorage.setItem('dukex-renderer', 'classic'); } catch (_) {}
      try { window.DukeXRenderer?.setMode?.('classic', true); } catch (_) {}
      message('Experimental renderer failed; Classic restored. ' + String(e.message || e));
    }
  });
}());
