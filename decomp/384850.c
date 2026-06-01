/* FUN_140384850  RVA 0x384850  size 473  params 0  ret undefined */


undefined8 FUN_140384850(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  longlong *plVar4;
  
  if ((param_2 != 0) &&
     (((*(byte *)(param_2 + 0x3a0) & 0x42) != 0 || ((char)*(byte *)(param_2 + 0x3a0) < '\0')))) {
    plVar4 = *(longlong **)(param_1 + 0x20);
    if (0xf < (ulonglong)plVar4[3]) {
      plVar4 = (longlong *)*plVar4;
    }
    FUN_140025760("Dead, delete pending or death clone object passed to HardPointClass::Does_Weapon_State_Allow_Firing_At() for hardpoint %s\n"
                  ,plVar4);
    return 0;
  }
  lVar1 = *(longlong *)(param_1 + 0x20);
  if (*(int *)(lVar1 + 0x48) - 5U < 6) {
    if ((((*(char *)(lVar1 + 0x4d) != '\0') && (lVar1 != 0)) && (*(char *)(lVar1 + 0x4d) == '\x01'))
       && (*(float *)(param_1 + 0x28) <= 0.0)) {
      return 0;
    }
    if (*(char *)(param_1 + 0x6e) == '\0') {
      if ((param_2 != 0) && (cVar2 = FUN_140540140(), cVar2 != '\0')) {
        return 0;
      }
      iVar3 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),0);
      if ((iVar3 - 1U < 0x4c) &&
         (cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar3,1), cVar2 != '\0')) {
        if (*(char *)((longlong)iVar3 + 900 + *(longlong *)(param_1 + 0x20)) == '\0') {
          return 0;
        }
        cVar2 = FUN_140397f60(*(undefined8 *)(param_1 + 0x10),1);
        if (cVar2 == '\0') {
          return 0;
        }
      }
      iVar3 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),1);
      if ((iVar3 - 1U < 0x4c) &&
         (cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar3,1), cVar2 != '\0')) {
        if (*(char *)((longlong)iVar3 + 900 + *(longlong *)(param_1 + 0x20)) == '\0') {
          return 0;
        }
        cVar2 = FUN_140397f60(*(undefined8 *)(param_1 + 0x10),1);
        if (cVar2 == '\0') {
          return 0;
        }
      }
      if ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x3d1) == '\0') &&
         (cVar2 = FUN_14039a390(*(undefined8 *)(param_1 + 0x10)), cVar2 != '\0')) {
        return 0;
      }
      if ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x3d2) == '\0') &&
         (cVar2 = FUN_14039a390(*(undefined8 *)(param_1 + 0x10)), cVar2 == '\0')) {
        return 0;
      }
      if ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x3d4) == '\0') &&
         (cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),0xb,1), cVar2 == '\0')) {
        return 0;
      }
      if ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x3d3) == '\0') &&
         (cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),0xb,1), cVar2 != '\0')) {
        return 0;
      }
      return 1;
    }
  }
  return 0;
}


