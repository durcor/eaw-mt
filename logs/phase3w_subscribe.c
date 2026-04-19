
// ================================================================
// FUN_1402215b0 — main signal subscriber (only real caller of FUN_1402406c0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2215b0  Name: FUN_1402215b0  Size: 602 bytes
// ------------------------------------------------------------

void FUN_1402215b0(longlong param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong lVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  uint uVar10;
  uint local_res8;
  longlong local_res10;
  undefined **local_res18;
  
  lVar4 = *(longlong *)(param_1 + 0x48);
  uVar8 = 0;
  local_res8 = 0;
  if (0 < (int)(*(longlong *)(param_1 + 0x50) - lVar4 >> 3)) {
    local_res10 = 0;
    uVar9 = uVar8;
    do {
      uVar10 = (uint)uVar8;
      if ((*(longlong *)(lVar4 + uVar9) != 0) &&
         (plVar1 = *(longlong **)(local_res10 + 8 + *(longlong *)(param_1 + 0x60)),
         plVar5 = (longlong *)*plVar1, plVar5 != plVar1)) {
        do {
          uVar7 = 0;
          lVar4 = plVar5[3];
          uVar8 = uVar7;
          if (0 < (int)(plVar5[4] - lVar4 >> 3)) {
            do {
              lVar6 = *(longlong *)(param_1 + 0x48);
              if (DAT_140a10b50 == (undefined8 *)0x0) {
                puVar2 = (undefined8 *)FUN_140769c58(0x78);
                if (puVar2 == (undefined8 *)0x0) {
                  DAT_140a10b50 = (undefined8 *)0x0;
                }
                else {
                  *puVar2 = SignalDispatcherClass::vftable;
                  *(undefined1 *)(puVar2 + 1) = 0;
                  *(undefined4 *)((longlong)puVar2 + 0xc) = 0;
                  puVar2[2] = 0;
                  puVar2[3] = 0;
                  lVar3 = FUN_140769c58(0x20);
                  *(longlong *)lVar3 = lVar3;
                  *(longlong *)(lVar3 + 8) = lVar3;
                  puVar2[2] = lVar3;
                  puVar2[4] = 0;
                  puVar2[5] = 0;
                  puVar2[6] = 0;
                  puVar2[7] = 7;
                  puVar2[8] = 8;
                  *(undefined4 *)((longlong)puVar2 + 0xc) = 0x3f800000;
                  FUN_140048250(puVar2 + 4,0x10,puVar2[2]);
                  puVar2[9] = 0;
                  puVar2[10] = 0;
                  puVar2[0xb] = 0;
                  puVar2[0xc] = 0;
                  puVar2[0xd] = 0;
                  puVar2[0xe] = 0;
                  DAT_140a10b50 = puVar2;
                }
                local_res18 = &PTR_vftable_140a10b48;
                if (DAT_140a7bc10 == DAT_140a7bc18) {
                  FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res18);
                }
                else {
                  *DAT_140a7bc10 = &PTR_vftable_140a10b48;
                  DAT_140a7bc10 = DAT_140a7bc10 + 1;
                }
              }
              lVar6 = *(longlong *)(uVar9 + lVar6);
              if ((lVar6 != 0) && (*(longlong *)(lVar4 + uVar7) != 0)) {
                FUN_1402406c0(lVar6,(int)plVar5[2]);
              }
              lVar4 = plVar5[3];
              uVar10 = (int)uVar8 + 1;
              uVar7 = uVar7 + 8;
              uVar8 = (ulonglong)uVar10;
            } while ((int)uVar10 < (int)(plVar5[4] - lVar4 >> 3));
          }
          plVar5 = (longlong *)*plVar5;
          uVar10 = local_res8;
        } while (plVar5 != (longlong *)
                           *(longlong *)(local_res10 + 8 + *(longlong *)(param_1 + 0x60)));
      }
      lVar4 = *(longlong *)(param_1 + 0x48);
      local_res8 = uVar10 + 1;
      uVar8 = (ulonglong)local_res8;
      local_res10 = local_res10 + 0x40;
      uVar9 = uVar9 + 8;
    } while ((int)local_res8 < (int)(*(longlong *)(param_1 + 0x50) - lVar4 >> 3));
  }
  lVar4 = *(longlong *)(param_1 + 0x68);
  lVar6 = *(longlong *)(param_1 + 0x60);
  if (lVar6 != lVar4) {
    do {
      FUN_1402223f0(lVar6);
      lVar6 = lVar6 + 0x40;
    } while (lVar6 != lVar4);
    lVar6 = *(longlong *)(param_1 + 0x60);
  }
  *(longlong *)(param_1 + 0x68) = lVar6;
  *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)(param_1 + 0x48);
  return;
}




// ================================================================
// FUN_14020aa60 — connect listener to signal list
// ================================================================

// ------------------------------------------------------------
// RVA: 0x20aa60  Name: FUN_14020aa60  Size: 167 bytes
// ------------------------------------------------------------

ulonglong FUN_14020aa60(longlong param_1,longlong param_2,char param_3)

