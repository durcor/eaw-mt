// Phase 3nn: Confirm vtable slot 43 for GalacticMode via ctor decompile.
// Find and decompile LoadingThreadClass::vftable[1] (actual LoadThread work).
// Begin shared-state survey for Pump_Threads: find all globals written in the DVC loop.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class Phase3nnLoadBody extends GhidraScript {

    // Mode ctors (found in Start_Game)
    static final long RVA_GALACTIC_CTOR = 0x4b1270L;
    static final long RVA_SPACE_CTOR    = 0x3b5210L;
    static final long RVA_LAND_CTOR     = 0x4d6ba0L;
    // LoadThread manager caller (single caller of FUN_14008df00)
    static final long RVA_LOAD_TRIGGER  = 0xc5250L;
    // FUN_140247a90 (Pump_Threads) callees to audit for shared state
    static final long RVA_PUMP_THREADS  = 0x247a90L;
    // FUN_140247310 — called on yield case 2
    static final long RVA_247310        = 0x247310L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();
        ReferenceManager refMgr = currentProgram.getReferenceManager();

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3nn_load_body.c"));

        // 1. Decompile GalacticMode ctor — vtable pointer reveals vtable base
        section(pw, "GalacticMode ctor (FUN_1404b1270) — vtable base + slot 43");
        decompile(decomp, pw, imageBase, RVA_GALACTIC_CTOR, 60);
        // Also brief look at the others
        section(pw, "SpaceMode ctor (FUN_1403b5210) — first 40 lines");
        decompile(decomp, pw, imageBase, RVA_SPACE_CTOR, 40);
        section(pw, "LandMode ctor (FUN_1404d6ba0) — first 40 lines");
        decompile(decomp, pw, imageBase, RVA_LAND_CTOR, 40);

        // 2. For each ctor vtable assignment, read the vtable slot 43 via data model
        section(pw, "VTABLE SLOT 43 VIA CTOR DATA REFS");
        long[] ctorRvas = {RVA_GALACTIC_CTOR, RVA_SPACE_CTOR, RVA_LAND_CTOR};
        String[] ctorNames = {"GalacticMode", "SpaceMode", "LandMode"};
        for (int i = 0; i < ctorRvas.length; i++) {
            Address ctorAddr = currentProgram.getImageBase().add(ctorRvas[i]);
            Function ctor = getFunctionAt(ctorAddr);
            if (ctor == null) { pw.println("// No fn at ctor RVA " + Long.toHexString(ctorRvas[i])); continue; }
            pw.println("// " + ctorNames[i] + " ctor data refs:");
            for (Reference ref : refMgr.getReferencesFrom(ctorAddr)) {
                if (ref.getReferenceType().isData()) {
                    long toRva = ref.getToAddress().getOffset() - imageBase;
                    if (toRva < 0 || toRva > 0xb00000L) continue;
                    pw.println("//   DataRef → RVA 0x" + Long.toHexString(toRva) +
                        " (possible vtable base)");
                    // Read slot 43 from this base
                    Address slot43 = currentProgram.getImageBase().add(toRva + 0x158L);
                    try {
                        Data d = listing.getDataAt(slot43);
                        if (d != null && d.getDataType() instanceof Pointer) {
                            Address target = (Address) d.getValue();
                            if (target != null) {
                                long tRva = target.getOffset() - imageBase;
                                Function fn = getFunctionAt(target);
                                pw.println("//   slot[43] → fn RVA 0x" + Long.toHexString(tRva) +
                                    " " + (fn != null ? fn.getName() : "?") +
                                    " " + (fn != null ? fn.getBody().getNumAddresses() + "b" : ""));
                            }
                        } else {
                            pw.println("//   slot[43] no pointer data at RVA 0x" + Long.toHexString(toRva + 0x158L));
                        }
                    } catch (Exception e) { pw.println("//   slot[43] error: " + e.getMessage()); }
                }
            }
        }

        // 3. Find LoadingThreadClass vtable via symbol table, read vftable[1]
        section(pw, "LoadingThreadClass vtable → vftable[1] = actual LoadThread body");
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator si = symTable.getAllSymbols(true);
        while (si.hasNext()) {
            Symbol sym = si.next();
            String name = sym.getName();
            long symRva = sym.getAddress().getOffset() - imageBase;
            if (symRva <= 0 || symRva > 0xb00000L) continue;
            if (name.contains("LoadingThread") && (name.contains("vtable") || name.contains("vftable"))) {
                pw.println("// LoadingThread vtable sym: " + name + " at RVA 0x" + Long.toHexString(symRva));
                // Read slot 1
                Address slot1Addr = currentProgram.getImageBase().add(symRva + 8L);
                try {
                    Data d = listing.getDataAt(slot1Addr);
                    if (d != null && d.getDataType() instanceof Pointer) {
                        Address target = (Address) d.getValue();
                        if (target != null) {
                            long tRva = target.getOffset() - imageBase;
                            Function fn = getFunctionAt(target);
                            pw.println("//   vftable[1] → fn RVA 0x" + Long.toHexString(tRva) +
                                " " + (fn != null ? fn.getName() : "?") +
                                " " + (fn != null ? fn.getBody().getNumAddresses() + "b" : ""));
                            if (fn != null) decompile(decomp, pw, imageBase, tRva, 120);
                        }
                    } else pw.println("//   vftable[1] no pointer data");
                } catch (Exception e) { pw.println("//   error: " + e.getMessage()); }
            }
        }

        // 4. LoadThread trigger caller
        section(pw, "FUN_1400c5250 — LoadThread trigger caller (caller of FUN_14008df00)");
        decompile(decomp, pw, imageBase, RVA_LOAD_TRIGGER, 80);

        // 5. Pump_Threads callees — for shared-state audit
        section(pw, "FUN_140247310 — yield case 2 handler (Pump_Threads callee)");
        decompile(decomp, pw, imageBase, RVA_247310, 80);

        // 6. Global data writes in Pump_Threads — scan all functions called by it
        section(pw, "Pump_Threads callees (for shared-state survey)");
        Address pumpAddr = currentProgram.getImageBase().add(RVA_PUMP_THREADS);
        Function pumpFn = getFunctionAt(pumpAddr);
        if (pumpFn != null) {
            pw.println("// Direct callees of Pump_Threads:");
            for (Function callee : pumpFn.getCalledFunctions(monitor)) {
                long calleeRva = callee.getEntryPoint().getOffset() - imageBase;
                if (calleeRva < 0 || calleeRva > 0xc00000L) continue;
                pw.println("//   RVA 0x" + Long.toHexString(calleeRva) +
                    "  " + callee.getName() + "  " + callee.getBody().getNumAddresses() + "b");
                // Brief decompile of each
                decompile(decomp, pw, imageBase, calleeRva, 30);
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3nn output written.");
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
