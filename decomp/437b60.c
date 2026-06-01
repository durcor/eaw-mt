/* NebulaBehaviorClass::vfunc_6  RVA 0x437b60  size 893  params 0  ret undefined */


undefined8 NebulaBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  int iVar1;
  longlong lVar2;
  bool bVar3;
  bool bVar4;
  float fVar5;
  char cVar6;
  longlong *plVar7;
  longlong lVar8;
  ulonglong uVar9;
  undefined4 *puVar10;
  undefined8 *puVar11;
  ulonglong uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  undefined1 local_res10 [8];
  longlong local_b8;
  longlong lStack_b0;
  longlong local_a8;
  undefined8 local_98;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  float local_78;
  float local_74;
  undefined4 local_70;
  undefined4 local_6c;
  longlong *local_58;
  
  LocomotorCommonClass::vfunc_6();
  fVar5 = DAT_1407ffaf8;
  lVar2 = *(longlong *)(param_2 + 0xf0);
  fVar16 = *(float *)(lVar2 + 300) * DAT_1408007d4;
  fVar13 = fVar16 * DAT_140b0a344;
  fVar14 = *(float *)(lVar2 + 0x11c) - *(float *)(lVar2 + 0x120);
  fVar15 = DAT_1407ffaf8 /
           (fVar13 * DAT_140819c54 * fVar13 + fVar13 + DAT_1407ffaf8 +
           fVar13 * DAT_140819c50 * fVar13 * fVar13);
  fVar13 = (fVar14 * fVar16 + *(float *)(lVar2 + 0x124)) * DAT_140b0a344;
  *(float *)(lVar2 + 0x11c) = (fVar13 + fVar14) * fVar15 + *(float *)(lVar2 + 0x120);
  *(float *)(lVar2 + 0x124) = (*(float *)(lVar2 + 0x124) - fVar13 * fVar16) * fVar15;
  if (lVar2 != 0) {
    cVar6 = FUN_140380bb0(lVar2);
    if (cVar6 == '\0') {
      plVar7 = (longlong *)
               (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                         (*(longlong **)(param_1 + 0x28),1);
      uVar12 = 0;
      bVar3 = true;
      if (plVar7 != (longlong *)0x0) {
        cVar6 = (**(code **)(*plVar7 + 0x2b0))(plVar7);
        bVar3 = true;
        if (cVar6 != '\0') {
          bVar3 = false;
        }
      }
      lVar8 = (**(code **)(*DAT_140b15418 + 0x238))();
      uVar9 = uVar12;
      if (lVar8 != 0) {
        uVar9 = FUN_1405387f0(lVar8,8);
      }
      bVar4 = false;
      if ((uVar9 != 0) && (bVar3)) {
        local_a8 = 0;
        local_b8 = 0;
        lStack_b0 = 0;
        FUN_1405c8a30(&local_98);
        local_88 = *(undefined4 *)(param_2 + 0x78);
        local_84 = *(undefined4 *)(param_2 + 0x7c);
        local_80 = local_88;
        local_7c = local_84;
        puVar10 = (undefined4 *)FUN_14041c000(param_2 + 0x84,local_res10);
        local_90 = *puVar10;
        lVar8 = FUN_14041c000(param_2 + 0x84,local_res10);
        local_8c = *(undefined4 *)(lVar8 + 4);
        local_70 = *(undefined4 *)(param_2 + 0x50);
        local_78 = (float)DAT_140b0a320;
        local_58 = &local_b8;
        local_6c = 0x3fffff;
        local_98 = 0;
        local_74 = local_78;
        FUN_1405d7c00(uVar9,&local_98);
        if (lStack_b0 - local_b8 >> 2 != 0) {
          do {
            lVar8 = FUN_1402aca60(DAT_140b15418[3],*(undefined4 *)(local_b8 + uVar12 * 4));
            if (*(char *)(*(longlong *)(lVar8 + 0x298) + 0x57) != '\0') {
              puVar11 = (undefined8 *)(*(longlong *)(param_2 + 0x298) + 0xf8);
              if (0xf < *(ulonglong *)(*(longlong *)(param_2 + 0x298) + 0x110)) {
                puVar11 = (undefined8 *)*puVar11;
              }
              FUN_140025760("Object %s entered Nebula\n",puVar11);
              iVar1 = DAT_140b0a320;
              if (*(int *)(lVar2 + 0x104) == -1) {
                FUN_1404376f0(param_1,param_2);
                *(undefined4 *)(lVar2 + 0x120) = 0x3f800000;
              }
              bVar4 = true;
              *(int *)(lVar2 + 0x104) = iVar1;
              break;
            }
            uVar12 = (ulonglong)((int)uVar12 + 1);
          } while (uVar12 < (ulonglong)(lStack_b0 - local_b8 >> 2));
        }
        if (local_b8 != 0) {
          uVar12 = local_a8 - local_b8 & 0xfffffffffffffffc;
          if (0xfff < uVar12) {
            if (0x1f < (local_b8 - *(longlong *)(local_b8 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_b8 + -8),uVar12 + 0x27);
            }
          }
          thunk_FUN_1407864b8();
        }
        if (bVar4) {
          return 0x650000;
        }
      }
      iVar1 = *(int *)(lVar2 + 0x104);
      *(undefined4 *)(lVar2 + 0x104) = 0xffffffff;
      if (iVar1 != -1) {
        FUN_14039fb40(param_2);
        *(undefined4 *)(lVar2 + 0x120) = 0;
      }
    }
    else if (*(float *)(lVar2 + 0x120) <= fVar5 && fVar5 != *(float *)(lVar2 + 0x120)) {
      *(undefined4 *)(lVar2 + 0x11c) = 0x3f800000;
      *(undefined8 *)(lVar2 + 0x120) = 0x3f800000;
    }
  }
  return 0x650000;
}


