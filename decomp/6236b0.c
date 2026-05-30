/* StarshipLocomotorBehaviorClass::vfunc_6  RVA 0x6236b0  size 2256  params 0  ret undefined */


undefined8 StarshipLocomotorBehaviorClass::vfunc_6(longlong *param_1,longlong param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong lVar5;
  bool bVar6;
  int iVar7;
  longlong *plVar8;
  undefined4 *puVar9;
  longlong lVar10;
  undefined8 uVar11;
  float *pfVar12;
  float fVar13;
  float fVar14;
  uint uVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float local_res10 [2];
  float local_a8;
  float local_a4;
  float local_a0;
  float local_98;
  float fStack_94;
  float local_90;
  float local_88;
  float fStack_84;
  float local_80;
  float local_7c;
  float local_78;
  float local_74;
  
  LocomotorCommonClass::vfunc_6();
  lVar4 = *(longlong *)(param_2 + 0xa8);
  bVar6 = true;
  if (lVar4 == 0) {
    return 0x650000;
  }
  *(undefined4 *)(lVar4 + 0x58) = 0;
  if (*(longlong *)(param_2 + 0x298) == 0) {
    return 0x650000;
  }
  local_a8 = *(float *)(param_2 + 0x78);
  local_a4 = *(float *)(param_2 + 0x7c);
  local_a0 = *(float *)(param_2 + 0x80);
  *(undefined4 *)(lVar4 + 0x60) = 0;
  fVar13 = DAT_1408007f4;
  fVar17 = DAT_1408007dc;
  fVar16 = DAT_1408007d4;
  switch(*(undefined4 *)(lVar4 + 0x48)) {
  case 0:
    pfVar12 = (float *)FUN_140623340(param_1);
    if (((*pfVar12 != local_a8) || (pfVar12[1] != local_a4)) || (pfVar12[2] != local_a0)) {
      *(undefined4 *)(lVar4 + 0x54) = 0x3d4ccccd;
      FUN_140559250(lVar4,1);
    }
    break;
  case 1:
    FUN_140622b80(param_1,param_2,lVar4);
    break;
  case 2:
  case 3:
    FUN_140622e90(param_1,param_2,lVar4);
    break;
  case 4:
    if (0 < *(int *)(lVar4 + 0x5c)) {
      *(int *)(lVar4 + 0x5c) = *(int *)(lVar4 + 0x5c) + -1;
      return 0x650000;
    }
    (**(code **)(*param_1 + 0x298))(param_1);
    goto LAB_140623bfd;
  case 5:
    fVar14 = *(float *)(param_1[5] + 0x88);
    local_88 = DAT_1407ffaf8;
    fStack_94 = 0.0;
    local_90 = 0.0;
    fVar18 = *(float *)(param_1[5] + 0x8c);
    fStack_84 = 0.0;
    local_80 = 0.0;
    local_98 = DAT_14080080c;
    FUN_1404aaa40(&local_98,((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * fVar14);
    FUN_1402bab90(&local_98,((fVar17 * fVar16) / fVar13) * fVar18);
    FUN_1404aaa40(&local_88,((fVar17 * fVar16) / fVar13) * fVar14);
    FUN_1402bab90(&local_88,((fVar17 * fVar16) / fVar13) * fVar18);
    fVar16 = fStack_94 * DAT_1408ae5d8;
    fVar17 = local_90 * DAT_1408ae5d8;
    fVar13 = local_98 * DAT_1408ae5d8;
    fVar19 = local_88 * DAT_1408906c8;
    fVar18 = fStack_84 * DAT_1408906c8;
    fVar14 = local_80 * DAT_1408906c8;
    *(float *)(lVar4 + 0x2c) = local_a8;
    local_a8 = local_a8 + fVar13;
    *(float *)(lVar4 + 0x30) = local_a4;
    local_a4 = local_a4 + fVar16;
    *(float *)(lVar4 + 0x34) = local_a0;
    local_a0 = local_a0 + fVar17;
    FUN_1403a8f90(param_1[5],&local_a8,0);
    FUN_140559250(lVar4,4);
    *(float *)(lVar4 + 0x14) = fVar19;
    *(float *)(lVar4 + 0x18) = fVar18;
    *(float *)(lVar4 + 0x1c) = fVar14;
    iVar7 = FUN_1401ffb40(&DAT_140a13e24,0,0x1e);
    *(int *)(lVar4 + 0x5c) = iVar7 + 0x78;
    return 0x650000;
  case 6:
    *(undefined4 *)(lVar4 + 0x54) = 0;
    if (((*(float *)(lVar4 + 0x14) == 0.0) && (*(float *)(lVar4 + 0x18) == 0.0)) &&
       (*(float *)(lVar4 + 0x1c) == 0.0)) {
      FUN_140559250(lVar4,7);
    }
    break;
  case 7:
    *(undefined4 *)(lVar4 + 0x54) = 0;
    break;
  case 8:
    iVar7 = *(int *)(lVar4 + 0x5c);
    if (iVar7 == -1) {
      *(undefined4 *)(lVar4 + 0x5c) = 0x28;
      *(float *)(lVar4 + 0x88) = local_a8;
      *(float *)(lVar4 + 0x8c) = local_a4;
      *(float *)(lVar4 + 0x90) = local_a0;
      *(float *)(lVar4 + 0xa0) = local_a8;
      *(float *)(lVar4 + 0xa4) = local_a4;
      *(float *)(lVar4 + 0xa8) = local_a0;
    }
    else {
      if ((0 < iVar7) && (*(int *)(lVar4 + 0x5c) = iVar7 + -1, iVar7 + -1 == 0)) {
        *(undefined8 *)(lVar4 + 0xa0) = 0;
        *(undefined4 *)(lVar4 + 0xa8) = 0;
      }
      local_a8 = *(float *)(lVar4 + 0x88);
      local_a4 = *(float *)(lVar4 + 0x8c);
      local_a0 = *(float *)(lVar4 + 0x90);
    }
    lVar10 = *(longlong *)(param_1[5] + 0x2b0);
    lVar5 = *(longlong *)(param_1[5] + 0xa8);
    if ((lVar10 == 0) || (*(longlong *)(lVar10 + 0x110) == 0)) {
      FUN_140559250(lVar5,1);
    }
    else {
      plVar8 = (longlong *)FUN_1404ec820();
      if ((plVar8 != (longlong *)0x0) && (lVar5 != 0)) {
        if ((*(byte *)(plVar8 + 0x74) & 2) == 0) {
          if (*(char *)((longlong)plVar8 + 0x347) != -1) {
            uVar11 = (**(code **)(*plVar8 + 0x10))(plVar8,0x15);
            local_res10[0] = (float)((uint)local_res10[0] & 0xffffff00);
            puVar9 = (undefined4 *)FUN_1405c3d10(uVar11,param_1[5],&local_88,local_res10);
            uVar1 = *puVar9;
            uVar2 = puVar9[1];
            uVar3 = puVar9[2];
            if (local_res10[0]._0_1_ != '\0') {
              *(undefined4 *)(lVar5 + 0x5c) = 0x28;
            }
            *(undefined4 *)(lVar5 + 0xa0) = uVar1;
            *(undefined4 *)(lVar5 + 0xa4) = uVar2;
            *(undefined4 *)(lVar5 + 0xa8) = uVar3;
            local_90 = local_80;
            FUN_1403a8710(param_1[5],&local_98);
          }
        }
        else {
          lVar10 = param_1[5];
          fVar16 = *(float *)(lVar10 + 0x7c);
          fVar17 = *(float *)(lVar10 + 0x78);
          *(float *)(lVar5 + 0xa8) = *(float *)(lVar5 + 0x1c) + *(float *)(lVar10 + 0x80);
          *(float *)(lVar5 + 0xa4) = *(float *)(lVar5 + 0x18) + fVar16;
          *(float *)(lVar5 + 0xa0) = *(float *)(lVar5 + 0x14) + fVar17;
        }
      }
    }
    if (*(int *)(lVar4 + 0x5c) == 0) {
      lVar10 = param_1[5];
      pfVar12 = (float *)(lVar4 + 0xa0);
      if (((*pfVar12 != *(float *)(lVar10 + 0x78)) ||
          (*(float *)(lVar4 + 0xa4) != *(float *)(lVar10 + 0x7c))) ||
         (*(float *)(lVar4 + 0xa8) != *(float *)(lVar10 + 0x80))) {
        fVar16 = *(float *)(lVar10 + 0x7c);
        fVar17 = *(float *)(lVar10 + 0x78);
        *(float *)(lVar4 + 0x1c) = *(float *)(lVar4 + 0xa8) - *(float *)(lVar10 + 0x80);
        *(float *)(lVar4 + 0x18) = *(float *)(lVar4 + 0xa4) - fVar16;
        *(float *)(lVar4 + 0x14) = *pfVar12 - fVar17;
        lVar10 = param_1[5];
      }
      FUN_1403a8f90(lVar10,pfVar12,0);
    }
LAB_140623bfd:
    bVar6 = false;
  }
  if ((*(int *)(lVar4 + 0x48) == 8) && (*(int *)(lVar4 + 0x5c) == 0)) {
    return 0x650000;
  }
  if (bVar6) {
    FUN_1406224b0(param_1,param_2,lVar4);
  }
  if (((*(float *)(lVar4 + 0x14) == 0.0) && (*(float *)(lVar4 + 0x18) == 0.0)) &&
     (*(float *)(lVar4 + 0x1c) == 0.0)) {
    return 0x650000;
  }
  local_a8 = local_a8 + *(float *)(lVar4 + 0x14);
  local_a4 = local_a4 + *(float *)(lVar4 + 0x18);
  fVar16 = *(float *)(param_2 + 0x80);
  local_a0 = local_a0 + *(float *)(lVar4 + 0x1c);
  lVar10 = FUN_140623340(param_1);
  fVar17 = *(float *)(lVar10 + 8);
  plVar8 = *(longlong **)(*(longlong *)(param_2 + 0x2b8) + 0x20);
  if (((plVar8 != (longlong *)0x0) && (iVar7 = (**(code **)(*plVar8 + 0xe0))(plVar8), iVar7 == 1))
     && (plVar8[0x67] != 0)) {
    fVar13 = (float)FUN_140135140(plVar8[0x67],local_a8,local_a4);
    fVar17 = fVar17 + fVar13;
  }
  if (fVar17 <= fVar16 + DAT_1408007b4) {
    if (fVar16 + DAT_1408007b4 <= fVar17) goto LAB_140623d22;
    uVar15 = FUN_140372440(*(undefined8 *)(param_2 + 0x298));
    fVar13 = fVar17 - fVar16;
    if (fVar17 - fVar16 <= (float)(uVar15 ^ DAT_140800860)) {
      fVar13 = (float)(uVar15 ^ DAT_140800860);
    }
  }
  else {
    fVar14 = (float)FUN_140372440(*(undefined8 *)(param_2 + 0x298));
    fVar13 = fVar17 - fVar16;
    if (fVar14 <= fVar17 - fVar16) {
      fVar13 = fVar14;
    }
  }
  *(float *)(lVar4 + 0x60) = fVar13;
LAB_140623d22:
  fVar16 = DAT_1407ffaf8;
  local_a0 = local_a0 + *(float *)(lVar4 + 0x60);
  if (*(int *)(lVar4 + 0x48) == 8) {
    *(float *)(lVar4 + 0x88) = local_a8;
    fVar17 = (float)*(int *)(lVar4 + 0x5c) / DAT_14085d684;
    *(float *)(lVar4 + 0x8c) = local_a4;
    fVar16 = fVar16 - fVar17;
    *(float *)(lVar4 + 0x90) = local_a0;
    local_80 = (*(float *)(lVar4 + 0xa8) - local_a0) * fVar16 + local_a0;
    fStack_84 = (*(float *)(lVar4 + 0xa4) - local_a4) * fVar16 + local_a4;
    local_88 = fVar16 * (*(float *)(lVar4 + 0xa0) - local_a8) + local_a8;
    FUN_1403a8f90(param_2,&local_88,0);
  }
  else {
    FUN_1403a8f90(param_2,&local_a8,0);
    if (*(int *)(lVar4 + 0x50) == 10) {
      uVar11 = FUN_140058570();
      FUN_140220ed0(uVar11,param_2 + 0x38,9);
      *(undefined4 *)(lVar4 + 0x50) = 0xb;
    }
    else if (*(int *)(lVar4 + 0x50) != 0xb) {
      return 0x650000;
    }
    lVar10 = param_1[5];
    fVar16 = DAT_140805898;
    if (*(longlong *)(lVar10 + 0x2a0) != 0) {
      FUN_140264ab0(*(longlong *)(lVar10 + 0x2a0),&local_88);
      if (local_88 + local_7c <= fStack_84 + local_78) {
        fVar16 = fStack_84 + local_78;
      }
      else {
        fVar16 = local_88 + local_7c;
      }
      if (fVar16 <= local_80 + local_74) {
        fVar16 = local_80 + local_74;
      }
      else if (local_88 + local_7c <= fStack_84 + local_78) {
        fVar16 = fStack_84 + local_78;
      }
      else {
        fVar16 = local_88 + local_7c;
      }
      lVar10 = param_1[5];
    }
    fVar17 = *(float *)(lVar10 + 0x78);
    fVar13 = *(float *)(lVar10 + 0x7c);
    fVar14 = *(float *)(lVar10 + 0x80);
    pfVar12 = (float *)FUN_140623340(param_1);
    fVar17 = *pfVar12 - fVar17;
    fVar13 = pfVar12[1] - fVar13;
    fVar14 = *(float *)(*(longlong *)(param_1[5] + 0x298) + 0xdfc) - fVar14;
    local_res10[0] = (float)FUN_140776d48(fVar17 * fVar17 + fVar13 * fVar13 + fVar14 * fVar14);
    local_res10[0] = ABS(local_res10[0]);
    if (local_res10[0] < fVar16) {
      uVar11 = FUN_140058570();
      FUN_140220ed0(uVar11,param_2 + 0x38,10);
      *(undefined4 *)(lVar4 + 0x50) = 9;
    }
  }
  return 0x650000;
}


