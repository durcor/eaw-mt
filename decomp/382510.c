/* FUN_140382510  RVA 0x382510  size 151  params 0  ret undefined */


void FUN_140382510(longlong param_1,longlong param_2,longlong *param_3)

{
  undefined8 uVar1;
  
  if ((param_2 != 0) && (*param_3 != param_2)) {
    if ((param_2 != *(longlong *)(param_1 + 0xc0)) &&
       (((param_2 != *(longlong *)(param_1 + 0xa8) && (param_2 != *(longlong *)(param_1 + 0x40))) &&
        (param_2 != *(longlong *)(param_1 + 0x50))))) {
      uVar1 = FUN_140058570();
      FUN_140220e90(uVar1,param_2 + 0x38,param_1,0x28);
      uVar1 = FUN_140058570();
      FUN_140220e90(uVar1,param_2 + 0x38,param_1,1);
    }
    *param_3 = param_2;
  }
  return;
}


