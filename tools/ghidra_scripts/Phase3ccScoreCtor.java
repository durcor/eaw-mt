// Phase 3cc: Find the GameScoringManager constructor and its vftable[1].
// FUN_14004b250 (10192b, only caller of FUN_1404889d0) creates the manager.
// Read lines 200-600 and 600-1000 and 1000-1200 of FUN_14004b250 to find the call.
// Also: search ALL Ghidra symbol names containing "vftable" for scoring/think managers.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3ccScoreCtor extends GhidraScript {

    static final long RVA_4B250  = 0x4b250L;  // FUN_14004b250 — 10192b init fn
    static final long RVA_4889D0 = 0x4889d0L; // FUN_1404889d0 — subscribes to event 0x23

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3cc_score_ctor.c"));

        // Decompile FUN_14004b250 in windows to find the FUN_1404889d0 call
        section(pw, "FUN_14004b250 — WINDOW lines 200-450 (seeking FUN_1404889d0 call)");
        decompileWindow(decomp, pw, imageBase, RVA_4B250, 200, 250);

        section(pw, "FUN_14004b250 — WINDOW lines 450-700");
        decompileWindow(decomp, pw, imageBase, RVA_4B250, 450, 250);

        section(pw, "FUN_14004b250 — WINDOW lines 700-950");
        decompileWindow(decomp, pw, imageBase, RVA_4B250, 700, 250);

        section(pw, "FUN_14004b250 — WINDOW lines 950-1200");
        decompileWindow(decomp, pw, imageBase, RVA_4B250, 950, 250);

        // Search for ALL vftable symbols to find GameScoringManager vtable
        section(pw, "ALL VTABLE SYMBOLS with 'Scoring', 'Think', 'Service', 'GameObj'");
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator syms = symTable.getAllSymbols(true);
        while (syms.hasNext()) {
            Symbol sym = syms.next();
            String name = sym.getName();
            if (name.contains("vftable") || name.contains("vtable")) {
                if (name.contains("Scor") || name.contains("Think") || name.contains("AIMan") ||
                    name.contains("GameObj") || name.contains("Signal") ||
                    name.contains("ObjectManager") || name.contains("UnitService") ||
                    name.contains("Service") || name.contains("Entity")) {
                    long symRva = sym.getAddress().getOffset() - imageBase;
                    if (symRva > 0 && symRva < 0x800000L) { // valid RVA range
                        pw.println("// " + name + " at RVA 0x" + Long.toHexString(symRva));
                    }
                }
            }
        }
        pw.println();

        pw.close();
        decomp.dispose();
        println("Phase 3cc output written to logs/phase3cc_score_ctor.c");
    }

    private void decompileWindow(DecompInterface decomp, PrintWriter pw,
                                  long imageBase, long rva, int skip, int count) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) { pw.println("// No function"); return; }

        DecompileResults res = decomp.decompileFunction(fn, 300, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage()); return;
        }
        String[] lines = res.getDecompiledFunction().getC().split("\n");
        int total = lines.length;
        int start = Math.min(skip, total);
        int end = Math.min(skip + count, total);
        pw.println("// Total lines: " + total + ", showing " + start + " to " + end);
        for (int i = start; i < end; i++) pw.println(lines[i]);
        pw.println();
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }
}
