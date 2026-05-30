/* FUN_1403a6b80  RVA 0x3a6b80  size 2862  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403a6b80(longlong *param_1,uint param_2,char param_3)

{
  longlong lVar1;
  uint uVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  longlong *plVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong *plVar9;
  longlong *plVar10;
  undefined *puVar11;
  int iVar12;
  undefined8 *puVar13;
  longlong lVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  ulonglong uVar17;
  undefined4 *puVar18;
  float fVar19;
  float fVar20;
  uint uVar21;
  double dVar22;
  undefined8 *in_stack_ffffffffffffff28;
  undefined1 local_88 [96];
  
  uVar17 = 0;
  if ((param_3 == '\0') &&
     (((*(byte *)(param_1 + 0x74) & 8) == 0 || (*(char *)(param_1[0x53] + 0x69) == '\0')))) {
    lVar8 = param_1[0x15];
    if (lVar8 != 0) {
      *(undefined4 *)(lVar8 + 0x194) = *(undefined4 *)(lVar8 + 0x164);
      *(undefined4 *)(lVar8 + 0x198) = *(undefined4 *)(lVar8 + 0x168);
      *(undefined4 *)(lVar8 + 0x19c) = *(undefined4 *)(lVar8 + 0x16c);
      *(undefined4 *)(lVar8 + 0x1a0) = *(undefined4 *)(lVar8 + 0x170);
      *(undefined4 *)(lVar8 + 0x1a4) = *(undefined4 *)(lVar8 + 0x174);
      *(undefined4 *)(lVar8 + 0x1a8) = *(undefined4 *)(lVar8 + 0x178);
      *(undefined4 *)(lVar8 + 0x1ac) = *(undefined4 *)(lVar8 + 0x17c);
      *(undefined4 *)(lVar8 + 0x1b0) = *(undefined4 *)(lVar8 + 0x180);
      *(undefined4 *)(lVar8 + 0x1b4) = *(undefined4 *)(lVar8 + 0x184);
      *(undefined4 *)(lVar8 + 0x1b8) = *(undefined4 *)(lVar8 + 0x188);
      *(undefined4 *)(lVar8 + 0x1bc) = *(undefined4 *)(lVar8 + 0x18c);
      *(undefined4 *)(lVar8 + 0x1c0) = *(undefined4 *)(lVar8 + 400);
      lVar8 = param_1[0x15];
      *(int *)(lVar8 + 0x164) = (int)param_1[0x49];
      *(undefined4 *)(lVar8 + 0x168) = *(undefined4 *)((longlong)param_1 + 0x24c);
      *(int *)(lVar8 + 0x16c) = (int)param_1[0x4a];
      *(undefined4 *)(lVar8 + 0x170) = *(undefined4 *)((longlong)param_1 + 0x254);
      *(int *)(lVar8 + 0x174) = (int)param_1[0x4b];
      *(undefined4 *)(lVar8 + 0x178) = *(undefined4 *)((longlong)param_1 + 0x25c);
      *(int *)(lVar8 + 0x17c) = (int)param_1[0x4c];
      *(undefined4 *)(lVar8 + 0x180) = *(undefined4 *)((longlong)param_1 + 0x264);
      *(int *)(lVar8 + 0x184) = (int)param_1[0x4d];
      *(undefined4 *)(lVar8 + 0x188) = *(undefined4 *)((longlong)param_1 + 0x26c);
      *(int *)(lVar8 + 0x18c) = (int)param_1[0x4e];
      *(undefined4 *)(lVar8 + 400) = *(undefined4 *)((longlong)param_1 + 0x274);
      cVar3 = FUN_140557ba0(param_1[0x15]);
      if (cVar3 == '\x01') {
        if (((param_1[0x15] != 0) && (*(longlong *)(param_1[0x15] + 0x1f0) != 0)) &&
           (plVar6 = (longlong *)FUN_14041aeb0(), plVar6 != (longlong *)0x0)) {
          (**(code **)(*plVar6 + 0x20))(plVar6,param_1);
          (**(code **)(*plVar6 + 0x18))(plVar6,param_1,0);
        }
        uVar7 = FUN_140058570();
        FUN_140220ed0(uVar7,param_1 + 7,0x17);
      }
    }
    if (param_1[0x1e] != 0) {
      FUN_1405369e0(*(undefined8 *)(param_1[0x1e] + 0x138),param_2);
    }
    lVar8 = param_1[0xc];
    if ((param_1[0xd] - lVar8) / 0x38 != 0) {
      do {
        puVar13 = (undefined8 *)0x0;
        lVar14 = uVar17 * 0x38;
        iVar5 = *(int *)(lVar14 + 0xc + lVar8);
        if (iVar5 < 1) {
LAB_1403a6ef6:
          uVar17 = (ulonglong)((int)uVar17 + 1);
        }
        else {
          *(int *)(lVar14 + 0xc + lVar8) = iVar5 + -1;
          lVar1 = param_1[0xc];
          if (0 < *(int *)(lVar14 + 0xc + lVar1)) goto LAB_1403a6ef6;
          if ((*(longlong *)(lVar14 + 0x28 + lVar1) != 0) &&
             (puVar13 = (undefined8 *)(lVar1 + 0x18 + lVar14), 0xf < (ulonglong)puVar13[3])) {
            puVar13 = (undefined8 *)*puVar13;
          }
          FUN_1403a9e30(param_1,*(undefined4 *)(lVar14 + lVar1),lVar8,0,puVar13,0,0,0,0,
                        *(undefined4 *)(lVar14 + 8 + lVar1),*(undefined4 *)(lVar14 + 0x10 + lVar1),0
                        ,0,0);
          lVar8 = param_1[0xc];
          if ((ulonglong)((param_1[0xd] - lVar8) / 0x38) < 2) {
            FUN_1403ac8a0(param_1 + 0xc);
            in_stack_ffffffffffffff28 = puVar13;
            break;
          }
          puVar16 = (undefined4 *)param_1[0xd];
          puVar18 = (undefined4 *)(lVar8 + 0x38 + lVar14);
          if (puVar18 != puVar16) {
            puVar15 = (undefined4 *)(lVar14 + 8 + lVar8);
            do {
              puVar15[-2] = *puVar18;
              puVar15[-1] = puVar15[0xd];
              *puVar15 = puVar15[0xe];
              puVar15[1] = puVar15[0xf];
              puVar15[2] = puVar15[0x10];
              FUN_1400582a0(puVar15 + 4,puVar15 + 0x12);
              puVar18 = puVar18 + 0xe;
              puVar15 = puVar15 + 0xe;
            } while (puVar18 != puVar16);
            puVar16 = (undefined4 *)param_1[0xd];
          }
          FUN_14038a1f0(puVar16 + -0xe);
          param_1[0xd] = param_1[0xd] + -0x38;
          in_stack_ffffffffffffff28 = puVar13;
        }
        lVar8 = param_1[0xc];
      } while (uVar17 < (ulonglong)((param_1[0xd] - lVar8) / 0x38));
    }
    iVar5 = (int)param_1[0x52];
    while (iVar5 = iVar5 + -1, -1 < iVar5) {
      plVar6 = *(longlong **)(param_1[0x4f] + (longlong)(char)iVar5 * 8);
      if ((*(uint *)(plVar6 + 6) <= param_2) && (cVar3 = FUN_1404c3700(plVar6), cVar3 == '\x01')) {
        (**(code **)(*plVar6 + 0x30))(plVar6,param_1,param_2);
      }
    }
    if ((*(byte *)((longlong)param_1 + 0x3a1) & 1) != 0) {
      DynamicTransformBehaviorClass::FUN_1403ac530(param_1);
      if (((param_1[0x16] != 0) && (*(char *)(param_1[0x16] + 0x38) != '\0')) &&
         (lVar8 = (**(code **)(*param_1 + 0x10))(param_1), lVar8 != 0)) {
        SelectBehaviorClass::FUN_1403c2710(lVar8);
      }
    }
    if ((param_1[0x54] != 0) && (*(int *)(param_1[0x54] + 0x84) != -1)) {
      uVar4 = FUN_140264840();
      cVar3 = FUN_140278fa0(uVar4);
      if (cVar3 != '\0') {
        FUN_140266340(param_1[0x54],param_1 + 0xf,
                      ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * *(float *)(param_1 + 0x13)
                      ,DAT_140b0a320,(ulonglong)in_stack_ffffffffffffff28 & 0xffffffffffffff00);
      }
    }
  }
  plVar6 = (longlong *)0x0;
  if (param_1[0x54] != 0) {
    lVar8 = param_1[0x5c];
    if (lVar8 != 0) {
      lVar14 = *(longlong *)(lVar8 + 0x298);
      if (lVar14 == 0) {
        plVar9 = (longlong *)0x0;
      }
      else {
        lVar8 = *(longlong *)(lVar8 + 0x2b8);
        if (*(char *)(lVar14 + 0x3e) == '\0') {
          if (*(char *)(lVar14 + 0x3f) == '\0') {
            lVar1 = **(longlong **)(lVar8 + 0x20);
            if (*(char *)(lVar14 + 0x40) == '\0') {
              plVar9 = (longlong *)(**(code **)(lVar1 + 0x100))(*(longlong **)(lVar8 + 0x20));
            }
            else {
              plVar9 = (longlong *)(**(code **)(lVar1 + 0x118))();
            }
          }
          else {
            plVar9 = (longlong *)(**(code **)(**(longlong **)(lVar8 + 0x20) + 0x110))();
          }
        }
        else {
          plVar9 = (longlong *)(**(code **)(**(longlong **)(lVar8 + 0x20) + 0x108))();
        }
      }
      lVar8 = param_1[0x53];
      plVar10 = plVar6;
      if (lVar8 != 0) {
        if (*(char *)(lVar8 + 0x3e) == '\0') {
          if (*(char *)(lVar8 + 0x3f) == '\0') {
            lVar14 = **(longlong **)(param_1[0x57] + 0x20);
            if (*(char *)(lVar8 + 0x40) == '\0') {
              plVar10 = (longlong *)(**(code **)(lVar14 + 0x100))();
            }
            else {
              plVar10 = (longlong *)(**(code **)(lVar14 + 0x118))();
            }
          }
          else {
            plVar10 = (longlong *)(**(code **)(**(longlong **)(param_1[0x57] + 0x20) + 0x110))();
          }
        }
        else {
          plVar10 = (longlong *)(**(code **)(**(longlong **)(param_1[0x57] + 0x20) + 0x108))();
        }
      }
      if ((plVar9 != plVar10) && (*(longlong *)(param_1[0x5c] + 0x2a0) != 0)) {
        lVar8 = FUN_1402648b0();
        param_1[0x5f] = lVar8;
        if (lVar8 != 0) {
          FUN_14012d2c0(lVar8,(int)param_1[0x5e],local_88);
          (**(code **)(*(longlong *)param_1[0x54] + 0x28))();
        }
      }
    }
    if (*(char *)(param_1[0x53] + 0xa2) == '\0') {
      uVar7 = FUN_140294a40(&DAT_140a16fd0);
      cVar3 = FUN_1402824d0(uVar7);
      if (cVar3 == '\0') {
        cVar3 = SceneClass::FUN_140264cb0(param_1[0x54]);
        if (cVar3 != '\0') goto LAB_1403a71b9;
      }
      else {
        cVar3 = SceneClass::FUN_140264cb0(param_1[0x54]);
        if (cVar3 == '\0') {
LAB_1403a71b9:
          FUN_140264c80(param_1[0x54]);
        }
      }
    }
    if (*(char *)((longlong)param_1 + 0x3a7) == '\0') {
      fVar19 = (float)FUN_1402647a0(param_1[0x54]);
      FUN_140265280(param_1[0x54],(float)DAT_140b0a320 / (float)DAT_140b0a340,0);
      fVar20 = (float)FUN_1402647a0(param_1[0x54]);
      if (((0.0 <= fVar20) && (0.0 <= fVar19)) && (fVar20 != fVar19)) {
        if ((undefined8 *)param_1[0x54] != (undefined8 *)0x0) {
          (*(code *)**(undefined8 **)param_1[0x54])();
        }
        FUN_1404775d0(&DAT_140a2a120);
      }
    }
    if ((*(byte *)(param_1 + 0x74) & 8) != 0) {
      fVar19 = DAT_14080080c;
      if ((param_1[0x54] == 0) || (fVar19 = (float)FUN_1402647a0(), param_1[0x54] == 0)) {
        iVar5 = 0;
      }
      else {
        iVar5 = FUN_1402647c0();
      }
      if ((float)iVar5 <= fVar19 + DAT_140866874) {
        *(byte *)(param_1 + 0x74) = *(byte *)(param_1 + 0x74) & 0xf7;
      }
    }
  }
  uVar2 = DAT_140800860;
  if (param_3 != '\0') goto LAB_1403a75b0;
  if (param_1[0x20] != 0) {
    FUN_14037da80(param_1[0x20] + 8);
  }
  if ((param_1[0x5b] != 0) && ((*(byte *)(param_1 + 0x74) & 2) == 0)) {
    plVar10 = (longlong *)FUN_140246fb0(param_1[0x5b],"ServiceRate",0);
    plVar9 = plVar6;
    if (plVar10 == (longlong *)0x0) {
LAB_1403a7367:
      if (plVar10 != (longlong *)0x0) goto LAB_1403a736f;
    }
    else {
      (**(code **)*plVar10)(plVar10);
      puVar11 = (undefined *)(**(code **)(*plVar10 + 0x20))(plVar10);
      if (puVar11 == (undefined *)0x0) goto LAB_1403a7367;
      do {
        if (puVar11 == &DAT_140a157c0) {
          (**(code **)*plVar10)(plVar10);
          plVar9 = plVar10;
          break;
        }
        puVar11 = *(undefined **)(puVar11 + 8);
      } while (puVar11 != (undefined *)0x0);
LAB_1403a736f:
      (**(code **)(*plVar10 + 8))(plVar10);
    }
    plVar10 = (longlong *)FUN_140246fb0(param_1[0x5b],"LastService",0);
    if (plVar10 != (longlong *)0x0) {
      (**(code **)*plVar10)(plVar10);
      for (puVar11 = (undefined *)(**(code **)(*plVar10 + 0x20))(plVar10);
          puVar11 != (undefined *)0x0; puVar11 = *(undefined **)(puVar11 + 8)) {
        if (puVar11 == &DAT_140a157c0) {
          (**(code **)*plVar10)(plVar10);
          plVar6 = plVar10;
          goto LAB_1403a73ca;
        }
      }
    }
    if (plVar10 != (longlong *)0x0) {
LAB_1403a73ca:
      (**(code **)(*plVar10 + 8))(plVar10);
    }
    fVar19 = (float)*(int *)(*(longlong *)(param_1[0x57] + 0x20) + 0x10) * DAT_140b0a344;
    if (plVar9 == (longlong *)0x0) {
LAB_1403a74cc:
      if (plVar6 != (longlong *)0x0) goto LAB_1403a74d1;
    }
    else {
      if ((plVar6 == (longlong *)0x0) ||
         (dVar22 = (double)fVar19 - (double)plVar6[2],
         (double)plVar9[2] <= dVar22 && dVar22 != (double)plVar9[2])) {
        FUN_140247a90(param_1[0x5b]);
        plVar10 = (longlong *)param_1[0x5b];
        if (*(char *)((longlong)plVar10 + 0x121) == '\0') {
          if (plVar6 == (longlong *)0x0) {
            plVar10 = (longlong *)FUN_140769c58(0x18);
            if (plVar10 != (longlong *)0x0) {
              *(undefined4 *)(plVar10 + 1) = 0;
              *plVar10 = (longlong)LuaValue<double,4>::vftable;
              (*(code *)LuaValue<double,4>::vftable[0])(plVar10);
              plVar6 = plVar10;
            }
            uVar21 = FUN_1401ffbb0(&DAT_140a13e24,0);
            plVar6[2] = (longlong)(double)(float)(uVar21 ^ uVar2);
          }
          else {
            plVar6[2] = (longlong)(double)fVar19;
          }
          FUN_140247000(param_1[0x5b],plVar6,"LastService");
        }
        else if (plVar10 != (longlong *)0x0) {
          (**(code **)(*plVar10 + 8))();
          param_1[0x5b] = 0;
        }
        goto LAB_1403a74cc;
      }
LAB_1403a74d1:
      (**(code **)(*plVar6 + 8))(plVar6);
    }
    if (plVar9 != (longlong *)0x0) {
      (**(code **)(*plVar9 + 8))(plVar9);
    }
  }
  lVar8 = param_1[0x5a];
  if ((lVar8 != 0) && ((*(byte *)(param_1 + 0x74) & 0x40) == 0)) {
    iVar5 = *(int *)(lVar8 + 0x10);
    lVar14 = (longlong)iVar5;
    if ((0 < iVar5) && (iVar12 = 0, 0 < iVar5)) {
      plVar6 = *(longlong **)(lVar8 + 8);
      do {
        if ((*plVar6 != 0) && (*(char *)(*(longlong *)(*plVar6 + 0x20) + 0x4d) == '\x01')) {
          iVar12 = iVar12 + 1;
        }
        plVar6 = plVar6 + 1;
        lVar14 = lVar14 + -1;
      } while (lVar14 != 0);
      if ((0 < iVar12) && (*(char *)(param_1[0x53] + 0xa8) == '\x01')) {
        fVar20 = (float)FUN_140396070(param_1);
        fVar19 = fVar20 + _DAT_140b16d78;
        if (DAT_1407ffaf8 <= fVar20 + _DAT_140b16d78) {
          fVar19 = DAT_1407ffaf8;
        }
        fVar20 = (float)FUN_140396df0(param_1);
        if (fVar19 < fVar20) {
          FUN_1403a8ad0(param_1,fVar19);
        }
      }
    }
    FUN_1403a76b0(param_1,param_2);
  }
  *(float *)(param_1 + 100) = DAT_140b26fa4 * *(float *)(param_1 + 100);
LAB_1403a75b0:
  if ((((*(byte *)((longlong)param_1 + 0x3a2) & 4) != 0) &&
      ((*(byte *)(param_1 + 0x74) & 0x40) == 0)) && (0.0 < *(float *)(param_1[0x53] + 0x234c))) {
    fVar19 = (float)FUN_1403727a0(param_1[0x53],param_1);
    fVar19 = (fVar19 / *(float *)(param_1[0x53] + 0x234c)) / (float)DAT_140b0a340;
    if ((0.0 < fVar19) &&
       (fVar20 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)(fVar19 / DAT_140805898) ^ uVar2),
       0.0 < fVar19 + fVar20)) {
      FUN_1403ab890(param_1,0x11);
    }
  }
  return;
}


