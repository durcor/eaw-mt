// Lists all functions that call the function at the given RVA(s). Args: hex RVAs.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.RefType;

public class CallersOf extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        long base = currentProgram.getImageBase().getOffset();
        for (String a : args) {
            long rva = Long.parseLong(a.replace("0x",""), 16);
            Address target = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + rva);
            Function tf = getFunctionAt(target);
            println("== callers of 0x" + Long.toHexString(rva) + " (" + (tf!=null?tf.getName():"?") + ") ==");
            int n = 0;
            for (Reference ref : getReferencesTo(target)) {
                if (ref.getReferenceType().isCall() || ref.getReferenceType() == RefType.UNCONDITIONAL_JUMP) {
                    Address from = ref.getFromAddress();
                    Function cf = getFunctionContaining(from);
                    long frva = from.getOffset() - base;
                    println("  call@0x" + Long.toHexString(frva) + "  in  " +
                            (cf!=null?cf.getName()+" (0x"+Long.toHexString(cf.getEntryPoint().getOffset()-base)+")":"<none>"));
                    n++;
                }
            }
            println("  total: " + n);
        }
    }
}
