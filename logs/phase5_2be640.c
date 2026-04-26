/* =========================================================
 * FUN_1402be640  (RVA 0x2be640)
 * size: 1577 bytes  params: 0  return: undefined
 * ========================================================= */


void FUN_1402be640(longlong param_1,undefined4 param_2)

{
  bool bVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined4 *puVar7;
  longlong *plVar8;
  HANDLE pvVar9;
  int *piVar10;
  longlong *plVar11;
  longlong lVar12;
  uint uVar13;
  int iVar14;
  undefined8 local_48;
  LPVOID local_40;
  undefined8 local_38;
  undefined **local_30;
  LPVOID local_28;
  ulonglong local_20;
  
  if (*(longlong *)(param_1 + 0xd8) != 0) {
    FUN_14020ed70();
  }
  plVar11 = (longlong *)0x0;
  plVar6 = plVar11;
  if (0 < DAT_140a16fb0) {
    do {
      FUN_14020ed70((longlong)(int)plVar6 * 0x38 + *(longlong *)(param_1 + 0xd0));
      uVar13 = (int)plVar6 + 1;
      plVar6 = (longlong *)(ulonglong)uVar13;
    } while ((int)uVar13 < DAT_140a16fb0);
  }
  plVar6 = *(longlong **)(param_1 + 0x20);
  if (plVar6 == (longlong *)0x0) {
    return;
  }
  if ((char)plVar6[0x13] == '\0') {
    return;
  }
  lVar12 = *(longlong *)(param_1 + 0xf8);
  iVar4 = (**(code **)(*plVar6 + 0xe0))();
  if ((iVar4 == 0) && (0 < DAT_140b1540c)) {
    cVar3 = '\x01';
  }
  else {
    cVar3 = '\0';
  }
  if ((((DAT_140b154d0 == '\0') && ((int)(DAT_140b2b368 - DAT_140b2b360 >> 3) == 0)) ||
      (cVar2 = FUN_14028af60(&DAT_140b153e0), cVar2 != '\0')) ||
     (cVar2 = FUN_1402b0350(param_1), cVar2 == '\0')) {
    if (cVar3 != '\0') goto LAB_1402be731;
LAB_1402be73a:
    for (; lVar12 != param_1 + 0xf0; lVar12 = *(longlong *)(lVar12 + 8)) {
      plVar6 = (longlong *)(*(longlong *)(lVar12 + 0x18) + -0x18);
      if (*(longlong *)(lVar12 + 0x18) == 0) {
        plVar6 = plVar11;
      }
      FUN_1403a6b80(plVar6,param_2,cVar3);
    }
  }
  else {
    cVar3 = '\x01';
LAB_1402be731:
    if (DAT_140b2766c != '\0') goto LAB_1402be73a;
  }
  iVar4 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0xe0))();
  if ((iVar4 == 0) && (DAT_140b1540c < 1)) {
    for (lVar12 = *(longlong *)(param_1 + 0xf8); lVar12 != param_1 + 0xf0;
        lVar12 = *(longlong *)(lVar12 + 8)) {
      plVar6 = (longlong *)(*(longlong *)(lVar12 + 0x18) + -0x18);
      if (*(longlong *)(lVar12 + 0x18) == 0) {
        plVar6 = plVar11;
      }
      if (((plVar6 != (longlong *)0x0) && (*(char *)((longlong)plVar6 + 0x335) != -1)) &&
         (lVar5 = (**(code **)(*plVar6 + 0x10))(plVar6,3), lVar5 != 0)) {
        FUN_1403fc750(lVar5);
      }
    }
  }
  iVar4 = DAT_140b0a320;
  for (lVar12 = *(longlong *)(param_1 + 0x140); DAT_140b0a320 = iVar4, lVar12 != param_1 + 0x138;
      lVar12 = *(longlong *)(lVar12 + 8)) {
    plVar6 = (longlong *)(*(longlong *)(lVar12 + 0x18) + -0x18);
    if (*(longlong *)(lVar12 + 0x18) == 0) {
      plVar6 = plVar11;
    }
    FUN_1403ac530(plVar6,0);
    iVar4 = DAT_140b0a320;
  }
  iVar14 = *(int *)(param_1 + 0x600) + -1;
  if (-1 < iVar14) {
    lVar12 = (longlong)iVar14 * 0x30;
    do {
      lVar5 = *(longlong *)(param_1 + 0x5f8);
      if (*(int *)(lVar12 + lVar5) <= iVar4) {
        plVar6 = (longlong *)
                 FUN_14029f810(param_1,*(undefined8 *)(lVar12 + 8 + lVar5),
                               *(undefined4 *)(lVar12 + 0x10 + lVar5),lVar5 + 0x14 + lVar12,
                               lVar5 + 0x20 + lVar12,*(undefined1 *)(lVar12 + 0x2c + lVar5),
                               *(undefined1 *)(lVar12 + 0x2d + lVar5));
        if ((*(char *)(lVar12 + 0x2e + lVar5) == '\x01') &&
           (lVar5 = (**(code **)(*plVar6 + 0x10))(plVar6,0x19), lVar5 != 0)) {
          *(undefined8 *)(lVar5 + 0x40) = 0;
          *(undefined4 *)(lVar5 + 0x48) = 0;
        }
        if (iVar14 < *(int *)(param_1 + 0x600)) {
          FUN_1402b2fe0(param_1 + 0x5f0,iVar14,iVar14 + 1,(*(int *)(param_1 + 0x600) - iVar14) + -1)
          ;
          *(int *)(param_1 + 0x600) = *(int *)(param_1 + 0x600) + -1;
        }
      }
      lVar12 = lVar12 + -0x30;
      iVar14 = iVar14 + -1;
    } while (-1 < iVar14);
  }
  if (*(int *)(param_1 + 0x478) < 1) {
    plVar6 = plVar11;
    if (0 < *(int *)(param_1 + 0x490)) {
      puVar7 = *(undefined4 **)(param_1 + 0x488);
      goto LAB_1402be910;
    }
  }
  else {
    puVar7 = *(undefined4 **)(param_1 + 0x470);
LAB_1402be910:
    plVar6 = (longlong *)FUN_1402aca60(param_1,*puVar7);
  }
  iVar4 = *(int *)(param_1 + 0x618);
  if (iVar4 == 1) {
    if (((DAT_140b154d0 == '\0') ||
        (plVar8 = (longlong *)FUN_1402aca60(param_1,DAT_140b154dc), plVar8 == (longlong *)0x0)) &&
       (plVar8 = plVar6, plVar6 == (longlong *)0x0)) {
      *(undefined4 *)(param_1 + 0x618) = 3;
      FUN_14028c720(&DAT_140b153e0,0);
      FUN_14028c6c0(&DAT_140b153e0);
      if (DAT_140b154d0 != '\0') {
        *(undefined4 *)(param_1 + 0x618) = 0xffffffff;
        FUN_14028f590(&DAT_140b153e0);
      }
      iVar4 = *(int *)(param_1 + 0x618);
      goto LAB_1402be9f2;
    }
    lVar12 = (**(code **)(*(longlong *)plVar8[0x54] + 0x38))();
    local_48 = (undefined **)CONCAT44(*(undefined4 *)(lVar12 + 0x1c),*(undefined4 *)(lVar12 + 0xc));
    local_40 = (LPVOID)CONCAT44(local_40._4_4_,*(undefined4 *)(lVar12 + 0x2c));
    FUN_14044a1a0(&DAT_140b30350,&local_48);
    FUN_1403de570(&DAT_140b27f60,plVar8,0);
    *(undefined4 *)(param_1 + 0x618) = 2;
LAB_1402be9f7:
    if (DAT_140b303c0 == '\0') {
      *(undefined4 *)(param_1 + 0x618) = 3;
      FUN_14028c720(&DAT_140b153e0,0);
      FUN_14028c6c0(&DAT_140b153e0);
      if (DAT_140b154d0 != '\0') {
        *(undefined4 *)(param_1 + 0x618) = 0xffffffff;
        FUN_14028f590(&DAT_140b153e0);
      }
    }
  }
  else {
LAB_1402be9f2:
    if (iVar4 == 2) goto LAB_1402be9f7;
  }
  bVar1 = false;
  cVar3 = FUN_14028af60(&DAT_140b153e0);
  if ((cVar3 == '\0') &&
     ((DAT_140b154d0 != '\0' || ((int)(DAT_140b2b368 - DAT_140b2b360 >> 3) != 0)))) {
    iVar4 = FUN_140294a70(&DAT_140a16fd0);
    if (0 < *(int *)(param_1 + 0x478)) {
      piVar10 = (int *)(*(longlong *)(param_1 + 0x470) + 4);
      plVar6 = plVar11;
      do {
        if ((piVar10[1] == iVar4) || (*piVar10 == iVar4)) goto LAB_1402beae6;
        uVar13 = (int)plVar6 + 1;
        plVar6 = (longlong *)(ulonglong)uVar13;
        piVar10 = piVar10 + 0x16;
      } while ((int)uVar13 < *(int *)(param_1 + 0x478));
    }
    if (0 < *(int *)(param_1 + 0x490)) {
      piVar10 = (int *)(*(longlong *)(param_1 + 0x488) + 4);
      do {
        if ((piVar10[1] == iVar4) || (*piVar10 == iVar4)) goto LAB_1402beae6;
        uVar13 = (int)plVar11 + 1;
        plVar11 = (longlong *)(ulonglong)uVar13;
        piVar10 = piVar10 + 0x16;
      } while ((int)uVar13 < *(int *)(param_1 + 0x490));
    }
  }
