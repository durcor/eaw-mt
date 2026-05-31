/* StarshipLocomotorBehaviorClass::FUN_140623340  RVA 0x623340  size 206  params 0  ret undefined */


undefined * StarshipLocomotorBehaviorClass::FUN_140623340(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  longlong lVar3;
  undefined *puVar4;
  
  plVar1 = *(longlong **)(*(longlong *)(param_1 + 0x28) + 0x2b0);
  if ((plVar1 != (longlong *)0x0) && ((char)plVar1[0x69] != -1)) {
    lVar2 = (**(code **)(*plVar1 + 0x10))(plVar1,0x16);
    if (lVar2 != 0) {
      lVar3 = FUN_140405210(lVar2);
      if (lVar3 != 0) {
        return (undefined *)(lVar3 + 0x78);
      }
      puVar4 = (undefined *)FUN_140404c00(lVar2);
      return puVar4;
    }
  }
  lVar2 = *(longlong *)(param_1 + 0x28);
  if ((((*(char *)(lVar2 + 0x33b) != -1) && (*(longlong *)(lVar2 + 0xf0) != 0)) &&
      (lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0xf0) + 8), lVar3 != 0)) &&
     ((*(byte *)(lVar3 + 0x3a0) & 2) == 0)) {
    return (undefined *)(lVar3 + 0x78);
  }
  if (*(longlong *)(lVar2 + 0xa8) != 0) {
    return (undefined *)(*(longlong *)(lVar2 + 0xa8) + 8);
  }
  return &DAT_140b39920;
}


