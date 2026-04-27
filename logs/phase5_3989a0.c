/* FUN_1403989a0  RVA 0x3989a0  size: 2733 bytes */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403989a0(longlong param_1,char param_2,char param_3,char param_4)

{
  longlong lVar1;
  longlong lVar2;
  LPVOID lpMem;
  undefined8 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 *puVar9;
  HANDLE pvVar10;
  undefined8 uVar11;
  undefined8 *puVar12;
  int iVar13;
  longlong *plVar14;
  uint uVar15;
  undefined8 *puVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  longlong local_res8;
  uint local_res20;
  undefined8 in_stack_ffffffffffffff88;
  undefined **local_60;
  longlong *local_58;
  undefined8 local_50;
  
  puVar12 = (undefined8 *)0x0;
  *(undefined4 *)(param_1 + 0x290) = 0;
  *(undefined1 *)(param_1 + 0x39e) = 0;
  if (*(longlong **)(param_1 + 0x2a0) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x2a0) + 8))();
    *(undefined8 *)(param_1 + 0x2a0) = 0;
  }
  lVar7 = *(longlong *)(param_1 + 0x2a8);
  if (lVar7 != 0) {
    (**(code **)(*(longlong *)(lVar7 + 0x40) + 8))(lVar7 + 0x40);
    *(undefined8 *)(param_1 + 0x2a8) = 0;
  }
  *(undefined8 *)(param_1 + 0x2b0) = 0;
  *(undefined8 *)(param_1 + 0x2d0) = 0;
  if (param_4 != '\0') {
    *(undefined8 *)(param_1 + 0xa8) = 0;
    *(undefined8 *)(param_1 + 0xb0) = 0;
    *(undefined8 *)(param_1 + 0xb8) = 0;
    *(undefined8 *)(param_1 + 0xc0) = 0;
    *(undefined8 *)(param_1 + 200) = 0;
    *(undefined8 *)(param_1 + 0xd0) = 0;
    *(undefined8 *)(param_1 + 0xd8) = 0;
    *(undefined8 *)(param_1 + 0xe0) = 0;
    *(undefined8 *)(param_1 + 0xe8) = 0;
    *(undefined8 *)(param_1 + 0xf0) = 0;
    *(undefined8 *)(param_1 + 0xf8) = 0;
    *(undefined8 *)(param_1 + 0x100) = 0;
    *(undefined8 *)(param_1 + 0x108) = 0;
    *(undefined8 *)(param_1 + 0x110) = 0;
    *(undefined8 *)(param_1 + 0x118) = 0;
    *(undefined8 *)(param_1 + 0x120) = 0;
    *(undefined8 *)(param_1 + 0x128) = 0;
    *(undefined8 *)(param_1 + 0x130) = 0;
    *(undefined8 *)(param_1 + 0x150) = 0;
    *(undefined8 *)(param_1 + 0x138) = 0;
    *(undefined8 *)(param_1 + 0x140) = 0;
    *(undefined8 *)(param_1 + 0x148) = 0;
    *(undefined8 *)(param_1 + 0x158) = 0;
    *(undefined8 *)(param_1 + 0x160) = 0;
    *(undefined8 *)(param_1 + 0x168) = 0;
    *(undefined8 *)(param_1 + 0x180) = 0;
    *(undefined8 *)(param_1 + 0x188) = 0;
    *(undefined8 *)(param_1 + 400) = 0;
    *(undefined8 *)(param_1 + 0x198) = 0;
    *(undefined8 *)(param_1 + 0x1e0) = 0;
    *(undefined8 *)(param_1 + 0x1a0) = 0;
    *(undefined8 *)(param_1 + 0x1a8) = 0;
    *(undefined8 *)(param_1 + 0x1b0) = 0;
    *(undefined8 *)(param_1 + 0x1b8) = 0;
    *(undefined8 *)(param_1 + 0x1c0) = 0;
    *(undefined8 *)(param_1 + 0x1c8) = 0;
    *(undefined8 *)(param_1 + 0x1d0) = 0;
    *(undefined8 *)(param_1 + 0x1d8) = 0;
    *(undefined8 *)(param_1 + 0x1e8) = 0;
    *(undefined8 *)(param_1 + 0x170) = 0;
    *(undefined8 *)(param_1 + 0x178) = 0;
    *(undefined8 *)(param_1 + 0x1f0) = 0;
    *(undefined8 *)(param_1 + 0x1f8) = 0;
    *(undefined8 *)(param_1 + 0x200) = 0;
    *(undefined8 *)(param_1 + 0x208) = 0;
    *(undefined8 *)(param_1 + 0x210) = 0;
    *(undefined8 *)(param_1 + 0x218) = 0;
    *(undefined8 *)(param_1 + 0x220) = 0;
    *(undefined8 *)(param_1 + 0x228) = 0;
    *(undefined8 *)(param_1 + 0x230) = 0;
    *(undefined8 *)(param_1 + 0x240) = 0;
  }
  uVar3 = _UNK_140866888;
  uVar11 = _DAT_140866880;
  *(undefined1 *)(param_1 + 0x39f) = 1;
  *(undefined8 *)(param_1 + 0x332) = uVar11;
  *(undefined8 *)(param_1 + 0x33a) = uVar3;
  *(undefined8 *)(param_1 + 0x342) = uVar11;
  *(undefined8 *)(param_1 + 0x34a) = uVar3;
  *(undefined8 *)(param_1 + 0x352) = uVar11;
  *(undefined8 *)(param_1 + 0x35a) = uVar3;
  *(undefined8 *)(param_1 + 0x362) = uVar11;
  *(undefined8 *)(param_1 + 0x36a) = uVar3;
  *(undefined8 *)(param_1 + 0x372) = uVar11;
  *(undefined8 *)(param_1 + 0x37a) = uVar3;
  *(undefined8 *)(param_1 + 0x382) = uVar11;
  *(undefined8 *)(param_1 + 0x38a) = uVar3;
  *(undefined8 *)(param_1 + 0x392) = uVar11;
  *(short *)(param_1 + 0x39a) = (short)uVar11;
  uVar17 = FUN_1403727a0(*(undefined8 *)(param_1 + 0x298),0);
  lVar7 = *(longlong *)(param_1 + 0x298);
  *(undefined4 *)(param_1 + 0x5c) = uVar17;
  *(undefined8 *)(param_1 + 0x300) = 0;
  *(undefined2 *)(param_1 + 0x3a6) = 0;
  *(undefined4 *)(param_1 + 0x3a8) = 0;
  *(undefined4 *)(param_1 + 0x3b5) = 0;
  *(undefined8 *)(param_1 + 0x3c0) = 0;
  *(undefined1 *)(param_1 + 0x3b9) = 0;
  if (lVar7 == 0) goto LAB_140399405;
  lVar8 = *(longlong *)(lVar7 + 0x1ab0);
  puVar16 = puVar12;
  if (*(longlong *)(lVar7 + 0x1ab8) - lVar8 >> 3 != 0) {
    do {
      lVar8 = *(longlong *)(lVar8 + (longlong)puVar16 * 8);
      if (*(int *)(lVar8 + 0x18) - 1U < 0x4c) {
        lVar1 = (longlong)*(int *)(lVar8 + 0x18) + 0x3d;
        lVar2 = *(longlong *)(param_1 + lVar1 * 0x10);
        if (lVar2 != lVar8) {
          if (lVar2 != 0) {
            (**(code **)(*(longlong *)(lVar2 + 8) + 8))(lVar2 + 8);
          }
          *(longlong *)(param_1 + lVar1 * 0x10) = lVar8;
          if (lVar8 != 0) {
            (*(code *)**(undefined8 **)(lVar8 + 8))(lVar8 + 8);
          }
        }
      }
      lVar8 = *(longlong *)(lVar7 + 0x1ab0);
      puVar16 = (undefined8 *)(ulonglong)((int)puVar16 + 1);
    } while (puVar16 < (undefined8 *)(*(longlong *)(lVar7 + 0x1ab8) - lVar8 >> 3));
  }
  if (DAT_140b2c37b == '\0') {
    FUN_1403a4820(param_1);
  }
  if (*(longlong *)(param_1 + 0x2a0) != 0) {
    FUN_1403a8710(param_1,param_1 + 0x84);
    if ((*(char *)(*(longlong *)(param_1 + 0x298) + 0x23) != '\0') &&
       (lVar7 = (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x2b8) + 0x20) + 0x218))(),
       lVar7 != 0)) {
      FUN_1403a4510(param_1);
    }
    *(undefined4 *)(*(longlong *)(param_1 + 0x2a0) + 0x88) = 0xffffffff;
    if (param_2 == '\0') {
      *(undefined4 *)(param_1 + 0x78) = *(undefined4 *)(param_1 + 0x78);
      *(undefined4 *)(param_1 + 0x7c) = *(undefined4 *)(param_1 + 0x7c);
      *(undefined4 *)(param_1 + 0x80) = *(undefined4 *)(param_1 + 0x80);
      FUN_1403ac530(param_1,1);
    }
    FUN_140265ae0(*(undefined8 *)(param_1 + 0x2a0),1);
    FUN_140265ae0(*(undefined8 *)(param_1 + 0x2a0),5);
    FUN_140265ae0(*(undefined8 *)(param_1 + 0x2a0),7);
  }
  if ((*(byte *)(param_1 + 0x3a1) & 1) != 0) {
    FUN_1403ac530(param_1,0);
  }
  if ((param_2 == '\0') || (param_3 != '\0')) {
    lVar7 = *(longlong *)(param_1 + 0x298) + 0x6d0;
    if (lVar7 != 0) {
      FUN_14038cb30(param_1,lVar7,param_2);
    }
    iVar5 = (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x2b8) + 0x20) + 0xe0))();
    if (iVar5 == 0) {
      puVar16 = (undefined8 *)(*(longlong *)(param_1 + 0x298) + 0x6e8);
    }
    else if (iVar5 == 1) {
      puVar16 = (undefined8 *)(*(longlong *)(param_1 + 0x298) + 0x718);
    }
    else {
      puVar16 = puVar12;
      if (iVar5 == 2) {
        puVar16 = (undefined8 *)(*(longlong *)(param_1 + 0x298) + 0x700);
      }
    }
    if (*(char *)(param_1 + 0x289) < *(char *)(puVar16 + 2)) {
      FUN_1403a59f0();
    }
    FUN_14038cb30(param_1,puVar16,param_2);
    lVar7 = *(longlong *)(param_1 + 0x298);
    if (*(longlong *)(lVar7 + 0x1ae8) != 0) {
      puVar16 = (undefined8 *)(lVar7 + 0x1ad8);
      if (0xf < *(ulonglong *)(lVar7 + 0x1af0)) {
        puVar16 = (undefined8 *)*puVar16;
      }
      if (puVar16 != (undefined8 *)0x0) {
        puVar16 = (undefined8 *)(lVar7 + 0x1ad8);
        if (0xf < *(ulonglong *)(lVar7 + 0x1af0)) {
          puVar16 = (undefined8 *)*puVar16;
        }
        FUN_14038cf30(param_1,puVar16,0);
      }
    }
    iVar5 = FUN_140294a70(&DAT_140a16fd0);
    if ((*(int *)(param_1 + 0x58) == iVar5) && (DAT_140a7c476 != '\0')) {
      lVar7 = *(longlong *)(param_1 + 0x298);
      lVar8 = *(longlong *)(lVar7 + 0x1ab0);
      puVar16 = puVar12;
      if (*(longlong *)(lVar7 + 0x1ab8) - lVar8 >> 3 != 0) {
        do {
          lVar8 = *(longlong *)(lVar8 + (longlong)puVar16 * 8);
          if ((*(char *)(lVar8 + 0x24c) != '\0') &&
             (iVar5 = *(int *)(lVar8 + 0x18), iVar5 - 1U < 0x4c)) {
            local_60 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
            local_58 = (longlong *)0x0;
            local_50 = 0;
            local_58 = (longlong *)thunk_FUN_140769c58(0x50);
            *local_58 = param_1;
            local_50 = CONCAT44(local_50._4_4_,1) & 0x8000000affffffff | 0xa00000000;
            lVar8 = FUN_140769c58(0x70);
            puVar9 = puVar12;
            if (lVar8 != 0) {
              puVar9 = (undefined8 *)FUN_1404b3580(lVar8);
            }
            FUN_1404b3900(puVar9,&local_60,iVar5,1);
            FUN_1403b01f0(&DAT_140b2ed18,puVar9);
            local_60 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
            if (local_58 != (longlong *)0x0) {
              if ((local_50 & 0x8000000000000000) == 0) {
                thunk_FUN_1407864b8();
              }
              else {
                pvVar10 = GetProcessHeap();
                HeapFree(pvVar10,0,local_58);
              }
            }
          }
          lVar8 = *(longlong *)(lVar7 + 0x1ab0);
          puVar16 = (undefined8 *)(ulonglong)((int)puVar16 + 1);
        } while (puVar16 < (undefined8 *)(*(longlong *)(lVar7 + 0x1ab8) - lVar8 >> 3));
      }
    }
  }
  if (-1 < *(char *)(param_1 + 0x3a0)) {
    puVar16 = *(undefined8 **)(param_1 + 0x2d0);
    if (puVar16 != (undefined8 *)0x0) {
      lpMem = (LPVOID)puVar16[1];
      if (lpMem == (LPVOID)0x0) {
LAB_140399041:
        (**(code **)*puVar16)(puVar16,1);
      }
      else {
        if ((*(uint *)((longlong)puVar16 + 0x14) & 0x80000000) == 0) {
          thunk_FUN_1407864b8(lpMem);
        }
        else {
          pvVar10 = GetProcessHeap();
          HeapFree(pvVar10,0,lpMem);
        }
        *(uint *)((longlong)puVar16 + 0x14) = *(uint *)((longlong)puVar16 + 0x14) & 0x80000000;
        puVar16[1] = 0;
        *(undefined4 *)(puVar16 + 2) = 0;
        puVar16 = *(undefined8 **)(param_1 + 0x2d0);
        if (puVar16 != (undefined8 *)0x0) goto LAB_140399041;
      }
      *(undefined8 *)(param_1 + 0x2d0) = 0;
    }
    if (*(longlong *)(param_1 + 0x2a0) != 0) {
      iVar5 = *(int *)(*(longlong *)(param_1 + 0x298) + 0xe40);
      if (0 < iVar5) {
        puVar9 = (undefined8 *)FUN_140769c58(0x18);
        puVar16 = puVar12;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = 0;
          *puVar9 = DynamicVectorClass<class_HardPointClass*___ptr64>::vftable;
          puVar9[2] = 0;
          puVar16 = puVar9;
        }
        *(undefined8 **)(param_1 + 0x2d0) = puVar16;
        local_res20 = 0;
        if (0 < iVar5) {
          local_res8 = 0;
          puVar16 = puVar12;
          do {
            lVar7 = *(longlong *)
                     (*(longlong *)(*(longlong *)(param_1 + 0x298) + 0xe38) + local_res8 * 8);
            lVar8 = FUN_140769c58(0xd8);
            uVar17 = (undefined4)((ulonglong)in_stack_ffffffffffffff88 >> 0x20);
            puVar9 = puVar12;
            if (lVar8 != 0) {
              uVar18 = FUN_140540070(lVar7);
              in_stack_ffffffffffffff88 = CONCAT44(uVar17,uVar18);
              puVar9 = (undefined8 *)
                       FUN_140381a90(lVar8,param_1,puVar16,lVar7,in_stack_ffffffffffffff88);
            }
            uVar11 = FUN_1402648b0(*(undefined8 *)(param_1 + 0x2a0));
            if (*(longlong *)(lVar7 + 0x90) == 0) {
              uVar17 = 0xffffffff;
            }
            else {
              plVar14 = (longlong *)(lVar7 + 0x80);
              if (0xf < *(ulonglong *)(lVar7 + 0x98)) {
                plVar14 = (longlong *)*plVar14;
              }
              uVar17 = FUN_14012d2a0(uVar11,plVar14);
            }
            *(undefined4 *)((longlong)puVar9 + 0x2c) = uVar17;
            lVar8 = FUN_1403858b0(puVar9);
            if (lVar8 != 0) {
              if (*(longlong *)(lVar7 + 400) == 0) {
                *(undefined4 *)((longlong)puVar9 + 0x3c) = 0xffffffff;
              }
              else {
                plVar14 = (longlong *)(lVar7 + 0x180);
                if (0xf < *(ulonglong *)(lVar7 + 0x198)) {
                  plVar14 = (longlong *)*plVar14;
                }
                uVar17 = FUN_14012d2a0(lVar8,plVar14);
                *(undefined4 *)(puVar9 + 7) = uVar17;
              }
              if (*(longlong *)(lVar7 + 0x1b0) == 0) {
                *(undefined4 *)((longlong)puVar9 + 0x3c) = 0xffffffff;
              }
              else {
                plVar14 = (longlong *)(lVar7 + 0x1a0);
                if (0xf < *(ulonglong *)(lVar7 + 0x1b8)) {
                  plVar14 = (longlong *)*plVar14;
                }
                uVar17 = FUN_14012d2a0(lVar8,plVar14);
                *(undefined4 *)((longlong)puVar9 + 0x3c) = uVar17;
              }
            }
            *(undefined4 *)(puVar9 + 6) = 0xffffffff;
            uVar15 = (uint)puVar16;
            if (*(longlong *)(lVar7 + 0xf0) != 0) {
              plVar14 = (longlong *)(lVar7 + 0xe0);
              if (0xf < *(ulonglong *)(lVar7 + 0xf8)) {
                plVar14 = (longlong *)*plVar14;
              }
              uVar17 = FUN_14012d2a0(uVar11,plVar14);
              iVar6 = FUN_14012d6d0(uVar11);
              puVar16 = puVar12;
              if (0 < iVar6) {
                do {
                  cVar4 = FUN_14012dae0(uVar11,puVar16,uVar17);
                  if (cVar4 == '\x01') {
                    lVar8 = FUN_14012d6f0(uVar11,puVar16);
                    *(byte *)(lVar8 + 0x30) = *(byte *)(lVar8 + 0x30) | 1;
                  }
                  uVar15 = (int)puVar16 + 1;
                  puVar16 = (undefined8 *)(ulonglong)uVar15;
                } while ((int)uVar15 < iVar6);
              }
              *(undefined4 *)(puVar9 + 6) = uVar17;
              uVar15 = local_res20;
            }
            *(undefined4 *)((longlong)puVar9 + 0x34) = 0xffffffff;
            if (*(longlong *)(lVar7 + 0x110) != 0) {
              plVar14 = (longlong *)(lVar7 + 0x100);
              if (0xf < *(ulonglong *)(lVar7 + 0x118)) {
                plVar14 = (longlong *)*plVar14;
              }
              uVar17 = FUN_14012d2a0(uVar11,plVar14);
              *(undefined4 *)((longlong)puVar9 + 0x34) = uVar17;
            }
            lVar7 = *(longlong *)(param_1 + 0x2d0);
            iVar6 = *(int *)(lVar7 + 0x10);
            iVar13 = *(int *)(lVar7 + 0x14) * 2 >> 1;
            if (iVar13 <= iVar6) {
              iVar6 = iVar13 / 10;
              if (iVar13 / 10 < 10) {
                iVar6 = 10;
              }
              FUN_140047aa0(lVar7,iVar6 + iVar13,0,1);
              iVar6 = *(int *)(lVar7 + 0x10);
            }
            local_res20 = uVar15 + 1;
            puVar16 = (undefined8 *)(ulonglong)local_res20;
            local_res8 = local_res8 + 1;
            *(undefined8 **)(*(longlong *)(lVar7 + 8) + (longlong)iVar6 * 8) = puVar9;
            *(int *)(lVar7 + 0x10) = *(int *)(lVar7 + 0x10) + 1;
          } while (local_res8 < iVar5);
        }
      }
      lVar7 = *(longlong *)(param_1 + 0x2d0);
      if ((lVar7 != 0) && (puVar16 = puVar12, puVar9 = puVar12, 0 < *(int *)(lVar7 + 0x10))) {
        do {
          if (*(longlong *)((longlong)puVar9 + *(longlong *)(lVar7 + 8)) != 0) {
            FUN_140384740();
            lVar7 = *(longlong *)(param_1 + 0x2d0);
          }
          uVar15 = (int)puVar16 + 1;
          puVar16 = (undefined8 *)(ulonglong)uVar15;
          puVar9 = puVar9 + 1;
        } while ((int)uVar15 < *(int *)(lVar7 + 0x10));
      }
    }
  }
  cVar4 = FUN_140374da0(*(undefined8 *)(param_1 + 0x298),param_1);
  if (cVar4 != '\0') {
    puVar16 = *(undefined8 **)(param_1 + 0x100);
    if (puVar16 == (undefined8 *)0x0) {
      lVar7 = FUN_140769c58(0x3b8);
      puVar16 = puVar12;
      if (lVar7 != 0) {
        puVar16 = (undefined8 *)FUN_14055d530(lVar7);
      }
      *(undefined8 **)(param_1 + 0x100) = puVar16;
      if (puVar16 == (undefined8 *)0x0) goto LAB_1403993e9;
    }
    FUN_14037c050(puVar16 + 1,param_1,param_2,param_3,
                  *(undefined8 *)(*(longlong *)(param_1 + 0x2b8) + 0x20));
    cVar4 = FUN_14037d7e0(*(longlong *)(param_1 + 0x100) + 8);
    if ((cVar4 != '\0') && (*(char *)(param_1 + 0x39e) == '\0')) {
      FUN_14029bfe0(*(undefined8 *)(param_1 + 0x2b8),param_1);
      *(undefined1 *)(param_1 + 0x39e) = 1;
    }
  }
LAB_1403993e9:
  if (*(char *)(*(longlong *)(param_1 + 0x298) + 0x2f) != '\0') {
    FUN_14029bfa0(*(undefined8 *)(param_1 + 0x2b8),param_1);
  }
LAB_140399405:
  if (*(longlong *)(param_1 + 0x200) == 0) {
    lVar7 = FUN_140769c58(0x20);
    if (lVar7 != 0) {
      puVar12 = (undefined8 *)FUN_140532990(lVar7);
    }
    *(undefined8 **)(param_1 + 0x200) = puVar12;
  }
  return;
}

