/* FUN_14039b1a0  RVA 0x39b1a0  size 27  params 0  ret undefined */


longlong FUN_14039b1a0(longlong param_1)

{
  longlong lVar1;
  uint7 uVar2;
  
  lVar1 = *(longlong *)(param_1 + 0x2b0);
  uVar2 = (uint7)((ulonglong)lVar1 >> 8);
  if ((lVar1 != 0) && (*(char *)(lVar1 + 0x348) != -1)) {
    return CONCAT71(uVar2,1);
  }
  return (ulonglong)uVar2 << 8;
}


