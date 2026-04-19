
// ================================================================
// FUN_14027ff80 — per-player per-frame tick (MAIN CANDIDATE)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x27ff80  Name: FUN_14027ff80  Size: 17 bytes
// ------------------------------------------------------------

void FUN_14027ff80(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  int iVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *plVar6;
  
  lVar1 = *(longlong *)(param_1 + 0x360);
  if (lVar1 == 0) {
    return;
  }
  iVar3 = (**(code **)(*param_2 + 0xe0))(param_2);
  if ((iVar3 != 0) &&
     (plVar6 = *(longlong **)(lVar1 + 0x38), plVar6 != *(longlong **)(lVar1 + 0x40))) {
    while( true ) {
      plVar4 = (longlong *)FUN_1405f55e0(*plVar6);
      if (plVar4 == param_2) break;
      plVar6 = plVar6 + 1;
      if (plVar6 == *(longlong **)(lVar1 + 0x40)) {
        return;
      }
    }
    if ((((plVar6 != *(longlong **)(lVar1 + 0x40)) && (*(char *)(*plVar6 + 0x68) == '\0')) &&
        (DAT_140b153f8 != 3)) &&
       (lVar5 = FUN_1402acb70(*(undefined8 *)(DAT_140b15418 + 0x18),
                              *(undefined4 *)(*(longlong *)(lVar1 + 0x28) + 0x4c)),
       *(int *)(lVar5 + 0x30) == 0)) {
      (**(code **)(*(longlong *)*plVar6 + 0x10))();
      puVar2 = (undefined8 *)*plVar6;
      if (puVar2 != (undefined8 *)0x0) {
        (**(code **)*puVar2)(puVar2,1);
      }
      FUN_14076bd80(plVar6,plVar6 + 1,*(longlong *)(lVar1 + 0x40) - (longlong)(plVar6 + 1));
      *(longlong *)(lVar1 + 0x40) = *(longlong *)(lVar1 + 0x40) + -8;
    }
  }
  return;
}




// ================================================================
// FUN_14035cb10 — slot-3 real function (both Space+Land modes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35cb10  Name: FUN_14035cb10  Size: 246 bytes
// ------------------------------------------------------------

void FUN_14035cb10(longlong *param_1)

