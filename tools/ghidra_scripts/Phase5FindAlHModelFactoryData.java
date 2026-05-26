import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

public class Phase5FindAlHModelFactoryData extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long rva = 0x12bdf0L;
        Address target = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + rva);
        Function targetFn = currentProgram.getFunctionManager()
            .getFunctionContaining(target);
        if (targetFn == null) { println("No function at rva=0x12bdf0"); return; }

        ReferenceManager rm = currentProgram.getReferenceManager();
        ReferenceIterator refs = rm.getReferencesTo(targetFn.getEntryPoint());
        int count = 0;
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Address from = ref.getFromAddress();
            long fromRva = from.getOffset() - base;
            RefType rt = ref.getReferenceType();
            Function caller = currentProgram.getFunctionManager()
                .getFunctionContaining(from);
            String callerName = (caller != null) ? caller.getName() : "<data>";
            long callerRva = (caller != null)
                ? caller.getEntryPoint().getOffset() - base : 0;
            println(String.format("  type=%-12s from=0x%x  fn=%s@0x%x",
                rt.getName(), fromRva, callerName, callerRva));
            count++;
        }
        println("Total refs: " + count);
    }
}
