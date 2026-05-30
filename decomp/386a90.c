/* HardPointClass::vfunc_1  RVA 0x386a90  size 444  params 0  ret undefined */


void HardPointClass::vfunc_1(longlong param_1,longlong param_2,int param_3,longlong param_4)

{
  longlong *plVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  if (param_3 == 1) {
    lVar4 = param_2 + -0x38;
    if (param_2 == 0) {
      lVar4 = 0;
    }
    if ((lVar4 != 0) && (lVar4 != param_4)) {
      lVar3 = *(longlong *)(param_1 + 0x40);
      plVar1 = (longlong *)(param_1 + 0x40);
      if (lVar3 == lVar4) {
        *(undefined8 *)(param_1 + 0x48) = 0;
        if ((((plVar1 != (longlong *)0x0) && (lVar3 != 0)) &&
            (*plVar1 = 0, lVar3 != *(longlong *)(param_1 + 0xc0))) &&
           ((lVar3 != *(longlong *)(param_1 + 0xa8) && (lVar3 != *(longlong *)(param_1 + 0x50))))) {
          uVar5 = FUN_140058570();
          FUN_140220eb0(uVar5,lVar3 + 0x38,param_1,0x28);
          uVar5 = FUN_140058570();
          FUN_140220eb0(uVar5,lVar3 + 0x38,param_1,1);
        }
        if (param_4 != 0) {
          FUN_140382510(param_1,param_4,plVar1);
        }
      }
      lVar3 = *(longlong *)(param_1 + 0x50);
      puVar2 = (undefined8 *)(param_1 + 0x50);
      if (lVar3 == lVar4) {
        if (((puVar2 != (undefined8 *)0x0) && (lVar3 != 0)) &&
           ((*puVar2 = 0, lVar3 != *(longlong *)(param_1 + 0xc0) &&
            ((lVar3 != *(longlong *)(param_1 + 0xa8) && (lVar3 != *plVar1)))))) {
          uVar5 = FUN_140058570();
          FUN_140220eb0(uVar5,lVar3 + 0x38,param_1,0x28);
          uVar5 = FUN_140058570();
          FUN_140220eb0(uVar5,lVar3 + 0x38,param_1,1);
        }
        if (param_4 != 0) {
          FUN_140382510(param_1,param_4,puVar2);
        }
      }
    }
  }
  else if (param_3 == 0x28) {
    lVar4 = param_2 + -0x38;
    if (param_2 == 0) {
      lVar4 = 0;
    }
    if (lVar4 != 0) {
      if (lVar4 == *(longlong *)(param_1 + 0x40)) {
        *(undefined8 *)(param_1 + 0x40) = 0;
        *(undefined8 *)(param_1 + 0x48) = 0;
      }
      if (lVar4 == *(longlong *)(param_1 + 0x50)) {
        *(undefined8 *)(param_1 + 0x50) = 0;
      }
      if (lVar4 == *(longlong *)(param_1 + 0xa8)) {
        *(undefined8 *)(param_1 + 0xa8) = 0;
      }
      if (lVar4 == *(longlong *)(param_1 + 0xc0)) {
        *(undefined8 *)(param_1 + 0xc0) = 0;
        return;
      }
    }
  }
  return;
}


