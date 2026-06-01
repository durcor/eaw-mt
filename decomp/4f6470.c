/* UnitAIBehaviorClass::FUN_1404f6470  RVA 0x4f6470  size 1129  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UnitAIBehaviorClass::FUN_1404f6470(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  float *pfVar8;
  undefined8 uVar9;
  longlong *plVar10;
  longlong lVar11;
  int iVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  int local_res8 [2];
  undefined8 local_b8;
  ulonglong uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined4 local_98;
  undefined4 local_94;
  longlong local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined8 local_6c;
  undefined8 local_64;
  undefined4 local_5c;
  undefined4 local_58;
  
  lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x108);
  if ((lVar11 != 0) && (*(longlong *)(lVar11 + 0x1f0) != 0)) {
    local_res8[0] = -1;
    lVar5 = FUN_14041a2e0(*(undefined8 *)(lVar11 + 0x1f0),local_res8,0xffffffff);
    if ((lVar5 != 0) &&
       ((*(char *)(*(longlong *)(param_1 + 0x28) + 0x348) == -1 || (*(char *)(lVar5 + 0x348) == -1))
       )) {
      iVar12 = 0;
      iVar3 = iVar12;
      if (DAT_140b15418 != 0) {
        iVar3 = *(int *)(DAT_140b15418 + 0x10);
      }
      if ((*(int *)(lVar1 + 0x34) <= iVar3) ||
         (lVar6 = FUN_1403973b0(*(longlong *)(param_1 + 0x28)), *(char *)(lVar6 + 0x6a) != '\0')) {
        if (DAT_140b15418 != 0) {
          iVar12 = *(int *)(DAT_140b15418 + 0x10);
        }
        iVar3 = 0;
        if (iVar12 < *(int *)(lVar1 + 0x34)) {
          lVar6 = FUN_1403973b0(*(undefined8 *)(param_1 + 0x28));
          if (*(char *)(lVar6 + 0x6a) != '\0') {
            uVar9 = *(undefined8 *)(param_1 + 0x28);
            uVar7 = FUN_1403973b0(uVar9);
            fVar13 = (float)FUN_140370f00(uVar7,uVar9);
            lVar6 = FUN_1403973b0(*(undefined8 *)(param_1 + 0x28));
            if (0.0 < *(float *)(lVar6 + 900)) {
              lVar6 = FUN_1403973b0(*(undefined8 *)(param_1 + 0x28));
              fVar13 = (fVar13 * fVar13) / (*(float *)(lVar6 + 900) * DAT_1408007d4);
            }
            if ((*(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8) != 0) &&
               ((lVar6 = FUN_1405569b0(), lVar6 == 0 ||
                (pfVar8 = (float *)FUN_14054fe50((longlong *)(lVar6 + 0x18),
                                                 (int)((*(longlong *)(lVar6 + 0x20) -
                                                       *(longlong *)(lVar6 + 0x18)) / 0x34) + -1),
                fVar15 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x7c) - pfVar8[1],
                fVar14 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x78) - *pfVar8,
                fVar15 * fVar15 + fVar14 * fVar14 <= fVar13 * fVar13)))) {
              if (DAT_140b15418 != 0) {
                iVar3 = *(int *)(DAT_140b15418 + 0x10);
              }
              *(undefined1 *)(lVar1 + 0x24) = 0;
              *(int *)(lVar1 + 0x34) = iVar3 + 0x96;
            }
          }
        }
        else {
          if (DAT_140b15418 != 0) {
            iVar3 = *(int *)(DAT_140b15418 + 0x10);
          }
          *(undefined1 *)(lVar1 + 0x24) = 0;
          *(int *)(lVar1 + 0x34) = iVar3 + 0x96;
        }
        if (*(char *)(lVar1 + 0x24) == '\0') {
          FUN_1404f5390(param_1,lVar5,local_res8[0]);
        }
        plVar10 = *(longlong **)(param_1 + 0x28);
        if ((char)plVar10[0x69] != -1) {
          uVar9 = (**(code **)(*plVar10 + 0x10))(plVar10,0x16);
          plVar10 = (longlong *)FUN_140405870(uVar9);
        }
        uVar9 = FUN_1403973b0(plVar10);
        fVar13 = (float)FUN_1403749d0(uVar9,plVar10);
        fVar15 = *(float *)(lVar1 + 0x2c) - *(float *)(*(longlong *)(param_1 + 0x28) + 0x7c);
        fVar14 = *(float *)(lVar1 + 0x28) - *(float *)(*(longlong *)(param_1 + 0x28) + 0x78);
        bVar2 = fVar13 * fVar13 <= fVar15 * fVar15 + fVar14 * fVar14;
        lVar6 = FUN_1403973b0();
        if (((*(char *)(lVar6 + 0x6a) != '\0') || (bVar2)) || (*(char *)(lVar1 + 0x24) == '\0')) {
          lVar11 = FUN_1403973b0(*(undefined8 *)(param_1 + 0x28));
          if ((*(char *)(lVar11 + 0x6a) == '\0') && (!bVar2)) {
            return;
          }
          if (*(char *)(lVar1 + 0x24) != '\0') {
            return;
          }
          lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
          if (lVar11 == 0) {
            return;
          }
          if (*(longlong *)(lVar11 + 0x1f0) == 0) {
            return;
          }
          *(undefined1 *)(lVar1 + 0x24) = 1;
          uVar4 = FUN_14041b470(*(undefined8 *)(lVar11 + 0x1f0));
          if (1 < uVar4) {
            FUN_140424850(*(undefined8 *)(lVar11 + 0x1f0),*(undefined8 *)(param_1 + 0x28));
          }
          local_a8 = CONCAT44(_UNK_140800854,_DAT_140800850);
          uStack_a0 = CONCAT44(_UNK_14080085c,_UNK_140800858);
          uVar9 = *(undefined8 *)(lVar11 + 0x1f0);
          local_b8 = *(undefined8 *)(lVar1 + 0x28);
          uStack_b0 = (ulonglong)*(uint *)(lVar1 + 0x30);
          local_98 = 0x5d5e0b6b;
          local_90 = 0;
          local_88 = 0;
          local_80 = 0;
          local_78 = 0;
          local_6c = 0x3f;
          local_64 = 0x5d5e0b6b;
          local_5c = 0;
          local_58 = 0x1000001;
          local_94 = 1;
          local_70 = 4;
        }
        else {
          if (*(longlong *)(lVar11 + 0x1f0) == 0) {
            return;
          }
          *(undefined1 *)(lVar1 + 0x24) = 0;
          uVar4 = FUN_14041b470(*(undefined8 *)(lVar11 + 0x1f0));
          if (1 < uVar4) {
            FUN_140424850(*(undefined8 *)(lVar11 + 0x1f0),*(undefined8 *)(param_1 + 0x28));
          }
          local_b8 = 0;
          uStack_b0 = 0;
          local_a8 = CONCAT44(_UNK_140800854,_DAT_140800850);
          uStack_a0 = CONCAT44(_UNK_14080085c,_UNK_140800858);
          local_98 = 0x5d5e0b6b;
          local_80 = 0;
          local_78 = 0;
          local_70 = 0;
          local_6c = 0x3f;
          local_64 = 0x5d5e0b6b;
          local_5c = 0;
          local_58 = 0x1000001;
          local_94 = 3;
          local_90 = lVar5;
          if (local_res8[0] == -1) {
            uVar9 = *(undefined8 *)(lVar11 + 0x1f0);
            local_88 = 0;
          }
          else {
            local_88 = FUN_140396cb0(lVar5);
            uVar9 = *(undefined8 *)(lVar11 + 0x1f0);
          }
        }
        FUN_14041fd30(uVar9,&local_b8);
      }
    }
  }
  return;
}


