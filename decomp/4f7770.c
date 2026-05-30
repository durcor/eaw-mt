/* LandMovementCoordinatorClass::vfunc_6  RVA 0x4f7770  size 102  params 0  ret undefined */


void LandMovementCoordinatorClass::vfunc_6(longlong param_1,undefined8 param_2)

{
  uint uVar1;
  ulonglong uVar2;
  
  uVar1 = 0;
  if (*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3 != 0) {
    uVar2 = 0;
    do {
      FUN_140420f70(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar2 * 8),param_2);
      uVar1 = uVar1 + 1;
      uVar2 = (ulonglong)uVar1;
    } while (uVar2 < (ulonglong)(*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3)
            );
  }
  return;
}


