
// ================================================================
// DATA REFERENCES TO FUN_1402215b0 (vtable location)
// ================================================================

// DataRef at RVA 0xb5f670  Type: DATA
//   [-8] RVA 0xb5f630 (unreadable)
//   [-7] RVA 0xb5f638 (unreadable)
//   [-6] RVA 0xb5f640 (unreadable)
//   [-5] RVA 0xb5f648 (unreadable)
//   [-4] RVA 0xb5f650 -> fn 0xa1421ff885c9600 ?
//   [-3] RVA 0xb5f658 -> fn 0x101421fef9142200 ?
//   [-2] RVA 0xb5f660 (unreadable)
//   [-1] RVA 0xb5f668 (unreadable)
//   [0] RVA 0xb5f670 (unreadable)
//   [1] RVA 0xb5f678 (unreadable)
//   [2] RVA 0xb5f680 (unreadable)
//   [3] RVA 0xb5f688 (unreadable)
//   [4] RVA 0xb5f690 -> fn 0x245d95fee0182200 ?
//   [5] RVA 0xb5f698 (unreadable)
//   [6] RVA 0xb5f6a0 (unreadable)
//   [7] RVA 0xb5f6a8 -> fn 0x405d95ff00192200 ?
//   [8] RVA 0xb5f6b0 -> fn 0x5b1921ff68249400 ?

// DataRef at RVA 0x965d18  Type: DATA
//   [-8] RVA 0x965cd8 (unreadable)
//   [-7] RVA 0x965ce0 -> fn 0x10400fec4820000 ?
//   [-6] RVA 0x965ce8 -> fn 0x10a03feca340600 ?
//   [-5] RVA 0x965cf0 -> fn 0xa32065ec1090400 ?
//   [-4] RVA 0x965cf8 -> fn 0x97205cec3700230 ?
//   [-3] RVA 0x965d00 -> fn 0x211f09fedff40400 ?
//   [-2] RVA 0x965d08 -> fn 0x1ae404fed2c40600 ?
//   [-1] RVA 0x965d10 -> fn 0xd6406fec5540f00 ?
//   [0] RVA 0x965d18 (unreadable)
//   [1] RVA 0x965d20 (unreadable)
//   [2] RVA 0x965d28 (unreadable)
//   [3] RVA 0x965d30 (unreadable)
//   [4] RVA 0x965d38 -> fn 0xa3405feca320670 ?
//   [5] RVA 0x965d40 -> fn 0x10601fec6320230 ?
//   [6] RVA 0x965d48 -> fn 0x210501fec5640600 ?
//   [7] RVA 0x965d50 -> fn 0x702421ff45242200 ?
//   [8] RVA 0x965d58 -> fn 0x741993fee1000000 ?

// DataRef at RVA 0x965d28  Type: DATA
//   [-8] RVA 0x965ce8 -> fn 0x10a03feca340600 ?
//   [-7] RVA 0x965cf0 -> fn 0xa32065ec1090400 ?
//   [-6] RVA 0x965cf8 -> fn 0x97205cec3700230 ?
//   [-5] RVA 0x965d00 -> fn 0x211f09fedff40400 ?
//   [-4] RVA 0x965d08 -> fn 0x1ae404fed2c40600 ?
//   [-3] RVA 0x965d10 -> fn 0xd6406fec5540f00 ?
//   [-2] RVA 0x965d18 (unreadable)
//   [-1] RVA 0x965d20 (unreadable)
//   [0] RVA 0x965d28 (unreadable)
//   [1] RVA 0x965d30 (unreadable)
//   [2] RVA 0x965d38 -> fn 0xa3405feca320670 ?
//   [3] RVA 0x965d40 -> fn 0x10601fec6320230 ?
//   [4] RVA 0x965d48 -> fn 0x210501fec5640600 ?
//   [5] RVA 0x965d50 -> fn 0x702421ff45242200 ?
//   [6] RVA 0x965d58 -> fn 0x741993fee1000000 ?
//   [7] RVA 0x965d60 -> fn 0x702421ff45242200 ?
//   [8] RVA 0x965d68 -> fn 0x741993fee10e0400 ?


