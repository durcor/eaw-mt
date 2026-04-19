
// ================================================================
// SLOT 73 — FUN_14035eb40 (likely Add_Game_Object)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35eb40  Name: FUN_14035eb40  Size: 1522 bytes
// ------------------------------------------------------------

void FUN_14035eb40(longlong *param_1)

{
  int iVar1;
  LPVOID lpMem;
  longlong *plVar2;
  undefined1 auVar3 [16];
  char cVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong lVar7;
  ulonglong uVar8;
  int *piVar9;
  HANDLE hHeap;
  ulonglong uVar10;
  ulonglong uVar11;
  float fVar12;
  ulonglong uVar13;
  longlong lVar14;
  int iVar15;
  float fVar16;
  ulonglong uVar17;
  uint uVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float local_res8;
  float local_resc;
  float local_res10;
  float local_res14;
  float local_res18;
  float local_res1c;
  ulonglong in_stack_ffffffffffffff18;
  undefined8 local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  float local_a4;
  float local_a0;
  float local_9c;
  
  cVar4 = (**(code **)(*param_1 + 0x240))();
  lVar6 = DAT_140b2c1e8;
  if (cVar4 != '\0') {
    if (DAT_140a284c4 == '\0') {
      FUN_14013ecc0(0);
    }
    (**(code **)(*param_1 + 0x250))(param_1,1);
    (**(code **)(*param_1 + 0x1f8))(param_1,&local_b0);
    local_bc = 0;
    local_b8 = 0;
    local_res8 = 0.0;
    local_b4 = 0;
    if (((((local_b0 != DAT_1408007c0 * 0.0) || (local_ac != DAT_1408007c0 * 0.0)) ||
         (local_a8 != DAT_1408007c0 * 0.0)) ||
        ((local_a4 != DAT_1408007c0 * 0.0 || (local_a0 != DAT_1408007c0 * 0.0)))) ||
       (lVar6 = DAT_140b2c1e8, local_9c != DAT_1408007c0 * 0.0)) {
      fVar20 = local_b0 - local_a4;
      fVar21 = local_a4 + local_b0;
      fVar19 = local_ac + local_a0;
      fVar22 = local_ac - local_a0;
      iVar15 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
      auVar3._8_8_ = 0;
      auVar3._0_8_ = (longlong)iVar15;
      uVar5 = SUB168(ZEXT816(8) * auVar3,0);
      if (SUB168(ZEXT816(8) * auVar3,8) != 0) {
        uVar5 = 0xffffffffffffffff;
      }
      lVar6 = thunk_FUN_140769c58(uVar5);
      uVar13 = 0;
      param_1[0x33] = lVar6;
      *(int *)(param_1 + 0x32) = iVar15;
      fVar12 = 0.0;
      local_res8 = 0.0;
      uVar8 = uVar13;
      uVar17 = uVar13;
      fVar16 = fVar12;
      if (0 < iVar15) {
        do {
          lVar6 = FUN_140294bc0(&DAT_140a16fd0,uVar17);
          fVar12 = local_res8;
          if (lVar6 != 0) {
            iVar1 = *(int *)(lVar6 + 0x4c);
            lVar14 = (longlong)iVar1;
            uVar5 = FUN_140331cc0(&DAT_140a172d0);
            lVar7 = FUN_140294d30(&DAT_140a16fd0,uVar5);
            if (lVar6 == lVar7) {
              *(undefined8 *)(param_1[0x33] + lVar14 * 8) = 0;
              fVar12 = local_res8;
            }
            else {
              lVar6 = FUN_140769c58(0x58);
              uVar8 = uVar13;
              if (lVar6 != 0) {
                in_stack_ffffffffffffff18 = in_stack_ffffffffffffff18 & 0xffffffff00000000;
                local_res10 = fVar21;
                local_res14 = fVar22;
                local_res18 = fVar20;
                local_res1c = fVar19;
                uVar8 = FUN_1404c0890(lVar6,iVar1,&local_res18,&local_res10,
                                      in_stack_ffffffffffffff18,0);
              }
              *(ulonglong *)(param_1[0x33] + lVar14 * 8) = uVar8;
              lVar6 = *(longlong *)(param_1[0x33] + lVar14 * 8);
              piVar9 = (int *)param_1[0x38];
              fVar12 = *(float *)(lVar6 + 0x28);
              uVar8 = (ulonglong)*(uint *)(lVar6 + 0x24);
              local_res8 = fVar12;
              if ((piVar9 != (int *)0x0) &&
                 (uVar10 = uVar13, uVar11 = uVar13, 0 < (int)param_1[0x39])) {
                do {
                  if (*piVar9 == iVar1) {
                    if (-1 < (int)uVar11) {
                      FUN_1404c1560(lVar6);
                    }
                    break;
                  }
                  uVar11 = (ulonglong)((int)uVar11 + 1);
                  uVar10 = uVar10 + 1;
                  piVar9 = piVar9 + 1;
                } while ((longlong)uVar10 < (longlong)(int)param_1[0x39]);
              }
            }
          }
          fVar16 = (float)uVar8;
          uVar18 = (int)uVar17 + 1;
          uVar17 = (ulonglong)uVar18;
        } while ((int)uVar18 < iVar15);
      }
      lpMem = (LPVOID)param_1[0x38];
      if (lpMem != (LPVOID)0x0) {
        if ((*(uint *)((longlong)param_1 + 0x1cc) & 0x80000000) == 0) {
          thunk_FUN_1407864b8(lpMem);
        }
        else {
          hHeap = GetProcessHeap();
          HeapFree(hHeap,0,lpMem);
        }
        *(uint *)((longlong)param_1 + 0x1cc) = *(uint *)((longlong)param_1 + 0x1cc) & 0x80000000;
        param_1[0x38] = 0;
        *(undefined4 *)(param_1 + 0x39) = 0;
      }
      local_res8 = fVar21;
      local_resc = fVar22;
      local_res10 = fVar20;
      local_res14 = fVar19;
      FUN_1404dcfd0(&local_res10,&local_res8,
                    (longlong)(DAT_140b168f8 + -1 + (int)fVar16) / (longlong)DAT_140b168f8 &
                    0xffffffff,
                    (longlong)(DAT_140b168f8 + -1 + (int)fVar12) / (longlong)DAT_140b168f8 &
                    0xffffffff);
      lVar6 = FUN_140769c58(0x1390);
      uVar8 = uVar13;
      if (lVar6 != 0) {
        uVar8 = FUN_1407da510(lVar6);
      }
      param_1[0x34] = uVar8;
      local_res8 = fVar21;
      local_resc = fVar19;
      local_res10 = fVar20;
      local_res14 = fVar22;
      FUN_1407daec0(uVar8,&local_res10,&local_res8);
      FUN_140365810(param_1);
      if (param_1[3] != 0) {
        local_c8 = 0;
        local_c0 = 0;
        lVar6 = FUN_1402a9ff0(param_1[3],0x18,0xffffffff,0xffffffff,&local_c8,0,0x7fffffff,
                              0xffffffff);
        if ((lVar6 != 0) && (uVar8 = uVar13, uVar17 = uVar13, 0 < *(int *)(lVar6 + 0x10))) {
          do {
            plVar2 = *(longlong **)(uVar17 + *(longlong *)(lVar6 + 8));
            if ((plVar2 != (longlong *)0x0) &&
               (lVar7 = (**(code **)(*plVar2 + 0x10))(plVar2,0x18), lVar7 != 0)) {
              FUN_140537190(lVar7,plVar2);
            }
            uVar18 = (int)uVar8 + 1;
            uVar8 = (ulonglong)uVar18;
            uVar17 = uVar17 + 8;
          } while ((int)uVar18 < *(int *)(lVar6 + 0x10));
        }
        if (DAT_140b15490 == '\0') {
          local_c8 = 0;
          local_c0 = 0;
          lVar6 = FUN_1402a9ff0(param_1[3],0x19,0xffffffff,0xffffffff,&local_c8,0,0x7fffffff,
                                0xffffffff);
          if ((lVar6 != 0) && (uVar8 = uVar13, uVar17 = uVar13, 0 < *(int *)(lVar6 + 0x10))) {
            do {
              plVar2 = *(longlong **)(uVar17 + *(longlong *)(lVar6 + 8));
              if ((plVar2 != (longlong *)0x0) &&
                 (lVar7 = (**(code **)(*plVar2 + 0x10))(plVar2,0x19), lVar7 != 0)) {
                FUN_14053dd70(lVar7,plVar2);
              }
              uVar18 = (int)uVar8 + 1;
              uVar8 = (ulonglong)uVar18;
              uVar17 = uVar17 + 8;
            } while ((int)uVar18 < *(int *)(lVar6 + 0x10));
          }
        }
      }
      lVar6 = DAT_140b2c1e0;
      lVar7 = DAT_140b2c1e8;
      uVar8 = uVar13;
      if (0 < (int)(DAT_140b2c1e8 - DAT_140b2c1e0 >> 4)) {
        do {
          if (*(code **)(uVar8 + lVar6) != (code *)0x0) {
            (**(code **)(uVar8 + lVar6))(*(undefined8 *)(uVar8 + 8 + lVar6));
            lVar6 = DAT_140b2c1e0;
            lVar7 = DAT_140b2c1e8;
          }
          uVar18 = (int)uVar13 + 1;
          uVar13 = (ulonglong)uVar18;
          uVar8 = uVar8 + 0x10;
        } while ((int)uVar18 < (int)(lVar7 - lVar6 >> 4));
      }
    }
  }
  DAT_140b2c1e8 = lVar6;
  return;
}




