import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.Function;

public class Phase5DecompLuaThrow extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        // Decompile small candidates near lua_resume that could be luaD_throw/lua_error
        long[] rvas = { 0x7bca30L, 0x7bc470L, 0x7bc580L, 0x7bbbc0L };
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        for (long rva : rvas) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + rva);
            Function fn = currentProgram.getFunctionManager().getFunctionAt(addr);
            if (fn == null) { println("no fn @ 0x" + Long.toHexString(rva)); continue; }
            DecompileResults res = decomp.decompileFunction(fn, 30, monitor);
            if (!res.decompileCompleted()) continue;
            println("=== rva=0x" + Long.toHexString(rva) + " size=" + fn.getBody().getNumAddresses() + " ===");
            println(res.getDecompiledFunction().getC());
        }
        decomp.dispose();
    }
}
