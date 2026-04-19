// ================================================================
// RVA 0x1b2ae0  Name: FUN_1401b2ae0  Size: 2241 bytes
// ================================================================

// Callers of FUN_1401b2ae0:


/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1401b2ae0(longlong *param_1,uint param_2)

{
  longlong lVar1;
  float *pfVar2;
  char cVar3;
  longlong lVar4;
  ulonglong uVar5;
  longlong lVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined8 uStack_6cd98;
  undefined8 uStack_6cd90;
  undefined8 uStack_6cd88;
  float fStack_6cd80;
  float fStack_6cd7c;
  float fStack_6cd78;
  float fStack_6cd74;
  float fStack_6cd70;
  float fStack_6cd6c;
  undefined4 uStack_6cd68;
  undefined4 uStack_6cd64;
  undefined4 uStack_6cd60;
  ulonglong uStack_6cd58;
  longlong lStack_6cd50;
  undefined8 uStack_6cd48;
  undefined4 uStack_6cd40;
  undefined4 uStack_6cd3c;
  undefined4 uStack_6cd38;
  undefined4 uStack_6cd34;
  undefined4 uStack_6cd30;
  undefined4 uStack_6cd2c;
  longlong lStack_6cd28;
  longlong lStack_6cd20;
  longlong lStack_6cd18;
  float fStack_6cd08;
  float fStack_6cd04;
  undefined4 uStack_6cd00;
  float fStack_6ccfc;
  float fStack_6ccf8;
  float fStack_6ccf4;
  undefined4 uStack_6ccf0;
  float fStack_6ccec;
  undefined8 uStack_6cce8;
  undefined4 uStack_6cce0;
  float fStack_6ccdc;
  undefined4 uStack_6ccd8;
  undefined4 uStack_6ccd4;
  undefined1 auStack_6cc84 [652];
  float afStack_6c9f8 [196];
  float *apfStack_6c6e8 [3];
  uint uStack_6c6cc;
  undefined4 uStack_6c6c8;
  undefined4 uStack_6c6c4;
  undefined4 uStack_6c6c0;
  undefined4 uStack_6c6bc;
  longlong alStack_6c498 [16];
  int iStack_6c418;
  
  FUN_140148db0(apfStack_6c6e8,param_1[0x34],param_1);
  uStack_6c6cc = param_2;
  FUN_140199b80(param_1,param_2);
  thunk_FUN_140196fb0((int)param_1[0x35]);
  thunk_FUN_140195bd0(param_1[0x50]);
  alStack_6c498[iStack_6c418] = param_1[0x50] + 0x94;
  iStack_6c418 = iStack_6c418 + 1;
  FUN_140140100(afStack_6c9f8);
  lVar6 = param_1[0x50];
  uStack_6cd68 = *(undefined4 *)(lVar6 + 0x70);
  uStack_6cd64 = *(undefined4 *)(lVar6 + 0x74);
  uStack_6cd60 = *(undefined4 *)(lVar6 + 0x78);
  if ((uStack_6c6cc & 6) == 0) {
    uStack_6c6c8 = (undefined4)param_1[0x42];
    uStack_6c6c4 = *(undefined4 *)((longlong)param_1 + 0x214);
    uStack_6c6c0 = (undefined4)param_1[0x43];
    uStack_6c6bc = *(undefined4 *)((longlong)param_1 + 0x21c);
    FUN_140149a90(apfStack_6c6e8,0xbbf);
  }
  else {
    FUN_1401a2080(&uStack_6cd98);
    uVar7 = uStack_6cd98;
    uStack_6cd98 = uStack_6cd98 & 0xffffffffffff0000;
    uVar8 = uStack_6cd98;
    uStack_6cd98._4_4_ = SUB84(uVar7,4);
    uStack_6c6c8 = (float)uStack_6cd98;
    uStack_6c6c4 = uStack_6cd98._4_4_;
    uStack_6c6c0 = (float)uStack_6cd90;
    uStack_6c6bc = uStack_6cd90._4_4_;
    uStack_6cd98 = uVar8;
    FUN_140149a90(apfStack_6c6e8,299);
    FUN_1401a20b0(&uStack_6cd98);
  }
  if (param_1[0x41] != 0) {
    FUN_140132350(param_1[0x41],apfStack_6c6e8);
  }
  if (*(char *)((longlong)param_1 + 0x26a) != '\0') {
    (**(code **)(*param_1 + 0x40))(param_1,apfStack_6c6e8);
    FUN_1401496d0(apfStack_6c6e8,&fStack_6cd80);
    if (param_1[0x41] != 0) {
      FUN_1401320d0(param_1[0x41],&uStack_6cd98);
      fVar10 = (float)uStack_6cd90 - uStack_6cd88._4_4_;
      fVar15 = fStack_6cd78 - fStack_6cd6c;
      if (fVar15 < fVar10) {
        if (fVar15 <= fVar10) {
          fVar15 = fVar10;
        }
        fVar10 = (uStack_6cd88._4_4_ + (float)uStack_6cd90) * DAT_1408007d0;
        if (fStack_6cd6c + fStack_6cd78 <= fVar10) {
          fVar10 = fStack_6cd6c + fStack_6cd78;
        }
        fVar13 = (float)uStack_6cd98 - uStack_6cd90._4_4_;
        if ((float)uStack_6cd98 - uStack_6cd90._4_4_ <= fStack_6cd80 - fStack_6cd74) {
          fVar13 = fStack_6cd80 - fStack_6cd74;
        }
        fVar12 = uStack_6cd90._4_4_ + (float)uStack_6cd98;
        if (fStack_6cd74 + fStack_6cd80 <= uStack_6cd90._4_4_ + (float)uStack_6cd98) {
          fVar12 = fStack_6cd74 + fStack_6cd80;
        }
        fVar11 = uStack_6cd98._4_4_ - (float)uStack_6cd88;
        if (uStack_6cd98._4_4_ - (float)uStack_6cd88 <= fStack_6cd7c - fStack_6cd70) {
          fVar11 = fStack_6cd7c - fStack_6cd70;
        }
        fVar14 = (float)uStack_6cd88 + uStack_6cd98._4_4_;
        if (fStack_6cd70 + fStack_6cd7c <= (float)uStack_6cd88 + uStack_6cd98._4_4_) {
          fVar14 = fStack_6cd70 + fStack_6cd7c;
        }
        fStack_6cd78 = (fVar10 + fVar15) * DAT_1408007c0;
        fStack_6cd80 = (fVar12 + fVar13) * DAT_1408007c0;
        fStack_6cd7c = (fVar14 + fVar11) * DAT_1408007c0;
        fStack_6cd74 = ABS(fVar12 - fVar13) * DAT_1408007c0;
        fStack_6cd6c = ABS(fVar10 - fVar15) * DAT_1408007c0;
        fStack_6cd70 = ABS(fVar14 - fVar11) * DAT_1408007c0;
      }
    }
    if ((((*(char *)((longlong)param_1 + 0x269) != '\0') && ((uStack_6c6cc & 6) == 0)) &&
        (cVar3 = FUN_140140780(afStack_6c9f8,&uStack_6cd68,&fStack_6cd80,param_1[0x34],0),
        *(char *)((longlong)param_1 + 0x269) != '\0')) &&
       ((cVar3 != '\0' && ((uStack_6c6cc & 6) == 0)))) {
      apfStack_6c6e8[0] = afStack_6c9f8;
      FUN_140149a90(apfStack_6c6e8,0x40);
      (**(code **)(*param_1 + 0x40))(param_1,apfStack_6c6e8);
    }
  }
  FUN_140140100(&fStack_6cd08);
  pfVar2 = (float *)param_1[0x34];
  if ((char)param_1[0x4d] != '\0') {
    FUN_140140510(&fStack_6cd08,(float *)param_1[0x34]);
    lVar6 = param_1[0x34];
    fVar15 = *(float *)(lVar6 + 0xc);
    fVar10 = *(float *)(lVar6 + 0x1c);
    fVar13 = *(float *)(lVar6 + 0x2c);
    fVar11 = (float)FUN_14078437c(*(undefined4 *)(lVar6 + 0x18),*(undefined4 *)(lVar6 + 8));
    fVar11 = fVar11 + DAT_14081afc0;
    fVar12 = fVar11;
    if (0x1d000000 < ((uint)fVar11 & 0x7f800000)) {
      fVar12 = (float)FUN_140776650(fVar11);
    }
    fStack_6cd08 = DAT_1407ffaf8;
    if (0x1d000000 < ((uint)fVar11 & 0x7f800000)) {
      fStack_6cd08 = (float)FUN_140776150(fVar11);
    }
    fVar11 = DAT_140a13470;
    fVar14 = (float)((uint)fVar12 ^ DAT_140800860);
    uStack_6ccd8 = DAT_140805898;
    uStack_6ccd4 = DAT_1408007fc;
    FUN_140141260(&fStack_6cd08);
    FUN_1401417c0(&fStack_6cd08);
    fStack_6ccdc = fVar13 + fVar11;
    uStack_6cd00 = 0;
    uStack_6ccf0 = 0;
    fStack_6ccfc = fVar14 * DAT_140819108 + fVar15;
    fStack_6ccec = fStack_6cd08 * DAT_140819108 + fVar10;
    uStack_6cce8 = 0;
    uStack_6cce0 = 0x3f800000;
    fStack_6cd04 = fVar14;
    fStack_6ccf8 = fVar12;
    fStack_6ccf4 = fStack_6cd08;
    FUN_14022d390(&fStack_6cd08,auStack_6cc84);
    FUN_1401417c0(&fStack_6cd08);
    apfStack_6c6e8[0] = &fStack_6cd08;
    FUN_14013d940(0);
    pfVar2 = apfStack_6c6e8[0];
  }
  apfStack_6c6e8[0] = pfVar2;
  thunk_FUN_140180d90(apfStack_6c6e8);
  uVar7 = 0;
  if ((char)param_1[0x4d] != '\0') {
    uStack_6cd58 = _DAT_140800820;
    lStack_6cd50 = _UNK_140800828;
    uStack_6cd38 = _DAT_140800840;
    uStack_6cd34 = _UNK_140800844;
    uStack_6cd30 = _UNK_140800848;
    uStack_6cd2c = _UNK_14080084c;
    uStack_6cd48 = CONCAT44(_UNK_140800834,_DAT_140800830);
    uStack_6cd40 = _UNK_140800838;
    uStack_6cd3c = _UNK_14080083c;
    FUN_14013d850(&uStack_6cd58);
    FUN_14013d820(&fStack_6cd08);
    FUN_140140780(afStack_6c9f8,&uStack_6cd68,&fStack_6cd80,param_1[0x34],1);
    if (param_1[0x4e] == 0) goto LAB_1401b3334;
    lStack_6cd18 = 0;
    uStack_6cd88 = 0;
    lStack_6cd28 = 0;
    lStack_6cd20 = 0;
    uStack_6cd98 = 0;
    uStack_6cd90 = 0;
    FUN_14020eb90(param_1[0x4e],param_1[0x34] + 0xf4,&lStack_6cd28,&uStack_6cd98);
    lVar1 = lStack_6cd20;
    lVar6 = lStack_6cd28;
    uStack_6cd48 = 0;
    uStack_6cd58 = 0;
    lStack_6cd50 = 0;
    if (lStack_6cd28 != lStack_6cd20) {
      uVar8 = lStack_6cd20 - lStack_6cd28 >> 3;
      if (0x1fffffffffffffff < uVar8) goto LAB_1401b3397;
      uVar8 = uVar8 * 8;
      if (uVar8 < 0x1000) {
        uVar5 = uVar7;
        if (uVar8 != 0) {
          uVar5 = FUN_140769c58(uVar8);
        }
      }
      else {
        if (uVar8 + 0x27 <= uVar8) goto LAB_1401b3397;
        lVar4 = FUN_140769c58();
        if (lVar4 == 0) goto LAB_1401b339d;
        uVar5 = lVar4 + 0x27U & 0xffffffffffffffe0;
        *(longlong *)(uVar5 - 8) = lVar4;
      }
      uStack_6cd58 = uVar5;
      uStack_6cd48 = uVar8 + uVar5;
      FUN_14076bd80(uVar5,lVar6,lVar1 - lVar6);
      lStack_6cd50 = uVar8 + uVar5;
    }
    FUN_1401b4120(&uStack_6cd58,0,0xff,0xff,1);
    uVar5 = uStack_6cd90;
    uVar8 = uStack_6cd98;
    uStack_6cd48 = 0;
    uStack_6cd58 = 0;
    lStack_6cd50 = 0;
    if (uStack_6cd98 != uStack_6cd90) {
      uVar9 = (longlong)(uStack_6cd90 - uStack_6cd98) >> 3;
      if (0x1fffffffffffffff < uVar9) {
LAB_1401b3397:
        uStack_6cd48 = 0;
        lStack_6cd50 = 0;
        uStack_6cd58 = 0;
                    /* WARNING: Subroutine does not return */
        FUN_1400225f0();
      }
      uVar9 = uVar9 * 8;
      if (uVar9 < 0x1000) {
        if (uVar9 != 0) {
          uVar7 = FUN_140769c58(uVar9);
        }
      }
      else {
        if (uVar9 + 0x27 <= uVar9) goto LAB_1401b3397;
        lVar6 = FUN_140769c58();
        if (lVar6 == 0) goto LAB_1401b339d;
        uVar7 = lVar6 + 0x27U & 0xffffffffffffffe0;
        *(longlong *)(uVar7 - 8) = lVar6;
      }
      uStack_6cd58 = uVar7;
      uStack_6cd48 = uVar9 + uVar7;
      FUN_14076bd80(uVar7,uVar8,uVar5 - uVar8);
      lStack_6cd50 = uVar9 + uVar7;
    }
    FUN_1401b4120(&uStack_6cd58,0xff,0,0xff,1);
    if (uStack_6cd98 != 0) {
      uVar8 = uStack_6cd88 - uStack_6cd98 & 0xfffffffffffffff8;
      uVar7 = uStack_6cd98;
      if (0xfff < uVar8) {
        uVar7 = *(ulonglong *)(uStack_6cd98 - 8);
        uVar8 = uVar8 + 0x27;
        if (0x1f < (uStack_6cd98 - uVar7) - 8) goto LAB_1401b339d;
      }
      thunk_FUN_1407864b8(uVar7,uVar8);
      uStack_6cd88 = 0;
      uStack_6cd98 = 0;
      uStack_6cd90 = 0;
    }
    if (lStack_6cd28 != 0) {
      uVar7 = lStack_6cd18 - lStack_6cd28 & 0xfffffffffffffff8;
      lVar6 = lStack_6cd28;
      if (0xfff < uVar7) {
        lVar6 = *(longlong *)(lStack_6cd28 + -8);
        uVar7 = uVar7 + 0x27;
        if (0x1f < (lStack_6cd28 - lVar6) - 8U) {
LAB_1401b339d:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(lVar6,uVar7);
    }
  }
LAB_1401b3334:
  alStack_6c498[iStack_6c418] = 0;
  iStack_6c418 = iStack_6c418 + -1;
  FUN_140199f60(param_1);
  FUN_140140500(&fStack_6cd08);
  FUN_140140500(afStack_6c9f8);
  FUN_140149330(apfStack_6c6e8);
  return;
}


