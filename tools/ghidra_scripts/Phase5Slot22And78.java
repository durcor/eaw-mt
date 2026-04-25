// Phase5Slot22And78.java
// Two questions from Opus analysis of the lines 831-869 stall:
//
// Q1: What do SpaceModeClass and LandModeClass put at vtable slot 22 (offset 0xb0)?
//     If it's the base-class no-op (FUN_14076af38), the income-tick path is dead
//     during a space battle and the stall is in slot 78 instead.
//
// Q2: Decompile FUN_140364f30 (slot 78, base-class, 529 bytes).
//     This runs in ALL modes; the caller has a timeGetTime watchdog immediately
//     after, implying the engine itself expects it can be slow.
//
// Output: logs/phase5_slot22_78.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase5Slot22And78 extends GhidraScript {

    // Known vtable bases (from openspec / phase3i logs)
    static final long SPACE_VTABLE_RVA = 0x8792f8L;
    static final long LAND_VTABLE_RVA  = 0x8673a0L;
    static final long GALACTIC_VTABLE_RVA = 0x86dab0L; // for reference
    static final int  SLOT_22_OFFSET   = 0xb0;         // slot 22 * 8
    static final int  SLOT_78_OFFSET   = 0x270;        // slot 78 * 8

    // Slot 78 target to decompile
    static final long SLOT78_RVA = 0x364f30L;

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_slot22_78.c"));
        long imageBase = currentProgram.getImageBase().getOffset();
        AddressSpace as = currentProgram.getAddressFactory().getDefaultAddressSpace();

        // ----------------------------------------------------------------
        // Q1: Read slot 22 entries from Space, Land, Galactic vtables
        // ----------------------------------------------------------------
        out.println("/* =================================================================");
        out.println(" * Vtable slot 22 (offset 0xb0) for each GameMode subclass");
        out.println(" * Base no-op = FUN_14076af38 (RVA 0x76af38)");
        out.println(" * =================================================================*/\n");

        long[][] vtables = {
            { GALACTIC_VTABLE_RVA, 0 },
            { SPACE_VTABLE_RVA,    0 },
            { LAND_VTABLE_RVA,     0 },
        };
        String[] vtableNames = { "GalacticModeClass", "SpaceModeClass", "LandModeClass" };

        for (int i = 0; i < vtables.length; i++) {
            long vtableVA = imageBase + vtables[i][0];
            Address slot22addr = as.getAddress(vtableVA + SLOT_22_OFFSET);
            Address slot78addr = as.getAddress(vtableVA + SLOT_78_OFFSET);

            // Read 8-byte function pointer at slot 22
            long slot22ptr = currentProgram.getMemory().getLong(slot22addr);
            long slot78ptr = currentProgram.getMemory().getLong(slot78addr);
            long slot22rva = slot22ptr - imageBase;
            long slot78rva = slot78ptr - imageBase;

            Function fn22 = getFunctionAt(as.getAddress(slot22ptr));
            Function fn78 = getFunctionAt(as.getAddress(slot78ptr));

            out.printf("// %s\n", vtableNames[i]);
            out.printf("//   slot 22 (0xb0) → 0x%016x  RVA 0x%x  %s  (%d bytes)\n",
                slot22ptr, slot22rva,
                fn22 != null ? fn22.getName() : "???",
                fn22 != null ? (int)fn22.getBody().getNumAddresses() : -1);
            out.printf("//   slot 78 (0x270) → 0x%016x  RVA 0x%x  %s  (%d bytes)\n\n",
                slot78ptr, slot78rva,
                fn78 != null ? fn78.getName() : "???",
                fn78 != null ? (int)fn78.getBody().getNumAddresses() : -1);
        }

        // ----------------------------------------------------------------
        // Q2: Decompile FUN_140364f30 (slot 78 base-class)
        // ----------------------------------------------------------------
        out.println("\n/* =================================================================");
        out.println(" * FUN_140364f30  RVA 0x364f30  slot 78 base-class — all modes");
        out.println(" * Caller has timeGetTime watchdog immediately after this dispatch,");
        out.println(" * implying the engine expects this can be slow.");
        out.println(" * =================================================================*/\n");

        Address slot78addr = as.getAddress(imageBase + SLOT78_RVA);
        Function fn78base = getFunctionAt(slot78addr);
        if (fn78base == null) {
            out.println("// ERROR: no function at RVA 0x364f30");
        } else {
            out.printf("// size: %d bytes  params: %d  return: %s\n\n",
                fn78base.getBody().getNumAddresses(),
                fn78base.getParameterCount(),
                fn78base.getReturnType().getName());
            DecompInterface decomp = new DecompInterface();
            decomp.openProgram(currentProgram);
            DecompileResults res = decomp.decompileFunction(fn78base, 120, monitor);
            if (res != null && res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed");
            }
            decomp.dispose();
        }

        out.close();
        println("Done — logs/phase5_slot22_78.c");
    }
}
