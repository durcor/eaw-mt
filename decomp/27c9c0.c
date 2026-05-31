/* FUN_14027c9c0  RVA 0x27c9c0  size 425  params 0  ret undefined */


void FUN_14027c9c0(uint *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  DWORD DVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  undefined4 *puVar6;
  ulonglong uVar7;
  int iVar8;
  uint uVar9;
  _SYSTEMTIME local_2d8;
  undefined1 local_2c8 [143];
  undefined4 uStack_239;
  undefined1 local_234 [268];
  undefined1 local_128 [272];
  
  uVar9 = *param_1;
  puVar5 = (undefined1 *)thunk_FUN_140769c58((longlong)DAT_140a16d40);
  *puVar5 = 0;
  DVar3 = GetCurrentDirectoryA(0x104,(LPSTR)((longlong)&uStack_239 + 1));
  if (DVar3 - 1 < 0x103) {
    puVar2 = &uStack_239;
    do {
      puVar6 = puVar2;
      puVar2 = (undefined4 *)((longlong)puVar6 + 1);
    } while (*(char *)((longlong)puVar6 + 1) != '\0');
    *(undefined4 *)((longlong)puVar6 + 1) = DAT_14085ac90;
    *(undefined1 *)((longlong)puVar6 + 5) = DAT_14085ac94;
    CreateDirectoryA((LPCSTR)((longlong)&uStack_239 + 1),(LPSECURITY_ATTRIBUTES)0x0);
  }
  local_2d8.wYear = 0;
  local_2d8.wMonth = 0;
  local_2d8.wDayOfWeek = 0;
  local_2d8.wDay = 0;
  local_2d8.wHour = 0;
  local_2d8.wMinute = 0;
  local_2d8.wSecond = 0;
  local_2d8.wMilliseconds = 0;
  GetLocalTime(&local_2d8);
  iVar8 = 0;
  DAT_140a7c080 = 0;
  DAT_140b2c37b = 1;
  do {
    iVar1 = DAT_140a16d40;
    *puVar5 = 0;
    FUN_1402b4d70(*(undefined8 *)(DAT_140b15418 + 0x18),
                  param_1 + (ulonglong)(uVar9 & 0xff) * 0x28 + 0x324,puVar5,iVar1);
    uVar7 = 0xffffffffffffffff;
    do {
      uVar7 = uVar7 + 1;
    } while (puVar5[uVar7] != '\0');
    if (uVar7 != 0) {
      uVar4 = FUN_140294a70(&DAT_140a16fd0);
      FUN_140048ec0(local_128,"log\\Sync_p%d_frame%d.txt",uVar4,
                    param_1[(ulonglong)(uVar9 & 0xff) * 3 + 0x15]);
      FUN_1402130d0(local_2c8,local_128);
      FileClass::vfunc_1(local_2c8,1);
      FileClass::vfunc_6(local_2c8,puVar5,uVar7 & 0xffffffff);
      FileClass::vfunc_4(local_2c8);
      FUN_1402132f0(local_2c8);
    }
    uVar9 = uVar9 - 1;
  } while ((-1 < (int)uVar9) && (iVar8 = iVar8 + 1, iVar8 < 0x80));
  DAT_140b2c37b = 0;
  return;
}


