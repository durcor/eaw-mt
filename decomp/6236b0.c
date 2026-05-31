/* StarshipLocomotorBehaviorClass::vfunc_6  RVA 0x6236b0  size 2256  params 2  ret undefined */


/* WARNING: Unknown calling convention */

undefined8
StarshipLocomotorBehaviorClass::vfunc_6(LocomotorBehaviorClass *param_1,GameObjectClass *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  void *pvVar4;
  void *pvVar5;
  bool bVar6;
  int iVar7;
  longlong *plVar8;
  undefined4 *puVar9;
  longlong lVar10;
  undefined8 uVar11;
  GameObjectClass *pGVar12;
  float *pfVar13;
  float fVar14;
  float fVar15;
  uint uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float local_res10 [2];
  float local_a8;
  float local_a4;
  float local_a0;
  float local_98;
  float fStack_94;
  float local_90;
  float local_88;
  float fStack_84;
  float local_80;
  float local_7c;
  float local_78;
  float local_74;
  
  LocomotorCommonClass::vfunc_6();
  pvVar4 = param_2->locomotor_state;
  bVar6 = true;
  if (pvVar4 == (void *)0x0) {
    return 0x650000;
  }
  *(undefined4 *)((longlong)pvVar4 + 0x58) = 0;
  if (*(longlong *)&param_2->field_0x298 == 0) {
    return 0x650000;
  }
  local_a8 = param_2->pos_x;
  local_a4 = param_2->pos_y;
  local_a0 = param_2->pos_z;
  *(undefined4 *)((longlong)pvVar4 + 0x60) = 0;
  fVar14 = DAT_1408007f4;
  fVar18 = DAT_1408007dc;
  fVar17 = DAT_1408007d4;
  switch(*(undefined4 *)((longlong)pvVar4 + 0x48)) {
  case 0:
    pfVar13 = (float *)FUN_140623340(param_1);
    if (((*pfVar13 != local_a8) || (pfVar13[1] != local_a4)) || (pfVar13[2] != local_a0)) {
      *(undefined4 *)((longlong)pvVar4 + 0x54) = 0x3d4ccccd;
      FUN_140559250(pvVar4,1);
    }
    break;
  case 1:
    FUN_140622b80(param_1,param_2,pvVar4);
    break;
  case 2:
  case 3:
    FUN_140622e90(param_1,param_2,pvVar4);
    break;
  case 4:
    if (0 < *(int *)((longlong)pvVar4 + 0x5c)) {
      *(int *)((longlong)pvVar4 + 0x5c) = *(int *)((longlong)pvVar4 + 0x5c) + -1;
      return 0x650000;
    }
    (**(code **)((longlong)param_1->vtable + 0x298))(param_1);
    goto LAB_140623bfd;
  case 5:
    fVar15 = *(float *)&param_1->owner->field_0x88;
    local_88 = DAT_1407ffaf8;
    fStack_94 = 0.0;
    local_90 = 0.0;
    fVar19 = *(float *)&param_1->owner->field_0x8c;
    fStack_84 = 0.0;
    local_80 = 0.0;
    local_98 = DAT_14080080c;
    FUN_1404aaa40(&local_98,((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * fVar15);
    FUN_1402bab90(&local_98,((fVar18 * fVar17) / fVar14) * fVar19);
    FUN_1404aaa40(&local_88,((fVar18 * fVar17) / fVar14) * fVar15);
    FUN_1402bab90(&local_88,((fVar18 * fVar17) / fVar14) * fVar19);
    fVar17 = fStack_94 * DAT_1408ae5d8;
    fVar18 = local_90 * DAT_1408ae5d8;
    fVar14 = local_98 * DAT_1408ae5d8;
    fVar20 = local_88 * DAT_1408906c8;
    fVar19 = fStack_84 * DAT_1408906c8;
    fVar15 = local_80 * DAT_1408906c8;
    *(float *)((longlong)pvVar4 + 0x2c) = local_a8;
    local_a8 = local_a8 + fVar14;
    *(float *)((longlong)pvVar4 + 0x30) = local_a4;
    local_a4 = local_a4 + fVar17;
    *(float *)((longlong)pvVar4 + 0x34) = local_a0;
    local_a0 = local_a0 + fVar18;
    FUN_1403a8f90(param_1->owner,&local_a8,0);
    FUN_140559250(pvVar4,4);
    *(float *)((longlong)pvVar4 + 0x14) = fVar20;
    *(float *)((longlong)pvVar4 + 0x18) = fVar19;
    *(float *)((longlong)pvVar4 + 0x1c) = fVar15;
    iVar7 = FUN_1401ffb40(&DAT_140a13e24,0,0x1e);
    *(int *)((longlong)pvVar4 + 0x5c) = iVar7 + 0x78;
    return 0x650000;
  case 6:
    *(undefined4 *)((longlong)pvVar4 + 0x54) = 0;
    if (((*(float *)((longlong)pvVar4 + 0x14) == 0.0) &&
        (*(float *)((longlong)pvVar4 + 0x18) == 0.0)) &&
       (*(float *)((longlong)pvVar4 + 0x1c) == 0.0)) {
      FUN_140559250(pvVar4,7);
    }
    break;
  case 7:
    *(undefined4 *)((longlong)pvVar4 + 0x54) = 0;
    break;
  case 8:
    iVar7 = *(int *)((longlong)pvVar4 + 0x5c);
    if (iVar7 == -1) {
      *(undefined4 *)((longlong)pvVar4 + 0x5c) = 0x28;
      *(float *)((longlong)pvVar4 + 0x88) = local_a8;
      *(float *)((longlong)pvVar4 + 0x8c) = local_a4;
      *(float *)((longlong)pvVar4 + 0x90) = local_a0;
      *(float *)((longlong)pvVar4 + 0xa0) = local_a8;
      *(float *)((longlong)pvVar4 + 0xa4) = local_a4;
      *(float *)((longlong)pvVar4 + 0xa8) = local_a0;
    }
    else {
      if ((0 < iVar7) && (*(int *)((longlong)pvVar4 + 0x5c) = iVar7 + -1, iVar7 + -1 == 0)) {
        *(undefined8 *)((longlong)pvVar4 + 0xa0) = 0;
        *(undefined4 *)((longlong)pvVar4 + 0xa8) = 0;
      }
      local_a8 = *(float *)((longlong)pvVar4 + 0x88);
      local_a4 = *(float *)((longlong)pvVar4 + 0x8c);
      local_a0 = *(float *)((longlong)pvVar4 + 0x90);
    }
    lVar10 = *(longlong *)&param_1->owner->field_0x2b0;
    pvVar5 = param_1->owner->locomotor_state;
    if ((lVar10 == 0) || (*(longlong *)(lVar10 + 0x110) == 0)) {
      FUN_140559250(pvVar5,1);
    }
    else {
      plVar8 = (longlong *)FUN_1404ec820();
      if ((plVar8 != (longlong *)0x0) && (pvVar5 != (void *)0x0)) {
        if ((*(byte *)(plVar8 + 0x74) & 2) == 0) {
          if (*(char *)((longlong)plVar8 + 0x347) != -1) {
            uVar11 = (**(code **)(*plVar8 + 0x10))(plVar8,0x15);
            local_res10[0] = (float)((uint)local_res10[0] & 0xffffff00);
            puVar9 = (undefined4 *)FUN_1405c3d10(uVar11,param_1->owner,&local_88,local_res10);
            uVar1 = *puVar9;
            uVar2 = puVar9[1];
            uVar3 = puVar9[2];
            if (local_res10[0]._0_1_ != '\0') {
              *(undefined4 *)((longlong)pvVar5 + 0x5c) = 0x28;
            }
            *(undefined4 *)((longlong)pvVar5 + 0xa0) = uVar1;
            *(undefined4 *)((longlong)pvVar5 + 0xa4) = uVar2;
            *(undefined4 *)((longlong)pvVar5 + 0xa8) = uVar3;
            local_90 = local_80;
            FUN_1403a8710(param_1->owner,&local_98);
          }
        }
        else {
          pGVar12 = param_1->owner;
          fVar17 = pGVar12->pos_y;
          fVar18 = pGVar12->pos_x;
          *(float *)((longlong)pvVar5 + 0xa8) = *(float *)((longlong)pvVar5 + 0x1c) + pGVar12->pos_z
          ;
          *(float *)((longlong)pvVar5 + 0xa4) = *(float *)((longlong)pvVar5 + 0x18) + fVar17;
          *(float *)((longlong)pvVar5 + 0xa0) = *(float *)((longlong)pvVar5 + 0x14) + fVar18;
        }
      }
    }
    if (*(int *)((longlong)pvVar4 + 0x5c) == 0) {
      pGVar12 = param_1->owner;
      pfVar13 = (float *)((longlong)pvVar4 + 0xa0);
      if (((*pfVar13 != pGVar12->pos_x) || (*(float *)((longlong)pvVar4 + 0xa4) != pGVar12->pos_y))
         || (*(float *)((longlong)pvVar4 + 0xa8) != pGVar12->pos_z)) {
        fVar17 = pGVar12->pos_y;
        fVar18 = pGVar12->pos_x;
        *(float *)((longlong)pvVar4 + 0x1c) = *(float *)((longlong)pvVar4 + 0xa8) - pGVar12->pos_z;
        *(float *)((longlong)pvVar4 + 0x18) = *(float *)((longlong)pvVar4 + 0xa4) - fVar17;
        *(float *)((longlong)pvVar4 + 0x14) = *pfVar13 - fVar18;
        pGVar12 = param_1->owner;
      }
      FUN_1403a8f90(pGVar12,pfVar13,0);
    }
LAB_140623bfd:
    bVar6 = false;
  }
  if ((*(int *)((longlong)pvVar4 + 0x48) == 8) && (*(int *)((longlong)pvVar4 + 0x5c) == 0)) {
    return 0x650000;
  }
  if (bVar6) {
    FUN_1406224b0(param_1,param_2,pvVar4);
  }
  if (((*(float *)((longlong)pvVar4 + 0x14) == 0.0) && (*(float *)((longlong)pvVar4 + 0x18) == 0.0))
     && (*(float *)((longlong)pvVar4 + 0x1c) == 0.0)) {
    return 0x650000;
  }
  local_a8 = local_a8 + *(float *)((longlong)pvVar4 + 0x14);
  local_a4 = local_a4 + *(float *)((longlong)pvVar4 + 0x18);
  fVar17 = param_2->pos_z;
  local_a0 = local_a0 + *(float *)((longlong)pvVar4 + 0x1c);
  lVar10 = FUN_140623340(param_1);
  fVar18 = *(float *)(lVar10 + 8);
  plVar8 = *(longlong **)(*(longlong *)&param_2->field_0x2b8 + 0x20);
  if (((plVar8 != (longlong *)0x0) && (iVar7 = (**(code **)(*plVar8 + 0xe0))(plVar8), iVar7 == 1))
     && (plVar8[0x67] != 0)) {
    fVar14 = (float)FUN_140135140(plVar8[0x67],local_a8,local_a4);
    fVar18 = fVar18 + fVar14;
  }
  if (fVar18 <= fVar17 + DAT_1408007b4) {
    if (fVar17 + DAT_1408007b4 <= fVar18) goto LAB_140623d22;
    uVar16 = FUN_140372440(*(undefined8 *)&param_2->field_0x298);
    fVar14 = fVar18 - fVar17;
    if (fVar18 - fVar17 <= (float)(uVar16 ^ DAT_140800860)) {
      fVar14 = (float)(uVar16 ^ DAT_140800860);
    }
  }
  else {
    fVar15 = (float)FUN_140372440(*(undefined8 *)&param_2->field_0x298);
    fVar14 = fVar18 - fVar17;
    if (fVar15 <= fVar18 - fVar17) {
      fVar14 = fVar15;
    }
  }
  *(float *)((longlong)pvVar4 + 0x60) = fVar14;
LAB_140623d22:
  fVar17 = DAT_1407ffaf8;
  local_a0 = local_a0 + *(float *)((longlong)pvVar4 + 0x60);
  if (*(int *)((longlong)pvVar4 + 0x48) == 8) {
    *(float *)((longlong)pvVar4 + 0x88) = local_a8;
    fVar18 = (float)*(int *)((longlong)pvVar4 + 0x5c) / DAT_14085d684;
    *(float *)((longlong)pvVar4 + 0x8c) = local_a4;
    fVar17 = fVar17 - fVar18;
    *(float *)((longlong)pvVar4 + 0x90) = local_a0;
    local_80 = (*(float *)((longlong)pvVar4 + 0xa8) - local_a0) * fVar17 + local_a0;
    fStack_84 = (*(float *)((longlong)pvVar4 + 0xa4) - local_a4) * fVar17 + local_a4;
    local_88 = fVar17 * (*(float *)((longlong)pvVar4 + 0xa0) - local_a8) + local_a8;
    FUN_1403a8f90(param_2,&local_88,0);
  }
  else {
    FUN_1403a8f90(param_2,&local_a8,0);
    if (*(int *)((longlong)pvVar4 + 0x50) == 10) {
      uVar11 = FUN_140058570();
      FUN_140220ed0(uVar11,&param_2->listener_list,9);
      *(undefined4 *)((longlong)pvVar4 + 0x50) = 0xb;
    }
    else if (*(int *)((longlong)pvVar4 + 0x50) != 0xb) {
      return 0x650000;
    }
    pGVar12 = param_1->owner;
    fVar17 = DAT_140805898;
    if (*(longlong *)&pGVar12->field_0x2a0 != 0) {
      FUN_140264ab0(*(longlong *)&pGVar12->field_0x2a0,&local_88);
      if (local_88 + local_7c <= fStack_84 + local_78) {
        fVar17 = fStack_84 + local_78;
      }
      else {
        fVar17 = local_88 + local_7c;
      }
      if (fVar17 <= local_80 + local_74) {
        fVar17 = local_80 + local_74;
      }
      else if (local_88 + local_7c <= fStack_84 + local_78) {
        fVar17 = fStack_84 + local_78;
      }
      else {
        fVar17 = local_88 + local_7c;
      }
      pGVar12 = param_1->owner;
    }
    fVar18 = pGVar12->pos_x;
    fVar14 = pGVar12->pos_y;
    fVar15 = pGVar12->pos_z;
    pfVar13 = (float *)FUN_140623340(param_1);
    fVar18 = *pfVar13 - fVar18;
    fVar14 = pfVar13[1] - fVar14;
    fVar15 = *(float *)(*(longlong *)&param_1->owner->field_0x298 + 0xdfc) - fVar15;
    local_res10[0] = (float)FUN_140776d48(fVar18 * fVar18 + fVar14 * fVar14 + fVar15 * fVar15);
    local_res10[0] = ABS(local_res10[0]);
    if (local_res10[0] < fVar17) {
      uVar11 = FUN_140058570();
      FUN_140220ed0(uVar11,&param_2->listener_list,10);
      *(undefined4 *)((longlong)pvVar4 + 0x50) = 9;
    }
  }
  return 0x650000;
}


