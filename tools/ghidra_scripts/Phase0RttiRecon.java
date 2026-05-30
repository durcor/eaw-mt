//Phase 0 decomp recon: link MSVC RTTI TypeDescriptors -> CompleteObjectLocators -> vftables -> methods.
//Reports how many distinct .text functions are named/reachable via class vtables, and dumps a
//class->vtable->method map. No prior RTTI analysis required (raw memory scan of the x64 RTTI layout).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.listing.*;
import java.util.*;

public class Phase0RttiRecon extends GhidraScript {

    long imageBase;
    Memory mem;

    boolean inText(long va, long textLo, long textHi) { return va >= textLo && va < textHi; }

    @Override
    public void run() throws Exception {
        imageBase = currentProgram.getImageBase().getOffset();
        mem = currentProgram.getMemory();

        // Section bounds
        long textLo = 0, textHi = 0;
        MemoryBlock rdata = null, data = null;
        for (MemoryBlock b : mem.getBlocks()) {
            String n = b.getName();
            if (n.equals(".text")) { textLo = b.getStart().getOffset(); textHi = b.getEnd().getOffset()+1; }
            if (n.equals(".rdata")) rdata = b;
            if (n.equals(".data"))  data  = b;
        }
        println("text=["+Long.toHexString(textLo)+","+Long.toHexString(textHi)+")");

        // --- Pass 1: find TypeDescriptors by their ".?A" mangled name field ---
        // x64 TD layout: +0x00 vfptr(8) +0x08 spare(8) +0x10 name(".?A...@@\0")
        // Map: tdRva(int) -> className
        Map<Integer,String> tdByRva = new HashMap<>();
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            long lo = b.getStart().getOffset(), hi = b.getEnd().getOffset();
            for (long a = lo; a + 4 < hi; a++) {
                // look for '.', '?', 'A'
                if (mem.getByte(toAddr(a)) != (byte)'.') continue;
                if (mem.getByte(toAddr(a+1)) != (byte)'?') continue;
                if (mem.getByte(toAddr(a+2)) != (byte)'A') continue;
                // read the name string
                StringBuilder sb = new StringBuilder();
                long p = a; int max = 512;
                while (max-- > 0) {
                    byte c = mem.getByte(toAddr(p));
                    if (c == 0) break;
                    sb.append((char)(c & 0xff));
                    p++;
                }
                String name = sb.toString();
                if (!name.endsWith("@@")) continue;          // require well-formed mangled name
                long tdVa = a - 0x10;                          // name is at TD+0x10
                long tdRva = tdVa - imageBase;
                if (tdRva < 0 || tdRva > 0xffffffffL) continue;
                tdByRva.put((int)tdRva, name);
                a = p; // skip past this string
            }
        }
        println("TypeDescriptors found: " + tdByRva.size());

        // --- Pass 2: scan .rdata for COL candidates (DWORD == a TD rva at COL+0x0C) ---
        // x64 COL: +0x00 signature(=1) +0x04 offset +0x08 cdOffset +0x0C pTypeDescriptor(rva)
        //          +0x10 pClassDescriptor(rva) +0x14 pSelf(rva of this COL)
        Map<Long,String> colVaToName = new HashMap<>();
        {
            long lo = rdata.getStart().getOffset(), hi = rdata.getEnd().getOffset();
            for (long a = lo; a + 4 <= hi; a += 4) {
                int td = mem.getInt(toAddr(a));               // candidate pTypeDescriptor
                String name = tdByRva.get(td);
                if (name == null) continue;
                long colVa = a - 0x0C;
                if (colVa < lo) continue;
                int sig = mem.getInt(toAddr(colVa));
                if (sig != 1) continue;                        // x64 signature
                int pSelf = mem.getInt(toAddr(colVa + 0x14));
                if ((colVa - imageBase) != (pSelf & 0xffffffffL)) continue; // self-RVA must match
                colVaToName.put(colVa, name);
            }
        }
        println("CompleteObjectLocators (validated): " + colVaToName.size());

        // --- Pass 3: scan .rdata+.data for vftables (QWORD == a COL va, at vftable-8) ---
        FunctionManager fm = currentProgram.getFunctionManager();
        Set<Long> namedFns = new HashSet<>();
        int vtableCount = 0, methodSlots = 0;
        java.io.PrintWriter pw = new java.io.PrintWriter(new java.io.FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase0RttiMap.tsv"));
        pw.println("class\tvtable_rva\tmethod_count\tmethod_rvas");

        MemoryBlock[] scan = (data != null) ? new MemoryBlock[]{rdata, data} : new MemoryBlock[]{rdata};
        for (MemoryBlock blk : scan) {
            long lo = blk.getStart().getOffset(), hi = blk.getEnd().getOffset();
            for (long a = lo; a + 8 <= hi; a += 8) {
                long q = mem.getLong(toAddr(a));
                String name = colVaToName.get(q);
                if (name == null) continue;
                long vtVa = a + 8;                              // first method slot follows the COL ptr
                // walk consecutive method pointers into .text
                List<Long> methods = new ArrayList<>();
                long mp = vtVa;
                while (mp + 8 <= hi) {
                    long fnVa = mem.getLong(toAddr(mp));
                    if (!inText(fnVa, textLo, textHi)) break;
                    methods.add(fnVa);
                    mp += 8;
                    if (methods.size() > 4096) break;
                }
                if (methods.isEmpty()) continue;
                vtableCount++;
                methodSlots += methods.size();
                StringBuilder rvas = new StringBuilder();
                for (long fnVa : methods) {
                    namedFns.add(fnVa);
                    if (rvas.length() > 0) rvas.append(",");
                    rvas.append(String.format("0x%x", fnVa - imageBase));
                }
                pw.println(String.format("%s\t0x%x\t%d\t%s",
                    name, vtVa - imageBase, methods.size(), rvas.toString()));
            }
        }
        pw.close();

        // how many of those method addresses are recognized function entry points
        int atEntry = 0;
        for (long fnVa : namedFns) {
            Function f = fm.getFunctionAt(toAddr(fnVa));
            if (f != null) atEntry++;
        }

        println("vftables found: " + vtableCount);
        println("total method slots: " + methodSlots);
        println("distinct function addresses via vtables: " + namedFns.size());
        println("  ...of which are known function entry points: " + atEntry);
        println("Wrote logs/Phase0RttiMap.tsv");
    }
}
