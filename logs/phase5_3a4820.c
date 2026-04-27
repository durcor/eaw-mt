/* FUN_1403a4820  RVA 0x3a4820  size unknown */


void FUN_1403a4820(longlong *param_1)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  undefined4 uVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined *puVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 uVar10;
  undefined8 *puVar11;
  longlong *plVar12;
  ulonglong uVar13;
  undefined4 *puVar14;
  longlong *plVar15;
  longlong *plVar16;
  float fVar17;
  undefined4 local_res8;
  undefined4 local_resc;
  uint uVar18;
  longlong local_58;
  longlong local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  
  if (param_1[0x53] == 0) {
    return;
  }
  if (*(char *)(param_1[0x53] + 0x3d) != '\0') {
    lVar5 = *(longlong *)(param_1[0x57] + 0x20);
    iVar3 = FUN_14020bd00(*(longlong *)(*(longlong *)(lVar5 + 0x90) + 0x40) + 0xf4,param_1 + 0xf);
    if (iVar3 == 1) {
      return;
    }
    if ((DAT_140a284c4 != '\0') && (DAT_140b1574e == '\0')) {
      uVar4 = FUN_140294a70(&DAT_140a16fd0);
      cVar1 = FUN_14035f470(lVar5,uVar4,param_1,0);
      if (cVar1 != '\0') {
        return;
      }
    }
    if ((*(char *)((longlong)param_1 + 0x33e) != -1) &&
       (cVar1 = FUN_1403751f0(param_1[0x53]), cVar1 == '\0')) {
      return;
    }
  }
  plVar6 = *(longlong **)(param_1[0x57] + 0x20);
  lVar5 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
  plVar16 = (longlong *)0x0;
  plVar8 = plVar16;
  if (lVar5 != 0) {
    plVar8 = *(longlong **)(lVar5 + 0x68);
  }
  uVar4 = (**(code **)(*plVar6 + 0xe0))(plVar6);
  plVar6 = (longlong *)FUN_1403729f0(param_1[0x53],uVar4,param_1,plVar8);
  if ((((DAT_140b15418 != (longlong *)0x0) &&
       (cVar1 = (**(code **)(*DAT_140b15418 + 0xf8))(), cVar1 == '\x01')) &&
      (lVar5 = param_1[0x18], lVar5 != 0)) && ((lVar5 != -8 && (0 < *(int *)(lVar5 + 0x18))))) {
    plVar8 = *(longlong **)(lVar5 + 0x10);
    plVar12 = plVar16;
    do {
      if (((*plVar8 != 0) && (lVar9 = *(longlong *)(*plVar8 + 0x298), lVar9 != 0)) &&
         (plVar15 = *(longlong **)(lVar9 + 0x290), plVar15 != (longlong *)0x0)) goto LAB_1403a49b0;
      plVar12 = (longlong *)((longlong)plVar12 + 1);
      plVar8 = plVar8 + 1;
    } while ((longlong)plVar12 < (longlong)*(int *)(lVar5 + 0x18));
  }
  plVar15 = plVar6;
  if (plVar6 == (longlong *)0x0) {
    return;
  }
