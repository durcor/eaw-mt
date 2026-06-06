/* SelectBehaviorClass::vfunc_2  RVA 0x4c2f20  size 67  params 0  ret undefined */


longlong SelectBehaviorClass::vfunc_2(longlong *param_1,undefined4 param_2)

{
  longlong *plVar1;
  longlong lVar2;
  
  lVar2 = (**(code **)(*param_1 + 0x18))();
  if (lVar2 == 0) {
    plVar1 = (longlong *)param_1[5];
    if (plVar1 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0001404c2f52. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      lVar2 = (**(code **)(*plVar1 + 0x10))(plVar1,param_2);
      return lVar2;
    }
    lVar2 = 0;
  }
  return lVar2;
}


