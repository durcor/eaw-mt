/* AsteroidFieldDamageBehaviorClass::vfunc_6  RVA 0x437310  size 791  params 0  ret undefined */


undefined8 AsteroidFieldDamageBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  longlong lVar1;
  bool bVar2;
  undefined4 uVar3;
  float fVar4;
  float fVar5;
  bool bVar6;
  char cVar7;
  int iVar8;
  longlong *plVar9;
  longlong lVar10;
  undefined8 uVar11;
  undefined4 *puVar12;
  ulonglong uVar13;
  float fVar14;
  undefined1 local_res10 [8];
  longlong local_d8;
  longlong lStack_d0;
  longlong local_c8;
  undefined8 local_b8;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  float local_98;
  float local_94;
  undefined4 local_90;
  undefined4 local_8c;
  longlong *local_78;
  
  LocomotorCommonClass::vfunc_6();
  lVar1 = *(longlong *)(param_2 + 0xf0);
  if (((lVar1 != 0) &&
      ((iVar8 = FUN_140397900(*(undefined8 *)(param_1 + 0x28),0), iVar8 - 1U < 2 || (iVar8 == 0x20))
      )) && ((plVar9 = (longlong *)
                       (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                                 (*(longlong **)(param_1 + 0x28),1), plVar9 == (longlong *)0x0 ||
             (((cVar7 = (**(code **)(*plVar9 + 0x1a8))(plVar9), cVar7 != '\0' &&
               (cVar7 = (**(code **)(*plVar9 + 0x1b8))(plVar9), cVar7 == '\0')) &&
              (cVar7 = (**(code **)(*plVar9 + 0x2b0))(plVar9), cVar7 == '\0')))))) {
    if ((*(longlong *)(param_1 + 0x40) == 0) &&
       (lVar10 = (**(code **)(*DAT_140b15418 + 0x238))(), lVar10 != 0)) {
      uVar11 = FUN_1405387f0(lVar10,8);
      *(undefined8 *)(param_1 + 0x40) = uVar11;
    }
    if ((DAT_140b16d64 != 0.0) && (DAT_140b16d60 != 0.0)) {
      bVar2 = false;
      if (*(longlong *)(param_1 + 0x40) != 0) {
        uVar13 = 0;
        local_c8 = 0;
        local_d8 = 0;
        lStack_d0 = 0;
        FUN_1405c8a30(&local_b8);
        local_a8 = *(undefined4 *)(param_2 + 0x78);
        local_a4 = *(undefined4 *)(param_2 + 0x7c);
        local_a0 = local_a8;
        local_9c = local_a4;
        puVar12 = (undefined4 *)FUN_14041c000(param_2 + 0x84,local_res10);
        local_b0 = *puVar12;
        lVar10 = FUN_14041c000(param_2 + 0x84,local_res10);
        local_ac = *(undefined4 *)(lVar10 + 4);
        local_90 = *(undefined4 *)(param_2 + 0x50);
        local_98 = (float)DAT_140b0a320;
        local_78 = &local_d8;
        local_8c = 0x3fffff;
        local_b8 = 0;
        local_94 = local_98;
        FUN_1405d7c00(*(undefined8 *)(param_1 + 0x40),&local_b8);
        uVar3 = DAT_1407ffaf8;
        bVar6 = false;
        if (lStack_d0 - local_d8 >> 2 != 0) {
          do {
            bVar2 = bVar6;
            lVar10 = FUN_1402aca60(DAT_140b15418[3],*(undefined4 *)(local_d8 + uVar13 * 4));
            fVar5 = DAT_140b16d64;
            fVar4 = DAT_140b16d60;
            if (*(char *)(*(longlong *)(lVar10 + 0x298) + 0x54) != '\0') {
              fVar14 = (float)FUN_1401ffbb0(&DAT_140a13e24,0,uVar3);
              if (fVar14 <= fVar5) {
                FUN_140436920(param_1,param_2,lVar10,fVar4);
              }
              bVar2 = true;
              *(int *)(lVar1 + 0x10c) = DAT_140b0a320;
            }
            uVar13 = (ulonglong)((int)uVar13 + 1);
            bVar6 = bVar2;
          } while (uVar13 < (ulonglong)(lStack_d0 - local_d8 >> 2));
        }
        if (local_d8 != 0) {
          uVar13 = local_c8 - local_d8 & 0xfffffffffffffffc;
          if (0xfff < uVar13) {
            if (0x1f < (local_d8 - *(longlong *)(local_d8 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_d8 + -8),uVar13 + 0x27);
            }
          }
          thunk_FUN_1407864b8();
        }
        if (bVar2) {
          return 0x650000;
        }
      }
      *(undefined4 *)(lVar1 + 0x10c) = 0xffffffff;
    }
  }
  return 0x650000;
}


