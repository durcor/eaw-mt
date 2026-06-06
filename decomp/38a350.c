/* FUN_14038a350  RVA 0x38a350  size 8490  params 0  ret undefined */


/* WARNING: Type propagation algorithm not settling */

ulonglong FUN_14038a350(longlong *param_1,int param_2,char param_3,undefined8 *param_4,
                       undefined4 param_5,undefined4 param_6)

{
  float fVar1;
  char cVar2;
  byte bVar3;
  int iVar4;
  ulonglong uVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong *plVar8;
  ulonglong uVar9;
  int iVar10;
  uint uVar11;
  longlong lVar12;
  float fVar13;
  undefined8 in_stack_ffffffffffffff48;
  undefined4 uVar14;
  longlong *local_68;
  undefined4 local_60;
  longlong local_58 [6];
  
  lVar12 = (longlong)param_2;
  uVar5 = (ulonglong)(param_2 - 1U);
  if (((0x4b < param_2 - 1U) || (uVar5 = FUN_1403986b0(param_1,param_2), (char)uVar5 == '\0')) ||
     ((param_3 != '\0' &&
      ((uVar5 = FUN_14039b480(param_1,param_2,0,1), (char)uVar5 == '\0' ||
       ((uVar5 = FUN_14039b950(param_1,param_2,1), (char)uVar5 != '\0' &&
        ((((param_2 != 5 || (uVar5 = param_1[0x2f], uVar5 == 0)) || (*(char *)(uVar5 + 0xc) == '\0')
          ) || ((param_1[0x84] != 0 && ((char)param_1[0x85] != '\0')))))))))))) goto LAB_14038a897;
  cVar2 = FUN_14043fa90(&DAT_140b2f2e0,param_2);
  uVar14 = (undefined4)((ulonglong)in_stack_ffffffffffffff48 >> 0x20);
  uVar9 = 0;
  if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar10 = 0;
    iVar4 = FUN_140405300(uVar6);
    uVar14 = (undefined4)((ulonglong)in_stack_ffffffffffffff48 >> 0x20);
    if (0 < iVar4) {
      do {
        lVar7 = FUN_1404052d0(uVar6,iVar10);
        if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,param_2), cVar2 != '\0')) {
          local_68 = (longlong *)*param_4;
          uVar9 = 1;
          local_60 = *(undefined4 *)(param_4 + 1);
          in_stack_ffffffffffffff48 =
               CONCAT44((int)((ulonglong)in_stack_ffffffffffffff48 >> 0x20),param_5);
          uVar5 = FUN_14038a350(lVar7,param_2,param_3,&local_68,in_stack_ffffffffffffff48,0x3fffff);
          if ((char)uVar5 == '\0') goto LAB_14038a897;
        }
        iVar10 = iVar10 + 1;
        iVar4 = FUN_140405300(uVar6);
        uVar14 = (undefined4)((ulonglong)in_stack_ffffffffffffff48 >> 0x20);
      } while (iVar10 < iVar4);
    }
    cVar2 = FUN_1403986b0(param_1,param_2);
    if (cVar2 == '\0') {
      return uVar9;
    }
    if (param_1[(lVar12 + 0x3d) * 2] == 0) {
      return uVar9;
    }
    if (param_2 != 7) {
      return uVar9;
    }
  }
  uVar9 = 0;
  bVar3 = 0;
  cVar2 = FUN_1403986b0(param_1,param_2);
  if (cVar2 == '\0') goto switchD_14038a556_caseD_4;
  lVar7 = FUN_140397d90(param_1,param_2);
  uVar5 = 0;
  if (lVar7 == 0) goto LAB_14038a897;
  uVar5 = 0;
  iVar4 = 0;
  switch(param_2) {
  case 1:
  case 9:
  case 0x1d:
  case 0x2b:
  case 0x3a:
    bVar3 = FUN_1403a4250(param_1,param_2,param_3);
    break;
  case 2:
    bVar3 = FUN_1403a1ce0(param_1,param_3);
    break;
  case 3:
    bVar3 = FUN_1403a2240(param_1,param_3,param_5);
    break;
  default:
    goto switchD_14038a556_caseD_4;
  case 5:
    lVar7 = param_1[0x2f];
    if (lVar7 == 0) goto LAB_14038a897;
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    uVar5 = FUN_1403a1930(param_1,param_3,&local_68);
    bVar3 = (byte)uVar5;
    if ((param_3 != '\0') && (*(char *)(lVar7 + 0xc) == '\0')) {
      return uVar5;
    }
    break;
  case 6:
  case 10:
  case 0xc:
  case 0x26:
  case 0x27:
    bVar3 = FUN_1403a4070(param_1,param_2,param_3);
    break;
  case 7:
    bVar3 = FUN_1403a3500(param_1,param_3);
    break;
  case 0xb:
    bVar3 = FUN_1403a3260(param_1,param_3);
    break;
  case 0xe:
    bVar3 = FUN_1403a1fb0(param_1,param_3);
    break;
  case 0x11:
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    bVar3 = FUN_1403a38d0(param_1,param_3,&local_68);
    break;
  case 0x12:
    if (param_3 == '\0') {
      cVar2 = FUN_14039b950(param_1,param_2,1);
      if (((cVar2 != '\0') && ((char)param_1[0x6f] != -1)) &&
         (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) {
        FUN_14042fb70(lVar7,param_2);
      }
      goto LAB_14038b0b7;
    }
LAB_14038b161:
    if (((char)param_1[0x6f] != -1) &&
       (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), iVar4 = param_2, lVar7 != 0))
    goto LAB_14038c440;
    goto switchD_14038a556_caseD_16;
  case 0x13:
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    bVar3 = FUN_1403a3e20(param_1,0x13,param_3,&local_68,CONCAT44(uVar14,param_6));
    break;
  case 0x16:
  case 0x1e:
  case 0x1f:
  case 0x22:
  case 0x25:
  case 0x29:
  case 0x2d:
  case 0x37:
    goto switchD_14038a556_caseD_16;
  case 0x17:
    local_68 = (longlong *)0x0;
    local_60 = 0;
    bVar3 = FUN_1403a3e20(param_1,0x17,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x18:
    local_68 = (longlong *)0x0;
    local_60 = 0;
    bVar3 = FUN_1403a3e20(param_1,0x18,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x19:
    local_68 = (longlong *)0x0;
    local_60 = 0;
    bVar3 = FUN_1403a3e20(param_1,0x19,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x1a:
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    bVar3 = FUN_1403a3e20(param_1,0x1a,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x1c:
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    bVar3 = FUN_1403a3e20(param_1,0x1c,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x20:
    cVar2 = FUN_14039b480(param_1,param_2,1);
    if (cVar2 == '\0') {
      if (*(char *)(param_1[(lVar12 + 0x3d) * 2] + 0x10c) != '\0') {
        plVar8 = param_1 + 0x75;
        *(int *)plVar8 = (int)*plVar8 + -1;
        iVar10 = (int)param_1[0x75];
        if ((int)*plVar8 < 0) {
          iVar10 = iVar4;
        }
        *(int *)(param_1 + 0x75) = iVar10;
      }
      if ((param_1[0x53] != 0) && (*(char *)(param_1[0x53] + 0xd1) != '\0')) {
        FUN_1403ac100(param_1);
      }
    }
    else if (param_3 == '\0') {
      cVar2 = FUN_14039b950(param_1,param_2,1);
      if (cVar2 != '\0') {
        if (*(char *)(param_1[(lVar12 + 0x3d) * 2] + 0x10c) != '\0') {
          plVar8 = param_1 + 0x75;
          *(int *)plVar8 = (int)*plVar8 + -1;
          iVar10 = (int)param_1[0x75];
          if ((int)*plVar8 < 0) {
            iVar10 = iVar4;
          }
          *(int *)(param_1 + 0x75) = iVar10;
        }
        if ((param_1[0x53] != 0) && (*(char *)(param_1[0x53] + 0xd1) != '\0')) {
          FUN_1403ac100(param_1);
        }
        if ((char)param_1[0x6f] != -1) {
          lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46);
          if (lVar7 == 0) {
            return 1;
          }
          cVar2 = FUN_14042f7a0(lVar7,param_2);
          if (cVar2 == '\0') {
            FUN_14042f320(lVar7,param_2);
          }
        }
      }
    }
    else {
      local_58[2] = 0;
      local_58[0] = 0;
      local_58[1] = 0;
      local_68 = param_1;
      FUN_140045580(local_58,0,&local_68);
      lVar7 = (**(code **)(*DAT_140b15418 + 0x228))();
      if (lVar7 == 0) {
        uVar5 = 0;
        if (local_58[0] != 0) {
          uVar5 = local_58[2] - local_58[0] & 0xfffffffffffffff8;
          if (0xfff < uVar5) {
            lVar7 = *(longlong *)(local_58[0] + -8);
            uVar5 = uVar5 + 0x27;
            if (0x1f < (local_58[0] - lVar7) - 8U) goto LAB_14038c475;
          }
          uVar5 = thunk_FUN_1407864b8();
        }
        goto LAB_14038a897;
      }
      FUN_1404261f0(lVar7,local_58);
      if (((((param_1[0x53] != 0) && (*(char *)(param_1[0x53] + 0xd1) != '\0')) &&
           (*(char *)((longlong)param_1 + 0x333) != -1)) &&
          ((param_1[0x15] == 0 || (*(char *)(param_1[0x15] + 0x2b3) == '\0')))) &&
         (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0)
         ) {
        (**(code **)(*plVar8 + 0x118))(plVar8,1);
      }
      if (*(char *)(param_1[(lVar12 + 0x3d) * 2] + 0x10c) != '\0') {
        iVar4 = (int)param_1[0x75];
        if (iVar4 == 0) {
          uVar6 = (**(code **)(*DAT_140b15418 + 0x228))();
          lVar7 = FUN_1404261d0(uVar6,param_1);
          if (lVar7 != 0) {
            FUN_140424850(lVar7,param_1);
          }
          FUN_14039bcb0(param_1);
          iVar4 = (int)param_1[0x75];
        }
        *(int *)(param_1 + 0x75) = iVar4 + 1;
      }
      FUN_1403a97e0(param_1,param_2,param_3);
      if (local_58[0] != 0) {
        uVar5 = local_58[2] - local_58[0] & 0xfffffffffffffff8;
        lVar7 = local_58[0];
        if (0xfff < uVar5) {
          lVar7 = *(longlong *)(local_58[0] + -8);
          uVar5 = uVar5 + 0x27;
          if (0x1f < (local_58[0] - lVar7) - 8U) {
LAB_14038c475:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(lVar7,uVar5);
          }
        }
        thunk_FUN_1407864b8(lVar7,uVar5);
      }
    }
    if (param_1[0x54] != 0) {
      FUN_140265ae0(param_1[0x54],7,param_3);
    }
    goto switchD_14038a556_caseD_16;
  case 0x21:
    cVar2 = FUN_14039b480(param_1,param_2,1);
    if ((cVar2 != '\0') && ((char)param_1[0x6f] != -1)) {
      lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46);
      if (param_3 == '\0') {
        if (lVar7 == 0) {
          return 1;
        }
        cVar2 = FUN_14042f7a0(lVar7,param_2);
        if (cVar2 == '\0') {
          FUN_14042f320(lVar7,param_2);
        }
      }
      else if (lVar7 != 0) {
        FUN_14042fde0(lVar7,param_2);
      }
    }
    if (param_1[0x54] != 0) {
      FUN_140265ae0(param_1[0x54],6,param_3);
    }
    goto switchD_14038a556_caseD_16;
  case 0x23:
  case 0x24:
    cVar2 = FUN_14039b480(param_1,param_2,1);
    if (cVar2 == '\0') goto switchD_14038a556_caseD_4;
    local_68 = (longlong *)param_1[0xf];
    local_60 = (undefined4)param_1[0x10];
    bVar3 = FUN_1403a3e20(param_1,param_2,param_3,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x28:
    goto switchD_14038a556_caseD_28;
  case 0x2a:
    local_68 = (longlong *)*param_4;
    local_60 = *(undefined4 *)(param_4 + 1);
    bVar3 = FUN_1403a3e20(param_1,0x2a,1,&local_68,CONCAT44(uVar14,0x3fffff));
    break;
  case 0x2c:
    bVar3 = FUN_1403a2970(param_1,param_3);
    break;
  case 0x2e:
    if (param_3 != '\0') goto LAB_14038b161;
LAB_14038b0b7:
    cVar2 = FUN_14039b950(param_1,0x2e,1);
    if (cVar2 != '\0') {
      uVar5 = param_1[0x1f];
      if ((uVar5 == 0) || (*(int *)(uVar5 + 0x28) < 1)) {
LAB_14038a897:
        return uVar5 & 0xffffffffffffff00;
      }
      uVar6 = **(undefined8 **)(uVar5 + 0x20);
      if ((*(byte *)(param_1 + 0x74) & 0x40) == 0) {
        FUN_1403a9e30(param_1,0xd,DAT_140803524,uVar6,0,0,0,0,0,0xffffffff,0,0,0,0);
      }
      FUN_1403a9840(uVar6,0x12);
    }
    goto switchD_14038a556_caseD_16;
  case 0x30:
    if (param_3 == '\0') {
LAB_14038b412:
      if ((((char)param_1[0x6f] != -1) &&
          (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
         (cVar2 = FUN_14042f7a0(lVar7,param_2), cVar2 == '\0')) {
        FUN_14042f320(lVar7,param_2);
        FUN_140592d00(param_1);
      }
    }
    else {
      FUN_1403a97e0(param_1,param_2,param_3);
      if (((param_1[0x20] != 0) &&
          ((param_1[0x1e] == 0 || (cVar2 = FUN_140380bb0(), cVar2 == '\0')))) &&
         ((param_1[0x1e] == 0 ||
          ((cVar2 = FUN_140380b10(), cVar2 == '\0' ||
           (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x30), cVar2 == '\0')))))) {
        if (*(char *)((longlong)param_1 + 0x3a7) == '\0') {
LAB_14038b259:
          if ((((param_1[0x20] == 0) || (*(char *)(param_1[0x20] + 0x28) == '\0')) &&
              (*(char *)((longlong)param_1 + 0x38a) == -1)) &&
             ((((char)param_1[0x6f] == -1 ||
               (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 == 0)) ||
              ((cVar2 = FUN_14042f8e0(lVar7,0x30), cVar2 != '\0' ||
               (cVar2 = FUN_14042f7a0(lVar7,0x30), cVar2 == '\0')))))) {
            if ((char)param_1[0x69] != -1) {
              uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
              iVar4 = FUN_140405300(uVar6);
              uVar5 = uVar9;
              if (0 < iVar4) {
                do {
                  lVar7 = FUN_1404052d0(uVar6,uVar5);
                  if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x30,1), cVar2 == '\0'))
                  goto switchD_14038a556_caseD_16;
                  uVar11 = (int)uVar5 + 1;
                  iVar4 = FUN_140405300(uVar6);
                  uVar5 = (ulonglong)uVar11;
                } while ((int)uVar11 < iVar4);
              }
            }
            if (((*(char *)((longlong)param_1 + 0x333) == -1) ||
                (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1),
                plVar8 == (longlong *)0x0)) ||
               (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 == '\0')) {
              cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x30);
              if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
                uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
                iVar4 = FUN_140405300(uVar6);
                if (0 < iVar4) {
                  do {
                    lVar7 = FUN_1404052d0(uVar6,uVar9);
                    if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x30), cVar2 != '\0')) {
                      lVar7 = param_1[0xda];
                      goto LAB_14038b3ec;
                    }
                    uVar11 = (int)uVar9 + 1;
                    uVar9 = (ulonglong)uVar11;
                    iVar4 = FUN_140405300(uVar6);
                  } while ((int)uVar11 < iVar4);
                }
              }
              else {
                lVar7 = param_1[0xda];
                if (lVar7 != 0) {
LAB_14038b3ec:
                  FUN_14037b8a0(param_1[0x20] + 8,*(undefined4 *)(lVar7 + 0x88),0,1,0);
                }
              }
            }
          }
        }
        else if (*(char *)((longlong)param_1 + 899) == -1) {
          lVar7 = param_1[0x56];
          if (((lVar7 != 0) && (*(char *)(lVar7 + 899) != -1)) &&
             (*(longlong *)(lVar7 + 0x210) != 0)) {
            iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
            goto LAB_14038b253;
          }
        }
        else if (param_1[0x42] != 0) {
          iVar4 = *(int *)(param_1[0x42] + 8);
LAB_14038b253:
          if (iVar4 != 0) goto LAB_14038b259;
        }
      }
    }
    goto switchD_14038a556_caseD_16;
  case 0x31:
    if (((param_1[0x20] == 0) || ((param_1[0x1e] != 0 && (cVar2 = FUN_140380bb0(), cVar2 != '\0'))))
       || ((param_1[0x1e] != 0 &&
           ((cVar2 = FUN_140380b10(), cVar2 != '\0' &&
            (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x31), cVar2 != '\0'))))))
    goto switchD_14038a556_caseD_4;
    if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
      if (*(char *)((longlong)param_1 + 899) == -1) {
        lVar7 = param_1[0x56];
        if (((lVar7 == 0) || (*(char *)(lVar7 + 899) == -1)) || (*(longlong *)(lVar7 + 0x210) == 0))
        goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
      }
      else {
        if (param_1[0x42] == 0) goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(param_1[0x42] + 8);
      }
      if (iVar4 == 0) goto switchD_14038a556_caseD_4;
    }
    if ((((param_1[0x20] != 0) && (*(char *)(param_1[0x20] + 0x28) != '\0')) ||
        (*(char *)((longlong)param_1 + 0x38a) != -1)) ||
       ((((char)param_1[0x6f] != -1 &&
         (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
        ((cVar2 = FUN_14042f8e0(lVar7,0x31), cVar2 == '\0' &&
         (cVar2 = FUN_14042f7a0(lVar7,0x31), cVar2 != '\0')))))) goto switchD_14038a556_caseD_4;
    if ((char)param_1[0x69] != -1) {
      uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
      iVar4 = FUN_140405300(uVar6);
      uVar5 = uVar9;
      if (0 < iVar4) {
        do {
          lVar7 = FUN_1404052d0(uVar6,uVar5);
          if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x31,1), cVar2 == '\0'))
          goto switchD_14038a556_caseD_4;
          uVar11 = (int)uVar5 + 1;
          uVar5 = (ulonglong)uVar11;
          iVar4 = FUN_140405300(uVar6);
        } while ((int)uVar11 < iVar4);
      }
    }
    if (((*(char *)((longlong)param_1 + 0x333) != -1) &&
        (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0))
       && (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 != '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x31);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0xdc];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x31), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0xdc];
    goto LAB_14038c2b0;
  case 0x34:
    if (((param_1[0x20] == 0) || ((param_1[0x1e] != 0 && (cVar2 = FUN_140380bb0(), cVar2 != '\0'))))
       || ((param_1[0x1e] != 0 &&
           ((cVar2 = FUN_140380b10(), cVar2 != '\0' &&
            (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x34), cVar2 != '\0'))))))
    goto switchD_14038a556_caseD_4;
    if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
      if (*(char *)((longlong)param_1 + 899) == -1) {
        lVar7 = param_1[0x56];
        if (((lVar7 == 0) || (*(char *)(lVar7 + 899) == -1)) || (*(longlong *)(lVar7 + 0x210) == 0))
        goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
      }
      else {
        if (param_1[0x42] == 0) goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(param_1[0x42] + 8);
      }
      if (iVar4 == 0) goto switchD_14038a556_caseD_4;
    }
    if ((((param_1[0x20] != 0) && (*(char *)(param_1[0x20] + 0x28) != '\0')) ||
        (*(char *)((longlong)param_1 + 0x38a) != -1)) ||
       ((((char)param_1[0x6f] != -1 &&
         (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
        ((cVar2 = FUN_14042f8e0(lVar7,0x34), cVar2 == '\0' &&
         (cVar2 = FUN_14042f7a0(lVar7,0x34), cVar2 != '\0')))))) goto switchD_14038a556_caseD_4;
    if ((char)param_1[0x69] != -1) {
      uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
      iVar4 = FUN_140405300(uVar6);
      uVar5 = uVar9;
      if (0 < iVar4) {
        do {
          lVar7 = FUN_1404052d0(uVar6,uVar5);
          if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x34,1), cVar2 == '\0'))
          goto switchD_14038a556_caseD_4;
          uVar11 = (int)uVar5 + 1;
          uVar5 = (ulonglong)uVar11;
          iVar4 = FUN_140405300(uVar6);
        } while ((int)uVar11 < iVar4);
      }
    }
    if ((((*(char *)((longlong)param_1 + 0x333) != -1) &&
         (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0)
         ) && (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 != '\0')) ||
       ((param_1[0x20] != 0 && (cVar2 = FUN_14037c520(param_1[0x20] + 8), cVar2 != '\0'))))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x34);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0xe2];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x34), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0xe2];
    goto LAB_14038c2b0;
  case 0x36:
    if (param_3 == '\0') goto LAB_14038b412;
    FUN_1403a97e0(param_1,param_2,param_3);
    if (((param_1[0x20] != 0) && ((param_1[0x1e] == 0 || (cVar2 = FUN_140380bb0(), cVar2 == '\0'))))
       && ((param_1[0x1e] == 0 ||
           ((cVar2 = FUN_140380b10(), cVar2 == '\0' ||
            (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x36), cVar2 == '\0')))))) {
      if (*(char *)((longlong)param_1 + 0x3a7) == '\0') {
LAB_14038b9db:
        if ((((param_1[0x20] == 0) || (*(char *)(param_1[0x20] + 0x28) == '\0')) &&
            (*(char *)((longlong)param_1 + 0x38a) == -1)) &&
           ((((char)param_1[0x6f] == -1 ||
             (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 == 0)) ||
            ((cVar2 = FUN_14042f8e0(lVar7,0x36), cVar2 != '\0' ||
             (cVar2 = FUN_14042f7a0(lVar7,0x36), cVar2 == '\0')))))) {
          if ((char)param_1[0x69] != -1) {
            uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
            iVar4 = FUN_140405300(uVar6);
            uVar5 = uVar9;
            if (0 < iVar4) {
              do {
                lVar7 = FUN_1404052d0(uVar6,uVar5);
                if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x36,1), cVar2 == '\0'))
                goto switchD_14038a556_caseD_16;
                uVar11 = (int)uVar5 + 1;
                uVar5 = (ulonglong)uVar11;
                iVar4 = FUN_140405300(uVar6);
              } while ((int)uVar11 < iVar4);
            }
          }
          if (((*(char *)((longlong)param_1 + 0x333) == -1) ||
              (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1),
              plVar8 == (longlong *)0x0)) ||
             (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 == '\0')) {
            cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x36);
            if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
              uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
              iVar4 = FUN_140405300(uVar6);
              if (0 < iVar4) {
                do {
                  lVar7 = FUN_1404052d0(uVar6,uVar9);
                  if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x36), cVar2 != '\0')) {
                    lVar7 = param_1[0xe6];
                    goto LAB_14038bb6c;
                  }
                  uVar11 = (int)uVar9 + 1;
                  uVar9 = (ulonglong)uVar11;
                  iVar4 = FUN_140405300(uVar6);
                } while ((int)uVar11 < iVar4);
              }
            }
            else {
              lVar7 = param_1[0xe6];
              if (lVar7 != 0) {
LAB_14038bb6c:
                FUN_14037b8a0(param_1[0x20] + 8,*(undefined4 *)(lVar7 + 0x88),0,1,0);
              }
            }
          }
        }
      }
      else if (*(char *)((longlong)param_1 + 899) == -1) {
        lVar7 = param_1[0x56];
        if (((lVar7 != 0) && (*(char *)(lVar7 + 899) != -1)) && (*(longlong *)(lVar7 + 0x210) != 0))
        {
          iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
          goto LAB_14038b9d5;
        }
      }
      else if (param_1[0x42] != 0) {
        iVar4 = *(int *)(param_1[0x42] + 8);
LAB_14038b9d5:
        if (iVar4 != 0) goto LAB_14038b9db;
      }
    }
    goto switchD_14038a556_caseD_16;
  case 0x38:
    if (((param_1[0x20] == 0) || ((param_1[0x1e] != 0 && (cVar2 = FUN_140380bb0(), cVar2 != '\0'))))
       || ((param_1[0x1e] != 0 &&
           ((cVar2 = FUN_140380b10(), cVar2 != '\0' &&
            (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x38), cVar2 != '\0'))))))
    goto switchD_14038a556_caseD_4;
    if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
      if (*(char *)((longlong)param_1 + 899) == -1) {
        lVar7 = param_1[0x56];
        if (((lVar7 == 0) || (*(char *)(lVar7 + 899) == -1)) || (*(longlong *)(lVar7 + 0x210) == 0))
        goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
      }
      else {
        if (param_1[0x42] == 0) goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(param_1[0x42] + 8);
      }
      if (iVar4 == 0) goto switchD_14038a556_caseD_4;
    }
    if ((((param_1[0x20] != 0) && (*(char *)(param_1[0x20] + 0x28) != '\0')) ||
        (*(char *)((longlong)param_1 + 0x38a) != -1)) ||
       ((((char)param_1[0x6f] != -1 &&
         (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
        ((cVar2 = FUN_14042f8e0(lVar7,0x38), cVar2 == '\0' &&
         (cVar2 = FUN_14042f7a0(lVar7,0x38), cVar2 != '\0')))))) goto switchD_14038a556_caseD_4;
    if ((char)param_1[0x69] != -1) {
      uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
      iVar4 = FUN_140405300(uVar6);
      uVar5 = uVar9;
      if (0 < iVar4) {
        do {
          lVar7 = FUN_1404052d0(uVar6,uVar5);
          if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x38,1), cVar2 == '\0'))
          goto switchD_14038a556_caseD_4;
          uVar11 = (int)uVar5 + 1;
          iVar4 = FUN_140405300(uVar6);
          uVar5 = (ulonglong)uVar11;
        } while ((int)uVar11 < iVar4);
      }
    }
    if (((*(char *)((longlong)param_1 + 0x333) != -1) &&
        (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0))
       && (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 != '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x38);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0xea];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x38), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0xea];
    goto LAB_14038c2b0;
  case 0x39:
    if ((param_1[0x20] == 0) || (cVar2 = FUN_14039b480(param_1,0x39,1), cVar2 == '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x39);
    if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
      uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 < 1) goto switchD_14038a556_caseD_4;
      while( true ) {
        lVar7 = FUN_1404052d0(uVar6,uVar9);
        if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x39), cVar2 != '\0')) break;
        uVar11 = (int)uVar9 + 1;
        uVar9 = (ulonglong)uVar11;
        iVar4 = FUN_140405300(uVar6);
        if (iVar4 <= (int)uVar11) {
          return 0;
        }
      }
      lVar7 = param_1[0xec];
    }
    else {
      lVar7 = param_1[0xec];
LAB_14038c2a7:
      if (lVar7 == 0) goto switchD_14038a556_caseD_4;
    }
    goto LAB_14038c2b0;
  case 0x3c:
    if (1 < *(int *)(param_1[0x53] + 0xc48)) {
      *(uint *)((longlong)param_1 + 0x324) = (uint)(*(int *)((longlong)param_1 + 0x324) == 0);
    }
    goto switchD_14038a556_caseD_16;
  case 0x3d:
    if ((param_1[0x20] == 0) || (cVar2 = FUN_14039b480(param_1,0x3d,1), cVar2 == '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x3d);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0xf4];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x3d), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0xf4];
    goto LAB_14038c2b0;
  case 0x3e:
    if (param_3 == '\0') {
      if ((((char)param_1[0x6f] != -1) &&
          (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
         (cVar2 = FUN_14042f7a0(lVar7,param_2), cVar2 == '\0')) {
        FUN_14042f320(lVar7,param_2);
      }
    }
    else {
      FUN_1403a97e0(param_1,param_2,param_3);
    }
    if (param_1[0x54] != 0) {
      FUN_140265ae0(param_1[0x54],5,param_3);
    }
    goto switchD_14038a556_caseD_16;
  case 0x3f:
    if (param_3 == '\0') {
      if ((((char)param_1[0x6f] != -1) &&
          (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
         (cVar2 = FUN_14042f7a0(lVar7,param_2), cVar2 == '\0')) {
        FUN_14042f320(lVar7,param_2);
        plVar8 = (longlong *)FUN_14037bd50(param_1[0x20] + 8,*(undefined4 *)(param_1[0xf8] + 0x88));
        if (plVar8 != (longlong *)0x0) {
          (**(code **)(*plVar8 + 0x78))(plVar8,param_1,3);
        }
      }
    }
    else {
      FUN_1403a97e0(param_1,param_2,param_3);
      if (((param_1[0x20] != 0) &&
          ((param_1[0x1e] == 0 || (cVar2 = FUN_140380bb0(), cVar2 == '\0')))) &&
         ((param_1[0x1e] == 0 ||
          ((cVar2 = FUN_140380b10(), cVar2 == '\0' ||
           (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x3f), cVar2 == '\0')))))) {
        if (*(char *)((longlong)param_1 + 0x3a7) == '\0') {
LAB_14038ae3b:
          if ((((param_1[0x20] == 0) || (*(char *)(param_1[0x20] + 0x28) == '\0')) &&
              (*(char *)((longlong)param_1 + 0x38a) == -1)) &&
             ((((char)param_1[0x6f] == -1 ||
               (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 == 0)) ||
              ((cVar2 = FUN_14042f8e0(lVar7,0x3f), cVar2 != '\0' ||
               (cVar2 = FUN_14042f7a0(lVar7,0x3f), cVar2 == '\0')))))) {
            if ((char)param_1[0x69] != -1) {
              uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
              iVar4 = FUN_140405300(uVar6);
              uVar5 = uVar9;
              if (0 < iVar4) {
                do {
                  lVar7 = FUN_1404052d0(uVar6,uVar5);
                  if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x3f,1), cVar2 == '\0'))
                  goto switchD_14038a556_caseD_16;
                  uVar11 = (int)uVar5 + 1;
                  uVar5 = (ulonglong)uVar11;
                  iVar4 = FUN_140405300(uVar6);
                } while ((int)uVar11 < iVar4);
              }
            }
            if (((*(char *)((longlong)param_1 + 0x333) == -1) ||
                (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1),
                plVar8 == (longlong *)0x0)) ||
               (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 == '\0')) {
              cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x3f);
              if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
                uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
                iVar4 = FUN_140405300(uVar6);
                if (0 < iVar4) {
                  do {
                    lVar7 = FUN_1404052d0(uVar6,uVar9);
                    if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x3f), cVar2 != '\0')) {
                      lVar7 = param_1[0xf8];
                      goto LAB_14038afcc;
                    }
                    uVar11 = (int)uVar9 + 1;
                    uVar9 = (ulonglong)uVar11;
                    iVar4 = FUN_140405300(uVar6);
                  } while ((int)uVar11 < iVar4);
                }
              }
              else {
                lVar7 = param_1[0xf8];
                if (lVar7 != 0) {
LAB_14038afcc:
                  FUN_14037b8a0(param_1[0x20] + 8,*(undefined4 *)(lVar7 + 0x88),0,1,0);
                }
              }
            }
          }
        }
        else if (*(char *)((longlong)param_1 + 899) == -1) {
          lVar7 = param_1[0x56];
          if (((lVar7 != 0) && (*(char *)(lVar7 + 899) != -1)) &&
             (*(longlong *)(lVar7 + 0x210) != 0)) {
            iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
            goto LAB_14038ae35;
          }
        }
        else if (param_1[0x42] != 0) {
          iVar4 = *(int *)(param_1[0x42] + 8);
LAB_14038ae35:
          if (iVar4 != 0) goto LAB_14038ae3b;
        }
      }
    }
    goto switchD_14038a556_caseD_16;
  case 0x41:
    lVar7 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
    if (lVar7 == 0) goto switchD_14038a556_caseD_4;
    bVar3 = FUN_140280ec0(lVar7,param_1);
    break;
  case 0x44:
    if (param_3 != '\0') {
      fVar1 = *(float *)(param_1[(lVar12 + 0x3d) * 2] + 0x224);
      fVar13 = (float)FUN_1403727a0(param_1[0x53],param_1);
      FUN_1403a9e30(param_1,1,fVar13 * fVar1,param_1,0,0,0,0,0,0xffffffff,0,0,0,0);
    }
