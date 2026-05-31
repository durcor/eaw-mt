/* StarshipLocomotorBehaviorClass::FUN_1406224b0  RVA 0x6224b0  size 954  params 0  ret undefined */


void StarshipLocomotorBehaviorClass::FUN_1406224b0
               (undefined8 param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float local_a8;
  undefined8 local_a4;
  
  lVar1 = *(longlong *)(param_2 + 0x298);
  if (lVar1 == 0) {
    return;
  }
  fVar2 = (float)FUN_140370f00(lVar1);
  fVar12 = DAT_1408007dc;
  if (*(float *)(param_3 + 0x54) == 0.0) goto LAB_14062281d;
  local_a8 = *(float *)(lVar1 + 900) * *(float *)(param_3 + 0x54);
  local_a4 = 0;
  fVar11 = *(float *)(param_2 + 0x8c);
  FUN_1404aaa40(&local_a8,
                ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * *(float *)(param_2 + 0x88));
  FUN_1402bab90(&local_a8,((fVar12 * DAT_1408007d4) / DAT_1408007f4) * fVar11);
  fVar12 = local_a8;
  fVar11 = (float)local_a4;
  fVar7 = (float)local_a4 + *(float *)(param_3 + 0x18);
  fVar10 = local_a4._4_4_;
  fVar8 = local_a8 + *(float *)(param_3 + 0x14);
  fVar6 = local_a4._4_4_ + *(float *)(param_3 + 0x1c);
  *(float *)(param_3 + 0x18) = fVar7;
  *(float *)(param_3 + 0x14) = fVar8;
  *(float *)(param_3 + 0x1c) = fVar6;
  fVar3 = (float)FUN_140776d48(fVar8 * fVar8 + fVar7 * fVar7 + fVar6 * fVar6);
  if (fVar2 * fVar2 < fVar3 * fVar3) {
    fVar2 = (fVar2 * fVar2) / (fVar3 * fVar3);
    *(float *)(param_3 + 0x18) = fVar7 * fVar2;
    *(float *)(param_3 + 0x1c) = fVar6 * fVar2;
    *(float *)(param_3 + 0x14) = fVar8 * fVar2;
  }
  if (*(float *)(param_3 + 0x58) <= 0.0) goto LAB_14062281d;
  fVar2 = *(float *)(param_3 + 0x14);
  fVar3 = *(float *)(param_3 + 0x18);
  fVar6 = *(float *)(param_3 + 0x1c);
  fVar7 = (float)FUN_140776d48(fVar2 * fVar2 + fVar3 * fVar3 + fVar6 * fVar6);
  if (fVar7 != 0.0) {
    fVar2 = fVar2 / fVar7;
    fVar3 = fVar3 / fVar7;
    fVar6 = fVar6 / fVar7;
  }
  fVar8 = (float)FUN_140776d48(fVar11 * fVar11 + fVar12 * fVar12 + fVar10 * fVar10);
  if (0.0 < fVar8) {
    fVar8 = DAT_1407ffaf8 / fVar8;
    fVar12 = fVar12 * fVar8;
    fVar11 = fVar11 * fVar8;
    fVar10 = fVar10 * fVar8;
  }
  fVar4 = DAT_1408754bc * *(float *)(param_3 + 0x58);
  fVar8 = *(float *)(param_3 + 0x14);
  fVar5 = *(float *)(param_3 + 0x18);
  fVar9 = (fVar2 - fVar12) * fVar4;
  fVar3 = (fVar3 - fVar11) * fVar4;
  fVar4 = (fVar6 - fVar10) * fVar4;
  fVar2 = *(float *)(param_3 + 0x1c);
  fVar10 = fVar7 * fVar10;
  fVar11 = fVar7 * fVar11;
  fVar7 = fVar7 * fVar12;
  if ((fVar8 < 0.0) || (fVar8 <= fVar7)) {
    if ((fVar8 < 0.0) && (fVar8 < fVar7)) {
      if (fVar9 <= fVar8 - fVar7) {
        fVar9 = fVar8 - fVar7;
      }
      goto LAB_140622778;
    }
  }
  else {
    if (fVar8 - fVar7 <= fVar9) {
      fVar9 = fVar8 - fVar7;
    }
LAB_140622778:
    fVar8 = fVar8 - fVar9;
  }
  if ((fVar5 < 0.0) || (fVar5 <= fVar11)) {
    if ((fVar5 < 0.0) && (fVar5 < fVar11)) {
      if (fVar3 <= fVar5 - fVar11) {
        fVar3 = fVar5 - fVar11;
      }
      goto LAB_1406227a9;
    }
  }
  else {
    if (fVar5 - fVar11 <= fVar3) {
      fVar3 = fVar5 - fVar11;
    }
LAB_1406227a9:
    fVar5 = fVar5 - fVar3;
  }
  if ((fVar2 < 0.0) || (fVar2 <= fVar10)) {
    if ((fVar2 < 0.0) && (fVar2 < fVar10)) {
      if (fVar4 <= fVar2 - fVar10) {
        fVar4 = fVar2 - fVar10;
      }
      goto LAB_1406227d5;
    }
  }
  else {
    if (fVar2 - fVar10 <= fVar4) {
      fVar4 = fVar2 - fVar10;
    }
LAB_1406227d5:
    fVar2 = fVar2 - fVar4;
  }
  *(float *)(param_3 + 0x14) = fVar8;
  *(float *)(param_3 + 0x18) = fVar5;
  *(float *)(param_3 + 0x1c) = fVar2;
LAB_14062281d:
  fVar12 = *(float *)(lVar1 + 0x398);
  *(float *)(param_3 + 0x18) = fVar12 * *(float *)(param_3 + 0x18);
  *(float *)(param_3 + 0x1c) = fVar12 * *(float *)(param_3 + 0x1c);
  *(float *)(param_3 + 0x14) = fVar12 * *(float *)(param_3 + 0x14);
  return;
}


