/* FUN_140388b60  RVA 0x388b60  size 1998  params 0  ret undefined */


undefined8 *
FUN_140388b60(undefined8 *param_1,undefined8 param_2,undefined8 *param_3,undefined4 param_4,
             undefined4 *param_5,undefined4 *param_6,char param_7,char param_8)

{
  int *piVar1;
  undefined4 uVar2;
  char cVar3;
  undefined4 uVar4;
  undefined8 *puVar5;
  longlong lVar6;
  longlong *plVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong lVar10;
  int iVar11;
  int iVar12;
  
  iVar12 = 0;
  *(undefined4 *)(param_1 + 2) = 0;
  *param_1 = RootClass::vftable;
  param_1[4] = 0;
  param_1[1] = RootClass::vftable;
  param_1[6] = 0;
  param_1[3] = MultiLinkedListMember::vftable;
  param_1[5] = CullObjectClass::vftable;
  FUN_140240610(param_1 + 7);
  *(undefined4 *)(param_1 + 10) = 0x3fffff;
  *param_1 = GameObjectClass::vftable;
  param_1[1] = GameObjectClass::vftable;
  param_1[3] = GameObjectClass::vftable;
  param_1[5] = GameObjectClass::vftable;
  param_1[7] = GameObjectClass::vftable;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  *(undefined4 *)((longlong)param_1 + 0x54) = 0x3fffff;
  *(undefined4 *)(param_1 + 0xb) = param_4;
  *(undefined4 *)((longlong)param_1 + 0x5c) = 0;
  param_1[0x41] = 0;
  param_1[0x42] = 0;
  param_1[0x43] = 0;
  param_1[0x44] = 0;
  param_1[0x45] = 0;
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  param_1[0x48] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0x49] = 0x3f800000;
  param_1[0x4a] = 0;
  *(undefined4 *)(param_1 + 0x4b) = 0;
  *(undefined8 *)((longlong)param_1 + 0x25c) = 0x3f800000;
  *(undefined8 *)((longlong)param_1 + 0x264) = 0;
  *(undefined4 *)((longlong)param_1 + 0x26c) = 0;
  param_1[0x4e] = 0x3f800000;
  param_1[0x4f] = param_1 + 0x50;
  *(undefined2 *)(param_1 + 0x51) = 0x100;
  *(undefined4 *)(param_1 + 0x52) = 0;
  param_1[0x53] = param_3;
  if (param_3 != (undefined8 *)0x0) {
    (**(code **)*param_3)(param_3);
  }
  param_1[0x54] = 0;
  puVar5 = param_1 + 0x7a;
  param_1[0x55] = 0;
  lVar10 = 0x4d;
  param_1[0x57] = param_2;
  param_1[0x56] = 0;
  param_1[0x58] = 0;
  *(undefined4 *)(param_1 + 0x59) = 0;
  param_1[0x5a] = 0;
  param_1[0x5b] = 0;
  param_1[0x5c] = 0;
  *(undefined4 *)(param_1 + 0x5e) = 0;
  param_1[0x5f] = 0;
  param_1[99] = 0;
  param_1[0x5d] = 0;
  param_1[0x60] = 0;
  param_1[100] = 0;
  param_1[0x65] = 0;
  *(undefined2 *)(param_1 + 0x66) = 0xffff;
  *(undefined4 *)((longlong)param_1 + 0x39c) = 0x1000000;
  *(undefined2 *)((longlong)param_1 + 0x3a5) = 1;
  *(undefined1 *)((longlong)param_1 + 0x3a7) = 0;
  *(undefined4 *)(param_1 + 0x75) = 0;
  *(undefined1 *)((longlong)param_1 + 0x3ac) = 0xff;
  *(undefined4 *)(param_1 + 0x76) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3b4) = 0;
  *(undefined2 *)(param_1 + 0x77) = 0;
  param_1[0x78] = 0;
  *(undefined4 *)(param_1 + 0x79) = 1;
  *(undefined1 *)((longlong)param_1 + 0x3cc) = 0;
  do {
    *puVar5 = 0;
    *(undefined2 *)(puVar5 + 1) = 0;
    puVar5 = puVar5 + 2;
    lVar10 = lVar10 + -1;
  } while (lVar10 != 0);
  param_1[0x15] = 0;
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  param_1[0x1d] = 0;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  param_1[0x23] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x2a] = 0;
  param_1[0x27] = 0;
  param_1[0x28] = 0;
  param_1[0x29] = 0;
  param_1[0x2b] = 0;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  param_1[0x30] = 0;
  param_1[0x31] = 0;
  param_1[0x32] = 0;
  param_1[0x33] = 0;
  param_1[0x3c] = 0;
  param_1[0x34] = 0;
  param_1[0x35] = 0;
  param_1[0x36] = 0;
  param_1[0x37] = 0;
  param_1[0x38] = 0;
  param_1[0x39] = 0;
  param_1[0x3a] = 0;
  param_1[0x3b] = 0;
  param_1[0x3d] = 0;
  param_1[0x2e] = 0;
  param_1[0x2f] = 0;
  param_1[0x3e] = 0;
  param_1[0x3f] = 0;
  param_1[0x40] = 0;
  *(undefined1 *)(param_1 + 0x74) = DAT_140a28648;
  *(undefined1 *)((longlong)param_1 + 0x3a1) = DAT_140b2c379;
  *(undefined1 *)((longlong)param_1 + 0x3a2) = DAT_140a28649;
  *(undefined1 *)((longlong)param_1 + 0x3a3) = DAT_140b2c37a;
  *(undefined1 *)((longlong)param_1 + 0x3a4) = DAT_140a2864a;
  if (param_8 == '\x01') {
    *(byte *)(param_1 + 0x74) = *(byte *)(param_1 + 0x74) | 0xc0;
  }
  if (param_1[0x57] == 0) {
    return param_1;
  }
  uVar4 = FUN_1402ac980();
  *(undefined4 *)(param_1 + 10) = uVar4;
  *(undefined4 *)(param_1 + 0xf) = *param_5;
  *(undefined4 *)((longlong)param_1 + 0x7c) = param_5[1];
  *(undefined4 *)(param_1 + 0x10) = param_5[2];
  if ((param_7 == '\x01') && (*(float *)((longlong)param_3 + 0xdfc) != 0.0)) {
    *(float *)(param_1 + 0x10) = *(float *)((longlong)param_3 + 0xdfc) + *(float *)(param_1 + 0x10);
  }
  *(undefined4 *)((longlong)param_1 + 0x9c) = *(undefined4 *)(param_1 + 0xf);
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)((longlong)param_1 + 0x7c);
  *(undefined4 *)((longlong)param_1 + 0xa4) = *(undefined4 *)(param_1 + 0x10);
  uVar4 = param_6[1];
  uVar2 = param_6[2];
  *(undefined4 *)((longlong)param_1 + 0x84) = *param_6;
  *(undefined4 *)(param_1 + 0x11) = uVar4;
  *(undefined4 *)((longlong)param_1 + 0x8c) = uVar2;
  *(undefined4 *)(param_1 + 0x12) = *(undefined4 *)((longlong)param_1 + 0x84);
  *(undefined4 *)((longlong)param_1 + 0x94) = *(undefined4 *)(param_1 + 0x11);
  *(undefined4 *)(param_1 + 0x13) = *(undefined4 *)((longlong)param_1 + 0x8c);
  param_1[0x49] = 0x3f800000;
  param_1[0x4a] = 0;
  *(undefined4 *)(param_1 + 0x4b) = 0;
  *(undefined8 *)((longlong)param_1 + 0x25c) = 0x3f800000;
  *(undefined8 *)((longlong)param_1 + 0x264) = 0;
  *(undefined4 *)((longlong)param_1 + 0x26c) = 0;
  param_1[0x4e] = 0x3f800000;
  FUN_1403989a0(param_1,0,1);
  lVar10 = FUN_140294a40(&DAT_140a16fd0);
  lVar6 = FUN_140294a40(&DAT_140a16fd0);
  if (lVar10 == lVar6) {
    if ((DAT_140b1574e == '\0') && (iVar11 = iVar12, '\0' < *(char *)(param_1 + 0x51))) {
      do {
        plVar7 = *(longlong **)(param_1[0x4f] + (longlong)(char)iVar11 * 8);
        cVar3 = (**(code **)(*plVar7 + 0xa0))(plVar7,lVar10);
        if (cVar3 == '\0') {
          if (((*(char *)((longlong)param_1 + 0x39f) != '\0') &&
              (*(undefined1 *)((longlong)param_1 + 0x39f) = 0, param_1[0x54] != 0)) &&
             (plVar7 = (longlong *)FUN_140397680(param_1), param_1[0x54] != 0)) {
            if (param_1[0x5c] == 0) {
              if (plVar7 != (longlong *)0x0) {
                (**(code **)(*plVar7 + 0x30))(plVar7);
              }
            }
            else {
              if ((*(longlong *)(param_1[0x5c] + 0x2a0) != 0) &&
                 (lVar6 = FUN_1402648b0(), lVar6 != 0)) {
                uVar9 = ActionDrivenHeroClashClass::vfunc_4(param_1[0x54]);
                cVar3 = FUN_14012d860(lVar6,uVar9);
                if (cVar3 == '\0') {
                  (**(code **)(*plVar7 + 0x30))(plVar7,param_1[0x54]);
                }
              }
              if ((param_1[0x5c] != 0) && (0.0 < *(float *)(param_1[0x53] + 0x23ec))) {
                piVar1 = (int *)(param_1[0x5c] + 0x328);
                *piVar1 = *piVar1 + -1;
              }
              param_1[0x5c] = 0;
              param_1[0x5f] = 0;
            }
          }
          goto LAB_1403892db;
        }
        iVar11 = iVar11 + 1;
      } while (iVar11 < *(char *)(param_1 + 0x51));
    }
    if ((((*(char *)((longlong)param_1 + 0x39f) == '\0') &&
         (*(undefined1 *)((longlong)param_1 + 0x39f) = 1, param_1[0x54] != 0)) &&
        (plVar7 = (longlong *)FUN_140397680(param_1), plVar7 != (longlong *)0x0)) &&
       (param_1[0x54] != 0)) {
      lVar6 = ActionDrivenHeroClashClass::vfunc_4();
      if (lVar6 != 0) {
        *(byte *)(lVar6 + 0x32) =
             *(byte *)(lVar6 + 0x32) ^
             (*(byte *)(param_1[0x53] + 0x86) ^ *(byte *)(lVar6 + 0x32)) & 1;
      }
      if (param_1[0x5c] != 0) {
        lVar6 = FUN_140397680();
        lVar8 = FUN_140397680(param_1);
        if (lVar6 == lVar8) {
          if (*(longlong *)(param_1[0x5c] + 0x2a0) != 0) {
            lVar6 = FUN_1402648b0();
            param_1[0x5f] = lVar6;
            if (lVar6 != 0) {
              uVar9 = ActionDrivenHeroClashClass::vfunc_4(param_1[0x54]);
              FUN_14012d770(lVar6,uVar9,*(undefined4 *)(param_1 + 0x5e));
            }
          }
          goto LAB_1403892db;
        }
      }
      (**(code **)(*plVar7 + 0x28))(plVar7,param_1[0x54]);
      if ((param_1[0x5c] != 0) && (*(char *)((longlong)param_1 + 0x39e) == '\0')) {
        FUN_14029bfe0(param_1[0x57],param_1);
        *(undefined1 *)((longlong)param_1 + 0x39e) = 1;
      }
    }
  }
LAB_1403892db:
  if ('\0' < *(char *)(param_1 + 0x51)) {
    do {
      plVar7 = *(longlong **)(param_1[0x4f] + (longlong)(char)iVar12 * 8);
      (**(code **)(*plVar7 + 0xa8))(plVar7,lVar10);
      iVar12 = iVar12 + 1;
    } while (iVar12 < *(char *)(param_1 + 0x51));
  }
  return param_1;
}


