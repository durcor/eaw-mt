
// ================================================================
// thunk_FUN_1407bc310 — lua_resume wrapper
// ================================================================

// RVA: 0x7bc310  Name: FUN_1407bc310  Size: 344 bytes

int FUN_1407bc310(longlong param_1,int param_2,undefined8 param_3)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  char *pcVar5;
  undefined8 uVar6;
  int local_res10 [2];
  
  puVar2 = *(undefined8 **)(param_1 + 0x58);
  local_res10[0] = param_2;
  if (puVar2 == *(undefined8 **)(param_1 + 0x80)) {
    if ((longlong)param_2 < *(longlong *)(param_1 + 0x10) - *(longlong *)(param_1 + 0x18) >> 4) {
LAB_1407bc3d4:
      uVar1 = *(undefined1 *)(param_1 + 0x8d);
      uVar6 = *(undefined8 *)(param_1 + 200);
      *(undefined8 *)(param_1 + 200) = param_3;
      iVar4 = FUN_1407bc0d0(param_1,FUN_1407bc5e0,local_res10);
      if (iVar4 != 0) {
        *(undefined8 **)(param_1 + 0x58) = *(undefined8 **)(param_1 + 0x80);
        *(undefined8 *)(param_1 + 0x18) = **(undefined8 **)(param_1 + 0x80);
        *(undefined2 *)(param_1 + 0x8a) = 0;
        FUN_1407b9f90(param_1);
        FUN_1407bc690(param_1,iVar4,*(undefined8 *)(param_1 + 0x18));
        *(undefined1 *)(param_1 + 0x8d) = uVar1;
        FUN_1407bc580(param_1);
      }
      *(undefined8 *)(param_1 + 200) = uVar6;
      return iVar4;
    }
    uVar6 = 0x1c;
    pcVar5 = "cannot resume dead coroutine";
  }
  else {
    if ((*(byte *)(puVar2 + 2) & 0x10) != 0) goto LAB_1407bc3d4;
    uVar6 = 0x25;
    pcVar5 = "cannot resume non-suspended coroutine";
  }
  puVar3 = (undefined4 *)*puVar2;
  *(undefined4 **)(param_1 + 0x10) = puVar3;
  *puVar3 = 4;
  uVar6 = FUN_1407bf8f0(param_1,pcVar5,uVar6);
  *(undefined8 *)(puVar3 + 2) = uVar6;
  if (*(longlong *)(param_1 + 0x60) - *(longlong *)(param_1 + 0x10) < 0x11) {
    iVar4 = *(int *)(param_1 + 0x70);
    if (0 < iVar4) {
      FUN_1407bc220(param_1,iVar4 * 2);
      *(longlong *)(param_1 + 0x10) = *(longlong *)(param_1 + 0x10) + 0x10;
      return 1;
    }
    FUN_1407bc220(param_1,iVar4 + 6);
  }
  *(longlong *)(param_1 + 0x10) = *(longlong *)(param_1 + 0x10) + 0x10;
  return 1;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407bc220 (RVA 0x7bc220)
// ================================================================

// RVA: 0x7bc220  Name: FUN_1407bc220  Size: 232 bytes

