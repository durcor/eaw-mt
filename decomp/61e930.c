/* WalkLocomotorBehaviorClass::vfunc_6  RVA 0x61e930  size 2925  params 1  ret undefined */


/* WARNING: Unknown calling convention */

int WalkLocomotorBehaviorClass::vfunc_6(LocomotorBehaviorClass *param_1)

{
  GameObjectClass *pGVar1;
  void *pvVar2;
  longlong lVar3;
  void *pvVar4;
  undefined4 uVar5;
  char cVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  longlong lVar10;
  undefined8 uVar11;
  void **ppvVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined4 uVar18;
  int local_res8 [2];
  float local_108;
  float local_104;
  float local_100;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined1 local_e8 [16];
  undefined1 local_d8 [8];
  undefined4 local_d0;
  undefined1 local_c8 [16];
  undefined1 local_b8 [8];
  float local_b0;
  
  if (param_1->owner == (GameObjectClass *)0x0) {
    return -0x7f9affff;
  }
  LocomotorCommonClass::vfunc_6();
  cVar6 = (**(code **)((longlong)param_1->vtable + 0x210))(param_1);
  if ((cVar6 != '\0') || (param_1->owner == (GameObjectClass *)0x0)) {
    return 0x650000;
  }
  if (*(char *)(*(longlong *)&param_1->owner->field_0x298 + 0x26) == '\x01') {
    local_res8[0] = FUN_1403956a0();
    cVar6 = FUN_14027b4f0(local_res8);
    if ((cVar6 == '\x01') && (iVar7 = FUN_140395660(param_1->owner), iVar7 != 2)) {
      return 0x650000;
    }
  }
  iVar7 = DAT_140b153fc;
  if (DAT_140b15418 != (longlong *)0x0) {
    iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))();
  }
  if (iVar7 != 1) {
    return 0x650000;
  }
  pGVar1 = param_1->owner;
  pvVar2 = pGVar1->locomotor_state;
  if (pvVar2 == (void *)0x0) {
    return -0x7f9affff;
  }
  if (*(uint *)((longlong)pvVar2 + 0x2a8) < 2) {
    if ((*(int *)((longlong)pvVar2 + 0x48) != 0) && (0xb < *(int *)((longlong)pvVar2 + 0x48) - 0xfU)
       ) {
      return -0x7f9afffe;
    }
    lVar10 = FUN_1405569b0(pvVar2);
    if (lVar10 != 0) {
      if (DAT_140b0a320 < *(uint *)(lVar10 + 0x54)) {
        return 0x650000;
      }
      if (*(uint *)(lVar10 + 0x54) == DAT_140b0a320) {
        *(undefined1 *)((longlong)pvVar2 + 0x275) = 0;
      }
    }
    uVar5 = DAT_140866870;
    fVar14 = DAT_1408524f8;
    uVar18 = DAT_1408007b8;
    fVar13 = DAT_1407ffaf8;
    if (*(longlong *)&param_1->owner->field_0x298 == 0) {
      return -0x7f9affff;
    }
    local_res8[0] = 0x650000;
switchD_14061ec53_caseD_1:
    do {
      switch(*(undefined4 *)((longlong)pvVar2 + 0x48)) {
      case 0:
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          return -0x7f9affff;
        }
        lVar10 = FUN_1405569b0(pvVar4);
        if (lVar10 == 0) goto LAB_14061f3b4;
        if (*(char *)(lVar10 + 0x6d) == '\0') {
          FUN_140559250(pvVar4,0x16);
          goto LAB_14061f3b4;
        }
        lVar3 = *(longlong *)&param_1->owner->field_0x298;
        if (lVar3 == 0) {
          return -0x7f9affff;
        }
        fVar17 = (float)FUN_140372560(lVar3);
        pGVar1 = param_1->owner;
        local_108 = *(float *)&pGVar1->field_0x84;
        local_104 = *(float *)&pGVar1->field_0x88;
        local_100 = *(float *)&pGVar1->field_0x8c;
        FUN_14061d570(param_1,local_e8,0);
        FUN_14020b100(local_e8,local_d8);
        fVar15 = (float)FUN_14020b6d0(local_d0);
        fVar16 = (float)FUN_140371310(lVar3);
        lVar10 = FUN_1405c4b80(lVar10,*(undefined4 *)(lVar10 + 0x60));
        if ((ABS(fVar15) <= fVar17) || ((ABS(fVar15) <= fVar16 && (0.0 < *(float *)(lVar10 + 4)))))
        {
          FUN_140559250(pvVar4,0x12);
        }
        else {
          FUN_140559250(pvVar4,0xf);
        }
      default:
        goto switchD_14061ec53_caseD_1;
      case 0xf:
        pGVar1 = param_1->owner;
        pvVar4 = pGVar1->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          return -0x7f9affff;
        }
        lVar10 = *(longlong *)&pGVar1->field_0x298;
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        local_f8 = *(undefined4 *)&pGVar1->field_0x84;
        cVar6 = *(char *)(lVar10 + 0x36);
        local_f4 = *(undefined4 *)&pGVar1->field_0x88;
        local_f0 = *(undefined4 *)&pGVar1->field_0x8c;
        FUN_14061d570(param_1,local_c8,0);
        FUN_14020b100(local_c8,local_b8);
        if ((local_b0 <= 0.0) || (iVar7 = 0xc, fVar14 <= local_b0)) {
          iVar7 = 0xe;
        }
        if ((cVar6 != '\0') && (iVar9 = FUN_14038f9e0(param_1->owner,iVar7), iVar9 != 0)) {
          iVar9 = FUN_1403956a0(param_1->owner);
          if (iVar9 != iVar7) {
LAB_14061f280:
            uVar18 = FUN_1403736a0(lVar10);
            FUN_1403a92f0(param_1->owner,iVar7,0xffff,0,DAT_140b398fc,uVar18,0,0,0,0);
            goto LAB_14061f3b4;
          }
          cVar6 = FUN_14039a180(param_1->owner);
          if (cVar6 == '\0') goto LAB_14061f3b4;
        }
        *(undefined4 *)((longlong)pvVar4 + 0xe8) = 0;
        FUN_140559250(pvVar4,0x10);
        goto switchD_14061ec53_caseD_1;
      case 0x10:
        cVar6 = FUN_140620190(param_1,local_res8);
        break;
      case 0x11:
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          return -0x7f9affff;
        }
        lVar10 = *(longlong *)&param_1->owner->field_0x298;
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        cVar6 = *(char *)(lVar10 + 0x36);
        uVar8 = FUN_14061d810(param_1);
        if ((cVar6 != '\0') && (iVar7 = FUN_14038f9e0(param_1->owner,uVar8), iVar7 != 0)) {
          iVar7 = FUN_14061d810(param_1);
          iVar9 = FUN_1403956a0(param_1->owner);
          if (iVar9 != iVar7) goto LAB_14061f280;
          cVar6 = FUN_14039a180(param_1->owner);
          if (cVar6 == '\0') goto LAB_14061f3b4;
        }
        FUN_140559250(pvVar4,0x12);
        goto switchD_14061ec53_caseD_1;
      case 0x12:
        cVar6 = FUN_14061ff40(param_1,local_res8);
        break;
      case 0x13:
        cVar6 = FUN_14061f840(param_1,local_res8);
        break;
      case 0x14:
        cVar6 = FUN_14061f510(param_1,local_res8);
        break;
      case 0x15:
        cVar6 = FUN_14061fc30(param_1,local_res8);
        break;
      case 0x16:
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          return -0x7f9affff;
        }
        *(undefined4 *)((longlong)pvVar4 + 0xf0) = 0;
        lVar10 = FUN_1405569b0(pvVar4);
        if (lVar10 == 0) {
          if (*(char *)((longlong)pvVar4 + 0x2b3) == '\0') {
            cVar6 = FUN_14039a370(param_1->owner);
            if (((cVar6 == '\0') ||
                (*(float *)(*(longlong *)&param_1->owner->field_0x298 + 0x23b0) <= 0.0)) ||
               (cVar6 = FUN_1403986b0(param_1->owner,2), cVar6 != '\0')) goto LAB_14061f3b4;
            (**(code **)((longlong)param_1->vtable + 0x118))(param_1,1);
          }
          else {
            *(undefined1 *)((longlong)pvVar4 + 0x2b3) = 0;
            FUN_140559250(pvVar4,0x19);
            iVar7 = FUN_14038f9e0(param_1->owner,0x19);
            FUN_1403a92f0(param_1->owner,0x19,0,0,uVar18,fVar13,iVar7 + -1,0,0,0);
          }
        }
        else if (*(char *)(lVar10 + 0x6d) == '\0') {
          *(undefined1 *)(lVar10 + 0x6d) = 1;
          FUN_140559250(pvVar4,0);
        }
        else {
          FUN_140559250(pvVar4,0x15);
        }
        goto switchD_14061ec53_caseD_1;
      case 0x18:
        local_res8[0] = 0x650000;
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          local_res8[0] = -0x7f9affff;
        }
        else {
          iVar7 = FUN_1403956a0();
          if (iVar7 == 0x19) {
            cVar6 = FUN_14039a180(param_1->owner);
            if (cVar6 != '\0') {
              FUN_140559250(pvVar4,0x19);
              ppvVar12 = &param_1->owner->listener_list;
              if (param_1->owner == (GameObjectClass *)0x0) {
                ppvVar12 = (void **)0x0;
              }
              uVar11 = FUN_140058570();
              FUN_140220ed0(uVar11,ppvVar12,0x29);
              FUN_14039fec0(param_1->owner);
            }
          }
          else {
            pGVar1 = param_1->owner;
            if ((pGVar1->field_0x33d == -1) ||
               (cVar6 = FUN_140386170(*(longlong *)&pGVar1->field_0x298 + 0x5b4,
                                      *(longlong *)&pGVar1->field_0xf0 + 0x48), cVar6 != '\0')) {
              iVar9 = FUN_1403956a0(pGVar1);
              iVar7 = 0;
              if (iVar9 == 0x1a) {
                fVar17 = (float)FUN_140395550(param_1->owner);
                iVar9 = FUN_140395570(param_1->owner);
                iVar7 = iVar9 + ~(int)fVar17;
                if (iVar9 + ~(int)fVar17 < 0) {
                  iVar7 = 0;
                }
              }
              FUN_1403a92f0(param_1->owner,0x19,0xffff,0,uVar5,
                            *(undefined4 *)(*(longlong *)&param_1->owner->field_0x298 + 0x1668),
                            iVar7,0,0,0);
            }
            else {
              uVar11 = (**(code **)((longlong)pGVar1->vtable + 0x10))(pGVar1,0xb);
              FUN_1406319d0(uVar11,0);
            }
          }
        }
        goto LAB_14061f1d5;
      case 0x19:
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          local_res8[0] = -0x7f9affff;
        }
        else {
          *(undefined8 *)((longlong)pvVar4 + 0xec) = 0;
          if ((0.0 < *(float *)(*(longlong *)&param_1->owner->field_0x298 + 0x23b0)) &&
             (lVar10 = FUN_1405569b0(pvVar4), lVar10 != 0)) {
            *(undefined1 *)(lVar10 + 0x6d) = 1;
            FUN_140559250(pvVar4,0x12);
            *(undefined1 *)((longlong)pvVar4 + 0x2b3) = 1;
          }
          local_res8[0] = 0x650000;
        }
