// Phase5PumpThreadsCallers.java
// Decompile the 5 Pump_Threads callers that are NOT the battle game-loop path
// (FUN_1403a6b80). Goal: determine which, if any, are called during the loading
// screen phase — which would mean Lua AI coroutines are already initialized before
// the battle becomes player-visible (enabling "pre-warm" of the 14s stall).
//
// Pump_Threads callers from Phase5LuaThreadIsolation:
//   0x1dc60  FUN_14001dc60
//   0x488660 FUN_140488660
//   0x52f720 FUN_14052f720
//   0x4d5a70 FUN_1404d5a70
//   0x5f8310 FUN_1405f8310
//   0x3a6b80 FUN_1403a6b80  ← known battle-loop path (skip full decompile)
//
// For each: decompile the caller, list ITS callers (xrefs), and decompile the
// first 30 lines to understand the calling context (loading screen vs. gameplay).

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.*;

public class Phase5PumpThreadsCallers extends GhidraScript {

    static final long[] CALLER_RVAS = {
        0x1dc60L,   // FUN_14001dc60
        0x488660L,  // FUN_140488660
        0x52f720L,  // FUN_14052f720
        0x4d5a70L,  // FUN_1404d5a70
        0x5f8310L,  // FUN_1405f8310
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5PumpThreadsCallers.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();

        for (long rva : CALLER_RVAS) {
            Address addr = base.add(rva);
            Function fn = getFunctionAt(addr);

            out.printf("=== RVA 0x%x ===\n", rva);
            if (fn == null) {
                out.printf("  [no function]\n\n");
                continue;
            }
            int sz = (int) fn.getBody().getNumAddresses();
            out.printf("  Name: %s  Size: %d bytes\n\n", fn.getName(), sz);

            // Full decompile (up to 80 lines)
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

            // Who calls THIS function?
            out.printf("  -- Callers of RVA 0x%x --\n", rva);
            for (ghidra.program.model.symbol.Reference ref :
                    currentProgram.getReferenceManager().getReferencesTo(addr)) {
                if (!ref.getReferenceType().isCall()) continue;
                Address cs = ref.getFromAddress();
                Function caller = getFunctionContaining(cs);
                long cRva = cs.getOffset() - base.getOffset();
                out.printf("    Call from RVA 0x%x  in  %s  (RVA 0x%x, %d bytes)\n",
                    cRva,
                    caller != null ? caller.getName() : "???",
                    caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    caller != null ? (int)caller.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        // Also check: what is FUN_140244740 (coroutine creation) called from?
        // This tells us WHEN coroutines are enqueued — loading vs. battle start.
        out.println("=== CALLERS OF coroutine_create (FUN_140244740, RVA 0x244740) ===\n");
        Address coroCreate = base.add(0x244740L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(coroCreate)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            long cRva = cs.getOffset() - base.getOffset();
            out.printf("  Call from RVA 0x%x  in  %s  (RVA 0x%x, %d bytes)\n",
                cRva,
                caller != null ? caller.getName() : "???",
                caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                caller != null ? (int)caller.getBody().getNumAddresses() : -1);
        }
        out.println();

        // And FUN_1402490c0 (the thread-table resize called during coroutine enqueue)
        // as a secondary xref check
        out.println("=== CALLERS OF thread_table_resize (FUN_1402490c0, RVA 0x2490c0) ===\n");
        Address tblResize = base.add(0x2490c0L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(tblResize)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            long cRva = cs.getOffset() - base.getOffset();
            out.printf("  Call from RVA 0x%x  in  %s  (RVA 0x%x, %d bytes)\n",
                cRva,
                caller != null ? caller.getName() : "???",
                caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                caller != null ? (int)caller.getBody().getNumAddresses() : -1);
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5PumpThreadsCallers.log");
    }
}
