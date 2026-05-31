/* FUN_140139800  RVA 0x139800  size 116  params 0  ret undefined */


float FUN_140139800(float *param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  
  fVar1 = param_1[1];
  fVar2 = *param_1;
  fVar3 = param_1[2];
  fVar5 = SQRT(fVar2 * fVar2 + fVar1 * fVar1 + fVar3 * fVar3);
  if (0.0 < fVar5) {
    fVar4 = DAT_1407ffaf8 / fVar5;
    *param_1 = fVar2 * fVar4;
    param_1[1] = fVar1 * fVar4;
    param_1[2] = fVar3 * fVar4;
  }
  return fVar5;
}


