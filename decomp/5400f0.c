/* FUN_1405400f0  RVA 0x5400f0  size 76  params 0  ret undefined */


void FUN_1405400f0(longlong param_1)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  
  if ((*(int *)(param_1 + 0x4a0) == 0) && (*(ulonglong *)(param_1 + 0x490) != 0)) {
    puVar2 = (undefined8 *)(param_1 + 0x480);
    if (0xf < *(ulonglong *)(param_1 + 0x498)) {
      puVar2 = (undefined8 *)*puVar2;
    }
    uVar1 = FUN_140215a30(puVar2,*(ulonglong *)(param_1 + 0x490) & 0xffffffff,0);
    *(undefined4 *)(param_1 + 0x4a0) = uVar1;
    return;
  }
  return;
}


