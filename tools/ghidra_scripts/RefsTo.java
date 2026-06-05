//Lists ALL references (data read/write/ptr + calls) to the given RVA(s). Usage: just gs RefsTo b2f1f0 ...
//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

public class RefsTo extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        for (String a : getScriptArgs()) {
            long rva = Long.parseLong(a.replace("0x",""), 16);
            Address target = toAddr(base + rva);
            println("== refs to 0x" + Long.toHexString(rva) + " ==");
            int n = 0;
            for (Reference ref : getReferencesTo(target)) {
                Address from = ref.getFromAddress();
                Function cf = getFunctionContaining(from);
                long frva = from.getOffset() - base;
                println("  " + ref.getReferenceType() + " from 0x" + Long.toHexString(frva) +
                        "  in " + (cf!=null?cf.getName()+" (0x"+Long.toHexString(cf.getEntryPoint().getOffset()-base)+")":"<none>"));
                n++;
            }
            println("  total: " + n);
        }
    }
}
