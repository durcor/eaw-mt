/* FUN_1405c5910  RVA 0x5c5910  size 54  params 0  ret undefined */


longlong FUN_1405c5910(longlong param_1)

{
  uint uVar1;
  ulonglong uVar2;
  uint uVar3;
  longlong lVar4;
  
  lVar4 = *(longlong *)(param_1 + 0x20) - *(longlong *)(param_1 + 0x18);
  uVar2 = -(lVar4 >> 0x3f);
  uVar3 = (int)(lVar4 / 0x34) + (int)(lVar4 >> 0x3f) + (int)uVar2;
  if (uVar3 != 0) {
    uVar1 = *(int *)(param_1 + 0x60) + 1;
    uVar2 = (ulonglong)uVar1;
    if (uVar1 < uVar3) {
      return (ulonglong)(uint3)(uVar1 >> 8) << 8;
    }
  }
  return CONCAT71((int7)(uVar2 >> 8),1);
}


