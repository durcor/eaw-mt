// Phase4RenderCallsite.java
// Find all callers of FUN_140180d90 (render flush wrapper) and the thunk at 0x13D3F0.
// Also decompile FUN_140180d90 and FUN_140149650 (present/swap).
// Output: logs/phase4_render_callsite.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase4RenderCallsite extends GhidraScript {

    private PrintWriter out;
    private DecompInterface decomp;

    @Override
    public void run() throws Exception {
        out = new PrintWriter(new FileWriter("logs/phase4_render_callsite.c"));
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long imageBase = currentProgram.getImageBase().getOffset();

        long[] targets = {
            0x180d90L,  // FUN_140180d90 — render flush wrapper
            0x13D3F0L,  // thunk_FUN_140180d90
            0x149650L,  // FUN_140149650 — present/swap (called from wrapper)
            0x180dc0L,  // FUN_140180dc0 — render flush body (12 passes)
        };

        for (long rva : targets) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(imageBase + rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) {
                out.printf("// RVA 0x%x — no function found at address\n\n", rva);
                continue;
            }

            out.printf("// ================================================================\n");
            out.printf("// RVA 0x%x  Name: %s  Size: %d bytes\n",
                rva, fn.getName(), fn.getBody().getNumAddresses());
            out.printf("// ================================================================\n\n");

            // Callers
            out.printf("// Callers of %s:\n", fn.getName());
            ReferenceIterator refs = currentProgram.getReferenceManager()
                .getReferencesTo(addr);
            int callerCount = 0;
            while (refs.hasNext()) {
                Reference ref = refs.next();
                if (ref.getReferenceType().isCall() || ref.getReferenceType().isJump()) {
                    Address from = ref.getFromAddress();
                    Function caller = getFunctionContaining(from);
                    String callerName = (caller != null) ? caller.getName() : "??";
                    long callerRva = from.getOffset() - imageBase;
                    out.printf("//   call site RVA 0x%x  in %s (RVA 0x%x)\n",
                        callerRva, callerName,
                        caller != null ? caller.getEntryPoint().getOffset() - imageBase : 0);
                    callerCount++;
                }
            }
            if (callerCount == 0) out.printf("//   (no callers found)\n");
            out.printf("\n");

            // Decompile (skip the 2078-byte flush body — too large)
            if (rva != 0x180dc0L) {
                DecompileResults res = decomp.decompileFunction(fn, 60, monitor);
                if (res != null && res.decompileCompleted()) {
                    out.println(res.getDecompiledFunction().getC());
                } else {
                    out.printf("// decompile failed\n");
                }
            }
            out.printf("\n");
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase4_render_callsite.c");
    }
}
