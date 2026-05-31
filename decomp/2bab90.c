/* FUN_1402bab90  RVA 0x2bab90  size 155  params 0  ret undefined */


void FUN_1402bab90(float *param_1,float param_2)

{
  float fVar1;
  float fVar2;
  
  fVar2 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)param_2 & 0x7f800000)) {
    fVar2 = (float)FUN_140776150(param_2);
  }
  if (0x1d000000 < ((uint)param_2 & 0x7f800000)) {
    param_2 = (float)FUN_140776650(param_2);
  }
  fVar1 = param_1[1];
  param_1[1] = fVar2 * param_1[1] + param_2 * *param_1;
  *param_1 = fVar2 * *param_1 - param_2 * fVar1;
  return;
}


