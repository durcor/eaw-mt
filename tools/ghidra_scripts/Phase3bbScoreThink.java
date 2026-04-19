// Phase 3bb: Find the GameScoringManager class and its vftable[1] (the per-frame Think).
// Also: find what mode vtable[43] (offset 0x158) actually is for Space and Land modes.
// Also: search for "TheGameScoringManager" string references to find the constructor.
// Also: decompile FUN_14004b250 around the FUN_1404889d0 call site (the 10192b init fn).
// Also: check FUN_140240940 (signal dispatcher) to understand listener extraction.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3bbScoreThink extends GhidraScript {

    static final long RVA_240940  = 0x240940L; // FUN_140240940 — signal dispatcher
    // SpaceModeClass vtable base — need to find it
    // LandModeClass thunk at 0x3b5e10

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3bb_score_think.c"));

        // Decompile the signal dispatcher to understand listener layout
        section(pw, "FUN_140240940 — signal dispatcher (how it calls listener vftable[1])");
        decompile(decomp, pw, imageBase, RVA_240940, 300);

        // Search for string "Scoring" in the binary to find GameScoringManager ctor
        section(pw, "STRING SEARCH: functions containing 'Scoring' references");
        Set<Long> scoringFns = new HashSet<>();
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator syms = symTable.getDefinedSymbols();
        while (syms.hasNext()) {
            Symbol sym = syms.next();
            String name = sym.getName();
            if (name.contains("Scoring") || name.contains("GameScore") || name.contains("scoring")) {
                Address symAddr = sym.getAddress();
                long symRva = symAddr.getOffset() - imageBase;
                Function fn = getFunctionAt(symAddr);
                if (fn == null) fn = getFunctionContaining(symAddr);
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    if (scoringFns.add(fnRva)) {
                        pw.println("// Symbol: " + name + " at RVA 0x" + Long.toHexString(symRva));
                        pw.println("//   in fn RVA 0x" + Long.toHexString(fnRva) +
                            " Name: " + fn.getName() + " Size: " + fn.getBody().getNumAddresses());
                    }
                } else {
                    pw.println("// Symbol: " + name + " at RVA 0x" + Long.toHexString(symRva) + " (no fn)");
                }
            }
        }
        pw.println();

        // Search for vtable symbols containing "Scoring"
        section(pw, "VTABLE SEARCH: 'Scoring' or 'GameObjectManager' vtables");
        SymbolIterator syms2 = symTable.getDefinedSymbols();
        while (syms2.hasNext()) {
            Symbol sym = syms2.next();
            String name = sym.getName();
            if ((name.contains("Scoring") || name.contains("GameManager") ||
                 name.contains("ThinkManager") || name.contains("ServiceManager")) &&
                name.contains("vftable")) {
                long symRva = sym.getAddress().getOffset() - imageBase;
                pw.println("// Vtable: " + name + " at RVA 0x" + Long.toHexString(symRva));
                // Read the first 8 vtable entries (slots 0-7)
                for (int i = 0; i < 8; i++) {
                    long slotRva = symRva + i * 8;
                    Address slotAddr = currentProgram.getImageBase().add(slotRva);
                    try {
                        long ptr = currentProgram.getMemory().getLong(slotAddr, true);
                        long fnRva = ptr - imageBase;
                        Function fn = getFunctionAt(currentProgram.getImageBase().add(fnRva));
                        String fnName = (fn != null) ? fn.getName() : "?";
                        long size = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                        pw.println("//   vtable[" + i + "] RVA 0x" + Long.toHexString(fnRva) +
                            " " + fnName + " (" + size + "b)");
                    } catch (Exception e) {
                        pw.println("//   vtable[" + i + "] (unreadable)");
                    }
                }
                pw.println();
            }
        }

        // Search for ALL vtables to find the SpaceModeClass and LandModeClass vtables
        section(pw, "MODE CLASS VTABLES (SpaceMode, LandMode, CinematicMode, GalacticMode)");
        SymbolIterator syms3 = symTable.getDefinedSymbols();
        while (syms3.hasNext()) {
            Symbol sym = syms3.next();
            String name = sym.getName();
            if (name.contains("vftable") && (name.contains("Mode") || name.contains("mode"))) {
                long symRva = sym.getAddress().getOffset() - imageBase;
                pw.println("// ModeVtable: " + name + " at RVA 0x" + Long.toHexString(symRva));
                // Read slot 43 (offset 0x158)
                long slot43Rva = symRva + 43 * 8; // vtable[43] = offset 0x158
                try {
                    Address slot43Addr = currentProgram.getImageBase().add(slot43Rva);
                    long ptr = currentProgram.getMemory().getLong(slot43Addr, true);
                    long fnRva = ptr - imageBase;
                    Function fn = getFunctionAt(currentProgram.getImageBase().add(fnRva));
                    String fnName = (fn != null) ? fn.getName() : "?";
                    long size = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                    pw.println("//   [43] offset 0x158 -> RVA 0x" + Long.toHexString(fnRva) +
                        " " + fnName + " (" + size + "b)");
                    // Also read a few surrounding slots for context
                    for (int i = 40; i <= 46; i++) {
                        long slotRva = symRva + i * 8;
                        Address slotAddr = currentProgram.getImageBase().add(slotRva);
                        try {
                            ptr = currentProgram.getMemory().getLong(slotAddr, true);
                            fnRva = ptr - imageBase;
                            fn = getFunctionAt(currentProgram.getImageBase().add(fnRva));
                            fnName = (fn != null) ? fn.getName() : "?";
                            size = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                            String marker = (i == 43) ? " <-- WinMain calls this" : "";
                            pw.println("//   [" + i + "] RVA 0x" + Long.toHexString(fnRva) +
                                " " + fnName + " (" + size + "b)" + marker);
                        } catch (Exception e) {
                            pw.println("//   [" + i + "] (unreadable)");
                        }
                    }
                } catch (Exception e) {
                    pw.println("//   slot 43 (unreadable): " + e.getMessage());
                }
                pw.println();
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3bb output written to logs/phase3bb_score_think.c");
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
