
// ================================================================
// FUN_1403c05d0 — tick of mode+0x1f0 subsystem (BATTLE-ACTIVE, MAIN CANDIDATE)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3c05d0  Name: FUN_1403c05d0  Size: 162 bytes
// ------------------------------------------------------------

void FUN_1403c05d0(longlong param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  int iVar3;
  
  if ((*(char *)(param_1 + 0x30) == '\0') && (DAT_140b15aef != '\0')) {
    iVar3 = 0;
    if (*(char *)(param_1 + 0x31) == '\0') {
      if (0 < *(int *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x18) + 0x68)) {
        do {
          uVar1 = FUN_1402ac9c0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x18),iVar3);
          FUN_1403c0680(param_1,uVar1,1);
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x18) + 0x68));
      }
      *(undefined1 *)(param_1 + 0x31) = 1;
    }
    FUN_1403c01e0(param_1);
    puVar2 = (undefined8 *)(param_1 + 0x38);
    *(undefined8 *)(param_1 + 0x48) = 0;
    if (7 < *(ulonglong *)(param_1 + 0x50)) {
      puVar2 = (undefined8 *)*puVar2;
    }
    *(undefined2 *)puVar2 = 0;
    *(undefined1 *)(param_1 + 0x30) = 1;
  }
  return;
}




// ================================================================
// FUN_1404d9cd0 — singleton service tick (every frame)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d9cd0  Name: FUN_1404d9cd0  Size: 100 bytes
// ------------------------------------------------------------

void FUN_1404d9cd0(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
  if (0 < iVar3) {
    do {
      lVar1 = FUN_140294bc0(&DAT_140a16fd0,iVar2);
      if (lVar1 != 0) {
        FUN_14027ff80(lVar1,param_2);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < iVar3);
  }
  return;
}




// ================================================================
// FUN_14035b2e0 — slot-6 helper before slot-41 dispatch
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35b2e0  Name: FUN_14035b2e0  Size: 8 bytes
// ------------------------------------------------------------

void FUN_14035b2e0(longlong param_1)

{
  *(undefined1 *)(param_1 + 0x9a) = 1;
  return;
}




// ================================================================
// SpaceModeClass slot 41 — FUN_1404d7ce0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d7ce0  Name: FUN_1404d7ce0  Size: 48 bytes
// ------------------------------------------------------------

void FUN_1404d7ce0(void)

{
  FUN_14021d090(&DAT_140a7d0a0,DAT_140b30194);
  FUN_14021d0b0(&DAT_140a7d0a0,DAT_140b301a0);
  return;
}




// ================================================================
// LandModeClass slot 41 — FUN_1403b86c0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b86c0  Name: FUN_1403b86c0  Size: 48 bytes
// ------------------------------------------------------------

void FUN_1403b86c0(void)

{
  FUN_14021d090(&DAT_140a7d0a0,DAT_140b30198);
  FUN_14021d0b0(&DAT_140a7d0a0,DAT_140b301a4);
  return;
}




// ================================================================
// FUN_14035e560 — base mode initializer (who sets mode+0x1f0?)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35e560  Name: FUN_14035e560  Size: 641 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14035e560(longlong *param_1,int param_2,int param_3)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong *plVar5;
  
  *(int *)((longlong)param_1 + 0xc) = param_2;
  *(undefined1 *)(param_1 + 0x13) = 0;
  lVar2 = FUN_140769c58(0x48);
  if (lVar2 == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = FUN_140261470(lVar2);
  }
  (**(code **)(*param_1 + 0xc0))(param_1,uVar3);
  lVar4 = FUN_140769c58(0xa8);
  lVar2 = 0;
  if (lVar4 != 0) {
    lVar2 = FUN_14025be50(lVar4);
  }
  param_1[9] = lVar2;
  lVar4 = FUN_140769c58(0x640);
  lVar2 = 0;
  if (lVar4 != 0) {
    lVar2 = FUN_14029a0c0(lVar4);
  }
  param_1[3] = lVar2;
  lVar4 = FUN_140769c58(0x640);
  lVar2 = 0;
  if (lVar4 != 0) {
    lVar2 = FUN_14029a0c0(lVar4);
  }
  param_1[4] = lVar2;
  FUN_1402af130(param_1[3],DAT_140a16fb0,param_1,1);
  FUN_1402af130(param_1[4],DAT_140a16fb0,param_1,0);
  FUN_1402a74f0(param_1[3],param_2 == 3);
  FUN_1402a74f0(param_1[4],param_2 == 3);
  if (param_3 != 0) {
    lVar2 = FUN_140769c58(0x148);
    if (lVar2 == 0) {
      lVar2 = 0;
    }
    else {
      lVar2 = FUN_1403bd080(lVar2,param_1);
    }
    lVar4 = param_1[0x3e];
    if (lVar4 != lVar2) {
      if (lVar4 != 0) {
        (**(code **)(*(longlong *)(lVar4 + 0x10) + 8))(lVar4 + 0x10);
      }
      param_1[0x3e] = lVar2;
      if (lVar2 != 0) {
        (*(code *)**(undefined8 **)(lVar2 + 0x10))(lVar2 + 0x10);
      }
    }
  }
  lVar4 = FUN_140769c58(0x80);
  lVar2 = 0;
  if (lVar4 != 0) {
    lVar2 = FUN_140341850(lVar4,param_1,DAT_140b0a320);
  }
  param_1[5] = lVar2;
  if (param_3 == 2) {
    lVar2 = FUN_140769c58();
    if (lVar2 == 0) {
      param_1[6] = 0;
      (**(code **)(_DAT_00000000 + 0x18))(0,(int)param_1[2]);
    }
    else {
      plVar5 = (longlong *)FUN_140532b70(lVar2);
      param_1[6] = (longlong)plVar5;
      (**(code **)(*plVar5 + 0x18))(plVar5,(int)param_1[2]);
    }
  }
  else if (param_3 == 1) {
    lVar2 = FUN_140769c58();
    if (lVar2 == 0) {
      plVar5 = (longlong *)0x0;
    }
    else {
      plVar5 = (longlong *)FUN_140534bd0(lVar2);
    }
    param_1[6] = (longlong)plVar5;
    (**(code **)(*plVar5 + 0x18))(plVar5,(int)param_1[2]);
    lVar4 = FUN_140769c58(0x30);
    lVar2 = 0;
    if (lVar4 != 0) {
      lVar2 = FUN_1404922d0(lVar4);
    }
    param_1[7] = lVar2;
    lVar4 = FUN_140769c58(0x58);
    lVar2 = 0;
    if (lVar4 != 0) {
      lVar2 = FUN_140527f30(lVar4);
    }
    param_1[8] = lVar2;
  }
  cVar1 = FUN_14028afb0(&DAT_140b153e0);
  if (cVar1 == '\x01') {
    lVar4 = FUN_140769c58(0x10);
    lVar2 = 0;
    if (lVar4 != 0) {
      lVar2 = FUN_140537710(lVar4);
    }
    param_1[0x53] = lVar2;
  }
  param_1[0x1c] = (longlong)param_1;
  *(undefined1 *)((longlong)param_1 + 0x9a) = 1;
  param_1[0x44] = 0;
  *(undefined1 *)(param_1 + 0x4f) = 0;
  *(undefined1 *)(param_1 + 0x5a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x2d4) = 0;
  _DAT_140b27b70 = 0;
  return 1;
}




