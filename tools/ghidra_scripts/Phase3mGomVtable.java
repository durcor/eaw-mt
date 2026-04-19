// Phase 3m: Find GameObjectManagerClass vtable and its Think/Service method.
// The GOM constructor sets *param_1 = GameObjectManagerClass::vftable.
// The Think/Service slot on the GOM should be slot 1 or 2 (the per-frame tick).
// Also search for the GameObjectClass vftable to find Think on individual objects.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase3mGomVtable extends GhidraScript {

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3m_gom_vtable.c"));

        // 1. Find vtables via namespace search
        section(pw, "VTABLE SYMBOL SEARCH");
        long gomVtableRva = findVtable(pw, imageBase, "GameObjectManagerClass");
        long gameObjVtableRva = findVtable(pw, imageBase, "GameObjectClass");
        long refListVtableRva = findVtable(pw, imageBase, "ReferenceListClass");

        // 2. Read GameObjectManagerClass vtable (first 40 slots)
        if (gomVtableRva != 0) {
            section(pw, "GameObjectManagerClass VTABLE ENTRIES");
            readVtable(pw, imageBase, gomVtableRva, 40, "GameObjectManagerClass");
        }

        // 3. Read GameObjectClass vtable (first 20 slots — contains Think)
        if (gameObjVtableRva != 0) {
            section(pw, "GameObjectClass VTABLE ENTRIES");
            readVtable(pw, imageBase, gameObjVtableRva, 20, "GameObjectClass");
        }

        // 4. Decompile GOM vtable slots 0-10 to find Think_All_Objects
        if (gomVtableRva != 0) {
            section(pw, "GameObjectManagerClass VTABLE DECOMPILES (slots 0-15)");
            for (int i = 0; i < 16; i++) {
                long fnRva = readVtableSlot(imageBase, gomVtableRva, i);
                if (fnRva != 0 && fnRva != readVtableSlot(imageBase, gomVtableRva, 0)) {
                    decompile(decomp, pw, imageBase, fnRva, 150);
                } else if (fnRva != 0) {
                    decompile(decomp, pw, imageBase, fnRva, 150);
                }
            }
        }

        // 5. Decompile GameObjectClass vtable slots 0-10 to find Think
        if (gameObjVtableRva != 0) {
            section(pw, "GameObjectClass VTABLE DECOMPILES (slots 0-10)");
            for (int i = 0; i <= 10; i++) {
                long fnRva = readVtableSlot(imageBase, gameObjVtableRva, i);
                if (fnRva != 0) {
                    decompile(decomp, pw, imageBase, fnRva, 100);
                }
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3m output written to logs/phase3m_gom_vtable.c");
    }

    private long findVtable(PrintWriter pw, long imageBase, String className) throws Exception {
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator allSyms = symTable.getAllSymbols(false);
        while (allSyms.hasNext()) {
            Symbol sym = allSyms.next();
            Namespace ns = sym.getParentNamespace();
            String symName = sym.getName();
            if (symName.equals("vftable") && ns != null && ns.getName().equals(className)) {
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// " + className + "::vftable @ RVA 0x" + Long.toHexString(rva));
                return rva;
            }
            if (symName.equals(className + "::vftable")) {
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// " + symName + " @ RVA 0x" + Long.toHexString(rva));
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
                    pw.println("//   slot " + i + ": NULL — end of vtable"); break;
                }
                long fnRva = fnAbsAddr - imageBase;
                Function fn = getFunctionAt(toAddr(fnAbsAddr));
                if (fn == null) fn = getFunctionContaining(toAddr(fnAbsAddr));
                String fnName = fn != null ? fn.getName() : "(no function)";
                pw.println("//   slot " + String.format("%2d", i) +
                    " (+" + String.format("0x%03x", i*8) + "): RVA 0x" +
                    Long.toHexString(fnRva) + "  " + fnName);
            } catch (Exception e) {
                pw.println("//   slot " + i + ": read error"); break;
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
