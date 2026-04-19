
// ================================================================
// VTABLE BASE DISCOVERY VIA DATA REFS TO GetName FUNCTIONS
// ================================================================

// Mode: GalacticMode GetName=RVA 0x4b2060
//   Data ref (vtable entry?) at RVA 0x8763a8
//   Entry at 0x8763a8 is in static data (vtable in .rdata)
//   Nearby vtable entries (slots -2 to +5 relative to this entry):

// Mode: SpaceMode GetName=RVA 0x4d77c0
//   Data ref (vtable entry?) at RVA 0x8793e0
//   Entry at 0x8793e0 is in static data (vtable in .rdata)
//   Nearby vtable entries (slots -2 to +5 relative to this entry):

// Mode: LandMode GetName=RVA 0x3b8060
//   Data ref (vtable entry?) at RVA 0x867488
//   Entry at 0x867488 is in static data (vtable in .rdata)
//   Nearby vtable entries (slots -2 to +5 relative to this entry):


// ================================================================
// GameModeManagerClass::Start_Game (FUN_14028e890)
// ================================================================

// RVA: 0x28e890  Name: FUN_14028e890  Size: 1189 bytes

void FUN_14028e890(longlong param_1,int param_2,longlong *param_3)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  DWORD DVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  int iVar9;
  longlong *plVar10;
  undefined **local_res8;
  longlong local_48 [3];
  ulonglong local_30;
  
  plVar8 = (longlong *)0x0;
  *(undefined1 *)(param_1 + 0xb1) = 1;
  DAT_140a2ab00._0_4_ = 0;
  *(undefined8 *)(param_1 + 0x1d0) = 0;
  *(undefined1 *)(param_1 + 0x1d8) = 0;
  *(undefined4 *)(param_1 + 0x1dc) = 0;
  *(undefined2 *)(param_1 + 0x1e1) = 0;
  *(undefined1 *)(param_1 + 0x1e0) = 0;
  *(undefined1 *)(param_1 + 0x1e4) = 0;
  uVar5 = FUN_14028a6f0();
  FUN_1404e67d0(uVar5);
  if (param_3 == (longlong *)0x0) {
    plVar10 = (longlong *)&DAT_14085af28;
  }
  else {
    DAT_140b313e8 = 0;
    plVar10 = param_3;
    if (0xf < (ulonglong)param_3[3]) {
      plVar10 = (longlong *)*param_3;
    }
  }
  puVar6 = (undefined8 *)
           (**(code **)(*DAT_140b310b0 + 0x10))(DAT_140b310b0,local_48,(longlong)param_2);
  if (0xf < (ulonglong)puVar6[3]) {
    puVar6 = (undefined8 *)*puVar6;
  }
  FUN_140025760("GameModeManagerClass::Start_Game -- Starting %s Mode: %s.\n",puVar6,plVar10);
  if (0xf < local_30) {
    if (0xfff < local_30 + 1) {
      if (0x1f < (local_48[0] - *(longlong *)(local_48[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  if (*(int *)(param_1 + 500) == -1) {
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_140321830(&DAT_140b2b300,0);
  }
  FUN_140323660(&DAT_140b2b3b0);
  FUN_1403567c0(&DAT_140b2bcb0);
  bVar3 = false;
  if (*(int *)(param_1 + 0x50) == 0) {
    bVar3 = true;
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_14027d020(&DAT_140b0a320);
    DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe0);
    if (*(char *)(param_1 + 0xe8) != '\0') {
      DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe4);
    }
    *(undefined4 *)(param_1 + 0x2c) = 0;
    uVar5 = FUN_14028a5d0();
    FUN_1404d9d40(uVar5);
    FUN_140293390(&DAT_140a16fd0,0);
    FUN_1404907c0(param_1 + 0x180,"PENDING_TACTICAL_BATTLE_VOTE");
    FUN_1404908b0(param_1 + 0x180);
  }
  else {
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x50))();
  }
  if (param_2 == 0) {
    lVar7 = FUN_140769c58(0x378);
    if (lVar7 == 0) goto LAB_14028ec28;
    plVar8 = (longlong *)FUN_1404b1270(lVar7);
  }
  else {
    if (param_2 == 1) {
      lVar7 = FUN_140769c58(0x430);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1403b5210(lVar7);
      }
    }
    else {
      if (param_2 != 2) goto LAB_14028ec28;
      lVar7 = FUN_140769c58(0x3c8);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1404d6ba0(lVar7);
      }
    }
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    DVar4 = timeGetTime();
    *(DWORD *)(param_1 + 0x1e8) = DVar4;
  }
  if (plVar8 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x50);
    iVar9 = *(int *)(param_1 + 0x54) * 2 >> 1;
    if (iVar9 <= iVar1) {
      iVar2 = iVar9 / 10;
      if (iVar9 / 10 < 10) {
        iVar2 = 10;
      }
      FUN_140047aa0(param_1 + 0x40,iVar2 + iVar9,0,CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),1));
      iVar1 = *(int *)(param_1 + 0x50);
    }
    if (0 < iVar1) {
      FUN_14025c3c0(param_1 + 0x40,1);
    }
    **(undefined8 **)(param_1 + 0x48) = plVar8;
    *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + 1;
    (**(code **)(*plVar8 + 8))(plVar8,*(undefined4 *)(param_1 + 0x18),param_2);
    *(longlong **)(param_1 + 0x38) = plVar8;
    (**(code **)(*plVar8 + 0x18))(plVar8);
    (**(code **)(*plVar8 + 0x30))(plVar8);
    if (DAT_1409cf1c0 == 0) {
      lVar7 = FUN_140769c58(0x10c8);
      if (lVar7 == 0) {
        DAT_1409cf1c0 = 0;
      }
      else {
        DAT_1409cf1c0 = FUN_140262010(lVar7);
      }
      local_res8 = &PTR_vftable_1409cf1b8;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_1409cf1b8;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140262af0(DAT_1409cf1c0);
    *(int *)(param_1 + 0x1c) = param_2;
    if ((param_3 != (longlong *)0x0) && (param_2 == 0)) {
      *(undefined *)(plVar8 + 0x68) = *(undefined *)((longlong)param_3 + 0xcb);
      if (plVar8 + 0x69 != param_3) {
        plVar10 = param_3;
        if (0xf < (ulonglong)param_3[3]) {
          plVar10 = (longlong *)*param_3;
// ... [truncated at 150 / 176 lines]


// ================================================================
// LoadingThreadClass — find and decompile thread body
// ================================================================


// ================================================================
// FUN_14022e310 — thread name setter (sets 'LoadThread' etc)
// ================================================================

// RVA: 0x22e310  Name: FUN_14022e310  Size: 191 bytes

undefined ** FUN_14022e310(undefined **param_1,undefined *param_2)

{
  undefined8 *puVar1;
  int iVar2;
  undefined *puVar3;
  DWORD DVar4;
  int iVar5;
  undefined4 uStack_c;
  
  param_1[2] = param_2;
  *param_1 = (undefined *)ThreadClass::vftable;
  *(undefined1 *)(param_1 + 1) = 0;
  *(undefined1 *)((longlong)param_1 + 9) = 0;
  param_1[3] = (undefined *)0xffffffffffffffff;
  *(undefined4 *)(param_1 + 4) = 0;
  if (param_1 == &PTR_vftable_140a155b0) {
    DVar4 = GetCurrentThreadId();
    puVar3 = PTR_s_Main_Thread_140a155c0;
    iVar5 = DAT_140a155ac * 2 >> 1;
    DAT_140a155d0 = DVar4;
    if (iVar5 <= DAT_140a155a8) {
      iVar2 = iVar5 / 10;
      if (iVar5 / 10 < 10) {
        iVar2 = 10;
      }
      FUN_14022eb30(&PTR_vftable_140a15598,iVar2 + iVar5,0,1);
    }
    puVar1 = (undefined8 *)(DAT_140a155a0 + (longlong)DAT_140a155a8 * 0x10);
    *puVar1 = puVar3;
    puVar1[1] = CONCAT44(uStack_c,DVar4);
    DAT_140a155a8 = DAT_140a155a8 + 1;
  }
  return param_1;
}


// ================================================================
// Callers of FUN_14008df00 (LoadThread command handler)
// ================================================================

// Caller: RVA 0xc5250  FUN_1400c5250  1553b
