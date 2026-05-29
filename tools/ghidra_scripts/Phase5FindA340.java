//@category EawMT
import ghidra.app.script.GhidraScript; import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*; import ghidra.program.model.listing.*; import java.io.*;
public class Phase5FindA340 extends GhidraScript {
  public void run() throws Exception {
    long base=currentProgram.getImageBase().getOffset();
    PrintWriter pw=new PrintWriter(new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/Phase5FindA340.out"));
    for (long rva : new long[]{0xb0a340L}) {
      Address a=currentProgram.getImageBase().add(rva);
      ReferenceIterator it=currentProgram.getReferenceManager().getReferencesTo(a);
      while(it.hasNext()){ Reference r=it.next(); Function f=getFunctionContaining(r.getFromAddress());
        pw.printf("0x%x %s in %s @0x%x%n", rva, r.getReferenceType(),
          f!=null?f.getName():"?", r.getFromAddress().getOffset()-base); }
    }
    pw.close(); println("done");
  }
}
