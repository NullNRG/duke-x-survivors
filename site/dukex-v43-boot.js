// Duke X: lazy experimental backend. Classic costs no WebGL work until opt-in.
(function () {
  'use strict';
  let loading = null;
  async function loadRenderer() {
    if (window.DukeXRenderer) return window.DukeXRenderer;
    if (!window.WebGL2RenderingContext) throw Error('WebGL2 not supported');
    if (!loading) {
      loading = import('./dukex-v43-primary.mjs')
        .then(mod => mod.installDukeXPrimary())
        .catch(error => { loading = null; throw error; });
    }
    return loading;
  }
  window.DukeXRequestExperimentalRenderer = async function (enabled) {
    if (!enabled) {
      window.DukeXRenderer?.setMode?.('classic', true);
      return;
    }
    const renderer = await loadRenderer();
    renderer.setMode('three', true);
  };
  function boot() {
    let selected = false;
    try { selected = localStorage.getItem('dukex-renderer') === 'three'; } catch (_) {}
    if (selected) {
      window.DukeXRequestExperimentalRenderer(true).catch(e => {
        console.warn('[Duke X] Experimental renderer unavailable; using Classic:', e);
        try { localStorage.setItem('dukex-renderer', 'classic'); } catch (_) {}
        const input = document.getElementById('dukex-experimental-renderer');
        if (input) input.checked = false;
      });
    }
  }
  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', boot, {once:true});
  else boot();
}());