LAB_14061f1d5:
        cVar6 = '\x01';
        break;
      case 0x1a:
        local_res8[0] = 0x650000;
        pvVar4 = param_1->owner->locomotor_state;
        if (pvVar4 == (void *)0x0) {
          return -0x7f9affff;
        }
        *(undefined1 *)((longlong)pvVar4 + 0x2b3) = 0;
        iVar7 = FUN_1403956a0(param_1->owner);
        if (iVar7 != 0x1a) {
          pGVar1 = param_1->owner;
          if ((pGVar1->field_0x33d == -1) ||
             (cVar6 = FUN_140386170(*(longlong *)&pGVar1->field_0x298 + 0x5b4,
                                    *(longlong *)&pGVar1->field_0xf0 + 0x48), cVar6 != '\0')) {
            iVar9 = FUN_1403956a0(pGVar1);
            iVar7 = 0;
            if (iVar9 == 0x19) {
              iVar7 = FUN_140395570(param_1->owner);
              fVar14 = (float)FUN_140395550(param_1->owner);
              iVar7 = (int)((((float)iVar7 - fVar14) - fVar13) -
                           *(float *)(*(longlong *)&param_1->owner->field_0x298 + 0x1668));
              if (iVar7 < 0) {
                iVar7 = 0;
              }
            }
            FUN_1403a92f0(param_1->owner,0x1a,0xffff,0,uVar5,
                          *(undefined4 *)(*(longlong *)&param_1->owner->field_0x298 + 0x1668),iVar7,
                          0,0,0);
            FUN_1403a0580(param_1->owner);
          }
          else {
            uVar11 = (**(code **)((longlong)pGVar1->vtable + 0x10))(pGVar1,0xb);
            FUN_1406319d0(uVar11,0);
          }
          goto LAB_14061f3b4;
        }
        cVar6 = FUN_14039a180(param_1->owner);
        if (cVar6 == '\0') goto LAB_14061f3b4;
        FUN_140559250(pvVar4,0);
        ppvVar12 = &param_1->owner->listener_list;
        if (param_1->owner == (GameObjectClass *)0x0) {
          ppvVar12 = (void **)0x0;
        }
        uVar11 = FUN_140058570();
        FUN_140220ed0(uVar11,ppvVar12,0x29);
        goto switchD_14061ec53_caseD_1;
      }
      if (local_res8[0] != 0x650000) {
        return local_res8[0];
      }
      if (cVar6 != '\0') {
LAB_14061f3b4:
        uVar11 = (**(code **)((longlong)param_1->vtable + 0x1e8))(param_1,local_b8);
        FUN_1403a84f0(param_1->owner,uVar11);
        if (*(char *)(*(longlong *)&param_1->owner->field_0x298 + 0x74) != '\0') {
          return 0x650000;
        }
        if (*(float *)((longlong)pvVar2 + 0xec) != 0.0) {
          *(undefined1 *)((longlong)pvVar2 + 0x2b1) = 0;
          FUN_1403a7cf0(param_1->owner,0);
          *(undefined1 *)((longlong)pvVar2 + 0x2b0) = 1;
          FUN_1403a7e30(param_1->owner,1);
          return 0x650000;
        }
        *(undefined1 *)((longlong)pvVar2 + 0x2b0) = 0;
        FUN_1403a7e30(param_1->owner,0);
        *(undefined1 *)((longlong)pvVar2 + 0x2b1) = 1;
        FUN_1403a7cf0(param_1->owner,1);
        return 0x650000;
      }
    } while( true );
  }
  lVar10 = *(longlong *)&pGVar1->field_0x298;
  if (lVar10 == 0) {
    return 0x650000;
  }
  local_108 = pGVar1->pos_x;
  local_104 = pGVar1->pos_y;
  local_100 = pGVar1->pos_z;
  fVar13 = (float)FUN_140135140(DAT_140b15418[0x67]);
  if (*(int *)((longlong)pvVar2 + 0x2a8) == 2) {
    if (fVar13 + *(float *)(lVar10 + 0x23bc) <= local_100) {
      local_100 = fVar13 + *(float *)(lVar10 + 0x23bc);
      FUN_1403a8f90(param_1->owner,&local_108,0);
      *(undefined4 *)((longlong)pvVar2 + 0x2a8) = 1;
      *(undefined4 *)((longlong)pvVar2 + 0xec) = 0;
      return 0x650000;
    }
    fVar14 = (float)FUN_140370f00(lVar10,param_1->owner);
    local_100 = local_100 + fVar14;
    if (local_100 <= fVar13 + *(float *)(lVar10 + 0x23bc)) goto LAB_14061eae1;
    fVar14 = *(float *)(lVar10 + 0x23bc);
    *(undefined4 *)((longlong)pvVar2 + 0x2a8) = 1;
    local_100 = fVar13 + fVar14;
  }
  else {
    if (*(int *)((longlong)pvVar2 + 0x2a8) != 3) {
      return 0x650000;
    }
    if (local_100 <= fVar13) {
      local_100 = fVar13;
      FUN_1403a8f90(param_1->owner,&local_108,0);
      *(undefined4 *)((longlong)pvVar2 + 0x2a8) = 0;
      *(undefined4 *)((longlong)pvVar2 + 0xec) = 0;
      return 0x650000;
    }
    fVar14 = (float)FUN_140370f00(lVar10,param_1->owner);
    local_100 = local_100 - fVar14;
    if (fVar13 <= local_100) goto LAB_14061eae1;
    *(undefined4 *)((longlong)pvVar2 + 0x2a8) = 0;
    local_100 = fVar13;
  }
  *(undefined4 *)((longlong)pvVar2 + 0xec) = 0;
LAB_14061eae1:
  FUN_1403a8f90(param_1->owner,&local_108,0);
  return 0x650000;
}


