import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;
import java.util.ArrayList;

/**
 * Phase5FindLuaGC — locate luaC_step and luaC_fullgc.
 *
 * Strategy:
 *   luaM_realloc (known: RVA 0x7bf7e0) accesses G(L)+0x74 (totalbytes).
 *   luaC_step and luaC_fullgc WRITE to G(L)+0x70 (GCthreshold, one dword before).
 *
 *   Scan all functions in a conservative range around the known Lua area
 *   (RVA 0x780000 – 0x7e0000).  For each function:
 *     1. Scan bytes for 0x70 appearing as an 8-bit displacement (heuristic).
 *     2. Scan bytes for 0x74 appearing as an 8-bit displacement (heuristic).
 *     3. If both appear and function body is in [16, 512] bytes → candidate.
 *   Decompile candidates and log.
 *
 *   Also explicitly decompile functions immediately before and after luaM_realloc
 *   in case luaC_step is adjacent.
 */
public class Phase5FindLuaGC extends GhidraScript {
    static final long IMG_BASE   = 0x140000000L;
    static final long RVA_LO     = 0x780000L;
    static final long RVA_HI     = 0x7e0000L;
    static final int  MIN_BODY   = 16;
    static final int  MAX_BODY   = 600;

    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        FileWriter fw = new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5FindLuaGC.log");

        long addrLo = IMG_BASE + RVA_LO;
        long addrHi = IMG_BASE + RVA_HI;

        // Always decompile these known neighbours for cross-reference
        long[] pinned = {
            0x1407bf7e0L,  // luaM_realloc (confirmed)
            0x1407bfd90L,  // luaH_get (confirmed)
        };
        for (long a : pinned) {
            decompAndLog(decomp, fw, a, "pinned");
        }

        // Scan all functions in range
        Address lo = currentProgram.getAddressFactory().getAddress(
            String.format("0x%x", addrLo));
        Address hi = currentProgram.getAddressFactory().getAddress(
            String.format("0x%x", addrHi));

        FunctionIterator it = currentProgram.getFunctionManager()
            .getFunctions(lo, true);

        ArrayList<Long> candidates = new ArrayList<>();

        while (it.hasNext()) {
            Function fn = it.next();
            long entry = fn.getEntryPoint().getOffset();
            if (entry < addrLo || entry >= addrHi) break;

            long bodyLen = fn.getBody().getNumAddresses();
            if (bodyLen < MIN_BODY || bodyLen > MAX_BODY) continue;

            // Read function bytes
            byte[] bytes = new byte[(int) bodyLen];
            try {
                currentProgram.getMemory().getBytes(fn.getEntryPoint(), bytes);
            } catch (MemoryAccessException e) {
                continue;
            }

            // Heuristic: count occurrences of 0x70 and 0x74 as byte values
            // in the body.  Displacement bytes appear often in Lua GC code.
            int cnt70 = 0, cnt74 = 0;
            for (int i = 0; i < bytes.length - 1; i++) {
                int b0 = bytes[i] & 0xFF;
                int b1 = bytes[i+1] & 0xFF;
                // Look for 0x70 or 0x74 following a ModRM byte whose Mod field
                // is 01 (8-bit disp): ModRM = 0b01xxxxxx (0x40..0x7F or 0x80+)
                // Simplified: just count raw occurrences as displacement candidate.
                // A byte of 0x70 or 0x74 following ModRM 0x40-0x7F (mod=01 pattern).
                if ((b0 & 0xC0) == 0x40) {  // mod=01 in ModRM
                    if (b1 == 0x70) cnt70++;
                    if (b1 == 0x74) cnt74++;
                }
                // Also REX prefix followed by instruction with 0x70/0x74 disp
                if (b0 == 0x48 || b0 == 0x49 || b0 == 0x4C || b0 == 0x4D) {
                    if (i + 2 < bytes.length) {
                        int b2 = bytes[i+2] & 0xFF;
                        int b3 = (i + 3 < bytes.length) ? bytes[i+3] & 0xFF : 0;
                        if ((b2 & 0xC0) == 0x40) {
                            if (b3 == 0x70) cnt70++;
                            if (b3 == 0x74) cnt74++;
                        }
                    }
                }
            }

            if (cnt70 >= 1 && cnt74 >= 1) {
                candidates.add(entry);
            }
        }

        fw.write(String.format("/* Candidate count: %d in RVA [%x, %x] */\n\n",
            candidates.size(), RVA_LO, RVA_HI));

        // Decompile all candidates
        for (long addr : candidates) {
            decompAndLog(decomp, fw, addr, "candidate");
        }

        fw.close();
        println("Done — " + candidates.size() + " candidates");
    }

    private void decompAndLog(DecompInterface decomp, FileWriter fw,
                               long addr, String tag) throws Exception {
        Address a = currentProgram.getAddressFactory().getAddress(
            String.format("0x%x", addr));
        Function fn = currentProgram.getFunctionManager().getFunctionContaining(a);
        if (fn == null) {
            fw.write(String.format("/* [%s] no function at 0x%x */\n\n", tag, addr));
            return;
        }
        long entry = fn.getEntryPoint().getOffset();
        long rva   = entry - IMG_BASE;
        long size  = fn.getBody().getNumAddresses();
        fw.write(String.format("/* [%s] RVA=0x%x  entry=0x%x  size=%d */\n",
            tag, rva, entry, size));
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        if (res.getDecompiledFunction() != null) {
            fw.write(res.getDecompiledFunction().getC());
        } else {
            fw.write("/* decompile failed: " + res.getErrorMessage() + " */\n");
        }
        fw.write("\n\n");
    }
}
