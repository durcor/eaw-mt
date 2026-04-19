// Phase 3f: Find the simulation tick via the game mode vtable.
// DAT_140b15418 is the active game mode object (galactic/space/land).
// The scene manager (FUN_14030c3b0) calls vtable methods on it each frame.
// Goal: find which vtable slot is the mode's Update/Think, and what it dispatches.
// Also decompile the mode constructors to find the vtable address.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;

public class Phase3fModeVtable extends GhidraScript {

    // Scene manager — read full body (3157 bytes) to map all vtable calls
    static final long RVA_SCENE_MGR   = 0x30c3b0L;  // FUN_14030c3b0

    // Game mode constructors — these set vtable ptr, allowing us to find the vtable
    static final long RVA_GALACTIC_CTOR = 0x4b1270L; // galactic mode init (alloc 0x378)
    static final long RVA_SPACE_CTOR    = 0x3b5210L; // space mode init (alloc 0x430)
    static final long RVA_LAND_CTOR     = 0x4d6ba0L; // land mode init (alloc 0x3c8)

    // Unconditional scene manager sub-call (already in phase3b but need to re-read)
    static final long RVA_SCENE_SUB    = 0x305c40L;  // called from scene mgr every frame

    // FUN_1402aca60 — planet accessor called in Land_Invade
    static final long RVA_PLANET_GET   = 0x2aca60L;

    // FUN_14030c3b0 sub-calls: flag-gated (already phase3b) but need key inner dispatches
    static final long RVA_FD330        = 0x2fd330L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3f_mode_vtable.c"));

        // 1. Full scene manager decompile
        section(pw, "SCENE MANAGER — FUN_14030c3b0 (3157 bytes, full)");
        decompile(decomp, pw, imageBase, RVA_SCENE_MGR, 5000);

        // 2. Game mode constructors — find vtable pointer set
        section(pw, "GALACTIC MODE CTOR — FUN_1404b1270");
        decompile(decomp, pw, imageBase, RVA_GALACTIC_CTOR, 400);

        section(pw, "SPACE MODE CTOR — FUN_1403b5210");
        decompile(decomp, pw, imageBase, RVA_SPACE_CTOR, 400);

        section(pw, "LAND MODE CTOR — FUN_1404d6ba0");
        decompile(decomp, pw, imageBase, RVA_LAND_CTOR, 400);

        // 3. Unconditional scene sub-call (every frame)
        section(pw, "SCENE MANAGER UNCONDITIONAL SUB-CALL — FUN_140305c40");
        decompile(decomp, pw, imageBase, RVA_SCENE_SUB, 2000);

        // 4. Planet getter
        section(pw, "PLANET GETTER — FUN_1402aca60");
        decompile(decomp, pw, imageBase, RVA_PLANET_GET, 200);

        // 5. Find symbols for game mode vtables via RTTI
        section(pw, "RTTI SYMBOL SEARCH — game mode classes");
        findModeSymbols(pw, imageBase);

        pw.close();
        decomp.dispose();
        println("Phase 3f output written to logs/phase3f_mode_vtable.c");
    }

    private void findModeSymbols(PrintWriter pw, long imageBase) throws Exception {
        SymbolTable symTable = currentProgram.getSymbolTable();
        String[] names = {
            "GalacticModeClass",
            "GalacticConflictModeClass",
            "SpaceConflictModeClass",
            "LandConflictModeClass",
            "GameModeClass",
            "GameModeManagerClass",
            "GameObjectManagerClass",
            "GalacticModeClass::vftable",
            "SpaceConflictModeClass::vftable",
            "LandConflictModeClass::vftable",
            "GameModeClass::Think",
            "GameModeClass::Update",
            "GameModeClass::Service",
            "GameModeClass::Next_State",
            "GameObjectManagerClass::Think",
            "GameObjectManagerClass::Service",
            "DynamicVectorClass",
        };
        for (String name : names) {
            SymbolIterator it = symTable.getSymbols(name);
            int count = 0;
            while (it.hasNext()) {
                Symbol sym = it.next();
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// SYMBOL: " + sym.getName() +
                    " @ RVA 0x" + Long.toHexString(rva) +
                    " type=" + sym.getSymbolType() +
                    " source=" + sym.getSource());
                count++;
            }
            if (count == 0) pw.println("// NOT FOUND: " + name);
        }
        pw.println();

        // Also try prefix-matching via getSymbols(namespace)
        Memory mem = currentProgram.getMemory();
        pw.println("\n// Scanning for GameMode/GOM in all symbol names...");
        SymbolIterator allSyms = symTable.getAllSymbols(false);
        int found = 0;
        while (allSyms.hasNext() && found < 100) {
            Symbol sym = allSyms.next();
            String name = sym.getName();
            if (name.contains("GameMode") || name.contains("GOM") ||
                name.contains("Galactic") || name.contains("SpaceConflict") ||
                name.contains("LandConflict") || name.contains("GameObjectManager")) {
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("//   " + name + " @ RVA 0x" + Long.toHexString(rva) +
                    " type=" + sym.getSymbolType());
                found++;
            }
        }
        if (found == 0) pw.println("// (none found)");
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

        DecompileResults res = decomp.decompileFunction(fn, 180, monitor);
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
