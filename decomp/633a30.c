/* TargetingBehaviorClass::vfunc_6  RVA 0x633a30  size 174  params 0  ret undefined */


undefined8 TargetingBehaviorClass::vfunc_6(undefined8 param_1,longlong *param_2,undefined4 param_3)

{
  int iVar1;
  longlong *plVar2;
  undefined8 uVar3;
  
  LocomotorCommonClass::vfunc_6();
  plVar2 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1);
  if (plVar2 == (longlong *)0x0) {
    iVar1 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar1 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
  }
  else {
    iVar1 = (**(code **)(*plVar2 + 0x238))(plVar2);
  }
  FUN_140565e50(param_1);
  if (iVar1 == 1) {
    uVar3 = FUN_140634810(param_1,param_2,param_3);
  }
  else if (iVar1 == 2) {
    uVar3 = FUN_140633ae0(param_1,param_2,param_3);
  }
  else {
    uVar3 = 0x650000;
  }
  return uVar3;
}


