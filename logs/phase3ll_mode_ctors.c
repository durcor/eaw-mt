
// ================================================================
// VTABLE SLOT READING — data-model pointer resolution
// ================================================================

// Vtable entry sites for FUN_14035cc10 (combat mode tick):
// Assumption: it sits at slot 43 (offset 0x158) in each vtable.
// So vtable_base = entry_rva - 0x158.
// We then read vtable_base[i] for nearby slots to identify function neighbors.

// --- Entry at RVA 0x8651e8 → assumed vtable base 0x865090
//   [0] no data defined at RVA 0x865090
//   [1] no data defined at RVA 0x865098
//   [2] no data defined at RVA 0x8650a0
//   [3] no data defined at RVA 0x8650a8
//   [4] data type: string value: PASSABILITY_COLOR_SETTINGS
//   [5] no data defined at RVA 0x8650b8
//   [6] no data defined at RVA 0x8650c0
//   [7] no data defined at RVA 0x8650c8
//   [8] data type: string value: RADAR_MAP_EVENT
//   [9] no data defined at RVA 0x8650d8
//   [10] data type: string value: Warning: SurfaceFXEventClass::Parse_Database_Entry() - key %s value is TBD.

//   [11] no data defined at RVA 0x8650e8
//   [12] no data defined at RVA 0x8650f0
//   [13] no data defined at RVA 0x8650f8
//   [14] no data defined at RVA 0x865100
//   [15] no data defined at RVA 0x865108
//   [16] no data defined at RVA 0x865110
//   [17] no data defined at RVA 0x865118
//   [18] no data defined at RVA 0x865120
//   [19] no data defined at RVA 0x865128
//   [20] data type: string value: EVENT_SINGLE_INSTANCE
//   [21] no data defined at RVA 0x865138
//   [22] no data defined at RVA 0x865140
//   [23] data type: undefined value: 0x0
//   [24] data type: string value: Warning: RadarMapClass::Parse_Database_Entry() - Unprocessed entry '%s' with value '%s'.

//   [25] no data defined at RVA 0x865158
//   [26] no data defined at RVA 0x865160
//   [27] no data defined at RVA 0x865168
//   [28] no data defined at RVA 0x865170
//   [29] no data defined at RVA 0x865178
//   [30] no data defined at RVA 0x865180
//   [31] no data defined at RVA 0x865188
//   [32] no data defined at RVA 0x865190
//   [33] no data defined at RVA 0x865198
//   [34] no data defined at RVA 0x8651a0
//   [35] no data defined at RVA 0x8651a8
//   [36] → fn RVA 0x90ece8  ?  0b
//   [37] data type: pointer[1] value: null
//   [38] → fn RVA 0x90ed60  ?  0b
//   [39] data type: pointer[89] value: null
//   [40] no data defined at RVA 0x8651d0
//   [41] no data defined at RVA 0x8651d8
//   [42] no data defined at RVA 0x8651e0
//   [43] no data defined at RVA 0x8651e8
//   [44] no data defined at RVA 0x8651f0
//   [45] no data defined at RVA 0x8651f8
//   [46] no data defined at RVA 0x865200
//   [47] no data defined at RVA 0x865208
//   [48] no data defined at RVA 0x865210
//   [49] no data defined at RVA 0x865218
//   [50] no data defined at RVA 0x865220

