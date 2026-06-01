/* NebulaBehaviorClass::FUN_1404376f0  RVA 0x4376f0  size 653  params 0  ret undefined */


void NebulaBehaviorClass::FUN_1404376f0(undefined8 param_1,longlong param_2)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  HANDLE hHeap;
  int iVar4;
  ulonglong uVar5;
  longlong lVar6;
  undefined **local_res10;
  undefined8 local_68;
  undefined4 local_60;
  undefined **local_58;
  int local_50;
  undefined **local_48;
  longlong *local_40;
  undefined4 local_38;
  uint local_34;
  
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  local_40 = (longlong *)0x0;
  local_34 = 0;
  local_40 = (longlong *)thunk_FUN_140769c58(0x50);
  local_34 = local_34 & 0x8000000a | 10;
  *local_40 = param_2;
  local_38 = 1;
  cVar1 = FUN_14039b1a0(param_2);
  lVar6 = param_2;
  if (cVar1 != '\0') {
    lVar6 = *(longlong *)(param_2 + 0x2b0);
  }
  if (*(longlong *)(param_2 + 0x298) != 0) {
    local_58 = AbilitySignalDataClass::vftable;
    local_50 = 0;
    uVar5 = 0;
    do {
      iVar4 = (int)uVar5;
      cVar1 = FUN_1403986b0(param_2,uVar5);
      if (cVar1 != '\0') {
        cVar1 = FUN_14039b950(param_2,uVar5,1);
        if ((cVar1 != '\0') && (cVar1 = FUN_14043fa50(&DAT_140b2f2e0,uVar5), cVar1 != '\0')) {
          lVar2 = FUN_140769c58(0x268);
          if (lVar2 == 0) {
            plVar3 = (longlong *)0x0;
          }
          else {
            plVar3 = (longlong *)FUN_14042fee0(lVar2,0);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)*plVar3)(plVar3);
            }
          }
          local_68 = 0;
          local_60 = 0;
          FUN_1404305d0(plVar3,&local_48,uVar5,*(undefined4 *)(param_2 + 0x58),&local_68,1,0x3fffff)
          ;
          (**(code **)(*plVar3 + 0x20))(plVar3);
          uVar5 = lVar6 + 0x38;
          if (lVar6 == 0) {
            uVar5 = 0;
          }
          local_50 = iVar4;
          if (DAT_140a10b50 == 0) {
            lVar2 = FUN_140769c58(0x78);
            if (lVar2 == 0) {
              DAT_140a10b50 = 0;
            }
            else {
              DAT_140a10b50 = FUN_140221820(lVar2);
            }
            local_res10 = &PTR_vftable_140a10b48;
            if (DAT_140a7bc10 == DAT_140a7bc18) {
              FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res10);
            }
            else {
              *DAT_140a7bc10 = &PTR_vftable_140a10b48;
              DAT_140a7bc10 = DAT_140a7bc10 + 1;
            }
          }
          FUN_140220ed0(DAT_140a10b50,uVar5,0x2b,&local_58);
          (**(code **)(*plVar3 + 8))(plVar3);
        }
      }
      uVar5 = (ulonglong)(iVar4 + 1U);
    } while ((int)(iVar4 + 1U) < 0x4d);
    local_58 = SignalDataClass::vftable;
  }
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  if (local_40 != (longlong *)0x0) {
    if ((local_34 & 0x80000000) != 0) {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,local_40);
      return;
    }
    thunk_FUN_1407864b8();
  }
  return;
}


