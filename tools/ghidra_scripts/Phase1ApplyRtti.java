//Phase 1: apply the Phase-0 RTTI map as persistent Ghidra symbols.
//For each class: create a namespace, label its vftable(s), and move/rename its DEFAULT (FUN_)
//virtual methods into the class namespace as vfunc_<slot>. Only touches FUN_ defaults so any
//hand-named function is preserved. Saves into the project (.rep) for all future decompiles.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.util.*;
import java.io.*;

public class Phase1ApplyRtti extends GhidraScript {

    // .?AVFoo@@ / .?AUFoo@@ / template ?$... -> a safe Ghidra identifier
    String cleanClassName(String mangled) {
        String s = mangled;
        if (s.startsWith(".?AV") || s.startsWith(".?AU")) s = s.substring(4);
        if (s.endsWith("@@")) s = s.substring(0, s.length() - 2);
        // sanitize everything else to underscores; collapse runs
        StringBuilder sb = new StringBuilder();
        boolean lastUnder = false;
        for (char c : s.toCharArray()) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                sb.append(c); lastUnder = false;
            } else if (!lastUnder) { sb.append('_'); lastUnder = true; }
        }
        String out = sb.toString();
        while (out.endsWith("_")) out = out.substring(0, out.length() - 1);
        while (out.startsWith("_")) out = out.substring(1);
        if (out.isEmpty()) out = "UnnamedClass";
        if (Character.isDigit(out.charAt(0))) out = "C_" + out;
        return out;
    }

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        SymbolTable st = currentProgram.getSymbolTable();
        FunctionManager fm = currentProgram.getFunctionManager();
        Namespace global = currentProgram.getGlobalNamespace();

        BufferedReader br = new BufferedReader(new FileReader(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase0RttiMap.tsv"));

        Map<String,Namespace> nsCache = new HashMap<>();
        Set<Long> claimed = new HashSet<>();          // function addrs already renamed
        Map<String,Integer> vtblSeenPerClass = new HashMap<>();

        int classes = 0, vtbls = 0, renamed = 0, skippedNamed = 0, rows = 0;
        String line = br.readLine(); // header
        while ((line = br.readLine()) != null) {
            String[] f = line.split("\t", -1);
            if (f.length < 4) continue;
            rows++;
            String cls = cleanClassName(f[0]);
            long vtRva = Long.parseLong(f[1].substring(2), 16);
            String[] mrvas = f[3].isEmpty() ? new String[0] : f[3].split(",");

            // namespace per class
            Namespace ns = nsCache.get(cls);
            if (ns == null) {
                ns = st.getNamespace(cls, global);
                if (ns == null) {
                    ns = st.createNameSpace(global, cls, SourceType.IMPORTED);
                    classes++;
                }
                nsCache.put(cls, ns);
            }

            // label the vftable (suffix subsequent vtables of the same class)
            Address vtAddr = toAddr(imageBase + vtRva);
            int seen = vtblSeenPerClass.getOrDefault(cls, 0);
            String vtName = (seen == 0) ? "vftable" : "vftable_" + Long.toHexString(vtRva);
            vtblSeenPerClass.put(cls, seen + 1);
            try { st.createLabel(vtAddr, vtName, ns, SourceType.IMPORTED); vtbls++; } catch (Exception e) {}

            // rename DEFAULT virtual methods into the class namespace
            for (int slot = 0; slot < mrvas.length; slot++) {
                String mr = mrvas[slot].trim();
                if (mr.isEmpty()) continue;
                long mrva = Long.parseLong(mr.substring(2), 16);
                long mva = imageBase + mrva;
                if (claimed.contains(mva)) continue;
                Function fn = fm.getFunctionAt(toAddr(mva));
                if (fn == null) continue;
                if (!fn.getName().startsWith("FUN_")) { skippedNamed++; claimed.add(mva); continue; }
                try {
                    fn.setParentNamespace(ns);
                    fn.setName("vfunc_" + slot, SourceType.IMPORTED);
                    claimed.add(mva);
                    renamed++;
                } catch (Exception e) {}
            }
        }
        br.close();

        println("rows processed: " + rows);
        println("class namespaces created: " + classes);
        println("vftable labels created: " + vtbls);
        println("DEFAULT methods renamed into classes: " + renamed);
        println("methods already hand-named (preserved): " + skippedNamed);
    }
}
