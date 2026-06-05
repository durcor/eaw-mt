/* FUN_140388b60  RVA 0x388b60  size 1998  params 8  ret undefined */


/* WARNING: Unknown calling convention */

GameObjectClass *
FUN_140388b60(GameObjectClass *param_1,GameObjectManagerClass *param_2,undefined8 *param_3,
             int param_4,undefined4 *param_5,undefined4 *param_6,char param_7,char param_8)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  char cVar4;
  int iVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong lVar11;
  int iVar12;
  
  iVar12 = 0;
  *(undefined4 *)&param_1->field_0x10 = 0;
  param_1->vtable = RootClass::vftable;
  *(undefined8 *)&param_1->field_0x20 = 0;
  param_1->vtable_8 = RootClass::vftable;
  *(undefined8 *)&param_1->field_0x30 = 0;
  param_1->vtable_18 = MultiLinkedListMember::vftable;
  param_1->vtable_28 = CullObjectClass::vftable;
  FUN_140240610(&param_1->listener_list);
  param_1->object_id = 0x3fffff;
  param_1->vtable = GameObjectClass::vftable;
  param_1->vtable_8 = GameObjectClass::vftable;
  param_1->vtable_18 = GameObjectClass::vftable;
  param_1->vtable_28 = GameObjectClass::vftable;
  param_1->listener_list = GameObjectClass::vftable;
  param_1->pos_x = 0.0;
  param_1->pos_y = 0.0;
  *(undefined8 *)&param_1->pos_z = 0;
  *(undefined8 *)&param_1->field_0x88 = 0;
  *(undefined8 *)&param_1->field_0x90 = 0;
  *(undefined8 *)&param_1->field_0x98 = 0;
  *(undefined8 *)&param_1->field_0xa0 = 0;
  *(undefined4 *)&param_1->field_0x54 = 0x3fffff;
  param_1->type_index = param_4;
  *(undefined4 *)&param_1->field_0x5c = 0;
  *(undefined8 *)&param_1->field_0x208 = 0;
  *(undefined8 *)&param_1->field_0x210 = 0;
  *(undefined8 *)&param_1->field_0x218 = 0;
  *(undefined8 *)&param_1->field_0x220 = 0;
  *(undefined8 *)&param_1->field_0x228 = 0;
  *(undefined8 *)&param_1->field_0x230 = 0;
  *(undefined8 *)&param_1->field_0x238 = 0;
  *(undefined8 *)&param_1->field_0x240 = 0;
  *(undefined8 *)&param_1->field_0x60 = 0;
  *(undefined8 *)&param_1->field_0x68 = 0;
  *(undefined8 *)&param_1->field_0x70 = 0;
  *(undefined8 *)&param_1->field_0x248 = 0x3f800000;
  *(undefined8 *)&param_1->field_0x250 = 0;
  *(undefined4 *)&param_1->field_0x258 = 0;
  *(undefined8 *)&param_1->field_0x25c = 0x3f800000;
  *(undefined8 *)&param_1->field_0x264 = 0;
  *(undefined4 *)&param_1->field_0x26c = 0;
  *(undefined8 *)&param_1->field_0x270 = 0x3f800000;
  param_1->behaviors = &param_1->field_0x280;
  *(undefined2 *)&param_1->field_0x288 = 0x100;
  param_1->behavior_count = 0;
  param_1->type_def = param_3;
  if (param_3 != (undefined8 *)0x0) {
    (**(code **)*param_3)(param_3);
  }
  param_1->render_anim_obj = (void *)0x0;
  puVar6 = (undefined8 *)&param_1[1].field_0x20;
  *(undefined8 *)&param_1->field_0x2a8 = 0;
  lVar11 = 0x4d;
  param_1->manager = param_2;
  param_1->order_ctx = (void *)0x0;
  *(undefined8 *)&param_1->field_0x2c0 = 0;
  *(undefined4 *)&param_1->field_0x2c8 = 0;
  param_1->hardpoints = (void *)0x0;
  param_1->lua_ctx = (void *)0x0;
  *(undefined8 *)&param_1->field_0x2e0 = 0;
  *(undefined4 *)&param_1->field_0x2f0 = 0;
  *(undefined8 *)&param_1->field_0x2f8 = 0;
  *(undefined8 *)&param_1->field_0x318 = 0;
  *(undefined8 *)&param_1->field_0x2e8 = 0;
  *(undefined8 *)&param_1->field_0x300 = 0;
  *(undefined8 *)&param_1->field_0x320 = 0;
  *(undefined8 *)&param_1->field_0x328 = 0;
  *(undefined2 *)&param_1->field_0x330 = 0xffff;
  *(undefined4 *)&param_1->field_0x39c = 0x1000000;
  *(undefined2 *)&param_1->field_0x3a5 = 1;
  param_1->field_0x3a7 = 0;
  *(undefined4 *)&param_1->field_0x3a8 = 0;
  param_1->field_0x3ac = 0xff;
  *(undefined4 *)&param_1[1].vtable = 0xffffffff;
  *(undefined4 *)((longlong)&param_1[1].vtable + 4) = 0;
  *(undefined2 *)&param_1[1].vtable_8 = 0;
  *(undefined8 *)&param_1[1].field_0x10 = 0;
  *(undefined4 *)&param_1[1].vtable_18 = 1;
  *(undefined1 *)((longlong)&param_1[1].vtable_18 + 4) = 0;
  do {
    *puVar6 = 0;
    *(undefined2 *)(puVar6 + 1) = 0;
    puVar6 = puVar6 + 2;
    lVar11 = lVar11 + -1;
  } while (lVar11 != 0);
  param_1->locomotor_state = (void *)0x0;
  *(undefined8 *)&param_1->field_0xb0 = 0;
  *(undefined8 *)&param_1->field_0xb8 = 0;
  *(undefined8 *)&param_1->field_0xc0 = 0;
  *(undefined8 *)&param_1->field_0xc8 = 0;
  *(undefined8 *)&param_1->field_0xd0 = 0;
  *(undefined8 *)&param_1->field_0xd8 = 0;
  *(undefined8 *)&param_1->field_0xe0 = 0;
  *(undefined8 *)&param_1->field_0xe8 = 0;
  *(undefined8 *)&param_1->field_0xf0 = 0;
  *(undefined8 *)&param_1->field_0xf8 = 0;
  *(undefined8 *)&param_1->field_0x100 = 0;
  *(undefined8 *)&param_1->field_0x108 = 0;
  *(undefined8 *)&param_1->field_0x110 = 0;
  *(undefined8 *)&param_1->field_0x118 = 0;
  *(undefined8 *)&param_1->field_0x120 = 0;
  *(undefined8 *)&param_1->field_0x128 = 0;
  *(undefined8 *)&param_1->field_0x130 = 0;
  *(undefined8 *)&param_1->field_0x150 = 0;
  *(undefined8 *)&param_1->field_0x138 = 0;
  *(undefined8 *)&param_1->field_0x140 = 0;
  *(undefined8 *)&param_1->field_0x148 = 0;
  *(undefined8 *)&param_1->field_0x158 = 0;
  *(undefined8 *)&param_1->field_0x160 = 0;
  *(undefined8 *)&param_1->field_0x168 = 0;
  *(undefined8 *)&param_1->field_0x180 = 0;
  *(undefined8 *)&param_1->field_0x188 = 0;
  *(undefined8 *)&param_1->field_0x190 = 0;
  *(undefined8 *)&param_1->field_0x198 = 0;
  *(undefined8 *)&param_1->field_0x1e0 = 0;
  *(undefined8 *)&param_1->field_0x1a0 = 0;
  *(undefined8 *)&param_1->field_0x1a8 = 0;
  *(undefined8 *)&param_1->field_0x1b0 = 0;
  *(undefined8 *)&param_1->field_0x1b8 = 0;
  *(undefined8 *)&param_1->field_0x1c0 = 0;
  *(undefined8 *)&param_1->field_0x1c8 = 0;
  *(undefined8 *)&param_1->field_0x1d0 = 0;
  *(undefined8 *)&param_1->field_0x1d8 = 0;
  *(undefined8 *)&param_1->field_0x1e8 = 0;
  *(undefined8 *)&param_1->field_0x170 = 0;
  *(undefined8 *)&param_1->field_0x178 = 0;
  *(undefined8 *)&param_1->field_0x1f0 = 0;
  *(undefined8 *)&param_1->field_0x1f8 = 0;
  *(undefined8 *)&param_1->field_0x200 = 0;
  *(undefined1 *)&param_1->flags = DAT_140a28648;
  *(undefined1 *)((longlong)&param_1->flags + 1) = DAT_140b2c379;
  *(undefined1 *)((longlong)&param_1->flags + 2) = DAT_140a28649;
  *(undefined1 *)((longlong)&param_1->flags + 3) = DAT_140b2c37a;
  param_1->field_0x3a4 = DAT_140a2864a;
  if (param_8 == '\x01') {
    *(byte *)&param_1->flags = (byte)param_1->flags | 0xc0;
  }
  if (param_1->manager == (GameObjectManagerClass *)0x0) {
    return param_1;
  }
  iVar5 = FUN_1402ac980();
  param_1->object_id = iVar5;
  param_1->pos_x = (float)*param_5;
  param_1->pos_y = (float)param_5[1];
  param_1->pos_z = (float)param_5[2];
  if ((param_7 == '\x01') && (*(float *)((longlong)param_3 + 0xdfc) != 0.0)) {
    param_1->pos_z = *(float *)((longlong)param_3 + 0xdfc) + param_1->pos_z;
  }
  *(float *)&param_1->field_0x9c = param_1->pos_x;
  *(float *)&param_1->field_0xa0 = param_1->pos_y;
  *(float *)&param_1->field_0xa4 = param_1->pos_z;
  uVar2 = param_6[1];
  uVar3 = param_6[2];
  *(undefined4 *)&param_1->field_0x84 = *param_6;
  *(undefined4 *)&param_1->field_0x88 = uVar2;
  *(undefined4 *)&param_1->field_0x8c = uVar3;
  *(undefined4 *)&param_1->field_0x90 = *(undefined4 *)&param_1->field_0x84;
  *(undefined4 *)&param_1->field_0x94 = *(undefined4 *)&param_1->field_0x88;
  *(undefined4 *)&param_1->field_0x98 = *(undefined4 *)&param_1->field_0x8c;
  *(undefined8 *)&param_1->field_0x248 = 0x3f800000;
  *(undefined8 *)&param_1->field_0x250 = 0;
  *(undefined4 *)&param_1->field_0x258 = 0;
  *(undefined8 *)&param_1->field_0x25c = 0x3f800000;
  *(undefined8 *)&param_1->field_0x264 = 0;
  *(undefined4 *)&param_1->field_0x26c = 0;
  *(undefined8 *)&param_1->field_0x270 = 0x3f800000;
  FUN_1403989a0(param_1,0,1);
  lVar11 = FUN_140294a40(&DAT_140a16fd0);
  lVar7 = FUN_140294a40(&DAT_140a16fd0);
  if (lVar11 == lVar7) {
    if ((DAT_140b1574e == '\0') && (iVar5 = iVar12, '\0' < (char)param_1->field_0x288)) {
      do {
        plVar8 = *(longlong **)((longlong)param_1->behaviors + (longlong)(char)iVar5 * 8);
        cVar4 = (**(code **)(*plVar8 + 0xa0))(plVar8,lVar11);
        if (cVar4 == '\0') {
          if (((param_1->field_0x39f != '\0') &&
              (param_1->field_0x39f = 0, param_1->render_anim_obj != (void *)0x0)) &&
             (plVar8 = (longlong *)FUN_140397680(param_1), param_1->render_anim_obj != (void *)0x0))
          {
            if (*(longlong *)&param_1->field_0x2e0 == 0) {
              if (plVar8 != (longlong *)0x0) {
                (**(code **)(*plVar8 + 0x30))(plVar8);
              }
            }
            else {
              if ((*(longlong *)(*(longlong *)&param_1->field_0x2e0 + 0x2a0) != 0) &&
                 (lVar7 = FUN_1402648b0(), lVar7 != 0)) {
                uVar10 = ActionDrivenHeroClashClass::vfunc_4(param_1->render_anim_obj);
                cVar4 = FUN_14012d860(lVar7,uVar10);
                if (cVar4 == '\0') {
                  (**(code **)(*plVar8 + 0x30))(plVar8,param_1->render_anim_obj);
                }
              }
              if ((*(longlong *)&param_1->field_0x2e0 != 0) &&
                 (0.0 < *(float *)((longlong)param_1->type_def + 0x23ec))) {
                piVar1 = (int *)(*(longlong *)&param_1->field_0x2e0 + 0x328);
                *piVar1 = *piVar1 + -1;
              }
              *(undefined8 *)&param_1->field_0x2e0 = 0;
              *(undefined8 *)&param_1->field_0x2f8 = 0;
            }
          }
          goto LAB_1403892db;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < (char)param_1->field_0x288);
    }
    if ((((param_1->field_0x39f == '\0') &&
         (param_1->field_0x39f = 1, param_1->render_anim_obj != (void *)0x0)) &&
        (plVar8 = (longlong *)FUN_140397680(param_1), plVar8 != (longlong *)0x0)) &&
       (param_1->render_anim_obj != (void *)0x0)) {
      lVar7 = ActionDrivenHeroClashClass::vfunc_4();
      if (lVar7 != 0) {
        *(byte *)(lVar7 + 0x32) =
             *(byte *)(lVar7 + 0x32) ^
             (*(byte *)((longlong)param_1->type_def + 0x86) ^ *(byte *)(lVar7 + 0x32)) & 1;
      }
      if (*(longlong *)&param_1->field_0x2e0 != 0) {
        lVar7 = FUN_140397680();
        lVar9 = FUN_140397680(param_1);
        if (lVar7 == lVar9) {
          if (*(longlong *)(*(longlong *)&param_1->field_0x2e0 + 0x2a0) != 0) {
            lVar7 = FUN_1402648b0();
            *(longlong *)&param_1->field_0x2f8 = lVar7;
            if (lVar7 != 0) {
              uVar10 = ActionDrivenHeroClashClass::vfunc_4(param_1->render_anim_obj);
              FUN_14012d770(lVar7,uVar10,*(undefined4 *)&param_1->field_0x2f0);
            }
          }
          goto LAB_1403892db;
        }
      }
      (**(code **)(*plVar8 + 0x28))(plVar8,param_1->render_anim_obj);
      if ((*(longlong *)&param_1->field_0x2e0 != 0) && (param_1->field_0x39e == '\0')) {
        FUN_14029bfe0(param_1->manager,param_1);
        param_1->field_0x39e = 1;
      }
    }
  }
LAB_1403892db:
  if ('\0' < (char)param_1->field_0x288) {
    do {
      plVar8 = *(longlong **)((longlong)param_1->behaviors + (longlong)(char)iVar12 * 8);
      (**(code **)(*plVar8 + 0xa8))(plVar8,lVar11);
      iVar12 = iVar12 + 1;
    } while (iVar12 < (char)param_1->field_0x288);
  }
  return param_1;
}


