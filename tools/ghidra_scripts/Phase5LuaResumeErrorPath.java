// Phase5LuaResumeErrorPath.java
// Decompile FUN_140248d10 (the function in Pump_Threads that calls lua_resume)
// and FUN_140246940 (error handler branch) to understand what the game does
// when lua_resume returns non-zero (coroutine error/dead).
//
// Also decompile the lua_resume function itself (RVA 0x7bc310) to confirm
// its return value semantics.
//
// This determines whether lua_sethook + lua_error is safe:
//   - If game abandons AI → entity broken but stable
//   - If game retries with new coroutine → sethook causes infinite re-stall
//   - If game crashes → hard no

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5LuaResumeErrorPath extends GhidraScript {

    // RVAs to decompile — all identified from Phase5FindLuaVM output
    static final long[] TARGETS = {
        0x248d10L,  // FUN_140248d10 — Pump_Threads inner: calls lua_resume, checks result
        0x246940L,  // FUN_140246940 — error handler invoked when lua_resume != 0
        0x247a90L,  // FUN_140247a90 — Pump_Threads outer (already known, re-decompile for context)
        0x7bc310L,  // FUN_1407bc310 — lua_resume itself
        0x7bb640L,  // FUN_1407bb640 — lua_sethook itself
    };

    static final String[] LABELS = {
        "Pump_Threads_inner(248d10)",
        "lua_resume_error_handler(246940)",
        "Pump_Threads_outer(247a90)",
        "lua_resume(7bc310)",
        "lua_sethook(7bb640)",
    };

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5LuaResumeErrorPath.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();

        for (int idx = 0; idx < TARGETS.length; idx++) {
            long rva = TARGETS[idx];
            String label = LABELS[idx];
            Address addr = base.add(rva);
            Function fn = getFunctionAt(addr);

            out.printf("=== %s ===\n", label);
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

        // Also dump all call sites of FUN_1407bc310 (lua_resume) to find every
        // place that checks the return value
        out.println("=== XREFS TO lua_resume (0x7bc310) ===\n");
        Address luaResumeAddr = base.add(0x7bc310L);
        Function luaResumeFn = getFunctionAt(luaResumeAddr);
        if (luaResumeFn != null) {
            for (ghidra.program.model.symbol.Reference ref :
                    currentProgram.getReferenceManager().getReferencesTo(luaResumeAddr)) {
                if (!ref.getReferenceType().isCall()) continue;
                Address callSite = ref.getFromAddress();
                Function caller = getFunctionContaining(callSite);
                long callerRva = callSite.getOffset() - base.getOffset();
                out.printf("  Call from RVA 0x%x  in %s\n",
                    callerRva,
                    caller != null ? caller.getName() : "???");
            }
        }
        out.println();

        // Also dump xrefs to lua_sethook (0x7bb640)
        out.println("=== XREFS TO lua_sethook (0x7bb640) ===\n");
        Address luaSethookAddr = base.add(0x7bb640L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(luaSethookAddr)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address callSite = ref.getFromAddress();
            Function caller = getFunctionContaining(callSite);
            long callerRva = callSite.getOffset() - base.getOffset();
            out.printf("  Call from RVA 0x%x  in %s\n",
                callerRva,
                caller != null ? caller.getName() : "???");
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5LuaResumeErrorPath.log");
    }
}
