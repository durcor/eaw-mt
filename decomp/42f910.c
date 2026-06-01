/* AbilityCountdownBehaviorClass::vfunc_6  RVA 0x42f910  size 338  params 0  ret undefined */


undefined8 AbilityCountdownBehaviorClass::vfunc_6(longlong param_1,longlong param_2,int param_3)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  char cVar4;
  int iVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  char *pcVar8;
  uint uVar9;
  int *piVar11;
  int iVar12;
  undefined **local_48;
  int local_40;
  ulonglong uVar10;
  
  LocomotorCommonClass::vfunc_6();
  lVar3 = *(longlong *)(param_2 + 0x1e8);
  if (lVar3 != 0) {
    pcVar8 = (char *)(lVar3 + 0x270);
    piVar11 = (int *)(lVar3 + 8);
    iVar12 = param_3 - *(int *)(lVar3 + 0x2c0);
    uVar10 = 0;
    do {
      iVar5 = *piVar11;
      if (*pcVar8 == '\0') {
        if (0 < iVar5) {
          iVar5 = iVar5 - iVar12;
          if (iVar5 < 1) {
            iVar5 = 0;
          }
          *piVar11 = iVar5;
          if (iVar5 == 0) {
            cVar4 = FUN_14039b480(*(undefined8 *)(param_1 + 0x28),uVar10,1);
            if (cVar4 != '\0') {
              local_48 = AbilitySignalDataClass::vftable;
              uVar7 = *(longlong *)(param_1 + 0x28) + 0x38;
              if (*(longlong *)(param_1 + 0x28) == 0) {
                uVar7 = 0;
              }
              local_40 = (int)uVar10;
              uVar6 = FUN_140058570();
              FUN_140220ed0(uVar6,uVar7,0x2c,&local_48);
              local_48 = SignalDataClass::vftable;
            }
          }
        }
      }
      else {
        iVar2 = piVar11[0x4d];
        if (iVar5 < iVar2) {
          iVar1 = iVar5 + iVar12;
          iVar5 = iVar2;
          if (iVar1 < iVar2) {
            iVar5 = iVar1;
          }
          *piVar11 = iVar5;
        }
        if (iVar5 == iVar2) {
          FUN_14042f460(param_1,uVar10);
        }
      }
      uVar9 = (int)uVar10 + 1;
      uVar10 = (ulonglong)uVar9;
      piVar11 = piVar11 + 1;
      pcVar8 = pcVar8 + 1;
    } while ((int)uVar9 < 0x4d);
    *(int *)(lVar3 + 0x2c0) = param_3;
  }
  return 0x650000;
}