LAB_1403a49b0:
  if (plVar15 == (longlong *)param_1[0x54]) {
    return;
  }
  if (((longlong *)param_1[0x54] != (longlong *)0x0) &&
     (plVar6 = (longlong *)FUN_140264a30(), plVar6 != (longlong *)0x0)) {
    for (puVar7 = (undefined *)(**(code **)(*plVar6 + 0x20))(plVar6); puVar7 != (undefined *)0x0;
        puVar7 = *(undefined **)(puVar7 + 8)) {
      if (puVar7 == &DAT_140a124d8) {
        return;
      }
    }
  }
  lVar5 = FUN_140397680(param_1);
  if (lVar5 == 0) {
    return;
  }
  if (param_1[0x54] != 0) {
    lVar5 = param_1[0x5c];
    lVar9 = param_1[0x5e];
    FUN_1403a5840(param_1);
    param_1[0x5c] = lVar5;
    *(int *)(param_1 + 0x5e) = (int)lVar9;
  }
  if ((longlong *)param_1[0x54] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x54] + 8))();
    param_1[0x54] = 0;
  }
  uVar4 = (**(code **)(**(longlong **)(param_1[0x57] + 0x20) + 0xe0))();
  cVar1 = FUN_140370320(param_1[0x53],0x11,uVar4);
  if (((cVar1 != '\0') || (cVar1 = FUN_140370320(param_1[0x53],0x11,0xffffffff), cVar1 != '\0')) &&
     (DAT_140a17050 != '\x01')) {
    return;
  }
  plVar8 = (longlong *)(**(code **)(*plVar15 + 0x20))(plVar15,param_1);
  plVar6 = (longlong *)param_1[0x54];
  if (plVar6 != plVar8) {
    if (plVar6 != (longlong *)0x0) {
      (**(code **)(*plVar6 + 8))();
    }
    param_1[0x54] = (longlong)plVar8;
    plVar6 = (longlong *)0x0;
    if (plVar8 != (longlong *)0x0) {
      (**(code **)*plVar8)(plVar8);
      plVar6 = (longlong *)param_1[0x54];
    }
  }
  lVar5 = FUN_1402648b0(plVar6);
  if (lVar5 != 0) {
    *(undefined4 *)(lVar5 + 0x1a0) = *(undefined4 *)(param_1[0x53] + 0x240);
    *(code **)(lVar5 + 0x150) = FUN_1403a0c20;
    lVar9 = param_1[0x53];
    if (((*(float *)(lVar9 + 0x300) != 0.0) || (*(float *)(lVar9 + 0x304) != 0.0)) ||
       (*(float *)(lVar9 + 0x308) != 0.0)) {
      FUN_14012db90(lVar5,(float *)(lVar9 + 0x300),lVar9 + 0x30c);
    }
  }
  if (((*(char *)((longlong)param_1 + 0x39f) == '\x01') &&
      (plVar6 = (longlong *)FUN_140397680(param_1), plVar6 != (longlong *)0x0)) &&
     (param_1[0x54] != 0)) {
    lVar5 = FUN_140264a30();
    if (lVar5 != 0) {
      *(byte *)(lVar5 + 0x32) =
           *(byte *)(lVar5 + 0x32) ^ (*(byte *)(param_1[0x53] + 0x86) ^ *(byte *)(lVar5 + 0x32)) & 1
      ;
    }
    if (param_1[0x5c] != 0) {
      lVar5 = FUN_140397680();
      lVar9 = FUN_140397680(param_1);
      if (lVar5 == lVar9) {
        if (*(longlong *)(param_1[0x5c] + 0x2a0) != 0) {
          lVar5 = FUN_1402648b0();
          param_1[0x5f] = lVar5;
          if (lVar5 != 0) {
            uVar10 = FUN_140264a30(param_1[0x54]);
            FUN_14012d770(lVar5,uVar10,(int)param_1[0x5e]);
          }
        }
        goto LAB_1403a4c4b;
      }
    }
    (**(code **)(*plVar6 + 0x28))(plVar6,param_1[0x54]);
    if ((param_1[0x5c] != 0) && (*(char *)((longlong)param_1 + 0x39e) == '\0')) {
      FUN_14029bfe0(param_1[0x57],param_1);
      *(undefined1 *)((longlong)param_1 + 0x39e) = 1;
    }
  }
