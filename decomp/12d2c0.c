/* FUN_14012d2c0  RVA 0x12d2c0  size 219  params 0  ret undefined */


undefined8 FUN_14012d2c0(longlong *param_1,int param_2,undefined8 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  if (-1 < param_2) {
    iVar2 = FUN_140145fa0(param_1[0x1d]);
    if (param_2 < iVar2) {
      (**(code **)(*param_1 + 0x78))(param_1);
      puVar3 = (undefined4 *)((longlong)param_2 * 0x30 + *(longlong *)(param_1[0x1d] + 0x28));
      *(undefined4 *)param_3 = *puVar3;
      *(undefined4 *)((longlong)param_3 + 4) = puVar3[1];
      *(undefined4 *)(param_3 + 1) = puVar3[2];
      *(undefined4 *)((longlong)param_3 + 0xc) = puVar3[3];
      *(undefined4 *)(param_3 + 2) = puVar3[4];
      *(undefined4 *)((longlong)param_3 + 0x14) = puVar3[5];
      *(undefined4 *)(param_3 + 3) = puVar3[6];
      *(undefined4 *)((longlong)param_3 + 0x1c) = puVar3[7];
      *(undefined4 *)(param_3 + 4) = puVar3[8];
      *(undefined4 *)((longlong)param_3 + 0x24) = puVar3[9];
      *(undefined4 *)(param_3 + 5) = puVar3[10];
      uVar1 = puVar3[0xb];
      *(undefined4 *)((longlong)param_3 + 0x2c) = uVar1;
      return CONCAT71((uint7)(uint3)((uint)uVar1 >> 8),1);
    }
  }
  param_3[1] = 0;
  *(undefined4 *)(param_3 + 2) = 0;
  *(undefined8 *)((longlong)param_3 + 0x1c) = 0;
  *(undefined4 *)((longlong)param_3 + 0x24) = 0;
  *param_3 = 0x3f800000;
  *(undefined8 *)((longlong)param_3 + 0x14) = 0x3f800000;
  param_3[5] = 0x3f800000;
  return 0;
}


