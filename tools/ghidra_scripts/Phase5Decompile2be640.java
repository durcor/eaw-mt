// Phase5Decompile2be640.java
// Decompile FUN_1402be640 (RVA 0x2be640) — confirmed stall source.
// Called twice per FUN_1403639d0 invocation with different first args:
//   FUN_1402be640(param_1[3], uVar1)   — line 143 of tail22 decompile
//   FUN_1402be640(param_1[4], uVar1)   — line 147 of tail22 decompile
// Both calls together account for ~100% of the 1099ms avg / 22808ms max stall.
//
// Output: logs/phase5_2be640.c

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5Decompile2be640 extends GhidraScript {

    static final long[] RVAS  = { 0x2be640L };
    static final String[] NAMES = { "FUN_1402be640" };

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_2be640.c"));
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
        println("Done — logs/phase5_2be640.c");
    }
}
