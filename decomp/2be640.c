/* FUN_1402be640  RVA 0x2be640  size 1577  params 2  ret undefined */


/* WARNING: Unknown calling convention */

void FUN_1402be640(GameObjectManagerClass *param_1,uint param_2)

{
  undefined1 *puVar1;
  bool bVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  longlong *plVar7;
  longlong lVar8;
  undefined4 *puVar9;
  GameObjectClass *pGVar10;
  GameObjectClass *pGVar11;
  HANDLE pvVar12;
  int *piVar13;
  GameObjectClass *pGVar14;
  uint uVar15;
  int iVar16;
  undefined8 local_48;
  LPVOID local_40;
  undefined8 local_38;
  undefined **local_30;
  LPVOID local_28;
  ulonglong local_20;
  
  if (*(longlong *)&param_1->field_0xd8 != 0) {
    FUN_14020ed70();
  }
  pGVar14 = (GameObjectClass *)0x0;
  pGVar10 = pGVar14;
  if (0 < DAT_140a16fb0) {
    do {
      FUN_14020ed70((longlong)(int)pGVar10 * 0x38 + *(longlong *)&param_1->field_0xd0);
      uVar15 = (int)pGVar10 + 1;
      pGVar10 = (GameObjectClass *)(ulonglong)uVar15;
    } while ((int)uVar15 < DAT_140a16fb0);
  }
  plVar7 = *(longlong **)&param_1->field_0x20;
  if (plVar7 == (longlong *)0x0) {
    return;
  }
  if ((char)plVar7[0x13] == '\0') {
    return;
  }
  puVar1 = (param_1->master_update_list).head;
  iVar5 = (**(code **)(*plVar7 + 0xe0))();
  if ((iVar5 == 0) && (0 < DAT_140b1540c)) {
    cVar4 = '\x01';
  }
  else {
    cVar4 = '\0';
  }
  if ((((DAT_140b154d0 == '\0') && ((int)(DAT_140b2b368 - DAT_140b2b360 >> 3) == 0)) ||
      (cVar3 = FUN_14028af60(&DAT_140b153e0), cVar3 != '\0')) ||
     (cVar3 = FUN_1402b0350(param_1), cVar3 == '\0')) {
    if (cVar4 != '\0') goto LAB_1402be731;
LAB_1402be73a:
    for (; (void **)puVar1 != &(param_1->master_update_list).sentinel;
        puVar1 = *(undefined1 **)(puVar1 + 8)) {
      pGVar10 = (GameObjectClass *)(*(longlong *)(puVar1 + 0x18) + -0x18);
      if (*(longlong *)(puVar1 + 0x18) == 0) {
        pGVar10 = pGVar14;
      }
      FUN_1403a6b80(pGVar10,param_2,cVar4);
    }
  }
  else {
    cVar4 = '\x01';
LAB_1402be731:
    if (DAT_140b2766c != '\0') goto LAB_1402be73a;
  }
  iVar5 = (**(code **)(**(longlong **)&param_1->field_0x20 + 0xe0))();
  if ((iVar5 == 0) && (DAT_140b1540c < 1)) {
    for (puVar1 = (param_1->master_update_list).head;
        (void **)puVar1 != &(param_1->master_update_list).sentinel;
        puVar1 = *(undefined1 **)(puVar1 + 8)) {
      pGVar10 = (GameObjectClass *)(*(longlong *)(puVar1 + 0x18) - 0x18);
      if (*(longlong *)(puVar1 + 0x18) == 0) {
        pGVar10 = pGVar14;
      }
      if (((pGVar10 != (GameObjectClass *)0x0) && (pGVar10->field_0x335 != -1)) &&
         (lVar6 = (**(code **)((longlong)pGVar10->vtable + 0x10))(pGVar10,3), lVar6 != 0)) {
        FUN_1403fc750(lVar6);
      }
    }
  }
  iVar5 = DAT_140b0a320;
  for (puVar1 = (param_1->second_update_list).head; DAT_140b0a320 = iVar5,
      (void **)puVar1 != &(param_1->second_update_list).sentinel;
      puVar1 = *(undefined1 **)(puVar1 + 8)) {
    pGVar10 = (GameObjectClass *)(*(longlong *)(puVar1 + 0x18) - 0x18);
    if (*(longlong *)(puVar1 + 0x18) == 0) {
      pGVar10 = pGVar14;
    }
    DynamicTransformBehaviorClass::FUN_1403ac530(pGVar10,0);
    iVar5 = DAT_140b0a320;
  }
  iVar16 = *(int *)&param_1[1].vtable + -1;
  if (-1 < iVar16) {
    lVar6 = (longlong)iVar16 * 0x30;
    do {
      lVar8 = *(longlong *)&param_1->field_0x5f8;
      if (*(int *)(lVar6 + lVar8) <= iVar5) {
        plVar7 = (longlong *)
                 FUN_14029f810(param_1,*(undefined8 *)(lVar6 + 8 + lVar8),
                               *(undefined4 *)(lVar6 + 0x10 + lVar8),lVar8 + 0x14 + lVar6,
                               lVar8 + 0x20 + lVar6,*(undefined1 *)(lVar6 + 0x2c + lVar8),
                               *(undefined1 *)(lVar6 + 0x2d + lVar8));
        if ((*(char *)(lVar6 + 0x2e + lVar8) == '\x01') &&
           (lVar8 = (**(code **)(*plVar7 + 0x10))(plVar7,0x19), lVar8 != 0)) {
          *(undefined8 *)(lVar8 + 0x40) = 0;
          *(undefined4 *)(lVar8 + 0x48) = 0;
        }
        if (iVar16 < *(int *)&param_1[1].vtable) {
          FUN_1402b2fe0(&param_1->creation_params,iVar16,iVar16 + 1,
                        (*(int *)&param_1[1].vtable - iVar16) + -1);
          *(int *)&param_1[1].vtable = *(int *)&param_1[1].vtable + -1;
        }
      }
      lVar6 = lVar6 + -0x30;
      iVar16 = iVar16 + -1;
    } while (-1 < iVar16);
  }
  if (*(int *)&param_1->field_0x478 < 1) {
    pGVar10 = pGVar14;
    if (0 < *(int *)&param_1->field_0x490) {
      puVar9 = *(undefined4 **)&param_1->field_0x488;
      goto LAB_1402be910;
    }
  }
  else {
    puVar9 = *(undefined4 **)&param_1->field_0x470;
LAB_1402be910:
    pGVar10 = (GameObjectClass *)FUN_1402aca60(param_1,*puVar9);
  }
  iVar5 = *(int *)&param_1[1].field_0x18;
  if (iVar5 == 1) {
    if (((DAT_140b154d0 == '\0') ||
        (pGVar11 = (GameObjectClass *)FUN_1402aca60(param_1,DAT_140b154dc),
        pGVar11 == (GameObjectClass *)0x0)) &&
       (pGVar11 = pGVar10, pGVar10 == (GameObjectClass *)0x0)) {
      *(undefined4 *)&param_1[1].field_0x18 = 3;
      FUN_14028c720(&DAT_140b153e0,0);
      FUN_14028c6c0(&DAT_140b153e0);
      if (DAT_140b154d0 != '\0') {
        *(undefined4 *)&param_1[1].field_0x18 = 0xffffffff;
        FUN_14028f590(&DAT_140b153e0);
      }
      iVar5 = *(int *)&param_1[1].field_0x18;
      goto LAB_1402be9f2;
    }
    lVar6 = (**(code **)(**(longlong **)&pGVar11->field_0x2a0 + 0x38))();
    local_48 = (undefined **)CONCAT44(*(undefined4 *)(lVar6 + 0x1c),*(undefined4 *)(lVar6 + 0xc));
    local_40 = (LPVOID)CONCAT44(local_40._4_4_,*(undefined4 *)(lVar6 + 0x2c));
    FUN_14044a1a0(&DAT_140b30350,&local_48);
    FUN_1403de570(&DAT_140b27f60,pGVar11,0);
    *(undefined4 *)&param_1[1].field_0x18 = 2;
LAB_1402be9f7:
    if (DAT_140b303c0 == '\0') {
      *(undefined4 *)&param_1[1].field_0x18 = 3;
      FUN_14028c720(&DAT_140b153e0,0);
      FUN_14028c6c0(&DAT_140b153e0);
      if (DAT_140b154d0 != '\0') {
        *(undefined4 *)&param_1[1].field_0x18 = 0xffffffff;
        FUN_14028f590(&DAT_140b153e0);
      }
    }
  }
  else {
LAB_1402be9f2:
    if (iVar5 == 2) goto LAB_1402be9f7;
  }
  bVar2 = false;
  cVar4 = FUN_14028af60(&DAT_140b153e0);
  if ((cVar4 == '\0') &&
     ((DAT_140b154d0 != '\0' || ((int)(DAT_140b2b368 - DAT_140b2b360 >> 3) != 0)))) {
    iVar5 = FUN_140294a70(&DAT_140a16fd0);
    if (0 < *(int *)&param_1->field_0x478) {
      piVar13 = (int *)(*(longlong *)&param_1->field_0x470 + 4);
      pGVar10 = pGVar14;
      do {
        if ((piVar13[1] == iVar5) || (*piVar13 == iVar5)) goto LAB_1402beae6;
        uVar15 = (int)pGVar10 + 1;
        pGVar10 = (GameObjectClass *)(ulonglong)uVar15;
        piVar13 = piVar13 + 0x16;
      } while ((int)uVar15 < *(int *)&param_1->field_0x478);
    }
    if (0 < *(int *)&param_1->field_0x490) {
      piVar13 = (int *)(*(longlong *)&param_1->field_0x488 + 4);
      do {
        if ((piVar13[1] == iVar5) || (*piVar13 == iVar5)) goto LAB_1402beae6;
        uVar15 = (int)pGVar14 + 1;
        pGVar14 = (GameObjectClass *)(ulonglong)uVar15;
        piVar13 = piVar13 + 0x16;
      } while ((int)uVar15 < *(int *)&param_1->field_0x490);
    }
  }
LAB_1402beae9:
  local_28 = (LPVOID)0x0;
  local_30 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  local_20 = 0;
  local_40 = (LPVOID)0x0;
  local_38 = 0;
  cVar4 = FUN_1402ad100(param_1,&local_48,&local_30);
  if ((((cVar4 != '\0') &&
       ((DAT_140b15618 == (code *)0x0 || (cVar4 = (*DAT_140b15618)(), cVar4 == '\0')))) &&
      ((DAT_140b15640 == (code *)0x0 || (cVar4 = (*DAT_140b15640)(), cVar4 == '\0')))) &&
     ((((DAT_140b15650 == (code *)0x0 || (cVar4 = (*DAT_140b15650)(), cVar4 == '\0')) &&
       ((*(int *)(DAT_140b2eea0 + 0x44) != 1 ||
        (cVar4 = FUN_14028af60(&DAT_140b153e0), cVar4 != '\0')))) && (!bVar2)))) {
    FUN_1402b92e0(param_1,&local_48,&local_30);
  }
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  if (local_40 != (LPVOID)0x0) {
    if ((local_38 & 0x8000000000000000) == 0) {
      thunk_FUN_1407864b8();
    }
    else {
      pvVar12 = GetProcessHeap();
      HeapFree(pvVar12,0,local_40);
    }
    local_40 = (LPVOID)0x0;
    local_38 = (ulonglong)(local_38._4_4_ & 0x80000000) << 0x20;
  }
  local_30 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  if (local_28 != (LPVOID)0x0) {
    if ((local_20 & 0x8000000000000000) == 0) {
      thunk_FUN_1407864b8();
    }
    else {
      pvVar12 = GetProcessHeap();
      HeapFree(pvVar12,0,local_28);
    }
  }
  if (((*(int *)&param_1->field_0x490 != 0) &&
      ((DAT_140b155f8 == (code *)0x0 || (cVar4 = (*DAT_140b155f8)(), cVar4 == '\0')))) &&
     (((DAT_140b15640 == (code *)0x0 || (cVar4 = (*DAT_140b15640)(), cVar4 == '\0')) &&
      (((DAT_140b15650 == (code *)0x0 || (cVar4 = (*DAT_140b15650)(), cVar4 == '\0')) && (!bVar2))))
     )) {
    FUN_1402b76a0(param_1);
  }
  FUN_1402a62d0(param_1);
  return;
LAB_1402beae6:
  bVar2 = true;
  goto LAB_1402beae9;
}


