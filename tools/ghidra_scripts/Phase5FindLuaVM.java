import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

public class Phase5FindLuaVM extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long lo = 0x7b0000L, hi = 0x7d0000L;
        // Find the largest function in the LuaVM range (= luaV_execute, the opcode interpreter)
        Function bigFn = null; long bigSize = 0;
        FunctionIterator fns = currentProgram.getFunctionManager().getFunctions(
            currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + lo), true);
        while (fns.hasNext()) {
            Function fn = fns.next();
            long rva = fn.getEntryPoint().getOffset() - base;
            if (rva >= hi) break;
            long sz = fn.getBody().getNumAddresses();
            if (sz > bigSize) { bigSize = sz; bigFn = fn; }
        }
        if (bigFn == null) { println("no fn found"); return; }
        long bigRva = bigFn.getEntryPoint().getOffset() - base;
        println(String.format("Largest: %s rva=0x%x size=%d", bigFn.getName(), bigRva, bigSize));

        // Disassemble the first 200 instructions looking for hookmask/hookcount references
        Listing listing = currentProgram.getListing();
        InstructionIterator it = listing.getInstructions(bigFn.getEntryPoint(), true);
        int n = 0;
        while (it.hasNext() && n < 600) {
            Instruction instr = it.next();
            long rva = instr.getAddress().getOffset() - base;
            String line = instr.toString();
            // print instructions that reference offsets in range 0x60-0x80 (hook fields)
            if (line.matches(".*0x6[0-9a-f].*") || line.matches(".*0x7[0-9].*") ||
                line.matches(".*hookmask.*") || line.matches(".*hookcount.*")) {
                println(String.format("  rva=0x%x  %s", rva, line));
            }
            n++;
        }
    }
}
