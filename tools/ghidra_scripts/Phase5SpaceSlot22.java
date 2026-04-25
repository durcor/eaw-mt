// Phase5SpaceSlot22.java
// Decompile FUN_1404d95a0 (RVA 0x4d95a0, 358 bytes) — SpaceModeClass vtable slot 22.
// This is the live dispatch target in FUN_14028d400 during the space battle demo,
// called after ScoringManager::Service. Primary suspect for 1000ms episodic stalls
// with no disk I/O and no WaitMessage.
//
// Also decompile FUN_1403bb440 (RVA 0x3bb440, 598 bytes) — LandModeClass slot 22,
// for completeness.
//
// Output: logs/phase5_space_slot22.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase5SpaceSlot22 extends GhidraScript {

    static final long[] RVAS   = { 0x4d95a0L, 0x3bb440L };
    static final String[] NAMES = {
        "FUN_1404d95a0 — SpaceModeClass vtable slot 22",
        "FUN_1403bb440 — LandModeClass  vtable slot 22",
    };

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_space_slot22.c"));
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        long imageBase = currentProgram.getImageBase().getOffset();

        for (int k = 0; k < RVAS.length; k++) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(imageBase + RVAS[k]);
            Function fn = getFunctionAt(addr);

            out.printf("\n/* =========================================================\n");
            out.printf(" * %s\n", NAMES[k]);
            out.printf(" * RVA 0x%x\n", RVAS[k]);
            if (fn == null) {
                out.printf(" * ERROR: no function at this address\n */\n\n");
                continue;
            }
            out.printf(" * size: %d bytes  params: %d  return: %s\n",
                fn.getBody().getNumAddresses(),
                fn.getParameterCount(),
                fn.getReturnType().getName());
            out.printf(" * ========================================================= */\n\n");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res != null && res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed");
            }
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase5_space_slot22.c");
    }
}
