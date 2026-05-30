/* LandMovementCoordinatorClass::vfunc_1  RVA 0x4f77e0  size 863  params 0  ret undefined */


void LandMovementCoordinatorClass::vfunc_1
               (longlong *param_1,longlong param_2,int param_3,ulonglong param_4)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  longlong *plVar4;
  undefined8 *puVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  uint uVar9;
  ulonglong uVar10;
  ulonglong uVar12;
  ulonglong uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined8 local_28;
  undefined4 local_20;
  ulonglong uVar11;
  
  if (param_3 == 1) {
    uVar12 = 0;
    uVar10 = param_2 - 0x38;
    if (param_2 == 0) {
      uVar10 = uVar12;
    }
    if (((uVar10 != 0) && (uVar10 != param_4)) && (param_1[0xd] == uVar10)) {
      param_1[0xd] = param_4;
      lVar8 = param_1[2];
      uVar13 = uVar12;
      if (param_1[3] - lVar8 >> 3 != 0) {
        do {
          lVar8 = *(longlong *)(lVar8 + uVar13 * 8);
          if ((lVar8 != 0) && (iVar1 = FUN_14041afb0(lVar8), uVar11 = uVar12, iVar1 != 0)) {
            do {
              lVar6 = FUN_14041aec0(lVar8,uVar11);
              if (*(ulonglong *)(lVar6 + 0x28) == uVar10) {
                *(ulonglong *)(lVar6 + 0x28) = param_4;
              }
              uVar9 = (int)uVar11 + 1;
              uVar11 = (ulonglong)uVar9;
              uVar2 = FUN_14041afb0(lVar8);
            } while (uVar9 < uVar2);
          }
          lVar8 = param_1[2];
          uVar13 = (ulonglong)((int)uVar13 + 1);
        } while (uVar13 < (ulonglong)(param_1[3] - lVar8 >> 3));
      }
      uVar7 = FUN_140058570();
      FUN_140220eb0(uVar7,uVar10 + 0x38,param_1,0x10);
      uVar7 = FUN_140058570();
      FUN_140220eb0(uVar7,uVar10 + 0x38,param_1,0x1a);
      uVar7 = FUN_140058570();
      FUN_140220eb0(uVar7,uVar10 + 0x38,param_1,1);
      uVar7 = FUN_140058570();
      FUN_140220eb0(uVar7,uVar10 + 0x38,param_1,0x2e);
      if (param_4 != 0) {
        uVar7 = FUN_140058570();
        FUN_140220e90(uVar7,param_4 + 0x38,param_1,0x10);
        uVar7 = FUN_140058570();
        FUN_140220e90(uVar7,param_4 + 0x38,param_1,0x1a);
        uVar7 = FUN_140058570();
        FUN_140220e90(uVar7,param_4 + 0x38,param_1,1);
        uVar7 = FUN_140058570();
        FUN_140220e90(uVar7,param_4 + 0x38,param_1,0x2e);
      }
    }
  }
  else if (((param_3 == 0x10) || (param_3 == 0x1a)) || (param_3 == 0x2e)) {
    uVar12 = 0;
    uVar10 = param_2 - 0x38;
    if (param_2 == 0) {
      uVar10 = uVar12;
    }
    (**(code **)(*param_1 + 0x20))(param_1,uVar10);
    if (uVar10 == param_1[0xd]) {
      uVar14 = 0;
      uVar15 = 0;
      uVar16 = 0;
      if (param_1[6] - param_1[5] >> 3 != 0) {
        puVar3 = (undefined4 *)FUN_140583150(param_1 + 8,&local_28,*(undefined8 *)param_1[5]);
        uVar14 = *puVar3;
        uVar15 = puVar3[1];
        uVar16 = puVar3[2];
      }
      param_1[0xd] = 0;
      *(undefined4 *)(param_1 + 8) = uVar14;
      *(undefined4 *)((longlong)param_1 + 0x44) = uVar15;
      *(undefined4 *)(param_1 + 9) = uVar16;
      param_1[0xe] = 0;
      *(undefined4 *)((longlong)param_1 + 100) = 1;
      lVar8 = param_1[2];
      if (param_1[3] - lVar8 >> 3 != 0) {
        do {
          lVar8 = *(longlong *)(lVar8 + uVar12 * 8);
          if ((lVar8 != 0) && (iVar1 = FUN_14041afb0(lVar8), iVar1 != 0)) {
            puVar3 = (undefined4 *)FUN_14041a580(lVar8);
            local_28 = 0;
            local_20 = 0;
            plVar4 = (longlong *)FUN_14041b6a0(lVar8);
            iVar1 = (int)((plVar4[1] - *plVar4) / 0x34);
            if (iVar1 == 0) {
              FUN_14041a9b0(lVar8 + 0x10,&local_28);
            }
            else {
              puVar5 = (undefined8 *)FUN_14054fc40(plVar4,iVar1 + -1);
              local_28 = *puVar5;
              local_20 = *(undefined4 *)(puVar5 + 1);
            }
            *(undefined8 *)(puVar3 + 10) = 0;
            *(undefined8 *)(puVar3 + 0xc) = 0;
            puVar3[9] = 1;
            *puVar3 = (undefined4)local_28;
            puVar3[1] = local_28._4_4_;
            puVar3[2] = local_20;
          }
          lVar8 = param_1[2];
          uVar12 = (ulonglong)((int)uVar12 + 1);
        } while (uVar12 < (ulonglong)(param_1[3] - lVar8 >> 3));
      }
    }
  }
  return;
}


