/* WalkLocomotorBehaviorClass::vfunc_6  RVA 0x61e930  size 2925  params 0  ret undefined */


int WalkLocomotorBehaviorClass::vfunc_6(longlong *param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined4 uVar5;
  char cVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  longlong lVar10;
  longlong lVar11;
  undefined8 uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined4 uVar18;
  int local_res8 [2];
  undefined4 local_108;
  undefined4 local_104;
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
  
  if (param_1[5] == 0) {
    return -0x7f9affff;
  }
  LocomotorCommonClass::vfunc_6();
  cVar6 = (**(code **)(*param_1 + 0x210))(param_1);
  if ((cVar6 != '\0') || (param_1[5] == 0)) {
    return 0x650000;
  }
  if (*(char *)(*(longlong *)(param_1[5] + 0x298) + 0x26) == '\x01') {
    local_res8[0] = FUN_1403956a0();
    cVar6 = FUN_14027b4f0(local_res8);
    if ((cVar6 == '\x01') && (iVar7 = FUN_140395660(param_1[5]), iVar7 != 2)) {
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
  lVar10 = param_1[5];
  lVar1 = *(longlong *)(lVar10 + 0xa8);
  if (lVar1 == 0) {
    return -0x7f9affff;
  }
  if (*(uint *)(lVar1 + 0x2a8) < 2) {
    if ((*(int *)(lVar1 + 0x48) != 0) && (0xb < *(int *)(lVar1 + 0x48) - 0xfU)) {
      return -0x7f9afffe;
    }
    lVar10 = FUN_1405569b0(lVar1);
    if (lVar10 != 0) {
      if (DAT_140b0a320 < *(uint *)(lVar10 + 0x54)) {
        return 0x650000;
      }
      if (*(uint *)(lVar10 + 0x54) == DAT_140b0a320) {
        *(undefined1 *)(lVar1 + 0x275) = 0;
      }
    }
    uVar5 = DAT_140866870;
    fVar14 = DAT_1408524f8;
    uVar18 = DAT_1408007b8;
    fVar13 = DAT_1407ffaf8;
    if (*(longlong *)(param_1[5] + 0x298) == 0) {
      return -0x7f9affff;
    }
    local_res8[0] = 0x650000;
switchD_14061ec53_caseD_1:
    do {
      switch(*(undefined4 *)(lVar1 + 0x48)) {
      case 0:
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        lVar11 = FUN_1405569b0(lVar10);
        if (lVar11 == 0) goto LAB_14061f3b4;
        if (*(char *)(lVar11 + 0x6d) == '\0') {
          FUN_140559250(lVar10,0x16);
          goto LAB_14061f3b4;
        }
        lVar3 = *(longlong *)(param_1[5] + 0x298);
        if (lVar3 == 0) {
          return -0x7f9affff;
        }
        fVar17 = (float)FUN_140372560(lVar3);
        lVar2 = param_1[5];
        local_108 = *(undefined4 *)(lVar2 + 0x84);
        local_104 = *(undefined4 *)(lVar2 + 0x88);
        local_100 = *(float *)(lVar2 + 0x8c);
        FUN_14061d570(param_1,local_e8,0);
        FUN_14020b100(local_e8,local_d8);
        fVar15 = (float)FUN_14020b6d0(local_d0);
        fVar16 = (float)FUN_140371310(lVar3);
        lVar11 = FUN_1405c4b80(lVar11,*(undefined4 *)(lVar11 + 0x60));
        if ((ABS(fVar15) <= fVar17) || ((ABS(fVar15) <= fVar16 && (0.0 < *(float *)(lVar11 + 4)))))
        {
          FUN_140559250(lVar10,0x12);
        }
        else {
          FUN_140559250(lVar10,0xf);
        }
      default:
        goto switchD_14061ec53_caseD_1;
      case 0xf:
        lVar10 = param_1[5];
        lVar3 = *(longlong *)(lVar10 + 0xa8);
        if (lVar3 == 0) {
          return -0x7f9affff;
        }
        lVar11 = *(longlong *)(lVar10 + 0x298);
        if (lVar11 == 0) {
          return -0x7f9affff;
        }
        local_f8 = *(undefined4 *)(lVar10 + 0x84);
        cVar6 = *(char *)(lVar11 + 0x36);
        local_f4 = *(undefined4 *)(lVar10 + 0x88);
        local_f0 = *(undefined4 *)(lVar10 + 0x8c);
        FUN_14061d570(param_1,local_c8,0);
        FUN_14020b100(local_c8,local_b8);
        if ((local_b0 <= 0.0) || (iVar7 = 0xc, fVar14 <= local_b0)) {
          iVar7 = 0xe;
        }
        if ((cVar6 != '\0') && (iVar9 = FUN_14038f9e0(param_1[5],iVar7), iVar9 != 0)) {
          iVar9 = FUN_1403956a0(param_1[5]);
          if (iVar9 != iVar7) {
LAB_14061f280:
            uVar18 = FUN_1403736a0(lVar11);
            FUN_1403a92f0(param_1[5],iVar7,0xffff,0,DAT_140b398fc,uVar18,0,0,0,0);
            goto LAB_14061f3b4;
          }
          cVar6 = FUN_14039a180(param_1[5]);
          if (cVar6 == '\0') goto LAB_14061f3b4;
        }
        *(undefined4 *)(lVar3 + 0xe8) = 0;
        FUN_140559250(lVar3,0x10);
        goto switchD_14061ec53_caseD_1;
      case 0x10:
        cVar6 = FUN_140620190(param_1,local_res8);
        break;
      case 0x11:
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        lVar11 = *(longlong *)(param_1[5] + 0x298);
        if (lVar11 == 0) {
          return -0x7f9affff;
        }
        cVar6 = *(char *)(lVar11 + 0x36);
        uVar8 = FUN_14061d810(param_1);
        if ((cVar6 != '\0') && (iVar7 = FUN_14038f9e0(param_1[5],uVar8), iVar7 != 0)) {
          iVar7 = FUN_14061d810(param_1);
          iVar9 = FUN_1403956a0(param_1[5]);
          if (iVar9 != iVar7) goto LAB_14061f280;
          cVar6 = FUN_14039a180(param_1[5]);
          if (cVar6 == '\0') goto LAB_14061f3b4;
        }
        FUN_140559250(lVar10,0x12);
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
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        *(undefined4 *)(lVar10 + 0xf0) = 0;
        lVar11 = FUN_1405569b0(lVar10);
        if (lVar11 == 0) {
          if (*(char *)(lVar10 + 0x2b3) == '\0') {
            cVar6 = FUN_14039a370(param_1[5]);
            if (((cVar6 == '\0') || (*(float *)(*(longlong *)(param_1[5] + 0x298) + 0x23b0) <= 0.0))
               || (cVar6 = FUN_1403986b0(param_1[5],2), cVar6 != '\0')) goto LAB_14061f3b4;
            (**(code **)(*param_1 + 0x118))(param_1,1);
          }
          else {
            *(undefined1 *)(lVar10 + 0x2b3) = 0;
            FUN_140559250(lVar10,0x19);
            iVar7 = FUN_14038f9e0(param_1[5],0x19);
            FUN_1403a92f0(param_1[5],0x19,0,0,uVar18,fVar13,iVar7 + -1,0,0,0);
          }
        }
        else if (*(char *)(lVar11 + 0x6d) == '\0') {
          *(undefined1 *)(lVar11 + 0x6d) = 1;
          FUN_140559250(lVar10,0);
        }
        else {
          FUN_140559250(lVar10,0x15);
        }
        goto switchD_14061ec53_caseD_1;
      case 0x18:
        local_res8[0] = 0x650000;
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          local_res8[0] = -0x7f9affff;
        }
        else {
          iVar7 = FUN_1403956a0();
          if (iVar7 == 0x19) {
            cVar6 = FUN_14039a180(param_1[5]);
            if (cVar6 != '\0') {
              FUN_140559250(lVar10,0x19);
              lVar10 = param_1[5] + 0x38;
              if (param_1[5] == 0) {
                lVar10 = 0;
              }
              uVar12 = FUN_140058570();
              FUN_140220ed0(uVar12,lVar10,0x29);
              FUN_14039fec0(param_1[5]);
            }
          }
          else {
            plVar4 = (longlong *)param_1[5];
            if ((*(char *)((longlong)plVar4 + 0x33d) == -1) ||
               (cVar6 = FUN_140386170(plVar4[0x53] + 0x5b4,plVar4[0x1e] + 0x48), cVar6 != '\0')) {
              iVar9 = FUN_1403956a0(plVar4);
              iVar7 = 0;
              if (iVar9 == 0x1a) {
                fVar17 = (float)FUN_140395550(param_1[5]);
                iVar9 = FUN_140395570(param_1[5]);
                iVar7 = iVar9 + ~(int)fVar17;
                if (iVar9 + ~(int)fVar17 < 0) {
                  iVar7 = 0;
                }
              }
              FUN_1403a92f0(param_1[5],0x19,0xffff,0,uVar5,
                            *(undefined4 *)(*(longlong *)(param_1[5] + 0x298) + 0x1668),iVar7,0,0,0)
              ;
            }
            else {
              uVar12 = (**(code **)(*plVar4 + 0x10))(plVar4,0xb);
              FUN_1406319d0(uVar12,0);
            }
          }
        }
        goto LAB_14061f1d5;
      case 0x19:
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          local_res8[0] = -0x7f9affff;
        }
        else {
          *(undefined8 *)(lVar10 + 0xec) = 0;
          if ((0.0 < *(float *)(*(longlong *)(param_1[5] + 0x298) + 0x23b0)) &&
             (lVar11 = FUN_1405569b0(lVar10), lVar11 != 0)) {
            *(undefined1 *)(lVar11 + 0x6d) = 1;
            FUN_140559250(lVar10,0x12);
            *(undefined1 *)(lVar10 + 0x2b3) = 1;
          }
          local_res8[0] = 0x650000;
        }
LAB_14061f1d5:
        cVar6 = '\x01';
        break;
      case 0x1a:
        local_res8[0] = 0x650000;
        lVar10 = *(longlong *)(param_1[5] + 0xa8);
        if (lVar10 == 0) {
          return -0x7f9affff;
        }
        *(undefined1 *)(lVar10 + 0x2b3) = 0;
        iVar7 = FUN_1403956a0(param_1[5]);
        if (iVar7 != 0x1a) {
          plVar4 = (longlong *)param_1[5];
          if ((*(char *)((longlong)plVar4 + 0x33d) == -1) ||
             (cVar6 = FUN_140386170(plVar4[0x53] + 0x5b4,plVar4[0x1e] + 0x48), cVar6 != '\0')) {
            iVar9 = FUN_1403956a0(plVar4);
            iVar7 = 0;
            if (iVar9 == 0x19) {
              iVar7 = FUN_140395570(param_1[5]);
              fVar14 = (float)FUN_140395550(param_1[5]);
              iVar7 = (int)((((float)iVar7 - fVar14) - fVar13) -
                           *(float *)(*(longlong *)(param_1[5] + 0x298) + 0x1668));
              if (iVar7 < 0) {
                iVar7 = 0;
              }
            }
            FUN_1403a92f0(param_1[5],0x1a,0xffff,0,uVar5,
                          *(undefined4 *)(*(longlong *)(param_1[5] + 0x298) + 0x1668),iVar7,0,0,0);
            FUN_1403a0580(param_1[5]);
          }
          else {
            uVar12 = (**(code **)(*plVar4 + 0x10))(plVar4,0xb);
            FUN_1406319d0(uVar12,0);
          }
          goto LAB_14061f3b4;
        }
        cVar6 = FUN_14039a180(param_1[5]);
        if (cVar6 == '\0') goto LAB_14061f3b4;
        FUN_140559250(lVar10,0);
        lVar10 = param_1[5] + 0x38;
        if (param_1[5] == 0) {
          lVar10 = 0;
        }
        uVar12 = FUN_140058570();
        FUN_140220ed0(uVar12,lVar10,0x29);
        goto switchD_14061ec53_caseD_1;
      }
      if (local_res8[0] != 0x650000) {
        return local_res8[0];
      }
      if (cVar6 != '\0') {
LAB_14061f3b4:
        uVar12 = (**(code **)(*param_1 + 0x1e8))(param_1,local_b8);
        FUN_1403a84f0(param_1[5],uVar12);
        if (*(char *)(*(longlong *)(param_1[5] + 0x298) + 0x74) != '\0') {
          return 0x650000;
        }
        if (*(float *)(lVar1 + 0xec) != 0.0) {
          *(undefined1 *)(lVar1 + 0x2b1) = 0;
          FUN_1403a7cf0(param_1[5],0);
          *(undefined1 *)(lVar1 + 0x2b0) = 1;
          FUN_1403a7e30(param_1[5],1);
          return 0x650000;
        }
        *(undefined1 *)(lVar1 + 0x2b0) = 0;
        FUN_1403a7e30(param_1[5],0);
        *(undefined1 *)(lVar1 + 0x2b1) = 1;
        FUN_1403a7cf0(param_1[5],1);
        return 0x650000;
      }
    } while( true );
  }
  lVar11 = *(longlong *)(lVar10 + 0x298);
  if (lVar11 == 0) {
    return 0x650000;
  }
  local_108 = *(undefined4 *)(lVar10 + 0x78);
  local_104 = *(undefined4 *)(lVar10 + 0x7c);
  local_100 = *(float *)(lVar10 + 0x80);
  fVar13 = (float)FUN_140135140(DAT_140b15418[0x67]);
  if (*(int *)(lVar1 + 0x2a8) == 2) {
    if (fVar13 + *(float *)(lVar11 + 0x23bc) <= local_100) {
      local_100 = fVar13 + *(float *)(lVar11 + 0x23bc);
      FUN_1403a8f90(param_1[5],&local_108,0);
      *(undefined4 *)(lVar1 + 0x2a8) = 1;
      *(undefined4 *)(lVar1 + 0xec) = 0;
      return 0x650000;
    }
    fVar14 = (float)FUN_140370f00(lVar11,param_1[5]);
    local_100 = local_100 + fVar14;
    if (local_100 <= fVar13 + *(float *)(lVar11 + 0x23bc)) goto LAB_14061eae1;
    fVar14 = *(float *)(lVar11 + 0x23bc);
    *(undefined4 *)(lVar1 + 0x2a8) = 1;
    local_100 = fVar13 + fVar14;
  }
  else {
    if (*(int *)(lVar1 + 0x2a8) != 3) {
      return 0x650000;
    }
    if (local_100 <= fVar13) {
      local_100 = fVar13;
      FUN_1403a8f90(param_1[5],&local_108,0);
      *(undefined4 *)(lVar1 + 0x2a8) = 0;
      *(undefined4 *)(lVar1 + 0xec) = 0;
      return 0x650000;
    }
    fVar14 = (float)FUN_140370f00(lVar11,param_1[5]);
    local_100 = local_100 - fVar14;
    if (fVar13 <= local_100) goto LAB_14061eae1;
    *(undefined4 *)(lVar1 + 0x2a8) = 0;
    local_100 = fVar13;
  }
  *(undefined4 *)(lVar1 + 0xec) = 0;
LAB_14061eae1:
  FUN_1403a8f90(param_1[5],&local_108,0);
  return 0x650000;
}


