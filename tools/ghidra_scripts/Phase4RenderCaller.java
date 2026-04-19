// Phase4RenderCaller.java
// Decompile FUN_1401b2ae0 (caller of the render flush thunk) and find its callers.
// Output: logs/phase4_render_caller.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase4RenderCaller extends GhidraScript {

    private PrintWriter out;
    private DecompInterface decomp;

    @Override
    public void run() throws Exception {
        out = new PrintWriter(new FileWriter("logs/phase4_render_caller.c"));
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long imageBase = currentProgram.getImageBase().getOffset();

        // FUN_1401b2ae0 — caller of thunk_FUN_140180d90
        long rva = 0x1b2ae0L;
        Address addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(imageBase + rva);
        Function fn = getFunctionAt(addr);

        if (fn == null) {
            out.printf("// No function at RVA 0x%x\n", rva);
            out.close();
            return;
        }

        out.printf("// ================================================================\n");
        out.printf("// RVA 0x%x  Name: %s  Size: %d bytes\n",
            rva, fn.getName(), fn.getBody().getNumAddresses());
        out.printf("// ================================================================\n\n");

        // Callers
        out.printf("// Callers of %s:\n", fn.getName());
        ReferenceIterator refs = currentProgram.getReferenceManager()
            .getReferencesTo(addr);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall() || ref.getReferenceType().isJump()) {
                Address from = ref.getFromAddress();
                Function caller = getFunctionContaining(from);
                String callerName = (caller != null) ? caller.getName() : "??";
                long callerRva = from.getOffset() - imageBase;
                long callerFnRva = caller != null ? caller.getEntryPoint().getOffset() - imageBase : 0;
                out.printf("//   call site RVA 0x%x  in %s (fn RVA 0x%x)\n",
                    callerRva, callerName, callerFnRva);
            }
        }
        out.printf("\n");

        // Decompile
        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
        if (res != null && res.decompileCompleted()) {
            out.println(res.getDecompiledFunction().getC());
        } else {
            out.printf("// decompile failed\n");
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase4_render_caller.c");
    }
}
