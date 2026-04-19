//Find game loop: locate callers of Sleep/QueryPerformanceCounter and the entry point call chain
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import ghidra.program.model.pcode.*;
import java.util.*;
import java.io.*;

public class FindGameLoop extends GhidraScript {

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/game_loop_candidates.txt"));

        pw.println("=== Entry Point ===");
        Address ep = currentProgram.getImageBase().add(
            currentProgram.getMinAddress().getOffset());
        long epRva = currentProgram.getLanguage().getDefaultCompilerSpec()
            .getStackPointer().getAddress().getOffset();
        // Use the actual PE entry point
        Address entry = currentProgram.getSymbolTable()
            .getSymbols("entry").hasNext()
            ? currentProgram.getSymbolTable().getSymbols("entry").next().getAddress()
            : null;
        if (entry != null) {
            pw.println("entry: 0x" + Long.toHexString(entry.getOffset() - imageBase));
            Function entryFn = getFunctionAt(entry);
            if (entryFn != null) pw.println("  -> " + entryFn.getName());
        }

        pw.println("\n=== Callers of Sleep ===");
        findCallers("Sleep", imageBase, pw, 3);

        pw.println("\n=== Callers of QueryPerformanceCounter ===");
        findCallers("QueryPerformanceCounter", imageBase, pw, 5);

        pw.println("\n=== Callers of QueryPerformanceFrequency ===");
        findCallers("QueryPerformanceFrequency", imageBase, pw, 3);

        pw.println("\n=== Functions referencing 'Main Thread' string ===");
        findStringRefs("Main Thread", imageBase, pw);

        pw.println("\n=== Functions referencing 'LoadThread' string ===");
        findStringRefs("LoadThread", imageBase, pw);

        pw.println("\n=== Functions referencing 'FrameSynchronizer' string ===");
        findStringRefs("FrameSynchronizer", imageBase, pw);

        pw.close();
        println("Written to logs/game_loop_candidates.txt");
    }

    private void findCallers(String importName, long imageBase, PrintWriter pw, int depth) throws Exception {
        SymbolIterator it = currentProgram.getSymbolTable().getSymbols(importName);
        while (it.hasNext()) {
            Symbol sym = it.next();
            Address addr = sym.getAddress();
            pw.println("  " + importName + " @ 0x" + Long.toHexString(addr.getOffset() - imageBase));
            ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(addr);
            int count = 0;
            while (refs.hasNext() && count < 30) {
                Reference ref = refs.next();
                if (ref.getReferenceType().isCall() || ref.getReferenceType().isData()) {
                    Address from = ref.getFromAddress();
                    Function fn = getFunctionContaining(from);
                    if (fn != null) {
                        long rva = fn.getEntryPoint().getOffset() - imageBase;
                        pw.println("    called from: FUN_0x" + Long.toHexString(rva)
                            + " (call site 0x" + Long.toHexString(from.getOffset() - imageBase) + ")");
                    }
                    count++;
                }
            }
        }
    }

    private void findStringRefs(String searchStr, long imageBase, PrintWriter pw) throws Exception {
        DataIterator dit = currentProgram.getListing().getDefinedData(true);
        while (dit.hasNext()) {
            Data d = dit.next();
            Object val = d.getValue();
            if (val != null && val.toString().contains(searchStr)) {
                ReferenceIterator refs = currentProgram.getReferenceManager()
                    .getReferencesTo(d.getAddress());
                while (refs.hasNext()) {
                    Reference ref = refs.next();
                    Address from = ref.getFromAddress();
                    Function fn = getFunctionContaining(from);
                    if (fn != null) {
                        long rva = fn.getEntryPoint().getOffset() - imageBase;
                        pw.println("  \"" + val.toString() + "\" referenced in FUN_0x"
                            + Long.toHexString(rva));
                    }
                }
            }
        }
    }
}
