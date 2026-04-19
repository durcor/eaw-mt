// Phase4VtableOwner.java
// Identify the vtable containing the DATA reference to FUN_140180d90 at RVA 0xb56478.
// Output: logs/phase4_vtable_owner.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase4VtableOwner extends GhidraScript {

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase4_vtable_owner.txt"));
        long imageBase = currentProgram.getImageBase().getOffset();
        long dataRva = 0xb56478L;
        Address dataAddr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(imageBase + dataRva);

        out.printf("// DATA ref to FUN_140180d90 at RVA 0x%x\n\n", dataRva);

        // Dump 16 pointer slots before and 4 after to find vtable start + RTTI
        out.printf("// Surrounding 8-byte pointer slots:\n");
        for (int i = -16; i <= 4; i++) {
            Address slot = dataAddr.add((long)i * 8);
            long slotRva = slot.getOffset() - imageBase;
            long value;
            try {
                value = currentProgram.getMemory().getLong(slot);
            } catch (Exception e) {
                continue;
            }
            long valueRva = value - imageBase;

            // Symbol at the value address
            String sym = "";
            Address valAddr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(value);
            Function fn = getFunctionAt(valAddr);
            if (fn != null) {
                sym = " -> " + fn.getName();
            } else {
                SymbolIterator si = currentProgram.getSymbolTable()
                    .getSymbolsAsIterator(valAddr);
                if (si.hasNext()) sym = " -> " + si.next().getName();
            }

            String marker = (i == 0) ? "  <-- FUN_140180d90" : "";
            out.printf("  slot[%+3d]  slotRVA=0x%x  value=0x%x (valueRVA=0x%x)%s%s\n",
                i, slotRva, value, valueRva, sym, marker);
        }

        // Nearby symbols from the symbol table
        out.printf("\n// Symbols in RVA range [0x%x, 0x%x]:\n",
            dataRva - 0x300, dataRva + 0x100);
        Address rangeStart = dataAddr.subtract(0x300);
        Address rangeEnd   = dataAddr.add(0x100);
        SymbolIterator syms = currentProgram.getSymbolTable()
            .getSymbolIterator(rangeStart, true);
        while (syms.hasNext()) {
            Symbol s = syms.next();
            if (s.getAddress().compareTo(rangeEnd) > 0) break;
            long symRva = s.getAddress().getOffset() - imageBase;
            out.printf("  RVA 0x%x  %s\n", symRva, s.getName());
        }

        out.close();
        println("Done — logs/phase4_vtable_owner.txt");
    }
}
