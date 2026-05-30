/* SpaceMovementCoordinatorClass::vfunc_4  RVA 0x5d5130  size 166  params 0  ret undefined */


void SpaceMovementCoordinatorClass::vfunc_4(longlong param_1,longlong *param_2)

{
  char cVar1;
  longlong *plVar2;
  uint uVar3;
  ulonglong uVar4;
  
  if ((param_2 != (longlong *)0x0) &&
     ((((*(byte *)(param_2 + 0x74) & 2) != 0 ||
       (plVar2 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1), plVar2 == (longlong *)0x0))
      || (cVar1 = (**(code **)(*plVar2 + 0x2b0))(plVar2), cVar1 == '\0')))) {
    LandMovementCoordinatorClass::vfunc_4(param_1,param_2);
    uVar3 = 0;
    if (*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3 != 0) {
      uVar4 = 0;
      do {
        FUN_140410340(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar4 * 8),param_2);
        uVar3 = uVar3 + 1;
        uVar4 = (ulonglong)uVar3;
      } while (uVar4 < (ulonglong)
                       (*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3));
    }
  }
  return;
}


