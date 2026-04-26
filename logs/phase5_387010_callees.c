/* =========================================================
 * FUN_140385cf0  (RVA 0x385cf0)
 * size: 376 bytes  params: 0  return: undefined
 * ========================================================= */


longlong FUN_140385cf0(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  undefined1 *puVar5;
  char ****ppppcVar6;
  char ***local_138 [2];
  longlong local_128;
  ulonglong local_120;
  char local_118 [272];
  
  if ((((*(longlong *)(param_1 + 0x20) != 0) &&
       (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4e) != '\0')) &&
      (*(longlong *)(param_1 + 0x10) != 0)) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2a0) != 0)) {
    lVar4 = FUN_1402648b0();
    if (lVar4 != 0) {
      FUN_14001e680(local_138,*(longlong *)(param_1 + 0x20) + 0x60);
      if (local_128 != 0) {
        iVar3 = *(int *)(param_1 + 0x90);
        if (iVar3 < 0) {
          ppppcVar6 = local_138;
          if (0xf < local_120) {
            ppppcVar6 = (char ****)local_138[0];
          }
          lVar2 = -(longlong)ppppcVar6;
          do {
            cVar1 = *(char *)ppppcVar6;
            *(char *)((longlong)ppppcVar6 + (longlong)(local_118 + lVar2)) = cVar1;
            ppppcVar6 = (char ****)((longlong)ppppcVar6 + 1);
          } while (cVar1 != '\0');
          _strupr(local_118);
          puVar5 = (undefined1 *)FUN_14076c698(local_118,&DAT_140819540);
          if (puVar5 != (undefined1 *)0x0) {
            *puVar5 = 0;
          }
          iVar3 = FUN_14012d520(lVar4,local_118);
          *(int *)(param_1 + 0x90) = iVar3;
          if (iVar3 < 0) {
            lVar4 = 0;
            goto LAB_140385df9;
          }
        }
        lVar4 = FUN_14012d6f0(lVar4,iVar3);
      }
LAB_140385df9:
      if (0xf < local_120) {
        ppppcVar6 = (char ****)local_138[0];
        if (0xfff < local_120 + 1) {
          ppppcVar6 = (char ****)local_138[0][-1];
          if ((char *)0x1f < (char *)((longlong)local_138[0] + (-8 - (longlong)ppppcVar6))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
        }
        thunk_FUN_1407864b8(ppppcVar6);
      }
      return lVar4;
    }
  }
  return 0;
}



