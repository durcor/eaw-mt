/* FUN_140241df0  RVA 0x241df0  size 753  params 0  ret undefined */


longlong * FUN_140241df0(longlong param_1,longlong *param_2,uint *param_3)

{
  uint uVar1;
  longlong *plVar2;
  longlong *plVar3;
  int iVar4;
  undefined8 *puVar5;
  int iVar6;
  ulonglong uVar7;
  longlong lVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  uint uVar11;
  ulonglong uVar12;
  float fVar13;
  longlong *local_28;
  
  uVar1 = *param_3;
  uVar11 = (uVar1 ^ 0xdeadbeef) & 0x7fffffff;
  iVar6 = (uVar11 % 0x1f31d) * 0x41a7 + (uVar11 / 0x1f31d) * -0xb14;
  iVar4 = iVar6 + 0x7fffffff;
  if (-1 < iVar6) {
    iVar4 = iVar6;
  }
  uVar12 = (ulonglong)iVar4;
  uVar7 = uVar12 & *(ulonglong *)(param_1 + 0x30);
  plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x18) + 8 + uVar7 * 0x10);
  local_28 = *(longlong **)(param_1 + 8);
  if (plVar2 != *(longlong **)(param_1 + 8)) {
    iVar4 = (int)plVar2[2];
    while ((int)uVar1 < iVar4) {
      local_28 = plVar2;
      if (plVar2 == *(longlong **)(*(longlong *)(param_1 + 0x18) + uVar7 * 0x10))
      goto LAB_140241e9e;
      plVar2 = (longlong *)plVar2[1];
      iVar4 = (int)plVar2[2];
    }
    local_28 = (longlong *)*plVar2;
    if ((int)uVar1 <= (int)plVar2[2]) {
      *param_2 = (longlong)plVar2;
      *(undefined1 *)(param_2 + 1) = 0;
      return param_2;
    }
  }
LAB_140241e9e:
  if (*(longlong *)(param_1 + 0x10) == 0x7ffffffffffffff) {
                    /* WARNING: Subroutine does not return */
    FUN_1407408dc("unordered_map/set too long");
  }
  puVar5 = (undefined8 *)FUN_140769c58(0x20);
  *(uint *)(puVar5 + 2) = *param_3;
  puVar5[3] = *(undefined8 *)(param_3 + 2);
  lVar8 = *(longlong *)(param_1 + 0x10);
  uVar7 = *(ulonglong *)(param_1 + 0x38);
  fVar13 = (float)(lVar8 + 1U) / (float)uVar7;
  if (*(float *)(param_1 + 4) <= fVar13 && fVar13 != *(float *)(param_1 + 4)) {
    fVar13 = ceilf((float)(lVar8 + 1U) / *(float *)(param_1 + 4));
    lVar8 = 0;
    if ((DAT_140800808 <= fVar13) && (fVar13 = fVar13 - DAT_140800808, fVar13 < DAT_140800808)) {
      lVar8 = -0x8000000000000000;
    }
    uVar9 = 8;
    if (8 < (ulonglong)((longlong)fVar13 + lVar8)) {
      uVar9 = (longlong)fVar13 + lVar8;
    }
    uVar10 = uVar7;
    if ((uVar7 < uVar9) && ((0x1ff < uVar7 || (uVar10 = uVar7 * 8, uVar7 * 8 < uVar9)))) {
      uVar10 = uVar9;
    }
    FUN_140048860(param_1,uVar10);
    uVar7 = uVar12 & *(ulonglong *)(param_1 + 0x30);
    local_28 = *(longlong **)(param_1 + 8);
    plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x18) + 8 + uVar7 * 0x10);
    if (plVar2 != local_28) {
      iVar4 = (int)plVar2[2];
      local_28 = plVar2;
      while (*(int *)(puVar5 + 2) < iVar4) {
        if (local_28 == *(longlong **)(*(longlong *)(param_1 + 0x18) + uVar7 * 0x10))
        goto LAB_14024202a;
        local_28 = (longlong *)local_28[1];
        iVar4 = (int)local_28[2];
      }
      local_28 = (longlong *)*local_28;
    }
LAB_14024202a:
    lVar8 = *(longlong *)(param_1 + 0x10);
  }
  plVar2 = (longlong *)local_28[1];
  *(longlong *)(param_1 + 0x10) = lVar8 + 1;
  *puVar5 = local_28;
  puVar5[1] = plVar2;
  *plVar2 = (longlong)puVar5;
  local_28[1] = (longlong)puVar5;
  uVar12 = uVar12 & *(ulonglong *)(param_1 + 0x30);
  lVar8 = *(longlong *)(param_1 + 0x18);
  plVar3 = *(longlong **)(lVar8 + uVar12 * 0x10);
  if (plVar3 == *(longlong **)(param_1 + 8)) {
    *(undefined8 **)(lVar8 + uVar12 * 0x10) = puVar5;
  }
  else {
    if (plVar3 == local_28) {
      *(undefined8 **)(lVar8 + uVar12 * 0x10) = puVar5;
      *param_2 = (longlong)puVar5;
      *(undefined1 *)(param_2 + 1) = 1;
      return param_2;
    }
    if (*(longlong **)(lVar8 + 8 + uVar12 * 0x10) != plVar2) goto LAB_1402420a7;
  }
  *(undefined8 **)(lVar8 + 8 + uVar12 * 0x10) = puVar5;
LAB_1402420a7:
  *param_2 = (longlong)puVar5;
  *(undefined1 *)(param_2 + 1) = 1;
  return param_2;
}


