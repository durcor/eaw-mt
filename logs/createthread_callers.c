// CreateThread symbol: CreateThread @ 0x65 type=Function
//   called from FUN_0x786138 (site 0x78619d)

// Total CreateThread callers: 1

// ============================================================
// RVA: 0x786138  Size: 212 bytes
// ============================================================

/* Library Function - Single Match
    _beginthreadex
   
   Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release */

uintptr_t __cdecl
_beginthreadex(void *_Security,uint _StackSize,_StartAddress *_StartAddress,void *_ArgList,
              uint _InitFlag,uint *_ThrdAddr)

{
  DWORD DVar1;
  ulong *puVar2;
  __acrt_thread_parameter *lpParameter;
  HANDLE pvVar3;
  uint local_res18 [4];
  
  if (_StartAddress == (_StartAddress *)0x0) {
    puVar2 = __doserrno();
    *puVar2 = 0x16;
    FUN_1407711ac();
    return 0;
  }
  lpParameter = create_thread_parameter(_StartAddress,_ArgList);
  if (lpParameter != (__acrt_thread_parameter *)0x0) {
    pvVar3 = CreateThread(_Security,(ulonglong)_StackSize,FUN_14078600c,lpParameter,_InitFlag,
                          local_res18);
    if (pvVar3 != (HANDLE)0x0) {
      if (_ThrdAddr != (uint *)0x0) {
        *_ThrdAddr = local_res18[0];
      }
      lpParameter = (__acrt_thread_parameter *)0x0;
      goto LAB_1407861ba;
    }
    DVar1 = GetLastError();
    __acrt_errno_map_os_error(DVar1);
  }
  pvVar3 = (HANDLE)0x0;
LAB_1407861ba:
  if (lpParameter != (__acrt_thread_parameter *)0x0) {
    if (*(HANDLE *)(lpParameter + 0x10) != (HANDLE)0x0) {
      CloseHandle(*(HANDLE *)(lpParameter + 0x10));
    }
    if (*(HMODULE *)(lpParameter + 0x18) != (HMODULE)0x0) {
      FreeLibrary(*(HMODULE *)(lpParameter + 0x18));
    }
    _free_base(lpParameter);
  }
  return (uintptr_t)pvVar3;
}



