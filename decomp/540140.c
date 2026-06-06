/* FUN_140540140  RVA 0x540140  size 138  params 0  ret undefined */


undefined8 FUN_140540140(longlong param_1,longlong param_2)

{
  ulonglong uVar1;
  longlong lVar2;
  undefined8 *puVar3;
  
  if (param_2 != 0) {
    lVar2 = FUN_1403973b0(param_2);
    if (lVar2 != 0) {
      lVar2 = FUN_1403973b0(param_2);
      uVar1 = *(ulonglong *)(lVar2 + 0x1648);
      if (uVar1 == 0) {
        lVar2 = FUN_1403973b0(param_2);
        puVar3 = (undefined8 *)(lVar2 + 0xf8);
        if (0xf < *(ulonglong *)(lVar2 + 0x110)) {
          puVar3 = (undefined8 *)*puVar3;
        }
        FUN_140025760("Error: Object type %s has no XML category mask assigned to it.\n",puVar3);
      }
      if ((*(ulonglong *)(param_1 + 0x408) & uVar1) == 0) {
        return 0;
      }
    }
  }
  return 1;
}


