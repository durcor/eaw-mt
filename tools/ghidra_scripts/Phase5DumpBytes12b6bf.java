import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.Memory;

public class Phase5DumpBytes12b6bf extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        Memory mem  = currentProgram.getMemory();
        Listing listing = currentProgram.getListing();

        // dump disassembly from 0x12b6a0 .. 0x12b700 (crash context)
        long start = 0x12b6a0L, end = 0x12b700L;
        println(String.format("Disasm rva=0x%x .. 0x%x", start, end));
        Address a = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + start);
        Address aEnd = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + end);
        InstructionIterator it = listing.getInstructions(a, true);
        while (it.hasNext()) {
            Instruction instr = it.next();
            if (instr.getAddress().compareTo(aEnd) > 0) break;
            long rva = instr.getAddress().getOffset() - base;
            println(String.format("  rva=0x%x  %s  %s",
                rva, instr.getMnemonicString(), instr.getDefaultOperandRepresentation(0) +
                (instr.getNumOperands() > 1 ? ", " + instr.getDefaultOperandRepresentation(1) : "")));
        }
    }
}
