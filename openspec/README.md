# OpenSpec — EaW Engine Reverse Engineering

## Conventions

- All RVAs are relative to the binary's image base unless marked as VA.
- Confidence levels: **High** (confirmed by multiple methods), **Medium** (inferred, plausible), **Low** (guess).
- Primary analysis target: `binaries/original/StarWarsG.exe` (the actual game engine).
- `swfoc.exe` is the launcher/host process — analyzed separately.
- Dates are ISO 8601 (YYYY-MM-DD).

## Glossary

- **RVA** — Relative Virtual Address (offset from ImageBase)
- **VA** — Virtual Address (RVA + ImageBase = 0x140000000 for both binaries)
- **TBB** — Intel Threading Building Blocks
- **TLS** — Thread Local Storage
- **LAA** — Large Address Aware

## Primary Binary Summary

| Binary | Size | Arch | Role |
|--------|------|------|------|
| StarWarsG.exe | 12 MB | x86-64 | **Game engine — primary analysis target** |
| swfoc.exe | 1.7 MB | x86-64 | Launcher / host process (uses TBB) |
| sweaw.exe | 1.7 MB | x86-64 | Base EaW game launcher |
