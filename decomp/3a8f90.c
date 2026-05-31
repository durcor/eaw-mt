/* FUN_1403a8f90  RVA 0x3a8f90  size 88  params 0  ret undefined */


void FUN_1403a8f90(float param_1,float *param_2,char param_3)

{
  longlong in_RCX;
  
  if ((((param_3 != '\0') || (param_1 = *(float *)(in_RCX + 0x78), param_1 != *param_2)) ||
      (param_1 = *(float *)(in_RCX + 0x7c), param_1 != param_2[1])) ||
     (param_1 = *(float *)(in_RCX + 0x80), param_1 != param_2[2])) {
    *(float *)(in_RCX + 0x78) = *param_2;
    *(float *)(in_RCX + 0x7c) = param_2[1];
    *(float *)(in_RCX + 0x80) = param_2[2];
    if (param_3 != '\0') {
      DynamicTransformBehaviorClass::FUN_1403ac530(param_1,param_3);
      return;
    }
    *(byte *)(in_RCX + 0x3a1) = *(byte *)(in_RCX + 0x3a1) | 1;
  }
  return;
}


