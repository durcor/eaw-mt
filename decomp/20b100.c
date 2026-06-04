/* FUN_14020b100  RVA 0x20b100  size 117  params 0  ret undefined */


float * FUN_14020b100(float *param_1,float *param_2,float *param_3)

{
  bool bVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  
  fVar4 = param_1[1] - param_3[1];
  fVar3 = param_1[2] - param_3[2];
  *param_2 = *param_1 - *param_3;
  fVar2 = DAT_1408007f4;
  bVar1 = fVar4 < DAT_1408007f4;
  param_2[1] = fVar4;
  param_2[2] = fVar3;
  if (bVar1) {
    if (0.0 < fVar4) goto LAB_14020b151;
    fVar4 = fVar4 + fVar2;
  }
  else {
    fVar4 = fVar4 - fVar2;
  }
  param_2[1] = fVar4;
LAB_14020b151:
  if (fVar2 <= fVar3) {
    param_2[2] = fVar3 - fVar2;
    return param_2;
  }
  if (fVar3 < 0.0) {
    param_2[2] = fVar3 + fVar2;
  }
  return param_2;
}


