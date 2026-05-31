/* FighterLocomotorBehaviorClass::FUN_1405cc220  RVA 0x5cc220  size 1858  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405cc220(longlong param_1)

{
  int *piVar1;
  undefined4 uVar2;
  float fVar3;
  float fVar4;
  longlong lVar5;
  char cVar6;
  int iVar7;
  undefined8 uVar8;
  longlong lVar9;
  float *pfVar10;
  longlong lVar11;
  longlong *plVar12;
  longlong *plVar13;
  ulonglong uVar14;
  uint uVar15;
  ulonglong uVar16;
  longlong lVar17;
  longlong lVar18;
  float fVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  float fVar23;
  int local_res8 [2];
  undefined8 local_res10;
  longlong local_res18;
  longlong local_res20;
  undefined **in_stack_fffffffffffffef8;
  undefined8 in_stack_ffffffffffffff00;
  longlong *local_e8;
  undefined8 local_e0;
  float local_d8;
  float local_d0;
  float local_cc;
  float local_c8;
  undefined1 local_c0 [16];
  undefined1 local_b0 [112];
  
  uVar22 = (undefined4)((ulonglong)in_stack_ffffffffffffff00 >> 0x20);
  lVar11 = *(longlong *)(param_1 + 0x28);
  uVar16 = 0;
  local_e0 = 0;
  local_e8 = (longlong *)0x0;
  plVar12 = *(longlong **)(lVar11 + 0x2b0);
  lVar5 = *(longlong *)(lVar11 + 0xa8);
  if ((plVar12 != (longlong *)0x0) && ((char)plVar12[0x69] != -1)) {
    local_e0 = (**(code **)(*plVar12 + 0x10))(plVar12,0x16);
    local_e8 = (longlong *)FUN_1404ec820(plVar12[0x22]);
    lVar11 = *(longlong *)(param_1 + 0x28);
  }
  lVar9 = *(longlong *)(lVar11 + 0xf0);
  if (lVar9 != 0) {
    local_res10 = (ulonglong)local_res10._4_4_ << 0x20;
    uVar8 = FUN_1404ad4c0(local_b0,lVar11,0xffffffffffffffff);
    in_stack_fffffffffffffef8 = &PTR_1407ff8a8;
    FUN_1403fe810(*(undefined8 *)(lVar9 + 0x138),&local_res10,&DAT_1408a618c,uVar8,&PTR_1407ff8a8,
                  CONCAT44(uVar22,DAT_140891814));
    FUN_1404ad530(local_b0);
  }
  local_res8[0] = -1;
  lVar11 = lVar5;
  if (plVar12 != (longlong *)0x0) {
    lVar11 = plVar12[0x15];
  }
  local_res10 = lVar11;
  if (*(longlong *)(lVar11 + 0x1f0) == 0) {
LAB_1405cc431:
    lVar11 = local_res10;
    lVar9 = FUN_1405569b0(local_res10);
    if (lVar9 != 0) {
      uVar8 = FUN_1405569b0(lVar11);
      cVar6 = FUN_1405c5910(uVar8);
      if (cVar6 == '\0') {
        FUN_1405ce010(param_1);
        return;
      }
    }
    lVar11 = local_e0;
    if (local_e0 == 0) {
      lVar11 = *(longlong *)(param_1 + 0x28);
      fVar23 = *(float *)(lVar11 + 0x78);
      fVar19 = *(float *)(lVar11 + 0x7c);
      *(float *)(lVar5 + 0x10) = *(float *)(lVar5 + 0x1c) + *(float *)(lVar11 + 0x80);
      *(float *)(lVar5 + 0xc) = *(float *)(lVar5 + 0x18) + fVar19;
      *(float *)(lVar5 + 8) = fVar23 + *(float *)(lVar5 + 0x14);
      lVar11 = *(longlong *)(param_1 + 0x28);
      *(undefined4 *)(lVar5 + 0x38) = *(undefined4 *)(lVar11 + 0x84);
      *(undefined4 *)(lVar5 + 0x3c) = *(undefined4 *)(lVar11 + 0x88);
      *(undefined4 *)(lVar5 + 0x40) = *(undefined4 *)(lVar11 + 0x8c);
      FUN_1405cb750(param_1);
    }
    else {
      uVar8 = FUN_140404950();
      thunk_FUN_1404f9d50(uVar8,plVar12);
      if (*(longlong **)(param_1 + 0x28) == local_e8) {
        FUN_1404061d0(lVar11,0);
        lVar9 = *(longlong *)(param_1 + 0x28);
        fVar23 = *(float *)(lVar5 + 0x18);
        fVar19 = *(float *)(lVar9 + 0x78);
        fVar3 = *(float *)(lVar9 + 0x7c);
        fVar4 = *(float *)(lVar5 + 0x14);
        uVar22 = *(undefined4 *)(*(longlong *)(lVar9 + 0x298) + 0xdfc);
        iVar7 = FUN_140405300(lVar11);
        if (0 < iVar7) {
          do {
            plVar12 = (longlong *)FUN_1404052d0(lVar11,uVar16);
            lVar9 = plVar12[0x15];
            *(float *)(lVar9 + 8) = fVar19 + fVar4;
            *(float *)(lVar9 + 0xc) = fVar23 + fVar3;
            *(undefined4 *)(lVar9 + 0x10) = uVar22;
            lVar9 = plVar12[0x15];
            *(undefined4 *)(lVar9 + 0x38) = *(undefined4 *)(lVar5 + 0x38);
            *(undefined4 *)(lVar9 + 0x3c) = *(undefined4 *)(lVar5 + 0x3c);
            *(undefined4 *)(lVar9 + 0x40) = *(undefined4 *)(lVar5 + 0x40);
            uVar8 = (**(code **)(*plVar12 + 0x10))(plVar12,1);
            FUN_1405cb750(uVar8);
            uVar15 = (int)uVar16 + 1;
            uVar16 = (ulonglong)uVar15;
            iVar7 = FUN_140405300(lVar11);
          } while ((int)uVar15 < iVar7);
        }
      }
    }
    lVar11 = *(longlong *)(param_1 + 0x28);
    local_c8 = *(float *)(lVar5 + 0x1c) + *(float *)(lVar11 + 0x80);
    local_cc = *(float *)(lVar5 + 0x18) + *(float *)(lVar11 + 0x7c);
    local_d0 = *(float *)(lVar11 + 0x78) + *(float *)(lVar5 + 0x14);
    FUN_1403a8f90(lVar11,&local_d0,0);
    return;
  }
  lVar9 = FUN_14041a2e0(*(longlong *)(lVar11 + 0x1f0),local_res8,0xffffffff);
  uVar22 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
  if ((lVar9 == 0) || ((*(byte *)(lVar9 + 0x3a0) & 2) != 0)) {
    lVar18 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2b0);
    local_res18 = lVar18;
    if ((lVar18 != 0) &&
       ((*(longlong *)(lVar18 + 0xa8) != 0 &&
        (piVar1 = (int *)(*(longlong *)(lVar18 + 0xa8) + 0x1c4), -1 < *piVar1)))) {
      uVar8 = FUN_140404950();
      local_res20 = FUN_1404f9cf0(uVar8,piVar1);
      lVar17 = *(longlong *)(local_res20 + 0x10);
      local_res20 = local_res20 + 8;
      if (lVar17 != local_res20) {
        do {
          uVar14 = *(longlong *)(lVar17 + 0x18) - 0x18;
          if (*(longlong *)(lVar17 + 0x18) == 0) {
            uVar14 = uVar16;
          }
          if (((uVar14 != 0) && ((*(byte *)(uVar14 + 0x3a0) & 2) == 0)) &&
             (cVar6 = FUN_14039a450(lVar18,*(undefined4 *)(uVar14 + 0x58),0), cVar6 != '\0')) {
            in_stack_fffffffffffffef8 =
                 (undefined **)((ulonglong)in_stack_fffffffffffffef8 & 0xffffffff00000000);
            FUN_14038d730(lVar18,uVar14,0,0,in_stack_fffffffffffffef8,0xffffffff,0);
            lVar9 = FUN_140397a40(lVar18,0);
            uVar22 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
            lVar11 = local_res10;
            lVar18 = local_res18;
            if (lVar9 != 0) goto LAB_1405cc46f;
          }
          lVar17 = *(longlong *)(lVar17 + 8);
        } while (lVar17 != local_res20);
      }
    }
    goto LAB_1405cc431;
  }
LAB_1405cc46f:
  if (((*(byte *)(lVar9 + 0x3a0) & 2) != 0) ||
     (cVar6 = FUN_14035f470(DAT_140b15418,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58),
                            lVar9,1), cVar6 != '\0')) goto LAB_1405cc431;
  lVar18 = *(longlong *)(lVar9 + 0xa8);
  if (((lVar18 == 0) ||
      ((*(int *)(lVar11 + 0x1c4) == -1 || (*(int *)(lVar11 + 0x1c4) != *(int *)(lVar18 + 0x1c4)))))
     || (*(int *)(lVar11 + 0x1c8) != *(int *)(lVar18 + 0x1c8))) {
    local_d0 = *(float *)(lVar9 + 0x78);
    local_cc = *(float *)(lVar9 + 0x7c);
    local_c8 = *(float *)(lVar9 + 0x80);
    if (local_res8[0] != -1) {
      pfVar10 = (float *)TeamTargetingBehaviorClass::FUN_140396ce0(lVar9,local_c0);
      local_d0 = *pfVar10;
      local_cc = pfVar10[1];
      local_c8 = pfVar10[2];
    }
    fVar23 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298) + 0x2118);
    fVar19 = (float)FUN_140397780(lVar9);
    fVar23 = fVar23 + fVar19;
    plVar13 = *(longlong **)(param_1 + 0x28);
    if (local_e8 != (longlong *)0x0) {
      plVar13 = local_e8;
    }
    uVar20 = FUN_140372560((*(longlong **)(param_1 + 0x28))[0x53]);
    local_res20 = CONCAT44(local_res20._4_4_,uVar20);
    uVar2 = *(undefined4 *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298) + 900);
    local_res18 = CONCAT44(local_res18._4_4_,uVar2);
    uVar21 = FUN_140372440();
    fVar19 = local_cc - *(float *)((longlong)plVar13 + 0x7c);
    local_res10 = CONCAT44(local_res10._4_4_,uVar21);
    if (fVar23 * fVar23 <
        (local_d0 - *(float *)(plVar13 + 0xf)) * (local_d0 - *(float *)(plVar13 + 0xf)) +
        fVar19 * fVar19) {
      if (plVar12 == (longlong *)0x0) {
        plVar12 = *(longlong **)(param_1 + 0x28);
      }
      uVar8 = FUN_140404950();
      thunk_FUN_1404f9d50(uVar8,plVar12);
      lVar9 = FUN_1405569b0(lVar11);
      if (lVar9 != 0) {
        uVar8 = FUN_1405569b0(lVar11);
        cVar6 = FUN_1405c5910(uVar8);
        if (cVar6 == '\0') {
          FUN_1405ce010(param_1);
          return;
        }
      }
      cVar6 = FUN_1405c8b70(param_1,&local_d0,uVar20,uVar2,CONCAT44(uVar22,uVar21));
      if (cVar6 == '\0') {
        uVar22 = FUN_1405ca390(param_1);
        FUN_1405caaf0(param_1,&local_d0,uVar22,&local_res20,&local_res18,&local_res10,1);
      }
      lVar11 = *(longlong *)(param_1 + 0x28);
      pfVar10 = (float *)&local_e0;
      local_d8 = *(float *)(lVar5 + 0x1c) + *(float *)(lVar11 + 0x80);
      local_e0 = CONCAT44(*(float *)(lVar5 + 0x18) + *(float *)(lVar11 + 0x7c),
                          *(float *)(lVar5 + 0x14) + *(float *)(lVar11 + 0x78));
      goto LAB_1405cc69c;
    }
  }
  lVar18 = local_e0;
  plVar13 = *(longlong **)(param_1 + 0x28);
  if (plVar13 == local_e8) {
    iVar7 = FUN_140405300(local_e0);
    if (0 < iVar7) {
      do {
        lVar11 = FUN_1404052d0(lVar18,uVar16);
        FUN_140554480(*(longlong *)(lVar11 + 0xa8),lVar11,
                      *(undefined4 *)(*(longlong *)(lVar11 + 0xa8) + 0x248));
        uVar15 = (int)uVar16 + 1;
        uVar16 = (ulonglong)uVar15;
        iVar7 = FUN_140405300(lVar18);
      } while ((int)uVar15 < iVar7);
    }
    lVar11 = plVar12[0x15];
    if (lVar11 != 0) {
      uVar22 = *(undefined4 *)(lVar11 + 0x248);
      plVar13 = plVar12;
LAB_1405cc756:
      FUN_140554480(lVar11,plVar13,uVar22);
    }
  }
  else if (plVar12 == (longlong *)0x0) {
    uVar22 = *(undefined4 *)(lVar11 + 0x248);
    goto LAB_1405cc756;
  }
  if (*(char *)(lVar9 + 0x348) == -1) {
    if (plVar12 == (longlong *)0x0) {
      plVar12 = *(longlong **)(param_1 + 0x28);
    }
    uVar8 = FUN_140404950();
    thunk_FUN_1404f9d50(uVar8,plVar12);
    FUN_1405cedf0(param_1,lVar9,local_res8[0]);
  }
  else {
    FUN_1405ce4f0(param_1,lVar9);
  }
  lVar11 = *(longlong *)(param_1 + 0x28);
  pfVar10 = &local_d0;
  local_d0 = *(float *)(lVar5 + 0x14) + *(float *)(lVar11 + 0x78);
  local_c8 = *(float *)(lVar11 + 0x80) + *(float *)(lVar5 + 0x1c);
  local_cc = *(float *)(lVar11 + 0x7c) + *(float *)(lVar5 + 0x18);
LAB_1405cc69c:
  FUN_1403a8f90(lVar11,pfVar10,0);
  return;
}


