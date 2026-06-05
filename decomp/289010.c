/* FUN_140289010  RVA 0x289010  size 58  params 0  ret undefined */


void FUN_140289010(longlong param_1)

{
  undefined4 uVar1;
  
  if (DAT_140b15418 == 0) {
    *(undefined4 *)(param_1 + 0x404) = 0;
  }
  uVar1 = FUN_1402ac700(*(undefined8 *)(DAT_140b15418 + 0x18),*(undefined4 *)(param_1 + 0x4c));
  *(undefined4 *)(param_1 + 0x404) = uVar1;
  return;
}


