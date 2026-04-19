# Renderer

**Status:** Phase 4 Step 2 — render thread separation implemented (serialized baseline).
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

### Call Chain (confirmed)

```
WinMain game loop
  └─ thunk → FUN_140180d90  (RVA 0x180d90, 32 bytes, thin wrapper)
                ├─ FUN_140180dc0(param_1, 0xbff)   ← flush body, 12 passes
                └─ FUN_140149650(param_1)           ← drain/free all task lists
```

`FUN_140180d90` is the only caller of `FUN_140180dc0`. It passes bitmask `0xbff` (all 12 passes),
then calls `FUN_140149650` which destructs all render tasks and resets the 12 pass sentinels.

### WinMain Frame Structure (confirmed — Phase 4)

WinMain performs **two render flushes per frame** with no sim tick between them:

```
sim_tick (line 556)        ← game logic, writes render tasks
flush#1 (line 571)         ← FUN_140180d90 call #1
  [inter-flush work]       ← lines 571–626, modifies render task data
flush#2 (line 627)         ← FUN_140180d90 call #2
  [post-flush cleanup]     ← zeroes sentinel.next pointers
```

`FUN_140149650` (drain/free) must run between flush#1 and flush#2 in the original design.
It destructs all 12 pass lists and resets `sentinel.next = sentinel` on each pass.

### Render Task List Layout

12 pass sentinels at `driver + 0x6c2f0`, stride `0x48`, doubly-linked circular lists.
`sentinel.next == sentinel` means an empty pass list.

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

### Phase 4 Step 2 — Serialized render thread (implemented)

The winmm.dll proxy DLL hooks `FUN_140180d90` at RVA `0x180d90` via a 14-byte inline JMP. A dedicated render thread receives a kick event, calls the full trampoline (flush body + drain), then signals render_done. The main thread blocks on render_done before returning from `render_flush_hook`.

**Result:** Stable at 300+ frames. Profiling log available every 300 frames via OutputDebugString.

### Why true flush#1 async is blocked

Confirmed by disassembly at RVA `0x180e80` (FUN_140180dc0 +0xC0/+0xC9) and WinMain frame analysis:

1. **TOCTOU race in flush body:** `FUN_140180dc0` performs two loads of `sentinel.next` — one at `[R9+8]` (guard check) and one at `[RAX+8]` (actual load into RDI). The post-flush cleanup code (WinMain lines 627→next sim_tick) zeroes `sentinel.next` between these two reads, producing a null-deref at `[RDI+0x18]` (RVA `0x180e80`) or garbage read at `[RBX+0x5C]` (RVA `0x180e89`).

2. **Inter-flush data race:** WinMain lines 571–626 run concurrently with an async flush#1 and modify render task data read by the flush body. Confirmed empirically: async flush#1 produced visual flickering (lighting/texture corruption) before crashing at RVA `0x180637` (a task draw call reading `-1`).

**To enable true async:** double-buffer the 12 pass lists so that inter-flush WinMain writes land in a back-buffer while the render thread reads the front-buffer. Requires allocating a second `driver + 0x6c2f0` block and patching all task submission sites to target the back-buffer — significant surgery.

### D3D9 constraint

D3D9 device calls must remain on the thread that created the device. The render thread must own D3D context for the lifetime of the game.
