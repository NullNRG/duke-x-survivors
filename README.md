# Duke X — WebAssembly browser edition

Public EDuke32/Duke X fan project with the V45.1 WebAssembly runtime and corresponding modified engine source. Retail and shareware Duke Nukem 3D GRP game data are **not bundled**. Visitors select their own legally obtained DUKE3D.GRP locally in their browser; no game data is uploaded.

**Play:** https://nullnrg.github.io/duke-x-survivors/

Classic rendering is the default. Experimental Three.js is opt-in via Settings → Display. The site is an experimental fan project, not affiliated with the rights holders. Public matchmaking is disabled.

## Build

Web artifacts: `site/eduke32.wasm`, `site/eduke32.js` and `site/` browser dependencies.
Engine source: `source/duke-x-survivors/upstream/eduke32-wasm/` including `GNUmakefile`, `Common.mak`, and `source/build/`.

Install an Emscripten SDK compatible with this EDuke32 fork; load its environment, then inside the engine source directory run:

```sh
make PLATFORM=EMSCRIPTEN EM_SINGLE_FILE=0 NETDUKE32=1 USE_LIBVPX=0 obj=obj-em -j1
```

Build invocation is documented; a fresh rebuild has not been independently verified. To test the frontend locally: `cd site && python3 -m http.server 8080`.

## Licenses and privacy

See `LICENSES/` and `site/credits.html` for third-party attribution and game-data restrictions. This public tree excludes historic debug logs, handoff reports, machine-specific paths, old installation scripts and private assets. **Deleting files in a later commit does not erase them from Git history.**
