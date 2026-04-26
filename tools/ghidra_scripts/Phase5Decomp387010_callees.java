// Phase5Decomp387010_callees.java
// Decompile callees of FUN_140387010 (the per-movement-component terminal stall carrier).
// FUN_140385cf0: cold-cache path lookup (fires when param_1+0x94 == -1) — prime stall suspect.
// FUN_140381ff0: main movement step — secondary stall suspect.
// FUN_14012d2a0: name→index lookup (SSO string, likely dict/map search).
//
// Output: logs/phase5_387010_callees.c

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5Decomp387010_callees extends GhidraScript {

    static final long[] RVAS  = { 0x385cf0L, 0x381ff0L, 0x12d2a0L };
    static final String[] NAMES = { "FUN_140385cf0", "FUN_140381ff0", "FUN_14012d2a0" };

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_387010_callees.c"));
        long base = currentProgram.getImageBase().getOffset();

        for (int k = 0; k < RVAS.length; k++) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + RVAS[k]);
            Function fn = currentProgram.getListing().getFunctionAt(addr);
            if (fn == null) {
                out.printf("/* %s (RVA 0x%x) — no function at address */\n\n", NAMES[k], RVAS[k]);
                continue;
            }
            out.printf("/* =========================================================\n");
            out.printf(" * %s  (RVA 0x%x)\n", NAMES[k], RVAS[k]);
            out.printf(" * size: %d bytes  params: %d  return: %s\n",
                fn.getBody().getNumAddresses(),
                fn.getParameterCount(),
                fn.getReturnType().getName());
            out.printf(" * ========================================================= */\n\n");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.printf("/* decompile failed: %s */\n\n", res.getErrorMessage());
            }
            out.println();
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase5_387010_callees.c");
    }
}
