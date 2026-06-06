/* FUN_14039b140  RVA 0x39b140  size 83  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_14039b140(longlong param_1)

{
  uint uVar1;
  ulonglong uVar2;
  
  if (*(longlong *)(param_1 + 0x220) == 0) {
    uVar2 = *(ulonglong *)(param_1 + 0x100);
    if ((uVar2 == 0) || (*(char *)(uVar2 + 0x2a4) == '\0')) goto LAB_14039b190;
    uVar1 = *(uint *)(uVar2 + 700);
  }
  else {
    uVar1 = *(uint *)(*(longlong *)(param_1 + 0x220) + 0x20);
  }
  uVar2 = (ulonglong)uVar1;
  if ((int)((int)((float)DAT_140b0a340 * _DAT_140b27894) + uVar1) < DAT_140b0a320) {
    return CONCAT71((uint7)(uint3)(uVar1 >> 8),1);
  }
LAB_14039b190:
  return uVar2 & 0xffffffffffffff00;
}


