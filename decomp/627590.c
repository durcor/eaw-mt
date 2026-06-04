/* SimpleSpaceLocomotorBehaviorClass::FUN_140627590  RVA 0x627590  size 923  params 0  ret undefined */


undefined8
SimpleSpaceLocomotorBehaviorClass::FUN_140627590
          (longlong *param_1,longlong param_2,undefined4 *param_3)

{
  longlong lVar1;
  longlong lVar2;
  float fVar3;
  double dVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float local_b8;
  float local_b4;
  float local_b0;
  float local_a8;
  float local_a4;
  float local_a0;
  float local_98;
  float local_94;
  float local_90;
  
  *param_3 = 0x650000;
  *(undefined4 *)(param_2 + 0x60) = 0;
  lVar1 = param_1[5];
  local_a8 = *(float *)(lVar1 + 0x78);
  local_a4 = *(float *)(lVar1 + 0x7c);
  lVar2 = *param_1;
  local_a0 = *(float *)(lVar1 + 0x80);
  *(float *)(param_2 + 0x34) = local_a0;
  *(float *)(param_2 + 0x2c) = local_a8;
  *(float *)(param_2 + 0x30) = local_a4;
  (**(code **)(lVar2 + 0x2a0))(param_1,&local_a8,lVar1 + 0x84,&local_98,&local_b8);
  FUN_1403a8f90(param_1[5],&local_98,0);
  FUN_1403a8e50(param_1[5],1);
  *(float *)(param_2 + 0x14) = local_b8;
  *(float *)(param_2 + 0x18) = local_b4;
  *(float *)(param_2 + 0x1c) = local_b0;
  local_b8 = local_b8 * DAT_140b27b7c;
  local_b4 = local_b4 * DAT_140b27b7c;
  local_b0 = local_b0 * DAT_140b27b7c;
  if (DAT_140b1574e == '\0') {
    FUN_140264c80(*(undefined8 *)(param_1[5] + 0x2a0),1);
    *(byte *)(param_1[5] + 0x3a2) = *(byte *)(param_1[5] + 0x3a2) | 1;
  }
  lVar1 = *(longlong *)(param_1[5] + 0x2b8);
  fVar8 = local_b8 + *(float *)(lVar1 + 0x628);
  fVar10 = local_b4 + *(float *)(lVar1 + 0x62c);
  fVar5 = local_b0 + *(float *)(lVar1 + 0x630);
  fVar11 = *(float *)(lVar1 + 0x628) - fVar8;
  fVar6 = *(float *)(lVar1 + 0x62c) - fVar10;
  fVar7 = *(float *)(lVar1 + 0x630) - fVar5;
  fVar3 = (float)FUN_140776d48(fVar6 * fVar6 + fVar11 * fVar11 + fVar7 * fVar7);
  fVar9 = 0.0;
  if (0.0 < fVar3) {
    fVar3 = DAT_1407ffaf8 / fVar3;
    fVar11 = fVar11 * fVar3;
    fVar6 = fVar6 * fVar3;
    fVar7 = fVar7 * fVar3;
  }
  fVar3 = fVar8 * fVar11 + fVar6 * fVar10 + fVar7 * fVar5;
  fVar8 = fVar3 * fVar6 - local_94;
  fVar5 = fVar3 * fVar11 - local_98;
  fVar3 = fVar3 * fVar7 - local_90;
  fVar10 = (local_a4 - local_94) * fVar6 + fVar11 * (local_a8 - local_98) +
           (local_a0 - local_90) * fVar7;
  dVar4 = (double)FUN_1407760bc((double)fVar10);
  if ((float)dVar4 < DAT_140815de4) {
    local_94 = 0.0;
    local_90 = 0.0;
  }
  else {
    fVar10 = (fVar8 * fVar6 + fVar11 * fVar5 + fVar3 * fVar7) / fVar10;
    local_90 = local_90 + (local_a0 - local_90) * fVar10;
    local_94 = local_94 + (local_a4 - local_94) * fVar10;
    fVar9 = local_98 + fVar10 * (local_a8 - local_98);
  }
  fVar3 = (float)FUN_140776d48((local_a4 - local_94) * (local_a4 - local_94) +
                               (local_a8 - fVar9) * (local_a8 - fVar9) +
                               (local_a0 - local_90) * (local_a0 - local_90));
  if (fVar3 < DAT_140b27bbc) {
    DAT_140b27bbc = fVar3;
  }
  if (DAT_140b27bc0 < fVar3) {
    DAT_140b27bc0 = fVar3;
  }
  *(undefined4 *)(param_2 + 0x5c) = 0xffffffff;
  FUN_140559250(param_2,4);
  *param_3 = 0x650000;
  return 1;
}


