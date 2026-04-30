// Phase5LuaThreadIsolation.java
// Decompile the game's existing lua_sethook callers and the coroutine
// manager (FUN_1407b9920) to determine whether entity lua_States are
// truly isolated (safe for worker thread offload of pumpe calls).
//
// Key question: do coroutines share a global lua_State or is each entity's
// *(entity+0x58) an independent state with no cross-entity references?

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5LuaThreadIsolation extends GhidraScript {

    static final long[] TARGETS = {
        0x251360L,  // FUN_140251360 — lua_sethook caller (pair install/uninstall)
        0x251650L,  // FUN_140251650 — lua_sethook caller (pair install/uninstall)
        0x2517b0L,  // FUN_1402517b0 — lua_sethook caller (pair install/uninstall)
        0x244740L,  // FUN_140244740 — lua_sethook caller
        0x7b9920L,  // FUN_1407b9920 — the only caller of lua_resume (thunk path)
        0x247700L,  // FUN_140247700 — called twice per slot before lua_resume in Pump_Threads
    };

    static final String[] LABELS = {
        "lua_sethook_caller_251360",
        "lua_sethook_caller_251650",
        "lua_sethook_caller_2517b0",
        "lua_sethook_caller_244740",
        "lua_resume_wrapper(7b9920)",
        "pump_threads_prep(247700)",
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5LuaThreadIsolation.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        for (int idx = 0; idx < TARGETS.length; idx++) {
            long rva = TARGETS[idx];
            String label = LABELS[idx];
            Address addr = base.add(rva);
            Function fn = getFunctionAt(addr);

            out.printf("=== %s (RVA 0x%x) ===\n", label, rva);
            if (fn == null) {
                out.printf("  [no function at RVA 0x%x]\n\n", rva);
                continue;
            }
            int sz = (int) fn.getBody().getNumAddresses();
            out.printf("  Name: %s  Size: %d bytes\n\n", fn.getName(), sz);

            DecompileResults dr = di.decompileFunction(fn, 60, monitor);
            if (dr.decompileCompleted()) {
                out.println(dr.getDecompiledFunction().getC());
            } else {
                out.println("  [decompile failed: " + dr.getErrorMessage() + "]");
            }
            out.println();
        }

        // Callers of Pump_Threads inner (FUN_140247a90) — to understand when
        // coroutines are enqueued and whether they could be pre-run
        out.println("=== CALLERS OF Pump_Threads (0x247a90) ===\n");
        Address pumpeAddr = base.add(0x247a90L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(pumpeAddr)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address callSite = ref.getFromAddress();
            Function caller = getFunctionContaining(callSite);
            long callerRva = callSite.getOffset() - base.getOffset();
            out.printf("  Call from RVA 0x%x  in  %s  (RVA 0x%x)\n",
                callerRva,
                caller != null ? caller.getName() : "???",
                caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L);
        }
        out.println();

        // Callers of FUN_140247700 (per-slot prep in Pump_Threads)
        // to understand what "enqueuing a coroutine" looks like
        out.println("=== CALLERS OF pump_threads_prep (0x247700) ===\n");
        Address prepAddr = base.add(0x247700L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(prepAddr)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address callSite = ref.getFromAddress();
            Function caller = getFunctionContaining(callSite);
            long callerRva = callSite.getOffset() - base.getOffset();
            out.printf("  Call from RVA 0x%x  in  %s\n",
                callerRva,
                caller != null ? caller.getName() : "???");
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5LuaThreadIsolation.log");
    }
}
