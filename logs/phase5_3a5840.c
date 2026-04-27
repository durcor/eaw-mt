/* FUN_1403a5840  RVA 0x3a5840 */


void FUN_1403a5840(longlong param_1)

{
  int *piVar1;
  char cVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  plVar3 = (longlong *)FUN_140397680();
  if (*(longlong *)(param_1 + 0x2a0) != 0) {
    if (*(longlong *)(param_1 + 0x2e0) != 0) {
      if ((*(longlong *)(*(longlong *)(param_1 + 0x2e0) + 0x2a0) != 0) &&
         (lVar4 = FUN_1402648b0(), lVar4 != 0)) {
        uVar5 = FUN_140264a30(*(undefined8 *)(param_1 + 0x2a0));
        cVar2 = FUN_14012d860(lVar4,uVar5);
        if (cVar2 == '\0') {
          (**(code **)(*plVar3 + 0x30))(plVar3,*(undefined8 *)(param_1 + 0x2a0));
        }
      }
      if ((*(longlong *)(param_1 + 0x2e0) != 0) &&
         (0.0 < *(float *)(*(longlong *)(param_1 + 0x298) + 0x23ec))) {
        piVar1 = (int *)(*(longlong *)(param_1 + 0x2e0) + 0x328);
        *piVar1 = *piVar1 + -1;
      }
      *(undefined8 *)(param_1 + 0x2e0) = 0;
      *(undefined8 *)(param_1 + 0x2f8) = 0;
      return;
    }
    if (plVar3 != (longlong *)0x0) {
      (**(code **)(*plVar3 + 0x30))(plVar3);
    }
  }
  return;
}

