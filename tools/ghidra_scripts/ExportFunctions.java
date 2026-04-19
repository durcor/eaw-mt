//Export all functions with RVA and name to stdout
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;

public class ExportFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator it = fm.getFunctions(true);
        java.io.PrintWriter pw = new java.io.PrintWriter(
            new java.io.FileWriter("/home/ty/src/eaw-reverse-engineering/logs/functions.tsv"));
        int count = 0;
        while (it.hasNext()) {
            Function fn = it.next();
            long va  = fn.getEntryPoint().getOffset();
            long rva = va - imageBase;
            String name   = fn.getName();
            String source = fn.getSymbol().getSource().toString();
            pw.println(String.format("0x%08X\t%s\t%s", rva, name, source));
            count++;
        }
        pw.close();
        println("Wrote " + count + " functions to logs/functions.tsv");
    }
}
