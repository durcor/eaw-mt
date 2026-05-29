//@category EawMT
import ghidra.app.script.GhidraScript; import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*; import java.io.*;
public class Phase5DecompGameLoop extends GhidraScript {
  public void run() throws Exception {
    DecompInterface d=new DecompInterface(); d.openProgram(currentProgram);
    PrintWriter pw=new PrintWriter(new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/Phase5DecompGameLoop.out"));
    Function f=getFunctionAt(currentProgram.getImageBase().add(0x5d990L));
    DecompileResults r=d.decompileFunction(f,90,monitor);
    pw.println(r!=null&&r.decompileCompleted()?r.getDecompiledFunction().getC():"fail");
    pw.close(); println("done");
  }
}
