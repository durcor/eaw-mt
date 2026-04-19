// Phase 3y: FUN_1404889d0 subscribes param_1 to event 0x23 (per-frame Think).
// Find callers to identify the object type, then find the constructor to get the vtable,
// then look at vftable[1] = the per-unit Think function.
// Also decompile FUN_1404854e0 (called in FUN_1404889d0 — likely object init check).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3yThinkSub extends GhidraScript {

    static final long RVA_4889D0 = 0x4889d0L; // FUN_1404889d0 — subscribes param_1 to event 0x23
    static final long RVA_4854E0 = 0x4854e0L; // FUN_1404854e0 — called inside 4889d0

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3y_think_sub.c"));

        section(pw, "FUN_1404854e0 — called inside FUN_1404889d0 (object init check)");
        decompile(decomp, pw, imageBase, RVA_4854E0, 300);

        // Find all callers of FUN_1404889d0
        section(pw, "ALL CALLERS OF FUN_1404889d0 (subscribe to event 0x23)");
        Address addr4889d0 = currentProgram.getImageBase().add(RVA_4889D0);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr4889d0);
        List<long[]> callers = new ArrayList<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    boolean found = false;
                    for (long[] e : callers) if (e[0] == callerRva) { found = true; break; }
                    if (!found) {
                        callers.add(new long[]{callerRva, caller.getBody().getNumAddresses()});
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Decompile all callers to find the object type
        for (long[] entry : callers) {
            int limit = entry[1] <= 400 ? 200 : (entry[1] <= 1000 ? 100 : 60);
            section(pw, "Caller RVA 0x" + Long.toHexString(entry[0]) + " (" + entry[1] + " bytes)");
            decompile(decomp, pw, imageBase, entry[0], limit);
        }

        pw.close();
        decomp.dispose();
        println("Phase 3y output written to logs/phase3y_think_sub.c");
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