{
  char cVar1;
  longlong lVar2;
  ulonglong uVar3;
  undefined8 uVar4;
  uint uVar5;
  ulonglong uVar6;
  
  uVar6 = 0;
  *(undefined1 *)(param_1 + 0x13) = 1;
  *(undefined4 *)(param_1 + 2) = 0;
  cVar1 = (**(code **)(*param_1 + 0xf0))();
  if (cVar1 == '\x01') {
    lVar2 = FUN_140769c58(0x20);
    uVar3 = uVar6;
    if (lVar2 != 0) {
      uVar3 = FUN_1404eab00(lVar2);
    }
    param_1[0x54] = uVar3;
    FUN_1404eb4e0(uVar3);
  }
  uVar4 = FUN_14028a5d0();
  FUN_1404d9df0(uVar4,param_1);
  FUN_14044b500(&DAT_140a28c10,param_1);
  *(undefined1 *)((longlong)param_1 + 0x9a) = 1;
  cVar1 = FUN_14028afb0(&DAT_140b153e0);
  if ((cVar1 == '\x01') && (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
    do {
      lVar2 = FUN_140294bc0(&DAT_140a16fd0,uVar6);
      if (lVar2 != 0) {
        FUN_140288980(lVar2,1,0);
      }
      uVar5 = (int)uVar6 + 1;
      uVar6 = (ulonglong)uVar5;
    } while ((int)uVar5 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  if (DAT_140b31418 != (code *)0x0) {
    (*DAT_140b31418)();
  }
  return;
}




// ================================================================
// FUN_1403bd080 — alloc mode+0x1f0 subsystem (size 0x148)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3bd080  Name: FUN_1403bd080  Size: 455 bytes
// ------------------------------------------------------------

undefined8 * FUN_1403bd080(undefined8 *param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  *(undefined2 *)((longlong)param_1 + 0x34) = 1;
  *(undefined4 *)(param_1 + 3) = 0;
  *param_1 = SetupPhaseManagerClass::vftable;
  param_1[4] = 0;
  param_1[5] = param_2;
  param_1[2] = SetupPhaseManagerClass::vftable;
  param_1[1] = 0;
  *(undefined4 *)(param_1 + 6) = 0;
  *(undefined1 *)((longlong)param_1 + 0x36) = 0;
  param_1[7] = 0;
  param_1[9] = 0;
  param_1[10] = 7;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  *(undefined1 *)(param_1 + 0xe) = 0;
  *(undefined4 *)((longlong)param_1 + 0x74) = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  lVar1 = FUN_140769c58(0x20);
  *(longlong *)lVar1 = lVar1;
  *(longlong *)(lVar1 + 8) = lVar1;
  param_1[0xf] = lVar1;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 7;
  param_1[0x15] = 8;
  *(undefined4 *)((longlong)param_1 + 0x74) = 0x3f800000;
  FUN_140048250(param_1 + 0x11,0x10,param_1[0xf]);
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  param_1[0x1d] = 0;
  *(undefined4 *)((longlong)param_1 + 0x114) = 0x5d5e0b6b;
  *(undefined4 *)(param_1 + 0x23) = 0x5d5e0b6b;
  *(undefined4 *)((longlong)param_1 + 0x11c) = 0x5d5e0b6b;
  *(undefined4 *)(param_1 + 0x24) = 0x5d5e0b6b;
  *(undefined4 *)((longlong)param_1 + 0x124) = 0x5d5e0b6b;
  param_1[0x25] = 0x5d5e0b6b;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  *(undefined4 *)(param_1 + 0x28) = 0;
  lVar1 = param_1[5];
  uVar2 = FUN_140058570();
  FUN_140220e90(uVar2,*(undefined8 *)(lVar1 + 0x18),param_1,0x1c);
  lVar1 = param_1[5];
  uVar2 = FUN_140058570();
  FUN_140220e90(uVar2,*(undefined8 *)(lVar1 + 0x18),param_1,0x1d);
  uVar2 = FUN_140058570();
  FUN_140220e90(uVar2,&DAT_140b153e0,param_1,0x24);
  FUN_1404ad3a0(param_1 + 4,DAT_140800804);
  return param_1;
}




// ================================================================
// GOM neighborhood A — RVA ~0x29b020 (between GOM ctor end and b3c0)
// ================================================================

// Found function at RVA 0x29aaa0
// ------------------------------------------------------------
// RVA: 0x29aaa0  Name: FUN_14029aaa0  Size: 2069 bytes
// ------------------------------------------------------------

void FUN_14029aaa0(undefined8 *param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  LPVOID pvVar3;
  undefined8 *puVar4;
  HANDLE pvVar5;
  longlong lVar6;
  undefined1 local_res8 [8];
  
  *param_1 = GameObjectManagerClass::vftable;
  for (puVar1 = (undefined8 *)param_1[9]; puVar1 != param_1 + 8; puVar1 = (undefined8 *)puVar1[1]) {
    lVar6 = puVar1[3] + -0x18;
    if (puVar1[3] == 0) {
      lVar6 = 0;
    }
    FUN_1403a8380(lVar6);
  }
  plVar2 = (longlong *)param_1[0x19];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0x1a];
  if (puVar1 != (undefined8 *)0x0) {
    if (puVar1[-1] == 0) {
      thunk_FUN_1407864b8(puVar1 + -1,8);
    }
    else {
      (**(code **)*puVar1)(puVar1,3);
    }
  }
  plVar2 = (longlong *)param_1[0x38];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0xa8];
  if (puVar1 != (undefined8 *)0x0) {
    if (puVar1[-1] == 0) {
      thunk_FUN_1407864b8(puVar1 + -1,8);
    }
    else {
      (**(code **)*puVar1)(puVar1,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0xab];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  puVar1 = (undefined8 *)param_1[0x1b];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  puVar1 = (undefined8 *)param_1[0x1c];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  plVar2 = (longlong *)param_1[0xaa];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  plVar2 = (longlong *)param_1[0xa9];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  param_1[0x1b] = 0;
  param_1[0x38] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0xa8] = 0;
  param_1[0xab] = 0;
  param_1[0x1c] = 0;
  param_1[0xa9] = 0;
  *(undefined4 *)(param_1 + 3) = 0;
  pvVar3 = (LPVOID)param_1[0xbf];
  if (pvVar3 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar3);
    }
    else {
      pvVar5 = GetProcessHeap();
      HeapFree(pvVar5,0,pvVar3);
    }
    *(uint *)((longlong)param_1 + 0x604) = *(uint *)((longlong)param_1 + 0x604) & 0x80000000;
    param_1[0xbf] = 0;
    *(undefined4 *)(param_1 + 0xc0) = 0;
  }
  pvVar3 = (LPVOID)param_1[0xbf];
  param_1[0xbe] = DynamicVectorClass<struct_GameObjectManagerClass::CreationParamsStruct>::vftable;
  if (pvVar3 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar3);
    }
    else {
      pvVar5 = GetProcessHeap();
      HeapFree(pvVar5,0,pvVar3);
    }
    *(uint *)((longlong)param_1 + 0x604) = *(uint *)((longlong)param_1 + 0x604) & 0x80000000;
    param_1[0xbf] = 0;
    *(undefined4 *)(param_1 + 0xc0) = 0;
  }
  param_1[0xb5] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0xb5);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0xb5,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0xb5);
  }
  FUN_14020a960(param_1 + 0xb5);
  param_1[0xac] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0xac);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0xac,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0xac);
  }
  FUN_14020a960(param_1 + 0xac);
  FUN_1404f2f90(param_1 + 0x93);
  FUN_14029a8f0(param_1 + 0x90);
  FUN_14029a8f0(param_1 + 0x8d);
  FUN_140046490(param_1 + 0x8a);
  puVar1 = param_1 + 0x81;
  *puVar1 = MultiLinkedListClass<class_GameObjectClass>::vftable;
  puVar4 = (undefined8 *)param_1[0x83];
  while (puVar4 != param_1 + 0x82) {
    FUN_14020ac80(puVar1,local_res8);
    puVar4 = (undefined8 *)param_1[0x83];
  }
  FUN_14020a960(puVar1);
  FUN_140046490(param_1 + 0x7e);
  param_1[0x7b] = DynamicVectorRefClass<class_GameObjectClass*___ptr64>::vftable;
  FUN_14029c770(param_1 + 0x7b);
  FUN_140046490(param_1 + 0x7b);
  FUN_140046490(param_1 + 0x78);
  param_1[0x6f] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x6f);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x6f,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x6f);
  }
  FUN_14020a960(param_1 + 0x6f);
  param_1[0x66] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x66);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x66,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x66);
  }
  FUN_14020a960(param_1 + 0x66);
  param_1[0x5d] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x5d);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x5d,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x5d);
  }
  FUN_14020a960(param_1 + 0x5d);
  param_1[0x54] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x54);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x54,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x54);
  }
  FUN_14020a960(param_1 + 0x54);
  param_1[0x4b] = ReferenceListClass<class_GameObjectClass>::vftable;