// ================================================================
// SLOT 77 — FUN_140365150 (base-only, unknown)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x365150  Name: FUN_140365150  Size: 423 bytes
// ------------------------------------------------------------

void FUN_140365150(longlong param_1,longlong param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  longlong lVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined *puVar6;
  float fVar7;
  undefined1 local_38 [32];
  
  lVar2 = DAT_140b28f48;
  if (DAT_140b28f48 == 0) {
    return;
  }
  uVar5 = 0;
  if (*(longlong *)(param_1 + 0x220) != 0) {
    FUN_14025c490(DAT_140b28f48);
    puVar1 = *(undefined8 **)(param_1 + 0x220);
    if (puVar1 != (undefined8 *)0x0) {
      (**(code **)*puVar1)(puVar1,1);
    }
    *(undefined8 *)(param_1 + 0x220) = 0;
  }
  if (DAT_140b15468 == 0) {
    return;
  }
  lVar4 = FUN_140294a40(&DAT_140a16fd0);
  if (param_2 == 0) {
    return;
  }
  if (lVar4 == 0) {
    return;
  }
  if ((param_2 == lVar4) || (cVar3 = FUN_1402823e0(param_2,lVar4), cVar3 != '\0')) {
    fVar7 = (float)FUN_14025b900(param_3,DAT_140b15468,1);
    lVar4 = FUN_140769c58(200);
    if (lVar4 == 0) goto LAB_1403652be;
    puVar6 = &DAT_140b15b60;
  }
  else {
    cVar3 = FUN_1402824f0(lVar4,param_2);
    if (cVar3 == '\0') {
      return;
    }
    fVar7 = (float)FUN_14025b900(param_3,DAT_140b15468,1);
    lVar4 = FUN_140769c58(200);
    if (lVar4 == 0) goto LAB_1403652be;
    puVar6 = &DAT_140b15b70;
  }
  uVar5 = FUN_1401f9320(local_38,puVar6);
  uVar5 = FUN_14025b070(lVar4,param_3,DAT_1408007c0 - fVar7 * DAT_1408007c0,DAT_140804578,uVar5,
                        DAT_140b15468,0,1);
LAB_1403652be:
  *(undefined8 *)(param_1 + 0x220) = uVar5;
  FUN_14025c080(lVar2,uVar5);
  return;
}




