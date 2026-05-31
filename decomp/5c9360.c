/* FighterLocomotorBehaviorClass::FUN_1405c9360  RVA 0x5c9360  size 565  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FighterLocomotorBehaviorClass::FUN_1405c9360(longlong param_1,float param_2,float *param_3)

{
  longlong lVar1;
  undefined4 uVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  double dVar8;
  float fVar9;
  float fVar10;
  
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
  fVar4 = (float)FUN_140776d48(*(float *)(lVar1 + 0x14) * *(float *)(lVar1 + 0x14) +
                               *(float *)(lVar1 + 0x18) * *(float *)(lVar1 + 0x18) +
                               *(float *)(lVar1 + 0x1c) * *(float *)(lVar1 + 0x1c));
  fVar7 = *param_3;
  param_2 = param_2 - fVar4;
  if (fVar7 <= ABS(param_2)) {
    *param_3 = 0.0;
    if (0.0 <= param_2) {
      fVar4 = fVar4 + fVar7;
    }
    else {
      fVar4 = fVar4 - fVar7;
    }
  }
  else {
    fVar4 = fVar4 + param_2;
    dVar8 = (double)FUN_1407760bc((double)param_2);
    *param_3 = *param_3 - (float)dVar8;
  }
  uVar2 = _UNK_140800864;
  fVar10 = DAT_1408007dc;
  fVar6 = DAT_1408007d4;
  fVar7 = DAT_1407ffaf8;
  lVar3 = *(longlong *)(param_1 + 0x28);
  fVar9 = (float)(*(uint *)(lVar3 + 0x88) ^ DAT_140800860) *
          ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4);
  fVar5 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)fVar9 & 0x7f800000)) {
    fVar5 = (float)FUN_140776150(CONCAT44(_UNK_140800864,fVar9));
    lVar3 = *(longlong *)(param_1 + 0x28);
  }
  fVar10 = ((fVar10 * fVar6) / DAT_1408007f4) * *(float *)(lVar3 + 0x8c);
  fVar6 = fVar10;
  if (0x1d000000 < ((uint)fVar10 & 0x7f800000)) {
    fVar6 = (float)FUN_140776650(fVar10);
  }
  if (0x1d000000 < ((uint)fVar10 & 0x7f800000)) {
    fVar7 = (float)FUN_140776150(fVar10);
  }
  if (0x1d000000 < ((uint)fVar9 & 0x7f800000)) {
    fVar9 = (float)FUN_140776650(CONCAT44(uVar2,fVar9));
  }
  *(float *)(lVar1 + 0x18) = fVar6 * fVar5 * fVar4;
  *(float *)(lVar1 + 0x14) = fVar7 * fVar5 * fVar4;
  *(float *)(lVar1 + 0x1c) = fVar9 * fVar4;
  return;
}