switchD_14038a556_caseD_28:
    cVar2 = FUN_14039b480(param_1,param_2,1);
    if (cVar2 != '\0') {
      plVar8 = param_1;
      if ((longlong *)param_1[0x56] != (longlong *)0x0) {
        plVar8 = (longlong *)param_1[0x56];
      }
      if ((plVar8[0x15] == 0) || (*(longlong *)(plVar8[0x15] + 0x1f0) == 0))
      goto switchD_14038a556_caseD_4;
      TractorBeamAttackAbilityClass::FUN_140422040();
      if (param_3 == '\0') {
        if ((char)param_1[0x6f] != -1) {
          lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46);
          if (lVar7 == 0) {
            return 1;
          }
          cVar2 = FUN_14042f7a0(lVar7,param_2);
          if (cVar2 == '\0') {
            FUN_14042f320(lVar7,param_2);
          }
        }
      }
      else {
        FUN_1403a97e0(param_1,param_2,param_3);
      }
    }
    bVar3 = 1;
    if (param_1[0x54] != 0) {
      FUN_140265ae0(param_1[0x54],4,param_3);
    }
    goto LAB_14038c44b;
  case 0x45:
    if (((param_1[0x20] == 0) || ((param_1[0x1e] != 0 && (cVar2 = FUN_140380bb0(), cVar2 != '\0'))))
       || ((param_1[0x1e] != 0 &&
           ((cVar2 = FUN_140380b10(), cVar2 != '\0' &&
            (cVar2 = FUN_14043fa30(&DAT_140b2f2e0,0x45), cVar2 != '\0'))))))
    goto switchD_14038a556_caseD_4;
    if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
      if (*(char *)((longlong)param_1 + 899) == -1) {
        lVar7 = param_1[0x56];
        if (((lVar7 == 0) || (*(char *)(lVar7 + 899) == -1)) || (*(longlong *)(lVar7 + 0x210) == 0))
        goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(*(longlong *)(lVar7 + 0x210) + 8);
      }
      else {
        if (param_1[0x42] == 0) goto switchD_14038a556_caseD_4;
        iVar4 = *(int *)(param_1[0x42] + 8);
      }
      if (iVar4 == 0) goto switchD_14038a556_caseD_4;
    }
    if ((((param_1[0x20] != 0) && (*(char *)(param_1[0x20] + 0x28) != '\0')) ||
        (*(char *)((longlong)param_1 + 0x38a) != -1)) ||
       ((((char)param_1[0x6f] != -1 &&
         (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 != 0)) &&
        ((cVar2 = FUN_14042f8e0(lVar7,0x45), cVar2 == '\0' &&
         (cVar2 = FUN_14042f7a0(lVar7,0x45), cVar2 != '\0')))))) goto switchD_14038a556_caseD_4;
    if ((char)param_1[0x69] != -1) {
      uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
      iVar4 = FUN_140405300(uVar6);
      uVar5 = uVar9;
      if (0 < iVar4) {
        do {
          lVar7 = FUN_1404052d0(uVar6,uVar5);
          if ((lVar7 != 0) && (cVar2 = FUN_14039b480(lVar7,0x45,1), cVar2 == '\0'))
          goto switchD_14038a556_caseD_4;
          uVar11 = (int)uVar5 + 1;
          uVar5 = (ulonglong)uVar11;
          iVar4 = FUN_140405300(uVar6);
        } while ((int)uVar11 < iVar4);
      }
    }
    if (((*(char *)((longlong)param_1 + 0x333) != -1) &&
        (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0))
       && (cVar2 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar2 != '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x45);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0x104];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x45), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0x104];
    goto LAB_14038c2b0;
  case 0x46:
    if ((param_1[0x20] == 0) || (cVar2 = FUN_14039b480(param_1,0x46,1), cVar2 == '\0'))
    goto switchD_14038a556_caseD_4;
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x46);
    if ((cVar2 != '\0') || ((char)param_1[0x69] == -1)) {
      lVar7 = param_1[0x106];
      goto LAB_14038c2a7;
    }
    uVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar4 = FUN_140405300(uVar6);
    if (iVar4 < 1) goto switchD_14038a556_caseD_4;
    while( true ) {
      lVar7 = FUN_1404052d0(uVar6,uVar9);
      if ((lVar7 != 0) && (cVar2 = FUN_1403986b0(lVar7,0x46), cVar2 != '\0')) break;
      uVar11 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar11;
      iVar4 = FUN_140405300(uVar6);
      if (iVar4 <= (int)uVar11) {
        return 0;
      }
    }
    lVar7 = param_1[0x106];
    goto LAB_14038c2b0;
  case 0x48:
    if (((param_1[0x20] == 0) || (cVar2 = FUN_14039b480(param_1,param_2,1), cVar2 == '\0')) ||
       (cVar2 = FUN_1403986b0(param_1,param_2), cVar2 == '\0')) goto switchD_14038a556_caseD_4;
    lVar7 = param_1[(lVar12 + 0x3d) * 2];
LAB_14038c2b0:
    bVar3 = FUN_14037b8a0(param_1[0x20] + 8,*(undefined4 *)(lVar7 + 0x88),0,1,0);
    break;
  case 0x4b:
    lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x1a);
    if ((lVar7 == 0) || (param_1[0x23] == 0)) goto switchD_14038a556_caseD_4;
    FUN_140594600(lVar7);
    *(undefined4 *)(param_1[0x23] + 0x50) = 0;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar4 = (int)DAT_140b15418[2];
    }
    cVar2 = FUN_14043fa90(&DAT_140b2f2e0,0x4b);
    if ((cVar2 == '\0') && ((char)param_1[0x69] != -1)) {
      if (param_1[0x22] != 0) {
        uVar6 = FUN_1404ec820();
        uVar9 = FUN_140397d90(uVar6,0x4b);
      }
    }
    else {
      uVar9 = param_1[0x110];
    }
    *(int *)(param_1[0x23] + 100) =
         (int)((*(float *)(uVar9 + 0x24) - DAT_1407ffaf8) * (float)DAT_140b0a340 + (float)iVar4);
    if (((char)param_1[0x6f] == -1) ||
       (lVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar7 == 0))
    goto switchD_14038a556_caseD_16;
    iVar4 = 0x4b;
LAB_14038c440:
    FUN_14042fb70(lVar7,iVar4);
switchD_14038a556_caseD_16:
    bVar3 = 1;
    goto LAB_14038c44b;
  }
  if (bVar3 != 0) {
LAB_14038c44b:
    cVar2 = FUN_14043fa70(&DAT_140b2f2e0,param_2);
    if (cVar2 != '\0') {
      *(char *)(param_1 + lVar12 * 2 + 0x7b) = param_3;
    }
  }
switchD_14038a556_caseD_4:
  return (ulonglong)bVar3;
}


