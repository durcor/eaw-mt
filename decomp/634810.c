/* TargetingBehaviorClass::FUN_140634810  RVA 0x634810  size 2182  params 0  ret undefined */


undefined8
TargetingBehaviorClass::FUN_140634810(longlong *param_1,longlong *param_2,undefined4 param_3)

{
  bool bVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  ulonglong uVar6;
  longlong lVar7;
  longlong lVar8;
  longlong *plVar9;
  ulonglong uVar10;
  undefined8 uVar11;
  ulonglong uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  uint local_res8 [2];
  float local_res10 [2];
  undefined4 local_res18;
  undefined8 local_res20;
  float *pfVar16;
  undefined8 *puVar17;
  undefined4 local_98 [2];
  undefined8 local_90;
  undefined8 local_88;
  float local_80;
  
  lVar8 = param_2[0x1e];
  local_res18 = param_3;
  FUN_140565de0();
  lVar5 = param_2[0x15];
  local_88 = lVar5;
  cVar2 = (**(code **)(*param_1 + 0x108))(param_1);
  uVar10 = 0;
  if (((cVar2 == '\0') && (lVar5 != 0)) && (*(longlong *)(lVar5 + 0x1f0) != 0)) {
    local_res8[0] = 0xffffffff;
    lVar5 = FUN_14041a2e0(*(longlong *)(lVar5 + 0x1f0),local_res8,0);
    param_3 = local_res18;
    if ((lVar5 != 0) && (cVar2 = FUN_1405680b0(lVar5), param_3 = local_res18, cVar2 != '\0')) {
      uVar6 = uVar10;
      if (-1 < (int)local_res8[0]) {
        uVar6 = FUN_140396cb0(lVar5);
      }
      cVar2 = (**(code **)(*param_1 + 0x160))(param_1,lVar5,uVar6);
      param_3 = local_res18;
      if ((cVar2 != '\0') &&
         (cVar2 = (**(code **)(*param_1 + 0x168))(param_1,lVar5,uVar6), param_3 = local_res18,
         cVar2 == '\0')) {
        (**(code **)(*param_1 + 0xd0))(param_1,lVar5,uVar6,1,1);
        uVar12 = uVar10;
        while( true ) {
          iVar4 = 0;
          if (param_2[0x5a] != 0) {
            iVar4 = *(int *)(param_2[0x5a] + 0x10);
          }
          param_3 = local_res18;
          if (iVar4 <= (int)uVar12) break;
          lVar7 = FUN_140396cb0(param_2,uVar12);
          if ((lVar7 != 0) && (*(int *)(*(longlong *)(lVar7 + 0x20) + 0x48) - 5U < 6)) {
            FUN_140387b70(lVar7,lVar5,uVar6);
          }
          uVar12 = (ulonglong)((int)uVar12 + 1);
        }
      }
    }
  }
  lVar5 = *(longlong *)(lVar8 + 8);
  if (lVar5 == 0) {
    iVar4 = (**(code **)(*param_1 + 0xf0))(param_1);
    if (iVar4 != 0) {
      (**(code **)(*param_1 + 0xd8))(param_1,1);
    }
    if ((*(byte *)((longlong)param_2 + 0x3a1) & 0x10) != 0) {
      return 0x650000;
    }
    pfVar16 = local_res10;
    local_res10[0] = DAT_140899788;
    local_res20 = 0;
    local_res8[0] = local_res8[0] & 0xffffff00;
    lVar8 = FUN_140568a00(param_1,param_3,&local_res20,local_res8,pfVar16);
    if (lVar8 == 0) {
      return 0x650000;
    }
    FUN_14038d730(param_2,lVar8,0,0,(ulonglong)pfVar16 & 0xffffffff00000000,0xffffffff,0);
    return 0x650000;
  }
  if (((((*(char *)(lVar5 + 0x3a7) != '\0') || (*(char *)(lVar5 + 0x3b4) != '\0')) ||
       ((*(byte *)(lVar5 + 0x3a0) & 0x42) != 0)) ||
      ((cVar2 = FUN_14039b140(), cVar2 != '\0' && (*(char *)(param_2[0x53] + 0xa4) == '\0')))) ||
     (cVar2 = FUN_14035f470(DAT_140b15418,(int)param_2[0xb],*(undefined8 *)(lVar8 + 8),1),
     cVar2 == '\x01')) goto LAB_140635062;
  cVar2 = (**(code **)(*param_1 + 0x108))(param_1);
  if ((cVar2 == '\0') &&
     (((cVar2 = FUN_14039a390(param_2), cVar2 != '\0' ||
       (cVar2 = FUN_14039a370(param_2), cVar2 == '\0')) || (*(char *)(param_2[0x53] + 0xaf) != '\0')
      ))) {
    lVar5 = *(longlong *)(lVar8 + 8);
    local_res10[0] = DAT_140899784;
    local_98[0] = 0;
    cVar2 = FUN_140567c50(param_1,lVar5,local_res10,local_98);
    cVar3 = FUN_140569000(param_1,lVar5);
    if ((cVar2 == '\0') || ((cVar3 == '\0' && (local_res10[0] != DAT_140899780)))) {
      puVar17 = &local_res20;
      local_res20 = CONCAT44(local_res20._4_4_,DAT_140899788);
      local_90 = 0;
      local_res8[0] = local_res8[0] & 0xffffff00;
      lVar7 = FUN_140568a00(param_1,local_res18,&local_90,local_res8,puVar17);
      if (lVar7 == 0) {
        if (cVar2 == '\0') {
          (**(code **)(*param_1 + 0xd8))(param_1,0);
          return 0x650000;
        }
      }
      else if (lVar7 != lVar5) {
        if (cVar2 != '\0') {
          puVar17 = (undefined8 *)CONCAT44((int)((ulonglong)puVar17 >> 0x20),local_res10[0]);
          cVar2 = FUN_140567110(param_1,lVar7,(undefined4)local_res20,lVar5,puVar17);
          if (cVar2 == '\0') goto LAB_140634b7f;
        }
        FUN_14038d730(param_2,lVar7,0,0,(ulonglong)puVar17 & 0xffffffff00000000,0xffffffff,0);
      }
    }
  }
LAB_140634b7f:
  if ((*(char *)((longlong)param_2 + 0x333) != -1) &&
     (plVar9 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1), plVar9 != (longlong *)0x0)) {
    (**(code **)(*plVar9 + 0x260))(plVar9,*(undefined8 *)(lVar8 + 8),0);
  }
  cVar2 = (**(code **)(*param_1 + 0x160))
                    (param_1,*(undefined8 *)(lVar8 + 8),*(undefined8 *)(lVar8 + 0x10));
  if ((cVar2 == '\0') ||
     (cVar2 = (**(code **)(*param_1 + 0x168))
                        (param_1,*(undefined8 *)(lVar8 + 8),*(undefined8 *)(lVar8 + 0x10)),
     cVar2 != '\0')) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
    if (*(longlong *)(lVar8 + 0x88) == 0) {
      if (*(longlong *)(lVar8 + 0x80) != 0) {
        cVar2 = FUN_14039ade0(param_2);
        if (cVar2 != '\0') {
          iVar4 = FUN_140397ae0(param_2,5);
          if (iVar4 == 0) {
            (**(code **)(*param_1 + 0xf8))(param_1,4);
          }
          else {
            (**(code **)(*param_1 + 0xf8))(param_1,2);
          }
          goto LAB_140634f67;
        }
        cVar2 = (**(code **)(*param_1 + 0x180))(param_1,*(undefined8 *)(lVar8 + 8));
        cVar3 = FUN_140568e60(param_1);
        if (cVar3 != '\0') {
          FUN_140391260(param_2);
        }
        cVar3 = FUN_14039a370(param_2);
        if ((((cVar3 == '\0') || (*(char *)(param_2[0x53] + 0xa6) != '\0')) ||
            (*(char *)(param_2[0x53] + 0xaf) != '\0')) && (cVar2 != '\0')) {
          (**(code **)(*param_1 + 0x178))(param_1,1);
          uVar11 = 3;
        }
        else {
          cVar3 = FUN_14039a370(param_2);
          if (cVar3 == '\0') {
            lVar5 = *(longlong *)(lVar8 + 8);
            if (*(char *)(lVar5 + 0x348) != -1) {
              lVar5 = FUN_140566aa0(param_1,lVar5);
            }
            if (lVar5 != 0) {
              FUN_140394440(param_2,lVar5 + 0x78);
            }
          }
          uVar11 = 4;
        }
        (**(code **)(*param_1 + 0xf8))(param_1,uVar11);
        iVar4 = (**(code **)(*param_1 + 0xf0))(param_1);
        if (iVar4 != 3) goto LAB_140634f67;
        if (cVar2 == '\x01') {
          plVar9 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1);
          if (plVar9 == (longlong *)0x0) goto LAB_140634f67;
          (**(code **)(*plVar9 + 0x2e8))(plVar9,*(undefined8 *)(lVar8 + 8));
          cVar2 = (**(code **)(*param_1 + 0x180))(param_1,*(undefined8 *)(lVar8 + 8));
          if (cVar2 != '\0') goto LAB_140634f67;
        }
      }
      (**(code **)(*param_1 + 0xf8))(param_1,4);
    }
    else {
      if ((local_88 != 0) && (*(longlong *)(local_88 + 0x1f0) != 0)) {
        uVar10 = FUN_14041a2e0(*(longlong *)(local_88 + 0x1f0),0,0xffffffff);
      }
      cVar2 = FUN_14039ade0(param_2);
      if ((cVar2 == '\0') && (cVar2 = FUN_140568e60(param_1), cVar2 != '\0')) {
        FUN_140391260(param_2);
      }
      cVar2 = FUN_14039a400(param_2);
      if (((cVar2 == '\0') && (cVar2 = FUN_14039b2a0(param_2), cVar2 == '\0')) &&
         ((cVar2 = FUN_14039a320(param_2,1), cVar2 == '\0' ||
          ((cVar2 = FUN_14039a390(param_2), cVar2 != '\0' ||
           (*(char *)(param_2[0x53] + 0xaf) != '\0')))))) {
        cVar2 = FUN_140631cc0(*(undefined8 *)(lVar8 + 0x88),*(undefined8 *)(lVar8 + 8));
        if (cVar2 == '\0') {
          cVar2 = FUN_14039a390(param_2);
          if (((cVar2 != '\0') || (uVar10 != *(ulonglong *)(lVar8 + 8))) ||
             (*(char *)(*(longlong *)(*(ulonglong *)(lVar8 + 8) + 0x298) + 0x80) == '\0')) {
LAB_140635062:
            (**(code **)(*param_1 + 0xd8))
                      (param_1,*(longlong *)(lVar8 + 8) == *(longlong *)(lVar8 + 0x148));
            return 0x650000;
          }
          cVar2 = FUN_14039ade0(param_2);
          if ((cVar2 == '\0') &&
             (plVar9 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1),
             plVar9 != (longlong *)0x0)) {
            lVar5 = *(longlong *)(lVar8 + 8);
            fVar13 = *(float *)(lVar5 + 0x78);
            fVar15 = *(float *)(lVar5 + 0x7c);
            fVar14 = *(float *)(lVar5 + 0x80);
            if ((*(char *)(lVar5 + 0x348) != -1) && (lVar5 = FUN_140566aa0(param_1), lVar5 != 0)) {
              fVar13 = *(float *)(lVar5 + 0x78);
              fVar15 = *(float *)(lVar5 + 0x7c);
              fVar14 = *(float *)(lVar5 + 0x80);
            }
            local_80 = fVar14 - *(float *)(param_2 + 0x10);
            local_88 = CONCAT44(fVar15 - *(float *)((longlong)param_2 + 0x7c),
                                fVar13 - *(float *)(param_2 + 0xf));
            (**(code **)(*plVar9 + 0x218))(plVar9,&local_88);
          }
          (**(code **)(*param_1 + 0xf8))(param_1,3);
        }
        else {
          cVar2 = FUN_140632050(*(undefined8 *)(lVar8 + 0x88),*(undefined8 *)(lVar8 + 8));
          if (cVar2 == '\0') {
            (**(code **)(*param_1 + 0xf8))(param_1,3);
          }
          else {
            (**(code **)(*param_1 + 0xf8))(param_1,4);
          }
        }
      }
    }
  }
