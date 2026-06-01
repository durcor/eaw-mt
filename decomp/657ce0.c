/* UnitAIBehaviorClass::FUN_140657ce0  RVA 0x657ce0  size 202  params 0  ret undefined */


void UnitAIBehaviorClass::FUN_140657ce0(undefined8 param_1,longlong *param_2,longlong param_3)

{
  longlong lVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  
  if (param_2 != (longlong *)0x0) {
    lVar6 = (longlong)*(int *)(param_3 + 0x48);
    lVar4 = (**(code **)(*param_2 + 0x10))(param_2,0x12);
    if (lVar4 != 0) {
      iVar7 = 0;
      iVar3 = FUN_1404f5900(lVar4);
      if (0 < iVar3) {
        do {
          lVar5 = FUN_1404f58e0(lVar4,iVar7);
          iVar7 = iVar7 + 1;
          *(undefined4 *)(*(longlong *)(lVar5 + 0x40) + lVar6 * 4) = *(undefined4 *)(lVar5 + 0x1c);
          lVar1 = *(longlong *)(lVar5 + 0x58);
          *(undefined4 *)(lVar1 + lVar6 * 8) = *(undefined4 *)(lVar5 + 0x20);
          *(undefined4 *)(lVar1 + 4 + lVar6 * 8) = *(undefined4 *)(lVar5 + 0x24);
          lVar1 = *(longlong *)(lVar5 + 0x70);
          *(undefined4 *)(lVar1 + lVar6 * 0xc) = *(undefined4 *)(lVar5 + 0x28);
          lVar2 = *(longlong *)(lVar5 + 0xa0);
          *(undefined4 *)(lVar1 + 4 + lVar6 * 0xc) = *(undefined4 *)(lVar5 + 0x2c);
          *(undefined4 *)(lVar1 + 8 + lVar6 * 0xc) = *(undefined4 *)(lVar5 + 0x30);
          *(undefined4 *)(lVar2 + lVar6 * 4) = *(undefined4 *)(lVar5 + 0x38);
          iVar3 = FUN_1404f5900(lVar4);
        } while (iVar7 < iVar3);
      }
    }
  }
  return;
}