// --- Entry at RVA 0x8673c0 → assumed vtable base 0x867268
//   [0] no data defined at RVA 0x867268
//   [1] no data defined at RVA 0x867270
//   [2] no data defined at RVA 0x867278
//   [3] no data defined at RVA 0x867280
//   [4] no data defined at RVA 0x867288
//   [5] data type: string value:           Distance From 3D Listener: %6.1f
//   [6] no data defined at RVA 0x867298
//   [7] no data defined at RVA 0x8672a0
//   [8] no data defined at RVA 0x8672a8
//   [9] no data defined at RVA 0x8672b0
//   [10] no data defined at RVA 0x8672b8
//   [11] data type: string value: Mouse Pointer Data:
//   [12] no data defined at RVA 0x8672c8
//   [13] no data defined at RVA 0x8672d0
//   [14] data type: string value:           State: %s
//   [15] no data defined at RVA 0x8672e0
//   [16] no data defined at RVA 0x8672e8
//   [17] data type: unicode value: Cell Diagnostics:
//   [18] no data defined at RVA 0x8672f8
//   [19] no data defined at RVA 0x867300
//   [20] no data defined at RVA 0x867308
//   [21] no data defined at RVA 0x867310
//   [22] data type: string value:            PosX : %f, PosY: %f
//   [23] no data defined at RVA 0x867320
//   [24] no data defined at RVA 0x867328
//   [25] no data defined at RVA 0x867330
//   [26] data type: string value:            CellX : %d, CellY: %d
//   [27] no data defined at RVA 0x867340
//   [28] no data defined at RVA 0x867348
//   [29] no data defined at RVA 0x867350
//   [30] no data defined at RVA 0x867358
//   [31] data type: undefined value: 0x0
//   [32] → fn RVA 0x910d30  ?  0b
//   [33] data type: pointer[2] value: null
//   [34] no data defined at RVA 0x867378
//   [35] → fn RVA 0x910cb0  ?  0b
//   [36] data type: pointer[2] value: null
//   [37] no data defined at RVA 0x867390
//   [38] → fn RVA 0x910db0  ?  0b
//   [39] data type: pointer[89] value: null
//   [40] no data defined at RVA 0x8673a8
//   [41] no data defined at RVA 0x8673b0
//   [42] no data defined at RVA 0x8673b8
//   [43] no data defined at RVA 0x8673c0
//   [44] no data defined at RVA 0x8673c8
//   [45] no data defined at RVA 0x8673d0
//   [46] no data defined at RVA 0x8673d8
//   [47] no data defined at RVA 0x8673e0
//   [48] no data defined at RVA 0x8673e8
//   [49] no data defined at RVA 0x8673f0
//   [50] no data defined at RVA 0x8673f8

// --- Entry at RVA 0x8762e0 → assumed vtable base 0x876188
//   [0] no data defined at RVA 0x876188
//   [1] data type: string value: Warning: CampaignDataClass::Parse_Database_Entry() - Unprocessed entry '%s' in object '%s'.

//   [2] no data defined at RVA 0x876198
//   [3] no data defined at RVA 0x8761a0
//   [4] no data defined at RVA 0x8761a8
//   [5] no data defined at RVA 0x8761b0
//   [6] no data defined at RVA 0x8761b8
//   [7] no data defined at RVA 0x8761c0
//   [8] no data defined at RVA 0x8761c8
//   [9] no data defined at RVA 0x8761d0
//   [10] no data defined at RVA 0x8761d8
//   [11] no data defined at RVA 0x8761e0
//   [12] no data defined at RVA 0x8761e8
//   [13] → fn RVA 0x917280  ?  0b
//   [14] data type: pointer[2] value: null
//   [15] no data defined at RVA 0x876200
//   [16] → fn RVA 0x917350  ?  0b
//   [17] data type: pointer[5] value: null
//   [18] no data defined at RVA 0x876218
//   [19] no data defined at RVA 0x876220
//   [20] no data defined at RVA 0x876228
//   [21] no data defined at RVA 0x876230
//   [22] data type: string value: AI Player
//   [23] no data defined at RVA 0x876240
//   [24] data type: string value:   AI Player Type: 
//   [25] no data defined at RVA 0x876250
//   [26] no data defined at RVA 0x876258
//   [27] data type: string value:   Faction: 
//   [28] no data defined at RVA 0x876268
//   [29] data type: string value: Unrecognized AI player type: %s!

