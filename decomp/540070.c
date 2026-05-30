/* FUN_140540070  RVA 0x540070  size 116  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_140540070(longlong param_1)

{
  int iVar1;
  float fVar2;
  
  fVar2 = *(float *)(param_1 + 0x58);
  if (DAT_140b15418 != (longlong *)0x0) {
    iVar1 = (**(code **)(*DAT_140b15418 + 0xe0))();
    if (iVar1 == 1) {
      return fVar2 * _DAT_140b16dcc;
    }
    iVar1 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar1 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar1 == 2) {
      fVar2 = fVar2 * _DAT_140b16dc8;
    }
  }
  return fVar2;
}


