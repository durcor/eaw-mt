// Phase 3i: Read the GameModeClass vtable to find the simulation tick slot.
// The GameModeClass contains DynamicVectorClass<DynamicVectorClass<GameObjectClass*>>
// at offset 0x1d8 — it IS the GOM. Its vtable's Think/Service method is the sim tick.
// The scene manager calls vtable slots: +0xe0(28), +0xf0(30), +0xf8(31), +0x1e8(61).
// Goal: find and decompile ALL vtable methods, identify the per-frame simulation tick.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;

public class Phase3iVtableSlots extends GhidraScript {

    // GameModeClass::vftable — find via namespace-qualified symbol search
    // Also check GalacticModeClass::vftable (derived, overrides some slots)

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3i_vtable_slots.c"));

        // 1. Find vtable symbols via namespace
        section(pw, "VTABLE SYMBOL SEARCH (namespace-aware)");
        long gameModeVtableRva = findVtableByNamespace(pw, imageBase, "GameModeClass");
        long galacticVtableRva = findVtableByNamespace(pw, imageBase, "GalacticModeClass");
        long landVtableRva    = findVtableByNamespace(pw, imageBase, "LandModeClass");
        long spaceVtableRva   = findVtableByNamespace(pw, imageBase, "SpaceModeClass");

        // 2. If we found GameModeClass vtable, read its entries
        if (gameModeVtableRva != 0) {
            section(pw, "GameModeClass VTABLE ENTRIES");
            readVtable(pw, imageBase, gameModeVtableRva, 80, "GameModeClass");
        }
        if (galacticVtableRva != 0) {
            section(pw, "GalacticModeClass VTABLE ENTRIES (first 80 slots)");
            readVtable(pw, imageBase, galacticVtableRva, 80, "GalacticModeClass");
        }

        // 3. Decompile key vtable slots (slots called from scene manager)
        //    From scene manager: +0xe0=28, +0xf0=30, +0xf8=31, +0x1e8=61
        //    We need to find the "Think" slot — probably adjacent to known slots
        if (galacticVtableRva != 0) {
            section(pw, "KEY VTABLE SLOT DECOMPILES (GalacticModeClass)");
            int[] slots = {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                            58, 59, 60, 61, 62, 63, 64};
            for (int slot : slots) {
                long fnRva = readVtableSlot(imageBase, galacticVtableRva, slot);
                if (fnRva != 0) {
                    decompile(decomp, pw, imageBase, fnRva, 200);
                }
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3i output written to logs/phase3i_vtable_slots.c");
    }

    private long findVtableByNamespace(PrintWriter pw, long imageBase, String className)
            throws Exception {
        SymbolTable symTable = currentProgram.getSymbolTable();
        // Scan all symbols for matching vftable
        SymbolIterator allSyms = symTable.getAllSymbols(false);
        while (allSyms.hasNext()) {
            Symbol sym = allSyms.next();
            Namespace ns = sym.getParentNamespace();
            String symName = sym.getName();
            // Match "vftable" in namespace == className
            if (symName.equals("vftable") && ns != null &&
                    ns.getName().equals(className)) {
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// " + className + "::vftable @ RVA 0x" +
                    Long.toHexString(rva) + " type=" + sym.getSymbolType());
                return rva;
            }
            // Also match full qualified name like "ClassName::vftable"
            if (symName.equals(className + "::vftable")) {
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// " + symName + " @ RVA 0x" +
                    Long.toHexString(rva) + " type=" + sym.getSymbolType());
                return rva;
            }
        }
        pw.println("// " + className + "::vftable NOT FOUND");
        return 0;
    }

    private void readVtable(PrintWriter pw, long imageBase, long vtableRva, int maxSlots,
                             String className) throws Exception {
        Memory mem = currentProgram.getMemory();
        pw.println("// " + className + " vtable @ RVA 0x" + Long.toHexString(vtableRva));
        for (int i = 0; i < maxSlots; i++) {
            long entryRva = vtableRva + (long)i * 8;
            Address entryAddr = currentProgram.getImageBase().add(entryRva);
            try {
                long fnAbsAddr = mem.getLong(entryAddr);
                if (fnAbsAddr == 0) {
                    pw.println("//   slot " + i + " (+" + String.format("0x%x", i*8) +
                        "): NULL — end of vtable");
                    break;
                }
                long fnRva = fnAbsAddr - imageBase;
                Function fn = getFunctionAt(toAddr(fnAbsAddr));
                if (fn == null) fn = getFunctionContaining(toAddr(fnAbsAddr));
                String fnName = fn != null ? fn.getName() : "(no function)";
                pw.println("//   slot " + String.format("%2d", i) +
                    " (+" + String.format("0x%03x", i*8) + "): RVA 0x" +
                    Long.toHexString(fnRva) + "  " + fnName);
            } catch (Exception e) {
                pw.println("//   slot " + i + ": read error");
                break;
            }
        }
        pw.println();
    }

    private long readVtableSlot(long imageBase, long vtableRva, int slot) throws Exception {
        Memory mem = currentProgram.getMemory();
        long entryRva = vtableRva + (long)slot * 8;
        Address entryAddr = currentProgram.getImageBase().add(entryRva);
        long fnAbsAddr = mem.getLong(entryAddr);
        if (fnAbsAddr == 0) return 0;
        return fnAbsAddr - imageBase;
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
        // Skip duplicates
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");

        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
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
