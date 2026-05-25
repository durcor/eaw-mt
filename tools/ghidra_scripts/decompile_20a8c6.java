import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.*;

public class decompile_20a8c6 extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long rva = 0x20a8c6L;
        Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + rva);
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionContaining(addr);
        if (fn == null) {
            println("No function contains 0x" + Long.toHexString(base + rva));
            return;
        }
        println("Function: " + fn.getName() + " at " + fn.getEntryPoint() +
                " (RVA 0x" + Long.toHexString(fn.getEntryPoint().getOffset() - base) + ")");
        println("Function size: " + fn.getBody().getNumAddresses() + " bytes");
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        DecompileResults r = decomp.decompileFunction(fn, 60, monitor);
        if (r.decompileCompleted()) {
            println(r.getDecompiledFunction().getC());
        }
    }
}
