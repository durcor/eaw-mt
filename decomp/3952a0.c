/* FUN_1403952a0  RVA 0x3952a0  size 59  params 0  ret undefined */


undefined8 FUN_1403952a0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = FUN_140397d90();
  if ((lVar1 != 0) && (*(longlong *)(param_1 + 0x100) != 0)) {
    uVar2 = FUN_14037bd50(*(longlong *)(param_1 + 0x100) + 8,*(undefined4 *)(lVar1 + 0x88));
    return uVar2;
  }
  return 0;
}


