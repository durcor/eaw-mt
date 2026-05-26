import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

public class Phase5LuaHookFn extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        // Dump instructions 0x7c6740..0x7c6820 (around the allowhook check at 0x7c6750)
        long start = 0x7c6740L, end = 0x7c6880L;
        Listing listing = currentProgram.getListing();
        InstructionIterator it = listing.getInstructions(
            currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + start), true);
        while (it.hasNext()) {
            Instruction instr = it.next();
            long rva = instr.getAddress().getOffset() - base;
            if (rva > end) break;
            println(String.format("  rva=0x%x  %s", rva, instr.toString()));
        }
    }
}
