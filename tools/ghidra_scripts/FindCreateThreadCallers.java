//Find all callers of CreateThread and decompile them — for Lua OS-thread vs coroutine question
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class FindCreateThreadCallers extends GhidraScript {

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/createthread_callers.c"));

        // Find the CreateThread import
        ExternalManager extMgr = currentProgram.getExternalManager();
        SymbolTable symTable = currentProgram.getSymbolTable();

        // Search for CreateThread thunk or import by name
        SymbolIterator it = symTable.getSymbols("CreateThread");
        Set<Long> callerRVAs = new LinkedHashSet<>();

        while (it.hasNext()) {
            Symbol sym = it.next();
            pw.println("// CreateThread symbol: " + sym.getName() +
                " @ 0x" + Long.toHexString(sym.getAddress().getOffset()) +
                " type=" + sym.getSymbolType());

            ReferenceIterator refs = currentProgram.getReferenceManager()
                .getReferencesTo(sym.getAddress());
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Address fromAddr = ref.getFromAddress();
                Function fn = getFunctionContaining(fromAddr);
                if (fn == null) continue;
                long rva = fn.getEntryPoint().getOffset() - imageBase;
                pw.println("//   called from FUN_0x" + Long.toHexString(rva) +
                    " (site 0x" + Long.toHexString(fromAddr.getOffset() - imageBase) + ")");
                callerRVAs.add(rva);
            }
        }

        pw.println("\n// Total CreateThread callers: " + callerRVAs.size() + "\n");

        for (long rva : callerRVAs) {
            Address addr = currentProgram.getImageBase().add(rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) fn = getFunctionContaining(addr);
            if (fn == null) continue;

            pw.println("// ============================================================");
            pw.println("// RVA: 0x" + Long.toHexString(rva) +
                "  Size: " + fn.getBody().getNumAddresses() + " bytes");
            pw.println("// ============================================================");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
            } else {
                pw.println("// DECOMPILE FAILED: " + res.getErrorMessage());
            }
            pw.println();
        }

        pw.close();
        decomp.dispose();
        println("CreateThread callers written to logs/createthread_callers.c — " +
            callerRVAs.size() + " callers");
    }
}
