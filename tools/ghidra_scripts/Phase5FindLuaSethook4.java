import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

public class Phase5FindLuaSethook4 extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        // Scan all small functions in LuaVM range for writes to 0x98 (hook ptr) AND 0x8c (hookmask)
        long lo = 0x7b0000L, hi = 0x7d0000L;
        FunctionIterator fns = currentProgram.getFunctionManager().getFunctions(
            currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + lo), true);
        while (fns.hasNext()) {
            Function fn = fns.next();
            long rva = fn.getEntryPoint().getOffset() - base;
            if (rva >= hi) break;
            if (fn.getBody().getNumAddresses() > 200) continue;
            DecompileResults res = decomp.decompileFunction(fn, 20, monitor);
            if (!res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            if (c.contains("0x98") && c.contains("0x8c")) {
                println("=== rva=0x" + Long.toHexString(rva) + " name=" + fn.getName() + " ===");
                println(c);
            }
        }
        decomp.dispose();
    }
}
