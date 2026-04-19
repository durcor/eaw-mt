# Renderer

**Status:** Phase 1.2 complete — class/task structure identified. RVAs pending Phase 1.3.
**Last verified:** 2026-04-18

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

## Entry Point / Tick RVA

**TODO** — pending Phase 1.3.

## Parallelization Assessment

- **Rating:** High (blueprint Model A — lowest risk)
- **Approach:** Double-buffer render task list; game logic thread writes, render thread reads+flushes
- **Blocker:** Must verify game logic and render are currently serialized (not already pipelined)
- **D3D9 constraint:** D3D9 device calls must remain on the thread that created the device — render thread must own D3D context
