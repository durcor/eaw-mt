/* FUN_14025ec10  RVA 0x25ec10 */


undefined8 ******* FUN_14025ec10(char *param_1,char *param_2)

{
  char cVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 *******pppppppuVar4;
  undefined *puVar5;
  undefined8 ******ppppppuVar6;
  undefined8 *******pppppppuVar7;
  ulonglong uVar8;
  ulonglong *puVar9;
  ulonglong uVar10;
  undefined8 *******pppppppuVar11;
  undefined8 *******pppppppuVar12;
  uint uVar13;
  undefined8 ******local_648;
  undefined4 uStack_640;
  undefined4 uStack_63c;
  ulonglong local_638;
  ulonglong local_630;
  char local_628 [256];
  char local_528 [256];
  char local_428 [256];
  char local_328 [256];
  char local_228 [256];
  char local_128 [256];
  undefined8 *******pppppppuVar14;
  
  pppppppuVar12 = (undefined8 *******)0x0;
  plVar2 = (longlong *)FUN_140142d70(DAT_140a62700,param_1);
  if (plVar2 == (longlong *)0x0) {
    return (undefined8 *******)0x0;
  }
  lVar3 = FUN_140769c58(0x150);
  pppppppuVar4 = pppppppuVar12;
  if (lVar3 != 0) {
    pppppppuVar4 = (undefined8 *******)FUN_140263a20(lVar3);
  }
  FUN_140265d10(pppppppuVar4,plVar2);
  puVar5 = (undefined *)(**(code **)(*plVar2 + 0x20))(plVar2);
  while( true ) {
    if (puVar5 == (undefined *)0x0) {
      return pppppppuVar4;
    }
    if (puVar5 == &DAT_140a12370) break;
    puVar5 = *(undefined **)(puVar5 + 8);
  }
  _splitpath(param_1,local_128,local_228,local_428,local_328);
  if (param_2 != (char *)0x0) {
    _splitpath(param_2,(char *)0x0,(char *)0x0,local_428,(char *)0x0);
  }
  builtin_strncpy(local_328,"ALA",4);
  pppppppuVar14 = pppppppuVar12;
  do {
    lVar3 = (longlong)(int)pppppppuVar14 * 0x20;
    puVar9 = (ulonglong *)(&DAT_140a15d80 + lVar3);
    local_648 = (undefined8 *******)0x0;
    local_638 = 0;
    local_630 = 0;
    uVar8 = *(ulonglong *)(&DAT_140a15d90 + lVar3);
    if (0xf < *(ulonglong *)(&DAT_140a15d98 + lVar3)) {
      puVar9 = (ulonglong *)*puVar9;
    }
    if (uVar8 < 0x10) {
      local_648 = (undefined8 ******)*puVar9;
      uStack_640 = (undefined4)puVar9[1];
      uStack_63c = *(undefined4 *)((longlong)puVar9 + 0xc);
      uVar10 = 0xf;
    }
    else {
      uVar10 = uVar8 | 0xf;
      if (0x7fffffffffffffff < uVar10) {
        uVar10 = 0x7fffffffffffffff;
      }
      if (uVar10 + 1 < 0x1000) {
        pppppppuVar7 = pppppppuVar12;
        if (uVar10 != 0xffffffffffffffff) {
          pppppppuVar7 = (undefined8 *******)FUN_140769c58();
        }
      }
      else {
        if (uVar10 + 0x28 <= uVar10 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
        ppppppuVar6 = (undefined8 ******)FUN_140769c58(uVar10 + 0x28);
        if (ppppppuVar6 == (undefined8 ******)0x0) goto LAB_14025ef12;
        pppppppuVar7 = (undefined8 *******)((longlong)ppppppuVar6 + 0x27U & 0xffffffffffffffe0);
        pppppppuVar7[-1] = ppppppuVar6;
      }
      local_648 = pppppppuVar7;
      FUN_14076bd80(pppppppuVar7,puVar9,uVar8 + 1);
    }
    cVar1 = '\x01';
    pppppppuVar7 = pppppppuVar12;
    local_638 = uVar8;
    local_630 = uVar10;
    do {
      pppppppuVar11 = &local_648;
      if (0xf < local_630) {
        pppppppuVar11 = (undefined8 *******)local_648;
      }
      FUN_140048ec0(local_528,"%s_%s_%02d",local_428,pppppppuVar11,pppppppuVar7);
      _makepath(local_628,local_128,local_228,local_528,local_328);
      lVar3 = FUN_140142f80(DAT_140a62700,local_628);
      if ((lVar3 != 0) ||
         ((cVar1 = FUN_140141f70(DAT_140a62700,local_628), cVar1 == '\x01' &&
          (lVar3 = FUN_140142f80(DAT_140a62700,local_628), lVar3 != 0)))) {
        _strupr(local_528);
        FUN_140263fa0(pppppppuVar4,lVar3,pppppppuVar14,pppppppuVar7,local_528);
      }
      pppppppuVar7 = (undefined8 *******)(ulonglong)((int)pppppppuVar7 + 1);
    } while (cVar1 == '\x01');
    if (0xf < local_630) {
      uVar8 = local_630 + 1;
      pppppppuVar7 = (undefined8 *******)local_648;
      if (0xfff < uVar8) {
        pppppppuVar7 = (undefined8 *******)local_648[-1];
        uVar8 = local_630 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_648 + (-8 - (longlong)pppppppuVar7))) {
LAB_14025ef12:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(pppppppuVar7,uVar8);
    }
    uVar13 = (int)pppppppuVar14 + 1;
    pppppppuVar14 = (undefined8 *******)(ulonglong)uVar13;
    if (0x76 < uVar13) {
      return pppppppuVar4;
    }
  } while( true );
}

