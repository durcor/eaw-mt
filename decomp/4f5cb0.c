/* FUN_1404f5cb0  RVA 0x4f5cb0  size 376  params 0  ret undefined */


void FUN_1404f5cb0(longlong param_1,uint param_2,char param_3,char param_4)

{
  uint uVar1;
  HANDLE pvVar2;
  LPVOID pvVar3;
  longlong lVar4;
  SIZE_T dwBytes;
  
  pvVar3 = *(LPVOID *)(param_1 + 8);
  dwBytes = (SIZE_T)(int)param_2;
  if (param_2 == 0) {
    if (pvVar3 != (LPVOID)0x0) {
      if ((*(uint *)(param_1 + 0x14) & 0x80000000) == 0) {
        thunk_FUN_1407864b8(pvVar3);
      }
      else {
        pvVar2 = GetProcessHeap();
        HeapFree(pvVar2,0,pvVar3);
      }
      *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0x80000000;
      *(undefined8 *)(param_1 + 8) = 0;
      *(undefined4 *)(param_1 + 0x10) = 0;
    }
  }
  else if (pvVar3 == (LPVOID)0x0) {
    if (*(int *)(param_1 + 0x14) < 0) {
      pvVar2 = GetProcessHeap();
      pvVar3 = HeapAlloc(pvVar2,0,dwBytes);
    }
    else {
      pvVar3 = (LPVOID)thunk_FUN_140769c58(dwBytes);
    }
    *(LPVOID *)(param_1 + 8) = pvVar3;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0x80000000;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | param_2 & 0x7fffffff;
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  else {
    if (*(int *)(param_1 + 0x14) < 0) {
      pvVar2 = GetProcessHeap();
      pvVar3 = HeapReAlloc(pvVar2,0,pvVar3,dwBytes);
    }
    else {
      pvVar3 = (LPVOID)thunk_FUN_140769c58(dwBytes);
      if (param_4 != '\0') {
        uVar1 = *(int *)(param_1 + 0x14) * 2 >> 1;
        if ((int)param_2 < (int)uVar1) {
          uVar1 = param_2;
        }
        if (0 < (int)uVar1) {
          lVar4 = 0;
          do {
            *(undefined1 *)(lVar4 + (longlong)pvVar3) =
                 *(undefined1 *)(lVar4 + *(longlong *)(param_1 + 8));
            lVar4 = lVar4 + 1;
          } while (lVar4 < (int)uVar1);
        }
      }
      thunk_FUN_1407864b8(*(undefined8 *)(param_1 + 8));
    }
    *(LPVOID *)(param_1 + 8) = pvVar3;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0x80000000;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | param_2 & 0x7fffffff;
    if (*(int *)(param_1 + 0x10) < (int)param_2) {
      param_2 = *(uint *)(param_1 + 0x10);
    }
    *(uint *)(param_1 + 0x10) = param_2;
  }
  if (param_3 != '\0') {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x14) * 2 >> 1;
  }
  return;
}


