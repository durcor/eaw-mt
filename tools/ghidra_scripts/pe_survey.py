#!/usr/bin/env python3
"""Phase 1.1 — PE Header Survey for EaW binaries."""

import sys
import hashlib
import pefile

THREADING_FUNCS = {
    "CreateThread", "CreateRemoteThread", "ExitThread", "SuspendThread",
    "ResumeThread", "TerminateThread", "GetCurrentThread", "GetCurrentThreadId",
    "SetThreadAffinityMask", "SetThreadPriority", "GetThreadPriority",
    "_beginthreadex", "_beginthread", "_endthreadex",
    "InitializeCriticalSection", "InitializeCriticalSectionEx",
    "EnterCriticalSection", "LeaveCriticalSection", "DeleteCriticalSection",
    "TryEnterCriticalSection",
    "CreateMutex", "CreateMutexEx", "OpenMutex", "ReleaseMutex",
    "CreateSemaphore", "ReleaseSemaphore",
    "CreateEvent", "SetEvent", "ResetEvent", "PulseEvent",
    "WaitForSingleObject", "WaitForSingleObjectEx",
    "WaitForMultipleObjects", "WaitForMultipleObjectsEx",
    "InterlockedIncrement", "InterlockedDecrement", "InterlockedExchange",
    "InterlockedCompareExchange",
    "InitializeSRWLock", "AcquireSRWLockExclusive", "ReleaseSRWLockExclusive",
    "AcquireSRWLockShared", "ReleaseSRWLockShared",
    "InitializeConditionVariable", "WakeConditionVariable",
    "SleepConditionVariableSRW", "SleepConditionVariableCS",
    "CreateThreadpool", "CreateThreadpoolWork", "SubmitThreadpoolWork",
    "TlsAlloc", "TlsGetValue", "TlsSetValue", "TlsFree",
    "FlsAlloc", "FlsGetValue", "FlsSetValue",
}


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()


def survey(path):
    print(f"\n{'='*70}")
    print(f"Binary: {path}")
    print(f"SHA-256: {sha256(path)}")
    print(f"{'='*70}")

    pe = pefile.PE(path, fast_load=False)

    # Architecture
    machine = pe.FILE_HEADER.Machine
    arch = "x86-64 (64-bit)" if machine == 0x8664 else \
           "x86-32 (32-bit)" if machine == 0x014c else \
           f"unknown (0x{machine:04X})"
    print(f"\n[ARCHITECTURE]")
    print(f"  Machine:      {arch}")
    print(f"  Subsystem:    {pe.OPTIONAL_HEADER.Subsystem}")
    print(f"  ImageBase:    0x{pe.OPTIONAL_HEADER.ImageBase:016X}")
    print(f"  EntryPoint:   RVA 0x{pe.OPTIONAL_HEADER.AddressOfEntryPoint:08X}")

    # Flags
    chars = pe.FILE_HEADER.Characteristics
    dll_chars = pe.OPTIONAL_HEADER.DllCharacteristics
    laa = bool(chars & 0x0020)
    print(f"\n[FLAGS]")
    print(f"  LAA (Large Address Aware): {'YES' if laa else 'NO'}")
    print(f"  ASLR (DynamicBase):        {'YES' if dll_chars & 0x0040 else 'NO'}")
    print(f"  NX (DEP):                  {'YES' if dll_chars & 0x0100 else 'NO'}")
    print(f"  64-bit App:                {'YES' if chars & 0x0020 == 0 and machine == 0x8664 else 'see arch'}")

    # Sections
    print(f"\n[SECTIONS]")
    print(f"  {'Name':<12} {'VSize':>10} {'RVA':>12} {'RawSize':>10}")
    for s in pe.sections:
        name = s.Name.rstrip(b'\x00').decode(errors='replace')
        print(f"  {name:<12} {s.Misc_VirtualSize:>10} 0x{s.VirtualAddress:>08X} {s.SizeOfRawData:>10}")

    # TLS directory
    print(f"\n[TLS DIRECTORY]")
    if hasattr(pe, 'DIRECTORY_ENTRY_TLS'):
        tls = pe.DIRECTORY_ENTRY_TLS.struct
        print(f"  PRESENT — StartAddressOfRawData: 0x{tls.StartAddressOfRawData:016X}")
    else:
        print(f"  NOT PRESENT")

    # Imports
    print(f"\n[IMPORTS]")
    threading_found = []
    d3d_dlls = []
    lua_dlls = []
    tbb_dlls = []

    if hasattr(pe, 'DIRECTORY_ENTRY_IMPORT'):
        for entry in pe.DIRECTORY_ENTRY_IMPORT:
            dll = entry.dll.decode(errors='replace').lower()
            funcs = []
            for imp in entry.imports:
                if imp.name:
                    funcs.append(imp.name.decode(errors='replace'))

            # Categorise
            if 'd3d' in dll:
                d3d_dlls.append(dll)
            if 'lua' in dll:
                lua_dlls.append(dll)
            if 'tbb' in dll:
                tbb_dlls.append(dll)

            thread_hits = [f for f in funcs if f in THREADING_FUNCS]
            if thread_hits:
                threading_found.append((dll, thread_hits))

            print(f"  {entry.dll.decode(errors='replace')}")
            for f in funcs:
                marker = " *** THREADING ***" if f in THREADING_FUNCS else ""
                print(f"    {f}{marker}")

    print(f"\n[SUMMARY — KEY FINDINGS]")
    print(f"  Architecture:     {arch}")
    print(f"  LAA:              {'YES' if laa else 'NO'}")

    print(f"\n  DirectX imports:  {d3d_dlls if d3d_dlls else 'NONE (may be dynamic load)'}")
    print(f"  Lua DLLs:         {lua_dlls if lua_dlls else 'NONE (may be statically linked)'}")
    print(f"  TBB DLLs:         {tbb_dlls if tbb_dlls else 'NONE'}")

    if threading_found:
        print(f"\n  THREADING PRIMITIVES FOUND:")
        for dll, funcs in threading_found:
            print(f"    In {dll}:")
            for f in funcs:
                print(f"      - {f}")
    else:
        print(f"\n  Threading primitives: NONE FOUND")

    pe.close()
    return {
        "arch": arch,
        "machine": machine,
        "laa": laa,
        "threading": threading_found,
        "d3d": d3d_dlls,
        "lua": lua_dlls,
        "tbb": tbb_dlls,
    }


if __name__ == "__main__":
    targets = sys.argv[1:] if len(sys.argv) > 1 else [
        "binaries/original/swfoc.exe",
        "binaries/original/StarWarsG.exe",
    ]
    for t in targets:
        try:
            survey(t)
        except Exception as e:
            print(f"ERROR processing {t}: {e}")
