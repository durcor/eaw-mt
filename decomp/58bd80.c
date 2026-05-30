/* DamageTrackingBehaviorClass::vfunc_6  RVA 0x58bd80  size 550  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 DamageTrackingBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  int iVar3;
  longlong lVar4;
  int iVar5;
  ulonglong uVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  uint uVar9;
  float fVar10;
  float fVar11;
  undefined **local_res10;
  
  LocomotorCommonClass::vfunc_6();
  fVar10 = (float)FUN_1403727a0(*(undefined8 *)(param_2 + 0x298),param_2);
  fVar11 = (float)FUN_1403725f0(*(undefined8 *)(param_2 + 0x298),param_2);
  lVar1 = *(longlong *)(param_2 + 0x1a0);
  fVar10 = (fVar10 + fVar11) * _DAT_140b168fc;
  plVar7 = (longlong *)**(longlong **)(lVar1 + 0x18);
  if (plVar7 != *(longlong **)(lVar1 + 0x18)) {
    do {
      fVar11 = *(float *)((longlong)plVar7 + 0x14) - fVar10;
      *(float *)((longlong)plVar7 + 0x14) = fVar11;
      if (0.0 < fVar11) {
        puVar8 = (undefined8 *)*plVar7;
      }
      else {
        uVar9 = (*(uint *)(plVar7 + 2) ^ 0xdeadbeef) & 0x7fffffff;
        lVar4 = *(longlong *)(lVar1 + 0x28);
        iVar5 = (uVar9 % 0x1f31d) * 0x41a7 + (uVar9 / 0x1f31d) * -0xb14;
        iVar3 = iVar5 + 0x7fffffff;
        if (-1 < iVar5) {
          iVar3 = iVar5;
        }
        uVar6 = (longlong)iVar3 & *(ulonglong *)(lVar1 + 0x40);
        puVar8 = *(undefined8 **)(lVar4 + uVar6 * 0x10);
        if (*(undefined8 **)(lVar4 + 8 + uVar6 * 0x10) == plVar7) {
          if (puVar8 == plVar7) {
            uVar2 = *(undefined8 *)(lVar1 + 0x18);
            *(undefined8 *)(lVar4 + uVar6 * 0x10) = uVar2;
            *(undefined8 *)(lVar4 + 8 + uVar6 * 0x10) = uVar2;
          }
          else {
            *(longlong *)(lVar4 + 8 + uVar6 * 0x10) = plVar7[1];
          }
        }
        else if (puVar8 == plVar7) {
          *(longlong *)(lVar4 + uVar6 * 0x10) = *plVar7;
        }
        puVar8 = (undefined8 *)*plVar7;
        *(longlong *)(lVar1 + 0x20) = *(longlong *)(lVar1 + 0x20) + -1;
        *(undefined8 **)plVar7[1] = puVar8;
        puVar8[1] = plVar7[1];
        thunk_FUN_1407864b8(plVar7,0x18);
        if (*(longlong *)(lVar1 + 0x20) == 0) {
          if (DAT_140a10b50 == 0) {
            lVar4 = FUN_140769c58(0x78);
            if (lVar4 == 0) {
              DAT_140a10b50 = 0;
            }
            else {
              DAT_140a10b50 = FUN_140221820(lVar4);
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
          FUN_140220ed0(DAT_140a10b50,param_2 + 0x38,0x2d);
        }
      }
      plVar7 = puVar8;
    } while (puVar8 != *(undefined8 **)(lVar1 + 0x18));
  }
  iVar3 = *(int *)(param_1 + 0x34);
  fVar11 = (float)DAT_140b0a340;
  fVar10 = *(float *)(lVar1 + 8);
  *(undefined4 *)(lVar1 + 8) = 0;
  *(float *)(lVar1 + 0xc) = (fVar11 * fVar10) / (float)iVar3;
  return 0x650000;
}