// ================================================================
// FUN_14029c800 — GOM destructor (gom+0x38 iteration reference)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x29c800  Name: FUN_14029c800  Size: 1971 bytes
// ------------------------------------------------------------

void FUN_14029c800(longlong param_1)

{
  uint *puVar1;
  LPVOID pvVar2;
  longlong lVar3;
  HANDLE pvVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  undefined1 local_res8 [8];
  undefined1 local_res10 [8];
  undefined1 local_res18 [16];
  
  *(undefined1 *)(param_1 + 0x63d) = 1;
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x18);
  if (param_1 == lVar3) {
    lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x20);
  }
  uVar8 = 0;
  lVar5 = *(longlong *)(param_1 + 0x48);
  while (lVar5 != param_1 + 0x40) {
    uVar9 = *(longlong *)(lVar5 + 0x18) - 0x18;
    if (*(longlong *)(lVar5 + 0x18) == 0) {
      uVar9 = uVar8;
    }
    FUN_1403a8380(uVar9);
    if (lVar3 != 0) {
      FUN_1402a6a00(lVar3,uVar9,0);
    }
    FUN_1402a6a00(param_1,uVar9,0);
    lVar5 = *(longlong *)(param_1 + 0x48);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x408);
  while (lVar3 != 0) {
    FUN_14020ac80(param_1 + 0x408,local_res8);
    lVar3 = FUN_1402ac2f0(param_1 + 0x408);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x38);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x38,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x38);
  }
  FUN_14029c770(param_1 + 0x3d8);
  lVar3 = FUN_1402ac2f0(param_1 + 0xe8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0xe8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0xe8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x130);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x130,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x130);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x178);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x178,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x178);
  }
  pvVar2 = *(LPVOID *)(param_1 + 0x488);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x494) & 0x80000000) == 0) {
      FUN_14029b5c0(pvVar2,3);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) & 0x80000000;
    *(undefined8 *)(param_1 + 0x488) = 0;
    *(undefined4 *)(param_1 + 0x490) = 0;
  }
  pvVar2 = *(LPVOID *)(param_1 + 0x470);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x47c) & 0x80000000) == 0) {
      FUN_14029b5c0(pvVar2,3);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x47c) = *(uint *)(param_1 + 0x47c) & 0x80000000;
    *(undefined8 *)(param_1 + 0x470) = 0;
    *(undefined4 *)(param_1 + 0x478) = 0;
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x1c8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x1c8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x1c8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x210);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x210,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x210);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 600);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 600,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 600);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x2a0);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x2a0,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x2a0);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x2e8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x2e8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x2e8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x330);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x330,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x330);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x378);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x378,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x378);
  }
  FUN_140139170(param_1 + 0x3c0);
  pvVar2 = *(LPVOID *)(param_1 + 0x5f8);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar2);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x604) = *(uint *)(param_1 + 0x604) & 0x80000000;
    *(undefined8 *)(param_1 + 0x5f8) = 0;
    *(undefined4 *)(param_1 + 0x600) = 0;
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x560);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x560,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x560);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x5a8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x5a8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x5a8);
  }
  uVar9 = uVar8;
  if (0 < *(int *)(param_1 + 0x18)) {
    do {
      iVar7 = (int)uVar8;
      if (*(longlong *)(param_1 + 200) != 0) {
        lVar3 = *(longlong *)(param_1 + 200) + (longlong)iVar7 * 0x48;
        lVar5 = FUN_1402ac2f0(lVar3);
        while (lVar5 != 0) {
          lVar5 = FUN_14020ac80(lVar3,local_res8);
          if ((lVar5 != 0) && ((longlong *)(lVar5 + -0x18) != (longlong *)0x0)) {
            (**(code **)(*(longlong *)(lVar5 + -0x18) + 8))();
          }
          lVar5 = FUN_1402ac2f0(lVar3);
        }
      }
      lVar3 = (longlong)iVar7;
      if (*(longlong *)(param_1 + 0xd0) != 0) {
        FUN_14020e550(lVar3 * 0x38 + *(longlong *)(param_1 + 0xd0));
      }
      lVar5 = *(longlong *)(param_1 + 0x540);
      if ((lVar5 != 0) && (pvVar2 = *(LPVOID *)(uVar9 + 8 + lVar5), pvVar2 != (LPVOID)0x0)) {
        if ((*(uint *)(uVar9 + 0x14 + lVar5) & 0x80000000) == 0) {
          thunk_FUN_1407864b8(pvVar2);
        }
        else {
          pvVar4 = GetProcessHeap();
          HeapFree(pvVar4,0,pvVar2);
        }
        puVar1 = (uint *)(uVar9 + 0x14 + lVar5);
        *puVar1 = *puVar1 & 0x80000000;
        *(undefined8 *)(uVar9 + 8 + lVar5) = 0;
        *(undefined4 *)(uVar9 + 0x10 + lVar5) = 0;
      }
      if (*(longlong *)(param_1 + 0x1c0) != 0) {
        lVar5 = *(longlong *)(param_1 + 0x1c0) + lVar3 * 0x48;
        lVar6 = FUN_1402ac2f0(lVar5);
        while (lVar6 != 0) {
          lVar6 = FUN_14020ac80(lVar5,local_res10);
          if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
            (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
          }
          lVar6 = FUN_1402ac2f0(lVar5);
        }
      }
      if (*(longlong *)(param_1 + 0x548) != 0) {
        lVar3 = *(longlong *)(param_1 + 0x548) + lVar3 * 0x48;
        lVar5 = FUN_1402ac2f0(lVar3);
        while (lVar5 != 0) {
          lVar5 = FUN_14020ac80(lVar3,local_res18);
          if ((lVar5 != 0) && ((longlong *)(lVar5 + -0x18) != (longlong *)0x0)) {
            (**(code **)(*(longlong *)(lVar5 + -0x18) + 8))();
          }
          lVar5 = FUN_1402ac2f0(lVar3);
        }
      }
      uVar8 = (ulonglong)(iVar7 + 1U);
      uVar9 = uVar9 + 0x18;
    } while ((int)(iVar7 + 1U) < *(int *)(param_1 + 0x18));
  }
  if (*(longlong *)(param_1 + 0xd8) != 0) {
    FUN_14020e550();
  }
  if (*(longlong *)(param_1 + 0x558) != 0) {
    FUN_14020e550();
  }
  if (*(longlong *)(param_1 + 0xe0) != 0) {
    FUN_14020e550();
  }
  FUN_1402af4f0(param_1);
  *(undefined1 *)(param_1 + 0x63d) = 0;
  return;
}



