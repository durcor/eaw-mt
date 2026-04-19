// Phase 3n: Find the tactical battle simulation tick via SpaceModeClass/LandModeClass.
// SpaceModeClass::vftable @ RVA 0x8792f8, LandModeClass::vftable @ 0x8673a0.
// For tactical battles, the per-frame unit update (AI + physics) lives in one of these vtable
// slots. Also check GOM cleanup (FUN_14029c800, 1971 bytes — may be init not dtor).
// And look for what directly calls FUN_1403b08c0 (command queue processor, confirmed per-frame).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase3nTacticalTick extends GhidraScript {

    static final long RVA_SPACE_VTABLE = 0x8792f8L;
    static final long RVA_LAND_VTABLE  = 0x8673a0L;

    // GOM "cleanup" — 1971 bytes, likely a per-tick method not destructor
    static final long RVA_GOM_C800     = 0x29c800L;

    // GOM Think candidate: neighbor functions around RVA 0x29c800
    static final long RVA_GOM_B3C0     = 0x29b3c0L; // FUN_14029b3c0 (256 bytes, GOM slot 13)
    static final long RVA_GOM_E730     = 0x29e730L; // FUN_14029e730 (GOM slot 11)

    // Command queue processor caller — find by reading FUN_1403b08c0 xref
    // (approach: decompile functions that call it; we'll instead read large GOM fns)
    static final long RVA_B03E0        = 0x3b03e0L; // FUN_1403b03e0 (phase3h)
    static final long RVA_B08C0        = 0x3b08c0L; // FUN_1403b08c0 (command queue)

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Memory mem = currentProgram.getMemory();

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3n_tactical_tick.c"));

        // 1. SpaceModeClass vtable entries
        section(pw, "SpaceModeClass VTABLE ENTRIES (first 50 slots)");
        readVtable(pw, imageBase, mem, RVA_SPACE_VTABLE, 50, "SpaceModeClass");

        // 2. LandModeClass vtable entries
        section(pw, "LandModeClass VTABLE ENTRIES (first 50 slots)");
        readVtable(pw, imageBase, mem, RVA_LAND_VTABLE, 50, "LandModeClass");

        // 3. Decompile SpaceModeClass vtable slots 1-10 (where Think lives)
        section(pw, "SpaceModeClass VTABLE SLOTS 1-10 DECOMPILES");
        for (int i = 1; i <= 10; i++) {
            long fnRva = readVtableSlot(imageBase, mem, RVA_SPACE_VTABLE, i);
            if (fnRva != 0) decompile(decomp, pw, imageBase, fnRva, 200);
        }

        // 4. GOM cleanup (may be major sim tick method)
        section(pw, "GOM FUN_14029c800 (1971 bytes — possible sim tick or per-frame)");
        decompile(decomp, pw, imageBase, RVA_GOM_C800, 600);

        // 5. Command queue processor (already in phase3g, just for context)
        section(pw, "COMMAND QUEUE PROCESSOR — FUN_1403b08c0");
        decompile(decomp, pw, imageBase, RVA_B08C0, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3n output written to logs/phase3n_tactical_tick.c");
    }

    private void readVtable(PrintWriter pw, long imageBase, Memory mem,
                             long vtableRva, int maxSlots, String className) throws Exception {
        pw.println("// " + className + " vtable @ RVA 0x" + Long.toHexString(vtableRva));
        for (int i = 0; i < maxSlots; i++) {
            long entryRva = vtableRva + (long)i * 8;
            Address entryAddr = currentProgram.getImageBase().add(entryRva);
            try {
                long fnAbsAddr = mem.getLong(entryAddr);
                if (fnAbsAddr == 0) {
                    pw.println("//   slot " + i + ": NULL — end"); break;
                }
                long fnRva = fnAbsAddr - imageBase;
                if (fnRva > 0xc00000L || fnRva < 0) {
                    pw.println("//   slot " + String.format("%2d", i) +
                        " (+" + String.format("0x%03x", i*8) + "): OUT OF RANGE 0x" +
                        Long.toHexString(fnRva)); break;
                }
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

    private long readVtableSlot(long imageBase, Memory mem, long vtableRva, int slot)
            throws Exception {
        long entryRva = vtableRva + (long)slot * 8;
        Address entryAddr = currentProgram.getImageBase().add(entryRva);
        long fnAbsAddr = mem.getLong(entryAddr);
        if (fnAbsAddr == 0) return 0;
        long fnRva = fnAbsAddr - imageBase;
        if (fnRva > 0xc00000L || fnRva < 0) return 0;
        return fnRva;
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
