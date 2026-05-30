/* FUN_140387400  RVA 0x387400  size 1904  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140387400(longlong param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  bool bVar3;
  char cVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong lVar11;
  longlong lVar12;
  int iVar13;
  float local_res18 [2];
  longlong local_res20;
  undefined **local_68;
  longlong local_60;
  longlong local_58;
  
  lVar9 = *(longlong *)(param_1 + 0x20);
  if (lVar9 == 0) {
    return;
  }
  if (5 < *(int *)(lVar9 + 0x48) - 5U) {
    return;
  }
  lVar12 = 0;
  if (((*(longlong *)(param_1 + 0x40) != 0) && (*(longlong *)(param_1 + 0x48) != 0)) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x10) != *(longlong *)(param_1 + 0x40))) {
    *(undefined8 *)(param_1 + 0x48) = 0;
  }
  if (DAT_140a28640 == '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x6c) == '\0') {
    return;
  }
  if ((*(char *)(lVar9 + 0x4d) == '\x01') && (*(float *)(param_1 + 0x28) <= 0.0)) {
    return;
  }
  if (*(char *)(param_1 + 0x6e) == '\x01') {
    return;
  }
  if ((*(byte *)(*(longlong *)(param_1 + 0x10) + 0x3a0) & 0x10) != 0) {
    return;
  }
  cVar4 = FUN_140396fe0();
  if (cVar4 == '\x01') {
    return;
  }
  cVar4 = FUN_14039a400(*(undefined8 *)(param_1 + 0x10));
  if (cVar4 == '\x01') {
    return;
  }
  cVar4 = FUN_14039b2a0(*(undefined8 *)(param_1 + 0x10));
  if (cVar4 == '\x01') {
    return;
  }
  uVar1 = *(uint *)(param_1 + 0x58);
  iVar6 = 0;
  if (uVar1 != 0) {
    if (uVar1 < param_2) {
      *(undefined4 *)(param_1 + 0x58) = 0;
    }
    else {
      *(uint *)(param_1 + 0x58) = uVar1 - param_2;
      if (uVar1 - param_2 != 0) {
        return;
      }
    }
  }
  if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x50) == '\x01') {
    if (*(longlong *)(param_1 + 0xc0) == 0) {
      return;
    }
    cVar4 = FUN_140384850(param_1);
    if (cVar4 == '\0') {
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar6 = (int)DAT_140b15418[2];
      }
      if (iVar6 <= *(int *)(param_1 + 0xcc) + 300) {
        return;
      }
      iVar6 = FUN_140294a70(&DAT_140a16fd0);
      if (*(int *)(param_1 + 200) == iVar6) {
        FUN_1402d5290(&DAT_140b27e60,DAT_140b301c8,0,0,0);
      }
      FUN_1403846c0(param_1,param_1 + 0xc0);
      *(undefined4 *)(param_1 + 200) = 0xffffffff;
      *(undefined4 *)(param_1 + 0xcc) = 0;
      return;
    }
  }
  else {
    cVar4 = FUN_140384850(param_1,*(undefined8 *)(param_1 + 0x40));
    if (cVar4 == '\0') {
      return;
    }
  }
  lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xa8);
  if (((lVar9 != 0) &&
      (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298) + 0x8a) == '\0')) &&
     (*(int *)(lVar9 + 0x48) == 0x1c)) {
    return;
  }
  lVar9 = *(longlong *)(param_1 + 0x40);
  if (((lVar9 != 0) && (lVar11 = *(longlong *)(param_1 + 0x48), lVar11 != 0)) &&
     ((*(longlong *)(lVar11 + 0x10) != lVar9 ||
      (((*(longlong *)(lVar11 + 0x20) != 0 &&
        (*(char *)(*(longlong *)(lVar11 + 0x20) + 0x4d) == '\x01')) &&
       (*(float *)(lVar11 + 0x28) <= 0.0)))))) {
    *(undefined8 *)(param_1 + 0x48) = 0;
  }
  if (((*(char *)(*(longlong *)(param_1 + 0x20) + 0x50) == '\0') && (lVar9 == 0)) &&
     (cVar4 = FUN_140385620(param_1), cVar4 == '\0')) {
    return;
  }
  bVar3 = false;
  cVar4 = '\0';
  if (*(longlong *)(param_1 + 0x40) != 0) {
    cVar5 = FUN_140385620(param_1);
    if (cVar5 == '\0') {
      plVar8 = (longlong *)
               (**(code **)(**(longlong **)(param_1 + 0x10) + 0x10))
                         (*(longlong **)(param_1 + 0x10),9);
      iVar6 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar6 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (((iVar6 == 2) &&
          (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2b0), lVar9 != 0)) &&
         (*(char *)(lVar9 + 0x348) != -1)) {
        lVar11 = *(longlong *)(*(longlong *)(lVar9 + 0xa8) + 0x1f0);
      }
      else {
        lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xa8);
        lVar11 = lVar12;
        if (lVar9 != 0) {
          lVar11 = *(longlong *)(lVar9 + 0x1f0);
        }
      }
      if ((plVar8 != (longlong *)0x0) &&
         (cVar5 = (**(code **)(*plVar8 + 0x108))(plVar8), cVar5 == '\0')) {
        if (lVar11 == 0) {
          return;
        }
        lVar9 = FUN_14041a2e0(lVar11,0,0xffffffff);
        if (lVar9 != *(longlong *)(param_1 + 0x40)) {
          return;
        }
      }
    }
    lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xf0);
    if (((lVar9 != 0) && (*(longlong *)(param_1 + 0x40) == *(longlong *)(lVar9 + 0x148))) &&
       (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x40) + 0x298) + 0x9e) != '\0')) {
      return;
    }
    if ((*(longlong *)(param_1 + 0x48) == 0) ||
       (*(longlong *)(param_1 + 0x40) == *(longlong *)(*(longlong *)(param_1 + 0x48) + 0x10))) {
      cVar4 = FUN_1403825b0(param_1);
    }
  }
  if ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x50) == '\x01') &&
     (*(longlong *)(param_1 + 0xc0) != 0)) {
    cVar4 = FUN_1403825b0(param_1,*(longlong *)(param_1 + 0xc0),0);
    if (cVar4 != '\x01') goto LAB_1403877f0;
    FUN_1403846c0(param_1,param_1 + 0xc0);
LAB_140387791:
    if (*(char *)(param_1 + 0x6d) == '\0') {
      lVar9 = *(longlong *)(param_1 + 0x10);
      *(undefined1 *)(param_1 + 0x6d) = 1;
      if ((*(longlong *)(lVar9 + 0x2b0) != 0) &&
         (*(longlong *)(*(longlong *)(lVar9 + 0x2b0) + 0x110) != 0)) {
        lVar9 = FUN_1404ec820();
        if (lVar9 != *(longlong *)(param_1 + 0x10)) goto LAB_1403877f9;
        lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2b0);
      }
      lVar11 = lVar9 + 0x38;
      if (lVar9 == 0) {
        lVar11 = lVar12;
      }
      uVar10 = FUN_140058570();
      FUN_140220ed0(uVar10,lVar11,0x20);
    }
  }
  else {
    if (cVar4 == '\x01') goto LAB_140387791;
LAB_1403877f0:
    if (cVar4 == '\0') {
      *(undefined1 *)(param_1 + 0x6d) = 0;
    }
  }
LAB_1403877f9:
  if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x50) == '\x01') {
    return;
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    cVar5 = *(char *)(*(longlong *)(param_1 + 0x20) + 0x3d6);
    if (((((*(longlong *)(param_1 + 0x10) == 0) ||
          (lVar9 = FUN_140294bc0(&DAT_140a16fd0,
                                 *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58)), lVar9 == 0)
          ) || (*(longlong *)(lVar9 + 0x360) == 0)) ||
        (_DAT_140b27048 < *(float *)(*(longlong *)(param_1 + 0x20) + 0x22c))) && (cVar5 == '\0')) {
      return;
    }
    if (*(longlong *)(param_1 + 0x40) != 0) goto LAB_14038786d;
  }
  cVar5 = FUN_140385620(param_1);
  if (cVar5 == '\0') {
    return;
  }
LAB_14038786d:
  if (cVar4 == '\0') {
    plVar8 = (longlong *)(param_1 + 0x50);
    lVar9 = 0;
    if (*(longlong *)(param_1 + 0x50) != 0) {
      cVar4 = FUN_14039aa60(*(undefined8 *)(param_1 + 0x10));
      if (((cVar4 == '\0') ||
          (cVar4 = FUN_14035f3b0(DAT_140b15418,*(undefined4 *)(*plVar8 + 0x58),
                                 *(longlong *)(param_1 + 0x10) + 0x78,1), cVar4 == '\0')) &&
         (cVar4 = FUN_1403825b0(param_1,*plVar8,0), cVar4 != '\0')) {
        return;
      }
      FUN_1403846c0(param_1,plVar8);
      lVar9 = *plVar8;
      bVar3 = true;
    }
    lVar11 = *(longlong *)(param_1 + 0x10);
    iVar6 = *(int *)(*(longlong *)(*(longlong *)(lVar11 + 0x2b8) + 0x20) + 0x10);
    if ((((lVar9 == 0) && (bVar3)) ||
        ((int)((float)DAT_140b0a340 * DAT_1408007c0) < iVar6 - *(int *)(param_1 + 100))) &&
       ((*(int *)(param_1 + 100) = iVar6, (*(byte *)(lVar11 + 0x3a1) & 0x10) == 0 &&
        ((*(longlong *)(lVar11 + 0x100) == 0 ||
         (*(char *)(*(longlong *)(lVar11 + 0x100) + 0x2a4) == '\0')))))) {
      cVar4 = FUN_14039aa60(lVar11);
      lVar9 = DAT_140a16fd8 - DAT_140a16fd0;
      local_res20 = FUN_140294bc0(&DAT_140a16fd0,
                                  *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58));
      iVar2 = (int)(lVar9 >> 3);
      local_res18[0] = DAT_140899788;
      iVar6 = *(int *)(local_res20 + 0x4c);
      iVar7 = FUN_1401ffb40(&DAT_140a13e24,0,iVar2 + -1);
      iVar13 = 0;
      lVar9 = lVar12;
      if (0 < iVar2) {
        do {
          if (iVar7 == iVar6) {
LAB_140387a62:
            iVar7 = iVar7 + 1;
            lVar12 = lVar9;
            if (iVar2 <= iVar7) {
              iVar7 = 0;
            }
          }
          else {
            lVar11 = FUN_140294bc0(&DAT_140a16fd0,iVar7);
            lVar12 = lVar9;
            if ((lVar11 != 0) &&
               ((cVar4 == '\0' ||
                (cVar5 = FUN_14035f3b0(DAT_140b15418,*(undefined4 *)(lVar11 + 0x4c),
                                       *(longlong *)(param_1 + 0x10) + 0x78,1), cVar5 == '\0')))) {
              if (((*(longlong *)(lVar11 + 0x68) != 0) &&
                  (*(char *)(*(longlong *)(lVar11 + 0x68) + 0x108) != '\0')) ||
                 (((cVar5 = FUN_1402824f0(local_res20,lVar11), cVar5 != '\x01' ||
                   (lVar12 = FUN_140385190(param_1,lVar11,local_res18), lVar12 == 0)) ||
                  (lVar9 = lVar12, local_res18[0] != DAT_140899780)))) goto LAB_140387a62;
              break;
            }
          }
          iVar13 = iVar13 + 1;
          lVar9 = lVar12;
        } while (iVar13 < iVar2);
      }
      plVar8 = (longlong *)(param_1 + 0x50);
      if (lVar12 != 0) {
        if (*plVar8 != 0) {
          FUN_1403846c0(param_1,plVar8);
        }
        FUN_140382510(param_1,lVar12,plVar8);
      }
    }
    plVar8 = (longlong *)(param_1 + 0x50);
    if (*plVar8 != 0) {
      cVar4 = FUN_1403825b0(param_1,*plVar8,0);
      if (cVar4 == '\0') {
        FUN_1403846c0(param_1,plVar8);
      }
      else {
        local_68 = OpportunityTargetAcquiredDataClass::vftable;
        local_60 = *plVar8;
        lVar9 = *(longlong *)(param_1 + 0x10);
        local_58 = param_1;
        if ((*(longlong *)(lVar9 + 0x2b0) != 0) &&
           (*(longlong *)(*(longlong *)(lVar9 + 0x2b0) + 0x110) != 0)) {
          lVar9 = FUN_1404ec820();
          if (lVar9 != *(longlong *)(param_1 + 0x10)) {
            return;
          }
          lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2b0);
        }
        lVar12 = lVar9 + 0x38;
        if (lVar9 == 0) {
          lVar12 = 0;
        }
        uVar10 = FUN_140058570();
        FUN_140220ed0(uVar10,lVar12,0x21,&local_68);
      }
    }
  }
  return;
}