// ================================================================
// BASE SLOT 1 THINK — FUN_14035e560
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
// BASE SLOT 2 — FUN_140365300 (called from galactic slot 2)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x365300  Name: FUN_140365300  Size: 564 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140365300(longlong *param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_14028a5d0();
  FUN_1404d9e60(uVar3,param_1);
  (**(code **)(*param_1 + 0x2a8))(param_1,0);
  _DAT_140b27b70 = 0;
  if (param_1[3] != 0) {
    FUN_14029c800();
    puVar1 = (undefined8 *)param_1[3];
    if (puVar1 != (undefined8 *)0x0) {
      (**(code **)*puVar1)(puVar1,1);
    }
    param_1[3] = 0;
  }
  if (param_1[4] != 0) {
    FUN_14029c800();
    puVar1 = (undefined8 *)param_1[4];
    if (puVar1 != (undefined8 *)0x0) {
      (**(code **)*puVar1)(puVar1,1);
    }
    param_1[4] = 0;
  }
  lVar2 = param_1[5];
  if (lVar2 != 0) {
    FUN_140341af0(lVar2);
    thunk_FUN_1407864b8(lVar2,0x80);
    param_1[5] = 0;
  }
  if ((longlong *)param_1[6] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[6] + 0x20))();
    lVar2 = param_1[6];
    if (lVar2 != 0) {
      (**(code **)(*(longlong *)(lVar2 + 8) + 0x10))(lVar2 + 8,1);
    }
    param_1[6] = 0;
  }
  if (param_1[7] != 0) {
    FUN_140496140();
    lVar2 = param_1[7];
    if (lVar2 != 0) {
      FUN_140492540(lVar2);
      thunk_FUN_1407864b8(lVar2,0x30);
    }
    param_1[7] = 0;
  }
  if (param_1[8] != 0) {
    FUN_14052af00();
    lVar2 = param_1[8];
    if (lVar2 != 0) {
      FUN_140528090(lVar2);
      thunk_FUN_1407864b8(lVar2,0x58);
    }
    param_1[8] = 0;
  }
  lVar2 = param_1[0x53];
  if (lVar2 != 0) {
    FUN_140537740(lVar2);
    thunk_FUN_1407864b8(lVar2,0x10);
    param_1[0x53] = 0;
  }
  if (param_1[0x54] != 0) {
    FUN_1404ec5f0();
    lVar2 = param_1[0x54];
    if (lVar2 != 0) {
      FUN_1404eab50(lVar2);
      thunk_FUN_1407864b8(lVar2,0x20);
    }
    param_1[0x54] = 0;
  }
  lVar2 = param_1[0x3e];
  if (lVar2 != 0) {
    (**(code **)(*(longlong *)(lVar2 + 0x10) + 8))(lVar2 + 0x10);
    param_1[0x3e] = 0;
  }
  if ((longlong *)param_1[0x12] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x12] + 8))();
    param_1[0x12] = 0;
  }
  puVar1 = (undefined8 *)param_1[9];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
    param_1[9] = 0;
  }
  (**(code **)(*param_1 + 0x250))(param_1,0);
  puVar1 = (undefined8 *)param_1[0x44];
  *(undefined1 *)((longlong)param_1 + 0x9a) = 1;
  *(undefined1 *)(param_1 + 0x4f) = 0;
  *(undefined1 *)(param_1 + 0x5a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x2d4) = 0;
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
    param_1[0x44] = 0;
  }
  return;
}




// ================================================================
// FUN_1404b12f0 (iterated in galactic slot 2 cleanup loop)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b12f0  Name: FUN_1404b12f0  Size: 400 bytes
// ------------------------------------------------------------

