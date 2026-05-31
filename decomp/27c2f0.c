/* FUN_14027c2f0  RVA 0x27c2f0  size 60  params 0  ret undefined */


void FUN_14027c2f0(undefined4 *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1[1] + 1;
  param_1[1] = iVar2;
  iVar1 = param_1[0x13];
  if (param_1[0x13] == 0) {
    param_1[0x12] = *param_1;
    param_1[0x13] = param_2;
    iVar1 = param_2;
  }
  if (1000 < (uint)(param_2 - iVar1)) {
    param_1[0x13] = param_2;
    param_1[0x10] = iVar2 - param_1[0x12];
    param_1[0x12] = iVar2;
  }
  return;
}


