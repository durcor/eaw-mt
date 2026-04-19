// Phase 3z: Find what vtable FUN_1402215b0 is stored in (no xref callers = vtable dispatch).
// Also: directly search for what other functions subscribe to event 0x23 via FUN_1402406c0
// by checking all direct/indirect call sites and looking for the pattern.
// Strategy: find data cross-references to FUN_1402215b0 to identify its vtable.
// Then decompile the constructor of that class to understand the subscription manager.
// Also: decompile FUN_14053cd60 (1984b caller of FUN_14020aa60) and FUN_1405262d0 (409b).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3zVtableSearch extends GhidraScript {

    static final long RVA_2215B0 = 0x2215b0L; // FUN_1402215b0 — no xref callers, in a vtable
    static final long RVA_53CD60 = 0x53cd60L; // FUN_14053cd60 — 1984b caller of FUN_14020aa60
    static final long RVA_5262D0 = 0x5262d0L; // FUN_1405262d0 — 409b caller of FUN_14020aa60
    static final long RVA_53B380 = 0x53b380L; // FUN_14053b380 — 203b caller of FUN_14020aa60
    static final long RVA_19DCA0 = 0x19dca0L; // FUN_14019dca0 — 61b caller of FUN_14020aa60
    static final long RVA_195430 = 0x195430L; // FUN_140195430 — 87b caller of FUN_14020aa60

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3z_vtable_search.c"));

        // Find data references to FUN_1402215b0 (vtable entries)
        section(pw, "DATA REFERENCES TO FUN_1402215b0 (vtable location)");
        Address addr2215b0 = currentProgram.getImageBase().add(RVA_2215B0);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr2215b0);
        Set<Long> seenData = new HashSet<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (!ref.getReferenceType().isCall()) {
                long refOffset = ref.getFromAddress().getOffset() - imageBase;
                if (seenData.add(refOffset)) {
                    pw.println("// DataRef at RVA 0x" + Long.toHexString(refOffset) +
                        "  Type: " + ref.getReferenceType());
                    // Read surrounding vtable context (8 entries before and after)
                    long vtableBase = refOffset - 0x40; // go back 8 slots
                    for (int i = -8; i <= 8; i++) {
                        long slotRva = refOffset + i * 8;
                        if (slotRva < 0) continue;
                        Address slotAddr = currentProgram.getImageBase().add(slotRva);
                        try {
                            long ptr = currentProgram.getMemory().getLong(slotAddr, true);
                            long fnRva = ptr - imageBase;
                            Function fn = getFunctionAt(currentProgram.getImageBase().add(fnRva));
                            String fnName = (fn != null) ? fn.getName() : "?";
                            String marker = (i == 0) ? " <-- FUN_1402215b0" : "";
                            pw.println("//   [" + i + "] RVA 0x" + Long.toHexString(slotRva) +
                                " -> fn 0x" + Long.toHexString(fnRva) + " " + fnName + marker);
                        } catch (Exception e) {
                            pw.println("//   [" + i + "] RVA 0x" + Long.toHexString(slotRva) + " (unreadable)");
                        }
                    }
                    pw.println();
                }
            }
        }

        // Small callers of FUN_14020aa60
        section(pw, "FUN_14019dca0 — 61b caller of FUN_14020aa60 (tiny)");
        decompile(decomp, pw, imageBase, RVA_19DCA0, 60);

        section(pw, "FUN_140195430 — 87b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_195430, 80);

        section(pw, "FUN_14053b380 — 203b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_53B380, 150);

        section(pw, "FUN_1405262d0 — 409b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_5262D0, 200);

        // FUN_14053cd60 head — look for event 0x23 pattern
        section(pw, "FUN_14053cd60 — 1984b caller of FUN_14020aa60 (head 150 lines)");
        decompile(decomp, pw, imageBase, RVA_53CD60, 150);

        pw.close();
        decomp.dispose();
        println("Phase 3z output written to logs/phase3z_vtable_search.c");
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
