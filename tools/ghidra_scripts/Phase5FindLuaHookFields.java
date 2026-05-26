import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;

// Search luaV_execute (0x7c6720) for the hookmask test instruction.
// In Lua 5.1: "test byte [L + hookmask_off], (LUA_MASKCOUNT|LUA_MASKCALL)"
// or "cmp/test [reg + off], imm" with imm being 1,2,4,8 or combos.
public class Phase5FindLuaHookFields extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        long vmRva = 0x7c6720L;
        Address vmAddr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + vmRva);
        Function vmFn = currentProgram.getFunctionManager().getFunctionAt(vmAddr);
        if (vmFn == null) { println("no vm fn"); return; }
        println("VM fn: " + vmFn.getName() + " size=" + vmFn.getBody().getNumAddresses());

        Listing listing = currentProgram.getListing();
        InstructionIterator it = listing.getInstructions(vmAddr, true);
        // Scan for TEST BYTE / MOVZX BYTE references that could be hookmask
        // Also look for DEC instructions (--hookcount) and stores of hookcount
        while (it.hasNext()) {
            Instruction instr = it.next();
            long rva = instr.getAddress().getOffset() - base;
            if (rva > vmRva + vmFn.getBody().getNumAddresses()) break;
            String op = instr.getMnemonicString();
            String str = instr.toString();
            // Hook mask: MOVZX/TEST/AND of a byte register from [reg+offset]
            // hookcount: DEC [reg+offset], MOV [reg+off], eax where offset is 4-byte int
            if ((op.equals("TEST") || op.equals("AND") || op.equals("MOVZX")) &&
                str.contains("byte")) {
                println(String.format("  rva=0x%x  %s", rva, str));
            }
            // DEC for hookcount decrement
            if (op.equals("DEC") && !str.contains("EAX") && !str.contains("ECX")) {
                println(String.format("  DEC rva=0x%x  %s", rva, str));
            }
        }
    }
}
