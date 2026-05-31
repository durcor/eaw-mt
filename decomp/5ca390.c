/* FighterLocomotorBehaviorClass::FUN_1405ca390  RVA 0x5ca390  size 148  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405ca390(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2b0);
  if (lVar2 == 0) {
    lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
  }
  else {
    lVar2 = *(longlong *)(lVar2 + 0xa8);
  }
  if (*(longlong *)(lVar2 + 0x1f0) != 0) {
    lVar1 = FUN_1405569b0(lVar2);
    if (lVar1 != 0) {
      lVar1 = FUN_14041af10(*(undefined8 *)(lVar2 + 0x1f0));
      if (((*(uint *)(lVar1 + 0x48) >> 1 & 1) != 0) && (*(longlong *)(lVar1 + 0x28) != 0)) {
        FUN_14041b440(*(undefined8 *)(lVar2 + 0x1f0));
        return;
      }
    }
  }
  FUN_140370f00(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298),*(longlong *)(param_1 + 0x28)
               );
  return;
}


