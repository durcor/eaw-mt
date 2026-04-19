// Phase 3l: Find the GameObjectManagerClass simulation tick via its constructor.
// The operational GOM is allocated in FUN_14035e560 (base-mode init) via:
//   param_1[3] = FUN_14029a0c0(alloc 0x640) @ offset +0x18
// FUN_14029a0c0 is the GOM ctor — it sets the vftable, revealing Think_All_Objects.
// FUN_14029c800 is the GOM cleanup (called in base-slot-2 destructor).
// FUN_1402af130 registers players with GOM.
// FUN_1402a74f0 sets up GOM (skirmish flag).
// FUN_14030ac40 is called from the Light_Loop music handler — may be per-frame dispatch.
// Also check FUN_140305890 (scene post-call, 930 bytes, from phase3d) for GOM iteration.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3lGomCtor extends GhidraScript {

    // GOM constructor — sets vftable with Think_All_Objects slot
    static final long RVA_GOM_CTOR    = 0x29a0c0L; // FUN_14029a0c0

    // GOM cleanup (destructor path)
    static final long RVA_GOM_DTOR    = 0x29c800L; // FUN_14029c800

    // GOM registration / setup calls
    static final long RVA_AF130       = 0x2af130L; // FUN_1402af130 (register players)
    static final long RVA_A74F0       = 0x2a74f0L; // FUN_1402a74f0 (skirmish setup)

    // Called from Light_Loop handler at end of FUN_140305c40
    static final long RVA_30AC40      = 0x30ac40L; // FUN_14030ac40

    // GOM per-frame Think candidates (functions adjacent to FUN_1402a9ff0)
    static final long RVA_A9FF0       = 0x2a9ff0L; // FUN_1402a9ff0 (Find_All_By_Type)
    static final long RVA_A87D0       = 0x2a87d0L; // FUN_1402a87d0 (find nearest planet?)
    static final long RVA_AACA0       = 0x2aaca0L; // neighbor after A9FF0
    static final long RVA_A8FF0       = 0x2a8ff0L; // neighbor before A9FF0

    // Scene post-call — large function (phase3d, 930 bytes)
    static final long RVA_305890      = 0x305890L; // FUN_140305890

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3l_gom_ctor.c"));

        section(pw, "GOM CONSTRUCTOR — FUN_14029a0c0 (sets vftable)");
        decompile(decomp, pw, imageBase, RVA_GOM_CTOR, 400);

        section(pw, "GOM DESTRUCTOR/CLEANUP — FUN_14029c800");
        decompile(decomp, pw, imageBase, RVA_GOM_DTOR, 200);

        section(pw, "GOM PLAYER REGISTRATION — FUN_1402af130");
        decompile(decomp, pw, imageBase, RVA_AF130, 200);

        section(pw, "GOM SKIRMISH SETUP — FUN_1402a74f0");
        decompile(decomp, pw, imageBase, RVA_A74F0, 200);

        section(pw, "LIGHT_LOOP ENDPOINT — FUN_14030ac40");
        decompile(decomp, pw, imageBase, RVA_30AC40, 400);

        section(pw, "Find_All_By_Type — FUN_1402a9ff0 (for vtable offset reference)");
        decompile(decomp, pw, imageBase, RVA_A9FF0, 200);

        section(pw, "GOM NEIGHBOR — FUN_1402a87d0");
        decompile(decomp, pw, imageBase, RVA_A87D0, 200);

        section(pw, "GOM NEIGHBOR after A9FF0 — FUN_1402aaca0");
        decompile(decomp, pw, imageBase, RVA_AACA0, 200);

        section(pw, "GOM NEIGHBOR before A9FF0 — FUN_1402a8ff0");
        decompile(decomp, pw, imageBase, RVA_A8FF0, 200);

        section(pw, "SCENE POST-CALL — FUN_140305890 (930 bytes)");
        decompile(decomp, pw, imageBase, RVA_305890, 500);

        pw.close();
        decomp.dispose();
        println("Phase 3l output written to logs/phase3l_gom_ctor.c");
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
