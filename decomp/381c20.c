/* HardPointClass::vfunc_0  RVA 0x381c20  size 406  params 0  ret undefined */


undefined8 * HardPointClass::vfunc_0(undefined8 *param_1,ulonglong param_2)

{
  LPVOID pvVar1;
  HANDLE pvVar2;
  
  *(undefined4 *)(param_1 + 3) = 0xffffffff;
  param_1[2] = 0;
  *param_1 = vftable;
  param_1[4] = 0;
  *(undefined4 *)(param_1 + 5) = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0x18] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  *(undefined4 *)((longlong)param_1 + 0x8c) = 0;
  *(undefined8 *)((longlong)param_1 + 0x2c) = 0xffffffffffffffff;
  *(undefined8 *)((longlong)param_1 + 0x34) = 0xffffffffffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3c) = 0xffffffff;
  pvVar1 = (LPVOID)param_1[0xf];
  if (pvVar1 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x84) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar1);
    }
    else {
      pvVar2 = GetProcessHeap();
      HeapFree(pvVar2,0,pvVar1);
    }
    *(uint *)((longlong)param_1 + 0x84) = *(uint *)((longlong)param_1 + 0x84) & 0x80000000;
    param_1[0xf] = 0;
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  *(undefined8 *)((longlong)param_1 + 0x94) = 0xffffffffffffffff;
  *(undefined4 *)(param_1 + 0x12) = 0xffffffff;
  *(undefined8 *)((longlong)param_1 + 0x9c) = 0;
  *(undefined4 *)((longlong)param_1 + 0xa4) = 0;
  param_1[0x15] = 0;
  *(undefined1 *)(param_1 + 0x1a) = 0;
  *(undefined4 *)(param_1 + 0x19) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xcc) = 0;
  param_1[0x16] = 0;
  *(undefined4 *)(param_1 + 0x17) = 0;
  pvVar1 = (LPVOID)param_1[0xf];
  param_1[0xe] = DynamicVectorClass<int>::vftable;
  if (pvVar1 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x84) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar1);
    }
    else {
      pvVar2 = GetProcessHeap();
      HeapFree(pvVar2,0,pvVar1);
    }
    *(uint *)((longlong)param_1 + 0x84) = *(uint *)((longlong)param_1 + 0x84) & 0x80000000;
    param_1[0xf] = 0;
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  *param_1 = SignalListenerClass::vftable;
  FUN_14020a820(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0xd8);
  }
  return param_1;
}