// ================================================================
// FUN_14019dca0 — 61b caller of FUN_14020aa60 (tiny)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x19dca0  Name: FUN_14019dca0  Size: 61 bytes
// ------------------------------------------------------------

void FUN_14019dca0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  undefined8 *puVar2;
  
  puVar2 = param_2 + 2;
  if (param_2 == (undefined8 *)0x0) {
    puVar2 = (undefined8 *)0x0;
  }
  cVar1 = FUN_14020aa60(param_1 + 0x1360,puVar2,1);
  if (cVar1 == '\x01') {
                    /* WARNING: Could not recover jumptable at 0x00014019dcd4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)*param_2)(param_2);
    return;
  }
  return;
}




// ================================================================
// FUN_140195430 — 87b caller of FUN_14020aa60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x195430  Name: FUN_140195430  Size: 87 bytes
// ------------------------------------------------------------

undefined8 * FUN_140195430(undefined8 *param_1,undefined4 param_2)

{
  char cVar1;
  
  param_1[3] = 0;
  *(undefined4 *)(param_1 + 1) = 0;
  *param_1 = alDrawPrimitive::vftable;
  param_1[2] = alDrawPrimitive::vftable;
  *(undefined4 *)(param_1 + 4) = param_2;
  *(undefined1 *)((longlong)param_1 + 0x24) = 0;
  cVar1 = FUN_14020aa60(&PTR_vftable_140a12ae0,param_1 + 2,1);
  if (cVar1 == '\x01') {
    (**(code **)*param_1)(param_1);
  }
  return param_1;
}




// ================================================================
// FUN_14053b380 — 203b caller of FUN_14020aa60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x53b380  Name: FUN_14053b380  Size: 203 bytes
// ------------------------------------------------------------

void FUN_14053b380(longlong *param_1)

{
  longlong lVar1;
  ulonglong uVar2;
  int iVar3;
  longlong lVar4;
  
  lVar1 = *param_1;
  iVar3 = 0;
  if (0 < (int)(param_1[1] - lVar1 >> 3)) {
    lVar4 = 0;
    do {
      if (*(longlong *)(lVar4 + lVar1) != 0) {
        FUN_14020aa60(param_1[3],*(longlong *)(lVar4 + lVar1),1);
        lVar1 = *param_1;
      }
      iVar3 = iVar3 + 1;
      lVar4 = lVar4 + 8;
    } while (iVar3 < (int)(param_1[1] - lVar1 >> 3));
  }
  if (lVar1 != 0) {
    uVar2 = param_1[2] - lVar1 & 0xfffffffffffffff8;
    lVar4 = lVar1;
    if (0xfff < uVar2) {
      lVar4 = *(longlong *)(lVar1 + -8);
      if (0x1f < (lVar1 - lVar4) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(lVar1 - lVar4,uVar2 + 0x27);
      }
    }
    thunk_FUN_1407864b8(lVar4);
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
  }
  thunk_FUN_1407864b8(param_1,0x20);
  return;
}




// ================================================================
// FUN_1405262d0 — 409b caller of FUN_14020aa60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5262d0  Name: FUN_1405262d0  Size: 409 bytes
// ------------------------------------------------------------

byte FUN_1405262d0(longlong param_1,undefined8 param_2)

{
  longlong *plVar1;
  char cVar2;
  byte bVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  byte bVar7;
  
  bVar7 = 1;
  FUN_1401ff040(FUN_140524c10,param_1);
  if (*(longlong *)(param_1 + 0x38) != 0) {
    FUN_1404d6400();
    if (*(longlong **)(param_1 + 0x38) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x38) + 8))();
      *(undefined8 *)(param_1 + 0x38) = 0;
    }
  }
  plVar1 = *(longlong **)(param_1 + 0x40);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 8))(plVar1,1);
    *(undefined8 *)(param_1 + 0x40) = 0;
  }
  FUN_1405250c0(param_1);
  cVar2 = FUN_1402204a0(param_2);
  while (cVar2 != '\0') {
    iVar4 = FUN_1402205a0(param_2);
    if (iVar4 == 0x40001) {
      bVar3 = (**(code **)(**(longlong **)(param_1 + 0x38) + 0xd0))
                        (*(longlong **)(param_1 + 0x38),param_2);
      bVar7 = bVar7 & bVar3;
    }
    else if (iVar4 == 0x40002) {
      lVar5 = FUN_140769c58(0xe8);
      lVar6 = 0;
      if (lVar5 != 0) {
        lVar6 = FUN_1406478c0(lVar5);
      }
      bVar3 = FUN_1406492f0(lVar6,param_2);
      bVar7 = bVar7 & bVar3;
      lVar5 = lVar6 + 0x40;
      if (lVar6 == 0) {
        lVar5 = 0;
      }
      cVar2 = FUN_14020aa60(*(undefined8 *)(param_1 + 0x40),lVar5,1);
      if (cVar2 == '\x01') {
        (*(code *)**(undefined8 **)(lVar6 + 0x30))(lVar6 + 0x30);
      }
    }
    else if (iVar4 == 0x40003) {
      cVar2 = FUN_140220610(param_2);
      while (cVar2 != '\0') {
        iVar4 = FUN_140220710(param_2);
        if (iVar4 == 0) {
          FUN_140220730(param_2,param_1 + 0x50,4);
        }
        else {
          bVar7 = 0;
        }
        FUN_1402206a0(param_2);
        cVar2 = FUN_140220610(param_2);
      }
    }
    else {
      bVar7 = 0;
    }
    FUN_140220520(param_2);
    cVar2 = FUN_1402204a0(param_2);
  }
  return bVar7;
}




// ================================================================
// FUN_14053cd60 — 1984b caller of FUN_14020aa60 (head 150 lines)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x53cd60  Name: FUN_14053cd60  Size: 1984 bytes
// ------------------------------------------------------------

void FUN_14053cd60(longlong param_1,longlong param_2)

{
  longlong lVar1;
  longlong lVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  HANDLE hHeap;
  ulonglong uVar10;
  longlong *plVar11;
  uint uVar12;
  int iVar14;
  undefined4 uVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined8 extraout_XMM0_Qb;
  undefined1 auVar20 [16];
  undefined1 auVar21 [16];
  undefined1 auVar22 [16];
  undefined1 auVar23 [16];
  undefined8 local_res8;
  undefined8 in_stack_ffffffffffffff30;
  undefined8 in_stack_ffffffffffffff38;
  undefined4 uVar24;
  undefined1 local_b8 [36];
  undefined4 local_94;
  undefined **local_88;
  LPVOID local_80;
  uint local_74;
  ulonglong uVar13;
  undefined8 extraout_XMM0_Qb_00;
  undefined8 extraout_XMM0_Qb_01;
  
  uVar13 = 0;
  if (*(longlong *)(param_1 + 0x28) == 0) {
    *(longlong *)(param_1 + 0x28) = param_2;
    uVar10 = param_2 + 0x38;
    if (param_2 == 0) {
      uVar10 = uVar13;
    }
    uVar6 = FUN_140058570();
    FUN_140220e90(uVar6,uVar10,param_1,3);
    uVar10 = *(longlong *)(param_1 + 0x28) + 0x38;
    if (*(longlong *)(param_1 + 0x28) == 0) {
      uVar10 = uVar13;
    }
    uVar6 = FUN_140058570();
    FUN_140220e90(uVar6,uVar10,param_1,4);
    uVar10 = *(longlong *)(param_1 + 0x28) + 0x38;
    if (*(longlong *)(param_1 + 0x28) == 0) {
      uVar10 = uVar13;
    }
    uVar6 = FUN_140058570();
    FUN_140220e90(uVar6,uVar10,param_1,0xe);
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    FUN_14053ba60(param_1);
    uVar6 = FUN_1403956c0(*(undefined8 *)(param_1 + 0x28));
    cVar4 = FUN_1404ada70(uVar6);
    if (cVar4 == '\0') {
      iVar14 = 0;
      iVar5 = iVar14;
      if (DAT_140b15418 != 0) {
        iVar5 = *(int *)(DAT_140b15418 + 0x10);
      }
      if ((float)DAT_140b0a340 < (float)(iVar5 - *(int *)(param_1 + 0x30))) {
        if (DAT_140b15418 != 0) {
          iVar14 = *(int *)(DAT_140b15418 + 0x10);
        }
        *(int *)(param_1 + 0x30) = iVar14;
        uVar6 = (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                          (*(longlong **)(param_1 + 0x28),6);
        uVar15 = FUN_140401ba0(uVar6,*(undefined8 *)(param_1 + 0x28),*(undefined4 *)(param_1 + 0x54)
                              );
        lVar9 = *(longlong *)(param_1 + 0x10);
        *(undefined4 *)(param_1 + 0x50) = uVar15;
        lVar7 = *(longlong *)(lVar9 + 0x10);
        while (lVar7 != lVar9 + 8) {
          lVar8 = *(longlong *)(lVar7 + 0x18);
          lVar1 = *(longlong *)(lVar8 + 0x38);
          if ((*(float *)(lVar8 + 0x40) < 0.0) ||
             (((cVar4 = FUN_1406955a0(lVar8,*(undefined8 *)(param_1 + 0x28)), cVar4 != '\0' &&
               (cVar4 = FUN_140695780(*(undefined8 *)(lVar7 + 0x18)), cVar4 == '\0')) &&
              (fVar16 = (float)FUN_1406490d0(lVar1), *(float *)(param_1 + 0x50) <= fVar16)))) {
            uVar6 = FUN_140649110(lVar1);
            cVar4 = FUN_140289050(uVar6);
            if (cVar4 == '\0') {
              fVar16 = (float)FUN_1406490d0(lVar1);
              if (fVar16 == 0.0) {
                uVar6 = FUN_140649110(lVar1);
                in_stack_ffffffffffffff38 = 0;
                in_stack_ffffffffffffff30 = 0;
                cVar4 = FUN_1402804d0(uVar6,*(undefined8 *)(lVar1 + 0x88),
                                      *(undefined8 *)(param_1 + 0x28),1,1,0,0);
                if (cVar4 == '\0') goto LAB_14053d0a0;
                auVar21._0_8_ =
                     FUN_140371230(*(undefined8 *)(lVar1 + 0x88),*(undefined8 *)(param_1 + 0x28));
                auVar21._8_8_ = extraout_XMM0_Qb_00;
                goto LAB_14053cfd4;
              }
LAB_14053cfde:
              lVar7 = *(longlong *)(lVar7 + 8);
            }
            else {
              uVar6 = FUN_140649110();
              in_stack_ffffffffffffff38 = 0;
              in_stack_ffffffffffffff30 = 0;
              cVar4 = FUN_1402804d0(uVar6,*(undefined8 *)(lVar1 + 0x88),
                                    *(undefined8 *)(param_1 + 0x28),0,1,0,0);
              if (cVar4 != '\0') {
                auVar20._0_4_ = (float)FUN_1406490d0(lVar1);
                auVar21._0_8_ =
                     FUN_140371230(*(undefined8 *)(lVar1 + 0x88),*(undefined8 *)(param_1 + 0x28));
                auVar21._8_8_ = extraout_XMM0_Qb;
                if ((float)auVar21._0_8_ <= auVar20._0_4_) {
                  auVar20._4_12_ = auVar21._4_12_;
                  auVar21 = auVar20;
                }
LAB_14053cfd4:
                *(float *)(param_1 + 0x50) = auVar21._0_4_ + *(float *)(param_1 + 0x50);
                goto LAB_14053cfde;
              }
LAB_14053d0a0:
              lVar8 = *(longlong *)(lVar7 + 0x18);
              if (0.0 <= *(float *)(lVar8 + 0x40)) goto LAB_14053cf0b;
              FUN_1405fdd90(*(undefined8 *)(lVar1 + 0xa0),*(undefined8 *)(lVar1 + 0x88));
              lVar7 = *(longlong *)(lVar7 + 8);
              FUN_14064ad00(lVar1);
            }
          }
          else {
            lVar8 = *(longlong *)(lVar7 + 0x18);
LAB_14053cf0b:
            FUN_140695a30(lVar8);
            local_res8 = *(undefined8 *)(lVar7 + 0x18);
            if (DAT_140b39338 == DAT_140b39340) {
              FUN_140045580(&DAT_140b39330,DAT_140b39338,&local_res8);
            }
            else {
              *DAT_140b39338 = local_res8;
              DAT_140b39338 = DAT_140b39338 + 1;
            }
// ... [truncated at 150 lines, total 324]

