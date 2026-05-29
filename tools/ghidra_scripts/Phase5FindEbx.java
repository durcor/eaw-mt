//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*; import ghidra.program.model.lang.Register;
import java.io.*;
public class Phase5FindEbx extends GhidraScript {
  public void run() throws Exception {
    long base=currentProgram.getImageBase().getOffset();
    PrintWriter pw=new PrintWriter(new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/Phase5FindEbx.out"));
    Function f=getFunctionContaining(currentProgram.getImageBase().add(0x5f2d1L));
    Register ebx=currentProgram.getRegister("EBX");
    InstructionIterator it=currentProgram.getListing().getInstructions(f.getBody(),true);
    while(it.hasNext()){
      Instruction in=it.next();
      String m=in.getMnemonicString();
      Register d=in.getRegister(0);
      // writes to EBX/RBX (skip CALL/PUSH/CMP/TEST)
      if(d!=null && (d.equals(ebx)||d.getName().equals("RBX")) &&
         !m.startsWith("CMP")&&!m.startsWith("TEST")&&!m.startsWith("PUSH")){
        pw.printf("0x%-8x %s%n", in.getAddress().getOffset()-base, in.toString());
      }
    }
    pw.close(); println("done");
  }
}
