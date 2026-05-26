import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

// Decompile candidates from 0x7b9440..0x7ba000 (ldebug.c region).
// lua_sethook writes hook fn ptr + mask (lu_byte) + count (int) to L.
// Any function writing both a pointer and a byte to fixed offsets of arg0 is a match.
public class Phase5FindLuaSethook3 extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long[] candidates = {
            0x7b9440L, 0x7b9460L, 0x7b9480L, 0x7b9510L, 0x7b9520L,
            0x7b95e0L, 0x7b9600L, 0x7b9640L, 0x7b9720L, 0x7b9770L,
            0x7b97c0L, 0x7b9820L, 0x7b9880L, 0x7b98e0L, 0x7b9920L,
            0x7b9930L, 0x7b9990L, 0x7b9a60L, 0x7b9ab0L, 0x7b9b10L
        };
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        for (long rva : candidates) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + rva);
            Function fn = currentProgram.getFunctionManager().getFunctionAt(addr);
            if (fn == null) { println("no fn @ 0x" + Long.toHexString(rva)); continue; }
            DecompileResults res = decomp.decompileFunction(fn, 30, monitor);
            if (!res.decompileCompleted()) { println("fail @ 0x" + Long.toHexString(rva)); continue; }
            String c = res.getDecompiledFunction().getC();
            println("=== rva=0x" + Long.toHexString(rva) + " size=" + fn.getBody().getNumAddresses() + " ===");
            println(c);
        }
        decomp.dispose();
    }
}
