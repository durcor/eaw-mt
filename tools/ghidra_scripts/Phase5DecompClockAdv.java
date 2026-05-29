//@category EawMT
import ghidra.app.script.GhidraScript; import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*; import java.io.*;
public class Phase5DecompClockAdv extends GhidraScript {
  public void run() throws Exception {
    DecompInterface d=new DecompInterface(); d.openProgram(currentProgram);
    PrintWriter pw=new PrintWriter(new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/Phase5DecompClockAdv.out"));
    for (long rva : new long[]{0x27c360L}) {
      Function f=getFunctionAt(currentProgram.getImageBase().add(rva));
      DecompileResults r=d.decompileFunction(f,45,monitor);
      pw.println(r!=null&&r.decompileCompleted()?r.getDecompiledFunction().getC():"fail");
    }
    pw.close(); println("done");
  }
}
