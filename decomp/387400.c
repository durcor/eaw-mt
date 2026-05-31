/* FUN_140387400  RVA 0x387400  size 1904  params 2  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Unknown calling convention */

void FUN_140387400(HardPointClass *param_1,uint param_2)

{
  float fVar1;
  HardPointOwnerRecord *pHVar2;
  void *pvVar3;
  int iVar4;
  bool bVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  longlong *plVar10;
  void *pvVar11;
  undefined8 uVar12;
  longlong lVar13;
  longlong lVar14;
  longlong lVar15;
  int iVar16;
  void **ppvVar17;
  float local_res18 [2];
  longlong local_res20;
  undefined **local_68;
  void *local_60;
  HardPointClass *local_58;
  
  pHVar2 = param_1->owner_record;
  if (pHVar2 == (HardPointOwnerRecord *)0x0) {
    return;
  }
  if (5 < pHVar2->motion_state - 5U) {
    return;
  }
  lVar15 = 0;
  if (((param_1->target_slot != (void *)0x0) && (param_1->target != (void *)0x0)) &&
     (*(void **)((longlong)param_1->target + 0x10) != param_1->target_slot)) {
    param_1->target = (void *)0x0;
  }
  if (DAT_140a28640 == '\0') {
    return;
  }
  if (param_1->enable_flag == '\0') {
    return;
  }
  if ((pHVar2->active == '\x01') && (param_1->fire_rate_weight <= 0.0)) {
    return;
  }
  if (param_1->flag_6e == '\x01') {
    return;
  }
  if ((*(byte *)((longlong)param_1->context + 0x3a0) & 0x10) != 0) {
    return;
  }
  cVar6 = FUN_140396fe0();
  if (cVar6 == '\x01') {
    return;
  }
  cVar6 = FUN_14039a400(param_1->context);
  if (cVar6 == '\x01') {
    return;
  }
  cVar6 = FUN_14039b2a0(param_1->context);
  if (cVar6 == '\x01') {
    return;
  }
  fVar1 = param_1->fire_rate_weight2;
  iVar8 = 0;
  if (fVar1 != 0.0) {
    if ((uint)fVar1 < param_2) {
      param_1->fire_rate_weight2 = 0.0;
    }
    else {
      param_1->fire_rate_weight2 = (float)((int)fVar1 - param_2);
      if ((float)((int)fVar1 - param_2) != 0.0) {
        return;
      }
    }
  }
  if (param_1->owner_record->state_flag == '\x01') {
    if (param_1->order_object == (void *)0x0) {
      return;
    }
    cVar6 = FUN_140384850(param_1);
    if (cVar6 == '\0') {
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar8 = (int)DAT_140b15418[2];
      }
      if (iVar8 <= *(int *)&param_1->field_0xcc + 300) {
        return;
      }
      iVar8 = FUN_140294a70(&DAT_140a16fd0);
      if (*(int *)&param_1->field_0xc8 == iVar8) {
        FUN_1402d5290(&DAT_140b27e60,DAT_140b301c8,0,0,0);
      }
      FUN_1403846c0(param_1,&param_1->order_object);
      *(undefined4 *)&param_1->field_0xc8 = 0xffffffff;
      *(undefined4 *)&param_1->field_0xcc = 0;
      return;
    }
  }
  else {
    cVar6 = FUN_140384850(param_1,param_1->target_slot);
    if (cVar6 == '\0') {
      return;
    }
  }
  lVar13 = *(longlong *)((longlong)param_1->context + 0xa8);
  if (((lVar13 != 0) &&
      (*(char *)(*(longlong *)((longlong)param_1->context + 0x298) + 0x8a) == '\0')) &&
     (*(int *)(lVar13 + 0x48) == 0x1c)) {
    return;
  }
  pvVar11 = param_1->target_slot;
  if (((pvVar11 != (void *)0x0) && (pvVar3 = param_1->target, pvVar3 != (void *)0x0)) &&
     ((*(void **)((longlong)pvVar3 + 0x10) != pvVar11 ||
      (((*(longlong *)((longlong)pvVar3 + 0x20) != 0 &&
        (*(char *)(*(longlong *)((longlong)pvVar3 + 0x20) + 0x4d) == '\x01')) &&
       (*(float *)((longlong)pvVar3 + 0x28) <= 0.0)))))) {
    param_1->target = (void *)0x0;
  }
  if (((param_1->owner_record->state_flag == '\0') && (pvVar11 == (void *)0x0)) &&
     (cVar6 = FUN_140385620(param_1), cVar6 == '\0')) {
    return;
  }
  bVar5 = false;
  cVar6 = '\0';
  if (param_1->target_slot != (void *)0x0) {
    cVar7 = FUN_140385620(param_1);
    if (cVar7 == '\0') {
      plVar10 = (longlong *)(**(code **)(*(longlong *)param_1->context + 0x10))(param_1->context,9);
      iVar8 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar8 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (((iVar8 == 2) && (lVar13 = *(longlong *)((longlong)param_1->context + 0x2b0), lVar13 != 0)
          ) && (*(char *)(lVar13 + 0x348) != -1)) {
        lVar14 = *(longlong *)(*(longlong *)(lVar13 + 0xa8) + 0x1f0);
      }
      else {
        lVar13 = *(longlong *)((longlong)param_1->context + 0xa8);
        lVar14 = lVar15;
        if (lVar13 != 0) {
          lVar14 = *(longlong *)(lVar13 + 0x1f0);
        }
      }
      if ((plVar10 != (longlong *)0x0) &&
         (cVar7 = (**(code **)(*plVar10 + 0x108))(plVar10), cVar7 == '\0')) {
        if (lVar14 == 0) {
          return;
        }
        pvVar11 = (void *)FUN_14041a2e0(lVar14,0,0xffffffff);
        if (pvVar11 != param_1->target_slot) {
          return;
        }
      }
    }
    lVar13 = *(longlong *)((longlong)param_1->context + 0xf0);
    if (((lVar13 != 0) && (param_1->target_slot == *(void **)(lVar13 + 0x148))) &&
       (*(char *)(*(longlong *)((longlong)param_1->target_slot + 0x298) + 0x9e) != '\0')) {
      return;
    }
    if ((param_1->target == (void *)0x0) ||
       (param_1->target_slot == *(void **)((longlong)param_1->target + 0x10))) {
      cVar6 = FUN_1403825b0(param_1);
    }
  }
  if ((param_1->owner_record->state_flag == '\x01') && (param_1->order_object != (void *)0x0)) {
    cVar6 = FUN_1403825b0(param_1,param_1->order_object,0);
    if (cVar6 != '\x01') goto LAB_1403877f0;
    FUN_1403846c0(param_1,&param_1->order_object);
LAB_140387791:
    if (param_1->in_progress_flag == '\0') {
      pvVar11 = param_1->context;
      param_1->in_progress_flag = '\x01';
      if ((*(longlong *)((longlong)pvVar11 + 0x2b0) != 0) &&
         (*(longlong *)(*(longlong *)((longlong)pvVar11 + 0x2b0) + 0x110) != 0)) {
        pvVar11 = (void *)FUN_1404ec820();
        if (pvVar11 != param_1->context) goto LAB_1403877f9;
        pvVar11 = *(void **)((longlong)param_1->context + 0x2b0);
      }
      lVar13 = (longlong)pvVar11 + 0x38;
      if (pvVar11 == (void *)0x0) {
        lVar13 = lVar15;
      }
      uVar12 = FUN_140058570();
      FUN_140220ed0(uVar12,lVar13,0x20);
    }
  }
  else {
    if (cVar6 == '\x01') goto LAB_140387791;
LAB_1403877f0:
    if (cVar6 == '\0') {
      param_1->in_progress_flag = '\0';
    }
  }
