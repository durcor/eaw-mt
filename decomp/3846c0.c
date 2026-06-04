/* FUN_1403846c0  RVA 0x3846c0  size 126  params 0  ret undefined */


void FUN_1403846c0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if ((((param_2 != (longlong *)0x0) && (lVar1 = *param_2, lVar1 != 0)) &&
      (*param_2 = 0, lVar1 != *(longlong *)(param_1 + 0xc0))) &&
     (((lVar1 != *(longlong *)(param_1 + 0xa8) && (lVar1 != *(longlong *)(param_1 + 0x40))) &&
      (lVar1 != *(longlong *)(param_1 + 0x50))))) {
    uVar2 = FUN_140058570();
    FUN_140220eb0(uVar2,lVar1 + 0x38,param_1,0x28);
    uVar2 = FUN_140058570();
    FUN_140220eb0(uVar2,lVar1 + 0x38,param_1,1);
  }
  return;
}