void FUN_1404b12f0(longlong param_1)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  longlong lVar5;
  longlong lVar6;
  ulonglong uVar7;
  
  lVar5 = *(longlong *)(param_1 + 0x18);
  uVar7 = *(longlong *)(param_1 + 0x20) - lVar5 & 0xfffffffffffffff8;
  lVar6 = lVar5;
  if (0xfff < uVar7) {
    lVar6 = *(longlong *)(lVar5 + -8);
    uVar7 = uVar7 + 0x27;
    lVar5 = lVar5 - lVar6;
    if (0x1f < lVar5 - 8U) {
LAB_1404b147b:
                    /* WARNING: Subroutine does not return */
      FUN_1407711cc(lVar5,uVar7);
    }
  }
  thunk_FUN_1407864b8(lVar6,uVar7);
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  puVar1 = *(undefined8 **)(param_1 + 8);
  *(undefined8 *)puVar1[1] = 0;
  puVar1 = (undefined8 *)*puVar1;
  do {
    if (puVar1 == (undefined8 *)0x0) {
      thunk_FUN_1407864b8(*(undefined8 *)(param_1 + 8),0x70);
      return;
    }
    lVar5 = puVar1[9];
    puVar2 = (undefined8 *)*puVar1;
    uVar7 = puVar1[10] - lVar5 & 0xfffffffffffffff8;
    lVar6 = lVar5;
    if (0xfff < uVar7) {
      lVar6 = *(longlong *)(lVar5 + -8);
      uVar7 = uVar7 + 0x27;
      lVar5 = lVar5 - lVar6;
      if (0x1f < lVar5 - 8U) goto LAB_1404b147b;
    }
    thunk_FUN_1407864b8(lVar6,uVar7);
    puVar1[9] = 0;
    puVar1[10] = 0;
    puVar1[0xb] = 0;
    puVar3 = (undefined8 *)puVar1[7];
    *(undefined8 *)puVar3[1] = 0;
    puVar3 = (undefined8 *)*puVar3;
    while (puVar3 != (undefined8 *)0x0) {
      puVar4 = (undefined8 *)*puVar3;
      thunk_FUN_1407864b8(puVar3,0x20);
      puVar3 = puVar4;
    }
    thunk_FUN_1407864b8(puVar1[7],0x20);
    uVar7 = puVar1[5];
    if (0xf < uVar7) {
      lVar5 = puVar1[2];
      lVar6 = lVar5;
      if (0xfff < uVar7 + 1) {
        lVar6 = *(longlong *)(lVar5 + -8);
        uVar7 = uVar7 + 0x28;
        lVar5 = lVar5 - lVar6;
        if (0x1f < lVar5 - 8U) goto LAB_1404b147b;
      }
      thunk_FUN_1407864b8(lVar6);
    }
    puVar1[4] = 0;
    puVar1[5] = 0xf;
    *(undefined1 *)(puVar1 + 2) = 0;
    thunk_FUN_1407864b8(puVar1,0x70);
    puVar1 = puVar2;
  } while( true );
}




// ================================================================
// FUN_140305c40 — UNCONDITIONAL SCENE SUB-CALL (primary sim-tick candidate)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x305c40  Name: FUN_140305c40  Size: 875 bytes
// ------------------------------------------------------------

void FUN_140305c40(longlong param_1)

{
  longlong *plVar1;
  byte bVar2;
  byte bVar3;
  ulonglong uVar4;
  char cVar5;
  int iVar6;
  longlong *plVar7;
  undefined8 uVar8;
  longlong lVar9;
  uint uVar10;
  byte *pbVar11;
  byte *pbVar12;
  uint uVar13;
  undefined1 local_18 [16];
  
  if (DAT_140b15418 == (longlong *)0x0) {
LAB_140305ddb:
    cVar5 = *(char *)(param_1 + 0x26b1);
    if ((cVar5 == '\0') &&
       (((*(char *)(param_1 + 0x26b2) != '\0' || (*(char *)(param_1 + 0x26b3) != '\0')) &&
        (DAT_140b30690 == 0)))) {
      FUN_1402fc7a0(param_1,"Light_Loop",1);
      return;
    }
    if ((*(char *)(param_1 + 0x26b2) == '\0') && (*(char *)(param_1 + 0x26b3) == '\0')) {
      if (cVar5 == '\0') {
        return;
      }
      plVar1 = (longlong *)(param_1 + 0x26b8);
      uVar4 = *(ulonglong *)(param_1 + 0x26d0);
      plVar7 = plVar1;
      if (0xf < uVar4) {
        plVar7 = (longlong *)*plVar1;
      }
      if (((*(longlong *)(param_1 + 0x26c8) == 10) && (*plVar7 == 0x6f4c5f746867694c)) &&
         ((short)plVar7[1] == 0x706f)) {
        pbVar12 = (byte *)&DAT_140a28ca0;
        if (0xf < DAT_140a28cb8) {
          pbVar12 = DAT_140a28ca0;
        }
        iVar6 = strcmp((char *)pbVar12,"Light_Loop");
        if (iVar6 != 0) {
          plVar7 = plVar1;
          if (0xf < uVar4) {
            plVar7 = (longlong *)*plVar1;
          }
          pbVar12 = (byte *)&DAT_140a28ca0;
          if (0xf < DAT_140a28cb8) {
            pbVar12 = DAT_140a28ca0;
          }
          lVar9 = (longlong)plVar7 - (longlong)pbVar12;
          do {
            bVar2 = *pbVar12;
            bVar3 = pbVar12[lVar9];
            if (bVar2 != bVar3) break;
            pbVar12 = pbVar12 + 1;
          } while (bVar3 != 0);
          if (bVar2 == bVar3) {
            return;
          }
          *(undefined1 *)(param_1 + 0x26b1) = 0;
          FUN_140201910(plVar1,0,0);
          return;
        }
        goto LAB_140305cc2;
      }
    }
    else if (cVar5 == '\0') {
      return;
    }
    pbVar12 = (byte *)&DAT_140a28ca0;
    if (0xf < DAT_140a28cb8) {
      pbVar12 = DAT_140a28ca0;
    }
    iVar6 = strcmp((char *)pbVar12,"Light_Loop");
    if (iVar6 == 0) {
      bVar2 = *(byte *)(param_1 + 0x2a30);
      uVar13 = (uint)(*(float *)(param_1 + 0x5c0) * DAT_1408054e8);
      uVar8 = FUN_1401ce750(*(undefined8 *)(param_1 + 0xdd0));
      FUN_1401f9320(local_18,uVar8);
      uVar10 = uVar13 & 0xff;
      if ((byte)uVar13 <= bVar2) {
        uVar10 = (uint)bVar2;
      }
      FUN_1401f9590(local_18,uVar10);
      (**(code **)(**(longlong **)(param_1 + 0xdd0) + 0x28))
                (*(longlong **)(param_1 + 0xdd0),local_18);
      FUN_1401f9440(local_18);
    }
  }
  else {
    cVar5 = (**(code **)(*DAT_140b15418 + 0xf8))();
    uVar4 = DAT_140a28cb8;
    pbVar12 = DAT_140a28ca0;
    if ((cVar5 == '\0') || (*(char *)(param_1 + 0x26a4) != '\0')) {
      cVar5 = (**(code **)(*DAT_140b15418 + 0xf0))();
      uVar4 = DAT_140a28cb8;
      pbVar12 = DAT_140a28ca0;
      if ((cVar5 == '\0') || (*(int *)(param_1 + 0x26a0) < 1)) goto LAB_140305ddb;
      if (*(char *)(param_1 + 0x26b1) == '\0') {
        return;
      }
      pbVar11 = (byte *)&DAT_140a28ca0;
      if (0xf < DAT_140a28cb8) {
        pbVar11 = DAT_140a28ca0;
      }
      iVar6 = strcmp((char *)pbVar11,"Light_Loop");
      if (iVar6 != 0) {
        lVar9 = param_1 + 0x26b8;
        if (0xf < *(ulonglong *)(param_1 + 0x26d0)) {
          lVar9 = *(longlong *)(param_1 + 0x26b8);
        }
        pbVar11 = (byte *)&DAT_140a28ca0;
        if (0xf < uVar4) {
          pbVar11 = pbVar12;
        }
        lVar9 = lVar9 - (longlong)pbVar11;
        do {
          pbVar12 = pbVar11 + lVar9;
          iVar6 = (uint)*pbVar11 - (uint)*pbVar12;
          if (iVar6 != 0) break;
          pbVar11 = pbVar11 + 1;
        } while (*pbVar12 != 0);
        goto LAB_140305d04;
      }
    }
    else {
      if (*(char *)(param_1 + 0x26b1) == '\0') {
        return;
      }
      pbVar11 = (byte *)&DAT_140a28ca0;
      if (0xf < DAT_140a28cb8) {
        pbVar11 = DAT_140a28ca0;
      }
      iVar6 = strcmp((char *)pbVar11,"Light_Loop");
      if (iVar6 != 0) {
        lVar9 = param_1 + 0x26b8;
        if (0xf < *(ulonglong *)(param_1 + 0x26d0)) {
          lVar9 = *(longlong *)(param_1 + 0x26b8);
        }
        pbVar11 = (byte *)&DAT_140a28ca0;
        if (0xf < uVar4) {
          pbVar11 = pbVar12;
        }
        lVar9 = lVar9 - (longlong)pbVar11;
        do {
          pbVar12 = pbVar11 + lVar9;
          iVar6 = (uint)*pbVar11 - (uint)*pbVar12;
          if (iVar6 != 0) break;
          pbVar11 = pbVar11 + 1;
        } while (*pbVar12 != 0);
LAB_140305d04:
        if (iVar6 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x26b1) = 0;
        FUN_140201910(param_1 + 0x26b8,0,0);
        return;
      }
    }
LAB_140305cc2:
    FUN_14030ac40(param_1);
  }
  return;
}




