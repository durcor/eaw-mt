import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

// lua_sethook(L, hook_fn, mask, count) in Lua 5.1 x86-64:
//   stores fn ptr -> L+0x70, mask -> L+0x64, count -> L+0x68 and L+0x6c
// Scan small functions in LuaVM range for decompiled text containing both
// "0x70" and "0x64" to narrow to sethook-like functions.
public class Phase5FindLuaSethook2 extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long lo = 0x7b0000L, hi = 0x7d0000L;
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        FunctionIterator fns = currentProgram.getFunctionManager().getFunctions(
            currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + lo),
            true);
        while (fns.hasNext()) {
            Function fn = fns.next();
            long rva = fn.getEntryPoint().getOffset() - base;
            if (rva >= hi) break;
            if (fn.getBody().getNumAddresses() > 0x80) continue;
            DecompileResults res = decomp.decompileFunction(fn, 30, monitor);
            if (!res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            // lua_sethook writes hook fn at +0x70 AND hookmask at +0x64
            if (c.contains("0x70") && c.contains("0x64") && c.contains("0x6c")) {
                println(String.format("=== MATCH rva=0x%x  name=%s ===", rva, fn.getName()));
                println(c);
            }
        }
        decomp.dispose();
    }
}
