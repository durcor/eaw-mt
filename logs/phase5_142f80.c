/* FUN_140142f80  RVA 0x142f80 */


undefined8 FUN_140142f80(longlong param_1,longlong param_2)

{
  int iVar1;
  char *_Str2;
  int iVar2;
  longlong lVar3;
  char local_408 [1024];
  
  if (param_2 != 0) {
    FUN_140143740(local_408);
    iVar2 = 0;
    if (0 < *(int *)(param_1 + 0x30)) {
      lVar3 = 0;
      do {
        _Str2 = (char *)(**(code **)(**(longlong **)(lVar3 + *(longlong *)(param_1 + 0x28)) + 0x20))
                                  ();
        iVar1 = _stricmp(local_408,_Str2);
        if (iVar1 == 0) {
          return *(undefined8 *)(*(longlong *)(param_1 + 0x28) + (longlong)iVar2 * 8);
        }
        iVar2 = iVar2 + 1;
        lVar3 = lVar3 + 8;
      } while (iVar2 < *(int *)(param_1 + 0x30));
    }
  }
  return 0;
}

