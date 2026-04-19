
// ================================================================
// FUN_140493d50 — tick subsystem param_1[7] (UNIT THINK CANDIDATE)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x493d50  Name: FUN_140493d50  Size: 1716 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140493d50(char *param_1,longlong *param_2)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  longlong lVar5;
  ulonglong uVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  longlong lVar9;
  longlong lVar10;
  HANDLE pvVar11;
  int iVar12;
  longlong *plVar13;
  ulonglong *puVar14;
  longlong lVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  ulonglong uVar18;
  undefined2 *puVar19;
  uint uVar20;
  ulonglong uVar21;
  float fVar22;
  float fVar23;
  undefined **local_138;
  LPVOID local_130;
  ulonglong local_128;
  longlong local_120;
  longlong local_118;
  undefined4 *local_110;
  longlong local_108;
  longlong local_100;
  longlong local_f8;
  undefined1 local_e8 [56];
  LPVOID local_b0;
  uint local_a4;
  LPVOID local_98;
  uint local_8c;
  LPVOID local_80;
  uint local_74;
  
  if (*param_1 != '\0') {
    FUN_140496140();
  }
  if ((param_2 != (longlong *)0x0) &&
     (iVar3 = (**(code **)(*param_2 + 0xe0))(param_2), iVar1 = DAT_140b0a340, iVar3 == 1)) {
    fVar22 = (float)DAT_140b0a340 * _DAT_140b27680;
    *(longlong **)(param_1 + 8) = param_2;
    *(int *)(param_1 + 0x14) = (int)(longlong)fVar22;
    lVar5 = FUN_140331cc0(&DAT_140a172d0);
    if ((lVar5 != 0) && (lVar5 = FUN_140294d30(&DAT_140a16fd0,lVar5), lVar5 != 0)) {
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(lVar5 + 0x4c);
      FUN_140492340(local_e8);
      uVar6 = DAT_140a16fd8 - DAT_140a16fd0 >> 3;
      if (0 < (int)uVar6) {
        uVar6 = uVar6 & 0xffffffff;
        do {
          iVar3 = *(int *)(param_1 + 0x28);
          iVar12 = *(int *)(param_1 + 0x2c) * 2 >> 1;
          if (iVar12 <= iVar3) {
            iVar3 = iVar12 / 10;
            if (iVar12 / 10 < 10) {
              iVar3 = 10;
            }
            FUN_140495440(param_1 + 0x18,iVar3 + iVar12,0,1);
            iVar3 = *(int *)(param_1 + 0x28);
          }
          *(int *)(param_1 + 0x28) = iVar3 + 1;
          FUN_140492560((longlong)iVar3 * 0x78 + *(longlong *)(param_1 + 0x20),local_e8);
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
      *param_1 = '\x01';
      lVar5 = FUN_14028aa00(&DAT_140b153e0,*(undefined8 *)(param_1 + 8));
      if (lVar5 == 0) {
        uVar17 = 0;
        uVar6 = uVar17;
        if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
          do {
            lVar5 = FUN_140294bc0(&DAT_140a16fd0,uVar6);
            if ((*(char *)(*(longlong *)(lVar5 + 0x68) + 0x20a8) == '\x01') &&
               (lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x68) + 11000), lVar5 != 0)) {
              lVar15 = *(longlong *)(param_1 + 0x20);
              *(undefined2 *)(uVar17 + lVar15) = 1;
              *(undefined1 *)(uVar17 + 0x28 + lVar15) = 0;
              iVar1 = *(int *)(param_1 + 0x14);
              *(longlong *)(uVar17 + 0x20 + lVar15) = lVar5;
              *(int *)(uVar17 + 8 + lVar15) = iVar1;
              *(int *)(uVar17 + 4 + lVar15) = *(int *)(*(longlong *)(param_1 + 8) + 0x10) + iVar1;
            }
            uVar4 = (int)uVar6 + 1;
            uVar17 = uVar17 + 0x78;
            uVar6 = (ulonglong)uVar4;
          } while ((int)uVar4 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
        }
      }
      else {
        puVar7 = (undefined4 *)FUN_1402ac5a0(*(undefined8 *)(lVar5 + 0x18));
        local_110 = puVar7;
        uVar8 = FUN_14028aa50(&DAT_140b153e0,*(undefined8 *)(param_1 + 8),*puVar7);
        lVar5 = FUN_1402acb70(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x18),puVar7[2]);
        uVar6 = 0;
        fVar22 = 0.0;
        if (0 < *(int *)(lVar5 + 0x30)) {
          lVar15 = *(longlong *)(lVar5 + 0x40);
          uVar17 = (ulonglong)*(uint *)(lVar5 + 0x38);
          uVar16 = uVar6;
          do {
            uVar4 = (uint)uVar17 + 1;
            uVar20 = (uint)uVar16;
            if (uVar20 == uVar4) {
              *(uint *)(lVar5 + 0x38) = uVar4;
              lVar15 = *(longlong *)(lVar15 + 8);
              *(longlong *)(lVar5 + 0x40) = lVar15;
              uVar17 = (ulonglong)uVar4;
            }
            else if (uVar20 != (uint)uVar17) {
              lVar15 = *(longlong *)(lVar5 + 0x10);
              *(longlong *)(lVar5 + 0x40) = lVar15;
              *(undefined4 *)(lVar5 + 0x38) = 0;
              uVar17 = uVar6;
              if (uVar20 != 0) {
                do {
                  lVar15 = *(longlong *)(lVar15 + 8);
                  uVar4 = (int)uVar17 + 1;
                  uVar17 = (ulonglong)uVar4;
                  *(longlong *)(lVar5 + 0x40) = lVar15;
                  *(uint *)(lVar5 + 0x38) = uVar4;
                } while ((int)uVar4 < (int)uVar20);
              }
            }
            plVar13 = (longlong *)(*(longlong *)(lVar15 + 0x18) + 0x280);
            if (*(longlong *)(lVar15 + 0x18) == 0) {
              plVar13 = (longlong *)&DAT_00000298;
            }
            if (*(char *)(*plVar13 + 0x73) != '\0') {
              fVar22 = fVar22 + *(float *)(*plVar13 + 0x1f90);
            }
            uVar16 = (ulonglong)(uVar20 + 1);
          } while ((int)(uVar20 + 1) < *(int *)(lVar5 + 0x30));
        }
        lVar5 = FUN_140395900(uVar8);
        bVar2 = false;
        local_f8 = (longlong)*(int *)(lVar5 + 0x10);
        local_138 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
        local_130 = (LPVOID)0x0;
        local_128 = 0;
        local_118 = lVar5;
        if (0 < *(int *)(lVar5 + 0x10)) {
          local_108 = 0;
          uVar16 = uVar6;
          uVar17 = uVar6;
          do {
            lVar15 = *(longlong *)(*(longlong *)(lVar5 + 8) + local_108 * 8);
            if (*(int *)(lVar15 + 0x58) == puVar7[1]) {
              FUN_1403965f0(lVar15,&local_138,1);
              local_100 = (longlong)(int)local_128;
              local_120 = 0;
              lVar5 = local_118;
              puVar7 = local_110;
              if (0 < local_100) {
                do {
                  lVar10 = local_120;
                  lVar5 = *(longlong *)((longlong)local_130 + local_120 * 8);
                  if (*(int *)(*(ulonglong *)(lVar5 + 0x298) + 0x650) == 1) {
                    bVar2 = true;
                    uVar16 = (ulonglong)((int)uVar16 + 1);
                    uVar17 = *(ulonglong *)(lVar5 + 0x298);
                  }
                  else {
                    lVar9 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(lVar15 + 0x58));
                    iVar3 = *(int *)(lVar9 + 0x84);
                    uVar21 = uVar6;
                    if (-1 < iVar3) {
                      do {
                        iVar12 = FUN_140373fd0(*(undefined8 *)(lVar5 + 0x298),uVar21);
                        uVar18 = uVar6;
                        if (0 < iVar12) {
                          do {
                            lVar10 = FUN_140373ee0(*(undefined8 *)(lVar5 + 0x298),uVar21,uVar18);
                            if ((lVar10 != 0) &&
                               (lVar9 = (longlong)*(int *)(lVar10 + 0x980), 0 < lVar9)) {
                              puVar14 = (ulonglong *)(*(longlong *)(lVar10 + 0x978) + 8);
                              do {
                                uVar4 = (uint)uVar16;
                                if (*(int *)(*puVar14 + 0x650) == 1) {
                                  bVar2 = true;
                                  uVar4 = uVar4 + 1;
                                  uVar17 = *puVar14;
                                }
                                puVar14 = puVar14 + 2;
                                uVar16 = (ulonglong)uVar4;
                                lVar9 = lVar9 + -1;
                              } while (lVar9 != 0);
                            }
                            uVar4 = (int)uVar18 + 1;
                            uVar18 = (ulonglong)uVar4;
                          } while ((int)uVar4 < iVar12);
                        }
                        uVar4 = (int)uVar21 + 1;
                        uVar21 = (ulonglong)uVar4;
                        lVar10 = local_120;
                      } while ((int)uVar4 <= iVar3);
                    }
                  }
                  local_120 = lVar10 + 1;
                  lVar5 = local_118;
                  puVar7 = local_110;
                } while (local_120 < local_100);
              }
            }
            local_108 = local_108 + 1;
          } while (local_108 < local_f8);
          if (bVar2) {
            if ((int)uVar16 < 3) {
              uVar16 = 3;
            }
            puVar19 = (undefined2 *)
                      ((longlong)(int)puVar7[1] * 0x78 + *(longlong *)(param_1 + 0x20));
            *(float *)(puVar19 + 10) = fVar22;
            fVar22 = fVar22 + DAT_1407ffaf8;
            *puVar19 = 1;
            *(undefined1 *)(puVar19 + 0x14) = 0;
            fVar23 = (float)((int)(uVar16 / 3) + -1) * _DAT_140b27688;
            *(float *)(puVar19 + 8) = fVar23;
            fVar22 = fVar22 - fVar23;
            if (fVar22 <= 0.0) {
              fVar22 = 0.0;
            }
            *(float *)(puVar19 + 0xc) = fVar22;
            iVar3 = (int)((float)iVar1 * _DAT_140b27684);
            if ((int)((float)iVar1 * _DAT_140b27684) <
                (int)((float)*(uint *)(param_1 + 0x14) * fVar22)) {
              iVar3 = (int)((float)*(uint *)(param_1 + 0x14) * fVar22);
            }
            *(int *)(puVar19 + 4) = iVar3;
            *(undefined8 *)(puVar19 + 0x10) = *(undefined8 *)(uVar17 + 0x1f88);
            *(int *)(puVar19 + 2) = *(int *)(*(longlong *)(param_1 + 8) + 0x10) + iVar3;
            *(undefined4 *)(puVar19 + 6) = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x10);
          }
        }
        local_138 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
        if (local_130 != (LPVOID)0x0) {
          if ((local_128 & 0x8000000000000000) == 0) {
            thunk_FUN_1407864b8(local_130);
          }
          else {
            pvVar11 = GetProcessHeap();
            HeapFree(pvVar11,0,local_130);
          }
        }
      }
      if (local_80 != (LPVOID)0x0) {
        if ((local_74 & 0x80000000) == 0) {
          thunk_FUN_1407864b8(local_80);
        }
        else {
          pvVar11 = GetProcessHeap();
          HeapFree(pvVar11,0,local_80);
        }
      }
      if (local_98 != (LPVOID)0x0) {
        if ((local_8c & 0x80000000) == 0) {
          thunk_FUN_1407864b8(local_98);
        }
        else {
          pvVar11 = GetProcessHeap();
          HeapFree(pvVar11,0,local_98);
        }
      }
      if (local_b0 != (LPVOID)0x0) {
        if ((local_a4 & 0x80000000) != 0) {
          pvVar11 = GetProcessHeap();
                    /* WARNING: Could not recover jumptable at 0x0001404943eb. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          HeapFree(pvVar11,0,local_b0);
          return;
        }
        thunk_FUN_1407864b8(local_b0);
      }
    }
  }
  return;
}




// ================================================================
// FUN_1405284b0 — tick subsystem param_1[8]
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5284b0  Name: FUN_1405284b0  Size: 1203 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1405284b0(char *param_1,longlong *param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  bool bVar3;
  int iVar4;
  ulonglong uVar5;
  longlong lVar6;
  longlong lVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  ulonglong *puVar10;
  HANDLE hHeap;
  int iVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  uint uVar14;
  int iVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  float fVar18;
  undefined **local_d8;
  LPVOID local_d0;
  ulonglong local_c8;
  undefined4 local_ac;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  
  if (*param_1 != '\0') {
    FUN_14052af00();
  }
  if ((param_2 != (longlong *)0x0) && (iVar4 = (**(code **)(*param_2 + 0xe0))(param_2), iVar4 == 1))
  {
    uVar13 = 0;
    param_1[0x50] = '\0';
    iVar4 = DAT_140b0a340;
    param_1[0x51] = '\0';
    param_1[0x52] = '\0';
    param_1[0x53] = '\0';
    fVar18 = (float)DAT_140b0a340 * _DAT_140b2768c;
    *(longlong **)(param_1 + 8) = param_2;
    *(int *)(param_1 + 0x10) = (int)(longlong)fVar18;
    uVar5 = DAT_140a16fd8 - DAT_140a16fd0 >> 3;
    if (0 < (int)uVar5) {
      uVar5 = uVar5 & 0xffffffff;
      do {
        iVar15 = *(int *)(param_1 + 0x28);
        iVar11 = *(int *)(param_1 + 0x2c) * 2 >> 1;
        if (iVar11 <= iVar15) {
          iVar15 = iVar11 / 10;
          if (iVar11 / 10 < 10) {
            iVar15 = 10;
          }
          FUN_140529740(param_1 + 0x18,iVar15 + iVar11,0,1);
          iVar15 = *(int *)(param_1 + 0x28);
        }
        lVar7 = *(longlong *)(param_1 + 0x20);
        lVar6 = (longlong)iVar15;
        *(int *)(param_1 + 0x28) = iVar15 + 1;
        *(undefined4 *)(lVar7 + 0x30 + lVar6 * 0x48) = local_88;
        *(undefined4 *)(lVar7 + 0x34 + lVar6 * 0x48) = local_84;
        *(undefined4 *)(lVar7 + 0x38 + lVar6 * 0x48) = local_80;
        *(undefined2 *)(lVar7 + lVar6 * 0x48) = 0;
        *(undefined4 *)(lVar7 + 4 + lVar6 * 0x48) = 0xffffffff;
        *(undefined4 *)(lVar7 + 8 + lVar6 * 0x48) = 500;
        *(undefined4 *)(lVar7 + 0xc + lVar6 * 0x48) = local_ac;
        *(undefined4 *)(lVar7 + 0x10 + lVar6 * 0x48) = 0xffffffff;
        *(undefined8 *)(lVar7 + 0x18 + lVar6 * 0x48) = 0;
        *(undefined1 *)(lVar7 + 0x20 + lVar6 * 0x48) = 0;
        *(undefined4 *)(lVar7 + 0x24 + lVar6 * 0x48) = local_94;
        *(undefined4 *)(lVar7 + 0x28 + lVar6 * 0x48) = local_90;
        *(undefined4 *)(lVar7 + 0x2c + lVar6 * 0x48) = local_8c;
        *(undefined4 *)(lVar7 + 0x3c + lVar6 * 0x48) = local_7c;
        *(undefined1 *)(lVar7 + 0x40 + lVar6 * 0x48) = 1;
        uVar5 = uVar5 - 1;
      } while (uVar5 != 0);
      param_2 = *(longlong **)(param_1 + 8);
    }
    *param_1 = '\x01';
    lVar7 = FUN_14028aa00(&DAT_140b153e0,param_2);
    if (lVar7 == 0) {
      if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
        uVar5 = uVar13;
        do {
          lVar7 = FUN_140294bc0(&DAT_140a16fd0,uVar5);
          lVar7 = *(longlong *)(lVar7 + 0x68);
          if (((*(char *)(lVar7 + 0x20a8) == '\x01') && (0 < *(int *)(lVar7 + 0x2780))) &&
             (lVar7 = **(longlong **)(lVar7 + 0x2778), lVar7 != 0)) {
            lVar6 = *(longlong *)(param_1 + 0x20);
            *(undefined2 *)(uVar13 + lVar6) = 1;
            *(undefined1 *)(uVar13 + 0x20 + lVar6) = 0;
            fVar18 = (float)iVar4 * _DAT_140b27690;
            *(longlong *)(uVar13 + 0x18 + lVar6) = lVar7;
            iVar11 = (int)fVar18;
            *(int *)(uVar13 + 8 + lVar6) = iVar11;
            iVar15 = *(int *)(*(longlong *)(param_1 + 8) + 0x10);
            *(undefined1 *)(uVar13 + 0x40 + lVar6) = 0;
            *(int *)(uVar13 + 4 + lVar6) = iVar15 + iVar11;
          }
          uVar14 = (int)uVar5 + 1;
          uVar5 = (ulonglong)uVar14;
          uVar13 = uVar13 + 0x48;
        } while ((int)uVar14 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
      }
    }
    else {
      puVar8 = (undefined4 *)FUN_1402ac5a0(*(undefined8 *)(lVar7 + 0x18));
      uVar9 = FUN_14028aa50(&DAT_140b153e0,*(undefined8 *)(param_1 + 8),*puVar8);
      lVar6 = FUN_140395900(uVar9);
      bVar3 = false;
      iVar15 = *(int *)(lVar6 + 0x10);
      local_d8 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
      local_d0 = (LPVOID)0x0;
      local_c8 = 0;
      lVar7 = FUN_140294bc0(&DAT_140a16fd0,puVar8[1]);
      lVar7 = *(longlong *)(lVar7 + 0x68);
      uVar5 = uVar13;
      uVar17 = uVar13;
      if (0 < iVar15) {
        do {
          lVar1 = *(longlong *)(*(longlong *)(lVar6 + 8) + uVar5 * 8);
          if (*(int *)(lVar1 + 0x58) == puVar8[1]) {
            FUN_1403965f0(lVar1,&local_d8,0);
            if (0 < (longlong)(int)local_c8) {
              iVar11 = *(int *)(lVar7 + 0x2780);
              uVar16 = uVar13;
              do {
                if (0 < iVar11) {
                  uVar2 = *(ulonglong *)(*(longlong *)((longlong)local_d0 + uVar16 * 8) + 0x298);
                  puVar10 = *(ulonglong **)(lVar7 + 0x2778);
                  uVar12 = uVar13;
                  do {
                    if (uVar2 == *puVar10) {
                      bVar3 = true;
                      uVar17 = uVar2;
                      break;
                    }
                    uVar12 = uVar12 + 1;
                    puVar10 = puVar10 + 1;
                  } while ((longlong)uVar12 < (longlong)iVar11);
                }
                uVar16 = uVar16 + 1;
              } while ((longlong)uVar16 < (longlong)(int)local_c8);
            }
          }
          uVar5 = uVar5 + 1;
        } while ((longlong)uVar5 < (longlong)iVar15);
        if (bVar3) {
          lVar6 = (longlong)(int)puVar8[1];
          lVar7 = *(longlong *)(param_1 + 0x20);
          *(undefined2 *)(lVar7 + lVar6 * 0x48) = 1;
          *(undefined1 *)(lVar7 + 0x20 + lVar6 * 0x48) = 0;
          fVar18 = (float)iVar4 * _DAT_140b27690;
          *(ulonglong *)(lVar7 + 0x18 + lVar6 * 0x48) = uVar17;
          iVar4 = (int)fVar18;
          *(int *)(lVar7 + 8 + lVar6 * 0x48) = iVar4;
          *(int *)(lVar7 + 4 + lVar6 * 0x48) = *(int *)(*(longlong *)(param_1 + 8) + 0x10) + iVar4;
          *(undefined4 *)(lVar7 + 0x10 + lVar6 * 0x48) =
               *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x10);
          *(undefined1 *)(lVar7 + 0x40 + lVar6 * 0x48) = 0;
        }
      }
      local_d8 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
      if (local_d0 != (LPVOID)0x0) {
        if ((local_c8 & 0x8000000000000000) == 0) {
          thunk_FUN_1407864b8(local_d0);
        }
        else {
          hHeap = GetProcessHeap();
          HeapFree(hHeap,0,local_d0);
        }
      }
    }
  }
  return;
}




// ================================================================
// FUN_1403a8380 — per-object destructor (object structure reference)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3a8380  Name: FUN_1403a8380  Size: 367 bytes
// ------------------------------------------------------------

void FUN_1403a8380(longlong *param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined1 uVar5;
  float fVar6;
  
  *(byte *)(param_1 + 0x74) = *(byte *)(param_1 + 0x74) | 2;
  *(undefined4 *)(param_1 + 0x52) = 0;
  if (param_1[0x54] != 0) {
    FUN_1403a5840();
  }
  if (param_1[0x20] != 0) {
    FUN_14037cc50(param_1[0x20] + 8);
  }
  if (*(char *)((longlong)param_1 + 0x346) != -1) {
    lVar2 = (**(code **)(*param_1 + 0x10))(param_1,0x14);
    if (lVar2 != 0) {
      FUN_1404f9100(lVar2,param_1);
    }
  }
  cVar1 = FUN_1401fecd0();
  if ((cVar1 == '\0') && (*(char *)((longlong)param_1 + 0x344) != -1)) {
    cVar1 = (**(code **)(*DAT_140b15418 + 0xf0))();
    if (cVar1 != '\0') {
      lVar2 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
      if (lVar2 != 0) {
        uVar3 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
        cVar1 = FUN_1402814d0(uVar3);
        if ((cVar1 != '\0') && (*(char *)((longlong)param_1 + 0x337) == -1)) {
          lVar2 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
          lVar2 = FUN_1404b06d0(*(undefined8 *)(lVar2 + 0x360),0);
          if (lVar2 != 0) {
            uVar3 = *(undefined8 *)(lVar2 + 0x58);
            fVar6 = (float)FUN_140396df0(param_1);
            if ((fVar6 <= 0.0) || ((param_1[0x22] != 0 && (*(int *)(param_1[0x22] + 0x18) == 0)))) {
              uVar5 = 0;
            }
            else {
              uVar5 = 1;
            }
            uVar4 = FUN_1403973b0(param_1);
            FUN_14058a9a0(uVar3,uVar4,uVar5);
          }
        }
      }
    }
  }
  uVar3 = FUN_140058570();
  FUN_140220ed0(uVar3,param_1 + 7,0x10);
  return;
}




// ================================================================
// FUN_1402a6a00 — Remove_Object (remove from GOM ReferenceList)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2a6a00  Name: FUN_1402a6a00  Size: 1930 bytes
// ------------------------------------------------------------

void FUN_1402a6a00(longlong param_1,longlong *param_2)

{
  int *piVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  longlong lVar15;
  longlong *plVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  longlong lVar20;
  uint uVar21;
  ulonglong uVar22;
  longlong lVar23;
  ulonglong uVar24;
  longlong local_res8;
  longlong local_res10;
  
  if (param_2[0x57] == param_1) {
    uVar2 = FUN_140058570();
    FUN_140220ed0(uVar2,param_2 + 7,0x28);
  }
  plVar16 = (longlong *)0x0;
  if (*(char *)((longlong)param_2 + 0x33a) == -1) {
    for (lVar3 = *(longlong *)(param_1 + 0x48); lVar3 != param_1 + 0x40;
        lVar3 = *(longlong *)(lVar3 + 8)) {
      plVar5 = (longlong *)(*(longlong *)(lVar3 + 0x18) + -0x18);
      if (*(longlong *)(lVar3 + 0x18) == 0) {
        plVar5 = plVar16;
      }
      if (plVar5 != param_2) {
        FUN_1403920c0(plVar5,param_2);
      }
    }
  }
  if (param_2[0x57] == param_1) {
    lVar3 = FUN_14020abe0(param_1 + 0xe8,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x130,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x178,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x1c8,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x210,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 600,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x2a0,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x2e8,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x330,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x378,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    puVar4 = *(undefined8 **)(param_1 + 0x3c8);
    if ((puVar4 != (undefined8 *)0x0) &&
       (iVar17 = *(int *)(param_1 + 0x3d0), plVar5 = plVar16, 0 < iVar17)) {
      do {
        iVar6 = (int)plVar5;
        if ((longlong *)*puVar4 == param_2) {
          if ((-1 < iVar6) && (iVar6 < iVar17)) {
            FUN_14025c3c0(param_1 + 0x3c0,plVar5,iVar6 + 1,(iVar17 - iVar6) + -1);
            *(int *)(param_1 + 0x3d0) = *(int *)(param_1 + 0x3d0) + -1;
          }
          break;
        }
        plVar5 = (longlong *)(ulonglong)(iVar6 + 1U);
        puVar4 = puVar4 + 1;
      } while ((int)(iVar6 + 1U) < iVar17);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x560,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(param_1 + 0x5a8,param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    lVar3 = FUN_14020abe0(*(longlong *)(param_1 + 200) + (longlong)(int)param_2[0xb] * 0x48,
                          param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    plVar5 = (longlong *)((longlong)(int)param_2[0xb] * 0x38 + *(longlong *)(param_1 + 0xd0));
    (**(code **)(*plVar5 + 0x10))(plVar5,param_2 + 5);
    (**(code **)(**(longlong **)(param_1 + 0xd8) + 0x10))
              (*(longlong **)(param_1 + 0xd8),param_2 + 5);
    plVar5 = *(longlong **)(param_1 + 0x558);
    if (plVar5 != (longlong *)0x0) {
      (**(code **)(*plVar5 + 0x10))(plVar5,param_2 + 5);
    }
    plVar5 = *(longlong **)(param_1 + 0xe0);
    if (plVar5 != (longlong *)0x0) {
      (**(code **)(*plVar5 + 0x10))(plVar5,param_2 + 5);
    }
    lVar3 = FUN_14020abe0(*(longlong *)(param_1 + 0x1c0) + (longlong)(int)param_2[0xb] * 0x48,
                          param_2 + 3);
    if (lVar3 != 0) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    if (((*(longlong *)(param_1 + 0x550) != 0) &&
        (lVar3 = FUN_14020abe0(*(longlong *)(param_1 + 0x550) + (longlong)(int)param_2[0xb] * 0x48,
                               param_2 + 3), lVar3 != 0)) &&
       ((**(code **)(*param_2 + 8))(param_2),
       *(longlong *)(*(longlong *)(param_1 + 0x550) + 0x10 + (longlong)(int)param_2[0xb] * 0x48) ==
       *(longlong *)(param_1 + 0x550) + 8 + (longlong)(int)param_2[0xb] * 0x48)) {
      uVar2 = FUN_140294bc0(&DAT_140a16fd0);
      FUN_140280c50(uVar2);
    }
    iVar17 = (int)param_2[0xb];
    if (((-1 < iVar17) && (iVar17 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) &&
       ((lVar3 = *(longlong *)(param_1 + 0x548) + (longlong)iVar17 * 0x48, lVar3 != 0 &&
        (lVar3 = FUN_14020abe0(lVar3,param_2 + 3), lVar3 != 0)))) {
      (**(code **)(*param_2 + 8))(param_2);
    }
    FUN_1402e9d60(&DAT_140b27f60,param_2);
    FUN_1402d4250(&DAT_140b27e60,param_2);
    (**(code **)(**(longlong **)(param_1 + 0x20) + 600))(*(longlong **)(param_1 + 0x20),param_2);
    lVar3 = *(longlong *)(param_1 + 0x20);
    if (*(longlong *)(lVar3 + 0x30) != 0) {
      FUN_14033fd40(*(longlong *)(lVar3 + 0x30),param_2);
      lVar3 = *(longlong *)(param_1 + 0x20);
    }
    if (*(longlong *)(lVar3 + 0x38) != 0) {
      FUN_140492910(*(longlong *)(lVar3 + 0x38),param_2);
    }
    FUN_1402cbb00(&DAT_140b27a10,param_2);
    lVar3 = *(longlong *)(param_1 + 0x540) + (longlong)(int)param_2[0xb] * 0x18;
    if (lVar3 != 0) {
      iVar17 = *(int *)(lVar3 + 0x10);
      uVar24 = (ulonglong)iVar17;
      if ((iVar17 != 0) && (iVar17 = iVar17 + -1, -1 < iVar17)) {
        lVar23 = (longlong)iVar17 * 8;
        local_res8 = uVar24 << 3;
        local_res10 = lVar23;
        do {
          piVar1 = *(int **)(lVar23 + *(longlong *)(lVar3 + 8));
          iVar6 = (int)param_2[10];
          iVar7 = *piVar1;
          if (iVar7 == iVar6) {
            iVar7 = 0x3fffff;
            *piVar1 = 0x3fffff;
            iVar6 = (int)param_2[10];
          }
          iVar8 = piVar1[1];
          if (iVar8 == iVar6) {
            iVar8 = 0x3fffff;
            piVar1[1] = 0x3fffff;
            iVar6 = (int)param_2[10];
          }
          iVar9 = piVar1[2];
          if (iVar9 == iVar6) {
            iVar9 = 0x3fffff;
            piVar1[2] = 0x3fffff;
            iVar6 = (int)param_2[10];
          }
          iVar10 = piVar1[3];
          if (iVar10 == iVar6) {
            iVar10 = 0x3fffff;
            piVar1[3] = 0x3fffff;
            iVar6 = (int)param_2[10];
          }
          iVar11 = piVar1[4];
          if (iVar11 == iVar6) {
            iVar11 = 0x3fffff;
// ... [truncated at 200 lines, total 316]


// ================================================================
// FUN_1402af130 — register players with GOM
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2af130  Name: FUN_1402af130  Size: 947 bytes
// ------------------------------------------------------------

void FUN_1402af130(longlong param_1,int param_2,undefined8 param_3,undefined1 param_4)

{
  ulonglong *puVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  int iVar8;
  ulonglong uVar9;
  longlong lVar10;
  ulonglong *puVar11;
  ulonglong *puVar12;
  ulonglong *puVar13;
  ulonglong *puVar14;
  ulonglong uVar15;
  
  *(int *)(param_1 + 0x18) = param_2;
  uVar15 = (ulonglong)param_2;
  auVar2._8_8_ = 0;
  auVar2._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar2,0);
  if (SUB168(ZEXT816(0x48) * auVar2,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  *(undefined8 *)(param_1 + 0x20) = param_3;
  lVar10 = uVar9 + 8;
  *(undefined1 *)(param_1 + 0x63e) = param_4;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  puVar13 = (ulonglong *)0x0;
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    }
  }
  *(ulonglong **)(param_1 + 0x1c0) = puVar12;
  auVar3._8_8_ = 0;
  auVar3._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar3,0);
  if (SUB168(ZEXT816(0x48) * auVar3,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    }
  }
  *(ulonglong **)(param_1 + 200) = puVar12;
  auVar4._8_8_ = 0;
  auVar4._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x18) * auVar4,0);
  if (SUB168(ZEXT816(0x18) * auVar4,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar11 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar11 = puVar11 + 1;
    puVar12 = puVar11;
    for (uVar9 = uVar15; uVar9 != 0; uVar9 = uVar9 - 1) {
      *puVar12 = (ulonglong)DynamicVectorClass<struct_CompanyStruct*___ptr64>::vftable;
      puVar12[1] = 0;
      puVar12[2] = 0;
      puVar12 = puVar12 + 3;
    }
  }
  *(ulonglong **)(param_1 + 0x540) = puVar11;
  auVar5._8_8_ = 0;
  auVar5._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x38) * auVar5,0);
  if (SUB168(ZEXT816(0x38) * auVar5,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar12 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  puVar11 = puVar13;
  if (puVar12 != (ulonglong *)0x0) {
    puVar11 = puVar12 + 1;
    *puVar12 = uVar15;
    puVar12 = puVar11;
    for (uVar9 = uVar15; uVar9 != 0; uVar9 = uVar9 - 1) {
      FUN_14020e370(puVar12);
      puVar12 = puVar12 + 7;
    }
  }
  *(ulonglong **)(param_1 + 0xd0) = puVar11;
  lVar10 = FUN_140769c58(0x38);
  puVar11 = puVar13;
  if (lVar10 != 0) {
    puVar11 = (ulonglong *)FUN_14020e370(lVar10);
  }
  *(ulonglong **)(param_1 + 0xd8) = puVar11;
  lVar10 = FUN_140769c58(0x38);
  puVar11 = puVar13;
  if (lVar10 != 0) {
    puVar11 = (ulonglong *)FUN_14020e370(lVar10);
  }
  *(ulonglong **)(param_1 + 0xe0) = puVar11;
  auVar6._8_8_ = 0;
  auVar6._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar6,0);
  if (SUB168(ZEXT816(0x48) * auVar6,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
// ... [truncated at 200 lines, total 255]


// ================================================================
// FUN_1404922d0 — alloc land battle subsystem 7
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4922d0  Name: FUN_1404922d0  Size: 109 bytes
// ------------------------------------------------------------

undefined1 * FUN_1404922d0(undefined1 *param_1)

{
  *(undefined ***)(param_1 + 0x18) = DynamicVectorClass<class_PlayerBombingRunClass>::vftable;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  if (*(longlong *)(param_1 + 0x20) != 0) {
    FUN_140492670(*(longlong *)(param_1 + 0x20),3);
    *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0x80000000;
    *(undefined8 *)(param_1 + 0x20) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
  }
  *param_1 = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0xffffffff;
  return param_1;
}




// ================================================================
// FUN_140527f30 — alloc land battle subsystem 8
// ================================================================

// ------------------------------------------------------------
// RVA: 0x527f30  Name: FUN_140527f30  Size: 125 bytes
// ------------------------------------------------------------

undefined1 * FUN_140527f30(undefined1 *param_1)

{
  *(undefined ***)(param_1 + 0x18) = DynamicVectorClass<class_PlayerPlanetaryBombardClass>::vftable;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined ***)(param_1 + 0x30) =
       DynamicVectorClass<class_PlanetaryBombardLightClass*___ptr64>::vftable;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  if (*(longlong *)(param_1 + 0x20) != 0) {
    thunk_FUN_1407864b8(*(longlong *)(param_1 + 0x20));
    *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0x80000000;
    *(undefined8 *)(param_1 + 0x20) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
  }
  *param_1 = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined4 *)(param_1 + 0x50) = 0;
  return param_1;
}




// ================================================================
// FUN_140532b70 — space battle AI/render system (param_1[6] for space)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x532b70  Name: FUN_140532b70  Size: 44 bytes
// ------------------------------------------------------------

undefined8 * FUN_140532b70(undefined8 *param_1)

{
  FUN_14033f930();
  *param_1 = SpaceRetreatCoordinatorClass::vftable;
  param_1[1] = SpaceRetreatCoordinatorClass::vftable;
  return param_1;
}




// ================================================================
// FUN_140534bd0 — land battle AI/render system (param_1[6] for land)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x534bd0  Name: FUN_140534bd0  Size: 44 bytes
// ------------------------------------------------------------

undefined8 * FUN_140534bd0(undefined8 *param_1)

{
  FUN_14033f930();
  *param_1 = LandRetreatCoordinatorClass::vftable;
  param_1[1] = LandRetreatCoordinatorClass::vftable;
  return param_1;
}



