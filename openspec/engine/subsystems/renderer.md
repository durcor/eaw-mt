# Renderer

**Status:** Phase 2 complete — flush entry point and call chain confirmed.
**Last verified:** 2026-04-19

---

## API

- **Direct3D 9** via `d3d9.dll` + `d3dx9_43.dll`
- `Direct3DCreate9` imported — standard D3D9 device creation
- D3DX9 utilities used for: matrix ops, texture loading, effect compilation, surface save/restore

## Renderer Namespace

All renderer classes use the `al` prefix (Alamo engine renderer):

| Class | Description |
|-------|-------------|
| `alGraphicsDriver` | Top-level D3D device wrapper |
| `alD3dEffect` | HLSL effect/shader wrapper |
| `alD3dTerrainPatch` | Terrain geometry patch |
| `alD3dWaterRenderTask` | Water surface render task |
| `alD3dSimpleEngineParam` | Shader parameter block |
| Various `alD3dVertexType<T>` | Typed vertex buffer layouts |

## Render Task System

The renderer uses a **task queue architecture** — render work is accumulated as task objects then flushed. This is the natural insertion point for render thread separation (Model A).

### Known Task Types

| Task Class | Description |
|-----------|-------------|
| `alRenderTask` | Base class |
| `ValMeshRenderTask` | Opaque mesh draw |
| `ValD3dTerrainRenderTask` | Terrain tile |
| `ValD3dTerrainDecorationRenderTask` | Terrain decoration |
| `ValD3dWaterRenderTask` | Water surface |
| `ValParticleGroupRenderTask` | Particle emitter batch |
| `ValSimpleRenderTask` | Simple geometry (debug, UI) |
| `Val3DCableRenderTask` | Cable/rope physics mesh |
| `ValLaserBeamRenderTask` / `ValLaserBeamFlushTask` | Laser FX |
| `ValLaserKiteRenderTask` / `ValLaserKiteFlushTask` | Laser kite FX |
| `ValBlobFlushTask` | Blob shadow |
| `ValLensFlareTask` | Lens flare |
| `ValSplineRendererRenderTask` | Spline path |
| `ValLightningBoltRenderTask` | Lightning |

Tasks are stored in `MultiLinkedListClass<alRenderTask>`.

The "Flush" task variants (`ValLaserBeamFlushTask`, `ValLaserKiteFlushTask`, `ValBlobFlushTask`) confirm a **two-phase pattern**:
1. **Accumulate** — game logic pushes render tasks onto the list each frame
2. **Flush/Submit** — renderer drains the list and issues D3D draw calls

This maps directly to **Model A** (render thread separation): the flush phase can be moved to a dedicated render thread consuming a double-buffered task list.

## Error Strings

```
*** WARNING *** alGraphicsDriver::Load_Texture - Failed to find texture (tga/dds) %s
*** WARNING *** alD3dEffect::Load - Failed to load effect %s [%s]
*** WARNING *** alD3dEffect::Load - Shader Compile Errors detected on effect %s [%s]
render task for %s exceeded the dynamic geometry limits (%d pverts, %d lverts, %d indices)
```

The geometry limit message confirms render tasks carry vertex/index budget metadata.

## Render Flush Entry Point

- **RVA:** `0x180dc0`
- **Size:** 2078 bytes
- **Signature (inferred):** `void FUN_140180dc0(alGraphicsDriver *driver, uint pass_mask)`

### Call Chain (Phase 2 — confirmed)

```
WinMain game loop
  └─ FUN_0x180d90  (32 bytes, thin wrapper)
       ├─ FUN_140180dc0(param_1, 0xbff)   ← render flush, 12 passes
       └─ FUN_140149650(param_1)           ← post-flush (present / swap)
```

`FUN_0x180d90` is the only caller of `FUN_0x180dc0`. It passes bitmask `0xbff` (all passes except bit 10),
then calls `FUN_140149650` which is the present/swap-chain step. The wrapper is called from the scene
manager / always-path section of the main loop (not gated on game-active).

### Flush Structure

12 render passes per frame (`local_68 = 0xc` countdown loop), one per render pass type. For each pass:

1. Check which passes are active via `pass_mask` bitmask
2. Call pass vtable `[0x40]` — begin pass / set render target
3. Iterate the render task linked list (at `driver + 0xd85e * 8`):
   - Accumulate tasks into a batch array (`DAT_140a6fd98`)
   - Geometry budget check: flush via `FUN_140183e80(driver, pverts, lverts, indices)` when 0xfffd limit approached
   - For each task: call `vftable[0x10](task, driver)` — Execute/Draw
4. Call `FUN_140183e80` for remaining batch
5. Call pass vtable `[0x48]`, `[0x50]`, `[0x58]` — draw calls / end pass / post-pass

After all 12 passes: handle scene texture + distortion texture for post-processing
(`FUN_1401b0280(…,"SceneTexture",…)` / `"DistortionTexture"`).

The geometry limit error string `"ERROR: render task for %s exceeded the dynamic geometry limits"` is
produced here when a task's pverts/lverts/indices exceeds 0xffff.

## Parallelization Assessment

- **Rating:** High (blueprint Model A — lowest risk)
- **Approach:** Double-buffer render task list; game logic thread writes, render thread reads+flushes
- **Blocker resolved:** `FUN_0x180d90` is called from the always-path of the main loop; the accumulate phase (game-active path) runs first, then flush. Fully serialized — no current pipelining.
- **D3D9 constraint:** D3D9 device calls must remain on the thread that created the device — render thread must own D3D context
- **Model A insertion point:** Between the game-active accumulate phase and the `FUN_0x180d90` flush call. Double-buffer the `MultiLinkedListClass<alRenderTask>` list; main thread writes, render thread drains.