// ... [truncated at 200 lines, total 273]


// ================================================================
// GOM neighborhood B — RVA ~0x29c500 (just before GOM dtor)
// ================================================================

// Found function at RVA 0x29c0c0
// ------------------------------------------------------------
// RVA: 0x29c0c0  Name: FUN_14029c0c0  Size: 1607 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x00014029c6a5) */
/* WARNING: Removing unreachable block (ram,0x00014029c6d7) */

undefined4 FUN_14029c0c0(undefined8 param_1,longlong param_2,longlong param_3)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  char cVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  HANDLE pvVar9;
  undefined4 *puVar10;
  undefined8 uVar11;
  undefined4 *puVar12;
  ulonglong uVar13;
  undefined4 *puVar14;
  char *pcVar15;
  char *pcVar16;
  uint uVar17;
  int iVar18;
  undefined4 uVar19;
  int iVar20;
  uint uVar21;
  undefined4 *lpMem;
  uint uVar22;
  uint uVar23;
  uint local_res20;
  uint local_58;
  int local_54;
  longlong local_50;
  
  puVar12 = (undefined4 *)0x0;
  lpMem = (undefined4 *)0x0;
  uVar22 = 0;
  uVar23 = 0;
  local_58 = 0;
  local_res20 = 0;
  local_54 = 0;
  if (0 < *(int *)(param_2 + 0x10)) {
    local_50 = 0;
    puVar10 = puVar12;
    do {
      lVar7 = FUN_1402aca60(param_1,*(undefined4 *)(local_50 + *(longlong *)(param_2 + 8)));
      puVar12 = puVar10;
      if (lVar7 != 0) {
        if (((*(longlong *)(lVar7 + 0x1b0) != 0) &&
            (*(char *)(*(longlong *)(lVar7 + 0x1b0) + 0x32) != '\0')) ||
           ((param_3 != 0 && (lVar8 = FUN_1403973b0(lVar7), lVar8 == param_3)))) {
          uVar19 = *(undefined4 *)(lVar7 + 0x50);
          goto LAB_14029c67a;
        }
        cVar5 = FUN_14039a600(lVar7);
        if ((cVar5 == '\0') && (lVar7 = FUN_140394bf0(lVar7), lVar7 == 0)) {
          lVar7 = *(longlong *)(param_2 + 8);
          uVar17 = (int)(local_res20 * 2) >> 1;
          puVar10 = lpMem;
          uVar21 = uVar23;
          if ((int)uVar17 <= (int)uVar23) {
            iVar20 = (int)uVar17 / 10;
            if ((int)uVar17 / 10 < 10) {
              iVar20 = 10;
            }
            uVar21 = iVar20 + uVar17;
            if (uVar21 == 0) {
              uVar21 = uVar23;
              if (lpMem != (undefined4 *)0x0) {
                if ((int)local_res20 < 0) {
                  pvVar9 = GetProcessHeap();
                  HeapFree(pvVar9,0,lpMem);
                  local_res20 = 0;
                  puVar10 = (undefined4 *)0x0;
                  uVar21 = 0;
                }
                else {
                  thunk_FUN_1407864b8(lpMem);
                  local_res20 = 0;
                  puVar10 = (undefined4 *)0x0;
                  uVar21 = 0;
                }
              }
            }
            else {
              uVar13 = (ulonglong)(int)uVar21;
              if (lpMem == (undefined4 *)0x0) {
                if ((int)local_res20 < 0) {
                  pvVar9 = GetProcessHeap();
                  puVar10 = HeapAlloc(pvVar9,0,uVar13 * 4);
                }
                else {
                  auVar1._8_8_ = 0;
                  auVar1._0_8_ = uVar13;
                  uVar11 = SUB168(ZEXT816(4) * auVar1,0);
                  if (SUB168(ZEXT816(4) * auVar1,8) != 0) {
                    uVar11 = 0xffffffffffffffff;
                  }
                  puVar10 = (undefined4 *)thunk_FUN_140769c58(uVar11);
                }
                local_res20 = local_res20 ^ (local_res20 ^ uVar21) & 0x7fffffff;
                uVar21 = 0;
              }
              else {
                if ((int)local_res20 < 0) {
                  pvVar9 = GetProcessHeap();
                  puVar10 = HeapReAlloc(pvVar9,0,lpMem,uVar13 * 4);
                }
                else {
                  auVar2._8_8_ = 0;
                  auVar2._0_8_ = uVar13;
                  uVar11 = SUB168(ZEXT816(4) * auVar2,0);
                  if (SUB168(ZEXT816(4) * auVar2,8) != 0) {
                    uVar11 = 0xffffffffffffffff;
                  }
                  puVar10 = (undefined4 *)thunk_FUN_140769c58(uVar11);
                  if ((int)uVar21 < (int)uVar17) {
                    uVar17 = uVar21;
                  }
                  lVar8 = (longlong)(int)uVar17;
                  if (0 < (int)uVar17) {
                    puVar14 = puVar10;
                    do {
                      *puVar14 = *(undefined4 *)
                                  (((longlong)lpMem - (longlong)puVar10) + (longlong)puVar14);
                      puVar14 = puVar14 + 1;
                      lVar8 = lVar8 + -1;
                    } while (lVar8 != 0);
                  }
                  thunk_FUN_1407864b8(lpMem);
                }
                local_res20 = local_res20 ^ (local_res20 ^ uVar21) & 0x7fffffff;
                if ((int)uVar23 < (int)uVar21) {
                  uVar21 = uVar23;
                }
              }
            }
          }
          uVar23 = uVar21 + 1;
          puVar10[(int)uVar21] = *(undefined4 *)(lVar7 + local_50);
          lpMem = puVar10;
        }
        else {
          lVar7 = *(longlong *)(param_2 + 8);
          uVar17 = (int)(local_58 * 2) >> 1;
          uVar21 = uVar22;
          if ((int)uVar17 <= (int)uVar22) {
            iVar20 = (int)uVar17 / 10;
            if ((int)uVar17 / 10 < 10) {
              iVar20 = 10;
            }
            uVar21 = iVar20 + uVar17;
            if (uVar21 == 0) {
              uVar21 = uVar22;
              if (puVar10 != (undefined4 *)0x0) {
                if ((int)local_58 < 0) {
                  pvVar9 = GetProcessHeap();
                  HeapFree(pvVar9,0,puVar10);
                  local_58 = 0;
                  puVar12 = (undefined4 *)0x0;
                  uVar21 = 0;
                }
                else {
                  thunk_FUN_1407864b8(puVar10);
                  local_58 = 0;
                  puVar12 = (undefined4 *)0x0;
                  uVar21 = 0;
                }
              }
            }
            else {
              uVar13 = (ulonglong)(int)uVar21;
              if (puVar10 == (undefined4 *)0x0) {
                if ((int)local_58 < 0) {
                  pvVar9 = GetProcessHeap();
                  puVar12 = HeapAlloc(pvVar9,0,uVar13 * 4);
                }
                else {
                  auVar3._8_8_ = 0;
                  auVar3._0_8_ = uVar13;
                  uVar11 = SUB168(ZEXT816(4) * auVar3,0);
                  if (SUB168(ZEXT816(4) * auVar3,8) != 0) {
                    uVar11 = 0xffffffffffffffff;
                  }
                  puVar12 = (undefined4 *)thunk_FUN_140769c58(uVar11);
                }
                local_58 = local_58 ^ (local_58 ^ uVar21) & 0x7fffffff;
                uVar21 = 0;
              }
              else {
                if ((int)local_58 < 0) {
                  pvVar9 = GetProcessHeap();
                  puVar12 = HeapReAlloc(pvVar9,0,puVar10,uVar13 * 4);
                }
                else {
                  auVar4._8_8_ = 0;
                  auVar4._0_8_ = uVar13;
                  uVar11 = SUB168(ZEXT816(4) * auVar4,0);
// ... [truncated at 200 lines, total 325]


// ================================================================
// GOM neighborhood C — RVA ~0x29d000 (just after GOM dtor end)
// ================================================================

// Found function at RVA 0x29cfc0
// ------------------------------------------------------------
// RVA: 0x29cfc0  Name: FUN_14029cfc0  Size: 1570 bytes
// ------------------------------------------------------------

void FUN_14029cfc0(undefined8 param_1,longlong param_2,float param_3,longlong param_4)

{
  float fVar1;
  float fVar2;
  float fVar3;
  bool bVar4;
  longlong *plVar5;
  int iVar6;
  longlong lVar7;
  ulonglong uVar8;
  int iVar9;
  longlong *plVar10;
  int iVar11;
  longlong *plVar12;
  longlong *plVar13;
  uint uVar14;
  longlong lVar16;
  longlong lVar17;
  int iVar18;
  longlong lVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  int local_b8;
  longlong *local_b0;
  longlong local_a0;
  longlong *plVar15;
  
  plVar13 = (longlong *)0x0;
  fVar1 = DAT_140803524;
  if (0 < *(int *)(param_2 + 0x10)) {
    plVar12 = plVar13;
    plVar15 = plVar13;
    do {
      plVar5 = (longlong *)FUN_140769c58(0x28);
      plVar10 = plVar13;
      if (plVar5 != (longlong *)0x0) {
        *plVar5 = 0;
        plVar5[1] = 0;
        plVar5[2] = 0;
        plVar10 = plVar5;
      }
      plVar5 = (longlong *)plVar10[1];
      lVar7 = *(longlong *)((longlong)plVar12 + *(longlong *)(param_2 + 8));
      fVar1 = *(float *)(lVar7 + 0x78);
      fVar23 = *(float *)(lVar7 + 0x7c);
      if (((longlong)plVar5 - *plVar10 & 0xfffffffffffffff8U) == 0) {
        *(float *)(plVar10 + 4) = fVar1;
        *(float *)(plVar10 + 3) = fVar1;
        *(float *)((longlong)plVar10 + 0x1c) = fVar23;
LAB_14029d0c0:
        *(float *)((longlong)plVar10 + 0x24) = fVar23;
      }
      else {
        if (fVar1 < *(float *)(plVar10 + 3)) {
          *(float *)(plVar10 + 3) = fVar1;
        }
        if (*(float *)(plVar10 + 4) <= fVar1 && fVar1 != *(float *)(plVar10 + 4)) {
          *(float *)(plVar10 + 4) = fVar1;
        }
        if (fVar23 < *(float *)((longlong)plVar10 + 0x1c)) {
          *(float *)((longlong)plVar10 + 0x1c) = fVar23;
        }
        if (*(float *)((longlong)plVar10 + 0x24) <= fVar23 &&
            fVar23 != *(float *)((longlong)plVar10 + 0x24)) goto LAB_14029d0c0;
      }
      if (plVar5 == (longlong *)plVar10[2]) {
        FUN_140045580(plVar10);
      }
      else {
        *plVar5 = lVar7;
        plVar10[1] = plVar10[1] + 8;
      }
      iVar11 = *(int *)(param_4 + 0x10);
      iVar6 = *(int *)(param_4 + 0x14) * 2 >> 1;
      if (iVar6 <= iVar11) {
        iVar11 = iVar6 / 10;
        if (iVar6 / 10 < 10) {
          iVar11 = 10;
        }
        FUN_140047aa0(param_4,iVar11 + iVar6,0,1);
        iVar11 = *(int *)(param_4 + 0x10);
      }
      uVar14 = (int)plVar15 + 1;
      plVar15 = (longlong *)(ulonglong)uVar14;
      plVar12 = plVar12 + 1;
      *(longlong **)(*(longlong *)(param_4 + 8) + (longlong)iVar11 * 8) = plVar10;
      *(int *)(param_4 + 0x10) = *(int *)(param_4 + 0x10) + 1;
      fVar1 = DAT_140803524;
    } while ((int)uVar14 < *(int *)(param_2 + 0x10));
  }
LAB_14029d150:
  do {
    iVar11 = *(int *)(param_4 + 0x10);
    plVar13 = (longlong *)0x0;
    plVar12 = (longlong *)0x0;
    local_b0 = (longlong *)0x0;
    iVar6 = -1;
    iVar9 = -1;
    if (iVar11 < 1) {
      return;
    }
    local_b8 = 1;
    local_a0 = 0;
    fVar23 = fVar1;
    do {
      plVar15 = *(longlong **)(local_a0 + *(longlong *)(param_4 + 8));
      iVar18 = local_b8;
      lVar7 = local_a0;
      if (local_b8 < iVar11) {
        do {
          iVar6 = iVar9;
          plVar13 = *(longlong **)(lVar7 + 8 + *(longlong *)(param_4 + 8));
          fVar22 = *(float *)(plVar15 + 3);
          if (*(float *)(plVar13 + 3) <= *(float *)(plVar15 + 3)) {
            fVar22 = *(float *)(plVar13 + 3);
          }
          fVar20 = *(float *)(plVar15 + 4);
          if (*(float *)(plVar15 + 4) <= *(float *)(plVar13 + 4)) {
            fVar20 = *(float *)(plVar13 + 4);
          }
          fVar20 = fVar20 - fVar22;
          if (fVar20 <= param_3) {
            fVar22 = *(float *)((longlong)plVar15 + 0x1c);
            if (*(float *)((longlong)plVar13 + 0x1c) <= *(float *)((longlong)plVar15 + 0x1c)) {
              fVar22 = *(float *)((longlong)plVar13 + 0x1c);
            }
            fVar20 = *(float *)((longlong)plVar15 + 0x24);
            if (*(float *)((longlong)plVar15 + 0x24) <= *(float *)((longlong)plVar13 + 0x24)) {
              fVar20 = *(float *)((longlong)plVar13 + 0x24);
            }
            fVar20 = fVar20 - fVar22;
            if (fVar20 <= param_3) {
              lVar17 = plVar15[1];
              iVar11 = 0;
              lVar16 = *plVar15;
              fVar22 = 0.0;
              if (0 < (int)(lVar17 - lVar16 >> 3)) {
                lVar19 = 0;
                do {
                  iVar9 = 0;
                  lVar17 = *plVar13;
                  fVar2 = *(float *)(*(longlong *)(lVar19 + lVar16) + 0x78);
                  fVar3 = *(float *)(*(longlong *)(lVar19 + lVar16) + 0x7c);
                  if (0 < (int)(plVar13[1] - lVar17 >> 3)) {
                    lVar16 = 0;
                    do {
                      fVar20 = *(float *)(*(longlong *)(lVar16 + lVar17) + 0x78) - fVar2;
                      fVar21 = *(float *)(*(longlong *)(lVar16 + lVar17) + 0x7c) - fVar3;
                      fVar20 = (float)FUN_140776d48(lVar17,fVar21 * fVar21 + fVar20 * fVar20);
                      if (param_3 < fVar20) goto LAB_14029d2ff;
                      lVar17 = *plVar13;
                      fVar22 = fVar22 + fVar20;
                      iVar9 = iVar9 + 1;
                      lVar16 = lVar16 + 8;
                    } while (iVar9 < (int)(plVar13[1] - lVar17 >> 3));
                  }
                  lVar17 = plVar15[1];
                  iVar11 = iVar11 + 1;
                  lVar16 = *plVar15;
                  lVar19 = lVar19 + 8;
                } while (iVar11 < (int)(lVar17 - lVar16 >> 3));
              }
              fVar20 = fVar22 / (float)((int)(plVar13[1] - *plVar13 >> 3) *
                                       (int)(lVar17 - lVar16 >> 3));
            }
          }
LAB_14029d2ff:
          if ((fVar20 < fVar23) && (fVar20 < param_3)) {
            plVar12 = plVar15;
            fVar23 = fVar20;
            local_b0 = plVar13;
            iVar6 = iVar18;
          }
          iVar11 = *(int *)(param_4 + 0x10);
          iVar18 = iVar18 + 1;
          plVar13 = local_b0;
          lVar7 = lVar7 + 8;
          iVar9 = iVar6;
        } while (iVar18 < iVar11);
      }
      local_a0 = local_a0 + 8;
      bVar4 = local_b8 < iVar11;
      local_b8 = local_b8 + 1;
    } while (bVar4);
    if (plVar12 == (longlong *)0x0) {
      return;
    }
    lVar7 = *plVar13;
    iVar11 = 0;
    if (0 < (int)(plVar13[1] - lVar7 >> 3)) {
      lVar17 = 0;
      do {
        lVar7 = *(longlong *)(lVar17 + lVar7);
        plVar15 = (longlong *)plVar12[1];
        fVar23 = *(float *)(lVar7 + 0x78);
        fVar22 = *(float *)(lVar7 + 0x7c);
// ... [truncated at 200 lines, total 286]


// ================================================================
// GOM neighborhood D — RVA ~0x29e000
// ================================================================

// Found function at RVA 0x29dc30
// ------------------------------------------------------------
// RVA: 0x29dc30  Name: FUN_14029dc30  Size: 2813 bytes
// ------------------------------------------------------------

void FUN_14029dc30(longlong param_1,int param_2)

{
  undefined **ppuVar1;
  bool bVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  undefined **ppuVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined *puVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  undefined **ppuVar12;
  undefined *puVar13;
  longlong *plVar14;
  HANDLE pvVar15;
  undefined1 uVar16;
  int iVar17;
  undefined **ppuVar18;
  longlong lVar19;
  ulonglong uVar20;
  ulonglong uVar21;
  int iVar22;
  uint uVar23;
  int local_res18;
  int local_res20;
  longlong *local_f8;
  longlong *plStack_f0;
  longlong *local_e8;
  undefined **local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined **local_c8;
  longlong local_c0;
  undefined8 *local_b8;
  undefined8 *puStack_b0;
  undefined8 *local_a8;
  undefined4 local_a0;
  int local_9c;
  undefined **local_98;
  undefined *local_90;
  longlong local_88;
  undefined8 local_80;
  longlong *local_78;
  longlong *local_70;
  undefined **local_68;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  
  cVar3 = FUN_14013ed10();
  if (cVar3 == '\0') {
    local_c0 = 0;
    local_c8 = (undefined **)FUN_140769c58(0x28);
    *local_c8 = (undefined *)local_c8;
    local_c8[1] = (undefined *)local_c8;
    local_c8[2] = (undefined *)local_c8;
    *(undefined2 *)(local_c8 + 3) = 0x101;
    *(undefined4 *)(param_1 + 0x460) = 0;
    lVar7 = *(longlong *)(param_1 + 0x48);
    if (lVar7 != param_1 + 0x40) {
      do {
        lVar19 = *(longlong *)(lVar7 + 0x18) + -0x18;
        if (*(longlong *)(lVar7 + 0x18) == 0) {
          lVar19 = 0;
        }
        if (*(int *)(lVar19 + 0x58) != -1) {
          iVar5 = *(int *)(param_1 + 0x460);
          iVar17 = *(int *)(param_1 + 0x464) * 2 >> 1;
          if (iVar17 <= iVar5) {
            iVar5 = iVar17 / 10;
            if (iVar17 / 10 < 10) {
              iVar5 = 10;
            }
            FUN_140047aa0(param_1 + 0x450,iVar5 + iVar17,0,1);
            iVar5 = *(int *)(param_1 + 0x460);
          }
          *(longlong *)(*(longlong *)(param_1 + 0x458) + (longlong)iVar5 * 8) = lVar19;
          *(int *)(param_1 + 0x460) = *(int *)(param_1 + 0x460) + 1;
          if (0x7ffffffe < *(int *)(param_1 + 0x460)) break;
        }
        lVar7 = *(longlong *)(lVar7 + 8);
      } while (lVar7 != param_1 + 0x40);
    }
    uVar20 = 0;
    ppuVar12 = local_c8;
    uVar21 = uVar20;
    if (0 < *(int *)(param_1 + 0x460)) {
      do {
        puVar13 = *(undefined **)(*(longlong *)(*(longlong *)(param_1 + 0x458) + uVar21) + 0x298);
        cVar3 = *(char *)((longlong)ppuVar12[1] + 0x19);
        ppuVar6 = (undefined **)ppuVar12[1];
        ppuVar1 = ppuVar12;
        while (ppuVar18 = ppuVar6, cVar3 == '\0') {
          if (ppuVar18[4] < puVar13) {
            ppuVar6 = (undefined **)ppuVar18[2];
            ppuVar18 = ppuVar1;
          }
          else {
            ppuVar6 = (undefined **)*ppuVar18;
          }
          cVar3 = *(char *)((longlong)ppuVar6 + 0x19);
          ppuVar1 = ppuVar18;
        }
        if ((((*(char *)((longlong)ppuVar1 + 0x19) != '\0') || (puVar13 < ppuVar1[4])) ||
            (ppuVar1 == ppuVar12)) && (puVar13[0x3b] != '\0')) {
          uVar4 = (**(code **)(*DAT_140b15418 + 0xe0))();
          lVar7 = FUN_1403729f0(puVar13,uVar4,0,0);
          ppuVar12 = local_c8;
          if ((lVar7 != 0) && (lVar7 = FUN_1402648b0(lVar7), ppuVar12 = local_c8, lVar7 != 0)) {
            iVar17 = 0;
            bVar2 = false;
            iVar5 = FUN_14012d6d0(lVar7);
            if (0 < iVar5) {
              do {
                ppuVar12 = local_c8;
                if (bVar2) goto LAB_14029df4f;
                plVar8 = (longlong *)FUN_14012d6f0(lVar7);
                if (plVar8 != (longlong *)0x0) {
                  for (puVar9 = (undefined *)(**(code **)(*plVar8 + 0x20))(plVar8);
                      puVar9 != (undefined *)0x0; puVar9 = *(undefined **)(puVar9 + 8)) {
                    if (puVar9 == &DAT_140a12540) {
                      if (((int)plVar8[0x1f] == 1) &&
                         ((((lVar19 = *(longlong *)(plVar8[0x1e] + 8), lVar19 != 0 &&
                            (*(int *)(lVar19 + 0x24) != 3)) || (bVar2 = true, lVar19 != 0)) &&
                          (*(int *)(lVar19 + 0x24) != 2)))) goto LAB_14029de90;
                      break;
                    }
                  }
                }
                bVar2 = true;
LAB_14029de90:
                iVar17 = iVar17 + 1;
                iVar5 = FUN_14012d6d0(lVar7);
              } while (iVar17 < iVar5);
              ppuVar12 = local_c8;
              if (bVar2) goto LAB_14029df4f;
            }
            ppuVar1 = local_c8;
            uStack_d8 = (longlong *)((ulonglong)uStack_d8 & 0xffffffff00000000);
            ppuVar12 = (undefined **)local_c8[1];
            cVar3 = *(char *)((longlong)ppuVar12 + 0x19);
            local_e0 = ppuVar12;
            ppuVar6 = local_c8;
            while (ppuVar18 = ppuVar12, cVar3 == '\0') {
              uStack_d8._4_4_ = (uint)((ulonglong)uStack_d8 >> 0x20);
              if (ppuVar18[4] < puVar13) {
                ppuVar12 = (undefined **)ppuVar18[2];
                uStack_d8 = (longlong *)((ulonglong)uStack_d8._4_4_ << 0x20);
              }
              else {
                uStack_d8 = (longlong *)CONCAT44(uStack_d8._4_4_,1);
                ppuVar12 = (undefined **)*ppuVar18;
                ppuVar6 = ppuVar18;
              }
              cVar3 = *(char *)((longlong)ppuVar12 + 0x19);
              local_e0 = ppuVar18;
            }
            if ((*(char *)((longlong)ppuVar6 + 0x19) != '\0') ||
               (ppuVar12 = local_c8, puVar13 < ppuVar6[4])) {
              if (local_c0 == 0x666666666666666) {
                    /* WARNING: Subroutine does not return */
                FUN_1400501c0();
              }
              puVar10 = (undefined8 *)FUN_140769c58(0x28);
              puVar10[4] = puVar13;
              *puVar10 = ppuVar1;
              puVar10[1] = ppuVar1;
              puVar10[2] = ppuVar1;
              *(undefined2 *)(puVar10 + 3) = 0;
              local_68 = local_e0;
              uStack_60 = (undefined4)uStack_d8;
              uStack_5c = uStack_d8._4_4_;
              FUN_14004fe80(&local_c8);
              ppuVar12 = local_c8;
            }
          }
        }
LAB_14029df4f:
        uVar23 = (int)uVar20 + 1;
        uVar20 = (ulonglong)uVar23;
        uVar21 = uVar21 + 8;
      } while ((int)uVar23 < *(int *)(param_1 + 0x460));
    }
    uVar4 = DAT_14085c06c;
    local_98 = (undefined **)*ppuVar12;
    local_e0 = DynamicVectorClass<class_GameObjectClusterClass*___ptr64>::vftable;
    uStack_d8 = (longlong *)0x0;
    local_d0 = 0;
    if (local_98 != ppuVar12) {
      do {
        ppuVar12 = local_98;
        puVar13 = local_98[4];
        local_90 = puVar13;
        lVar7 = FUN_1402a87d0(param_1,puVar13,0);
        if (*(int *)(lVar7 + 0x10) < 300) {
          FUN_14029cfc0();
        }
// ... [truncated at 200 lines, total 495]

