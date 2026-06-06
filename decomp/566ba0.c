/* TeamTargetingBehaviorClass::vfunc_35  RVA 0x566ba0  size 213  params 0  ret undefined */


ulonglong TeamTargetingBehaviorClass::vfunc_35(longlong param_1,longlong param_2)

{
  char cVar1;
  longlong lVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  if (param_2 == 0) {
LAB_140566c62:
    return (ulonglong)DAT_140899788;
  }
  cVar1 = FUN_1403751e0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298),
                        *(undefined8 *)(param_2 + 0x1648));
  if (cVar1 != '\0') goto LAB_140566c62;
  lVar2 = *(longlong *)(param_1 + 0x28);
  if (*(longlong *)(lVar2 + 0xf0) == 0) goto LAB_140566c4f;
  lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0xf0) + 0x160);
  if (lVar3 == 0) {
    lVar3 = *(longlong *)(lVar2 + 0x298);
    if (*(char *)(lVar2 + 0x348) != -1) {
      if (*(longlong *)(lVar2 + 0x110) == 0) goto LAB_140566c4f;
      lVar2 = FUN_1404ec820();
      if (lVar2 == 0) goto LAB_140566c4f;
      lVar2 = FUN_1404ec820(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x110));
      lVar3 = *(longlong *)(lVar2 + 0x298);
    }
    lVar3 = *(longlong *)(lVar3 + 0x520);
    if (lVar3 == 0) {
LAB_140566c4f:
      return (ulonglong)DAT_140899780;
    }
  }
  uVar4 = FUN_14054dc00(lVar3,param_2);
  return uVar4;
}


