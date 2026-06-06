/* FUN_140397780  RVA 0x397780  size 167  params 0  ret undefined */


undefined8 FUN_140397780(longlong *param_1)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  float local_res8 [2];
  float local_res10 [2];
  
  lVar2 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
  plVar3 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1);
  if ((lVar2 != 0) && (plVar3 != (longlong *)0x0)) {
    cVar1 = (**(code **)(*plVar3 + 0x230))(plVar3);
    if (cVar1 != '\0') {
      FUN_140396850(param_1,local_res8,local_res10);
      uVar4 = FUN_140776d48(local_res8[0] * local_res8[0] + local_res10[0] * local_res10[0]);
      return uVar4;
    }
  }
  if (param_1[0x53] == 0) {
    return 0;
  }
  uVar4 = FUN_140373b30();
  return uVar4;
}


