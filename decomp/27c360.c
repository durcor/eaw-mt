/* FUN_14027c360  RVA 0x27c360  size 103  params 0  ret undefined */


void FUN_14027c360(int *param_1,int param_2)

{
  undefined8 uVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = *param_1 + 1;
  *(undefined1 *)(param_1 + 5) = 1;
  *param_1 = iVar2;
  iVar3 = param_1[0xf];
  if (param_1[0xf] == 0) {
    param_1[0xe] = iVar2;
    param_1[0xf] = param_2;
    iVar3 = param_2;
  }
  if (1000 < (uint)(param_2 - iVar3)) {
    param_1[0xf] = param_2;
    param_1[7] = iVar2 - param_1[0xe];
    param_1[0xe] = iVar2;
  }
  if (DAT_140a7c080 != '\0') {
    uVar1 = FUN_140200080();
    FUN_140214a70(uVar1,"__________________________Frame %d________________________________\n",iVar2
                 );
    return;
  }
  return;
}