void FUN_1407bc220(longlong param_1,int param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong *plVar4;
  longlong *plVar5;
  
  lVar1 = *(longlong *)(param_1 + 0x68);
  lVar3 = FUN_1407bf7e0(param_1,lVar1,*(int *)(param_1 + 0x70) << 4,param_2 << 4);
  *(longlong *)(param_1 + 0x68) = lVar3;
  *(int *)(param_1 + 0x70) = param_2;
  *(longlong *)(param_1 + 0x60) = ((longlong)param_2 + -6) * 0x10 + lVar3;
  *(ulonglong *)(param_1 + 0x10) =
       (*(longlong *)(param_1 + 0x10) - lVar1 & 0xfffffffffffffff0U) + lVar3;
  for (puVar2 = *(undefined8 **)(param_1 + 0xb0); puVar2 != (undefined8 *)0x0;
      puVar2 = (undefined8 *)*puVar2) {
    puVar2[2] = (puVar2[2] - lVar1 & 0xfffffffffffffff0U) + *(longlong *)(param_1 + 0x68);
  }
  plVar4 = *(longlong **)(param_1 + 0x80);
  plVar5 = *(longlong **)(param_1 + 0x58);
  if (plVar4 <= plVar5) {
    do {
      plVar4[1] = (plVar4[1] - lVar1 & 0xfffffffffffffff0U) + *(longlong *)(param_1 + 0x68);
      *plVar4 = (*plVar4 - lVar1 & 0xfffffffffffffff0U) + *(longlong *)(param_1 + 0x68);
      plVar4 = plVar4 + 6;
      plVar5 = *(longlong **)(param_1 + 0x58);
    } while (plVar4 <= plVar5);
  }
  *(longlong *)(param_1 + 0x18) = *plVar5;
  return;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407bc580 (RVA 0x7bc580)
// ================================================================

// RVA: 0x7bc580  Name: FUN_1407bc580  Size: 93 bytes

void FUN_1407bc580(longlong param_1)

{
  *(longlong *)(param_1 + 0x60) =
       (longlong)*(int *)(param_1 + 0x70) * 0x10 + *(longlong *)(param_1 + 0x68) + -0x10;
  if ((0x1000 < *(ushort *)(param_1 + 0x88)) &&
     ((int)((*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x80)) / 0x30) + 1 < 0x1000)) {
    FUN_1407bc180(param_1,0x1000);
    return;
  }
  return;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407b9f90 (RVA 0x7b9f90)
// ================================================================

// RVA: 0x7b9f90  Name: FUN_1407b9f90  Size: 109 bytes

void FUN_1407b9f90(longlong param_1,undefined4 *param_2)

{
  undefined8 *puVar1;
  undefined4 *puVar2;
  
  puVar1 = *(undefined8 **)(param_1 + 0xb0);
  while ((puVar1 != (undefined8 *)0x0 && (puVar2 = (undefined4 *)puVar1[2], param_2 <= puVar2))) {
    *(undefined4 *)(puVar1 + 3) = *puVar2;
    puVar1[4] = *(undefined8 *)(puVar2 + 2);
    puVar1[2] = puVar1 + 3;
    *(undefined8 *)(param_1 + 0xb0) = *puVar1;
    FUN_1407bca30(param_1,puVar1,CONCAT71((int7)((ulonglong)puVar2 >> 8),10));
    puVar1 = *(undefined8 **)(param_1 + 0xb0);
  }
  return;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407bc690 (RVA 0x7bc690)
// ================================================================

// RVA: 0x7bc690  Name: FUN_1407bc690  Size: 190 bytes

void FUN_1407bc690(longlong param_1,int param_2,undefined4 *param_3)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if ((param_2 == 1) || (param_2 == 3)) {
    lVar1 = *(longlong *)(param_1 + 0x10);
    *param_3 = *(undefined4 *)(lVar1 + -0x10);
    *(undefined8 *)(param_3 + 2) = *(undefined8 *)(lVar1 + -8);
    *(undefined4 **)(param_1 + 0x10) = param_3 + 4;
    return;
  }
  if (param_2 == 4) {
    *param_3 = 4;
    uVar2 = FUN_1407bf8f0(param_1,"not enough memory",0x11);
    *(undefined8 *)(param_3 + 2) = uVar2;
    *(undefined4 **)(param_1 + 0x10) = param_3 + 4;
    return;
  }
  if (param_2 == 5) {
    *param_3 = 4;
    uVar2 = FUN_1407bf8f0(param_1,"error in error handling",0x17);
    *(undefined8 *)(param_3 + 2) = uVar2;
    *(undefined4 **)(param_1 + 0x10) = param_3 + 4;
    return;
  }
  *(undefined4 **)(param_1 + 0x10) = param_3 + 4;
  return;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407bc0d0 (RVA 0x7bc0d0)
// ================================================================

// RVA: 0x7bc0d0  Name: FUN_1407bc0d0  Size: 161 bytes

undefined4 FUN_1407bc0d0(longlong param_1,code *param_2,undefined8 param_3)

{
  int iVar1;
  undefined1 auStack_148 [32];
  undefined8 local_128 [2];
  undefined1 local_118 [256];
  undefined4 local_18;
  
  local_18 = 0;
  local_128[0] = *(undefined8 *)(param_1 + 0xc0);
  *(undefined8 **)(param_1 + 0xc0) = local_128;
  iVar1 = FUN_1407e2370(local_118,auStack_148);
  if (iVar1 == 0) {
    (*param_2)(param_1,param_3);
    *(undefined8 *)(param_1 + 0xc0) = local_128[0];
    return local_18;
  }
  *(undefined8 *)(param_1 + 0xc0) = local_128[0];
  return local_18;
}


// ================================================================
// callee of 0x7bc310 → FUN_1407bf8f0 (RVA 0x7bf8f0)
// ================================================================

// RVA: 0x7bf8f0  Name: FUN_1407bf8f0  Size: 476 bytes

undefined8 * FUN_1407bf8f0(longlong param_1,void *param_2,ulonglong param_3)

{
  longlong *plVar1;
  longlong *plVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  longlong lVar6;
  longlong *plVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar10;
  int iVar11;
  ulonglong uVar12;
  longlong lVar13;
  
  uVar12 = (param_3 >> 5) + 1;
  uVar8 = param_3 & 0xffffffff;
  uVar9 = (uint)param_3;
  for (uVar10 = param_3; uVar12 <= uVar10; uVar10 = uVar10 - uVar12) {
    uVar9 = (uint)uVar8 ^
            (uint)*(byte *)((uVar10 - 1) + (longlong)param_2) + (uint)uVar8 * 0x20 +
            (int)(uVar8 >> 2);
    uVar8 = (ulonglong)uVar9;
  }
  puVar5 = *(undefined8 **)
            (**(longlong **)(param_1 + 0x20) +
            ((longlong)*(int *)((longlong)*(longlong **)(param_1 + 0x20) + 0xc) - 1U &
            (longlong)(int)uVar9) * 8);
  while( true ) {
    if (puVar5 == (undefined8 *)0x0) {
      puVar5 = (undefined8 *)FUN_1407bf7e0(param_1,0,0,(uint)param_3 + 0x19);
      puVar5[2] = param_3;
      *(uint *)((longlong)puVar5 + 0xc) = uVar9;
      *(undefined2 *)(puVar5 + 1) = 4;
      *(undefined1 *)((longlong)puVar5 + 10) = 0;
      FUN_14076bd80(puVar5 + 3,param_2,param_3);
      *(undefined1 *)((longlong)puVar5 + param_3 + 0x18) = 0;
      plVar1 = *(longlong **)(param_1 + 0x20);
      uVar8 = (ulonglong)(*(int *)((longlong)plVar1 + 0xc) - 1U & uVar9);
      *puVar5 = *(undefined8 *)(*plVar1 + uVar8 * 8);
      *(undefined8 **)(*plVar1 + uVar8 * 8) = puVar5;
      *(int *)(plVar1 + 1) = (int)plVar1[1] + 1;
      iVar4 = *(int *)((longlong)plVar1 + 0xc);
      if ((iVar4 < (int)plVar1[1]) && (iVar4 < 0x3fffffff)) {
        iVar3 = iVar4 * 2;
        lVar6 = FUN_1407bf7e0(param_1,0,0,iVar4 << 4);
        plVar1 = *(longlong **)(param_1 + 0x20);
        if (0 < iVar3) {
          FUN_14076c430(lVar6,0,(longlong)iVar3 << 3);
        }
        iVar4 = *(int *)((longlong)plVar1 + 0xc);
        iVar11 = 0;
        if (0 < iVar4) {
          lVar13 = 0;
          do {
            if (*(longlong **)(lVar13 + *plVar1) != (longlong *)0x0) {
              plVar7 = *(longlong **)(lVar13 + *plVar1);
              do {
                plVar2 = (longlong *)*plVar7;
                uVar8 = (longlong)*(int *)((longlong)plVar7 + 0xc) & (longlong)iVar3 - 1U;
                *plVar7 = *(longlong *)(lVar6 + uVar8 * 8);
                *(longlong **)(lVar6 + uVar8 * 8) = plVar7;
                plVar7 = plVar2;
              } while (plVar2 != (longlong *)0x0);
            }
            iVar4 = *(int *)((longlong)plVar1 + 0xc);
            iVar11 = iVar11 + 1;
            lVar13 = lVar13 + 8;
          } while (iVar11 < iVar4);
        }
        FUN_1407bf7e0(param_1,*plVar1,iVar4 * 8,0);
        *(int *)((longlong)plVar1 + 0xc) = iVar3;
        *plVar1 = lVar6;
      }
      return puVar5;
    }
    if ((puVar5[2] == param_3) && (iVar4 = memcmp(param_2,puVar5 + 3,param_3), iVar4 == 0)) break;
// ... [truncated at 80 / 84 lines]


// ================================================================
// FUN_140215b90 — frame profiler / QPC timer candidate
// ================================================================

// RVA: 0x215b90  Name: FUN_140215b90  Size: 474 bytes

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140215b90(float param_1)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined1 *puVar3;
  undefined1 local_4a8 [144];
  undefined1 local_418 [1040];
  
  if (DAT_140a7cc34 != '\0') {
    if (0.0 < DAT_140a7cc3c) {
      if (DAT_140a7cc38 < 1) {
        if (_DAT_140a15120 < param_1 / DAT_140a7cc3c) {
          DAT_140a7cc40 = DAT_140a7cc40 + 1;
          FUN_140048ec0(local_418,PTR_s__Slow_Frame_Profile__03d_xml_140a15128);
          FUN_140216150(0,"<Profile_Tree>\n");
          FUN_140216060(DAT_140a7cc28,1);
          FUN_140216150(0,"</Profile_Tree>\n");
          FUN_1402130d0(local_4a8,local_418);
          FUN_140213840(local_4a8,1);
          puVar3 = &DAT_140a15130;
          if (0xf < DAT_140a15148) {
            puVar3 = _DAT_140a15130;
          }
          FUN_140213a50(local_4a8,puVar3,_DAT_140a15140 & 0xffffffff);
          FUN_1402138d0(local_4a8);
          _DAT_140a15140 = 0;
          puVar3 = &DAT_140a15130;
          if (0xf < DAT_140a15148) {
            puVar3 = _DAT_140a15130;
          }
          *puVar3 = 0;
          FUN_1402132f0(local_4a8);
          DAT_140a7cc38 = 0x14;
        }
      }
      else {
        DAT_140a7cc38 = DAT_140a7cc38 + -1;
      }
    }
    puVar2 = DAT_140a7cc28;
    if (DAT_140a7cc34 != '\0') {
      for (; puVar2 != (undefined8 *)0x0; puVar2 = (undefined8 *)*puVar2) {
        puVar1 = (undefined8 *)puVar2[2];
        *(undefined4 *)(puVar2 + 4) = 0;
        puVar2[5] = 0;
        for (; puVar1 != (undefined8 *)0x0; puVar1 = (undefined8 *)*puVar1) {
          FUN_140215dd0(puVar1);
        }
      }
    }
    DAT_140a7cc3c = param_1;
    return;
  }
  DAT_140a7cc3c = param_1;
  return;
}


// ================================================================
// FUN_140247700 — per-coroutine setup (Pump_Threads slot 0)
// ================================================================

// RVA: 0x247700  Name: FUN_140247700  Size: 622 bytes

void FUN_140247700(undefined8 param_1,longlong *param_2)

{
  undefined8 *puVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  longlong lVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  longlong *plVar11;
  longlong lVar12;
  
  uVar6 = (**(code **)(*param_2 + 0x28))(param_2);
  switch(uVar6) {
  case 1:
    FUN_1407b9140(param_1);
    puVar9 = (undefined8 *)param_2[2];
    puVar1 = (undefined8 *)*puVar9;
    while (puVar1 != puVar9) {
      plVar11 = (longlong *)puVar1[4];
      if (plVar11 != (longlong *)0x0) {
        (**(code **)*plVar11)(plVar11);
      }
      plVar2 = (longlong *)puVar1[5];
      if (plVar2 != (longlong *)0x0) {
        (**(code **)*plVar2)(plVar2);
      }
      FUN_140247700(param_1,plVar11);
      FUN_140247700(param_1,plVar2);
      FUN_1407b9a60(param_1,0xfffffffd);
      if (plVar2 != (longlong *)0x0) {
        (**(code **)(*plVar2 + 8))(plVar2);
      }
      if (plVar11 != (longlong *)0x0) {
        (**(code **)(*plVar11 + 8))(plVar11);
      }
      puVar3 = (undefined8 *)puVar1[2];
      if (*(char *)((longlong)puVar3 + 0x19) == '\0') {
        cVar5 = *(char *)((longlong)*puVar3 + 0x19);
        puVar1 = puVar3;
        puVar3 = (undefined8 *)*puVar3;
        while (cVar5 == '\0') {
          cVar5 = *(char *)((longlong)*puVar3 + 0x19);
          puVar1 = puVar3;
          puVar3 = (undefined8 *)*puVar3;
        }
      }
      else {
        cVar5 = *(char *)((longlong)puVar1[1] + 0x19);
        puVar4 = (undefined8 *)puVar1[1];
        puVar3 = puVar1;
        while ((puVar1 = puVar4, cVar5 == '\0' && (puVar3 == (undefined8 *)puVar1[2]))) {
          cVar5 = *(char *)((longlong)puVar1[1] + 0x19);
          puVar4 = (undefined8 *)puVar1[1];
          puVar3 = puVar1;
        }
      }
    }
    break;
  case 2:
    FUN_140247310(param_1,param_2);
    break;
  case 4:
    FUN_1407b9520(param_1,param_2[2]);
    break;
  case 5:
    FUN_1407b9320(param_1,(char)param_2[2]);
    break;
  case 6:
    plVar11 = param_2 + 2;
    if (0xf < (ulonglong)param_2[5]) {
      plVar11 = (longlong *)*plVar11;
    }
    FUN_1407b94a0(param_1,plVar11,param_2[4]);
    break;
  case 8:
    FUN_1407b9460(param_1,param_2[2]);
    break;
  case 9:
    lVar12 = param_2[3];
    lVar8 = FUN_1407ce3b0(param_1);
    if ((lVar8 != 0) && (lVar8 = *(longlong *)(lVar8 + 0x38), lVar8 != 0)) {
      if (((lVar12 == 0) || (iVar7 = FUN_1407b8920(*(undefined8 *)(lVar12 + 8),param_1), iVar7 != 0)
          ) && (cVar5 = (**(code **)(*param_2 + 0x68))(param_2,lVar8), cVar5 != '\0')) {
LAB_1402478f5:
        if (lVar12 != 0) {
          FUN_1407b9440(param_1,lVar12);
          return;
        }
        puVar9 = (undefined8 *)FUN_1407b91d0(param_1,0x10);
        if (puVar9 == (undefined8 *)0x0) {
          FUN_1407b9ab0(param_1,0xfffffffd);
          return;
        }
        param_2[3] = (longlong)puVar9;
// ... [truncated at 100 / 123 lines]


// ================================================================
// FUN_140248d10 — post-resume cleanup (reinit on coroutine finish)
// ================================================================

// RVA: 0x248d10  Name: FUN_140248d10  Size: 131 bytes

void FUN_140248d10(longlong param_1,undefined8 param_2)

{
  int iVar1;
  
  FUN_1407b94a0(*(undefined8 *)(param_1 + 0x58),"LuaThreadTable",0xe);
  FUN_1407b8e90(*(undefined8 *)(param_1 + 0x58),0xffffd8ef);
  iVar1 = FUN_1407b9e00(*(undefined8 *)(param_1 + 0x58),0xffffffff);
  if (iVar1 == 5) {
    FUN_1407b95e0(*(undefined8 *)(param_1 + 0x58),param_2);
    FUN_1407b9510(*(undefined8 *)(param_1 + 0x58));
    FUN_1407b9a60(*(undefined8 *)(param_1 + 0x58),0xfffffffd);
    FUN_1407b9ab0(*(undefined8 *)(param_1 + 0x58),0xfffffffe);
  }
  return;
}


// ================================================================
// FUN_140246940 — called on lua_resume error path
// ================================================================

// RVA: 0x246940  Name: FUN_140246940  Size: 395 bytes

undefined8 FUN_140246940(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 ***pppuVar3;
  longlong lVar4;
  undefined8 ****ppppuVar5;
  ulonglong uVar6;
  longlong lVar7;
  int iVar8;
  undefined8 ***local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  if (DAT_140b09d40 != (code *)0x0) {
    iVar1 = FUN_1407b8ef0();
    iVar8 = 1;
    if (0 < iVar1) {
      lVar7 = 0;
      do {
        lVar2 = FUN_1407b9cc0(param_1,iVar8);
        if (lVar2 != 0) {
          FUN_140252be0(param_1,0);
          local_28[0] = (undefined8 ****)0x0;
          local_18 = 0;
          local_10 = 0xf;
          pppuVar3 = (undefined8 ***)FUN_140769c58(0x2037);
          if (pppuVar3 == (undefined8 ***)0x0) {
LAB_140246ac6:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
          ppppuVar5 = (undefined8 ****)((longlong)pppuVar3 + 0x27U & 0xffffffffffffffe0);
          ppppuVar5[-1] = pppuVar3;
          local_10 = 0x200f;
          *(undefined1 *)ppppuVar5 = local_28[0]._0_1_;
          local_18 = 0;
          local_28[0] = ppppuVar5;
          FUN_140200cd0(local_28,"LuaScriptClass::ERROR -- %s",lVar2);
          ppppuVar5 = local_28;
          if (0xf < local_10) {
            ppppuVar5 = (undefined8 ****)local_28[0];
          }
          (*DAT_140b09d40)(ppppuVar5);
          if (0xf < local_10) {
            uVar6 = local_10 + 1;
            ppppuVar5 = (undefined8 ****)local_28[0];
            if (0xfff < uVar6) {
              ppppuVar5 = (undefined8 ****)local_28[0][-1];
              uVar6 = local_10 + 0x28;
              if (0x1f < (ulonglong)((longlong)local_28[0] + (-8 - (longlong)ppppuVar5)))
              goto LAB_140246ac6;
            }
            thunk_FUN_1407864b8(ppppuVar5,uVar6);
          }
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 <= iVar1);
// ... [truncated at 60 / 76 lines]


// ================================================================
// Callers of FUN_140215b90
// ================================================================

// Caller: RVA 0x5d990  FUN_14005d990  7440 bytes
