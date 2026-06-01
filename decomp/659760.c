/* UnitAIBehaviorClass::FUN_140659760  RVA 0x659760  size 139  params 0  ret undefined */


void UnitAIBehaviorClass::FUN_140659760(undefined8 param_1,longlong *param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  
  lVar2 = (**(code **)(*param_2 + 0x10))(param_2,0x12);
  if (lVar2 != 0) {
    iVar4 = 0;
    iVar1 = FUN_1404f5900(lVar2);
    if (0 < iVar1) {
      do {
        lVar3 = FUN_1404f58e0(lVar2,iVar4);
        if (lVar3 != 0) {
          FUN_140656d10(param_1,lVar3);
          FUN_140656a10(param_1,lVar3);
        }
        iVar4 = iVar4 + 1;
        iVar1 = FUN_1404f5900(lVar2);
      } while (iVar4 < iVar1);
    }
  }
  return;
}


