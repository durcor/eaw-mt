/* FUN_14063ea50  RVA 0x63ea50  size 216  params 0  ret undefined */


undefined8 *
FUN_14063ea50(undefined8 *param_1,undefined4 *param_2,undefined4 *param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = FUN_14020b710(*param_2);
  *param_2 = uVar1;
  uVar1 = FUN_14020b710(param_2[1]);
  param_2[1] = uVar1;
  uVar1 = FUN_14020b710(param_2[2]);
  param_2[2] = uVar1;
  uVar1 = FUN_14020b710(*param_3);
  *param_3 = uVar1;
  uVar1 = FUN_14020b710(param_3[1]);
  param_3[1] = uVar1;
  uVar2 = FUN_14020b710(param_3[2]);
  uVar1 = *param_3;
  param_3[2] = uVar2;
  uVar2 = *param_2;
  *param_1 = 0;
  *(undefined4 *)(param_1 + 1) = 0;
  uVar2 = FUN_14063efd0(uVar2,uVar1,param_4);
  uVar1 = param_3[1];
  *(undefined4 *)param_1 = uVar2;
  uVar2 = FUN_14063efd0(param_2[1],uVar1,param_4);
  uVar1 = param_3[2];
  *(undefined4 *)((longlong)param_1 + 4) = uVar2;
  uVar1 = FUN_14063efd0(param_2[2],uVar1,param_4);
  *(undefined4 *)(param_1 + 1) = uVar1;
  return param_1;
}


