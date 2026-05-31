/* FighterLocomotorBehaviorClass::FUN_1405c9ca0  RVA 0x5c9ca0  size 157  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405c9ca0(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong *plVar4;
  
  plVar4 = *(longlong **)(param_1 + 0x28);
  plVar1 = (longlong *)plVar4[0x56];
  if ((plVar1 != (longlong *)0x0) && ((char)plVar1[0x69] != -1)) {
    uVar3 = (**(code **)(*plVar1 + 0x10))(plVar1,0x16);
    FUN_1404061d0(uVar3,0);
    FUN_140406370(uVar3,0,0xffffffff);
    lVar2 = *(longlong *)(param_1 + 0x28);
    uVar3 = FUN_140404950();
    thunk_FUN_1404f9d50(uVar3,*(undefined8 *)(lVar2 + 0x2b0));
    return;
  }
  plVar4 = (longlong *)(**(code **)(*plVar4 + 0x10))(plVar4,9);
  if (plVar4 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0001405c9d30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*plVar4 + 0xd8))(plVar4,0);
    return;
  }
  return;
}


