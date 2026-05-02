// Phase5EntitySpawnChain.java
// Trace the entity spawn / AI-init call chain up to the loading/battle boundary.
//
// From the previous script we know:
//   FUN_14038cf30 (AI init) is called by:
//     FUN_1403989a0 (entity reset + AI init, 2733 bytes) ← via FUN_140388b60
//     FUN_14039c3a0 (XML/save deserializer, 1671 bytes)  ← via FUN_1402b0a20
//
//   FUN_1403989a0 is called from:
//     FUN_140388b60 (1998 bytes)   ← this is the entity activate/spawn fn
//     FUN_14039fc90 (364 bytes)
//     FUN_14039ce40 (1900 bytes)
//
//   FUN_14039c3a0 is called from:
//     FUN_1402b0a20 (3976 bytes)   ← likely save/load deserializer
//
// Goal: decompile FUN_140388b60, FUN_1402b0a20, and their callers to determine
// whether these run during the loading screen or at battle mode entry.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5EntitySpawnChain extends GhidraScript {

    static final long[] TARGETS = {
        0x388b60L,  // FUN_140388b60 — entity spawn/activate (1998 bytes)
        0x2b0a20L,  // FUN_1402b0a20 — XML/save deserializer (3976 bytes)
        0x39fc90L,  // FUN_14039fc90 — other caller of FUN_1403989a0 (364 bytes)
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5EntitySpawnChain.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        for (long rva : TARGETS) {
            Address addr = base.add(rva);
            Function fn = getFunctionAt(addr);

            out.printf("=== RVA 0x%x ===\n", rva);
            if (fn == null) {
                out.printf("  [no function]\n\n");
                continue;
            }
            int sz = (int) fn.getBody().getNumAddresses();
            out.printf("  Name: %s  Size: %d bytes\n\n", fn.getName(), sz);

            DecompileResults dr = di.decompileFunction(fn, 60, monitor);
            if (dr.decompileCompleted()) {
                String[] lines = dr.getDecompiledFunction().getC().split("\n");
                int lim = Math.min(lines.length, 80);
                for (int i = 0; i < lim; i++) out.println(lines[i]);
                if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
            } else {
                out.println("  [decompile failed: " + dr.getErrorMessage() + "]");
            }
            out.println();

            // xrefs — who calls this function?
            out.printf("  -- Callers of RVA 0x%x --\n", rva);
            for (ghidra.program.model.symbol.Reference ref :
                    currentProgram.getReferenceManager().getReferencesTo(addr)) {
                if (!ref.getReferenceType().isCall()) continue;
                Address cs = ref.getFromAddress();
                Function caller = getFunctionContaining(cs);
                long cRva = cs.getOffset() - base.getOffset();
                out.printf("    RVA 0x%x  in  %s  (fn RVA 0x%x, %d bytes)\n",
                    cRva,
                    caller != null ? caller.getName() : "???",
                    caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    caller != null ? (int)caller.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        // The string at DAT_140866674 — the AI script name passed to coroutine_create
        // Dump 128 bytes from that address to identify the script
        out.println("=== DAT_140866674 (AI script name arg to coroutine_create) ===\n");
        Address scriptNameAddr = base.add(0x866674L);
        try {
            byte[] buf = new byte[128];
            for (int i = 0; i < 128; i++) {
                buf[i] = currentProgram.getMemory().getByte(scriptNameAddr.add(i));
                if (buf[i] == 0) { buf[i] = 0; break; }
            }
            StringBuilder sb = new StringBuilder();
            for (byte b : buf) {
                if (b == 0) break;
                sb.append((char)b);
            }
            out.printf("  String: \"%s\"\n\n", sb.toString());
        } catch (Exception e) {
            out.println("  [could not read: " + e.getMessage() + "]");
        }

        // What does FUN_1402437c0 (loads "Base_Definitions.lua") look like?
        out.println("=== FUN_1402437c0 — Lua script loader (called before coroutine_create) ===\n");
        Address luaLoader = base.add(0x2437c0L);
        Function luaLoaderFn = getFunctionAt(luaLoader);
        if (luaLoaderFn != null) {
            int sz = (int) luaLoaderFn.getBody().getNumAddresses();
            out.printf("  Name: %s  Size: %d bytes\n\n", luaLoaderFn.getName(), sz);
            DecompileResults dr = di.decompileFunction(luaLoaderFn, 60, monitor);
            if (dr.decompileCompleted()) {
                String[] lines = dr.getDecompiledFunction().getC().split("\n");
                int lim = Math.min(lines.length, 60);
                for (int i = 0; i < lim; i++) out.println(lines[i]);
                if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
            }
        }
        out.println();

        // Dump the string at DAT_140866674 — alternate approach via references
        out.println("=== String refs to DAT_140866674 ===\n");
        Address dat866674 = base.add(0x866674L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(dat866674)) {
            Address cs = ref.getFromAddress();
            Function fn2 = getFunctionContaining(cs);
            long cRva = cs.getOffset() - base.getOffset();
            out.printf("  Ref from RVA 0x%x  type=%s  in  %s\n",
                cRva, ref.getReferenceType(),
                fn2 != null ? fn2.getName() + " (fn 0x" + Long.toHexString(fn2.getEntryPoint().getOffset() - base.getOffset()) + ")" : "???");
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5EntitySpawnChain.log");
    }
}
