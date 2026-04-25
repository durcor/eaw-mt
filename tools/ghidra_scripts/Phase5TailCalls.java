// Phase5TailCalls.java
// Both SpaceModeClass and LandModeClass vtable slot 22 end with an unconditional
// tail call to FUN_1403639d0(param_1, param_2). Also called per-player in the
// list walk: FUN_140527dd0(entity, tick).
//
// FUN_1403639d0 RVA 0x3639d0 — shared base call at end of both mode slot-22 impls
// FUN_140527dd0 RVA 0x527dd0 — per-player entity update called inside the list walk
//
// Output: logs/phase5_tail_calls.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase5TailCalls extends GhidraScript {

    static final long[] RVAS   = { 0x3639d0L, 0x527dd0L };
    static final String[] NAMES = {
        "FUN_1403639d0 — shared tail call in SpaceMode+LandMode slot 22",
        "FUN_140527dd0 — per-player entity update in slot-22 list walk",
    };

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_tail_calls.c"));
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
        println("Done — logs/phase5_tail_calls.c");
    }
}
