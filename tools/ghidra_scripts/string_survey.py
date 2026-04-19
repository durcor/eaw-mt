#!/usr/bin/env python3
"""Phase 1.2 — String survey of StarWarsG.exe.

Scans .rdata and .data sections for printable ASCII strings (length >= 6),
then filters and reports hits against categories of interest.
"""
import sys
import re
import pefile

TARGET = "binaries/original/StarWarsG.exe"

CATEGORIES = {
    "threading": re.compile(
        r"thread|worker|job|fiber|queue|pool|task|mutex|semaphore|critical.section"
        r"|lock|synchron|async|spawn|dispatch|parallel|concurrent",
        re.IGNORECASE
    ),
    "lua": re.compile(
        r"lua|\.lua|lua_|luaL_|luaopen_|lua_State|lua_pcall|lua_call|lua_push"
        r"|Lua\s*\d|\[string|attempt to|stack traceback",
        re.IGNORECASE
    ),
    "source_paths": re.compile(
        r"(?:\.cpp|\.h|\.c|\.inl)\b|(?:/|\\\\)(?:src|source|code|engine|AI|Game|Script|Render|Map|Unit|Faction)"
        r"(?:/|\\\\)\w",
        re.IGNORECASE
    ),
    "eaw_assets": re.compile(
        r"swfoc|sweaw|corruption|petroglyph|empire.at.war|thrawn|galactic|alamo|"
        r"\.meg\b|\.alo\b|\.tga\b|\.dds\b|\.lua\b|gamedata|factionname",
        re.IGNORECASE
    ),
    "d3d_renderer": re.compile(
        r"d3d|direct3d|shader|hlsl|vertex.buffer|index.buffer|render.target"
        r"|texture|IDirect3D|D3DFMT|D3DPOOL|D3DUSAGE",
        re.IGNORECASE
    ),
    "network": re.compile(
        r"socket|connect|listen|send|recv|host|port|\bLAN\b|\bIP\b|lobby|peer|packet",
        re.IGNORECASE
    ),
}


def extract_strings(data, min_len=6):
    """Yield (offset, string) for all printable ASCII runs >= min_len."""
    pattern = re.compile(rb"[\x20-\x7e]{%d,}" % min_len)
    for m in pattern.finditer(data):
        yield m.start(), m.group().decode("ascii", errors="replace")


def main():
    pe = pefile.PE(TARGET, fast_load=False)

    hits = {cat: [] for cat in CATEGORIES}
    all_strings = []

    for section in pe.sections:
        name = section.Name.rstrip(b"\x00").decode(errors="replace")
        if name not in (".rdata", ".data", ".text"):
            continue
        data = section.get_data()
        base_rva = section.VirtualAddress
        for off, s in extract_strings(data):
            rva = base_rva + off
            all_strings.append((rva, s))
            for cat, pattern in CATEGORIES.items():
                if pattern.search(s):
                    hits[cat].append((rva, s))

    # Write full string list
    with open("logs/strings_all.txt", "w") as f:
        for rva, s in all_strings:
            f.write(f"0x{rva:08X}\t{s}\n")

    # Write per-category hits
    with open("logs/strings_categorized.txt", "w") as f:
        for cat, entries in hits.items():
            f.write(f"\n{'='*60}\n[{cat.upper()}] — {len(entries)} hits\n{'='*60}\n")
            for rva, s in entries:
                f.write(f"  0x{rva:08X}  {s}\n")

    # Print summary to stdout
    print(f"\nTotal strings extracted: {len(all_strings)}")
    print(f"\nCategory hit counts:")
    for cat, entries in hits.items():
        print(f"  {cat:<20} {len(entries):>5}")

    print(f"\n--- SOURCE PATH FRAGMENTS (most valuable) ---")
    for rva, s in hits["source_paths"][:80]:
        print(f"  0x{rva:08X}  {s}")

    print(f"\n--- LUA STRINGS ---")
    for rva, s in hits["lua"][:40]:
        print(f"  0x{rva:08X}  {s}")

    print(f"\n--- THREADING STRINGS ---")
    for rva, s in hits["threading"][:60]:
        print(f"  0x{rva:08X}  {s}")

    pe.close()


if __name__ == "__main__":
    main()