LAB_1402beae9:
  local_28 = (LPVOID)0x0;
  local_30 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  local_20 = 0;
  local_40 = (LPVOID)0x0;
  local_38 = 0;
  cVar3 = FUN_1402ad100(param_1,&local_48,&local_30);
  if ((((cVar3 != '\0') &&
       ((DAT_140b15618 == (code *)0x0 || (cVar3 = (*DAT_140b15618)(), cVar3 == '\0')))) &&
      ((DAT_140b15640 == (code *)0x0 || (cVar3 = (*DAT_140b15640)(), cVar3 == '\0')))) &&
     ((((DAT_140b15650 == (code *)0x0 || (cVar3 = (*DAT_140b15650)(), cVar3 == '\0')) &&
       ((*(int *)(DAT_140b2eea0 + 0x44) != 1 ||
        (cVar3 = FUN_14028af60(&DAT_140b153e0), cVar3 != '\0')))) && (!bVar1)))) {
    FUN_1402b92e0(param_1,&local_48,&local_30);
  }
  local_48 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  if (local_40 != (LPVOID)0x0) {
    if ((local_38 & 0x8000000000000000) == 0) {
      thunk_FUN_1407864b8();
    }
    else {
      pvVar9 = GetProcessHeap();
      HeapFree(pvVar9,0,local_40);
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
      pvVar9 = GetProcessHeap();
      HeapFree(pvVar9,0,local_28);
    }
  }
  if (((*(int *)(param_1 + 0x490) != 0) &&
      ((DAT_140b155f8 == (code *)0x0 || (cVar3 = (*DAT_140b155f8)(), cVar3 == '\0')))) &&
     (((DAT_140b15640 == (code *)0x0 || (cVar3 = (*DAT_140b15640)(), cVar3 == '\0')) &&
      (((DAT_140b15650 == (code *)0x0 || (cVar3 = (*DAT_140b15650)(), cVar3 == '\0')) && (!bVar1))))
     )) {
    FUN_1402b76a0(param_1);
  }
  FUN_1402a62d0(param_1);
  return;
LAB_1402beae6:
  bVar1 = true;
  goto LAB_1402beae9;
}



