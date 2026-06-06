/* FUN_1402cf8d0  RVA 0x2cf8d0  size 269  params 0  ret undefined */


void FUN_1402cf8d0(longlong param_1,float param_2)

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
  fVar1 = *(float *)(param_1 + 8);
  *(float *)(param_1 + 8) = fVar1 * fVar4 - *(float *)(param_1 + 4) * fVar3;
  fVar2 = *(float *)(param_1 + 0x18);
  *(float *)(param_1 + 4) = *(float *)(param_1 + 4) * fVar4 + fVar1 * fVar3;
  *(float *)(param_1 + 0x18) = fVar2 * fVar4 - *(float *)(param_1 + 0x14) * fVar3;
  fVar1 = *(float *)(param_1 + 0x28);
  *(float *)(param_1 + 0x14) = *(float *)(param_1 + 0x14) * fVar4 + fVar2 * fVar3;
  *(float *)(param_1 + 0x28) = fVar1 * fVar4 - *(float *)(param_1 + 0x24) * fVar3;
  *(float *)(param_1 + 0x24) = *(float *)(param_1 + 0x24) * fVar4 + fVar1 * fVar3;
  return;
}


