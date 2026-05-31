//Phase 2: apply the recovered struct layouts as persistent Ghidra data types.
//Creates StructureDataTypes for the confirmed sim-core classes (GameObjectClass, HardPointClass,
//LocomotorBehaviorClass, GameObjectManagerClass) + a few container/placeholder helpers, then retypes
//the FIRST parameter of the key sim-tick functions so the decompiler renders named struct fields
//(entity->pos_x instead of *(float*)(entity+0x78)) in every downstream decompile. Idempotent:
//re-running REPLACEs the types in place. Saves into the project (.rep).
//
//IMPORTANT design decision (resolves a documented contradiction in movement_structs.md):
//  The +0x48 motion-state cluster and the +0x60 std::string node-name were empirically attributed to
//  GameObjectClass, but an MSVC std::string at +0x60 puts its capacity word at +0x78 — which collides
//  with the position floats at +0x78/+0x7c/+0x80 that FOUR independent readers (and in-game DIFFTRACE)
//  confirm. GameObjectClass has its vtable at +0x0; *(HardPoint+0x20) is non-polymorphic (no vtable).
//  => the +0x48 cluster + +0x60 string belong to a SEPARATE owner/render component, NOT GameObjectClass.
//  This script encodes that split: GameObjectClass keeps position at +0x78; the cluster goes on
//  HardPointOwnerRecord (class name TBD). The two are structurally incompatible at offset 0.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.pcode.*;
import ghidra.program.model.symbol.SourceType;
import java.util.*;

public class Phase2ApplyStructs extends GhidraScript {

    DataTypeManager dtm;
    DecompInterface dc;
    CategoryPath CAT = new CategoryPath("/EawMT");
    int placed = 0, placeFail = 0, retyped = 0, retypeFail = 0;

    // primitives
    DataType U1, U4, U8, I4, F4, VOIDP, PTRSZ;

    // a fixed-size struct shell, REPLACE-resolved into the dtm so cross-refs can point at it
    Structure shell(String name, int size, String comment) throws Exception {
        StructureDataType s = new StructureDataType(CAT, name, size, dtm);
        if (comment != null) s.setDescription(comment);
        return (Structure) dtm.addDataType(s, DataTypeConflictHandler.REPLACE_HANDLER);
    }

    // place a named, typed field; never overlaps because we only touch distinct offsets with gaps
    void put(Structure s, int off, DataType dt, String name, String comment) {
        try {
            s.replaceAtOffset(off, dt, dt.getLength(), name, comment);
            placed++;
        } catch (Exception e) {
            placeFail++;
            println("  ! field " + s.getName() + "+0x" + Integer.toHexString(off) + " (" + name + "): " + e.getMessage());
        }
    }

    Pointer ptr(DataType dt) { return dtm.getPointer(dt); }

    // Recover the function's signature from the decompiler (it has no committed params under
    // -noanalysis), then rebuild it keeping the decompiler's storage/arity but overriding the
    // datatype of the parameters given in `overrides` (index -> type; nulls left as-recovered).
    void typeFunction(long rva, String label, Map<Integer,DataType> overrides) {
        try {
            long base = currentProgram.getImageBase().getOffset();
            Function fn = getFunctionAt(toAddr(base + rva));
            if (fn == null) { println("  ! no function at 0x" + Long.toHexString(rva) + " (" + label + ")"); retypeFail++; return; }

            DecompileResults res = dc.decompileFunction(fn, 60, monitor);
            if (res == null || res.getHighFunction() == null) {
                println("  ! decompile failed for " + fn.getName() + " (" + label + ")"); retypeFail++; return;
            }
            LocalSymbolMap lsm = res.getHighFunction().getLocalSymbolMap();
            int n = lsm.getNumParams();
            List<Variable> params = new ArrayList<>();
            for (int i = 0; i < n; i++) {
                HighSymbol hs = lsm.getParamSymbol(i);
                VariableStorage storage = hs.getStorage();
                DataType dt = overrides.get(i);
                if (dt == null) dt = hs.getDataType();
                String nm = hs.getName();
                params.add(new ParameterImpl(nm, dt, storage, currentProgram));
            }
            fn.updateFunction(null, null, params, FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);
            retyped++;
            println("  typed " + fn.getName() + " (" + n + " params) :: " + label);
        } catch (Exception e) {
            retypeFail++;
            println("  ! typeFunction 0x" + Long.toHexString(rva) + ": " + e.getMessage());
        }
    }

    Map<Integer,DataType> ov(Object... kv) {
        Map<Integer,DataType> m = new HashMap<>();
        for (int i = 0; i < kv.length; i += 2) m.put((Integer) kv[i], (DataType) kv[i + 1]);
        return m;
    }

