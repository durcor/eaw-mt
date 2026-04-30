// Phase5LuaHookFn.java
// Decompile FUN_1402531f0 — the Lua debug hook function the game installs
// via lua_sethook(L, FUN_1402531f0, LUA_MASKCOUNT, count).
// We need to know: does it call lua_yield, raise lua_error, set a flag,
// or do something else? This determines whether cooperative script suspension
// is already available and what mechanism it uses.
//
// Also decompile the callers of FUN_140251360/FUN_140251650/FUN_1402517b0
// (the install/uninstall functions) to understand when this pause is triggered.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5LuaHookFn extends GhidraScript {

    static final long[] TARGETS = {
        0x2531f0L,  // FUN_1402531f0 — the actual debug hook callback
        0x251360L,  // FUN_140251360 — install hook on one entity (or all if -1)
        0x251650L,  // FUN_140251650 — install hook on entity's main+thread states
        0x2517b0L,  // FUN_1402517b0 — uninstall hook
    };

    static final String[] LABELS = {
        "lua_hook_callback(2531f0)",
        "install_hook_entity_or_all(251360)",
        "install_hook_entity(251650)",
        "uninstall_hook(2517b0)",
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5LuaHookFn.log"));

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

        // Callers of install_hook_entity (251650) and uninstall_hook (2517b0)
        // to understand when the game pauses/unpauses Lua
        long[] xrefTargets = { 0x251650L, 0x2517b0L, 0x251360L };
        String[] xrefLabels = { "install_hook_entity(251650)", "uninstall_hook(2517b0)", "install_hook_all(251360)" };
        for (int i = 0; i < xrefTargets.length; i++) {
            out.printf("=== CALLERS OF %s ===\n\n", xrefLabels[i]);
            Address tgt = base.add(xrefTargets[i]);
            for (ghidra.program.model.symbol.Reference ref :
                    currentProgram.getReferenceManager().getReferencesTo(tgt)) {
                if (!ref.getReferenceType().isCall()) continue;
                Address callSite = ref.getFromAddress();
                Function caller = getFunctionContaining(callSite);
                long callerRva = callSite.getOffset() - base.getOffset();
                out.printf("  Call from RVA 0x%x  in  %s  (RVA 0x%x, %d bytes)\n",
                    callerRva,
                    caller != null ? caller.getName() : "???",
                    caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    caller != null ? (int)caller.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5LuaHookFn.log");
    }
}
