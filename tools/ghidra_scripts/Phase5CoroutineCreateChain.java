// Phase5CoroutineCreateChain.java
// Decompile FUN_14038cf30 (battle unit coroutine creation, RVA 0x38cf30) and
// trace its caller chain to find:
//   - when battle unit coroutines are first enqueued
//   - what entity type/state triggers creation
//   - whether creation happens at loading boundary or battle mode entry
//
// Also decompile FUN_1405f7090 (RVA 0x5f7090) — the other coroutine creation
// site that's near the simple Pump_Threads wrapper at 0x5f8310. That wrapper's
// caller (FUN_140725bc0) may be a loading-time or transition-time path.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5CoroutineCreateChain extends GhidraScript {

    static final long[] PRIMARY = {
        0x38cf30L,  // FUN_14038cf30 — coroutine creation for battle unit AI
        0x5f7090L,  // FUN_1405f7090 — other coroutine creation near simple wrapper
        0x725bc0L,  // FUN_140725bc0 — caller of the 0x5f8310 Pump_Threads wrapper
    };

    // Second-level: callers of each primary target
    static final long[] SECONDARY = {
        // callers of FUN_14038cf30 will be listed via xrefs
        // callers of FUN_1405f7090 will be listed via xrefs
        // callers of FUN_140725bc0 will be listed via xrefs
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5CoroutineCreateChain.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        for (long rva : PRIMARY) {
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
                int lim = Math.min(lines.length, 100);
                for (int i = 0; i < lim; i++) out.println(lines[i]);
                if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
            } else {
                out.println("  [decompile failed: " + dr.getErrorMessage() + "]");
            }
            out.println();

            // Xrefs to this function
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

        // Now decompile the callers of FUN_14038cf30 to find the entity spawn context
        out.println("=== CALLERS OF FUN_14038cf30 (decompile each) ===\n");
        Address target38cf30 = base.add(0x38cf30L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(target38cf30)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            if (caller == null) continue;
            long cRva = caller.getEntryPoint().getOffset() - base.getOffset();
            int sz = (int) caller.getBody().getNumAddresses();
            out.printf("--- Caller RVA 0x%x  %s  (%d bytes) ---\n", cRva, caller.getName(), sz);
            DecompileResults dr = di.decompileFunction(caller, 60, monitor);
            if (dr.decompileCompleted()) {
                String[] lines = dr.getDecompiledFunction().getC().split("\n");
                int lim = Math.min(lines.length, 80);
                for (int i = 0; i < lim; i++) out.println(lines[i]);
                if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
            } else {
                out.println("  [decompile failed]");
            }
            // xrefs to this caller
            out.printf("\n  -- Callers of RVA 0x%x --\n", cRva);
            for (ghidra.program.model.symbol.Reference r2 :
                    currentProgram.getReferenceManager()
                        .getReferencesTo(caller.getEntryPoint())) {
                if (!r2.getReferenceType().isCall()) continue;
                Address cs2 = r2.getFromAddress();
                Function c2 = getFunctionContaining(cs2);
                long c2Rva = cs2.getOffset() - base.getOffset();
                out.printf("    RVA 0x%x  in  %s  (fn RVA 0x%x, %d bytes)\n",
                    c2Rva,
                    c2 != null ? c2.getName() : "???",
                    c2 != null ? c2.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    c2 != null ? (int)c2.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        // Decompile FUN_140725bc0 caller chain
        out.println("=== CALLERS OF FUN_140725bc0 (decompile each) ===\n");
        Address target725bc0 = base.add(0x725bc0L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(target725bc0)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            if (caller == null) continue;
            long cRva = caller.getEntryPoint().getOffset() - base.getOffset();
            int sz = (int) caller.getBody().getNumAddresses();
            out.printf("--- Caller RVA 0x%x  %s  (%d bytes) ---\n", cRva, caller.getName(), sz);
            DecompileResults dr = di.decompileFunction(caller, 60, monitor);
            if (dr.decompileCompleted()) {
                String[] lines = dr.getDecompiledFunction().getC().split("\n");
                int lim = Math.min(lines.length, 60);
                for (int i = 0; i < lim; i++) out.println(lines[i]);
                if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
            } else {
                out.println("  [decompile failed]");
            }
            out.printf("\n  -- Callers of RVA 0x%x --\n", cRva);
            for (ghidra.program.model.symbol.Reference r2 :
                    currentProgram.getReferenceManager()
                        .getReferencesTo(caller.getEntryPoint())) {
                if (!r2.getReferenceType().isCall()) continue;
                Address cs2 = r2.getFromAddress();
                Function c2 = getFunctionContaining(cs2);
                long c2Rva = cs2.getOffset() - base.getOffset();
                out.printf("    RVA 0x%x  in  %s  (fn RVA 0x%x, %d bytes)\n",
                    c2Rva,
                    c2 != null ? c2.getName() : "???",
                    c2 != null ? c2.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    c2 != null ? (int)c2.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5CoroutineCreateChain.log");
    }
}