//   [30] no data defined at RVA 0x876278
//   [31] no data defined at RVA 0x876280
//   [32] no data defined at RVA 0x876288
//   [33] no data defined at RVA 0x876290
//   [34] data type: string value: Internal_Dummy_Adjustment
//   [35] no data defined at RVA 0x8762a0
//   [36] no data defined at RVA 0x8762a8
//   [37] no data defined at RVA 0x8762b0
//   [38] → fn RVA 0x9173d0  ?  0b
//   [39] data type: pointer[89] value: null
//   [40] no data defined at RVA 0x8762c8
//   [41] no data defined at RVA 0x8762d0
//   [42] no data defined at RVA 0x8762d8
//   [43] no data defined at RVA 0x8762e0
//   [44] no data defined at RVA 0x8762e8
//   [45] no data defined at RVA 0x8762f0
//   [46] no data defined at RVA 0x8762f8
//   [47] no data defined at RVA 0x876300
//   [48] no data defined at RVA 0x876308
//   [49] no data defined at RVA 0x876310
//   [50] no data defined at RVA 0x876318

// --- Entry at RVA 0x879318 → assumed vtable base 0x8791c0
//   [0] no data defined at RVA 0x8791c0
//   [1] no data defined at RVA 0x8791c8
//   [2] no data defined at RVA 0x8791d0
//   [3] no data defined at RVA 0x8791d8
//   [4] no data defined at RVA 0x8791e0
//   [5] no data defined at RVA 0x8791e8
//   [6] no data defined at RVA 0x8791f0
//   [7] data type: undefined value: 0x0
//   [8] data type: string value: AIFreeStoreClass::Is_Object_On_Free_Store -- Parameter 1 is not a valid object.
//   [9] no data defined at RVA 0x879208
//   [10] no data defined at RVA 0x879210
//   [11] no data defined at RVA 0x879218
//   [12] no data defined at RVA 0x879220
//   [13] no data defined at RVA 0x879228
//   [14] no data defined at RVA 0x879230
//   [15] no data defined at RVA 0x879238
//   [16] no data defined at RVA 0x879240
//   [17] no data defined at RVA 0x879248
//   [18] data type: string value: On_Unit_Added
//   [19] no data defined at RVA 0x879258
//   [20] data type: string value: UnitServiceRate
//   [21] no data defined at RVA 0x879268
//   [22] data type: string value: On_Unit_Service
//   [23] no data defined at RVA 0x879278
//   [24] → fn RVA 0x91a210  ?  0b
//   [25] data type: pointer[2] value: null
//   [26] no data defined at RVA 0x879290
//   [27] → fn RVA 0x91a290  ?  0b
//   [28] data type: pointer[9] value: null
//   [29] no data defined at RVA 0x8792a8
//   [30] no data defined at RVA 0x8792b0
//   [31] no data defined at RVA 0x8792b8
//   [32] no data defined at RVA 0x8792c0
//   [33] no data defined at RVA 0x8792c8
//   [34] no data defined at RVA 0x8792d0
//   [35] no data defined at RVA 0x8792d8
//   [36] no data defined at RVA 0x8792e0
//   [37] → fn RVA 0xfffffffec0000000  ?  0b
//   [38] → fn RVA 0x91a328  ?  0b
//   [39] data type: pointer[89] value: null
//   [40] no data defined at RVA 0x879300
//   [41] no data defined at RVA 0x879308
//   [42] no data defined at RVA 0x879310
//   [43] no data defined at RVA 0x879318
//   [44] no data defined at RVA 0x879320
//   [45] no data defined at RVA 0x879328
//   [46] no data defined at RVA 0x879330
//   [47] no data defined at RVA 0x879338
//   [48] no data defined at RVA 0x879340
//   [49] no data defined at RVA 0x879348
//   [50] no data defined at RVA 0x879350

