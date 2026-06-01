/* IonStunEffectBehaviorClass::vfunc_6  RVA 0x62bba0  size 73  params 0  ret undefined */


undefined8 IonStunEffectBehaviorClass::vfunc_6(longlong *param_1,longlong param_2)

{
  LocomotorCommonClass::vfunc_6();
  if (*(longlong *)(param_2 + 0x2a0) != 0) {
    FUN_140265ae0(*(longlong *)(param_2 + 0x2a0),2);
  }
  (**(code **)(*param_1 + 0xc0))(param_1);
  return 0x650000;
}