// ================================================================
// FUN_1402fd330 — FLAG-GATED SCENE SUB-CALL (2461 bytes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2fd330  Name: FUN_1402fd330  Size: 2461 bytes
// ------------------------------------------------------------

void FUN_1402fd330(longlong param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  float fVar5;
  char cVar6;
  int iVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  ulonglong uVar11;
  char ****ppppcVar12;
  int iVar13;
  char *pcVar14;
  char *****pppppcVar15;
  undefined8 *puVar16;
  ulonglong *puVar17;
  undefined8 uVar18;
  undefined8 *puVar19;
  ulonglong uVar20;
  ulonglong *puVar21;
  ulonglong *puVar22;
  ulonglong uVar23;
  char *****pppppcVar24;
  undefined8 *puVar25;
  longlong lVar26;
  uint uVar27;
  char *pcVar28;
  char *****pppppcVar29;
  longlong lVar30;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  char ****local_78 [2];
  undefined8 local_68;
  ulonglong local_60;
  ulonglong local_58 [2];
  undefined8 local_48;
  ulonglong local_40;
  
  *(undefined1 *)(param_1 + 0x2118) = *(undefined1 *)(param_1 + 0x49);
  *(undefined1 *)(param_1 + 0x48) = 0;
  if ((undefined8 **)(param_1 + 0x2708) != &DAT_140b16ae0) {
    puVar16 = &DAT_140b16ae0;
    if (0xf < DAT_140b16af8) {
      puVar16 = DAT_140b16ae0;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2708),puVar16,DAT_140b16af0);
  }
  if ((undefined8 **)(param_1 + 0x2728) != &DAT_140b16b20) {
    puVar16 = &DAT_140b16b20;
    if (0xf < DAT_140b16b38) {
      puVar16 = DAT_140b16b20;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2728),puVar16,DAT_140b16b30);
  }
  if ((undefined8 **)(param_1 + 0x2748) != &DAT_140b16b60) {
    puVar16 = &DAT_140b16b60;
    if (0xf < DAT_140b16b78) {
      puVar16 = DAT_140b16b60;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2748),puVar16,DAT_140b16b70);
  }
  if (*(char *)(param_1 + 0x2798) == '\0') {
    FUN_1402fa890(param_1);
  }
  if (*(longlong *)(param_1 + 0x27a0) == 0) {
    FUN_1402fa810(param_1);
  }
  if (*(char *)(param_1 + 0x2698) == '\0') {
    FUN_1402fab80(param_1,"data/art/models/i_tutorial_arrow.alo");
  }
  puVar22 = (ulonglong *)(param_1 + 0x2298);
  lVar26 = 10;
  pcVar28 = (char *)(param_1 + 0xb0);
  lVar30 = param_1 + 0x22b0;
  puVar21 = (ulonglong *)(param_1 + 0xa0);
  do {
    if ((char)puVar21[-5] != '\0') {
      iVar7 = (int)puVar21[1];
      if (iVar7 == 0) {
        *(undefined1 *)(puVar22 + -1) = 1;
        *puVar22 = puVar21[-4];
        *(undefined4 *)((longlong)puVar22 + -4) = *(undefined4 *)((longlong)puVar21 + -0x24);
        *(undefined1 *)((longlong)puVar22 + -7) = *(undefined1 *)((longlong)puVar21 + -0x27);
        *(undefined1 *)((longlong)puVar22 + -6) = *(undefined1 *)((longlong)puVar21 + -0x26);
        if (puVar21[-1] != 0) {
          puVar17 = puVar21 + -3;
          if (0xf < *puVar21) {
            puVar17 = (ulonglong *)*puVar17;
          }
          lVar8 = FUN_140407e00(&DAT_140b2f020,puVar17);
          if ((lVar8 != 0) &&
             (lVar8 = FUN_1402a87d0(*(undefined8 *)(DAT_140b15418 + 0x18),lVar8,0),
             0 < *(int *)(lVar8 + 0x10))) {
            puVar22[1] = **(ulonglong **)(lVar8 + 8);
          }
        }
        pcVar14 = pcVar28;
        do {
          cVar6 = *pcVar14;
          pcVar14[lVar30 - (longlong)pcVar28] = cVar6;
          pcVar14 = pcVar14 + 1;
        } while (cVar6 != '\0');
      }
      else {
        if (*(int *)(param_1 + 0xde8) == 0) {
          FUN_1403d3790(param_1);
          iVar7 = (int)puVar21[1];
        }
        puVar17 = puVar21 + -3;
        if (0xf < *puVar21) {
          puVar17 = (ulonglong *)*puVar17;
        }
        FUN_1402f55a0(param_1,puVar17,iVar7,*(undefined4 *)((longlong)puVar21 + 0xc),puVar21 + 2,1,
                      *(undefined1 *)((longlong)puVar21 + -0x26));
      }
    }
    lVar30 = lVar30 + 0x68;
    pcVar28 = pcVar28 + 0x78;
    puVar21 = puVar21 + 0xf;
    puVar22 = puVar22 + 0xd;
    lVar26 = lVar26 + -1;
  } while (lVar26 != 0);
  if (*(longlong *)(param_1 + 0x60) != 0) {
    plVar9 = (longlong *)(param_1 + 0x50);
    if (0xf < *(ulonglong *)(param_1 + 0x68)) {
      plVar9 = (longlong *)*plVar9;
    }
    FUN_1402fc7a0(param_1,plVar9,*(undefined1 *)(param_1 + 0x70));
  }
  plVar9 = (longlong *)FUN_140347b50(0x3f);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 1) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x40);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 2) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x41);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 4) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x42);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 8) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  fVar5 = DAT_1408054e8;
  uVar23 = 0;
  if (0 < *(int *)(param_1 + 0x538)) {
    uVar20 = uVar23;
    do {
      puVar16 = (undefined8 *)0x0;
      puVar10 = (undefined8 *)FUN_140769c58(0x70);
      if (puVar10 != (undefined8 *)0x0) {
        puVar10[2] = 0;
        puVar10[4] = 0;
        puVar10[5] = 0xf;
        puVar10[6] = 0;
        puVar10[8] = 0;
        puVar10[9] = 7;
        FUN_1401f92f0(puVar10 + 0xc);
        puVar16 = puVar10;
      }
      lVar30 = *(longlong *)(param_1 + 0x530);
      uVar18 = FUN_140347b50(0x20d);
      *puVar16 = *(undefined8 *)(uVar23 + lVar30);
      puVar10 = (undefined8 *)(lVar30 + 0x10 + uVar23);
      *(undefined4 *)(puVar16 + 1) = *(undefined4 *)(uVar23 + 8 + lVar30);
      if (puVar16 + 2 != puVar10) {
        puVar25 = puVar10;
        if (0xf < (ulonglong)puVar10[3]) {
          puVar25 = (undefined8 *)*puVar10;
        }
        FUN_140022730(puVar16 + 2,puVar25,puVar10[2]);
      }
      puVar25 = (undefined8 *)(lVar30 + 0x30 + uVar23);
      puVar10 = puVar16 + 6;
      if (puVar10 != puVar25) {
        puVar19 = puVar25;
        if (7 < (ulonglong)puVar25[3]) {
          puVar19 = (undefined8 *)*puVar25;
        }
        FUN_1400228a0(puVar10,puVar19,puVar25[2]);
      }
      *(undefined4 *)(puVar16 + 10) = *(undefined4 *)(uVar23 + 0x50 + lVar30);
      *(undefined4 *)((longlong)puVar16 + 0x54) = *(undefined4 *)(uVar23 + 0x54 + lVar30);
      *(undefined4 *)(puVar16 + 0xb) = *(undefined4 *)(uVar23 + 0x58 + lVar30);
      *(undefined1 *)((longlong)puVar16 + 0x5c) = *(undefined1 *)(uVar23 + 0x5c + lVar30);
      *(undefined1 *)((longlong)puVar16 + 0x5d) = *(undefined1 *)(uVar23 + 0x5d + lVar30);
      puVar1 = (undefined4 *)(uVar23 + 0x60 + lVar30);
      uVar2 = puVar1[1];
      uVar3 = puVar1[2];
      uVar4 = puVar1[3];
      *(undefined4 *)(puVar16 + 0xc) = *puVar1;
      *(undefined4 *)((longlong)puVar16 + 100) = uVar2;
      *(undefined4 *)(puVar16 + 0xd) = uVar3;
      *(undefined4 *)((longlong)puVar16 + 0x6c) = uVar4;
      plVar9 = (longlong *)FUN_14034b350(uVar18,0,0,0);
      *puVar16 = plVar9;
      (**(code **)(*plVar9 + 0x128))
                (plVar9,(int)(*(float *)(puVar16 + 0xc) * fVar5),
                 (int)(*(float *)((longlong)puVar16 + 100) * fVar5) & 0xff,
                 (int)(*(float *)(puVar16 + 0xd) * fVar5) & 0xff,
                 (char)(int)(*(float *)((longlong)puVar16 + 0x6c) * fVar5),1);
      if ((*(char *)((longlong)puVar16 + 0x5d) == '\0') ||
         (*(float *)(puVar16 + 10) == (float)(ulonglong)puVar16[8])) {
        (**(code **)(*(longlong *)*puVar16 + 0x148))((longlong *)*puVar16,puVar10,1);
      }
      iVar7 = *(int *)(param_1 + 0x1178);
      iVar13 = *(int *)(param_1 + 0x117c) * 2 >> 1;
      if (iVar13 <= iVar7) {
        iVar7 = iVar13 / 10;
        if (iVar13 / 10 < 10) {
          iVar7 = 10;
        }
        FUN_140047aa0(param_1 + 0x1168,iVar7 + iVar13,0);
        iVar7 = *(int *)(param_1 + 0x1178);
      }
      uVar27 = (int)uVar20 + 1;
      uVar20 = (ulonglong)uVar27;
      uVar23 = uVar23 + 0x70;
      *(undefined8 **)(*(longlong *)(param_1 + 0x1170) + (longlong)iVar7 * 8) = puVar16;
      *(int *)(param_1 + 0x1178) = *(int *)(param_1 + 0x1178) + 1;
    } while ((int)uVar27 < *(int *)(param_1 + 0x538));
  }
  pppppcVar24 = (char *****)0x0;
  *(undefined4 *)(param_1 + 0x2238) = 0;
  pppppcVar15 = pppppcVar24;
  pppppcVar29 = pppppcVar24;
  if (0 < *(int *)(param_1 + 0x550)) {
    do {
      pcVar28 = (char *)((longlong)pppppcVar29 + *(longlong *)(param_1 + 0x548));
      if (*(longlong *)(pcVar28 + 8) != 0) {
        FUN_1402dd5a0(param_1 + 0x2228,pcVar28);
        lVar30 = *(longlong *)(pcVar28 + 8);
        local_88 = *(undefined4 *)(lVar30 + 0x78);
        local_84 = *(undefined4 *)(lVar30 + 0x7c);
        local_80 = *(undefined4 *)(lVar30 + 0x80);
        FUN_1402fccb0(param_1,&local_88,
                      *(longlong *)(param_1 + 0x2230) + (longlong)(int)pppppcVar15 * 0x28);
      }
      uVar27 = (int)pppppcVar15 + 1;
      pppppcVar15 = (char *****)(ulonglong)uVar27;
      pppppcVar29 = pppppcVar29 + 5;
    } while ((int)uVar27 < *(int *)(param_1 + 0x550));
  }
  lVar30 = *(longlong *)(param_1 + 0x2150);
  if (lVar30 != 0) {
    *(undefined8 *)(param_1 + 0x2150) = 0;
    FUN_1403de570(param_1,lVar30,0);
  }
  local_78[0] = (char ****)0x0;
  local_60 = 0xf;
  local_58[0] = 0;
  local_48 = 0;
  local_40 = 0xf;
  if (*(char *)(param_1 + 0x21ac) == '\0') {
    plVar9 = (longlong *)FUN_140347b50(0x46);
    uVar23 = local_60;
    if (0xb < local_60) {
      local_68 = 0xc;
      pppppcVar15 = local_78;
      if (0xf < local_60) {
        pppppcVar15 = (char *****)local_78[0];
      }
      FUN_14076bd80(pppppcVar15,"b_planet_tab",0xc);
      *(char *)((longlong)pppppcVar15 + 0xc) = '\0';
      pppppcVar24 = (char *****)local_78[0];
      goto LAB_1402fdb6f;
    }
    uVar20 = 0x7fffffffffffffff;
    if (0x7fffffffffffffff - (local_60 >> 1) < local_60) {
      uVar11 = 0x8000000000000027;
LAB_1402fdab1:
      ppppcVar12 = (char ****)FUN_140769c58(uVar11);
      if (ppppcVar12 == (char ****)0x0) goto LAB_1402fdcc3;
      pppppcVar24 = (char *****)((longlong)ppppcVar12 + 0x27U & 0xffffffffffffffe0);
      pppppcVar24[-1] = ppppcVar12;
    }
    else {
      uVar11 = local_60 + (local_60 >> 1);
      uVar20 = 0xf;
      if (0xf < uVar11) {
        uVar20 = uVar11;
      }
      if (0xfff < uVar20 + 1) {
        uVar11 = uVar20 + 0x28;
        if (uVar11 <= uVar20 + 1) goto LAB_1402fdcc9;
        goto LAB_1402fdab1;
      }
      if (uVar20 != 0xffffffffffffffff) {
        pppppcVar24 = (char *****)FUN_140769c58();
      }
    }
    local_68 = 0xc;
    *pppppcVar24 = (char ****)s_b_planet_tab_14085d660._0_8_;
    *(undefined4 *)(pppppcVar24 + 1) = s_b_planet_tab_14085d660._8_4_;
    *(char *)((longlong)pppppcVar24 + 0xc) = '\0';
    local_60 = uVar20;
LAB_1402fdb33:
    if (0xf < uVar23) {
      uVar20 = uVar23 + 1;
      pppppcVar15 = (char *****)local_78[0];
      if (0xfff < uVar20) {
        pppppcVar15 = (char *****)local_78[0][-1];
        uVar20 = uVar23 + 0x28;
        if ((char *)0x1f < (char *)((longlong)local_78[0] + (-8 - (longlong)pppppcVar15)))
        goto LAB_1402fdcc3;
      }
      thunk_FUN_1407864b8(pppppcVar15,uVar20);
    }
  }
  else {
    plVar9 = (longlong *)FUN_140347b50(0x45);
    uVar23 = local_60;
    if (local_60 < 0xb) {
      uVar20 = 0x7fffffffffffffff;
      if (0x7fffffffffffffff - (local_60 >> 1) < local_60) {
        uVar11 = 0x8000000000000027;
LAB_1402fd9d7:
        ppppcVar12 = (char ****)FUN_140769c58(uVar11);
        if (ppppcVar12 == (char ****)0x0) goto LAB_1402fdcc3;
        pppppcVar24 = (char *****)((longlong)ppppcVar12 + 0x27U & 0xffffffffffffffe0);
        pppppcVar24[-1] = ppppcVar12;
      }
      else {
        uVar11 = local_60 + (local_60 >> 1);
        uVar20 = 0xf;
        if (0xf < uVar11) {
          uVar20 = uVar11;
        }
        if (0xfff < uVar20 + 1) {
          uVar11 = uVar20 + 0x28;
          if (uVar11 <= uVar20 + 1) {
LAB_1402fdcc9:
                    /* WARNING: Subroutine does not return */
            FUN_1400225f0();
          }
          goto LAB_1402fd9d7;
        }
        if (uVar20 != 0xffffffffffffffff) {
          pppppcVar24 = (char *****)FUN_140769c58();
        }
      }
      local_68 = 0xb;
      *pppppcVar24 = (char ****)s_b_space_tab_14085d650._0_8_;
      *(undefined2 *)(pppppcVar24 + 1) = s_b_space_tab_14085d650._8_2_;
      *(char *)((longlong)pppppcVar24 + 10) = s_b_space_tab_14085d650[10];
      *(char *)((longlong)pppppcVar24 + 0xb) = '\0';
      local_60 = uVar20;
      goto LAB_1402fdb33;
    }
    local_68 = 0xb;
    pppppcVar15 = local_78;
    if (0xf < local_60) {
      pppppcVar15 = (char *****)local_78[0];
    }
    FUN_14076bd80(pppppcVar15,"b_space_tab",0xb);
    *(char *)((longlong)pppppcVar15 + 0xb) = '\0';
    pppppcVar24 = (char *****)local_78[0];
  }