// --- Entry at RVA 0xb6f858 → assumed vtable base 0xb6f700
//   [0] no data defined at RVA 0xb6f700
//   [1] no data defined at RVA 0xb6f708
//   [2] no data defined at RVA 0xb6f710
//   [3] no data defined at RVA 0xb6f718
//   [4] no data defined at RVA 0xb6f720
//   [5] no data defined at RVA 0xb6f728
//   [6] no data defined at RVA 0xb6f730
//   [7] no data defined at RVA 0xb6f738
//   [8] no data defined at RVA 0xb6f740
//   [9] no data defined at RVA 0xb6f748
//   [10] no data defined at RVA 0xb6f750
//   [11] no data defined at RVA 0xb6f758
//   [12] no data defined at RVA 0xb6f760
//   [13] no data defined at RVA 0xb6f768
//   [14] no data defined at RVA 0xb6f770
//   [15] no data defined at RVA 0xb6f778
//   [16] no data defined at RVA 0xb6f780
//   [17] no data defined at RVA 0xb6f788
//   [18] no data defined at RVA 0xb6f790
//   [19] no data defined at RVA 0xb6f798
//   [20] no data defined at RVA 0xb6f7a0
//   [21] no data defined at RVA 0xb6f7a8
//   [22] no data defined at RVA 0xb6f7b0
//   [23] no data defined at RVA 0xb6f7b8
//   [24] no data defined at RVA 0xb6f7c0
//   [25] no data defined at RVA 0xb6f7c8
//   [26] no data defined at RVA 0xb6f7d0
//   [27] no data defined at RVA 0xb6f7d8
//   [28] no data defined at RVA 0xb6f7e0
//   [29] no data defined at RVA 0xb6f7e8
//   [30] no data defined at RVA 0xb6f7f0
//   [31] no data defined at RVA 0xb6f7f8
//   [32] no data defined at RVA 0xb6f800
//   [33] no data defined at RVA 0xb6f808
//   [34] no data defined at RVA 0xb6f810
//   [35] no data defined at RVA 0xb6f818
//   [36] no data defined at RVA 0xb6f820
//   [37] no data defined at RVA 0xb6f828
//   [38] no data defined at RVA 0xb6f830
//   [39] no data defined at RVA 0xb6f838
//   [40] no data defined at RVA 0xb6f840
//   [41] no data defined at RVA 0xb6f848
//   [42] no data defined at RVA 0xb6f850
//   [43] no data defined at RVA 0xb6f858
//   [44] no data defined at RVA 0xb6f860
//   [45] no data defined at RVA 0xb6f868
//   [46] no data defined at RVA 0xb6f870
//   [47] no data defined at RVA 0xb6f878
//   [48] no data defined at RVA 0xb6f880
//   [49] no data defined at RVA 0xb6f888
//   [50] no data defined at RVA 0xb6f890


// ================================================================
// REFERENCES TO MODE NAME STRINGS
// ================================================================

// String "GalacticMode" at RVA 0x876588:
//   Ref from fn RVA 0x4b2060  FUN_1404b2060  8b
// String "SpaceMode" at RVA 0x8795c0:
//   Ref from fn RVA 0x4d77c0  FUN_1404d77c0  8b
// String "LandMode" at RVA 0x867668:
//   Ref from fn RVA 0x3b8060  FUN_1403b8060  8b
// String "GameModeManagerClass::Start_Game" at RVA 0x85af30:
//   Ref from fn RVA 0x28e890  FUN_14028e890  1189b

// ================================================================
// GalacticMode-referencing functions
// ================================================================

// RVA: 0x4b2060  Name: FUN_1404b2060  Size: 8 bytes

char * FUN_1404b2060(void)

{
  return "GalacticMode";
}


// ================================================================
// SpaceMode-referencing functions
// ================================================================

// RVA: 0x4d77c0  Name: FUN_1404d77c0  Size: 8 bytes

char * FUN_1404d77c0(void)

{
  return "SpaceMode";
}


// ================================================================
// FUN_14022e400 — thread entry fn (called by _beginthreadex)
// ================================================================

// RVA: 0x22e400  Name: FUN_14022e400  Size: 58 bytes

undefined8 FUN_14022e400(longlong *param_1)

