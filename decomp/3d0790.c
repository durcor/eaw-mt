/* FUN_1403d0790  RVA 0x3d0790  size 2756  params 0  ret undefined */


/* WARNING: Removing unreachable block (ram,0x0001403d0f23) */

void FUN_1403d0790(longlong param_1,longlong param_2,undefined8 param_3,longlong param_4)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  longlong *plVar10;
  longlong *plVar11;
  HANDLE pvVar12;
  longlong *plVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  longlong *plVar17;
  int iVar18;
  undefined4 *puVar19;
  ulonglong uVar20;
  longlong *plVar21;
  uint uVar22;
  longlong local_res8;
  undefined4 in_stack_ffffffffffffff58;
  uint3 uVar23;
  longlong *local_88;
  ulonglong local_80;
  undefined **local_70;
  longlong *local_68;
  undefined8 local_60;
  uint local_58;
  longlong *local_50;
  longlong *local_48;
  longlong local_40;
  
  if (*(longlong *)(param_1 + 0x2150) == 0) {
    return;
  }
  if (*(longlong *)(param_1 + 0xdf0) == 0) {
    return;
  }
  lVar6 = FUN_14034c770(param_3,0);
  lVar9 = *(longlong *)(param_2 + 0x90);
  plVar11 = (longlong *)0x0;
  local_50 = (longlong *)0x0;
  iVar5 = 0;
  lVar7 = FUN_140347b50(0x78);
  if ((lVar7 != 0) && (*(longlong *)(lVar7 + 0x90) == lVar9)) {
    lVar7 = FUN_140347b50(*(int *)(param_2 + 0xa8) + 1);
    if (lVar7 != 0) {
      lVar9 = *(longlong *)(lVar7 + 0x90);
    }
    lVar7 = FUN_14034c770(param_2,0);
    if (lVar7 != 0) {
      FUN_1403df5a0(param_1,lVar7,0xffffffff);
    }
  }
  puVar19 = &DAT_140a28880;
  plVar10 = plVar11;
  do {
    lVar7 = FUN_140347b50(*puVar19);
    iVar18 = (int)plVar10;
    if (*(longlong *)(lVar7 + 0x90) == lVar9) {
      local_50 = *(longlong **)(*(longlong *)(param_1 + 0xdf0) + 8 + (longlong)iVar18 * 8);
      plVar17 = local_50;
      iVar5 = iVar18;
      break;
    }
    plVar10 = (longlong *)(ulonglong)(iVar18 + 1);
    puVar19 = puVar19 + 1;
    plVar17 = plVar11;
  } while ((longlong)puVar19 < 0x140a2888c);
  iVar18 = *(int *)(param_2 + 0xa8) - (&DAT_140a28880)[iVar5];
  uVar8 = FUN_14034c630(param_3);
  FUN_140025760("Item %s dropped in slot %d\n",uVar8,iVar18);
  plVar10 = *(longlong **)(lVar6 + 0x2b0);
  local_48 = plVar10;
  if (plVar17 == (longlong *)0x0) {
    if (*(char *)(param_1 + 0x21a9) == '\0') {
      if ((iVar18 == DAT_140b278b0) ||
         ((*(longlong *)(lVar6 + 0x1b0) != 0 &&
          (*(char *)(*(longlong *)(lVar6 + 0x1b0) + 0x32) != '\0')))) {
        lVar9 = FUN_140769c58(0x50);
        plVar10 = plVar11;
        if (lVar9 != 0) {
          plVar10 = (longlong *)FUN_1405b1170(lVar9);
        }
        FUN_1405b06b0(plVar10,*(undefined4 *)(lVar6 + 0x50),iVar18);
        OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar10);
      }
      FUN_1403a87c0(lVar6,iVar18);
    }
    else {
      uVar8 = FUN_1402f6270(param_1,lVar6);
      FUN_1403e0eb0(param_1,plVar10,uVar8,iVar18);
    }
    if (param_4 != 0) {
      plVar11 = (longlong *)FUN_14034c770(param_4,0);
    }
    FUN_1403cf7d0(param_1,lVar6,iVar5,plVar11);
    return;
  }
  if ((*(char *)(lVar6 + 0x336) != -1) &&
     ((plVar10 == (longlong *)0x0 || (*(char *)((longlong)plVar10 + 0x336) == -1)))) {
    lVar9 = FUN_140769c58(0x68);
    if (lVar9 != 0) {
      plVar11 = (longlong *)FUN_1405af020(lVar9);
    }
    FUN_1405afb60(plVar11,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x50),(int)plVar17[10],
                  *(undefined4 *)(lVar6 + 0x50),3,0,0x3fffff,0x3fffff);
    OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar11);
    return;
  }
  uVar4 = FUN_140294a70(&DAT_140a16fd0);
  cVar3 = FUN_14039a230(plVar17,uVar4,0);
  if (cVar3 == '\0') {
    return;
  }
  FUN_14032edd0(DAT_140b15418 + 0xa0,"drag_unit");
  if (plVar10 == (longlong *)0x0) {
    return;
  }
  uVar23 = (uint3)((uint)in_stack_ffffffffffffff58 >> 8);
  if (*(char *)((longlong)plVar10 + 0x336) == -1) {
    if (*(char *)((longlong)plVar10 + 0x335) == -1) {
      return;
    }
    iVar5 = FUN_1403cb630(param_1,lVar6,iVar5,iVar18,(uint)uVar23 << 8);
    if (*(char *)(param_1 + 0x21a9) == '\0') {
      FUN_1403df5a0(param_1,lVar6,iVar5);
    }
    else {
      uVar8 = FUN_1402f6270();
      FUN_1403e0eb0(param_1,plVar10,uVar8,iVar5);
    }
    if (*(char *)(lVar6 + 0x337) == -1) {
      return;
    }
    local_68 = (longlong *)0x0;
    local_70 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    local_60 = 0;
    if ((*(char *)(param_1 + 0x21a9) == '\0') || (cVar3 = FUN_14034cef0(param_3), cVar3 == '\0')) {
      local_88 = (longlong *)thunk_FUN_140769c58(0x50);
      *local_88 = lVar6;
      uVar16 = 1;
      iVar18 = 10;
LAB_1403d1047:
      plVar10 = local_48;
      local_80 = (ulonglong)uVar16;
      plVar17 = local_88;
      uVar20 = local_80;
      do {
        lVar9 = *plVar17;
        lVar6 = FUN_140769c58(0x68);
        plVar13 = plVar11;
        if (lVar6 != 0) {
          plVar13 = (longlong *)FUN_1405af020(lVar6);
        }
        FUN_1405afb60(plVar13,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x50),
                      (int)plVar10[10],*(undefined4 *)(lVar9 + 0x50),4,0,0x3fffff,0x3fffff);
        OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar13);
        plVar13 = local_50;
        plVar17 = plVar17 + 1;
        uVar20 = uVar20 - 1;
        plVar21 = local_88;
      } while (uVar20 != 0);
      do {
        lVar9 = *plVar21;
        if ((lVar9 != 0) &&
           ((iVar5 == DAT_140b278b0 ||
            ((*(longlong *)(lVar9 + 0x1b0) != 0 &&
             (*(char *)(*(longlong *)(lVar9 + 0x1b0) + 0x32) != '\0')))))) {
          lVar6 = FUN_140769c58(0x50);
          plVar10 = plVar11;
          if (lVar6 != 0) {
            plVar10 = (longlong *)FUN_1405b1170(lVar6);
          }
          FUN_1405b06b0(plVar10,*(undefined4 *)(lVar9 + 0x50),iVar5);
          OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar10);
        }
        FUN_1403a87c0(lVar9,iVar5);
        if (*(longlong *)(lVar9 + 0x1b0) != 0) {
          *(undefined1 *)(*(longlong *)(lVar9 + 0x1b0) + 0x30) = 1;
        }
        lVar6 = FUN_140769c58(0x68);
        plVar10 = plVar11;
        if (lVar6 != 0) {
          plVar10 = (longlong *)FUN_1405af020(lVar6);
        }
        FUN_1405afb60(plVar10,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x50),
                      (int)plVar13[10],*(undefined4 *)(lVar9 + 0x50),0,0,0x3fffff,0x3fffff);
        OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar10);
        local_80 = local_80 - 1;
        plVar21 = plVar21 + 1;
      } while (local_80 != 0);
    }
    else {
      uVar8 = FUN_140397120(*(undefined8 *)(param_1 + 0x2150));
      FUN_1403cfe70(param_1,&local_70,lVar6,uVar8,0);
      local_88 = local_68;
      uVar16 = (uint)local_60;
      iVar18 = local_60._4_4_;
      if (0 < (int)(uint)local_60) goto LAB_1403d1047;
    }
    FUN_14032edd0(DAT_140b15418 + 0xa0,"drag_land_to_space");
    if (local_88 == (longlong *)0x0) {
      return;
    }
    if (iVar18 < 0) {
      pvVar12 = GetProcessHeap();
      HeapFree(pvVar12,0,local_88);
      return;
    }
    goto LAB_1403d1213;
  }
  iVar5 = FUN_1403cb630(param_1,lVar6,iVar5,iVar18,(uint)uVar23 << 8);
  if (plVar10 == plVar17) {
    return;
  }
  local_88 = (longlong *)0x0;
  if (*(char *)(param_1 + 0x21a9) == '\0') {
    local_88 = (longlong *)thunk_FUN_140769c58(0x50);
    uVar16 = 1;
    *local_88 = lVar6;
LAB_1403d0d89:
    plVar10 = local_48;
    local_80 = (ulonglong)uVar16;
    uVar20 = local_80;
    plVar17 = local_88;
    do {
      lVar9 = *plVar17;
      lVar6 = FUN_140769c58(0x68);
      plVar13 = plVar11;
      if (lVar6 != 0) {
        plVar13 = (longlong *)FUN_1405af020(lVar6);
      }
      FUN_1405afb60(plVar13,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x50),(int)plVar10[10]
                    ,*(undefined4 *)(lVar9 + 0x50),2,0,0x3fffff,0x3fffff);
      OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar13);
      plVar13 = local_50;
      plVar17 = plVar17 + 1;
      uVar20 = uVar20 - 1;
      plVar21 = local_88;
    } while (uVar20 != 0);
    do {
      lVar9 = *plVar21;
      if ((lVar9 != 0) &&
         ((iVar5 == DAT_140b278b0 ||
          ((*(longlong *)(lVar9 + 0x1b0) != 0 &&
           (*(char *)(*(longlong *)(lVar9 + 0x1b0) + 0x32) != '\0')))))) {
        lVar6 = FUN_140769c58(0x50);
        plVar10 = plVar11;
        if (lVar6 != 0) {
          plVar10 = (longlong *)FUN_1405b1170(lVar6);
        }
        FUN_1405b06b0(plVar10,*(undefined4 *)(lVar9 + 0x50),iVar5);
        OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar10);
      }
      FUN_1403a87c0(lVar9,iVar5);
      if (*(longlong *)(lVar9 + 0x1b0) != 0) {
        *(undefined1 *)(*(longlong *)(lVar9 + 0x1b0) + 0x30) = 1;
      }
      lVar6 = FUN_140769c58(0x68);
      plVar10 = plVar11;
      if (lVar6 != 0) {
        plVar10 = (longlong *)FUN_1405af020(lVar6);
      }
      FUN_1405afb60(plVar10,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x50),(int)plVar13[10]
                    ,*(undefined4 *)(lVar9 + 0x50),0,0,0x3fffff,0x3fffff);
      OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,plVar10);
      local_80 = local_80 - 1;
      plVar21 = plVar21 + 1;
    } while (local_80 != 0);
  }
  else {
    local_68 = (longlong *)0x0;
    local_70 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    local_60 = 0;
    FUN_1403965f0(plVar10,&local_70,1);
    lVar9 = *(longlong *)(lVar6 + 0x2b0);
    local_40 = FUN_1402f6270(param_1,lVar6);
    local_58 = 0;
    uVar16 = 0;
    if (0 < (int)(uint)local_60) {
      local_res8 = 0;
      plVar10 = plVar11;
      plVar17 = plVar11;
      plVar13 = plVar11;
      plVar21 = plVar11;
      do {
        uVar16 = (uint)plVar17;
        lVar6 = *(longlong *)((longlong)local_68 + local_res8);
        local_88 = plVar13;
        uVar15 = (uint)plVar10;
        if ((*(longlong *)(lVar6 + 0x2b0) == lVar9) &&
           (lVar7 = FUN_1402f6270(param_1,lVar6), lVar7 == local_40)) {
          uVar15 = (uint)plVar21;
          uVar14 = (int)(uVar15 * 2) >> 1;
          uVar22 = uVar16;
          if ((int)uVar14 <= (int)uVar16) {
            iVar18 = (int)uVar14 / 10;
            if ((int)uVar14 / 10 < 10) {
              iVar18 = 10;
            }
            uVar22 = iVar18 + uVar14;
            if (uVar22 == 0) {
              uVar22 = uVar16;
              if (plVar13 != (longlong *)0x0) {
                local_88 = plVar11;
                if ((int)uVar15 < 0) {
                  pvVar12 = GetProcessHeap();
                  HeapFree(pvVar12,0,plVar13);
                  plVar21 = (longlong *)0x0;
                  uVar22 = 0;
                }
                else {
                  thunk_FUN_1407864b8(plVar13);
                  plVar21 = (longlong *)0x0;
                  uVar22 = 0;
                }
              }
            }
            else {
              uVar20 = (ulonglong)(int)uVar22;
              if (plVar13 == (longlong *)0x0) {
                if ((int)uVar15 < 0) {
                  pvVar12 = GetProcessHeap();
                  local_88 = HeapAlloc(pvVar12,0,uVar20 * 8);
                }
                else {
                  auVar1._8_8_ = 0;
                  auVar1._0_8_ = uVar20;
                  uVar8 = SUB168(ZEXT816(8) * auVar1,0);
                  if (SUB168(ZEXT816(8) * auVar1,8) != 0) {
                    uVar8 = 0xffffffffffffffff;
                  }
                  local_88 = (longlong *)thunk_FUN_140769c58(uVar8);
                }
                plVar21 = (longlong *)(ulonglong)(uVar15 ^ (uVar15 ^ uVar22) & 0x7fffffff);
                uVar22 = 0;
              }
              else {
                if ((int)uVar15 < 0) {
                  pvVar12 = GetProcessHeap();
                  local_88 = HeapReAlloc(pvVar12,0,plVar13,uVar20 * 8);
                }
                else {
                  auVar2._8_8_ = 0;
                  auVar2._0_8_ = uVar20;
                  uVar8 = SUB168(ZEXT816(8) * auVar2,0);
                  if (SUB168(ZEXT816(8) * auVar2,8) != 0) {
                    uVar8 = 0xffffffffffffffff;
                  }
                  local_88 = (longlong *)thunk_FUN_140769c58(uVar8);
                  if ((int)uVar22 < (int)uVar14) {
                    uVar14 = uVar22;
                  }
                  lVar7 = (longlong)(int)uVar14;
                  if (0 < (int)uVar14) {
                    plVar10 = local_88;
                    do {
                      *plVar10 = *(longlong *)
                                  (((longlong)plVar13 - (longlong)local_88) + (longlong)plVar10);
                      plVar10 = plVar10 + 1;
                      lVar7 = lVar7 + -1;
                    } while (lVar7 != 0);
                  }
                  thunk_FUN_1407864b8(plVar13);
                }
                plVar21 = (longlong *)(ulonglong)(uVar15 ^ (uVar15 ^ uVar22) & 0x7fffffff);
                if ((int)uVar16 < (int)uVar22) {
                  uVar22 = uVar16;
                }
              }
            }
          }
          plVar17 = (longlong *)(ulonglong)(uVar22 + 1);
          local_88[(int)uVar22] = lVar6;
          uVar15 = local_58;
        }
        uVar16 = (uint)plVar17;
        local_58 = uVar15 + 1;
        plVar10 = (longlong *)(ulonglong)local_58;
        local_res8 = local_res8 + 8;
        plVar13 = local_88;
      } while ((int)local_58 < (int)(uint)local_60);
    }
    local_70 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    if (local_68 != (longlong *)0x0) {
      if ((local_60 & 0x8000000000000000) == 0) {
        thunk_FUN_1407864b8(local_68);
      }
      else {
        pvVar12 = GetProcessHeap();
        HeapFree(pvVar12,0,local_68);
      }
    }
    if (0 < (int)uVar16) goto LAB_1403d0d89;
  }
  if (local_88 == (longlong *)0x0) {
    return;
  }
LAB_1403d1213:
  thunk_FUN_1407864b8(local_88);
  return;
}


