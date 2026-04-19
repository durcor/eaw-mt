// Phase 3ii: Confirm thunk_FUN_1407bc310 is lua_resume and understand coroutine-to-unit mapping.
// Also confirm FUN_140215b90 identity (frame profiler vs QPC timer).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3iiLuaResume extends GhidraScript {

    // thunk_FUN_1407bc310 — the lua_resume wrapper called from Pump_Threads
    static final long RVA_THUNK     = 0x7bc310L;
    // FUN_140215b90 — frame profiler candidate
    static final long RVA_215B90    = 0x215b90L;
    // FUN_140247700 — called at start of pump loop per thread (setup?)
    static final long RVA_247700    = 0x247700L;
    // FUN_140248d10 — called after lua_resume returns in pump loop (cleanup?)
    static final long RVA_248D10    = 0x248d10L;
    // FUN_140246940 — called on error path in pump loop
    static final long RVA_246940    = 0x246940L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3ii_lua_resume.c"));

        // --- thunk ---
        section(pw, "thunk_FUN_1407bc310 — lua_resume wrapper");
        decompile(decomp, pw, imageBase, RVA_THUNK, 80);
        // also follow what thunk calls
        followCallees(decomp, pw, imageBase, RVA_THUNK, 80, 2);

        // --- FUN_140215b90 ---
        section(pw, "FUN_140215b90 — frame profiler / QPC timer candidate");
        decompile(decomp, pw, imageBase, RVA_215B90, 120);

        // --- FUN_140247700 — per-coroutine setup in Pump_Threads ---
        section(pw, "FUN_140247700 — per-coroutine setup (Pump_Threads slot 0)");
        decompile(decomp, pw, imageBase, RVA_247700, 100);

        // --- FUN_140248d10 — post-resume cleanup ---
        section(pw, "FUN_140248d10 — post-resume cleanup (reinit on coroutine finish)");
        decompile(decomp, pw, imageBase, RVA_248D10, 100);

        // --- FUN_140246940 --- error path
        section(pw, "FUN_140246940 — called on lua_resume error path");
        decompile(decomp, pw, imageBase, RVA_246940, 60);

        // Callers of FUN_140215b90 — to understand call context
        section(pw, "Callers of FUN_140215b90");
        Address addr215 = currentProgram.getImageBase().add(RVA_215B90);
        Function fn215 = getFunctionAt(addr215);
        if (fn215 != null) {
            for (Function caller : fn215.getCallingFunctions(monitor)) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                    "  " + caller.getName() + "  " + caller.getBody().getNumAddresses() + " bytes");
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3ii output written.");
    }

    private void followCallees(DecompInterface decomp, PrintWriter pw,
                                long imageBase, long rva, int maxLines, int depth) throws Exception {
        if (depth <= 0) return;
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) return;
        Set<Function> callees = fn.getCalledFunctions(monitor);
        if (callees.isEmpty()) {
            pw.println("// (no callees in " + fn.getName() + ")");
            return;
        }
        for (Function callee : callees) {
            long calleeRva = callee.getEntryPoint().getOffset() - imageBase;
            if (calleeRva < 0 || calleeRva > 0xc00000L) continue; // skip external/bad
            section(pw, "callee of 0x" + Long.toHexString(rva) + " → " +
                callee.getName() + " (RVA 0x" + Long.toHexString(calleeRva) + ")");
            decompile(decomp, pw, imageBase, calleeRva, maxLines);
        }
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }

    private void decompile(DecompInterface decomp, PrintWriter pw,
                            long imageBase, long rva, int maxLines) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function at RVA 0x" + Long.toHexString(rva)); return;
        }
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() + "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        DecompileResults res = decomp.decompileFunction(fn, 240, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage()); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        for (int i = 0; i < Math.min(maxLines, lines.length); i++) pw.println(lines[i]);
        if (lines.length > maxLines)
            pw.println("// ... [truncated at " + maxLines + " / " + lines.length + " lines]");
        pw.println();
    }
}
