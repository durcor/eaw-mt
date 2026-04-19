// Phase4VtableRefs.java
// Find ALL references to FUN_140180d90 (RVA 0x180d90) including data references
// (vtable slots, function pointer assignments) that static call analysis misses.
// Output: logs/phase4_vtable_refs.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase4VtableRefs extends GhidraScript {

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase4_vtable_refs.txt"));
        long imageBase = currentProgram.getImageBase().getOffset();
        long targetRva = 0x180d90L;
        Address targetAddr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(imageBase + targetRva);

        out.printf("// All references to FUN_140180d90 (RVA 0x%x)\n\n", targetRva);

        ReferenceIterator refs = currentProgram.getReferenceManager()
            .getReferencesTo(targetAddr);

        int count = 0;
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Address from = ref.getFromAddress();
            long fromRva = from.getOffset() - imageBase;
            Function caller = getFunctionContaining(from);
            String callerDesc = (caller != null)
                ? String.format("%s (fn RVA 0x%x)", caller.getName(),
                    caller.getEntryPoint().getOffset() - imageBase)
                : "(no function)";

            out.printf("  RVA 0x%x  type=%-25s  in %s\n",
                fromRva, ref.getReferenceType().getName(), callerDesc);
            count++;
        }

        out.printf("\n// Total references: %d\n", count);
        out.close();
        println("Done — logs/phase4_vtable_refs.txt (" + count + " refs)");
    }
}