{
  DWORD DVar1;
  HANDLE pvVar2;
  
  param_1 = (longlong *)*param_1;
  if (param_1 != (longlong *)0x0) {
    pvVar2 = GetCurrentThread();
    param_1[3] = (longlong)pvVar2;
    DVar1 = GetCurrentThreadId();
    *(DWORD *)(param_1 + 4) = DVar1;
    *(undefined1 *)(param_1 + 1) = 1;
    (**(code **)(*param_1 + 8))(param_1);
    *(undefined1 *)((longlong)param_1 + 9) = 1;
  }
  return 0;
}


// ================================================================
// FUN_14008df00 — LoadThread manager (first 120 lines)
// ================================================================

// RVA: 0x8df00  Name: FUN_14008df00  Size: 2340 bytes

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14008df00(int *param_1)

{
  float fVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong *plVar4;
  char cVar5;
  int iVar6;
  DWORD DVar7;
  ulonglong uVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  uint uVar14;
  longlong lVar15;
  longlong lVar16;
  int iVar17;
  undefined **local_res10;
  char *local_c8 [2];
  undefined8 local_b8;
  ulonglong local_b0;
  undefined **local_a8;
  char local_9f;
  longlong *local_80;
  undefined4 local_78;
  undefined4 local_74;
  char local_70;
  ulonglong local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  int local_48;
  undefined8 local_40;
  
  cVar5 = FUN_1401c1e90(&DAT_140a78510,DAT_140a110d4);
  plVar4 = DAT_140b15418;
  if (cVar5 == '\0') {
    return;
  }
  iVar6 = *param_1;
  if (iVar6 == 0) {
LAB_14008df5a:
    if (DAT_140b15418 != (longlong *)0x0) {
      (**(code **)(*DAT_140b15418 + 0x40))();
    }
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 == 1) {
    plVar11 = (longlong *)(param_1 + 2);
    if (0xf < *(ulonglong *)(param_1 + 8)) {
      plVar11 = *(longlong **)(param_1 + 2);
    }
    if (*(longlong *)(param_1 + 6) != 0x10) {
      return;
    }
    if (*plVar11 != 0x494745425f434449) {
      return;
    }
    if (plVar11[1] != 0x4e4f545455425f4e) {
      return;
    }
    if (DAT_140a28c20 == '\0') {
      if (DAT_140b15418 == (longlong *)0x0) goto LAB_14008e800;
    }
    else {
      FUN_1402d2790(&DAT_140b27dc0);
      FUN_14021cf60(&DAT_140a7d0a0,0);
      FUN_14021ce30(&DAT_140a7d0a0,0,0,0);
      if (plVar4 == (longlong *)0x0) goto LAB_14008e800;
      *(undefined1 *)((longlong)plVar4 + 0x9a) = 0;
    }
    (**(code **)(*plVar4 + 0x40))(plVar4);
LAB_14008e800:
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 != 0xc) {
    if (iVar6 != 0xe) {
      return;
    }
    if ((param_1[0xb] != 0x20) && (param_1[0xb] != 0xd)) {
      return;
    }
    goto LAB_14008df5a;
  }
  if (DAT_140a61d39 == '\0') {
    return;
  }
  DAT_140a61a68 = &LAB_14008e8c0;
  DAT_140a110d0 = 0;
  FUN_14021d0f0(&DAT_140a7d0a0,DAT_140b30328,DAT_140b3032c,DAT_140b30330,DAT_140b30334);
  if (DAT_140a61df8 != (longlong *)0x0) {
    iVar6 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar6 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar6 == 0) {
      FUN_14007fa80();
    }
  }
  plVar4 = DAT_140a61df8;
  uVar9 = DAT_140a61d58;
  iVar6 = DAT_140a61d54;
  uVar3 = DAT_140a110dc;
  uVar2 = DAT_140a110d8;
  puVar10 = &DAT_140a110e8;
  if (0xf < DAT_140a11100) {
    puVar10 = DAT_140a110e8;
  }
  cVar5 = DAT_140a61d3a;
  FUN_14022e310(&local_a8,"LoadThread");
  local_a8 = LoadingThreadClass::vftable;
// ... [truncated at 120 / 365 lines]

