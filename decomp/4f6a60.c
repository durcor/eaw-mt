/* MovementCoordinatorClass::vfunc_0  RVA 0x4f6a60  size 242  params 0  ret undefined */


undefined8 * MovementCoordinatorClass::vfunc_0(undefined8 *param_1,ulonglong param_2)

{
  longlong lVar1;
  longlong lVar2;
  ulonglong uVar3;
  
  *param_1 = vftable;
  FUN_1404f6c60();
  lVar1 = param_1[5];
  if (lVar1 != 0) {
    uVar3 = param_1[7] - lVar1 & 0xfffffffffffffff8;
    lVar2 = lVar1;
    if (0xfff < uVar3) {
      lVar2 = *(longlong *)(lVar1 + -8);
      uVar3 = uVar3 + 0x27;
      lVar1 = lVar1 - lVar2;
      if (0x1f < lVar1 - 8U) goto LAB_1404f6b4d;
    }
    thunk_FUN_1407864b8(lVar2,uVar3);
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
  }
  lVar1 = param_1[2];
  if (lVar1 != 0) {
    uVar3 = param_1[4] - lVar1 & 0xfffffffffffffff8;
    lVar2 = lVar1;
    if (0xfff < uVar3) {
      lVar2 = *(longlong *)(lVar1 + -8);
      uVar3 = uVar3 + 0x27;
      lVar1 = lVar1 - lVar2;
      if (0x1f < lVar1 - 8U) {
LAB_1404f6b4d:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(lVar1,uVar3);
      }
    }
    thunk_FUN_1407864b8(lVar2);
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
  }
  *param_1 = SignalListenerClass::vftable;
  FUN_14020a820(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0xb8);
  }
  return param_1;
}


