/* FUN_1402d5290  RVA 0x2d5290  size 142  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_1402d5290(undefined8 param_1,longlong param_2,undefined4 param_3,undefined1 param_4,uint param_5
             )

{
  undefined4 uVar1;
  DWORD DVar2;
  ulonglong uVar3;
  undefined1 local_18 [16];
  
  if (param_2 != 0) {
    uVar1 = FUN_1402d5440(param_1,param_2,0,local_18,0,0,param_3,param_4);
    if (0 < (int)param_5) {
      uVar3 = (ulonglong)param_5;
      do {
        DVar2 = timeGetTime();
        FUN_1402d72c0(&DAT_140b27e60,DVar2 - _DAT_140a7bec0,DAT_140b0a320,1);
        uVar3 = uVar3 - 1;
      } while (uVar3 != 0);
    }
    return uVar1;
  }
  return 0;
}


