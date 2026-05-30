/* LandMovementCoordinatorClass::vfunc_4  RVA 0x4f7d30  size 246  params 0  ret undefined */


void LandMovementCoordinatorClass::vfunc_4(longlong param_1,longlong param_2)

{
  char cVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong *plVar4;
  uint uVar5;
  ulonglong uVar6;
  
  if ((param_2 != 0) &&
     (((*(byte *)(param_2 + 0x3a0) & 2) != 0 || (cVar1 = FUN_14039ad40(param_2), cVar1 != '\x01'))))
  {
    plVar4 = *(longlong **)(param_1 + 0x28);
    do {
      plVar2 = plVar4;
      if (plVar2 == *(longlong **)(param_1 + 0x30)) goto LAB_1404f7dd8;
      plVar4 = plVar2 + 1;
    } while (*plVar2 != param_2);
    FUN_14076bd80(plVar2,plVar4,(longlong)*(longlong **)(param_1 + 0x30) - (longlong)plVar4);
    plVar4 = (longlong *)(*(longlong *)(param_1 + 0x30) + -8);
    *(longlong **)(param_1 + 0x30) = plVar4;
    if (param_2 != *(longlong *)(param_1 + 0x68)) {
      for (plVar2 = *(longlong **)(param_1 + 0x28); plVar2 != plVar4; plVar2 = plVar2 + 1) {
        if (*plVar2 == param_2) goto LAB_1404f7dd8;
      }
      uVar3 = FUN_140058570();
      FUN_140220eb0(uVar3,param_2 + 0x38,param_1,0x10);
    }
LAB_1404f7dd8:
    uVar5 = 0;
    if (*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3 != 0) {
      uVar6 = 0;
      do {
        FUN_140421910(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar6 * 8),param_2);
        uVar5 = uVar5 + 1;
        uVar6 = (ulonglong)uVar5;
      } while (uVar6 < (ulonglong)
                       (*(longlong *)(param_1 + 0x18) - *(longlong *)(param_1 + 0x10) >> 3));
    }
  }
  return;
}


