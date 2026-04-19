# Threading Model

**Status:** Phase 1.1 complete — initial survey only. Full analysis pending Phase 1.3+.
**Last verified:** 2026-04-18

---

## Current State (as of Phase 1.1)

The engine (`StarWarsG.exe`) imports extensive Win32 threading primitives:
`CreateThread`, `TlsAlloc/Get/Set/Free`, `CriticalSection`, `WaitForSingleObject`, `Mutex`, `Event`.

TLS (Thread Local Storage) directory is present — at least one thread-local variable is defined at link time.

**This is NOT a naive single-threaded engine.** Some threading infrastructure exists. The extent and purpose of that threading is unknown until Phase 1.3.

## Known Threading Infrastructure (Phase 1.1)

| Component | Evidence | Status |
|-----------|----------|--------|
| Win32 CreateThread | Imported in StarWarsG.exe | Confirmed import, usage TBD |
| TLS | Directory present, TlsAlloc imported | Active use confirmed, locations TBD |
| Critical sections | InitializeCriticalSectionEx imported | Active use confirmed, locations TBD |
| Intel TBB | tbbR.dll imported by swfoc.exe (launcher only) | Launcher-only, engine does not use TBB directly |

## TODO

- [ ] Phase 1.3: Locate main loop, identify which threads exist at startup
- [ ] Phase 1.3: Map TLS slot usage — what data is thread-local?
- [ ] Phase 2: For each subsystem, determine if it runs on main thread or a worker
- [ ] Phase 3: Design threading architecture based on Phase 2 findings
