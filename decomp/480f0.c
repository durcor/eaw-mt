/* FUN_1400480f0  RVA 0x480f0  size 267  params 0  ret undefined */


void FUN_1400480f0(float *param_1,float param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  
  fVar3 = param_2;
  if (0x1d000000 < ((uint)param_2 & 0x7f800000)) {
    fVar3 = (float)FUN_140776650(param_2);
  }
  fVar4 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)param_2 & 0x7f800000)) {
    fVar4 = (float)FUN_140776150(param_2);
  }
  fVar1 = param_1[1];
  param_1[1] = fVar1 * fVar4 - *param_1 * fVar3;
  fVar2 = param_1[5];
  *param_1 = *param_1 * fVar4 + fVar1 * fVar3;
  param_1[5] = fVar2 * fVar4 - param_1[4] * fVar3;
  fVar1 = param_1[9];
  param_1[4] = param_1[4] * fVar4 + fVar2 * fVar3;
  param_1[9] = fVar1 * fVar4 - param_1[8] * fVar3;
  param_1[8] = param_1[8] * fVar4 + fVar1 * fVar3;
  return;
}