LAB_1402fdb6f:
  local_78[0] = (char ****)pppppcVar24;
  cVar6 = FUN_140446220(&DAT_140b30350,1,local_78,local_58);
  if (cVar6 == '\0') {
    FUN_140446c90(&DAT_140b30350,1,local_78,local_58);
  }
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x208))(plVar9,0);
  }
  if (cVar6 == '\0') {
    FUN_1404468e0(&DAT_140b30350,1,local_78,local_58);
  }
  DAT_140b30392 = 0;
  if (*(int *)(param_1 + 0x558) == 3) {
    if (*(int *)(param_1 + 0x211c) == 1) {
      uVar18 = 0x26a;
    }
    else {
      if (*(int *)(param_1 + 0x211c) == 0) goto LAB_1402fdc29;
      uVar18 = 0x26d;
    }
    plVar9 = (longlong *)FUN_140347b50(uVar18);
    if (plVar9 != (longlong *)0x0) {
      (**(code **)(*plVar9 + 0x1d0))(plVar9,3);
      FUN_14031f870(param_1);
    }
  }
LAB_1402fdc29:
  if (0xf < local_40) {
    uVar20 = local_40 + 1;
    uVar23 = local_58[0];
    if (0xfff < uVar20) {
      uVar23 = *(ulonglong *)(local_58[0] - 8);
      uVar20 = local_40 + 0x28;
      if (0x1f < (local_58[0] - uVar23) - 8) goto LAB_1402fdcc3;
    }
    thunk_FUN_1407864b8(uVar23,uVar20);
  }
  local_48 = 0;
  local_40 = 0xf;
  local_58[0] = local_58[0] & 0xffffffffffffff00;
  if (0xf < local_60) {
    uVar23 = local_60 + 1;
    pppppcVar15 = (char *****)local_78[0];
    if (0xfff < uVar23) {
      pppppcVar15 = (char *****)local_78[0][-1];
      uVar23 = local_60 + 0x28;
      if ((char *)0x1f < (char *)((longlong)local_78[0] + (-8 - (longlong)pppppcVar15))) {
LAB_1402fdcc3:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
    }
    thunk_FUN_1407864b8(pppppcVar15,uVar23);
  }
  return;
}



