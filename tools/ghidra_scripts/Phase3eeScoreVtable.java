// Phase 3ee: Decompile FUN_14004fd50 (TheGameScoringManagerClass::Get) to find the
// GameScoringManager constructor → vtable → vftable[1] = per-frame Think callback.
// Also decompile FUN_1404889d0 callees: FUN_140245690 already known.
// Also check what FUN_14046a350 is (called after FUN_1404889d0 in init).
// And: decompile FUN_14004da50 (called on init error, might reveal class layout).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3eeScoreVtable extends GhidraScript {

    static final long RVA_4FD50  = 0x4fd50L;  // FUN_14004fd50 — TheGameScoringManagerClass::Get()
    static final long RVA_46A350 = 0x46a350L; // FUN_14046a350 — called after scoring manager init
    static final long RVA_4DA50  = 0x4da50L;  // FUN_14004da50 — called on init error

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3ee_score_vtable.c"));

        section(pw, "FUN_14004fd50 — TheGameScoringManagerClass::Get() singleton getter");
        decompile(decomp, pw, imageBase, RVA_4FD50, 200);

        section(pw, "FUN_14046a350 — called after GameScoringManager::System_Initialize");
        decompile(decomp, pw, imageBase, RVA_46A350, 200);

        section(pw, "FUN_14004da50 — called on system init error");
        decompile(decomp, pw, imageBase, RVA_4DA50, 150);

        // Find callers of FUN_14004fd50 (uses of the GameScoringManager singleton)
        section(pw, "ALL CALLERS OF FUN_14004fd50 (GameScoringManagerClass::Get)");
        Address addrFd50 = currentProgram.getImageBase().add(RVA_4FD50);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addrFd50);
        Set<Long> seen = new HashSet<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    if (seen.add(callerRva)) {
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Decompile small callers of FUN_14004fd50 to find what methods are called on it
        for (long callerRva : seen) {
            Function fn = getFunctionAt(currentProgram.getImageBase().add(callerRva));
            if (fn == null) fn = getFunctionContaining(currentProgram.getImageBase().add(callerRva));
            if (fn != null && fn.getBody().getNumAddresses() < 300) {
                section(pw, "Small caller: RVA 0x" + Long.toHexString(callerRva) +
                    " (" + fn.getBody().getNumAddresses() + " bytes)");
                decompile(decomp, pw, imageBase, callerRva, 150);
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3ee output written to logs/phase3ee_score_vtable.c");
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
