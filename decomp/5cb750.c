/* FighterLocomotorBehaviorClass::FUN_1405cb750  RVA 0x5cb750  size 220  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405cb750(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  lVar1 = *(longlong *)(param_1 + 0x28);
  plVar2 = *(longlong **)(lVar1 + 0x2b0);
  uVar4 = *(undefined8 *)(lVar1 + 0xa8);
  if ((plVar2 == (longlong *)0x0) || ((char)plVar2[0x69] == -1)) {
    FUN_140559250(uVar4,0x1c);
    FUN_1405c9ca0(param_1);
    FUN_140554480(uVar4,*(undefined8 *)(param_1 + 0x28),0);
  }
  else {
    uVar3 = FUN_140404950();
    FUN_1404f9d40(uVar3,lVar1);
    uVar3 = (**(code **)(*plVar2 + 0x10))(plVar2,0x16);
    FUN_1404061f0(uVar3,0);
    FUN_1404061d0(uVar3,0);
    FUN_140559250(uVar4,0x1c);
    FUN_1405c9ca0(param_1);
    FUN_140554480(uVar4,*(undefined8 *)(param_1 + 0x28),0);
    uVar4 = FUN_140404950();
    FUN_1404fafd0(uVar4,*(undefined8 *)(param_1 + 0x28));
  }
  return;
}


