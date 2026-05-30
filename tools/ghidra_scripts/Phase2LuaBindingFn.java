//Phase 2: resolve a Lua-exposed binding's C implementation. EaW registers methods as luaL_Reg
//{const char* name; lua_CFunction func} pairs. The data reference to the name-string literal is at
//&reg.name, so the implementation pointer is the qword at (ref + 8). For each string arg, find its
//bytes in memory, find references to it, and report the function pointed to at ref+8.
//Usage: just gs Phase2LuaBindingFn Get_Position Teleport Get_Bone_Position
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

public class Phase2LuaBindingFn extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) { println("need string name args"); return; }
        long base = currentProgram.getImageBase().getOffset();
        Memory mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        ReferenceManager rm = currentProgram.getReferenceManager();

        long textLo = 0, textHi = 0;
        for (MemoryBlock b : mem.getBlocks())
            if (b.getName().equals(".text")) { textLo = b.getStart().getOffset(); textHi = b.getEnd().getOffset()+1; }

        for (String s : args) {
            byte[] needle = (s + "\0").getBytes("US-ASCII");
            Address found = null, start = currentProgram.getMinAddress();
            // search all blocks for the null-terminated literal
            while (true) {
                Address a = mem.findBytes(start, needle, null, true, monitor);
                if (a == null) break;
                // require the byte BEFORE to be 0 (string boundary) to avoid substring hits
                try { if (mem.getByte(a.subtract(1)) == 0) { found = a; break; } } catch (Exception e) { found = a; break; }
                start = a.add(1);
            }
            if (found == null) { println("\"" + s + "\": not found"); continue; }
            println("\"" + s + "\" @ RVA 0x" + Long.toHexString(found.getOffset() - base));
            ReferenceIterator it = rm.getReferencesTo(found);
            int n = 0;
            while (it.hasNext()) {
                Reference r = it.next();
                Address ref = r.getFromAddress();           // &reg.name
                Function inFn = fm.getFunctionContaining(ref);
                String where = inFn != null ? " (in code " + inFn.getName(true) + ")" : "";
                String impl = "";
                try {
                    long fp = mem.getLong(ref.add(8));       // reg.func
                    if (fp >= textLo && fp < textHi) {
                        Function f = fm.getFunctionAt(toAddr(fp));
                        impl = "  -> impl " + (f != null ? f.getName(true) : "?")
                             + " RVA 0x" + Long.toHexString(fp - base);
                    }
                } catch (Exception e) {}
                println(String.format("   ref @0x%x %s%s", ref.getOffset() - base,
                    impl.isEmpty() ? "(no luaL_Reg func at +8)" + where : impl, ""));
                n++;
            }
            if (n == 0) println("   (no references)");
        }
    }
}
