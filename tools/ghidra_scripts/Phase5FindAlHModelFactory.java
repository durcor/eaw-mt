import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

public class Phase5FindAlHModelFactory extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long rva = 0x12bdf0L;
        Address target = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + rva);
        Function targetFn = currentProgram.getFunctionManager()
            .getFunctionContaining(target);
        if (targetFn == null) { println("No function at rva=0x12bdf0"); return; }
        println("Target: " + targetFn.getName() + " @ rva=0x"
            + Long.toHexString(targetFn.getEntryPoint().getOffset() - base));

        ReferenceManager rm = currentProgram.getReferenceManager();
        ReferenceIterator refs = rm.getReferencesTo(targetFn.getEntryPoint());
        int count = 0;
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Address from = ref.getFromAddress();
                long fromRva = from.getOffset() - base;
                Function caller = currentProgram.getFunctionManager()
                    .getFunctionContaining(from);
                String callerName = (caller != null) ? caller.getName() : "<unknown>";
                long callerRva = (caller != null)
                    ? caller.getEntryPoint().getOffset() - base : 0;
                println(String.format("  caller rva=0x%x  site=0x%x  fn=%s",
                    callerRva, fromRva, callerName));
                count++;
            }
        }
        println("Total call sites: " + count);
    }
}
