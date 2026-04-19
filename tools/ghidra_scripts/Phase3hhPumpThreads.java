// Phase 3hh: Decompile FUN_140247a90 (Pump_Threads — executes Lua AI coroutines).
// Also decompile FUN_140246fb0 (reads Lua variable, used in Service).
// And FUN_140247000 (sets Lua "LastService" variable).
// And FUN_1402488e0 (called in shutdown, likely Lua cleanup).
// This is the bottom of the per-unit AI call chain.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3hhPumpThreads extends GhidraScript {

    static final long RVA_247A90 = 0x247a90L; // FUN_140247a90 — Pump_Threads (Lua execution)
    static final long RVA_246FB0 = 0x246fb0L; // FUN_140246fb0 — read Lua variable
    static final long RVA_247000 = 0x247000L; // FUN_140247000 — set Lua variable
    static final long RVA_2488E0 = 0x2488e0L; // FUN_1402488e0 — called in shutdown

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3hh_pump_threads.c"));

        section(pw, "FUN_140247a90 — Pump_Threads (Lua AI coroutine execution)");
        decompile(decomp, pw, imageBase, RVA_247A90, 400);

        section(pw, "FUN_140246fb0 — read Lua variable (ServiceRate/LastService lookup)");
        decompile(decomp, pw, imageBase, RVA_246FB0, 200);

        section(pw, "FUN_140247000 — set Lua 'LastService' variable");
        decompile(decomp, pw, imageBase, RVA_247000, 200);

        section(pw, "FUN_1402488e0 — Lua cleanup (called in shutdown)");
        decompile(decomp, pw, imageBase, RVA_2488E0, 150);

        pw.close();
        decomp.dispose();
        println("Phase 3hh output written to logs/phase3hh_pump_threads.c");
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
            pw.println("// No function at RVA 0x" + Long.toHexString(rva) + "\n"); return;
        }
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");

        DecompileResults res = decomp.decompileFunction(fn, 240, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage() + "\n"); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        if (lines.length > maxLines) {
            for (int i = 0; i < maxLines; i++) pw.println(lines[i]);
            pw.println("// ... [truncated at " + maxLines + " lines, total " + lines.length + "]");
        } else {
            pw.println(code);
        }
        pw.println();
    }
}
