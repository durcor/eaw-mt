/* FUN_14012d520  RVA 0x12d520  size 162  params 0  ret undefined */


ulonglong FUN_14012d520(longlong param_1,char *param_2)

{
  int iVar1;
  char *_Str1;
  ulonglong uVar2;
  longlong lVar3;
  
  if (param_2 != (char *)0x0) {
    lVar3 = *(longlong *)(param_1 + 0xf0);
    uVar2 = 0;
    if (*(longlong *)(param_1 + 0xf8) - lVar3 >> 4 != 0) {
      do {
        _Str1 = (char *)(**(code **)(**(longlong **)(lVar3 + 8 + uVar2 * 0x10) + 0x30))();
        iVar1 = _stricmp(_Str1,param_2);
        if (iVar1 == 0) {
          return uVar2;
        }
        lVar3 = *(longlong *)(param_1 + 0xf0);
        uVar2 = (ulonglong)((int)uVar2 + 1);
      } while (uVar2 < (ulonglong)(*(longlong *)(param_1 + 0xf8) - lVar3 >> 4));
    }
  }
  return 0xffffffff;
}


