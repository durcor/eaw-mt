/* FUN_140385620  RVA 0x385620  size 115  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_140385620(longlong param_1)

{
  undefined1 uVar1;
  undefined1 uVar2;
  longlong lVar3;
  
  uVar1 = *(undefined1 *)(*(longlong *)(param_1 + 0x20) + 0x3d5);
  if (*(longlong *)(param_1 + 0x10) != 0) {
    lVar3 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58));
    if ((lVar3 != 0) && (*(longlong *)(lVar3 + 0x360) != 0)) {
      uVar2 = 1;
      if (_DAT_140b27048 < *(float *)(*(longlong *)(param_1 + 0x20) + 0x22c)) {
        uVar2 = uVar1;
      }
      return uVar2;
    }
  }
  return uVar1;
}


