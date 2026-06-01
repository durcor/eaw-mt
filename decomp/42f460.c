/* AbilityCountdownBehaviorClass::FUN_14042f460  RVA 0x42f460  size 410  params 0  ret undefined */


void AbilityCountdownBehaviorClass::FUN_14042f460(longlong param_1,int param_2)

{
  int iVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  undefined8 local_28;
  undefined4 local_20;
  
  lVar7 = *(longlong *)(param_1 + 0x28);
  lVar9 = (longlong)param_2;
  lVar2 = *(longlong *)(lVar7 + 0x1e8);
  if ((lVar2 != 0) && (param_2 - 1U < 0x4c)) {
    lVar5 = lVar7;
    if (*(char *)(lVar7 + 0x348) != -1) {
      lVar5 = FUN_1404ec820(*(undefined8 *)(lVar7 + 0x110));
      lVar7 = *(longlong *)(param_1 + 0x28);
    }
    cVar3 = FUN_1403986b0(lVar7,param_2);
    if (cVar3 != '\0') {
      cVar3 = FUN_14043fa10(&DAT_140b2f2e0,param_2);
      if (cVar3 != '\0') {
        local_28 = 0;
        local_20 = 0;
        FUN_14038a350(lVar5,param_2,0,&local_28,0xffffffff,0x3fffff);
        lVar6 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58));
        lVar5 = 0;
        lVar7 = lVar5;
        if (lVar6 != 0) {
          lVar7 = *(longlong *)(lVar6 + 0x68);
        }
        if ((*(longlong *)(lVar7 + (lVar9 + 199) * 0x10) != 0) && (DAT_140b155c4 == '\0')) {
          iVar1 = *(int *)(*(longlong *)(param_1 + 0x28) + 0x58);
          iVar4 = FUN_140294a70(&DAT_140a16fd0);
          if (iVar4 == iVar1) {
            lVar7 = FUN_140294bc0(&DAT_140a16fd0,iVar1);
            if (lVar7 != 0) {
              lVar5 = *(longlong *)(lVar7 + 0x68);
            }
            FUN_1402d5290(&DAT_140b27e60,*(undefined8 *)(lVar5 + (lVar9 + 0x161) * 0x10),0,0,0);
          }
        }
        *(undefined1 *)(lVar9 + 0x270 + lVar2) = 0;
        FUN_14042fb70(param_1,param_2);
      }
      lVar7 = *(longlong *)(param_1 + 0x28) + 0x38;
      if (*(longlong *)(param_1 + 0x28) == 0) {
        lVar7 = 0;
      }
      uVar8 = FUN_140058570();
      FUN_140220ed0(uVar8,lVar7,0x29);
    }
  }
  return;
}