    @Override
    public void run() throws Exception {
        dtm = currentProgram.getDataTypeManager();
        dc = new DecompInterface();
        dc.openProgram(currentProgram);
        U1 = new UnsignedCharDataType();
        U4 = new UnsignedIntegerDataType();
        U8 = new UnsignedLongLongDataType();
        I4 = new IntegerDataType();
        F4 = new FloatDataType();
        VOIDP = ptr(VoidDataType.dataType);
        PTRSZ = VOIDP;

        // ---- helper / container placeholders ------------------------------------------------
        // MSVC std::string (32 bytes): _Bx union(16) | _Mysize(8) | _Myres(8)
        Structure stdstr = shell("eaw_std_string", 0x20, "MSVC std::string: SSO buf[16], size, capacity");
        put(stdstr, 0x00, new ArrayDataType(new CharDataType(), 16, 1), "_Bx", "SSO buffer / heap ptr (when cap>=16)");
        put(stdstr, 0x10, U8, "_Mysize", "length");
        put(stdstr, 0x18, U8, "_Myres", "capacity");

        // ReferenceListClass node-list head (0x48 stride observed between GOM list members)
        Structure reflist = shell("ReferenceListClass", 0x48,
            "Intrusive ref list. node+0x8=next, node+0x18=entity ref (entity = *(node+0x18)-0x18)");
        put(reflist, 0x00, VOIDP, "vtable", null);
        put(reflist, 0x08, VOIDP, "sentinel", "list sentinel node (== &this+0x8)");
        put(reflist, 0x10, VOIDP, "head", "first node; walk via node+0x8");

        // ---- HardPointOwnerRecord (the *(HardPoint+0x20) record; class TBD) -----------------
        // Holds the render/state cluster earlier mis-attributed to GameObjectClass. Non-polymorphic.
        Structure owner = shell("HardPointOwnerRecord", 0x88,
            "*(HardPointClass+0x20) owner/state record. NON-POLYMORPHIC (no vtable at +0). Class TBD. "
          + "Carries the motion-state/active/render-node cluster + node-name string used by 387400/385cf0.");
        put(owner, 0x48, I4, "motion_state", "read as *(int*)(rec+0x48); [5,10] gate in 387400");
        put(owner, 0x4d, U1, "active", "==1 => serviced (387400 / a76b0 filter)");
        put(owner, 0x4e, U1, "render_node_gate", "==1 => live Alamo scene node; ==0 for all sim-serviced comps");
        put(owner, 0x50, U1, "state_flag", "gates path branches in 387400");
        put(owner, 0x60, stdstr, "node_name", "keys the entity into the Alamo node manager");

        // ---- GameObjectClass (the sim entity) ----------------------------------------------
        Structure go = shell("GameObjectClass", 0x3b0,
            "Sim entity. vtable at +0; authoritative world position floats at +0x78/+0x7c/+0x80.");
        put(go, 0x00, VOIDP, "vtable", "primary vtable (0x8661b8)");
        put(go, 0x08, VOIDP, "vtable_8",  "secondary base vtable (MI)");
        put(go, 0x18, VOIDP, "vtable_18", "secondary base vtable (MI)");
        put(go, 0x28, VOIDP, "vtable_28", "secondary base vtable (MI)");
        put(go, 0x38, VOIDP, "listener_list", "subscription/listener list (cross-entity write target)");
        put(go, 0x78, F4, "pos_x", "world X (locomotor vfunc_6 writes; 4 confirmed readers)");
        put(go, 0x7c, F4, "pos_y", "world Y");
        put(go, 0x80, F4, "pos_z", "world Z");
        put(go, 0xa8, VOIDP, "locomotor_state", "locomotor state record (+0x48 state enum, +0x54/58/5c/60 timers)");
        put(go, 0x278, VOIDP, "behaviors", "ptr to array of BehaviorClass* (slot-6 ticked each frame)");
        put(go, 0x290, I4, "behavior_count", "count for the +0x278 behavior array (low byte read in 3a6b80)");
        put(go, 0x2d0, VOIDP, "hardpoints", "DynamicVectorClass<HardPointClass*> (inline; serviced by a76b0)");
        put(go, 0x2d8, VOIDP, "lua_ctx", "Lua context; *ctx+0x58 = lua_State*");
        put(go, 0x3a0, U4, "flags", "flag word (bit 0x40 = coordinator disabled, per dt_fold_coordinator)");

        // ---- HardPointClass (weapon fire-control element; coord+0x2d0 element) --------------
        Structure hp = shell("HardPointClass", 0xd0,
            "Weapon hardpoint / fire-control record (vtable 0x865de8). NOT a movement component.");
        put(hp, 0x00, VOIDP, "vtable", "0x865de8");
        put(hp, 0x10, VOIDP, "context", "context/owner ptr (-> +0x298/+0x2b0 in 387400)");
        put(hp, 0x20, ptr(owner), "owner_record", "owner/state record (non-poly); see HardPointOwnerRecord");
        put(hp, 0x28, F4, "fire_rate_weight", "distribution weight (3a76b0 numerator)");
        put(hp, 0x40, VOIDP, "target_slot", "cleared on target-death event in 386a90");
        put(hp, 0x48, VOIDP, "target", "current target (target+0x10 compared)");
        put(hp, 0x50, VOIDP, "opp_target_slot", "secondary/opportunity target slot");
        put(hp, 0x58, F4, "fire_rate_weight2", "weight read by 540070");
        put(hp, 0x60, I4, "last_serviced_tick", "387010: delta = tick - this");
        put(hp, 0x6c, U1, "enable_flag", null);
        put(hp, 0x6d, U1, "in_progress_flag", "set/cleared around target step in 387400");
        put(hp, 0x6e, U1, "flag_6e", null);
        put(hp, 0x90, I4, "cached_id", "name->id via 12d520 (render path; unused here)");
        put(hp, 0x94, I4, "cached_bone_idx", "name->idx via 12d2a0 (<0 => unresolved)");
        put(hp, 0x98, I4, "cached_bone_idx2", "cached secondary bone index");
        put(hp, 0xa8, VOIDP, "target_slot_a8", "cleared in 386a90");
        put(hp, 0xc0, VOIDP, "order_object", "order/target object (alloc'd lazily; +0xcc reset to 0)");

        // ---- LocomotorBehaviorClass (the real mover) ---------------------------------------
        Structure loco = shell("LocomotorBehaviorClass", 0x40,
            "Locomotor behavior base (vtable 0x8adda0). vfunc_6 = per-tick position integrator.");
        put(loco, 0x00, VOIDP, "vtable", "0x8adda0 base; derived override slot 6");
        put(loco, 0x28, ptr(go), "owner", "back-ref to the owning GameObjectClass it moves");

        // ---- GameObjectManagerClass (the GOM) ----------------------------------------------
        Structure gom = shell("GameObjectManagerClass", 0x600,
            "GOM (vtable 0x85b9a8). Master per-tick update list at +0xe8.");
        put(gom, 0x00, VOIDP, "vtable", "0x85b9a8");
        put(gom, 0xe8, reflist, "master_update_list", "ReferenceList<GameObject> iterated each tick by 2be640");
        put(gom, 0x130, reflist, "second_update_list", "second update pass list");
        put(gom, 0x178, VOIDP, "category_list_178", "ReferenceList<GameObject> category/index view (inline; label TBD)");
        put(gom, 0x1c8, VOIDP, "category_list_1c8", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x210, VOIDP, "category_list_210", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x258, VOIDP, "category_list_258", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x2a0, VOIDP, "category_list_2a0", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x2e8, VOIDP, "category_list_2e8", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x330, VOIDP, "category_list_330", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x378, VOIDP, "category_list_378", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x3d8, VOIDP, "ref_vector", "DynamicVectorRefClass<GameObjectClass*> view (inline; internals TBD)");
        put(gom, 0x408, VOIDP, "owning_multilist", "MultiLinkedListClass<GameObjectClass> primary collection (inline)");
        put(gom, 0x560, VOIDP, "category_list_560", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x5a8, VOIDP, "category_list_5a8", "ReferenceList<GameObject> category/index view (label TBD)");
        put(gom, 0x5f0, VOIDP, "creation_params", "DynamicVectorClass<CreationParams> deferred-creation (inline)");

        // ---- retype the key sim-tick function parameters ------------------------------------
        println("--- retyping function parameters ---");
        Pointer goP = ptr(go), gomP = ptr(gom), hpP = ptr(hp), locoP = ptr(loco);
        typeFunction(0x3a6b80, "FUN_1403a6b80 per-GameObject update: entity", ov(0, goP));
        typeFunction(0x2be640, "FUN_1402be640 GOM iterator: gom",            ov(0, gomP));
        typeFunction(0x3a76b0, "FUN_1403a76b0 hardpoint service: coord(ship)", ov(0, goP));
        typeFunction(0x387400, "FUN_140387400 opp-target acquire: hardpoint", ov(0, hpP));
        typeFunction(0x387010, "FUN_140387010 hardpoint service-inner: hardpoint", ov(0, hpP));
        // locomotor vfunc_6 bodies: (behavior, entity, tick)
        typeFunction(0x6236b0, "StarshipLocomotor::vfunc_6: (behavior, entity)", ov(0, locoP, 1, goP));
        typeFunction(0x61e930, "WalkLocomotor::vfunc_6: (behavior, entity)",     ov(0, locoP, 1, goP));

        println("=== Phase2ApplyStructs done ===");
        println("fields placed: " + placed + "   field failures: " + placeFail);
        println("params retyped: " + retyped + "   retype failures: " + retypeFail);
        println("data types created under " + CAT.getPath() + ": GameObjectClass, HardPointClass, "
              + "HardPointOwnerRecord, LocomotorBehaviorClass, GameObjectManagerClass, ReferenceListClass, eaw_std_string");
    }
}
