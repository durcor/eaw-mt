/* FUN_14020acd0  RVA 0x20acd0  size 349  params 0  ret undefined */


undefined4 * FUN_14020acd0(undefined4 *param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  undefined4 uVar3;
  float fVar4;
  uint uVar5;
  
  fVar2 = DAT_1408007f4;
  fVar1 = DAT_1408007dc;
  uVar5 = 0;
  if ((*param_2 == *param_3) && (param_2[1] == param_3[1])) {
    param_1[2] = 0;
  }
  else {
    fVar4 = (float)FUN_14078437c(param_3[1] - param_2[1],*param_3 - *param_2);
    fVar4 = fVar4 * (fVar2 / (fVar1 * DAT_1408007d4));
    param_1[2] = fVar4;
    if (fVar4 < 0.0) {
      param_1[2] = fVar4 + fVar2;
    }
  }
  if ((param_2[2] != param_3[2]) || (*param_2 != *param_3)) {
    uVar3 = FUN_140776d48((param_3[1] - param_2[1]) * (param_3[1] - param_2[1]) +
                          (*param_3 - *param_2) * (*param_3 - *param_2));
    fVar4 = (float)FUN_14078437c(param_3[2] - param_2[2],uVar3);
    uVar5 = (uint)(fVar4 * (fVar2 / (fVar1 * DAT_1408007d4))) ^ DAT_140800860;
  }
  param_1[1] = uVar5;
  *param_1 = 0;
  return param_1;
}


