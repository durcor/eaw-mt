/* FUN_140385190  RVA 0x385190  size 1014  params 0  ret undefined */


longlong * FUN_140385190(longlong param_1,longlong param_2,float *param_3)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float local_res18 [4];
  undefined1 local_98 [16];
  float local_88;
  float local_84;
  float local_80;
  float local_7c;
  float local_78;
  float local_74;
  
  if ((*param_3 == DAT_140899780) || (param_2 == 0)) {
    return (longlong *)0x0;
  }
  lVar3 = FUN_1402acb60(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2b8),
                        *(undefined4 *)(param_2 + 0x4c));
  fVar8 = (float)FUN_1403857d0(param_1);
  fVar8 = fVar8 * DAT_140819b38;
  lVar1 = *(longlong *)(param_1 + 0x10);
  fVar9 = *(float *)(lVar1 + 0x78) - fVar8;
  fVar12 = *(float *)(lVar1 + 0x78) + fVar8;
  fVar11 = *(float *)(lVar1 + 0x80) - fVar8;
  fVar10 = *(float *)(lVar1 + 0x7c) - fVar8;
  fVar13 = *(float *)(lVar1 + 0x7c) + fVar8;
  fVar8 = *(float *)(lVar1 + 0x80) + fVar8;
  local_88 = (fVar9 + fVar12) * DAT_1408007c0;
  local_res18[0] = fVar8 - fVar11;
  local_80 = (fVar11 + fVar8) * DAT_1408007c0;
  local_84 = (fVar10 + fVar13) * DAT_1408007c0;
  local_7c = ABS(fVar12 - fVar9) * DAT_1408007c0;
  local_78 = ABS(fVar13 - fVar10) * DAT_1408007c0;
  local_74 = ABS(local_res18[0]) * DAT_1408007c0;
  FUN_14020e780(lVar3,&local_88);
  lVar1 = *(longlong *)(lVar3 + 8);
  plVar7 = (longlong *)0x0;
  do {
    do {
      if (lVar1 == 0) {
        return plVar7;
      }
      plVar6 = (longlong *)(*(longlong *)(lVar1 + 8) + -0x28);
      if (*(longlong *)(lVar1 + 8) == 0) {
        plVar6 = (longlong *)0x0;
      }
    } while (plVar6 == (longlong *)0x0);
    if ((((((plVar6[0x54] != 0) && (*(char *)(plVar6[0x53] + 0x6b) != '\0')) &&
          (*(char *)((longlong)plVar6 + 0x3a7) != '\x01')) &&
         (((*(byte *)(plVar6 + 0x74) & 0x40) == 0 &&
          ((cVar2 = FUN_14039b140(plVar6), cVar2 == '\0' ||
           (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298) + 0xa4) != '\0')))))) &&
        ((lVar3 = (**(code **)(*plVar6 + 0x10))(plVar6,0x11), lVar3 == 0 &&
         (((*(char *)(*(longlong *)(param_1 + 0x10) + 0x3b4) != '\x01' &&
           (*(char *)((longlong)plVar6 + 0x3b4) != '\x01')) ||
          (cVar2 = FUN_14039a540(*(longlong *)(param_1 + 0x10),plVar6), cVar2 != '\0')))))) &&
       ((((cVar2 = FUN_140397060(plVar6), cVar2 == '\0' &&
          (cVar2 = FUN_14035f470(DAT_140b15418,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58)
                                 ,plVar6,1), cVar2 != '\x01')) &&
         ((*(char *)(*(longlong *)(param_1 + 0x20) + 0x4e) != '\x01' ||
          (cVar2 = FUN_140383ba0(param_1,plVar6 + 0xf), cVar2 != '\0')))) &&
        ((*(char *)(plVar6[0x53] + 0x41) == '\x01' &&
         (cVar2 = FUN_140540140(*(undefined8 *)(param_1 + 0x20),plVar6), cVar2 == '\0')))))) {
      uVar4 = FUN_1403973b0(plVar6);
      plVar5 = *(longlong **)(param_1 + 0x10);
      fVar8 = DAT_140899780;
      if ((*(char *)((longlong)plVar5 + 0x33b) != -1) &&
         (plVar5 = (longlong *)(**(code **)(*plVar5 + 0x10))(plVar5,9), fVar8 = DAT_140899780,
         plVar5 != (longlong *)0x0)) {
        fVar8 = (float)(**(code **)(*plVar5 + 0x118))(plVar5,uVar4);
      }
      if (fVar8 != DAT_140899788) {
        local_res18[0] = (float)((uint)local_res18[0] & 0xffffff00);
        FUN_140383f70(param_1,local_98,plVar6,local_res18);
        if (((local_res18[0]._0_1_ == '\x01') &&
            (((fVar8 < *param_3 || (*param_3 == DAT_140899788)) &&
             ((*(byte *)((longlong)plVar6 + 0x3a1) & 0x10) == 0)))) &&
           (*param_3 = fVar8, plVar7 = plVar6, fVar8 == DAT_140899780)) {
          return plVar6;
        }
      }
    }
    lVar1 = *(longlong *)(lVar1 + 0x18);
  } while( true );
}


