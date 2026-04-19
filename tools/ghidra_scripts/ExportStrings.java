//Export all defined strings (length > 3) with address to stdout
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
import ghidra.program.model.address.*;

public class ExportStrings extends GhidraScript {
    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        DataIterator it = listing.getDefinedData(true);
        java.io.PrintWriter pw = new java.io.PrintWriter(
            new java.io.FileWriter("/home/ty/src/eaw-reverse-engineering/logs/ghidra_strings.tsv"));
        int count = 0;
        while (it.hasNext()) {
            Data d = it.next();
            DataType dt = d.getDataType();
            if (dt instanceof StringDataType || dt instanceof UnicodeDataType ||
                dt instanceof Unicode32DataType || dt instanceof TerminatedStringDataType ||
                dt instanceof TerminatedUnicodeDataType) {
                try {
                    Object val = d.getValue();
                    if (val != null) {
                        String s = val.toString();
                        if (s.length() > 3) {
                            pw.println(d.getAddress() + "\t" + s);
                            count++;
                        }
                    }
                } catch (Exception e) { /* skip */ }
            }
        }
        pw.close();
        println("Wrote " + count + " strings to logs/ghidra_strings.tsv");
    }
}
