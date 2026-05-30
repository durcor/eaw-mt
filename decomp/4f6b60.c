/* MovementCoordinatorClass::vfunc_3  RVA 0x4f6b60  size 255  params 0  ret undefined */


void MovementCoordinatorClass::vfunc_3(longlong param_1,longlong param_2,char param_3)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong local_res10;
  
  if ((((param_2 != 0) && ((*(byte *)(param_2 + 0x3a0) & 2) == 0)) &&
      (*(char *)(param_2 + 0x333) != -1)) &&
     ((local_res10 = param_2, param_3 != '\0' || (lVar1 = FUN_140397260(param_2), lVar1 == 0)))) {
    for (plVar2 = *(longlong **)(param_1 + 0x28); plVar2 != *(longlong **)(param_1 + 0x30);
        plVar2 = plVar2 + 1) {
      if (*plVar2 == param_2) {
        return;
      }
    }
    plVar2 = *(longlong **)(param_1 + 0x30);
    if (plVar2 == *(longlong **)(param_1 + 0x38)) {
      FUN_140045580(param_1 + 0x28,plVar2,&local_res10);
    }
    else {
      *plVar2 = param_2;
      *(longlong *)(param_1 + 0x30) = *(longlong *)(param_1 + 0x30) + 8;
    }
    if (*(int *)(param_1 + 0xa8) == 0) {
      *(undefined4 *)(param_1 + 0xa8) = 1;
    }
    uVar3 = FUN_140058570();
    FUN_140220e90(uVar3,param_2 + 0x38,param_1,0x10);
    uVar3 = FUN_140058570();
    FUN_140220ed0(uVar3,param_2 + 0x38,9);
    if (*(longlong *)(param_2 + 0x100) != 0) {
      FUN_14037ca30(*(longlong *)(param_2 + 0x100) + 8);
    }
  }
  return;
}


