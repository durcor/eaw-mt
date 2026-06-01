/* FUN_14035f470  RVA 0x35f470  size 567  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14035f470(longlong *param_1,int param_2,longlong param_3,char param_4)

{
  LPVOID pvVar1;
  char cVar2;
  int iVar3;
  HANDLE hHeap;
  longlong lVar4;
  int iVar5;
  
  if ((((DAT_140a284c4 == '\0') || (param_3 == 0)) ||
      ((*(char *)(param_3 + 0x34b) == -1 && (*(char *)(param_3 + 0x348) == -1)))) ||
     ((((cVar2 = FUN_14039a230(param_3,param_2,0), cVar2 == '\x01' &&
        (*(char *)(param_3 + 0x34a) != -1)) || (param_1[0x33] == 0)) ||
      ((lVar4 = *(longlong *)(param_1[0x33] + (longlong)param_2 * 8), lVar4 == 0 ||
       (*(char *)(lVar4 + 0x50) == '\x01')))))) {
    return 0;
  }
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
               0xd4) < DAT_140b2c228) && (_Init_thread_header(&DAT_140b2c228), DAT_140b2c228 == -1))
  {
    atexit(FUN_1407f0090);
    _Init_thread_footer(&DAT_140b2c228);
  }
  iVar5 = 0;
  if (DAT_140a28538 != (LPVOID)0x0) {
    if ((_DAT_140a28544 & 0x80000000) == 0) {
      thunk_FUN_1407864b8();
    }
    else {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,DAT_140a28538);
    }
    _DAT_140a28544 = _DAT_140a28544 & 0x80000000;
    DAT_140a28538 = (LPVOID)0x0;
    DAT_140a28540 = 0;
  }
  FUN_14035dec0(param_1,param_3,&PTR_vftable_140a28530);
  if (0 < DAT_140a28540) {
    do {
      pvVar1 = (LPVOID)((longlong)DAT_140a28538 + (longlong)iVar5 * 0xc);
      cVar2 = (**(code **)(*param_1 + 0x240))(param_1);
      if (cVar2 == '\0') {
        return 0;
      }
      if (DAT_140a284c4 == '\0') {
        return 0;
      }
      if ((param_4 == '\0') &&
         (lVar4 = FUN_140294bc0(&DAT_140a16fd0,param_2), *(char *)(lVar4 + 0x62) == '\0')) {
        iVar3 = (**(code **)(*param_1 + 0xe0))(param_1);
        cVar2 = DAT_140b15aec;
        if ((iVar3 != 0) &&
           ((cVar2 = DAT_140b15aee, iVar3 != 1 && (cVar2 = DAT_140b15aed, iVar3 != 2)))) {
          return 0;
        }
        if (cVar2 == '\0') {
          return 0;
        }
      }
      if (param_1[0x33] == 0) {
        return 0;
      }
      lVar4 = *(longlong *)(param_1[0x33] + (longlong)param_2 * 8);
      if (lVar4 == 0) {
        return 0;
      }
      cVar2 = FUN_1404c0e00(lVar4,pvVar1);
      if (cVar2 == '\0') {
        return 0;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < DAT_140a28540);
  }
  return 1;
}