LAB_1403877f9:
  if (param_1->owner_record->state_flag == '\x01') {
    return;
  }
  if (param_1->target_slot != (void *)0x0) {
    cVar7 = param_1->owner_record[7].field_0x1e;
    if (((((param_1->context == (void *)0x0) ||
          (lVar13 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)((longlong)param_1->context + 0x58))
          , lVar13 == 0)) || (*(longlong *)(lVar13 + 0x360) == 0)) ||
        (_DAT_140b27048 < *(float *)&param_1->owner_record[4].field_0xc)) && (cVar7 == '\0')) {
      return;
    }
    if (param_1->target_slot != (void *)0x0) goto LAB_14038786d;
  }
  cVar7 = FUN_140385620(param_1);
  if (cVar7 == '\0') {
    return;
  }
LAB_14038786d:
  if (cVar6 == '\0') {
    ppvVar17 = &param_1->opp_target_slot;
    pvVar11 = (void *)0x0;
    if (param_1->opp_target_slot != (void *)0x0) {
      cVar6 = FUN_14039aa60(param_1->context);
      if (((cVar6 == '\0') ||
          (cVar6 = FUN_14035f3b0(DAT_140b15418,*(undefined4 *)((longlong)*ppvVar17 + 0x58),
                                 (longlong)param_1->context + 0x78,1), cVar6 == '\0')) &&
         (cVar6 = FUN_1403825b0(param_1,*ppvVar17,0), cVar6 != '\0')) {
        return;
      }
      FUN_1403846c0(param_1,ppvVar17);
      pvVar11 = *ppvVar17;
      bVar5 = true;
    }
    pvVar3 = param_1->context;
    iVar8 = *(int *)(*(longlong *)(*(longlong *)((longlong)pvVar3 + 0x2b8) + 0x20) + 0x10);
    if ((((pvVar11 == (void *)0x0) && (bVar5)) ||
        ((int)((float)DAT_140b0a340 * DAT_1408007c0) < iVar8 - *(int *)&param_1->field_0x64)) &&
       ((*(int *)&param_1->field_0x64 = iVar8, (*(byte *)((longlong)pvVar3 + 0x3a1) & 0x10) == 0 &&
        ((*(longlong *)((longlong)pvVar3 + 0x100) == 0 ||
         (*(char *)(*(longlong *)((longlong)pvVar3 + 0x100) + 0x2a4) == '\0')))))) {
      cVar6 = FUN_14039aa60(pvVar3);
      lVar13 = DAT_140a16fd8 - DAT_140a16fd0;
      local_res20 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)((longlong)param_1->context + 0x58))
      ;
      iVar4 = (int)(lVar13 >> 3);
      local_res18[0] = DAT_140899788;
      iVar8 = *(int *)(local_res20 + 0x4c);
      iVar9 = FUN_1401ffb40(&DAT_140a13e24,0,iVar4 + -1);
      iVar16 = 0;
      lVar13 = lVar15;
      if (0 < iVar4) {
        do {
          if (iVar9 == iVar8) {
LAB_140387a62:
            iVar9 = iVar9 + 1;
            lVar15 = lVar13;
            if (iVar4 <= iVar9) {
              iVar9 = 0;
            }
          }
          else {
            lVar14 = FUN_140294bc0(&DAT_140a16fd0,iVar9);
            lVar15 = lVar13;
            if ((lVar14 != 0) &&
               ((cVar6 == '\0' ||
                (cVar7 = FUN_14035f3b0(DAT_140b15418,*(undefined4 *)(lVar14 + 0x4c),
                                       (longlong)param_1->context + 0x78,1), cVar7 == '\0')))) {
              if (((*(longlong *)(lVar14 + 0x68) != 0) &&
                  (*(char *)(*(longlong *)(lVar14 + 0x68) + 0x108) != '\0')) ||
                 (((cVar7 = FUN_1402824f0(local_res20,lVar14), cVar7 != '\x01' ||
                   (lVar15 = FUN_140385190(param_1,lVar14,local_res18), lVar15 == 0)) ||
                  (lVar13 = lVar15, local_res18[0] != DAT_140899780)))) goto LAB_140387a62;
              break;
            }
          }
          iVar16 = iVar16 + 1;
          lVar13 = lVar15;
        } while (iVar16 < iVar4);
      }
      ppvVar17 = &param_1->opp_target_slot;
      if (lVar15 != 0) {
        if (*ppvVar17 != (void *)0x0) {
          FUN_1403846c0(param_1,ppvVar17);
        }
        FUN_140382510(param_1,lVar15,ppvVar17);
      }
    }
    ppvVar17 = &param_1->opp_target_slot;
    if (*ppvVar17 != (void *)0x0) {
      cVar6 = FUN_1403825b0(param_1,*ppvVar17,0);
      if (cVar6 == '\0') {
        FUN_1403846c0(param_1,ppvVar17);
      }
      else {
        local_68 = OpportunityTargetAcquiredDataClass::vftable;
        local_60 = *ppvVar17;
        pvVar11 = param_1->context;
        local_58 = param_1;
        if ((*(longlong *)((longlong)pvVar11 + 0x2b0) != 0) &&
           (*(longlong *)(*(longlong *)((longlong)pvVar11 + 0x2b0) + 0x110) != 0)) {
          pvVar11 = (void *)FUN_1404ec820();
          if (pvVar11 != param_1->context) {
            return;
          }
          pvVar11 = *(void **)((longlong)param_1->context + 0x2b0);
        }
        lVar15 = (longlong)pvVar11 + 0x38;
        if (pvVar11 == (void *)0x0) {
          lVar15 = 0;
        }
        uVar12 = FUN_140058570();
        FUN_140220ed0(uVar12,lVar15,0x21,&local_68);
      }
    }
  }
  return;
}


