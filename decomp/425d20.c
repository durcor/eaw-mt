/* MovementCoordinatorSystemClass::vfunc_0  RVA 0x425d20  size 256  params 0  ret undefined */


undefined8 * MovementCoordinatorSystemClass::vfunc_0(undefined8 *param_1,uint param_2)

{
  undefined8 *puVar1;
  longlong lVar2;
  ulonglong uVar3;
  uint uVar4;
  longlong lVar5;
  
  uVar4 = 0;
  *param_1 = vftable;
  lVar2 = param_1[1];
  if (param_1[2] - lVar2 >> 3 != 0) {
    uVar3 = 0;
    do {
      FUN_1404f6c60(*(undefined8 *)(lVar2 + uVar3 * 8));
      puVar1 = *(undefined8 **)(param_1[1] + uVar3 * 8);
      if (puVar1 != (undefined8 *)0x0) {
        (**(code **)*puVar1)(puVar1,1);
      }
      lVar2 = param_1[1];
      uVar4 = uVar4 + 1;
      uVar3 = (ulonglong)uVar4;
    } while (uVar3 < (ulonglong)(param_1[2] - lVar2 >> 3));
  }
  param_1[2] = lVar2;
  if (lVar2 != 0) {
    uVar3 = param_1[3] - lVar2 & 0xfffffffffffffff8;
    lVar5 = lVar2;
    if (0xfff < uVar3) {
      lVar5 = *(longlong *)(lVar2 + -8);
      if (0x1f < (lVar2 - lVar5) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(lVar5,uVar3 + 0x27);
      }
    }
    thunk_FUN_1407864b8(lVar5);
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
  }
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0x28);
  }
  return param_1;
}


