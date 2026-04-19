// Phase 3c: Full WinMain decompile + GOM search via RTTI string xrefs.
// WinMain is 7440 bytes and may have more game-active calls we missed.
// Also search for "GameObjectManager" and "Think" via string table xrefs.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3cWinmainFull extends GhidraScript {

    static final long RVA_WINMAIN = 0x5d990L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3c_winmain_full.c"));

        // 1. Full WinMain decompile (no truncation)
        section(pw, "WINMAIN FULL DECOMPILE (FUN_14005d990)");
        Address addr = currentProgram.getImageBase().add(RVA_WINMAIN);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn != null) {
            pw.println("// RVA: 0x" + Long.toHexString(RVA_WINMAIN) +
                "  Size: " + fn.getBody().getNumAddresses() + " bytes");
            DecompileResults res = decomp.decompileFunction(fn, 180, monitor);
            if (res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
            } else {
                pw.println("// DECOMPILE FAILED: " + res.getErrorMessage());
            }
        }

        // 2. Search for GOM-related strings in memory
        section(pw, "STRING SEARCH: GameObjectManager / Think / GOM_Update");
        searchStrings(pw, imageBase, new String[]{
            "GameObjectManager",
            "GameObjectClass",
            "GOM",
            "Think",
            "Update_All",
            "Sim_Object",
            "SimObject",
            "game_object",
            "unit_update",
            "PhysicsClass",
            "AIClass",
            "TacticalAI",
            "BehaviorClass",
        });

        pw.close();
        decomp.dispose();
        println("Phase 3c output written to logs/phase3c_winmain_full.c");
    }

    private void searchStrings(PrintWriter pw, long imageBase, String[] terms) throws Exception {
        Memory mem = currentProgram.getMemory();
        for (String term : terms) {
            pw.println("\n// --- Searching for: \"" + term + "\" ---");
            byte[] pattern = term.getBytes("US-ASCII");
            Address start = currentProgram.getMinAddress();
            int found = 0;
            while (found < 20) {
                Address hit = mem.findBytes(start, pattern, null, true, monitor);
                if (hit == null) break;
                long rva = hit.getOffset() - imageBase;
                pw.println("//   String at RVA 0x" + Long.toHexString(rva));
                // Find xrefs to this string
                ReferenceIterator refs = currentProgram.getReferenceManager()
                    .getReferencesTo(hit);
                int xrefCount = 0;
                while (refs.hasNext() && xrefCount < 10) {
                    Reference ref = refs.next();
                    Function callerFn = getFunctionContaining(ref.getFromAddress());
                    if (callerFn != null) {
                        long fnRva = callerFn.getEntryPoint().getOffset() - imageBase;
                        pw.println("//     xref from FUN_0x" + Long.toHexString(fnRva) +
                            " @ site 0x" + Long.toHexString(ref.getFromAddress().getOffset() - imageBase));
                    }
                    xrefCount++;
                }
                start = hit.add(1);
                found++;
            }
            if (found == 0) {
                pw.println("//   (not found)");
            }
        }
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }
}