{
  longlong *plVar1;
  ulonglong uVar2;
  longlong lVar3;
  longlong *plVar4;
  
  if (param_3 != '\0') {
    for (uVar2 = *(ulonglong *)(param_2 + 8); uVar2 != 0; uVar2 = *(ulonglong *)(uVar2 + 0x10)) {
      if (*(longlong *)(uVar2 + 0x20) == param_1) {
        return uVar2 & 0xffffffffffffff00;
      }
    }
  }
  plVar4 = (longlong *)FUN_140769c58(0x28);
  if (plVar4 != (longlong *)0x0) {
    plVar4[2] = 0;
    plVar4[1] = 0;
    *plVar4 = 0;
    plVar4[4] = 0;
  }
  plVar4[3] = param_2;
  plVar1 = (longlong *)(param_1 + 8);
  plVar4[2] = *(longlong *)(param_2 + 8);
  *(longlong **)(param_2 + 8) = plVar4;
  *plVar4 = *plVar1;
  plVar4[1] = (longlong)plVar1;
  *plVar1 = (longlong)plVar4;
  lVar3 = *plVar4;
  *(longlong **)(lVar3 + 8) = plVar4;
  plVar4[4] = param_1;
  *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + 1;
  *(undefined4 *)(param_1 + 0x38) = 0xfffffc19;
  return CONCAT71((int7)((ulonglong)lVar3 >> 8),1);
}




// ================================================================
// FUN_140221410 — called from FUN_1402406c0 on first subscribe for signal source
// ================================================================

// ------------------------------------------------------------
// RVA: 0x221410  Name: FUN_140221410  Size: 41 bytes
// ------------------------------------------------------------

void FUN_140221410(longlong param_1,undefined8 param_2)

{
  undefined8 local_28;
  undefined4 local_20;
  undefined1 local_18 [24];
  
  local_20 = 0xffffffff;
  local_28 = param_2;
  FUN_1402220f0(param_1 + 8,local_18,&local_28);
  return;
}




// ================================================================
// FUN_140240f50 — lookup signal slot (called in FUN_1402406c0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x240f50  Name: FUN_140240f50  Size: 768 bytes
// ------------------------------------------------------------

longlong * FUN_140240f50(longlong param_1,longlong *param_2,uint *param_3)

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
      goto LAB_140240ffe;
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
LAB_140240ffe:
  if (*(longlong *)(param_1 + 0x10) == 0x7ffffffffffffff) {
                    /* WARNING: Subroutine does not return */
    FUN_1407408dc("unordered_map/set too long");
  }
  puVar5 = (undefined8 *)FUN_140769c58(0x20);
  *(uint *)(puVar5 + 2) = *param_3;
  lVar8 = *(longlong *)(param_3 + 2);
  puVar5[3] = lVar8;
  if (lVar8 != 0) {
    (*(code *)**(undefined8 **)(lVar8 + 0x48))();
  }
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
        goto LAB_140241199;
        local_28 = (longlong *)local_28[1];
        iVar4 = (int)local_28[2];
      }
      local_28 = (longlong *)*local_28;
    }
LAB_140241199:
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
    if (*(longlong **)(lVar8 + 8 + uVar12 * 0x10) != plVar2) goto LAB_140241216;
  }
  *(undefined8 **)(lVar8 + 8 + uVar12 * 0x10) = puVar5;
LAB_140241216:
  *param_2 = (longlong)puVar5;
  *(undefined1 *)(param_2 + 1) = 1;
  return param_2;
}




// ================================================================
// CALLERS OF FUN_1402215b0 (signal subscription setup)
// ================================================================



// ================================================================
// CALLERS OF FUN_14020aa60 (connect listener to signal) — sample
// ================================================================

// Caller: RVA 0x18ada0  Name: FUN_14018ada0  Size: 611 bytes
// Caller: RVA 0x189fa0  Name: FUN_140189fa0  Size: 360 bytes
// Caller: RVA 0x19dca0  Name: FUN_14019dca0  Size: 61 bytes
// Caller: RVA 0x4fa540  Name: FUN_1404fa540  Size: 388 bytes
// Caller: RVA 0x195430  Name: FUN_140195430  Size: 87 bytes
// Caller: RVA 0x2fb150  Name: FUN_1402fb150  Size: 2839 bytes
// Caller: RVA 0x53cd60  Name: FUN_14053cd60  Size: 1984 bytes
// Caller: RVA 0x4f97d0  Name: FUN_1404f97d0  Size: 207 bytes
// Caller: RVA 0x524ee0  Name: FUN_140524ee0  Size: 169 bytes
// Caller: RVA 0x5262d0  Name: FUN_1405262d0  Size: 409 bytes
// Caller: RVA 0x53b380  Name: FUN_14053b380  Size: 203 bytes
// Caller: RVA 0x6477f0  Name: FUN_1406477f0  Size: 207 bytes
// Caller: RVA 0x725910  Name: FUN_140725910  Size: 43 bytes
// Caller: RVA 0x725940  Name: FUN_140725940  Size: 372 bytes
// Caller: RVA 0x2406c0  Name: FUN_1402406c0  Size: 505 bytes
// Caller: RVA 0x149490  Name: FUN_140149490  Size: 26 bytes

