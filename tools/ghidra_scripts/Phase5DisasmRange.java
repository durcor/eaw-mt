//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*; import ghidra.program.model.address.*;
import java.io.*;
public class Phase5DisasmRange extends GhidraScript {
  public void run() throws Exception {
    long base=currentProgram.getImageBase().getOffset();
    PrintWriter pw=new PrintWriter(new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/Phase5DisasmRange.out"));
    Address a=currentProgram.getImageBase().add(0x5f200L);
    Address end=currentProgram.getImageBase().add(0x5f2d6L);
    Listing l=currentProgram.getListing();
    Instruction in=l.getInstructionAt(a);
    while(in!=null && in.getAddress().compareTo(end)<=0){
      pw.printf("0x%-8x %s%n", in.getAddress().getOffset()-base, in.toString());
      in=in.getNext();
    }
    pw.close(); println("done");
  }
}