LAB_1403a4c4b:
  iVar3 = DAT_140b153fc;
  if (DAT_140b15418 != (longlong *)0x0) {
    iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
  }
  if ((iVar3 != 2) || (cVar1 = FUN_140370320(param_1[0x53],0x61,0xffffffff), cVar1 == '\0')) {
    uVar4 = FUN_140395920(param_1);
    iVar3 = FUN_140264a40(param_1[0x54],uVar4);
    if (iVar3 != 0) {
      uVar13 = (ulonglong)DAT_140b0a320;
      fVar17 = (float)DAT_140b0a340;
      FUN_140265560(param_1[0x54],uVar4,0,*(undefined1 *)(param_1[0x53] + 0x30),0,
                    *(undefined4 *)(param_1[0x53] + 0x1640),(float)uVar13 / fVar17,0);
      iVar3 = FUN_1402647c0(param_1[0x54]);
      if ((param_1[0x57] == 0) ||
         (puVar14 = &DAT_140a13e24, *(char *)(param_1[0x57] + 0x63e) != '\x01')) {
        puVar14 = &DAT_140a13e20;
      }
      uVar4 = FUN_1401ffb40(puVar14,0,iVar3 + -1);
      FUN_140265490(param_1[0x54],uVar4,(float)uVar13 / fVar17);
    }
  }
  if (param_1[0x54] != 0) {
    if ((((*(char *)(param_1[0x53] + 0xd9) == '\0') || (DAT_140b169e0 == '\0')) ||
        ((char)*(byte *)(param_1 + 0x74) < '\0')) || ((*(byte *)(param_1 + 0x74) & 0x40) != 0)) {
      FUN_140265b70(param_1[0x54],0);
      FUN_1401f93e0(&local_48,0xff,0xff,0xff,0xff);
      if (((param_1[0x54] != 0) && (lVar5 = FUN_1402648b0(), lVar5 != 0)) &&
         (FUN_14012dc40(lVar5,&local_58), local_50 != local_58 + 8)) {
        do {
          plVar6 = (longlong *)(*(longlong *)(local_50 + 0x18) + -0x10);
          if (*(longlong *)(local_50 + 0x18) == 0) {
            plVar6 = plVar16;
          }
          cVar1 = FUN_14048a670(plVar6);
          if (cVar1 == '\0') {
            local_50 = *(longlong *)(local_50 + 8);
          }
          else {
            plVar6 = (longlong *)(*(longlong *)(local_50 + 0x18) + -0x10);
            if (*(longlong *)(local_50 + 0x18) == 0) {
              plVar6 = plVar16;
            }
            if (plVar6 != (longlong *)0x0) {
              local_50 = *(longlong *)(local_50 + 8);
              lVar5 = FUN_14020abe0(local_58,plVar6 + 2);
              if (lVar5 != 0) {
                (**(code **)(*plVar6 + 8))(plVar6);
              }
            }
          }
        } while (local_50 != local_58 + 8);
      }
    }
    else {
      lVar5 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
      FUN_1401f92f0(&local_48);
      cVar1 = FUN_14028af60(&DAT_140b153e0);
      if (cVar1 == '\0') {
        puVar11 = (undefined8 *)FUN_14033e340(*(undefined8 *)(lVar5 + 0x68),0);
      }
      else {
        puVar11 = (undefined8 *)FUN_140281940(lVar5);
      }
      local_48 = *puVar11;
      uStack_40 = puVar11[1];
      FUN_1401f9590(&local_48,0x80);
      FUN_140265b70(param_1[0x54],1);
      if ((param_1[0x54] != 0) && (lVar5 = FUN_1402648b0(), lVar5 != 0)) {
        FUN_14012dc40(lVar5,&local_58);
        if (local_50 != local_58 + 8) {
          do {
            plVar6 = (longlong *)(*(longlong *)(local_50 + 0x18) + -0x10);
            if (*(longlong *)(local_50 + 0x18) == 0) {
              plVar6 = plVar16;
            }
            cVar1 = FUN_14048a670(plVar6);
            if (cVar1 == '\0') {
              local_50 = *(longlong *)(local_50 + 8);
            }
            else {
              plVar6 = (longlong *)(*(longlong *)(local_50 + 0x18) + -0x10);
              if (*(longlong *)(local_50 + 0x18) == 0) {
                plVar6 = plVar16;
              }
              if (plVar6 != (longlong *)0x0) {
                local_50 = *(longlong *)(local_50 + 8);
                lVar9 = FUN_14020abe0(local_58,plVar6 + 2);
                if (lVar9 != 0) {
                  (**(code **)(*plVar6 + 8))(plVar6);
                }
              }
            }
          } while (local_50 != local_58 + 8);
        }
        uVar10 = FUN_14048a3e0(&local_48,1);
        FUN_14012dbf0(lVar5,uVar10);
        uVar10 = FUN_14048a3e0(&local_48,2);
        FUN_14012dbf0(lVar5,uVar10);
      }
    }
    FUN_1401f9440(&local_48);
  }
  if (param_1[0x54] == 0) {
    return;
  }
  iVar3 = FUN_140373ac0(param_1[0x53]);
  if (*(char *)((longlong)param_1 + 0x39f) == '\0') {
    iVar3 = 0;
  }
  if (((*(byte *)((longlong)param_1 + 0x3a1) & 8) == 0) && (iVar3 != 0)) {
    if (iVar3 == 1) {
      FUN_140264750(param_1[0x54],0);
      iVar3 = (**(code **)(**(longlong **)(param_1[0x57] + 0x20) + 0xe0))();
      if (((iVar3 == 1) && (*(longlong *)(param_1[0x57] + 0x20) != 0)) &&
         (lVar5 = *(longlong *)(*(longlong *)(param_1[0x57] + 0x20) + 0x338), lVar5 != 0)) {
        lVar9 = param_1[0x53];
        local_res8 = *(undefined4 *)(lVar9 + 0x17c0);
        local_resc = *(undefined4 *)(lVar9 + 0x17c4);
        uVar18 = *(uint *)(lVar9 + 0x17cc);
        FUN_140264000(param_1[0x54],lVar5,&local_res8,*(undefined4 *)(lVar9 + 0x17c8),uVar18);
        puVar11 = (undefined8 *)(param_1[0x53] + 0x17f0);
        if (0xf < *(ulonglong *)(param_1[0x53] + 0x1808)) {
          puVar11 = (undefined8 *)*puVar11;
        }
        FUN_140265f20(param_1[0x54],puVar11);
        FUN_140266340(param_1[0x54],param_1 + 0xf,
                      ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) *
                      *(float *)((longlong)param_1 + 0x8c),DAT_140b0a320,uVar18 & 0xffffff00);
        if (((DAT_140b1574e == '\0') && (*(char *)((longlong)param_1 + 0x34b) != -1)) &&
           ((lVar5 = (**(code **)(*param_1 + 0x10))(param_1,0x19), lVar5 != 0 &&
            (*(char *)(lVar5 + 0x54) == '\0')))) {
          FUN_140264c80(param_1[0x54],1);
        }
      }
      goto LAB_1403a5171;
    }
    if (iVar3 != 2) goto LAB_1403a5171;
    uVar4 = 1;
  }
  else {
    uVar4 = 0;
  }
  FUN_140264750(param_1[0x54],uVar4);
  FUN_140264f00(param_1[0x54]);
LAB_1403a5171:
  uVar2 = FUN_140375250(param_1[0x53]);
  FUN_1402646f0(param_1[0x54],uVar2);
  uVar2 = FUN_140375260(param_1[0x53]);
  FUN_140264720(param_1[0x54],uVar2);
  return;
}

