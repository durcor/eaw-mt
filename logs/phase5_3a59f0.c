/* FUN_1403a59f0  RVA 0x3a59f0 */


void FUN_1403a59f0(longlong *param_1,char param_2)

{
  longlong *lpMem;
  HANDLE pvVar1;
  LPVOID pvVar2;
  char cVar3;
  
  lpMem = (longlong *)*param_1;
  if (param_2 == '\0') {
    if (lpMem != (longlong *)0x0) {
      if (lpMem != param_1 + 1) {
        pvVar1 = GetProcessHeap();
        HeapFree(pvVar1,0,lpMem);
        *param_1 = (longlong)(param_1 + 1);
      }
      *(undefined2 *)(param_1 + 2) = 0x100;
      return;
    }
  }
  else {
    pvVar1 = GetProcessHeap();
    if (lpMem == param_1 + 1) {
      pvVar2 = HeapAlloc(pvVar1,0,(longlong)param_2 << 3);
      *param_1 = (longlong)pvVar2;
      FUN_14076bd80(pvVar2,param_1 + 1,(longlong)(char)param_1[2] << 3);
    }
    else {
      pvVar2 = HeapReAlloc(pvVar1,0,lpMem,(longlong)param_2 << 3);
      *param_1 = (longlong)pvVar2;
      cVar3 = param_2;
      if ((char)param_1[2] < param_2) {
        cVar3 = (char)param_1[2];
      }
      *(char *)(param_1 + 2) = cVar3;
    }
    *(char *)((longlong)param_1 + 0x11) = param_2;
  }
  return;
}

