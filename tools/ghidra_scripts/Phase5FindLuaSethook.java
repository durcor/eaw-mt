import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;

// lua_sethook(lua_State *L, lua_Hook f, int mask, int count)
// Characteristic: writes to L->hook, L->hookmask, L->hookcount.
// In Lua 5.1, lua_sethook is close to lua_resume in the binary.
// Strategy: search for a function that references both lua_resume's region
// AND stores to small offsets (hook fields in lua_State).
// Simpler: scan .text in LuaVM range (0x7b0000..0x7d0000) for functions
// that take a function-pointer arg and have a small instruction count.
// Actually: look for a function with xref FROM lua_resume's area that matches
// the sethook signature pattern.
//
// Better approach: scan for the string "hook" or look at known Lua 5.1 layout.
// lua_State +0xac = hook (function pointer)
// lua_State +0xb0 = hookmask (int)
// lua_State +0xb4 = hookcount (int)
// lua_State +0xb8 = basehookcount
// Pattern: function stores to L+0xac, L+0xb0, L+0xb4

public class Phase5FindLuaSethook extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        // lua_sethook is near lua_resume (0x7bc310). Search 0x7b0000..0x7d0000.
        long lo = 0x7b0000L, hi = 0x7d0000L;
        FunctionIterator fns = currentProgram.getFunctionManager().getFunctions(
            currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + lo),
            true);
        int found = 0;
        while (fns.hasNext()) {
            Function fn = fns.next();
            long rva = fn.getEntryPoint().getOffset() - base;
            if (rva >= hi) break;
            // Check if this function has ≤50 instructions (sethook is tiny ~8 instrs)
            // and body size ≤ 0x60
            long bodySize = fn.getBody().getNumAddresses();
            if (bodySize > 0x60) continue;
            println(String.format("  candidate rva=0x%x  size=%d  name=%s",
                rva, bodySize, fn.getName()));
            found++;
        }
        println("Total small fns in LuaVM range: " + found);
    }
}
