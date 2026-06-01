/* FUN_140380bb0  RVA 0x380bb0  size 59  params 0  ret undefined */


ulonglong FUN_140380bb0(longlong param_1)

{
  uint uVar1;
  ulonglong in_RAX;
  
  if (-1 < *(int *)(param_1 + 0x104)) {
    uVar1 = DAT_140b0a320 - *(int *)(param_1 + 0x104);
    in_RAX = (ulonglong)uVar1;
    if ((float)(int)uVar1 / (float)DAT_140b0a340 < DAT_140b2701c) {
      return CONCAT71((uint7)(uint3)(uVar1 >> 8),1);
    }
  }
  return in_RAX & 0xffffffffffffff00;
}


