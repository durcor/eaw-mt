/* FUN_1403a8710  RVA 0x3a8710  size 174  params 0  ret undefined */


void FUN_1403a8710(longlong param_1,float *param_2)

{
  float fVar1;
  
  if ((*(byte *)(param_1 + 0x3a0) & 0x10) == 0) {
    if (((*(float *)(param_1 + 0x84) != *param_2) || (*(float *)(param_1 + 0x88) != param_2[1])) ||
       (*(float *)(param_1 + 0x8c) != param_2[2])) {
      *(float *)(param_1 + 0x84) = *param_2;
      *(float *)(param_1 + 0x88) = param_2[1];
      *(float *)(param_1 + 0x8c) = param_2[2];
      *(byte *)(param_1 + 0x3a1) = *(byte *)(param_1 + 0x3a1) | 1;
    }
    if (((*(float *)(param_1 + 0x90) != *param_2) || (*(float *)(param_1 + 0x94) != param_2[1])) ||
       (*(float *)(param_1 + 0x98) != param_2[2])) {
      *(float *)(param_1 + 0x90) = *param_2;
      *(float *)(param_1 + 0x94) = param_2[1];
      fVar1 = param_2[2];
      *(byte *)(param_1 + 0x3a1) = *(byte *)(param_1 + 0x3a1) | 1;
      *(float *)(param_1 + 0x98) = fVar1;
    }
  }
  return;
}