/* =========================================================
 * FUN_140381ff0  (RVA 0x381ff0)
 * size: 746 bytes  params: 0  return: undefined
 * ========================================================= */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_140381ff0(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  undefined1 local_98 [4];
  float local_94;
  float local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  float local_78;
  undefined4 uStack_74;
  float fStack_70;
  undefined4 uStack_6c;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  float local_48;
  undefined4 uStack_44;
  float fStack_40;
  undefined4 uStack_3c;
  undefined8 local_38;
  undefined8 uStack_30;
  
  lVar2 = FUN_140385cf0();
  uVar3 = 0;
  if (lVar2 != 0) {
    FUN_140383650(param_1,local_98);
    cVar1 = FUN_140386170(local_98,param_1 + 0x9c);
    if (cVar1 != '\x01') {
      fVar4 = *(float *)(param_1 + 0xa0);
      fVar6 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x4c0);
      if (local_94 <= fVar4) {
        fVar5 = fVar4;
        if ((local_94 < fVar4) && (fVar5 = local_94, local_94 <= fVar4 - fVar6)) {
          fVar5 = fVar4 - fVar6;
        }
      }
      else {
        fVar5 = local_94;
        if (fVar4 + fVar6 <= local_94) {
          fVar5 = fVar4 + fVar6;
        }
      }
      fVar4 = *(float *)(param_1 + 0xa4);
      *(float *)(param_1 + 0xa0) = fVar5;
      if (local_90 <= fVar4) {
        fVar5 = fVar4;
        if ((local_90 < fVar4) && (fVar5 = local_90, local_90 <= fVar4 - fVar6)) {
          fVar5 = fVar4 - fVar6;
        }
      }
      else {
        fVar5 = local_90;
        if (fVar4 + fVar6 <= local_90) {
          fVar5 = fVar4 + fVar6;
        }
      }
      *(float *)(param_1 + 0xa4) = fVar5;
      fVar4 = (float)FUN_14020b6d0(local_90 - fVar5);
      if (fVar4 != 0.0) {
        if (fVar4 <= 0.0) {
          fVar5 = (float)((uint)fVar6 ^ DAT_140800860);
          if ((float)((uint)fVar6 ^ DAT_140800860) <= fVar4) {
            fVar5 = fVar4;
          }
        }
        else {
          fVar5 = fVar6;
          if (fVar4 <= fVar6) {
            fVar5 = fVar4;
          }
        }
        *(float *)(param_1 + 0xa4) = fVar5 + *(float *)(param_1 + 0xa4);
      }
      fVar5 = (float)FUN_14020b6d0(*(undefined4 *)(param_1 + 0xa4));
      uStack_3c = _UNK_14080083c;
      fStack_40 = _UNK_140800838;
      uStack_44 = _UNK_140800834;
      local_48 = _DAT_140800830;
      fVar6 = DAT_1408007dc;
      fVar4 = DAT_1408007d4;
      local_58 = _DAT_140800820;
      uStack_50 = _UNK_140800828;
      local_38 = _DAT_140800840;
      uStack_30 = _UNK_140800848;
      *(float *)(param_1 + 0xa4) = fVar5;
      FUN_1400480f0(&local_58,((fVar6 * fVar4) / DAT_1408007f4) * fVar5);
      FUN_14012d480(lVar2,*(undefined4 *)(param_1 + 0x94),&local_58);
      if (-1 < *(int *)(param_1 + 0x98)) {
        local_88 = _DAT_140800820;
        uStack_80 = _UNK_140800828;
        fVar6 = ((fVar6 * fVar4) / DAT_1408007f4) * *(float *)(param_1 + 0xa0);
        local_78 = _DAT_140800830;
        uStack_74 = _UNK_140800834;
        fStack_70 = _UNK_140800838;
        uStack_6c = _UNK_14080083c;
        local_68 = _DAT_140800840;
        uStack_60 = _UNK_140800848;
        fVar4 = fVar6;
        if (0x1d000000 < ((uint)fVar6 & 0x7f800000)) {
          fVar4 = (float)FUN_140776650(fVar6);
        }
        fVar5 = DAT_1407ffaf8;
        if (0x1d000000 < ((uint)fVar6 & 0x7f800000)) {
          fVar5 = (float)FUN_140776150(fVar6);
        }
        fVar6 = (float)local_88 * fVar4;
        local_88 = CONCAT44(local_88._4_4_,(float)local_88 * fVar5 - (float)uStack_80 * fVar4);
        uStack_80 = CONCAT44(uStack_80._4_4_,(float)uStack_80 * fVar5 + fVar6);
        fVar6 = fStack_70 * fVar4;
        fStack_70 = fStack_70 * fVar5 + local_78 * fVar4;
        local_78 = local_78 * fVar5 - fVar6;
        fVar6 = (float)local_68 * fVar4;
        local_68 = CONCAT44(local_68._4_4_,(float)local_68 * fVar5 - (float)uStack_60 * fVar4);
        uStack_60 = CONCAT44(uStack_60._4_4_,(float)uStack_60 * fVar5 + fVar6);
        FUN_14012d480(lVar2,*(undefined4 *)(param_1 + 0x98),&local_88);
      }
    }
    uVar3 = 1;
  }
  return uVar3;
}



/* =========================================================
 * FUN_14012d2a0  (RVA 0x12d2a0)
 * size: 12 bytes  params: 0  return: undefined
 * ========================================================= */


void FUN_14012d2a0(longlong param_1)

{
  FUN_140145fb0(*(undefined8 *)(param_1 + 0xe8));
  return;
}