LAB_140634f67:
  iVar4 = (**(code **)(*param_1 + 0xf0))(param_1);
  if (iVar4 == 4) {
    (**(code **)(*param_1 + 0x178))(param_1,1);
  }
  lVar5 = *(longlong *)(lVar8 + 0x88);
  if (bVar1) {
    if (lVar5 != 0) {
      FUN_140631cc0(lVar5,*(undefined8 *)(lVar8 + 8));
    }
  }
  else {
    if (lVar5 != 0) {
      FUN_1406319d0(lVar5,1);
    }
    if (*(longlong *)(lVar8 + 8) != 0) {
      cVar2 = FUN_14039a390(param_2);
      if (((cVar2 == '\0') && (cVar2 = FUN_14039a400(param_2), cVar2 == '\0')) ||
         (0.0 < *(float *)(param_2[0x53] + 0x23b0))) {
        iVar4 = (**(code **)(*param_1 + 0xf0))(param_1);
        if (iVar4 != 2) {
          (**(code **)(*param_1 + 0xf8))(param_1,2);
        }
      }
      else {
        (**(code **)(*param_1 + 0xd8))
                  (param_1,*(longlong *)(lVar8 + 8) == *(longlong *)(lVar8 + 0x148));
      }
    }
  }
  if (*(int *)(lVar8 + 0x18) != 4) {
    return 0x650000;
  }
  lVar5 = *(longlong *)(lVar8 + 8);
  if (lVar5 == 0) {
    return 0x650000;
  }
  if (lVar5 != *(longlong *)(lVar8 + 0x148)) {
    return 0x650000;
  }
  if (*(char *)(*(longlong *)(lVar5 + 0x298) + 0x9e) == '\0') {
    return 0x650000;
  }
  (**(code **)(*param_1 + 0xf8))(param_1,2);
  return 0x650000;
}


