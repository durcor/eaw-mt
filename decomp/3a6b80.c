/* FUN_1403a6b80  RVA 0x3a6b80  size 2862  params 3  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Unknown calling convention */

void FUN_1403a6b80(GameObjectClass *param_1,uint param_2,char param_3)

{
  void *pvVar1;
  longlong lVar2;
  uint uVar3;
  char cVar4;
  undefined4 uVar5;
  int iVar6;
  longlong *plVar7;
  undefined8 uVar8;
  longlong lVar9;
  longlong *plVar10;
  longlong *plVar11;
  undefined *puVar12;
  int iVar13;
  undefined8 *puVar14;
  longlong lVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  ulonglong uVar18;
  undefined4 *puVar19;
  float fVar20;
  float fVar21;
  uint uVar22;
  double dVar23;
  undefined8 *in_stack_ffffffffffffff28;
  undefined1 local_88 [96];
  
  uVar18 = 0;
  if ((param_3 == '\0') &&
     (((param_1->flags & 8) == 0 || (*(char *)(*(longlong *)&param_1->field_0x298 + 0x69) == '\0')))
     ) {
    pvVar1 = param_1->locomotor_state;
    if (pvVar1 != (void *)0x0) {
      *(undefined4 *)((longlong)pvVar1 + 0x194) = *(undefined4 *)((longlong)pvVar1 + 0x164);
      *(undefined4 *)((longlong)pvVar1 + 0x198) = *(undefined4 *)((longlong)pvVar1 + 0x168);
      *(undefined4 *)((longlong)pvVar1 + 0x19c) = *(undefined4 *)((longlong)pvVar1 + 0x16c);
      *(undefined4 *)((longlong)pvVar1 + 0x1a0) = *(undefined4 *)((longlong)pvVar1 + 0x170);
      *(undefined4 *)((longlong)pvVar1 + 0x1a4) = *(undefined4 *)((longlong)pvVar1 + 0x174);
      *(undefined4 *)((longlong)pvVar1 + 0x1a8) = *(undefined4 *)((longlong)pvVar1 + 0x178);
      *(undefined4 *)((longlong)pvVar1 + 0x1ac) = *(undefined4 *)((longlong)pvVar1 + 0x17c);
      *(undefined4 *)((longlong)pvVar1 + 0x1b0) = *(undefined4 *)((longlong)pvVar1 + 0x180);
      *(undefined4 *)((longlong)pvVar1 + 0x1b4) = *(undefined4 *)((longlong)pvVar1 + 0x184);
      *(undefined4 *)((longlong)pvVar1 + 0x1b8) = *(undefined4 *)((longlong)pvVar1 + 0x188);
      *(undefined4 *)((longlong)pvVar1 + 0x1bc) = *(undefined4 *)((longlong)pvVar1 + 0x18c);
      *(undefined4 *)((longlong)pvVar1 + 0x1c0) = *(undefined4 *)((longlong)pvVar1 + 400);
      pvVar1 = param_1->locomotor_state;
      *(undefined4 *)((longlong)pvVar1 + 0x164) = *(undefined4 *)&param_1->field_0x248;
      *(undefined4 *)((longlong)pvVar1 + 0x168) = *(undefined4 *)&param_1->field_0x24c;
      *(undefined4 *)((longlong)pvVar1 + 0x16c) = *(undefined4 *)&param_1->field_0x250;
      *(undefined4 *)((longlong)pvVar1 + 0x170) = *(undefined4 *)&param_1->field_0x254;
      *(undefined4 *)((longlong)pvVar1 + 0x174) = *(undefined4 *)&param_1->field_0x258;
      *(undefined4 *)((longlong)pvVar1 + 0x178) = *(undefined4 *)&param_1->field_0x25c;
      *(undefined4 *)((longlong)pvVar1 + 0x17c) = *(undefined4 *)&param_1->field_0x260;
      *(undefined4 *)((longlong)pvVar1 + 0x180) = *(undefined4 *)&param_1->field_0x264;
      *(undefined4 *)((longlong)pvVar1 + 0x184) = *(undefined4 *)&param_1->field_0x268;
      *(undefined4 *)((longlong)pvVar1 + 0x188) = *(undefined4 *)&param_1->field_0x26c;
      *(undefined4 *)((longlong)pvVar1 + 0x18c) = *(undefined4 *)&param_1->field_0x270;
      *(undefined4 *)((longlong)pvVar1 + 400) = *(undefined4 *)&param_1->field_0x274;
      cVar4 = FUN_140557ba0(param_1->locomotor_state);
      if (cVar4 == '\x01') {
        if (((param_1->locomotor_state != (void *)0x0) &&
            (*(longlong *)((longlong)param_1->locomotor_state + 0x1f0) != 0)) &&
           (plVar7 = (longlong *)FUN_14041aeb0(), plVar7 != (longlong *)0x0)) {
          (**(code **)(*plVar7 + 0x20))(plVar7,param_1);
          (**(code **)(*plVar7 + 0x18))(plVar7,param_1,0);
        }
        uVar8 = FUN_140058570();
        FUN_140220ed0(uVar8,&param_1->listener_list,0x17);
      }
    }
    if (*(longlong *)&param_1->field_0xf0 != 0) {
      FUN_1405369e0(*(undefined8 *)(*(longlong *)&param_1->field_0xf0 + 0x138),param_2);
    }
    lVar9 = *(longlong *)&param_1->field_0x60;
    if ((*(longlong *)&param_1->field_0x68 - lVar9) / 0x38 != 0) {
      do {
        puVar14 = (undefined8 *)0x0;
        lVar15 = uVar18 * 0x38;
        iVar6 = *(int *)(lVar15 + 0xc + lVar9);
        if (iVar6 < 1) {
LAB_1403a6ef6:
          uVar18 = (ulonglong)((int)uVar18 + 1);
        }
        else {
          *(int *)(lVar15 + 0xc + lVar9) = iVar6 + -1;
          lVar2 = *(longlong *)&param_1->field_0x60;
          if (0 < *(int *)(lVar15 + 0xc + lVar2)) goto LAB_1403a6ef6;
          if ((*(longlong *)(lVar15 + 0x28 + lVar2) != 0) &&
             (puVar14 = (undefined8 *)(lVar2 + 0x18 + lVar15), 0xf < (ulonglong)puVar14[3])) {
            puVar14 = (undefined8 *)*puVar14;
          }
          FUN_1403a9e30(param_1,*(undefined4 *)(lVar15 + lVar2),lVar9,0,puVar14,0,0,0,0,
                        *(undefined4 *)(lVar15 + 8 + lVar2),*(undefined4 *)(lVar15 + 0x10 + lVar2),0
                        ,0,0);
          lVar9 = *(longlong *)&param_1->field_0x60;
          if ((ulonglong)((*(longlong *)&param_1->field_0x68 - lVar9) / 0x38) < 2) {
            FUN_1403ac8a0(&param_1->field_0x60);
            in_stack_ffffffffffffff28 = puVar14;
            break;
          }
          puVar17 = *(undefined4 **)&param_1->field_0x68;
          puVar19 = (undefined4 *)(lVar9 + 0x38 + lVar15);
          if (puVar19 != puVar17) {
            puVar16 = (undefined4 *)(lVar15 + 8 + lVar9);
            do {
              puVar16[-2] = *puVar19;
              puVar16[-1] = puVar16[0xd];
              *puVar16 = puVar16[0xe];
              puVar16[1] = puVar16[0xf];
              puVar16[2] = puVar16[0x10];
              FUN_1400582a0(puVar16 + 4,puVar16 + 0x12);
              puVar19 = puVar19 + 0xe;
              puVar16 = puVar16 + 0xe;
            } while (puVar19 != puVar17);
            puVar17 = *(undefined4 **)&param_1->field_0x68;
          }
          FUN_14038a1f0(puVar17 + -0xe);
          *(longlong *)&param_1->field_0x68 = *(longlong *)&param_1->field_0x68 + -0x38;
          in_stack_ffffffffffffff28 = puVar14;
        }
        lVar9 = *(longlong *)&param_1->field_0x60;
      } while (uVar18 < (ulonglong)((*(longlong *)&param_1->field_0x68 - lVar9) / 0x38));
    }
    iVar6 = param_1->behavior_count;
    while (iVar6 = iVar6 + -1, -1 < iVar6) {
      plVar7 = *(longlong **)((longlong)param_1->behaviors + (longlong)(char)iVar6 * 8);
      if ((*(uint *)(plVar7 + 6) <= param_2) && (cVar4 = FUN_1404c3700(plVar7), cVar4 == '\x01')) {
        (**(code **)(*plVar7 + 0x30))(plVar7,param_1,param_2);
      }
    }
    if ((param_1->flags & 0x100) != 0) {
      DynamicTransformBehaviorClass::FUN_1403ac530(param_1);
      if (((*(longlong *)&param_1->field_0xb0 != 0) &&
          (*(char *)(*(longlong *)&param_1->field_0xb0 + 0x38) != '\0')) &&
         (lVar9 = (**(code **)((longlong)param_1->vtable + 0x10))(param_1), lVar9 != 0)) {
        SelectBehaviorClass::FUN_1403c2710(lVar9);
      }
    }
    if ((*(longlong *)&param_1->field_0x2a0 != 0) &&
       (*(int *)(*(longlong *)&param_1->field_0x2a0 + 0x84) != -1)) {
      uVar5 = FUN_140264840();
      cVar4 = FUN_140278fa0(uVar5);
      if (cVar4 != '\0') {
        FUN_140266340(*(undefined8 *)&param_1->field_0x2a0,&param_1->pos_x,
                      ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) *
                      *(float *)&param_1->field_0x98,DAT_140b0a320,
                      (ulonglong)in_stack_ffffffffffffff28 & 0xffffffffffffff00);
      }
    }
  }
  plVar7 = (longlong *)0x0;
  if (*(longlong *)&param_1->field_0x2a0 != 0) {
    lVar9 = *(longlong *)&param_1->field_0x2e0;
    if (lVar9 != 0) {
      lVar15 = *(longlong *)(lVar9 + 0x298);
      if (lVar15 == 0) {
        plVar10 = (longlong *)0x0;
      }
      else {
        lVar9 = *(longlong *)(lVar9 + 0x2b8);
        if (*(char *)(lVar15 + 0x3e) == '\0') {
          if (*(char *)(lVar15 + 0x3f) == '\0') {
            lVar2 = **(longlong **)(lVar9 + 0x20);
            if (*(char *)(lVar15 + 0x40) == '\0') {
              plVar10 = (longlong *)(**(code **)(lVar2 + 0x100))(*(longlong **)(lVar9 + 0x20));
            }
            else {
              plVar10 = (longlong *)(**(code **)(lVar2 + 0x118))();
            }
          }
          else {
            plVar10 = (longlong *)(**(code **)(**(longlong **)(lVar9 + 0x20) + 0x110))();
          }
        }
        else {
          plVar10 = (longlong *)(**(code **)(**(longlong **)(lVar9 + 0x20) + 0x108))();
        }
      }
      lVar9 = *(longlong *)&param_1->field_0x298;
      plVar11 = plVar7;
      if (lVar9 != 0) {
        if (*(char *)(lVar9 + 0x3e) == '\0') {
          plVar11 = *(longlong **)(*(longlong *)&param_1->field_0x2b8 + 0x20);
          if (*(char *)(lVar9 + 0x3f) == '\0') {
            lVar15 = *plVar11;
            if (*(char *)(lVar9 + 0x40) == '\0') {
              plVar11 = (longlong *)(**(code **)(lVar15 + 0x100))();
            }
            else {
              plVar11 = (longlong *)(**(code **)(lVar15 + 0x118))();
            }
          }
          else {
            plVar11 = (longlong *)(**(code **)(*plVar11 + 0x110))();
          }
        }
        else {
          plVar11 = (longlong *)
                    (**(code **)(**(longlong **)(*(longlong *)&param_1->field_0x2b8 + 0x20) + 0x108)
                    )();
        }
      }
      if ((plVar10 != plVar11) && (*(longlong *)(*(longlong *)&param_1->field_0x2e0 + 0x2a0) != 0))
      {
        lVar9 = FUN_1402648b0();
        *(longlong *)&param_1->field_0x2f8 = lVar9;
        if (lVar9 != 0) {
          FUN_14012d2c0(lVar9,*(undefined4 *)&param_1->field_0x2f0,local_88);
          (**(code **)(**(longlong **)&param_1->field_0x2a0 + 0x28))();
        }
      }
    }
    if (*(char *)(*(longlong *)&param_1->field_0x298 + 0xa2) == '\0') {
      uVar8 = FUN_140294a40(&DAT_140a16fd0);
      cVar4 = FUN_1402824d0(uVar8);
      if (cVar4 == '\0') {
        cVar4 = SceneClass::FUN_140264cb0(*(undefined8 *)&param_1->field_0x2a0);
        if (cVar4 != '\0') goto LAB_1403a71b9;
      }
      else {
        cVar4 = SceneClass::FUN_140264cb0(*(undefined8 *)&param_1->field_0x2a0);
        if (cVar4 == '\0') {
LAB_1403a71b9:
          FUN_140264c80(*(undefined8 *)&param_1->field_0x2a0);
        }
      }
    }
    if (param_1->field_0x3a7 == '\0') {
      fVar20 = (float)FUN_1402647a0(*(undefined8 *)&param_1->field_0x2a0);
      FUN_140265280(*(undefined8 *)&param_1->field_0x2a0,(float)DAT_140b0a320 / (float)DAT_140b0a340
                    ,0);
      fVar21 = (float)FUN_1402647a0(*(undefined8 *)&param_1->field_0x2a0);
      if (((0.0 <= fVar21) && (0.0 <= fVar20)) && (fVar21 != fVar20)) {
        if (*(undefined8 **)&param_1->field_0x2a0 != (undefined8 *)0x0) {
          (**(code **)**(undefined8 **)&param_1->field_0x2a0)();
        }
        FUN_1404775d0(&DAT_140a2a120);
      }
    }
    if ((param_1->flags & 8) != 0) {
      fVar20 = DAT_14080080c;
      if ((*(longlong *)&param_1->field_0x2a0 == 0) ||
         (fVar20 = (float)FUN_1402647a0(), *(longlong *)&param_1->field_0x2a0 == 0)) {
        iVar6 = 0;
      }
      else {
        iVar6 = FUN_1402647c0();
      }
      if ((float)iVar6 <= fVar20 + DAT_140866874) {
        *(byte *)&param_1->flags = (byte)param_1->flags & 0xf7;
      }
    }
  }
  uVar3 = DAT_140800860;
  if (param_3 != '\0') goto LAB_1403a75b0;
  if (*(longlong *)&param_1->field_0x100 != 0) {
    FUN_14037da80(*(longlong *)&param_1->field_0x100 + 8);
  }
  if ((param_1->lua_ctx != (void *)0x0) && ((param_1->flags & 2) == 0)) {
    plVar11 = (longlong *)FUN_140246fb0(param_1->lua_ctx,"ServiceRate",0);
    plVar10 = plVar7;
    if (plVar11 == (longlong *)0x0) {
LAB_1403a7367:
      if (plVar11 != (longlong *)0x0) goto LAB_1403a736f;
    }
    else {
      (**(code **)*plVar11)(plVar11);
      puVar12 = (undefined *)(**(code **)(*plVar11 + 0x20))(plVar11);
      if (puVar12 == (undefined *)0x0) goto LAB_1403a7367;
      do {
        if (puVar12 == &DAT_140a157c0) {
          (**(code **)*plVar11)(plVar11);
          plVar10 = plVar11;
          break;
        }
        puVar12 = *(undefined **)(puVar12 + 8);
      } while (puVar12 != (undefined *)0x0);
LAB_1403a736f:
      (**(code **)(*plVar11 + 8))(plVar11);
    }
    plVar11 = (longlong *)FUN_140246fb0(param_1->lua_ctx,"LastService",0);
    if (plVar11 != (longlong *)0x0) {
      (**(code **)*plVar11)(plVar11);
      for (puVar12 = (undefined *)(**(code **)(*plVar11 + 0x20))(plVar11);
          puVar12 != (undefined *)0x0; puVar12 = *(undefined **)(puVar12 + 8)) {
        if (puVar12 == &DAT_140a157c0) {
          (**(code **)*plVar11)(plVar11);
          plVar7 = plVar11;
          goto LAB_1403a73ca;
        }
      }
    }
    if (plVar11 != (longlong *)0x0) {
LAB_1403a73ca:
      (**(code **)(*plVar11 + 8))(plVar11);
    }
    fVar20 = (float)*(int *)(*(longlong *)(*(longlong *)&param_1->field_0x2b8 + 0x20) + 0x10) *
             DAT_140b0a344;
    if (plVar10 == (longlong *)0x0) {
LAB_1403a74cc:
      if (plVar7 != (longlong *)0x0) goto LAB_1403a74d1;
    }
    else {
      if ((plVar7 == (longlong *)0x0) ||
         (dVar23 = (double)fVar20 - (double)plVar7[2],
         (double)plVar10[2] <= dVar23 && dVar23 != (double)plVar10[2])) {
        FUN_140247a90(param_1->lua_ctx);
        plVar11 = param_1->lua_ctx;
        if (*(char *)((longlong)plVar11 + 0x121) == '\0') {
          if (plVar7 == (longlong *)0x0) {
            plVar11 = (longlong *)FUN_140769c58(0x18);
            if (plVar11 != (longlong *)0x0) {
              *(undefined4 *)(plVar11 + 1) = 0;
              *plVar11 = (longlong)LuaValue<double,4>::vftable;
              (*(code *)LuaValue<double,4>::vftable[0])(plVar11);
              plVar7 = plVar11;
            }
            uVar22 = FUN_1401ffbb0(&DAT_140a13e24,0);
            plVar7[2] = (longlong)(double)(float)(uVar22 ^ uVar3);
          }
          else {
            plVar7[2] = (longlong)(double)fVar20;
          }
          FUN_140247000(param_1->lua_ctx,plVar7,"LastService");
        }
        else if (plVar11 != (longlong *)0x0) {
          (**(code **)(*plVar11 + 8))();
          param_1->lua_ctx = (void *)0x0;
        }
        goto LAB_1403a74cc;
      }
LAB_1403a74d1:
      (**(code **)(*plVar7 + 8))(plVar7);
    }
    if (plVar10 != (longlong *)0x0) {
      (**(code **)(*plVar10 + 8))(plVar10);
    }
  }
  pvVar1 = param_1->hardpoints;
  if ((pvVar1 != (void *)0x0) && ((param_1->flags & 0x40) == 0)) {
    iVar6 = *(int *)((longlong)pvVar1 + 0x10);
    lVar9 = (longlong)iVar6;
    if ((0 < iVar6) && (iVar13 = 0, 0 < iVar6)) {
      plVar7 = *(longlong **)((longlong)pvVar1 + 8);
      do {
        if ((*plVar7 != 0) && (*(char *)(*(longlong *)(*plVar7 + 0x20) + 0x4d) == '\x01')) {
          iVar13 = iVar13 + 1;
        }
        plVar7 = plVar7 + 1;
        lVar9 = lVar9 + -1;
      } while (lVar9 != 0);
      if ((0 < iVar13) && (*(char *)(*(longlong *)&param_1->field_0x298 + 0xa8) == '\x01')) {
        fVar21 = (float)FUN_140396070(param_1);
        fVar20 = fVar21 + _DAT_140b16d78;
        if (DAT_1407ffaf8 <= fVar21 + _DAT_140b16d78) {
          fVar20 = DAT_1407ffaf8;
        }
        fVar21 = (float)FUN_140396df0(param_1);
        if (fVar20 < fVar21) {
          FUN_1403a8ad0(param_1,fVar20);
        }
      }
    }
    FUN_1403a76b0(param_1,param_2);
  }
  *(float *)&param_1->field_0x320 = DAT_140b26fa4 * *(float *)&param_1->field_0x320;
LAB_1403a75b0:
  if ((((param_1->flags & 0x40000) != 0) && ((param_1->flags & 0x40) == 0)) &&
     (0.0 < *(float *)(*(longlong *)&param_1->field_0x298 + 0x234c))) {
    fVar20 = (float)FUN_1403727a0(*(longlong *)&param_1->field_0x298,param_1);
    fVar20 = (fVar20 / *(float *)(*(longlong *)&param_1->field_0x298 + 0x234c)) /
             (float)DAT_140b0a340;
    if ((0.0 < fVar20) &&
       (fVar21 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)(fVar20 / DAT_140805898) ^ uVar3),
       0.0 < fVar20 + fVar21)) {
      FUN_1403ab890(param_1,0x11);
    }
  }
  return;
}


