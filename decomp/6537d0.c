/* FUN_1406537d0  RVA 0x6537d0  size 636  params 0  ret undefined */


void FUN_1406537d0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  int iVar10;
  int iVar11;
  
  lVar5 = (**(code **)(*param_2 + 0x10))(param_2,0x12);
  if (lVar5 != 0) {
    iVar10 = 0;
    iVar4 = FUN_140294df0(param_2);
    if (0 < iVar4) {
      do {
        lVar6 = FUN_140396cb0(param_2,iVar10);
        if ((*(int *)(*(longlong *)(lVar6 + 0x20) + 0x48) - 5U < 6) &&
           (*(char *)(*(longlong *)(lVar6 + 0x20) + 0x4c) != '\0')) {
          lVar7 = FUN_140769c58(0xb8);
          lVar6 = 0;
          if (lVar7 != 0) {
            lVar6 = FUN_140653100(lVar7);
          }
          *(longlong **)(lVar6 + 0x10) = param_2;
          *(int *)(lVar6 + 0x18) = iVar10;
          FUN_1404f4820(lVar5,lVar6);
        }
        iVar10 = iVar10 + 1;
        iVar4 = FUN_140294df0(param_2);
      } while (iVar10 < iVar4);
    }
    iVar4 = FUN_1404f5900(lVar5);
    if (iVar4 == 0) {
      lVar7 = FUN_140769c58(0xb8);
      lVar6 = 0;
      if (lVar7 != 0) {
        lVar6 = FUN_140653100(lVar7);
      }
      *(longlong **)(lVar6 + 0x10) = param_2;
      FUN_1404f4820(lVar5,lVar6);
    }
    if (*(char *)((longlong)param_2 + 0x337) == -1) {
      lVar6 = FUN_140396380(param_2);
    }
    else {
      lVar6 = param_2[0x53];
    }
    *(ulonglong *)(param_1 + 200) = *(ulonglong *)(param_1 + 200) | *(ulonglong *)(lVar6 + 0x1648);
    lVar6 = FUN_140294bc0(&DAT_140a16fd0,(int)param_2[0xb]);
    if (lVar6 != 0) {
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,(int)param_2[0xb]);
      FUN_14020a9b0(*(undefined8 *)
                     (*(longlong *)(param_1 + 0x88) + (longlong)*(int *)(lVar6 + 0x48) * 8),
                    param_2 + 3,1);
    }
    iVar4 = 0;
    if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
      do {
        lVar6 = FUN_140294bc0(&DAT_140a16fd0,iVar4);
        if ((lVar6 != 0) && (cVar3 = FUN_1404f57d0(lVar5,lVar6), cVar3 != '\0')) {
          lVar7 = (longlong)*(int *)(lVar6 + 0x48);
          lVar6 = (**(code **)(*param_2 + 0x10))(param_2,0x12);
          if (lVar6 != 0) {
            iVar11 = 0;
            iVar10 = FUN_1404f5900(lVar6);
            if (0 < iVar10) {
              do {
                lVar8 = FUN_1404f58e0(lVar6,iVar11);
                iVar11 = iVar11 + 1;
                *(undefined4 *)(*(longlong *)(lVar8 + 0x40) + lVar7 * 4) =
                     *(undefined4 *)(lVar8 + 0x1c);
                lVar1 = *(longlong *)(lVar8 + 0x58);
                *(undefined4 *)(lVar1 + lVar7 * 8) = *(undefined4 *)(lVar8 + 0x20);
                *(undefined4 *)(lVar1 + 4 + lVar7 * 8) = *(undefined4 *)(lVar8 + 0x24);
                lVar1 = *(longlong *)(lVar8 + 0x70);
                *(undefined4 *)(lVar1 + lVar7 * 0xc) = *(undefined4 *)(lVar8 + 0x28);
                lVar2 = *(longlong *)(lVar8 + 0xa0);
                *(undefined4 *)(lVar1 + 4 + lVar7 * 0xc) = *(undefined4 *)(lVar8 + 0x2c);
                *(undefined4 *)(lVar1 + 8 + lVar7 * 0xc) = *(undefined4 *)(lVar8 + 0x30);
                *(undefined4 *)(lVar2 + lVar7 * 4) = *(undefined4 *)(lVar8 + 0x38);
                iVar10 = FUN_1404f5900(lVar6);
              } while (iVar11 < iVar10);
            }
          }
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
    }
    uVar9 = FUN_140058570();
    FUN_140220e90(uVar9,param_2 + 7,param_1,0x28);
  }
  return;
}


