import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.Function;

public class Phase5DecompLuaCallhook extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        Address addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + 0x7bbaa0L);
        Function fn = currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("no fn"); return; }
        println("fn: " + fn.getName() + " size=" + fn.getBody().getNumAddresses());
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        DecompileResults res = decomp.decompileFunction(fn, 30, monitor);
        if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
        decomp.dispose();
    }
}
