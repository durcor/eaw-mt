/* FighterLocomotorBehaviorClass::FUN_1405cd8e0  RVA 0x5cd8e0  size 1840  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405cd8e0(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  float *pfVar8;
  longlong lVar9;
  undefined4 uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  uint uVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined4 local_res8 [2];
  undefined4 local_res10 [2];
  undefined4 local_res18 [2];
  float local_res20;
  undefined8 in_stack_fffffffffffffeb8;
  undefined4 uVar20;
  undefined8 in_stack_fffffffffffffec0;
  undefined4 uVar21;
  float local_128;
  float local_124;
  float local_120;
  float local_118;
  float local_114;
  float local_110;
  undefined1 local_10c [12];
  undefined1 local_100 [208];
  
  uVar20 = (undefined4)((ulonglong)in_stack_fffffffffffffeb8 >> 0x20);
  uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffec0 >> 0x20);
  lVar6 = *(longlong *)(param_1 + 0x28);
  lVar5 = 0;
  plVar1 = *(longlong **)(lVar6 + 0x2b0);
  lVar2 = *(longlong *)(lVar6 + 0xa8);
  if ((plVar1 != (longlong *)0x0) && ((char)plVar1[0x69] != -1)) {
    lVar5 = (**(code **)(*plVar1 + 0x10))(plVar1,0x16);
    lVar6 = *(longlong *)(param_1 + 0x28);
  }
  lVar9 = *(longlong *)(lVar6 + 0xf0);
  if (lVar9 != 0) {
    lVar6 = FUN_1403973b0();
    local_res8[0] = *(undefined4 *)(lVar6 + 0xde0);
    uVar7 = FUN_1404ad4c0(local_100,*(undefined8 *)(param_1 + 0x28),0xffffffffffffffff);
    uVar20 = 1;
    FUN_1403fe810(*(undefined8 *)(lVar9 + 0x138),local_res8,&DAT_1408a618c,uVar7,&PTR_1407ff8a8,
                  CONCAT44(uVar21,DAT_140891814));
    FUN_1404ad530(local_100);
    lVar6 = *(longlong *)(param_1 + 0x28);
  }
  uVar10 = FUN_140372560(*(undefined8 *)(lVar6 + 0x298));
  uVar21 = *(undefined4 *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298) + 900);
  local_res10[0] = uVar10;
  local_res18[0] = uVar21;
  local_res8[0] = FUN_140372440();
  if (plVar1 == (longlong *)0x0) {
    lVar6 = *(longlong *)(param_1 + 0x28);
  }
  else {
    lVar6 = FUN_1404ec820(plVar1[0x22]);
  }
  local_128 = *(float *)(lVar2 + 8);
  local_124 = *(float *)(lVar2 + 0xc);
  local_120 = *(float *)(lVar2 + 0x10);
  fVar11 = (float)FUN_1405ca430(param_1);
  fVar19 = DAT_140b16d54;
  fVar14 = DAT_1407ffaf8;
  if (lVar5 == 0) {
    iVar4 = (**(code **)(*DAT_140b15418 + 0xe0))();
    if (iVar4 == 1) {
      fVar19 = fVar11;
    }
    cVar3 = FUN_14059e5b0(*(longlong *)(param_1 + 0x28) + 0x78,0);
    if (cVar3 == '\0') {
      fVar19 = 0.0;
    }
    FUN_1405c95a0(param_1,0,local_res10);
    uVar15 = FUN_14020b6d0(*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x94));
    FUN_1405c8fb0(param_1,uVar15 ^ DAT_140800860,local_res8);
    FUN_1405c9360(param_1,fVar19,local_res18);
  }
  else {
    fVar17 = 0.0;
    lVar9 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2b0) + 0x110);
    *(undefined4 *)(lVar9 + 0xa4) = *(undefined4 *)(lVar2 + 8);
    *(undefined4 *)(lVar9 + 0xa8) = *(undefined4 *)(lVar2 + 0xc);
    *(undefined4 *)(lVar9 + 0xac) = *(undefined4 *)(lVar2 + 0x10);
    *(undefined1 *)
     (*(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2b0) + 0x110) + 0xbc) = 0;
    lVar9 = *(longlong *)(lVar6 + 0xa8);
    local_128 = *(float *)(lVar9 + 8);
    local_124 = *(float *)(lVar9 + 0xc);
    local_120 = *(float *)(lVar9 + 0x10);
    fVar19 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x78) - local_128;
    fVar18 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x7c) - local_124;
    if ((fVar19 == 0.0) && (fVar18 == 0.0)) {
      fVar18 = 0.0;
      fVar19 = fVar14;
    }
    fVar12 = (float)FUN_140776d48(fVar19 * fVar19 + fVar18 * fVar18 + 0.0);
    if (0.0 < fVar12) {
      fVar17 = fVar14 / fVar12;
      fVar19 = fVar19 * fVar17;
      fVar18 = fVar18 * fVar17;
      fVar17 = fVar17 * 0.0;
    }
    fVar13 = *(float *)(lVar6 + 0x78) - local_128;
    fVar16 = *(float *)(lVar6 + 0x7c) - local_124;
    fVar16 = (float)FUN_140776d48(fVar13 * fVar13 + fVar16 * fVar16);
    fVar13 = fVar14;
    if (fVar14 <= fVar16) {
      fVar13 = fVar16;
    }
    if ((fVar13 < DAT_1408007d0 * DAT_140804fb8) &&
       (fVar11 = (fVar12 / fVar13) * DAT_1408007c0, *(longlong *)(param_1 + 0x28) == lVar6)) {
      fVar12 = DAT_140805898 / fVar12;
      fVar13 = fVar12;
      if (0x1d000000 < ((uint)fVar12 & 0x7f800000)) {
        local_res20 = fVar12;
        fVar13 = (float)FUN_140776650(fVar12);
      }
      local_res20 = fVar12;
      if (0x1d000000 < ((uint)fVar12 & 0x7f800000)) {
        fVar14 = (float)FUN_140776150(fVar12);
      }
      local_124 = local_124 +
                  ((fVar14 - fVar13 * 0.0) * fVar18 + (fVar14 * 0.0 + fVar13) * fVar19 +
                  fVar17 * 0.0) * DAT_140804fb8;
      local_128 = local_128 +
                  ((fVar14 * 0.0 - fVar13) * fVar18 + (fVar13 * 0.0 + fVar14) * fVar19 +
                  fVar17 * 0.0) * DAT_140804fb8;
      local_120 = local_120 +
                  ((fVar14 * 0.0 - fVar13 * 0.0) * fVar18 + (fVar14 * 0.0 + fVar13 * 0.0) * fVar19 +
                  fVar17) * DAT_140804fb8;
    }
    lVar9 = *(longlong *)(param_1 + 0x28);
    if (lVar9 != lVar6) {
      pfVar8 = (float *)FUN_140404c60(lVar5,lVar6);
      fVar14 = *pfVar8;
      fVar19 = pfVar8[1];
      fVar17 = pfVar8[2];
      pfVar8 = (float *)FUN_140404c60(lVar5,*(undefined8 *)(param_1 + 0x28));
      lVar5 = *(longlong *)(lVar6 + 0xa8);
      lVar9 = *(longlong *)(param_1 + 0x28);
      local_120 = (pfVar8[2] - fVar17) * DAT_140803514 +
                  *(float *)(lVar5 + 0x1c) * DAT_14085d684 + *(float *)(lVar6 + 0x80);
      local_124 = (pfVar8[1] - fVar19) * DAT_140803514 +
                  *(float *)(lVar5 + 0x18) * DAT_14085d684 + *(float *)(lVar6 + 0x7c);
      local_128 = *(float *)(lVar5 + 0x14) * DAT_14085d684 + *(float *)(lVar6 + 0x78) +
                  (*pfVar8 - fVar14) * DAT_140803514;
    }
    if (DAT_140b395e5 != '\0') {
      local_120 = *(float *)(lVar9 + 0x80);
    }
    cVar3 = FUN_1405c8b70(param_1,&local_128,uVar10,uVar21,CONCAT44(uVar20,local_res8[0]));
    if (cVar3 == '\0') {
      FUN_1405caaf0(param_1,&local_128,fVar11,local_res10,local_res18,local_res8,0);
    }
  }
  lVar6 = *(longlong *)(param_1 + 0x28);
  local_110 = *(float *)(lVar6 + 0x80) + *(float *)(lVar2 + 0x1c);
  local_114 = *(float *)(lVar6 + 0x7c) + *(float *)(lVar2 + 0x18);
  local_118 = *(float *)(lVar2 + 0x14) + *(float *)(lVar6 + 0x78);
  uVar7 = FUN_1405c9090(param_1,local_10c,&local_118);
  FUN_1403a8f90(*(undefined8 *)(param_1 + 0x28),uVar7,0);
  lVar6 = lVar2;
  if (plVar1 != (longlong *)0x0) {
    lVar6 = plVar1[0x15];
  }
  if ((((lVar6 != 0) && (*(longlong *)(lVar6 + 0x1f0) != 0)) &&
      (lVar6 = FUN_14041a2e0(*(longlong *)(lVar6 + 0x1f0),0,0xffffffff), lVar6 != 0)) &&
     ((*(byte *)(lVar6 + 0x3a0) & 2) == 0)) {
    FUN_140559250(lVar2,0x1e);
  }
  return;
}


