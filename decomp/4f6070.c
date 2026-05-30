/* UnitAIBehaviorClass::vfunc_6  RVA 0x4f6070  size 977  params 0  ret undefined */


undefined8 UnitAIBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  longlong lVar1;
  longlong *plVar2;
  char cVar3;
  longlong lVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  uint uVar7;
  ulonglong uVar8;
  longlong lVar9;
  int iVar10;
  
  LocomotorCommonClass::vfunc_6();
  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x108);
  cVar3 = (**(code **)(*DAT_140b15418 + 0xf0))();
  if (cVar3 != '\0') {
    uVar6 = 0;
    iVar10 = 0;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar10 = (int)DAT_140b15418[2];
    }
    if ((*(int *)(lVar4 + 0x20) <= iVar10) && (DAT_140b31900 != 0)) {
      lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x108);
      if ((lVar1 != 0) && (*(int *)(lVar1 + 0x18) == 0)) {
        FUN_1406537d0();
        FUN_1404f4c60(param_1);
        FUN_1404f5cb0(lVar1 + 0x50,DAT_140a16fd8 - DAT_140a16fd0 >> 3,1);
        uVar5 = uVar6;
        uVar8 = uVar6;
        if (0 < *(int *)(lVar1 + 0x60)) {
          do {
            uVar7 = (int)uVar8 + 1;
            *(undefined1 *)(uVar5 + *(longlong *)(lVar1 + 0x58)) = 0;
            uVar5 = uVar5 + 1;
            uVar8 = (ulonglong)uVar7;
          } while ((int)uVar7 < *(int *)(lVar1 + 0x60));
        }
      }
      if (((((((*(float *)(param_2 + 0x248) == *(float *)(lVar4 + 0x68)) &&
              (*(float *)(param_2 + 0x24c) == *(float *)(lVar4 + 0x6c))) &&
             (*(float *)(param_2 + 0x250) == *(float *)(lVar4 + 0x70))) &&
            ((*(float *)(param_2 + 0x254) == *(float *)(lVar4 + 0x74) &&
             (*(float *)(param_2 + 600) == *(float *)(lVar4 + 0x78))))) &&
           ((*(float *)(param_2 + 0x25c) == *(float *)(lVar4 + 0x7c) &&
            ((*(float *)(param_2 + 0x260) == *(float *)(lVar4 + 0x80) &&
             (*(float *)(param_2 + 0x264) == *(float *)(lVar4 + 0x84))))))) &&
          (*(float *)(param_2 + 0x268) == *(float *)(lVar4 + 0x88))) &&
         (((*(float *)(param_2 + 0x26c) == *(float *)(lVar4 + 0x8c) &&
           (*(float *)(param_2 + 0x270) == *(float *)(lVar4 + 0x90))) &&
          (*(float *)(param_2 + 0x274) == *(float *)(lVar4 + 0x94))))) {
        *(int *)(lVar4 + 0x20) = *(int *)(lVar4 + 0x20) + 300;
      }
      else {
        FUN_140659760(DAT_140b31900,param_2);
        *(int *)(lVar4 + 0x20) = *(int *)(lVar4 + 0x20) + 300;
        *(undefined4 *)(lVar4 + 0x68) = *(undefined4 *)(param_2 + 0x248);
        *(undefined4 *)(lVar4 + 0x6c) = *(undefined4 *)(param_2 + 0x24c);
        *(undefined4 *)(lVar4 + 0x70) = *(undefined4 *)(param_2 + 0x250);
        *(undefined4 *)(lVar4 + 0x74) = *(undefined4 *)(param_2 + 0x254);
        *(undefined4 *)(lVar4 + 0x78) = *(undefined4 *)(param_2 + 600);
        *(undefined4 *)(lVar4 + 0x7c) = *(undefined4 *)(param_2 + 0x25c);
        *(undefined4 *)(lVar4 + 0x80) = *(undefined4 *)(param_2 + 0x260);
        *(undefined4 *)(lVar4 + 0x84) = *(undefined4 *)(param_2 + 0x264);
        *(undefined4 *)(lVar4 + 0x88) = *(undefined4 *)(param_2 + 0x268);
        *(undefined4 *)(lVar4 + 0x8c) = *(undefined4 *)(param_2 + 0x26c);
        *(undefined4 *)(lVar4 + 0x90) = *(undefined4 *)(param_2 + 0x270);
        *(undefined4 *)(lVar4 + 0x94) = *(undefined4 *)(param_2 + 0x274);
      }
    }
    if ((*(char *)(*(longlong *)(param_1 + 0x28) + 0x333) != -1) &&
       ((*(char *)(lVar4 + 0x38) != '\0' ||
        (lVar4 = FUN_1403973b0(), *(char *)(lVar4 + 0x6a) != '\0')))) {
      FUN_1404f6470(param_1);
    }
    plVar2 = DAT_140b15418;
    lVar4 = *(longlong *)(param_1 + 0x28);
    lVar1 = *(longlong *)(lVar4 + 0x108);
    if ((DAT_140a284c4 == '\0') ||
       ((*(char *)(lVar4 + 0x34b) == -1 && (*(char *)(lVar4 + 0x348) == -1)))) {
      uVar5 = uVar6;
      if (0 < *(int *)(lVar1 + 0x60)) {
        do {
          uVar7 = (int)uVar5 + 1;
          *(undefined1 *)(uVar6 + *(longlong *)(lVar1 + 0x58)) = 0;
          uVar6 = uVar6 + 1;
          uVar5 = (ulonglong)uVar7;
        } while ((int)uVar7 < *(int *)(lVar1 + 0x60));
      }
    }
    else {
      uVar5 = uVar6;
      if (0 < *(int *)(lVar1 + 0x60)) {
        do {
          if ((int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3) <= (int)uVar6) {
            return 0x650000;
          }
          lVar4 = FUN_140294bc0(&DAT_140a16fd0,uVar6);
          if (lVar4 != 0) {
            cVar3 = FUN_14035f470(plVar2,*(undefined4 *)(lVar4 + 0x4c),
                                  *(undefined8 *)(param_1 + 0x28),0);
            lVar9 = *(longlong *)(lVar1 + 0x58);
            if (cVar3 == '\0') {
              if ((*(char *)(lVar9 + uVar5) != '\0') && (DAT_140b31900 != 0)) {
                FUN_140657db0(DAT_140b31900,*(undefined8 *)(param_1 + 0x28),lVar4);
                lVar9 = *(longlong *)(lVar1 + 0x58);
              }
              *(undefined1 *)(uVar5 + lVar9) = 0;
            }
            else {
              if ((*(char *)(lVar9 + uVar5) == '\0') && (DAT_140b31900 != 0)) {
                FUN_140657ce0(DAT_140b31900,*(undefined8 *)(param_1 + 0x28),lVar4);
                lVar9 = *(longlong *)(lVar1 + 0x58);
              }
              *(undefined1 *)(uVar5 + lVar9) = 1;
            }
          }
          uVar7 = (int)uVar6 + 1;
          uVar6 = (ulonglong)uVar7;
          uVar5 = uVar5 + 1;
        } while ((int)uVar7 < *(int *)(lVar1 + 0x60));
      }
    }
  }
  return 0x650000;
}


