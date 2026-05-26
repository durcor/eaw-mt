import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class Phase5Decomp12bdf0 extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long rva  = 0x12bdf0L;
        Address addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + rva);
        Function fn = currentProgram.getFunctionManager().getFunctionContaining(addr);
        if (fn == null) { println("No function at rva=0x12bdf0"); return; }
        println("Decompiling: " + fn.getName() + " @ rva=0x"
            + Long.toHexString(fn.getEntryPoint().getOffset() - base));

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        DecompileResults res = decomp.decompileFunction(fn, 60, monitor);
        if (res.decompileCompleted()) {
            println(res.getDecompiledFunction().getC());
        } else {
            println("Decompile failed: " + res.getErrorMessage());
        }
        decomp.dispose();
    }
}
