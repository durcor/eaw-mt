// Phase 3ff: Decompile FUN_140485340 (TheGameScoringManagerClass constructor).
// Also: find its vtable, read vtable[0..4], and decompile vtable[1] = per-frame Think.
// Also: decompile FUN_140488ab0 (shutdown, called on manager in FUN_14004da50).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3ffScoreCtor2 extends GhidraScript {

    static final long RVA_485340 = 0x485340L; // FUN_140485340 — GameScoringManager ctor
    static final long RVA_488AB0 = 0x488ab0L; // FUN_140488ab0 — GameScoringManager shutdown

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Memory mem = currentProgram.getMemory();

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3ff_score_ctor2.c"));

        section(pw, "FUN_140485340 — TheGameScoringManagerClass constructor");
        decompile(decomp, pw, imageBase, RVA_485340, 200);

        section(pw, "FUN_140488ab0 — GameScoringManager shutdown (called in deinit)");
        decompile(decomp, pw, imageBase, RVA_488AB0, 200);

        // Try to read the vtable directly from any vftable the ctor sets
        // We'll scan for TheGameScoringManagerClass::vftable by name
        section(pw, "SEARCHING FOR TheGameScoringManagerClass VTABLE");
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator allSyms = symTable.getAllSymbols(true);
        while (allSyms.hasNext()) {
            Symbol sym = allSyms.next();
            String name = sym.getName();
            long symRva = sym.getAddress().getOffset() - imageBase;
            // Filter valid RVAs and names related to scoring
            if (symRva > 0 && symRva < 0x900000L &&
                (name.contains("Scoring") || name.contains("scoring") ||
                 name.contains("485340") || name.contains("488ab0") ||
                 (name.contains("vftable") && name.contains("Game") && name.contains("Scor")))) {
                pw.println("// Symbol: " + name + " at RVA 0x" + Long.toHexString(symRva));
            }
        }
        pw.println();

        // Also decompile FUN_140485340 callees to find where vtable is set
        section(pw, "CALLEES OF FUN_140485340 (constructor callees)");
        Function fn485340 = getFunctionAt(currentProgram.getImageBase().add(RVA_485340));
        if (fn485340 != null) {
            Set<Long> calleeSeen = new HashSet<>();
            Address bodyStart = fn485340.getBody().getMinAddress();
            Address bodyEnd = fn485340.getBody().getMaxAddress();
            Address cur = bodyStart;
            while (cur != null && cur.compareTo(bodyEnd) <= 0) {
                for (Reference ref : currentProgram.getReferenceManager().getReferencesFrom(cur)) {
                    if (ref.getReferenceType().isCall()) {
                        long calleeRva = ref.getToAddress().getOffset() - imageBase;
                        if (calleeSeen.add(calleeRva)) {
                            Function callee = getFunctionAt(ref.getToAddress());
                            String nm = (callee != null) ? callee.getName() : "?";
                            long sz = (callee != null) ? callee.getBody().getNumAddresses() : 0;
                            pw.println("// Callee: RVA 0x" + Long.toHexString(calleeRva) +
                                "  " + nm + "  " + sz + " bytes");
                        }
                    }
                }
                try { cur = cur.add(1); } catch (Exception e) { break; }
            }
        }
        pw.println();

        pw.close();
        decomp.dispose();
        println("Phase 3ff output written to logs/phase3ff_score_ctor2.c");
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
