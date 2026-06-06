/* FUN_140385c70  RVA 0x385c70  size 118  params 0  ret undefined */


undefined8 * FUN_140385c70(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  undefined8 uVar2;
  int iVar3;
  undefined1 local_38 [12];
  undefined4 local_2c;
  undefined4 local_1c;
  undefined4 local_c;
  
  iVar3 = *(int *)(param_1 + 0x2c);
  cVar1 = '\0';
  if ((-1 < iVar3) || (iVar3 = *(int *)(param_1 + 0x38), -1 < iVar3)) {
    uVar2 = FUN_1402648b0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2a0));
    cVar1 = FUN_14012d2c0(uVar2,iVar3,local_38);
  }
  *param_2 = 0;
  *(undefined4 *)(param_2 + 1) = 0;
  if (cVar1 == '\x01') {
    *(undefined4 *)param_2 = local_2c;
    *(undefined4 *)(param_2 + 1) = local_c;
    *(undefined4 *)((longlong)param_2 + 4) = local_1c;
  }
  return param_2;
}


