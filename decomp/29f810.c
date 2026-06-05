/* FUN_14029f810  RVA 0x29f810  size 2201  params 7  ret undefined */


/* WARNING: Unknown calling convention */

longlong *
FUN_14029f810(GameObjectManagerClass *param_1,longlong param_2,int param_3,undefined4 *param_4,
             undefined4 *param_5,char param_6,char param_7)

{
  undefined1 *puVar1;
  char cVar2;
  int iVar3;
  GameObjectClass *pGVar4;
  longlong *plVar5;
  ulonglong uVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong *plVar9;
  undefined8 uVar10;
  longlong lVar11;
  int iVar12;
  longlong *plVar13;
  uint uVar14;
  longlong *plVar15;
  undefined4 uVar16;
  longlong local_98;
  longlong *local_90;
  undefined1 local_88 [16];
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined8 local_64;
  undefined4 local_5c;
  undefined8 local_58;
  undefined4 local_50;
  undefined4 local_4c;
  
  if (param_2 == 0) {
    return (longlong *)0x0;
  }
  pGVar4 = (GameObjectClass *)FUN_140769c58(0x8a0);
  plVar15 = (longlong *)0x0;
  plVar5 = plVar15;
  if (pGVar4 != (GameObjectClass *)0x0) {
    plVar5 = (longlong *)
             FUN_140388b60(pGVar4,param_1,(undefined8 *)param_2,param_3,param_4,param_5,param_6,
                           param_7);
  }
  plVar8 = plVar5 + 3;
  if (plVar5 == (longlong *)0x0) {
    plVar8 = plVar15;
  }
  cVar2 = FUN_14020a9b0(&param_1->bucket_primary,plVar8,1);
  if (cVar2 == '\x01') {
    (**(code **)*plVar5)(plVar5);
  }
  local_98 = CONCAT44(local_98._4_4_,(int)plVar5[10]);
  local_90 = plVar5;
  FUN_140241df0(&param_1->id_index_map,local_88,&local_98);
  cVar2 = FUN_140370320(param_2,0x4a,0xffffffff);
  if (cVar2 != '\0') {
    FUN_14020a9b0(&param_1->owning_multilist,plVar8,1);
  }
  cVar2 = FUN_14020a9b0((void *)((longlong)param_1->category_array + (longlong)param_3 * 0x48),
                        plVar8,1);
  if (cVar2 == '\x01') {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(byte *)(plVar5 + 0x74) & 0x40) == 0) {
    cVar2 = *(char *)(plVar5[0x53] + 0x41);
  }
  else {
    cVar2 = *(char *)(plVar5[0x53] + 0x42);
  }
  if (cVar2 != '\0') {
    plVar13 = (longlong *)((longlong)(int)plVar5[0xb] * 0x38 + (longlong)param_1->type_table);
    (**(code **)(*plVar13 + 8))(plVar13,plVar5 + 5);
    (**(code **)(*(longlong *)param_1->notify_list_d8 + 8))(param_1->notify_list_d8,plVar5 + 5);
  }
  uVar6 = FUN_140397900(plVar5,1);
  plVar13 = param_1->optional_observer;
  if ((((plVar13 != (longlong *)0x0) && (lVar7 = plVar5[0x53], *(char *)(lVar7 + 0x54) == '\0')) &&
      (*(char *)(lVar7 + 0x56) == '\0')) &&
     ((*(char *)(lVar7 + 0x57) == '\0' && ((uVar6 & 0xffffffef) != 0)))) {
    (**(code **)(*plVar13 + 8))(plVar13,plVar5 + 5);
  }
  cVar2 = FUN_14039ac80(plVar5);
  if (cVar2 != '\0') {
    (**(code **)(*(longlong *)param_1->notify_list_e0 + 8))(param_1->notify_list_e0,plVar5 + 5);
  }
  cVar2 = FUN_140374da0(param_2,plVar5);
  if (cVar2 != '\0') {
    cVar2 = FUN_14020a9b0(&param_1->category_list_1c8,plVar8,1);
    if (cVar2 == '\x01') {
      (**(code **)*plVar5)(plVar5);
    }
    if ((DAT_140b15490 == '\0') && (plVar5[0x20] != 0)) {
      lVar7 = plVar5[0x20] + 8;
      cVar2 = *(char *)(plVar5[0x53] + 0xe8);
      local_98 = lVar7;
      for (puVar1 = param_1->observer_head; (void **)puVar1 != &param_1->observer_sentinel;
          puVar1 = *(undefined1 **)(puVar1 + 8)) {
        plVar13 = (longlong *)(*(longlong *)(puVar1 + 0x18) + -0x18);
        if (*(longlong *)(puVar1 + 0x18) == 0) {
          plVar13 = plVar15;
        }
        if (((*(longlong *)(plVar13[0x53] + 0x1648) != 0) && (plVar13 != (longlong *)0x0)) &&
           ((plVar13 != plVar5 || (cVar2 != '\0')))) {
          FUN_14037cab0(lVar7);
        }
      }
    }
  }
  if ((0.0 < *(float *)(param_2 + 0x64c)) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_210,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(char *)(param_2 + 0x52) != '\0') &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_258,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(char *)((longlong)plVar5 + 0x334) != -1) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_2a0,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if (*(char *)((longlong)plVar5 + 0x35e) != -1) {
    iVar3 = *(int *)&param_1->field_0x3d0;
    iVar12 = *(int *)&param_1->field_0x3d4 * 2 >> 1;
    if (iVar12 <= iVar3) {
      iVar3 = iVar12 / 10;
      if (iVar12 / 10 < 10) {
        iVar3 = 10;
      }
      FUN_140047aa0(&param_1->field_0x3c0,iVar3 + iVar12,0,1);
      iVar3 = *(int *)&param_1->field_0x3d0;
    }
    *(longlong **)(*(longlong *)&param_1->field_0x3c8 + (longlong)iVar3 * 8) = plVar5;
    *(int *)&param_1->field_0x3d0 = *(int *)&param_1->field_0x3d0 + 1;
  }
  if ((((0.0 < *(float *)(param_2 + 0x1e20)) || (0.0 < *(float *)(param_2 + 0x1df8))) ||
      (0.0 < *(float *)(param_2 + 0x21f4))) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_2e8,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if (((char)plVar5[0x6d] != -1) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_330,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(char *)((longlong)plVar5 + 0x397) != -1) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_378,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(char *)((longlong)plVar5 + 0x335) != -1) &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_560,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if ((*(char *)(param_2 + 0x13) != '\0') &&
     (cVar2 = FUN_14020a9b0(&param_1->category_list_5a8,plVar8,1), cVar2 == '\x01')) {
    (**(code **)*plVar5)(plVar5);
  }
  if (*(char *)((longlong)plVar5 + 0x34c) != -1) {
    *(undefined1 *)((longlong)plVar5 + 0x3cb) = 1;
  }
  if (plVar5[0x54] != 0) {
    FUN_140265280(plVar5[0x54],(float)DAT_140b0a320 / (float)DAT_140b0a340,0);
  }
  FUN_1402b38c0(param_1,plVar5);
  plVar8 = plVar15;
  if (((*(byte *)(plVar5 + 0x74) & 0x40) == 0) && ((char)plVar5[0x69] == -1)) {
    FUN_1402b3670(param_1,plVar5);
    FUN_1403a82a0(plVar5);
  }
  do {
    iVar3 = FUN_140374b50(plVar5[0x53],plVar8);
    if (iVar3 == 0x20) {
      FUN_1404cce30(plVar5);
      break;
    }
    uVar14 = (int)plVar8 + 1;
    plVar8 = (longlong *)(ulonglong)uVar14;
  } while ((int)uVar14 < 2);
  if (0.0 < *(float *)(plVar5[0x53] + 0x23e8)) {
    FUN_1404cce30(plVar5);
    if (plVar5[0x54] != 0) {
      FUN_140265ae0(plVar5[0x54],7,1);
    }
  }
  FUN_1403935b0(plVar5,param_1->create_flag_63c,0,1);
  if ((*(longlong *)(param_2 + 0x600) != 0) && (cVar2 = FUN_14013ecf0(), cVar2 != '\0')) {
    plVar8 = (longlong *)(param_2 + 0x5f0);
    if (0xf < *(ulonglong *)(param_2 + 0x608)) {
      plVar8 = (longlong *)*plVar8;
    }
    plVar8 = (longlong *)FUN_140488e00(plVar8);
    if (plVar8 != (longlong *)0x0) {
      (**(code **)*plVar8)(plVar8);
    }
    plVar13 = (longlong *)(**(code **)(*plVar8 + 0x28))(plVar8);
    if (plVar13 != (longlong *)0x0) {
      (**(code **)*plVar13)(plVar13);
    }
    local_6c = *param_4;
    local_5c = param_4[1];
    local_4c = param_4[2];
    local_78 = 0x3f800000;
    local_70 = 0;
    local_68 = 0;
    local_64 = 0x3f800000;
    local_58 = 0;
    local_50 = 0x3f800000;
    (**(code **)(*plVar13 + 0x58))(plVar13,&local_78);
    plVar9 = DAT_140b15418;
    if ((DAT_140b15418 != (longlong *)0x0) &&
       (lVar7 = (**(code **)(*DAT_140b15418 + 0x100))(DAT_140b15418), lVar7 != 0)) {
      plVar9 = (longlong *)(**(code **)(*plVar9 + 0x100))(plVar9);
      plVar9 = (longlong *)(**(code **)(*plVar9 + 0x78))(plVar9);
      if (plVar9 != (longlong *)0x0) {
        (**(code **)(*plVar9 + 0xa8))(plVar9,plVar13);
      }
    }
    (**(code **)(*plVar13 + 8))(plVar13);
    (**(code **)(*plVar8 + 8))(plVar8);
  }
  if (*(int *)(param_2 + 0x2138) != -1) {
    FUN_140470700(&DAT_140a29fa0,*(int *)(param_2 + 0x2138),plVar5 + 0x49);
  }
  if (((*(char *)(plVar5[0x53] + 0x3b) == '\0') &&
      (lVar7 = (**(code **)(*plVar5 + 0x10))(plVar5,8), lVar7 == 0)) &&
     (lVar7 = (**(code **)(*plVar5 + 0x10))(plVar5,0xc), lVar7 == 0)) {
    uVar10 = FUN_140058570();
    FUN_140220ed0(uVar10,param_1,0x1c,plVar5);
  }
  if (((*(longlong **)&param_1->field_0x20 != (longlong *)0x0) &&
      (cVar2 = (**(code **)(**(longlong **)&param_1->field_0x20 + 0xf0))(), cVar2 != '\0')) &&
     ((0 < *(int *)(plVar5[0x53] + 0x2120) &&
      ((lVar7 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar5[0xb]), lVar7 != 0 &&
       (plVar8 = plVar15, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))))))) {
    do {
      lVar11 = FUN_140294bc0(&DAT_140a16fd0,plVar8);
      if ((lVar11 != 0) && (cVar2 = FUN_1402823e0(lVar7,lVar11), cVar2 != '\0')) {
        FUN_140289010(lVar11);
      }
      uVar14 = (int)plVar8 + 1;
      plVar8 = (longlong *)(ulonglong)uVar14;
    } while ((int)uVar14 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  if ((0.0 < *(float *)(plVar5[0x53] + 0x23ec)) &&
     (lVar7 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar5[0xb]), lVar7 != 0)) {
    FUN_140282180(lVar7,1);
  }
  iVar3 = FUN_140374b50(plVar5[0x53],0);
  if ((iVar3 == 0x40) && (lVar7 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar5[0xb]), lVar7 != 0)) {
    FUN_1402820b0(lVar7,1);
  }
  FUN_1403a8ad0(plVar5,DAT_1407ffaf8);
  uVar16 = FUN_1403725f0(plVar5[0x53],plVar5);
  FUN_1403a91e0(plVar5,uVar16);
  iVar3 = FUN_140294df0(plVar5);
  if (0 < iVar3) {
    do {
      lVar7 = FUN_140396cb0(plVar5,plVar15);
      if (lVar7 != 0) {
        uVar16 = FUN_140385ae0(lVar7);
        FUN_140387ca0(lVar7,uVar16);
      }
      uVar14 = (int)plVar15 + 1;
      plVar15 = (longlong *)(ulonglong)uVar14;
      iVar3 = FUN_140294df0(plVar5);
    } while ((int)uVar14 < iVar3);
  }
  return plVar5;
}


