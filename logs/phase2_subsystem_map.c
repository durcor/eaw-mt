// ================================================================
// RENDER FLUSH CALL CHAIN
// ================================================================

// Render flush called from FUN_0x180d90 (site 0x180d9e)

// ------------------------------------------------------------
// RVA: 0x180d90  Name: FUN_140180d90  Size: 32 bytes
// ------------------------------------------------------------

void FUN_140180d90(undefined8 param_1)

{
  FUN_140180dc0(param_1,0xbff);
  FUN_140149650(param_1);
  return;
}




// ================================================================
// GAME-ACTIVE CALLEES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2d72c0  Name: FUN_1402d72c0  Size: 385 bytes
// ------------------------------------------------------------

void FUN_1402d72c0(longlong param_1,undefined4 param_2,undefined4 param_3,uint param_4)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  ulonglong uVar4;
  longlong lVar5;
  
  uVar4 = (ulonglong)param_4;
  if (*(int *)(param_1 + 4) < 1) {
    iVar3 = *(int *)(param_1 + 0x78);
    if (iVar3 != 0) {
      if (1 < iVar3) {
        qsort(*(void **)(param_1 + 0x70),(longlong)iVar3,8,FUN_1402d3d90);
      }
      if (0 < (int)param_4) {
        do {
          FUN_1402d7450(param_1,param_2,param_3);
          FUN_1402d7a40(param_1);
          iVar3 = 0;
          if (0 < *(int *)(param_1 + 0x78)) {
            lVar5 = 0;
            do {
              lVar5 = lVar5 + 8;
              iVar3 = iVar3 + 1;
              *(undefined4 *)(*(longlong *)(*(longlong *)(param_1 + 0x70) + -8 + lVar5) + 0x34) =
                   param_2;
              iVar2 = *(int *)(param_1 + 0x78);
            } while (iVar3 < iVar2);
            if ((0 < iVar2) && (iVar3 = iVar2 + -1, -1 < iVar3)) {
              lVar5 = (longlong)iVar3 * 8;
              do {
                lVar1 = *(longlong *)(lVar5 + *(longlong *)(param_1 + 0x70));
                if (lVar1 == 0) {
                  if ((-1 < iVar3) && (iVar3 < *(int *)(param_1 + 0x78))) {
                    FUN_14025c3c0(param_1 + 0x68,iVar3,iVar2,(*(int *)(param_1 + 0x78) - iVar3) + -1
                                 );
                    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + -1;
                  }
                }
                else if (*(int *)(lVar1 + 0x30) == 0) {
                  if ((-1 < iVar3) && (iVar3 < *(int *)(param_1 + 0x78))) {
                    FUN_14025c3c0(param_1 + 0x68,iVar3,iVar2,(*(int *)(param_1 + 0x78) - iVar3) + -1
                                 );
                    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + -1;
                  }
                  thunk_FUN_1407864b8(lVar1,0x88);
                }
                iVar2 = iVar2 + -1;
                lVar5 = lVar5 + -8;
                iVar3 = iVar3 + -1;
              } while (-1 < iVar3);
            }
          }
          uVar4 = uVar4 - 1;
        } while (uVar4 != 0);
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2d2ab0  Name: FUN_1402d2ab0  Size: 1894 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402d2ab0(longlong *param_1,undefined4 param_2)

{
  char *pcVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 *puVar4;
  float fVar5;
  char cVar6;
  DWORD DVar7;
  int iVar8;
  undefined4 uVar9;
  ulonglong uVar10;
  undefined8 ***pppuVar11;
  longlong *plVar12;
  int iVar13;
  ulonglong uVar14;
  longlong *plVar15;
  undefined8 ****ppppuVar16;
  longlong lVar17;
  longlong lVar18;
  undefined8 ****ppppuVar19;
  ulonglong *puVar20;
  ulonglong uVar21;
  ulonglong uVar22;
  double dVar23;
  longlong *local_res8;
  undefined4 local_res10;
  int local_res20;
  ulonglong in_stack_ffffffffffffff08;
  longlong local_d8;
  undefined8 ***local_d0;
  undefined8 uStack_c8;
  ulonglong local_c0;
  ulonglong uStack_b8;
  int local_b0;
  ulonglong local_a8;
  ulonglong local_a0;
  undefined8 ***local_98;
  undefined8 ***local_90;
  undefined4 uStack_88;
  undefined4 uStack_84;
  ulonglong local_80;
  ulonglong local_78;
  undefined8 ***local_70;
  undefined8 ***local_68;
  undefined8 uStack_60;
  ulonglong local_58;
  ulonglong uStack_50;
  
  fVar5 = DAT_1408007f0;
  if ((((char)*param_1 != '\0') && (*(int *)((longlong)param_1 + 4) < 1)) &&
     (plVar12 = (longlong *)param_1[0xd], plVar12 != (longlong *)param_1[0xe])) {
    local_d8 = 0;
    local_res20 = -1;
    lVar17 = 0;
    iVar8 = -1;
    local_res8 = param_1;
    local_res10 = param_2;
    if (plVar12 == (longlong *)param_1[0xe]) {
LAB_1402d3045:
      if ((lVar17 != 0) && (iVar8 != -1)) {
        FUN_1402d21f0(param_1,lVar17,iVar8,0,in_stack_ffffffffffffff08 & 0xffffffffffffff00);
      }
    }
    else {
      do {
        ppppuVar16 = (undefined8 ****)0x0;
        plVar2 = (longlong *)*plVar12;
        lVar17 = param_1[0x11];
        lVar3 = *plVar2;
        iVar8 = *(int *)((longlong)plVar2 + 0xc);
        lVar18 = (longlong)*(int *)(lVar3 + 0x38);
        iVar13 = (int)(*(longlong *)(lVar3 + 0x28) - *(longlong *)(lVar3 + 0x20) >> 5);
        if (iVar8 == 1) {
          local_b0 = *(int *)(lVar17 + lVar18 * 4);
          local_78 = 0;
          ppppuVar19 = (undefined8 ****)(ulonglong)(local_b0 + 1U);
          if (iVar13 <= (int)(local_b0 + 1U)) {
            ppppuVar19 = ppppuVar16;
          }
          *(int *)(lVar17 + lVar18 * 4) = (int)ppppuVar19;
          puVar20 = (ulonglong *)((longlong)local_b0 * 0x20 + *(longlong *)(*plVar2 + 0x20));
          uVar14 = puVar20[2];
          if (0xf < puVar20[3]) {
            puVar20 = (ulonglong *)*puVar20;
          }
          if (uVar14 < 0x10) {
            ppppuVar16 = (undefined8 ****)*puVar20;
            uStack_88 = (undefined4)puVar20[1];
            uStack_84 = *(undefined4 *)((longlong)puVar20 + 0xc);
            uVar22 = 0xf;
            local_90 = ppppuVar16;
          }
          else {
            uVar22 = uVar14 | 0xf;
            if (0x7fffffffffffffff < uVar22) {
              uVar22 = 0x7fffffffffffffff;
            }
            if (uVar22 + 1 < 0x1000) {
              if (uVar22 != 0xffffffffffffffff) {
                ppppuVar16 = (undefined8 ****)FUN_140769c58();
              }
            }
            else {
              if (uVar22 + 0x28 <= uVar22 + 1) goto LAB_1402d320d;
              pppuVar11 = (undefined8 ***)FUN_140769c58(uVar22 + 0x28);
              if (pppuVar11 == (undefined8 ***)0x0) goto LAB_1402d3213;
              ppppuVar16 = (undefined8 ****)((longlong)pppuVar11 + 0x27U & 0xffffffffffffffe0);
              ppppuVar16[-1] = pppuVar11;
            }
            local_90 = ppppuVar16;
            FUN_14076bd80(ppppuVar16,puVar20,uVar14 + 1);
          }
          local_d0 = (undefined8 ****)0x0;
          lVar17 = -1;
          local_c0 = 0;
          uStack_b8 = 0xf;
          do {
            lVar17 = lVar17 + 1;
          } while (PTR_s___Data_Audio_Music__140a17190[lVar17] != '\0');
          local_80 = uVar14;
          local_78 = uVar22;
          FUN_140022730(&local_d0);
          local_a8 = local_c0;
          if (0x7fffffffffffffff - local_c0 < uVar14) {
                    /* WARNING: Subroutine does not return */
            FUN_140022610();
          }
          local_68 = (undefined8 ****)0x0;
          local_98 = &local_d0;
          if (0xf < uStack_b8) {
            local_98 = local_d0;
          }
          ppppuVar19 = &local_68;
          uVar21 = 0xf;
          local_70 = &local_90;
          if (0xf < uVar22) {
            local_70 = ppppuVar16;
          }
          local_a0 = local_c0 + uVar14;
          if (0xf < local_a0) {
            uVar21 = local_a0 | 0xf;
            if (uVar21 < 0x8000000000000000) {
              if (uVar21 < 0x16) {
                uVar21 = 0x16;
              }
              if (uVar21 + 1 < 0x1000) {
                if (uVar21 == 0xffffffffffffffff) {
                  ppppuVar19 = (undefined8 ****)0x0;
                  local_68 = ppppuVar19;
                }
                else {
                  ppppuVar19 = (undefined8 ****)FUN_140769c58();
                  local_68 = ppppuVar19;
                }
                goto LAB_1402d2e03;
              }
              uVar10 = uVar21 + 0x28;
              if (uVar10 <= uVar21 + 1) {
LAB_1402d320d:
                    /* WARNING: Subroutine does not return */
                FUN_1400225f0();
              }
            }
            else {
              uVar21 = 0x7fffffffffffffff;
              uVar10 = 0x8000000000000027;
            }
            pppuVar11 = (undefined8 ***)FUN_140769c58(uVar10);
            if (pppuVar11 == (undefined8 ***)0x0) goto LAB_1402d3213;
            ppppuVar19 = (undefined8 ****)((longlong)pppuVar11 + 0x27U & 0xffffffffffffffe0);
            ppppuVar19[-1] = pppuVar11;
            local_68 = ppppuVar19;
          }
LAB_1402d2e03:
          local_58 = local_a0;
          uStack_50 = uVar21;
          FUN_14076bd80(ppppuVar19,local_98,local_a8);
          FUN_14076bd80(local_a8 + (longlong)ppppuVar19,local_70,uVar14);
          *(undefined1 *)((longlong)ppppuVar19 + local_a0) = 0;
          if (0xf < uStack_b8) {
            uVar14 = uStack_b8 + 1;
            ppppuVar19 = (undefined8 ****)local_d0;
            if (0xfff < uVar14) {
              ppppuVar19 = (undefined8 ****)local_d0[-1];
              uVar14 = uStack_b8 + 0x28;
              if (0x1f < (ulonglong)((longlong)local_d0 + (-8 - (longlong)ppppuVar19)))
              goto LAB_1402d3213;
            }
            thunk_FUN_1407864b8(ppppuVar19,uVar14);
          }
          param_1 = local_res8;
          local_c0 = local_58;
          uStack_b8 = uStack_50;
          local_d0 = local_68;
          uStack_c8 = uStack_60;
          lVar17 = *plVar2;
          ppppuVar19 = &local_d0;
          if (0xf < uStack_50) {
            ppppuVar19 = (undefined8 ****)local_68;
          }
          dVar23 = (double)*(float *)(lVar17 + 0x40);
          FUN_140025760("MusicEventManager: Starting stream %s (random index %d) - volume percent = %d, fade in = %.2f, fade out = %.2f, id = %d\n"
                        ,ppppuVar19,local_b0,*(undefined1 *)(lVar17 + 0x3c),dVar23,
                        (double)*(float *)(lVar17 + 0x44),(int)local_res8[1]);
          lVar17 = *plVar2;
          in_stack_ffffffffffffff08 =
               CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(lVar17 + 0x44));
          cVar6 = FUN_14022c030(&DAT_140a7d0b0,&local_d0,(float)*(byte *)(lVar17 + 0x3c) / fVar5,
                                *(undefined4 *)(lVar17 + 0x40),in_stack_ffffffffffffff08,
                                *(undefined1 *)(lVar17 + 0x3e),(int)param_1[1]);
          *(int *)((longlong)plVar2 + 0x14) = (int)param_1[1];
          *(int *)(param_1 + 1) = (int)param_1[1] + 1;
          if (cVar6 == '\0') {
            ppppuVar19 = &local_d0;
            if (0xf < uStack_b8) {
              ppppuVar19 = (undefined8 ****)local_d0;
            }
            FUN_140025760("Warning - MusicEventManager: Can\'t start music %s\n",ppppuVar19);
            iVar8 = 3;
          }
          else {
            iVar8 = 2;
          }
          if (0xf < uStack_b8) {
            uVar14 = uStack_b8 + 1;
            ppppuVar19 = (undefined8 ****)local_d0;
            if (0xfff < uVar14) {
              ppppuVar19 = (undefined8 ****)local_d0[-1];
              uVar14 = uStack_b8 + 0x28;
              if (0x1f < (ulonglong)((longlong)local_d0 + (-8 - (longlong)ppppuVar19)))
              goto LAB_1402d3213;
            }
            thunk_FUN_1407864b8(ppppuVar19,uVar14);
          }
          local_d0 = (undefined8 ***)((ulonglong)local_d0 & 0xffffffffffffff00);
          uStack_b8 = 0xf;
          local_c0 = 0;
          param_2 = local_res10;
          if (0xf < uVar22) {
            uVar14 = uVar22 + 1;
            ppppuVar19 = ppppuVar16;
            if (0xfff < uVar14) {
              ppppuVar19 = (undefined8 ****)ppppuVar16[-1];
              uVar14 = uVar22 + 0x28;
              if (0x1f < (ulonglong)((longlong)ppppuVar16 + (-8 - (longlong)ppppuVar19))) {
LAB_1402d3213:
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc();
              }
            }
            thunk_FUN_1407864b8(ppppuVar19,uVar14);
            param_2 = local_res10;
          }
        }
        else if (iVar8 == 2) {
          cVar6 = FUN_14022c370(&DAT_140a7d0b0,*(undefined4 *)((longlong)plVar2 + 0x14));
          param_2 = local_res10;
          if (cVar6 == '\0') {
            FUN_14022c200(&DAT_140a7d0b0);
            *(undefined4 *)((longlong)plVar2 + 0x14) = 0;
            iVar8 = 3;
            param_2 = local_res10;
          }
        }
        else if (iVar8 == 3) {
          if (*(char *)(lVar3 + 0x3d) == '\x01') {
            if (0 < iVar13) {
              *(int *)(plVar2 + 3) = (int)plVar2[3] + 1;
              iVar8 = (int)plVar2[3];
              if (iVar13 <= (int)plVar2[3]) {
                iVar8 = 0;
              }
              *(int *)(plVar2 + 3) = iVar8;
            }
            iVar8 = 1;
            if (((*(int *)((longlong)param_1 + 0xc) != 0xe) || (*(int *)(lVar17 + lVar18 * 4) != 0))
               || (iVar13 < 2)) goto LAB_1402d2ffa;
          }
          local_d8 = plVar2[4];
          local_res20 = (int)plVar2[5];
          *(undefined4 *)(plVar2 + 3) = 0xffffffff;
          iVar8 = 0;
        }
LAB_1402d2ffa:
        if ((int)param_1[1] == 0) {
          *(undefined4 *)(param_1 + 1) = 1;
        }
        *(int *)((longlong)plVar2 + 0xc) = iVar8;
        plVar12 = plVar12 + 1;
        *(undefined4 *)(plVar2 + 2) = param_2;
      } while (plVar12 != (longlong *)param_1[0xe]);
      plVar12 = (longlong *)param_1[0xd];
      do {
        plVar2 = plVar12;
        lVar17 = local_d8;
        iVar8 = local_res20;
        if (plVar2 == (longlong *)param_1[0xe]) goto LAB_1402d3045;
        puVar4 = (undefined8 *)*plVar2;
        plVar12 = plVar2 + 1;
      } while (*(int *)((longlong)puVar4 + 0xc) != 0);
      plVar15 = (longlong *)0x0;
      *(undefined4 *)(puVar4 + 3) = 0xffffffff;
      *puVar4 = 0;
      puVar4[1] = 0;
      puVar4[2] = 0;
      puVar4[4] = 0;
      *(undefined4 *)(puVar4 + 5) = 0xffffffff;
      thunk_FUN_1407864b8(puVar4,0x30);
      FUN_14076bd80(plVar2,plVar12,param_1[0xe] - (longlong)plVar12);
      lVar17 = param_1[0xe];
      param_1[0xe] = lVar17 + -8;
      plVar12 = (longlong *)param_1[0xd];
      if ((param_1[0xe] - (longlong)plVar12 & 0xfffffffffffffff8U) == 0) {
        pcVar1 = (char *)((longlong)param_1 + 0xc);
        pcVar1[0] = -1;
        pcVar1[1] = -1;
        pcVar1[2] = -1;
        pcVar1[3] = -1;
      }
      if ((((local_d8 != 0) && (local_res20 != -1)) &&
          ((DAT_140a7d0a0 != '\0' &&
           ((*(int *)((longlong)param_1 + 0xc) != 0xd || ((char)param_1[0x10] != '\x01')))))) &&
         (((lVar17 + -8) - (longlong)plVar12 >> 3 == 0 ||
          (((longlong *)*plVar12 == (longlong *)0x0 || (*(longlong *)*plVar12 != local_d8)))))) {
        FUN_1402d2850(param_1);
        plVar12 = (longlong *)FUN_140769c58(0x30);
        if (plVar12 != (longlong *)0x0) {
          DVar7 = timeGetTime();
          iVar8 = DVar7 - _DAT_140a7bec0;
          *(int *)(plVar12 + 1) = iVar8;
          *(int *)(plVar12 + 2) = iVar8;
          *plVar12 = local_d8;
          *(undefined4 *)((longlong)plVar12 + 0xc) = 1;
          *(undefined4 *)((longlong)plVar12 + 0x14) = 0;
          *(undefined4 *)(plVar12 + 3) = 0xffffffff;
          plVar12[4] = 0;
          *(undefined4 *)(plVar12 + 5) = 0xffffffff;
          iVar8 = (int)(*(longlong *)(local_d8 + 0x28) - *(longlong *)(local_d8 + 0x20) >> 5);
          plVar15 = plVar12;
          if (0 < iVar8) {
            uVar9 = FUN_1401ffb40(&DAT_140a13e20,0,iVar8 + -1);
            *(undefined4 *)(plVar12 + 3) = uVar9;
          }
        }
        plVar12 = (longlong *)param_1[0xe];
        if (plVar12 == (longlong *)param_1[0xf]) {
          local_res8 = plVar15;
          FUN_14001ded0(param_1 + 0xd,plVar12,&local_res8);
          *(int *)((longlong)param_1 + 0xc) = local_res20;
        }
        else {
          *plVar12 = (longlong)plVar15;
          param_1[0xe] = param_1[0xe] + 8;
          *(int *)((longlong)param_1 + 0xc) = local_res20;
        }
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x321dc0  Name: FUN_140321dc0  Size: 1532 bytes
// ------------------------------------------------------------

void FUN_140321dc0(char *param_1)

{
  undefined2 *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  undefined8 ******ppppppuVar7;
  longlong lVar8;
  undefined8 *******pppppppuVar9;
  undefined8 *******pppppppuVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong *puVar13;
  undefined8 *******pppppppuVar14;
  longlong *plVar15;
  longlong lVar16;
  undefined8 *puVar17;
  uint uVar18;
  ulonglong uVar19;
  ulonglong in_stack_ffffffffffffff88;
  undefined8 ******local_68 [2];
  ulonglong local_58;
  ulonglong local_50;
  undefined8 ******local_48;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  ulonglong local_38;
  ulonglong local_30;
  
  if (((*param_1 != '\0') && (*(int *)(param_1 + 4) < 1)) &&
     (puVar17 = *(undefined8 **)(param_1 + 0x60), puVar17 != *(undefined8 **)(param_1 + 0x68))) {
    puVar2 = (undefined8 *)*puVar17;
    plVar15 = (longlong *)*puVar2;
    if (*(char *)(puVar2 + 1) != '\0') {
      cVar4 = FUN_14022ccf0(&DAT_140a7dfe0,*(undefined4 *)(puVar2 + 2));
      if (cVar4 != '\0') {
        return;
      }
      plVar11 = (longlong *)*puVar2;
      if (0xf < (ulonglong)plVar11[3]) {
        plVar11 = (longlong *)*plVar11;
      }
      FUN_140025760("SpeechEventManager: Event %s: Speech stream ended naturally (System_Update) - removing from queue.\n"
                    ,plVar11);
      uVar12 = 0;
      *(undefined8 *)((longlong)puVar2 + 0xc) = 0xffffffffffffffff;
      *puVar2 = 0;
      *(undefined1 *)(puVar2 + 1) = 0;
      *(undefined4 *)((longlong)puVar2 + 0x14) = 0xffffffff;
      thunk_FUN_1407864b8(puVar2,0x18);
      FUN_14076bd80(puVar17,puVar17 + 1,*(longlong *)(param_1 + 0x68) - (longlong)(puVar17 + 1));
      *(longlong *)(param_1 + 0x68) = *(longlong *)(param_1 + 0x68) + -8;
      if (DAT_140b2b390 != '\0') {
        return;
      }
      DAT_140b2b390 = 1;
      uVar19 = uVar12;
      if (0 < *(int *)(param_1 + 0x88)) {
        do {
          if (*(longlong *)(uVar12 + *(longlong *)(param_1 + 0x80)) != 0) {
            plVar11 = plVar15;
            if (0xf < (ulonglong)plVar15[3]) {
              plVar11 = (longlong *)*plVar15;
            }
            FUN_140025760("SpeechEventManager: Event %s: Doing registered speech done callback %p...\n"
                          ,plVar11);
            (**(code **)(uVar12 + *(longlong *)(param_1 + 0x80)))(plVar15);
          }
          uVar18 = (int)uVar19 + 1;
          uVar12 = uVar12 + 8;
          uVar19 = (ulonglong)uVar18;
        } while ((int)uVar18 < *(int *)(param_1 + 0x88));
      }
      DAT_140b2b390 = 0;
      return;
    }
    iVar5 = FUN_1401ffb40(&DAT_140a13e20,0,(int)(plVar15[9] - plVar15[8] >> 5) + -1);
    puVar13 = (ulonglong *)((longlong)iVar5 * 0x20 + plVar15[8]);
    uVar12 = puVar13[2];
    if (0xf < puVar13[3]) {
      puVar13 = (ulonglong *)*puVar13;
    }
    pppppppuVar10 = (undefined8 *******)0x0;
    if (uVar12 < 0x10) {
      local_48 = (undefined8 ******)*puVar13;
      uStack_40 = (undefined4)puVar13[1];
      uStack_3c = *(undefined4 *)((longlong)puVar13 + 0xc);
      local_30 = 0xf;
    }
    else {
      uVar19 = uVar12 | 0xf;
      if (0x7fffffffffffffff < uVar19) {
        uVar19 = 0x7fffffffffffffff;
      }
      if (uVar19 + 1 < 0x1000) {
        local_48 = pppppppuVar10;
        if (uVar19 != 0xffffffffffffffff) {
          local_48 = (undefined8 ******)FUN_140769c58();
        }
      }
      else {
        if (uVar19 + 0x28 <= uVar19 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
        ppppppuVar7 = (undefined8 ******)FUN_140769c58(uVar19 + 0x28);
        if (ppppppuVar7 == (undefined8 ******)0x0) goto LAB_1403223b2;
        local_48 = (undefined8 ******)((longlong)ppppppuVar7 + 0x27U & 0xffffffffffffffe0);
        local_48[-1] = ppppppuVar7;
      }
      FUN_14076bd80(local_48,puVar13,uVar12 + 1);
      local_30 = uVar19;
    }
    local_38 = uVar12;
    if (*(int *)((longlong)puVar2 + 0xc) != 0) {
      pppppppuVar9 = &local_48;
      if (0xf < local_30) {
        pppppppuVar9 = (undefined8 *******)local_48;
      }
      lVar8 = FUN_140200240(pppppppuVar9);
      lVar16 = -1;
      do {
        lVar16 = lVar16 + 1;
      } while (*(char *)(lVar8 + lVar16) != '\0');
      FUN_140022730(&local_48,lVar8);
    }
    lVar8 = -1;
    local_68[0] = (undefined8 *******)0x0;
    local_58 = 0;
    local_50 = 0xf;
    do {
      lVar8 = lVar8 + 1;
    } while (PTR_s___Data_Audio_Speech__140a15590[lVar8] != '\0');
    FUN_140022730(local_68);
    lVar8 = (longlong)*(int *)((longlong)puVar2 + 0xc);
    puVar17 = &DAT_140a14190 + lVar8 * 4;
    if (0xf < (ulonglong)(&DAT_140a141a8)[lVar8 * 4]) {
      puVar17 = (undefined8 *)*puVar17;
    }
    uVar12 = (&DAT_140a141a0)[lVar8 * 4];
    if (local_50 - local_58 < uVar12) {
      FUN_14004f860(local_68,uVar12,0,puVar17,uVar12);
    }
    else {
      pppppppuVar9 = local_68;
      if (0xf < local_50) {
        pppppppuVar9 = (undefined8 *******)local_68[0];
      }
      lVar8 = (longlong)pppppppuVar9 + local_58;
      local_58 = uVar12 + local_58;
      FUN_14076bd80(lVar8,puVar17,uVar12);
      *(undefined1 *)(uVar12 + lVar8) = 0;
      uVar12 = in_stack_ffffffffffffff88;
    }
    if (local_50 == local_58) {
      uVar12 = 0;
      FUN_14004f860(local_68,1,0,&DAT_140802234,1);
    }
    else {
      pppppppuVar9 = local_68;
      if (0xf < local_50) {
        pppppppuVar9 = (undefined8 *******)local_68[0];
      }
      puVar1 = (undefined2 *)((longlong)pppppppuVar9 + local_58);
      local_58 = local_58 + 1;
      *puVar1 = 0x5c;
    }
    uVar19 = local_38;
    pppppppuVar9 = &local_48;
    if (0xf < local_30) {
      pppppppuVar9 = (undefined8 *******)local_48;
    }
    if (local_50 - local_58 < local_38) {
      uVar12 = local_38;
      FUN_14004f860(local_68,local_38,0,pppppppuVar9,local_38);
    }
    else {
      pppppppuVar14 = local_68;
      if (0xf < local_50) {
        pppppppuVar14 = (undefined8 *******)local_68[0];
      }
      lVar8 = (longlong)pppppppuVar14 + local_58;
      local_58 = local_58 + local_38;
      FUN_14076bd80(lVar8,pppppppuVar9,local_38);
      *(undefined1 *)(uVar19 + lVar8) = 0;
    }
    pppppppuVar9 = local_68;
    if (0xf < local_50) {
      pppppppuVar9 = (undefined8 *******)local_68[0];
    }
    plVar11 = plVar15;
    if (0xf < (ulonglong)plVar15[3]) {
      plVar11 = (longlong *)*plVar15;
    }
    FUN_140025760("SpeechEventManager: Event %s: Starting queued speech stream playback for sample %s - volume = %d\n"
                  ,plVar11,pppppppuVar9,(char)plVar15[0xb]);
    iVar6 = FUN_14022ca30(&DAT_140a7dfe0,local_68,(float)*(byte *)(plVar15 + 0xb) / DAT_1408007f0,3,
                          uVar12 & 0xffffffff00000000,DAT_1408007c0);
    if (iVar6 == -1) {
      *(undefined1 *)(puVar2 + 1) = 0;
      puVar2[2] = 0xffffffffffffffff;
      pppppppuVar9 = pppppppuVar10;
      if (0 < *(int *)(param_1 + 0x88)) {
        do {
          if (*(code **)((longlong)pppppppuVar10 + *(longlong *)(param_1 + 0x80)) != (code *)0x0) {
            (**(code **)((longlong)pppppppuVar10 + *(longlong *)(param_1 + 0x80)))(plVar15);
          }
          uVar18 = (int)pppppppuVar9 + 1;
          pppppppuVar10 = pppppppuVar10 + 1;
          pppppppuVar9 = (undefined8 *******)(ulonglong)uVar18;
        } while ((int)uVar18 < *(int *)(param_1 + 0x88));
      }
      puVar17 = *(undefined8 **)(param_1 + 0x60);
      if ((*(longlong *)(param_1 + 0x68) - (longlong)puVar17 & 0xfffffffffffffff8U) != 0) {
        for (; puVar17 != *(undefined8 **)(param_1 + 0x68); puVar17 = puVar17 + 1) {
          puVar3 = (undefined8 *)*puVar17;
          if (((puVar3 != (undefined8 *)0x0) && ((longlong *)*puVar3 != (longlong *)0x0)) &&
             ((puVar2 == puVar3 && (plVar15 == (longlong *)*puVar3)))) {
            *puVar3 = 0;
            *(undefined8 *)((longlong)puVar3 + 0xc) = 0xffffffffffffffff;
            *(undefined1 *)(puVar3 + 1) = 0;
            *(undefined4 *)((longlong)puVar3 + 0x14) = 0xffffffff;
            thunk_FUN_1407864b8(puVar3,0x18);
            FUN_14076bd80(puVar17,puVar17 + 1,
                          *(longlong *)(param_1 + 0x68) - (longlong)(puVar17 + 1));
            *(longlong *)(param_1 + 0x68) = *(longlong *)(param_1 + 0x68) + -8;
            break;
          }
        }
      }
    }
    else {
      pppppppuVar10 = local_68;
      if (0xf < local_50) {
        pppppppuVar10 = (undefined8 *******)local_68[0];
      }
      if (0xf < (ulonglong)plVar15[3]) {
        plVar15 = (longlong *)*plVar15;
      }
      FUN_140025760("SpeechEventManager: Event %s: Speech stream %s started successfully.\n",plVar15
                    ,pppppppuVar10);
      *(int *)((longlong)puVar2 + 0x14) = iVar5;
      *(undefined1 *)(puVar2 + 1) = 1;
      *(int *)(puVar2 + 2) = iVar6;
    }
    if (0xf < local_50) {
      uVar12 = local_50 + 1;
      pppppppuVar10 = (undefined8 *******)local_68[0];
      if (0xfff < uVar12) {
        pppppppuVar10 = (undefined8 *******)local_68[0][-1];
        uVar12 = local_50 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_68[0] + (-8 - (longlong)pppppppuVar10)))
        goto LAB_1403223b2;
      }
      thunk_FUN_1407864b8(pppppppuVar10,uVar12);
    }
    local_58 = 0;
    local_50 = 0xf;
    local_68[0] = (undefined8 ******)((ulonglong)local_68[0] & 0xffffffffffffff00);
    if (0xf < local_30) {
      uVar12 = local_30 + 1;
      pppppppuVar10 = (undefined8 *******)local_48;
      if (0xfff < uVar12) {
        pppppppuVar10 = (undefined8 *******)local_48[-1];
        uVar12 = local_30 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_48 + (-8 - (longlong)pppppppuVar10))) {
LAB_1403223b2:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(pppppppuVar10,uVar12);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x325190  Name: FUN_140325190  Size: 461 bytes
// ------------------------------------------------------------

void FUN_140325190(longlong param_1,undefined8 param_2)

{
  int iVar1;
  float fVar2;
  float fVar3;
  float local_48;
  float local_44;
  float local_40;
  
  fVar3 = DAT_1407ffaf8;
  if ((((*(char *)(param_1 + 0x30) != '\0') && (*(int *)(param_1 + 4) < 1)) &&
      (*(uint *)(param_1 + 0x34) < 5)) &&
     (((0.0 < *(float *)(param_1 + 0x38) || *(float *)(param_1 + 0x38) == 0.0 &&
       (*(float *)(param_1 + 0x38) <= DAT_1407ffaf8)) &&
      ((0 < *(int *)(param_1 + 0x58) &&
       (iVar1 = *(int *)(param_1 + 0x58) + -1, *(int *)(param_1 + 0x58) = iVar1, iVar1 < 1)))))) {
    if (*(longlong *)(param_1 + 0x48) != 0) {
      local_48 = DAT_140a7e0a0;
      local_44 = DAT_140a7e0a4;
      local_40 = DAT_140a7e0a8;
      fVar2 = (float)FUN_1401ffbb0(&DAT_140a13e20,param_2,DAT_140819b3c);
      if (0x1d000000 < ((uint)fVar2 & 0x7f800000)) {
        fVar3 = (float)FUN_140776150();
      }
      if (0x1d000000 < ((uint)fVar2 & 0x7f800000)) {
        fVar2 = (float)FUN_140776650(fVar2);
      }
      local_48 = local_48 + (fVar3 * DAT_140819104 - fVar2 * 0.0);
      local_44 = local_44 + fVar2 * DAT_140819104 + fVar3 * 0.0;
      local_40 = local_40 + 0.0;
      FUN_1402d5240(&DAT_140b27e60,*(undefined8 *)(param_1 + 0x48),&local_48,0,0,0);
    }
    fVar3 = (float)FUN_1401ffbb0(&DAT_140a13e20);
    *(int *)(param_1 + 0x58) = (int)(fVar3 * (float)DAT_140b0a340);
  }
  return;
}




// ================================================================
// UNCONDITIONAL PER-FRAME CALLEES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x24bb80  Name: FUN_14024bb80  Size: 351 bytes
// ------------------------------------------------------------

void FUN_14024bb80(void)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  longlong *plVar5;
  uint uVar6;
  longlong *plVar7;
  int iVar8;
  
  if ((ulonglong)DAT_140b09e10[1] < 0x100) {
    iVar8 = 0x200 - (int)DAT_140b09e10[1];
    plVar7 = (longlong *)0x0;
    if (0 < iVar8) {
      do {
        plVar3 = (longlong *)FUN_140769c58(0x28);
        plVar5 = (longlong *)0x0;
        if (plVar3 != (longlong *)0x0) {
          *(undefined4 *)(plVar3 + 1) = 0;
          *plVar3 = (longlong)
                    LuaValue<class_std::vector<class_SmartPtr<class_LuaVar>,class_std::allocator<class_SmartPtr<class_LuaVar>_>_>,2>
                    ::vftable;
          plVar3[2] = 0;
          plVar3[3] = 0;
          plVar3[4] = 0;
          (*(code *)LuaValue<class_std::vector<class_SmartPtr<class_LuaVar>,class_std::allocator<class_SmartPtr<class_LuaVar>_>_>,2>
                    ::vftable[0])(plVar3);
          plVar5 = plVar3;
        }
        if ((ulonglong)(plVar5[4] - plVar5[2] >> 3) < 8) {
          FUN_14024c540(plVar5 + 2,8);
        }
        if (plVar5 != (longlong *)0x0) {
          if ((int)plVar5[1] == 1) {
            FUN_140048df0(plVar5 + 2,0);
            plVar3 = DAT_140b09e10;
            lVar1 = *DAT_140b09e10;
            if (DAT_140b09e10[1] == 0xaaaaaaaaaaaaaaa) {
                    /* WARNING: Subroutine does not return */
              FUN_1407408dc("list too long");
            }
            plVar4 = (longlong *)FUN_140769c58(0x18);
            plVar4[2] = (longlong)plVar5;
            (**(code **)*plVar5)(plVar5);
            plVar3[1] = plVar3[1] + 1;
            puVar2 = *(undefined8 **)(lVar1 + 8);
            *plVar4 = lVar1;
            plVar4[1] = (longlong)puVar2;
            *(longlong **)(lVar1 + 8) = plVar4;
            *puVar2 = plVar4;
          }
          (**(code **)(*plVar5 + 8))(plVar5);
        }
        uVar6 = (int)plVar7 + 1;
        plVar7 = (longlong *)(ulonglong)uVar6;
      } while ((int)uVar6 < iVar8);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2505c0  Name: FUN_1402505c0  Size: 17 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402505c0(void)

{
  longlong lVar1;
  char cVar2;
  DWORD DVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined8 *puVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  uint uVar10;
  longlong lVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  longlong lVar14;
  longlong lVar15;
  uint uVar16;
  
  lVar1 = DAT_140b09e58;
  if (DAT_140b09e58 == 0) {
    return;
  }
  if (*(char *)(DAT_140b09e58 + 0x28) != '\0') {
    FUN_1402089e0();
    lVar5 = *(longlong *)(lVar1 + 0x10);
    uVar13 = 0;
    uVar12 = uVar13;
    uVar8 = uVar13;
    if (0 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5)) {
      do {
        if (*(int *)(uVar12 + 0x10 + lVar5) == 1) {
          lVar14 = (longlong)(int)uVar8 * 0x20;
          cVar2 = FUN_140208380(&DAT_140a7c4e0,lVar5 + lVar14);
          if (cVar2 != '\0') {
            lVar5 = *(longlong *)(lVar1 + 0x10);
            uVar4 = FUN_140208210(&DAT_140a7c4e0,lVar5 + lVar14);
            *(undefined8 *)(lVar5 + uVar12 + 8) = uVar4;
            FUN_14024ffd0(lVar1,*(longlong *)(lVar1 + 0x10) + lVar14);
          }
        }
        lVar5 = *(longlong *)(lVar1 + 0x10);
        uVar16 = (int)uVar8 + 1;
        uVar12 = uVar12 + 0x20;
        uVar8 = (ulonglong)uVar16;
      } while ((int)uVar16 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5));
    }
    uVar12 = uVar13;
    if (0 < DAT_140a7c4f0) {
      do {
        lVar5 = FUN_1402082d0(&DAT_140a7c4e0,uVar12);
        if (*(char *)(lVar5 + 0x1c) != '\0') {
          uVar10 = (uint)(*(longlong *)(lVar1 + 0x18) - *(longlong *)(lVar1 + 0x10) >> 5);
          uVar16 = 0;
          if (0 < (int)uVar10) {
            plVar6 = (longlong *)(*(longlong *)(lVar1 + 0x10) + 8);
            uVar8 = uVar13;
            do {
              uVar16 = (uint)uVar8;
              if (*plVar6 == lVar5) break;
              uVar16 = uVar16 + 1;
              uVar8 = (ulonglong)uVar16;
              plVar6 = plVar6 + 4;
            } while ((int)uVar16 < (int)uVar10);
          }
          if ((uVar16 == uVar10) && (lVar14 = FUN_14020a020(lVar5,&DAT_140b09e48), lVar14 != 0)) {
            lVar11 = *(longlong *)(lVar1 + 0x18);
            lVar15 = *(longlong *)(lVar1 + 0x10);
            uVar9 = lVar11 - lVar15 >> 5;
            uVar8 = uVar9 + 1;
            if (uVar8 < uVar9) {
              *(ulonglong *)(lVar1 + 0x18) = uVar8 * 0x20 + lVar15;
            }
            else if (uVar9 < uVar8) {
              if ((ulonglong)(*(longlong *)(lVar1 + 0x20) - lVar15 >> 5) < uVar8) {
                FUN_14024c9f0(lVar1 + 0x10,uVar8);
              }
              else {
                for (lVar15 = uVar8 - uVar9; lVar15 != 0; lVar15 = lVar15 + -1) {
                  FUN_140202370(lVar11,0,0);
                  *(undefined8 *)(lVar11 + 8) = 0;
                  *(undefined4 *)(lVar11 + 0x10) = 0;
                  *(undefined1 *)(lVar11 + 0x14) = 0;
                  lVar11 = lVar11 + 0x20;
                }
                *(longlong *)(lVar1 + 0x18) = lVar11;
              }
            }
            *(longlong *)(*(longlong *)(lVar1 + 0x18) + -0x18) = lVar5;
            FUN_1402024a0(*(longlong *)(lVar1 + 0x18) + -0x20,lVar5 + 0x40);
            *(undefined1 *)(*(longlong *)(lVar1 + 0x18) + -0xc) = 1;
            *(int *)(*(longlong *)(lVar1 + 0x18) + -8) =
                 (int)(*(longlong *)(lVar1 + 0x18) - *(longlong *)(lVar1 + 0x10) >> 5) + -1;
            *(undefined4 *)(*(longlong *)(lVar1 + 0x18) + -0x10) = 2;
            *(undefined4 *)(lVar1 + 0x2c) = *(undefined4 *)(*(longlong *)(lVar1 + 0x18) + -8);
            cVar2 = FUN_140207550(&DAT_140b09e48,lVar14);
            if (cVar2 != '\0') {
              FUN_14024cf20(lVar1,*(longlong *)(lVar1 + 0x18) + -0x20);
              FUN_140209540(&DAT_140a14b80,lVar14);
            }
          }
        }
        uVar16 = (int)uVar12 + 1;
        uVar12 = (ulonglong)uVar16;
      } while ((int)uVar16 < DAT_140a7c4f0);
    }
    if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
                 0xd4) < DAT_140b09e64) &&
       (_Init_thread_header(&DAT_140b09e64), DAT_140b09e64 == -1)) {
      DVar3 = timeGetTime();
      _DAT_140b09e60 = DVar3 - _DAT_140a7bec0;
      _Init_thread_footer(&DAT_140b09e64);
    }
    lVar5 = *(longlong *)(lVar1 + 0x10);
    uVar12 = uVar13;
    uVar8 = uVar13;
    if (0 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5)) {
      do {
        if (((*(longlong *)(uVar12 + 8 + lVar5) == 0) || (*(char *)(uVar12 + lVar5 + 0x14) == '\0'))
           || ((cVar2 = FUN_14020a7c0(), cVar2 == '\0' &&
               (lVar5 = *(longlong *)(lVar1 + 0x10), **(int **)(uVar12 + 8 + lVar5) != 7)))) {
          lVar5 = *(longlong *)(uVar12 + 8 + lVar5);
          if ((lVar5 == 0) || (lVar5 = FUN_14020a020(lVar5,&DAT_140b09e48), lVar5 == 0)) {
            uVar8 = (ulonglong)((int)uVar8 + 1);
            uVar12 = uVar12 + 0x20;
          }
          else {
            cVar2 = FUN_140207550(&DAT_140b09e48,lVar5);
            if (cVar2 != '\0') {
              FUN_14024cf20(lVar1,(longlong)(int)uVar8 * 0x20 + *(longlong *)(lVar1 + 0x10),lVar5);
              FUN_140209540(&DAT_140a14b80,lVar5);
            }
          }
        }
        else {
          FUN_14024fc30(lVar1,uVar8);
        }
        lVar5 = *(longlong *)(lVar1 + 0x10);
      } while ((int)uVar8 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5));
    }
    DVar3 = timeGetTime();
    if (5000 < (DVar3 - _DAT_140a7bec0) - _DAT_140b09e60) {
      lVar5 = *(longlong *)(lVar1 + 0x10);
      uVar12 = uVar13;
      if (0 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5)) {
        do {
          if ((((*(longlong *)(uVar12 + 8 + lVar5) != 0) &&
               (*(char *)(uVar12 + 0x14 + lVar5) != '\0')) && (*(int *)(uVar12 + 0x10 + lVar5) == 7)
              ) && (puVar7 = (undefined8 *)FUN_1402093c0(&DAT_140a14b80,0),
                   puVar7 != (undefined8 *)0x0)) {
            (**(code **)*puVar7)(puVar7);
            FUN_1402075b0(&DAT_140b09e48,puVar7);
            FUN_1402046f0(puVar7,0xd,0x20);
            FUN_140209ef0(*(undefined8 *)(uVar12 + 8 + lVar5),puVar7,0);
          }
          lVar5 = *(longlong *)(lVar1 + 0x10);
          uVar16 = (int)uVar13 + 1;
          uVar13 = (ulonglong)uVar16;
          uVar12 = uVar12 + 0x20;
        } while ((int)uVar16 < (int)(*(longlong *)(lVar1 + 0x18) - lVar5 >> 5));
      }
      DVar3 = timeGetTime();
      _DAT_140b09e60 = DVar3 - _DAT_140a7bec0;
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2089e0  Name: FUN_1402089e0  Size: 12 bytes
// ------------------------------------------------------------

void FUN_1402089e0(void)

{
  FUN_140207e50(&DAT_140a7c4e0);
  return;
}



// ------------------------------------------------------------
// RVA: 0x3b08c0  Name: FUN_1403b08c0  Size: 257 bytes
// ------------------------------------------------------------

void FUN_1403b08c0(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  char cVar4;
  longlong *plVar5;
  
  if (DAT_140b153f8 == 0) {
LAB_1403b092f:
    iVar1 = *(int *)(param_1 + 0x18);
    while (0 < iVar1) {
      plVar2 = *(longlong **)(param_1 + 8);
      if (plVar2 != (longlong *)0x0) {
        plVar5 = plVar2;
        if (plVar2[2] != 0) {
          *(undefined8 *)(plVar2[2] + 0x18) = 0;
          plVar5 = *(longlong **)(param_1 + 8);
        }
        lVar3 = plVar5[2];
        *(longlong *)(param_1 + 8) = lVar3;
        if (lVar3 == 0) {
          *(undefined8 *)(param_1 + 0x10) = 0;
        }
        plVar2[2] = 0;
        plVar2[3] = 0;
        *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + -1;
        cVar4 = (**(code **)(*plVar2 + 0x38))(plVar2);
        if (cVar4 != '\0') {
          *(undefined4 *)(plVar2 + 5) = DAT_140b0a320;
        }
      }
      FUN_1404b3a80(&DAT_140b15690,plVar2);
      (**(code **)(*plVar2 + 8))(plVar2);
      iVar1 = *(int *)(param_1 + 0x18);
    }
  }
  else {
    if ((DAT_140b153f8 != 1) && (DAT_140b153f8 != 2)) {
      if (DAT_140b153f8 == 3) goto LAB_1403b092f;
      if (DAT_140b153f8 != 4) {
        return;
      }
    }
    if (((0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) && (DAT_140b15418 != 0)) &&
       (*(char *)(DAT_140b15418 + 0x98) != '\0')) {
      FUN_1403b03e0(param_1);
      return;
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2c2910  Name: FUN_1402c2910  Size: 677 bytes
// ------------------------------------------------------------

void FUN_1402c2910(longlong param_1)

{
  undefined4 uVar1;
  char cVar2;
  uint uVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong lVar7;
  ulonglong uVar8;
  int iVar9;
  int iVar10;
  uint local_res18 [2];
  
  if (((DAT_140b15418 != 0) && (*(char *)(DAT_140b15418 + 0x98) != '\0')) &&
     ((DAT_140b153f8 == 1 || ((DAT_140b153f8 == 2 || (DAT_140b153f8 == 4)))))) {
    iVar9 = 0;
    iVar10 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
    if (0 < iVar10) {
      do {
        lVar4 = FUN_140294bc0(&DAT_140a16fd0,iVar9);
        if ((lVar4 != 0) && (lVar5 = FUN_140294a40(&DAT_140a16fd0), lVar4 != lVar5)) {
          uVar1 = *(undefined4 *)(lVar4 + 0x4c);
          lVar4 = FUN_140208290(&DAT_140a7c4e0,uVar1);
          if (lVar4 != 0) {
            local_res18[0] = 0;
            lVar5 = FUN_1402c1000(param_1,lVar4,local_res18);
            if (lVar5 != 0) {
              uVar8 = (ulonglong)local_res18[0];
              do {
                if ((int)uVar8 == 0) {
                  uVar8 = 1;
LAB_1402c2a20:
                  do {
                    local_res18[0] = 0xffffffff;
                    cVar2 = FUN_140512790(lVar5,local_res18);
                    if ((cVar2 != '\0') && (local_res18[0] < 0x3b)) {
                      plVar6 = (longlong *)FUN_140512b50(&DAT_140b36bc1);
                      cVar2 = (**(code **)(*plVar6 + 0x30))(plVar6,lVar5);
                      if (cVar2 != '\0') {
                        cVar2 = (**(code **)(*plVar6 + 0x38))(plVar6);
                        if (cVar2 == '\0') {
                          (**(code **)(*plVar6 + 0x20))(plVar6);
                          (**(code **)(*plVar6 + 0x10))(plVar6,1);
                        }
                        else {
                          lVar7 = 0;
                          if (*(longlong *)(param_1 + 0x10) != 0) {
                            *(longlong **)(*(longlong *)(param_1 + 0x10) + 0x10) = plVar6;
                            lVar7 = *(longlong *)(param_1 + 0x10);
                          }
                          plVar6[3] = lVar7;
                          plVar6[2] = 0;
                          *(longlong **)(param_1 + 0x10) = plVar6;
                          if (*(longlong *)(param_1 + 8) == 0) {
                            *(longlong **)(param_1 + 8) = plVar6;
                          }
                          *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + 1;
                          (**(code **)*plVar6)(plVar6);
                          FUN_1404b3ad0(param_1,uVar1);
                        }
                      }
                    }
                    uVar8 = uVar8 - 1;
                  } while (uVar8 != 0);
                }
                else if (0 < (int)uVar8) goto LAB_1402c2a20;
                FUN_140209540(&DAT_140a14b80,lVar5);
                uVar8 = 0;
                lVar5 = FUN_14020a020(lVar4,&DAT_140b31728);
                if ((lVar5 == 0) && (lVar5 = FUN_140209f40(lVar4,&DAT_140b31728), lVar5 == 0)) {
                  lVar5 = FUN_14020a020(lVar4,&DAT_140b31738);
                  if ((lVar5 != 0) || (lVar5 = FUN_140209f40(lVar4,&DAT_140b31738), lVar5 != 0)) {
                    FUN_140207550(&DAT_140b31738,lVar5);
                    uVar3 = FUN_1402043b0(lVar5,5);
                    uVar8 = (ulonglong)uVar3;
                  }
                }
                else {
                  FUN_140207550(&DAT_140b31728,lVar5);
                }
              } while (lVar5 != 0);
            }
          }
        }
        iVar9 = iVar9 + 1;
      } while (iVar9 < iVar10);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x4908c0  Name: FUN_1404908c0  Size: 723 bytes
// ------------------------------------------------------------

void FUN_1404908c0(longlong param_1)

{
  undefined8 uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  longlong lVar7;
  longlong lVar8;
  int *piVar9;
  longlong *plVar10;
  int iVar11;
  
  cVar2 = FUN_14028af60(&DAT_140b153e0);
  if ((cVar2 != '\0') && (iVar11 = 0, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
    do {
      lVar7 = FUN_140294bc0(&DAT_140a16fd0,iVar11);
      if ((lVar7 != 0) && (*(char *)(lVar7 + 0x62) != '\0')) {
        iVar4 = *(int *)(lVar7 + 0x4c);
        iVar3 = FUN_140294a70(&DAT_140a16fd0);
        if ((iVar4 != iVar3) && (lVar7 = FUN_140208290(&DAT_140a7c4e0,iVar4), lVar7 != 0)) {
          while (lVar8 = FUN_14020a120(lVar7,&DAT_140b31758), lVar8 != 0) {
            FUN_140205220(lVar8);
            iVar4 = 0;
            cVar2 = FUN_140207550(&DAT_140b31758,lVar8);
            if (cVar2 != '\0') {
              iVar4 = FUN_1402043b0(lVar8,0x20);
            }
            FUN_140205240(lVar8);
            if (iVar4 != *(int *)(param_1 + 0x40)) break;
            plVar10 = (longlong *)(param_1 + 0x20);
            if (0xf < *(ulonglong *)(param_1 + 0x38)) {
              plVar10 = (longlong *)*plVar10;
            }
            FUN_140025760("Received vote for dialog %s\n",plVar10);
            lVar8 = FUN_14020a020(lVar7,&DAT_140b31758);
            if (lVar8 == 0) break;
            iVar4 = *(int *)(param_1 + 0x10);
            iVar3 = *(int *)(param_1 + 0x14) * 2 >> 1;
            if (iVar3 <= iVar4) {
              iVar4 = iVar3 / 10;
              if (iVar3 / 10 < 10) {
                iVar4 = 10;
              }
              FUN_140047aa0(param_1,iVar4 + iVar3,0,1);
              iVar4 = *(int *)(param_1 + 0x10);
            }
            *(longlong *)(*(longlong *)(param_1 + 8) + (longlong)iVar4 * 8) = lVar8;
            *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
          }
        }
      }
      iVar11 = iVar11 + 1;
    } while (iVar11 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  if (*(char *)(param_1 + 0x44) != '\0') {
    iVar11 = 0;
    if (0 < *(int *)(param_1 + 0x10)) {
      lVar7 = 0;
      do {
        uVar1 = *(undefined8 *)(lVar7 + *(longlong *)(param_1 + 8));
        cVar2 = FUN_140207550(&DAT_140b31758,uVar1);
        if (cVar2 != '\0') {
          iVar4 = FUN_1402043b0(uVar1,0x20);
          iVar3 = FUN_1402043b0(uVar1,0x20);
          iVar5 = FUN_1402043b0(uVar1,0x20);
          uVar6 = FUN_1402043b0(uVar1);
          if ((iVar3 != -1) && (iVar4 == *(int *)(param_1 + 0x40))) {
            iVar4 = 0;
            lVar8 = 0;
            if (0 < *(int *)(param_1 + 0x48)) {
              piVar9 = *(int **)(param_1 + 0x18);
              do {
                if (*piVar9 == iVar3) {
                  (*(int **)(param_1 + 0x18))[lVar8 * 3 + 1] = iVar5;
                  plVar10 = (longlong *)(param_1 + 0x20);
                  *(undefined4 *)(*(longlong *)(param_1 + 0x18) + 8 + lVar8 * 0xc) = uVar6;
                  if (0xf < *(ulonglong *)(param_1 + 0x38)) {
                    plVar10 = (longlong *)*plVar10;
                  }
                  FUN_140025760("Registered vote for dialog %s. Type = %d, Param = %d\n",plVar10,
                                iVar5,uVar6);
                  break;
                }
                iVar4 = iVar4 + 1;
                lVar8 = lVar8 + 1;
                piVar9 = piVar9 + 3;
              } while (iVar4 < *(int *)(param_1 + 0x48));
            }
          }
        }
        FUN_140209540(&DAT_140a14b80,uVar1);
        iVar11 = iVar11 + 1;
        lVar7 = lVar7 + 8;
      } while (iVar11 < *(int *)(param_1 + 0x10));
    }
    *(undefined4 *)(param_1 + 0x10) = 0;
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0xcaa60  Name: FUN_1400caa60  Size: 258 bytes
// ------------------------------------------------------------

void FUN_1400caa60(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  int iVar6;
  longlong *plVar7;
  
  cVar2 = FUN_14028af60(&DAT_140b153e0);
  if (cVar2 != '\0') {
    plVar7 = &DAT_140a16fd0;
    if ((int)(DAT_140a62048 - DAT_140a62040 >> 3) != 0) {
      plVar7 = &DAT_140a62040;
    }
    iVar6 = 0;
    if (0 < (int)(plVar7[1] - *plVar7 >> 3)) {
      do {
        lVar4 = FUN_140294bc0(plVar7,iVar6);
        if ((lVar4 != 0) && (*(char *)(lVar4 + 0x62) != '\0')) {
          iVar1 = *(int *)(lVar4 + 0x4c);
          iVar3 = FUN_140294a70(plVar7);
          if ((iVar1 != iVar3) && (lVar4 = FUN_140208290(&DAT_140a7c4e0,iVar1), lVar4 != 0)) {
            while ((lVar5 = FUN_140209f40(lVar4,&DAT_140b31778), lVar5 != 0 ||
                   (lVar5 = FUN_14020a020(lVar4,&DAT_140b31778), lVar5 != 0))) {
              FUN_140209540(&DAT_140a14b80,lVar5);
            }
          }
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < (int)(plVar7[1] - *plVar7 >> 3));
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x215b90  Name: FUN_140215b90  Size: 474 bytes
// ------------------------------------------------------------

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



// ------------------------------------------------------------
// RVA: 0x339bc0  Name: FUN_140339bc0  Size: 1097 bytes
// ------------------------------------------------------------

void FUN_140339bc0(longlong param_1,uint param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  DWORD DVar3;
  uint uVar4;
  BOOL BVar5;
  longlong lVar6;
  undefined8 uVar7;
  short *psVar8;
  undefined8 *puVar9;
  longlong lVar10;
  undefined8 *puVar11;
  int iVar12;
  longlong lVar13;
  longlong *plVar14;
  ulonglong uVar15;
  longlong lVar16;
  longlong *plVar17;
  longlong lVar18;
  char *pcVar19;
  undefined8 *local_res8;
  undefined1 local_48 [32];
  
  if (*(char *)(param_1 + 0x150) != '\0') {
    if (1000 < param_2 - *(int *)(param_1 + 0x128)) {
      FUN_1403336c0();
      FUN_140333ac0(param_1);
    }
    if (*(int *)(param_1 + 0x168) != 0) {
      puVar9 = (undefined8 *)(param_1 + 0x140);
      puVar11 = local_res8;
      do {
        if (puVar9 != (undefined8 *)0x0) {
          DVar3 = WaitForSingleObject((HANDLE)*puVar9,10000);
          puVar11 = puVar9;
          if (DVar3 == 0) {
            *(char **)(param_1 + 0x148) = "BroadcasterClass::Service";
          }
          else {
            FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                          ,"BroadcasterClass::Service");
          }
        }
        puVar1 = *(undefined8 **)(param_1 + 0x160);
        uVar7 = *puVar1;
        if (0 < *(int *)(param_1 + 0x168)) {
          uVar4 = *(int *)(param_1 + 0x168) - 1;
          if (uVar4 != 0) {
            if ((*(uint *)(param_1 + 0x16c) & 0x80000000) == 0) {
              if (0 < (int)uVar4) {
                uVar15 = (ulonglong)uVar4;
                lVar18 = 0;
                do {
                  *(undefined8 *)(lVar18 + *(longlong *)(param_1 + 0x160)) =
                       *(undefined8 *)(lVar18 + 8 + *(longlong *)(param_1 + 0x160));
                  uVar15 = uVar15 - 1;
                  lVar18 = lVar18 + 8;
                } while (uVar15 != 0);
              }
            }
            else {
              FUN_14076bd80(puVar1,puVar1 + 1);
            }
          }
          *(int *)(param_1 + 0x168) = *(int *)(param_1 + 0x168) + -1;
        }
        if (puVar11 != (undefined8 *)0x0) {
          uVar2 = puVar11[1];
          puVar11[1] = 0;
          BVar5 = ReleaseMutex((HANDLE)*puVar11);
          if (BVar5 == 0) {
            DVar3 = GetLastError();
            FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                          "RunDebugBuild",uVar2);
          }
          puVar11 = (undefined8 *)0x0;
        }
        FUN_140336d50(param_1,uVar7);
        FUN_140209540(&DAT_140a14b80,uVar7);
      } while (*(int *)(param_1 + 0x168) != 0);
    }
    puVar9 = (undefined8 *)(param_1 + 0x130);
    if (puVar9 != (undefined8 *)0x0) {
      DVar3 = WaitForSingleObject((HANDLE)*puVar9,10000);
      local_res8 = puVar9;
      if (DVar3 == 0) {
        *(char **)(param_1 + 0x138) = "BroadcasterClass::Service";
      }
      else {
        pcVar19 = "Unknown";
        if (*(char **)(param_1 + 0x138) != (char *)0x0) {
          pcVar19 = *(char **)(param_1 + 0x138);
        }
        FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                      ,"BroadcasterClass::Service",pcVar19);
      }
    }
    iVar12 = *(int *)(param_1 + 0xd8) + -1;
    if (-1 < iVar12) {
      lVar13 = (longlong)iVar12 * 0x1d8;
      lVar18 = lVar13;
      do {
        lVar16 = *(longlong *)(param_1 + 0xd0);
        uVar4 = *(uint *)(lVar16 + 0xb0 + lVar13);
        if (((uVar4 < param_2) && (*(uint *)(param_1 + 0x224) < param_2 - uVar4)) &&
           (lVar6 = FUN_1403355d0(param_1), lVar6 != lVar16 + lVar18)) {
          puVar9 = (undefined8 *)(*(longlong *)(param_1 + 0xd0) + lVar13);
          if (7 < (ulonglong)puVar9[3]) {
            puVar9 = (undefined8 *)*puVar9;
          }
          FUN_140025760("BroadcasterClass::Time_Out_Player -- Player: %ls\n",puVar9);
          FUN_140339190(param_1,*(longlong *)(param_1 + 0xd0) + lVar18);
        }
        lVar18 = lVar18 + -0x1d8;
        lVar13 = lVar13 + -0x1d8;
        iVar12 = iVar12 + -1;
      } while (-1 < iVar12);
    }
    if (*(int *)(param_1 + 0x30) == 1) {
      uVar7 = FUN_14007a960(local_48,param_1 + 0x38);
      lVar18 = FUN_140335ae0(param_1,uVar7);
      iVar12 = 0;
      if (0 < *(int *)(lVar18 + 0x10)) {
        lVar13 = 0;
        do {
          lVar16 = *(longlong *)(lVar18 + 8) + lVar13;
          if ((*(char *)(lVar16 + 0xc5) != '\0') && (*(int *)(lVar16 + 0xb8) == 2)) {
            psVar8 = (short *)(param_1 + 0x38);
            if (7 < *(ulonglong *)(param_1 + 0x50)) {
              psVar8 = *(short **)(param_1 + 0x38);
            }
            lVar6 = lVar16 + 0x20;
            if (7 < *(ulonglong *)(lVar16 + 0x38)) {
              lVar6 = *(longlong *)(lVar16 + 0x20);
            }
            lVar10 = *(longlong *)(lVar16 + 0x30);
            if (lVar10 == *(longlong *)(param_1 + 0x48)) {
              if (lVar10 != 0) {
                lVar6 = lVar6 - (longlong)psVar8;
                do {
                  if (*(short *)(lVar6 + (longlong)psVar8) != *psVar8) goto LAB_140339ed8;
                  psVar8 = psVar8 + 1;
                  lVar10 = lVar10 + -1;
                } while (lVar10 != 0);
              }
              if (*(int *)(lVar16 + 0xa0) != -1) {
                FUN_140336a50(param_1);
              }
            }
          }
LAB_140339ed8:
          iVar12 = iVar12 + 1;
          lVar13 = lVar13 + 0x1d8;
        } while (iVar12 < *(int *)(lVar18 + 0x10));
      }
    }
    if (*(int *)(param_1 + 0x30) == 2) {
      uVar7 = FUN_14007a960(local_48,param_1 + 0x38);
      lVar18 = FUN_140335ae0(param_1,uVar7);
      if (0 < *(int *)(lVar18 + 0x10)) {
        lVar13 = 0;
        plVar17 = (longlong *)(*(longlong *)(lVar18 + 8) + 0x30);
        do {
          if ((int)plVar17[0x11] == 1) {
            psVar8 = (short *)(param_1 + 0x38);
            if (7 < *(ulonglong *)(param_1 + 0x50)) {
              psVar8 = *(short **)(param_1 + 0x38);
            }
            plVar14 = plVar17 + -2;
            if (7 < (ulonglong)plVar17[1]) {
              plVar14 = (longlong *)*plVar14;
            }
            lVar16 = *plVar17;
            if (lVar16 == *(longlong *)(param_1 + 0x48)) {
              if (lVar16 == 0) goto LAB_140339fb8;
              lVar6 = (longlong)plVar14 - (longlong)psVar8;
              while (*(short *)(lVar6 + (longlong)psVar8) == *psVar8) {
                psVar8 = psVar8 + 1;
                lVar16 = lVar16 + -1;
                if (lVar16 == 0) goto LAB_140339fb8;
              }
            }
          }
          lVar13 = lVar13 + 1;
          plVar17 = plVar17 + 0x3b;
        } while (lVar13 < *(int *)(lVar18 + 0x10));
      }
      FUN_140336c30(param_1);
      if (*(longlong *)(param_1 + 0x1f0) != 0) {
        uVar7 = FUN_1403355d0(param_1);
        (**(code **)(param_1 + 0x1f0))(uVar7);
      }
    }
LAB_140339fb8:
    if (local_res8 != (undefined8 *)0x0) {
      uVar7 = local_res8[1];
      local_res8[1] = 0;
      BVar5 = ReleaseMutex((HANDLE)*local_res8);
      if (BVar5 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",uVar7);
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x22c50  Name: FUN_140022c50  Size: 290 bytes
// ------------------------------------------------------------

void FUN_140022c50(int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  if (*(char *)((longlong)param_1 + 0xd) != '\0') {
    DAT_140b153d0 = (uint)(DAT_140b26fc8 * 1000) < (uint)(param_2 - param_1[2]);
    DAT_140b153d5 = 0;
    iVar2 = 0x78;
    if ((char)param_1[3] != '\0') {
      if (DAT_140a785a8 == 0) {
        if ((uint)(param_2 - param_1[2]) <= (uint)(DAT_140b26fc8 * 1000)) {
          iVar3 = 0x78;
          if (*param_1 < DAT_140b26fe0) {
            iVar3 = *(int *)(DAT_140b26fd8 + (longlong)*param_1 * 4);
          }
          if (((uint)(iVar3 * 1000) < (uint)(param_2 - param_1[1])) &&
             (FUN_140022f10(param_1), (char)param_1[3] == '\0')) {
            return;
          }
        }
        if (DAT_140a785a8 == 0) {
          return;
        }
      }
      if ((DAT_140b153d0 == '\0') && (DAT_1409cf314 != '\0')) {
        uVar1 = 8000;
        if (DAT_140a61fc8 == '\0') {
          uVar1 = 20000;
        }
        if (uVar1 < (uint)(param_2 - param_1[2])) {
          if (*param_1 < DAT_140b26fe0) {
            iVar2 = *(int *)(DAT_140b26fd8 + (longlong)*param_1 * 4);
          }
          if ((uint)(iVar2 * 1000) < (uint)(param_2 - param_1[1])) {
            FUN_140022f10(param_1);
          }
        }
      }
    }
  }
  return;
}




// ================================================================
// SCENE MANAGER / ALWAYS-PATH CALLEES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x30c3b0  Name: FUN_14030c3b0  Size: 3157 bytes
// ------------------------------------------------------------

void FUN_14030c3b0(longlong param_1,int param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  undefined1 uVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  int iVar11;
  longlong lVar12;
  longlong *plVar13;
  longlong lVar14;
  longlong lVar15;
  undefined8 uVar16;
  undefined1 *puVar17;
  undefined8 *puVar18;
  uint uVar19;
  longlong lVar20;
  bool bVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float local_res18;
  float local_res1c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  
  uVar19 = param_2 - *(int *)(param_1 + 0x2128);
  if (*(char *)(param_1 + 0x2699) != '\0') {
    iVar8 = *(int *)(param_1 + 0x2144) - uVar19;
    *(int *)(param_1 + 0x2128) = param_2;
    if (iVar8 < 0) {
      iVar8 = 0;
    }
    *(int *)(param_1 + 0x598) = *(int *)(param_1 + 0x598) - uVar19;
    *(int *)(param_1 + 0x2144) = iVar8;
    return;
  }
  if (*(char *)(param_1 + 0x48) != '\0') {
    FUN_1402fd330();
  }
  lVar20 = 0;
  iVar8 = 0;
  if (*(char *)(param_1 + 0x2065) == '\0') {
    piVar1 = (int *)(param_1 + 0x2068);
    *piVar1 = *piVar1 - uVar19;
    iVar10 = *(int *)(param_1 + 0x2068);
    if (*piVar1 < 0) {
      iVar10 = iVar8;
    }
    *(int *)(param_1 + 0x2068) = iVar10;
  }
  uVar5 = FUN_14020e2b0();
  *(undefined1 *)(param_1 + 0x21a9) = uVar5;
  if ((DAT_140b153d5 == '\0') && (*(int *)(param_1 + 0x2968) == 2)) {
    FUN_1402f4a80(param_1);
  }
  fVar24 = DAT_1408007f8;
  fVar23 = *(float *)(param_1 + 0x2a10) - (float)uVar19 / DAT_1408007f8;
  *(float *)(param_1 + 0x2a10) = fVar23;
  if (0.0 < fVar23) {
    puVar17 = &DAT_00000008;
LAB_14030c4e6:
    FUN_1402f4ca0(param_1,puVar17);
  }
  else {
    *(undefined4 *)(param_1 + 0x2a10) = 0;
    if (0 < *(int *)(param_1 + 0x2a28)) {
      puVar17 = (undefined1 *)0xfffffff8;
      goto LAB_14030c4e6;
    }
    if (*(longlong *)(param_1 + ((longlong)*(int *)(param_1 + 0x211c) + 0x10a) * 0x28) != 0) {
      FUN_1402e4260(param_1);
    }
  }
  FUN_140305c40(param_1);
  fVar23 = DAT_1407ffaf8;
  fVar24 = (float)uVar19 / fVar24;
  fVar22 = *(float *)(param_1 + 0x5cc) - fVar24;
  *(float *)(param_1 + 0x5cc) = fVar22;
  if (*(longlong *)(param_1 + 0x590) == 0) {
    *(undefined2 *)(param_1 + 0x5c4) = 0;
    *(undefined4 *)(param_1 + 0x5c0) = 0;
LAB_14030c5d6:
    *(undefined1 *)(param_1 + 0x26b3) = 0;
  }
  else if (*(char *)(param_1 + 0x5c4) == '\0') {
    if ((*(char *)(param_1 + 0x5c5) != '\0') && (fVar22 <= 0.0)) {
      if (0.0 < *(float *)(param_1 + 0x5c0)) {
        *(float *)(param_1 + 0x5c0) =
             *(float *)(param_1 + 0x5c0) - fVar24 * *(float *)(param_1 + 0x5c8);
      }
      if (*(float *)(param_1 + 0x5c0) <= 0.0) {
        *(undefined1 *)(param_1 + 0x5c5) = 0;
        *(undefined4 *)(param_1 + 0x5c0) = 0;
        FUN_1402e7620(param_1);
        goto LAB_14030c5d6;
      }
    }
  }
  else {
    if (*(float *)(param_1 + 0x5c0) <= fVar23 && fVar23 != *(float *)(param_1 + 0x5c0)) {
      *(float *)(param_1 + 0x5c0) =
           fVar24 * *(float *)(param_1 + 0x5c8) + *(float *)(param_1 + 0x5c0);
    }
    if (fVar23 <= *(float *)(param_1 + 0x5c0)) {
      *(undefined4 *)(param_1 + 0x5c0) = 0x3f800000;
      *(undefined1 *)(param_1 + 0x5c4) = 0;
    }
  }
  if (*(char *)(param_1 + 0x26b1) == '\0') {
    bVar21 = *(char *)(param_1 + 0x26d8) == '\0';
LAB_14030c635:
    if (!bVar21) {
      FUN_14025c490(*(undefined8 *)(param_1 + 0xff0),*(undefined8 *)(param_1 + 0xdd0));
      *(undefined1 *)(param_1 + 0x26d8) = 0;
    }
  }
  else {
    cVar6 = FUN_1402cd0c0(&DAT_140b27a10);
    cVar7 = *(char *)(param_1 + 0x26d8);
    if (cVar6 != '\0') {
      bVar21 = cVar7 == '\0';
      goto LAB_14030c635;
    }
    if (*(char *)(param_1 + 0x2a34) != '\0') {
      bVar21 = cVar7 == '\0';
      goto LAB_14030c635;
    }
    if (cVar7 == '\0') {
      FUN_14025c080(*(undefined8 *)(param_1 + 0xff0),*(undefined8 *)(param_1 + 0xdd0));
      *(undefined1 *)(param_1 + 0x26d8) = 1;
    }
  }
  lVar12 = FUN_140294a40(&DAT_140a16fd0);
  if ((lVar12 != 0) &&
     (lVar12 = *(longlong *)(lVar12 + 0x68), lVar12 != *(longlong *)(param_1 + 0x2b20))) {
    *(longlong *)(param_1 + 0x2b20) = lVar12;
    puVar18 = (undefined8 *)(lVar12 + 0x358);
    if (puVar18 != &DAT_140a28450) {
      if (0xf < *(ulonglong *)(lVar12 + 0x370)) {
        puVar18 = (undefined8 *)*puVar18;
      }
      FUN_140022730(&DAT_140a28450,puVar18,*(undefined8 *)(lVar12 + 0x368));
    }
    plVar13 = (longlong *)**(longlong **)(param_1 + 0x640);
    if (plVar13 != *(longlong **)(param_1 + 0x640)) {
      do {
        plVar3 = (longlong *)plVar13[5];
        if ((plVar3 != (longlong *)0x0) &&
           (lVar12 = (**(code **)(*plVar3 + 0xf8))(plVar3), lVar12 != 0)) {
          lVar15 = -1;
          do {
            lVar15 = lVar15 + 1;
          } while (*(char *)(lVar12 + lVar15) != '\0');
          if (lVar15 != 0) {
            cVar7 = (**(code **)(*plVar3 + 0x108))(plVar3);
            if (cVar7 != '\0') {
              lVar12 = lVar12 + 2;
            }
            (**(code **)(*plVar3 + 0xa0))(plVar3,lVar12,0);
          }
        }
        plVar3 = (longlong *)plVar13[2];
        if (*(char *)((longlong)plVar3 + 0x19) == '\0') {
          cVar7 = *(char *)(*plVar3 + 0x19);
          plVar13 = plVar3;
          plVar3 = (longlong *)*plVar3;
          while (cVar7 == '\0') {
            cVar7 = *(char *)(*plVar3 + 0x19);
            plVar13 = plVar3;
            plVar3 = (longlong *)*plVar3;
          }
        }
        else {
          cVar7 = *(char *)(plVar13[1] + 0x19);
          plVar4 = (longlong *)plVar13[1];
          plVar3 = plVar13;
          while ((plVar13 = plVar4, cVar7 == '\0' && (plVar3 == (longlong *)plVar13[2]))) {
            cVar7 = *(char *)(plVar13[1] + 0x19);
            plVar4 = (longlong *)plVar13[1];
            plVar3 = plVar13;
          }
        }
      } while (plVar13 != *(longlong **)(param_1 + 0x640));
    }
  }
  if (*(char *)(param_1 + 0x2120) == '\0') goto LAB_14030cbda;
  if (*(int *)(param_1 + 0x211c) == 1) {
    FUN_1403e8a80(param_1);
    FUN_1403e83a0(param_1);
    FUN_1403145c0(param_1);
    FUN_1403edd00(param_1);
    FUN_1403e5aa0(param_1);
    FUN_140310e40(param_1);
    FUN_1403e4cb0(param_1);
    FUN_1403e6ff0(param_1);
    FUN_1403e6630(param_1);
    FUN_1403e79b0(param_1);
    lVar12 = FUN_140294a40(&DAT_140a16fd0);
    if ((*(char *)(lVar12 + 0x3c0) != '\0') ||
       (cVar7 = FUN_14028af60(&DAT_140b153e0), cVar7 != '\0')) {
      plVar13 = (longlong *)FUN_140347b50(0x26a);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
      uVar16 = 0x26b;
LAB_14030cb81:
      plVar13 = (longlong *)FUN_140347b50(uVar16);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
    }
  }
  else {
    if (*(int *)(param_1 + 0x211c) == 0) goto LAB_14030cb9a;
    FUN_140317480(param_1);
    if (*(int *)(param_1 + 0x211c) == 2) {
      FUN_140317d00(param_1);
LAB_14030cb28:
      FUN_140316260(param_1);
    }
    else if (*(int *)(param_1 + 0x211c) == 3) {
      FUN_140312570(param_1);
      puVar2 = (undefined4 *)(param_1 + 0x26dc);
      if (*(longlong *)(param_1 + 0x20c0) != 0) {
        iVar10 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if (((iVar10 == 1) && (DAT_140b15418 != (longlong *)0x0)) &&
           (lVar12 = DAT_140b15418[7], lVar12 != 0)) {
          FUN_1401f9340(&local_68,0,fVar23,0,fVar23);
          uVar9 = FUN_140294a70(&DAT_140a16fd0);
          cVar7 = FUN_140494540(lVar12,uVar9,puVar2);
          if (cVar7 == '\0') {
            puVar18 = (undefined8 *)FUN_1401f9340(&local_78,fVar23,0,0,fVar23);
            local_68 = *puVar18;
            uStack_60 = puVar18[1];
            FUN_1401f9440(&local_78);
          }
          puVar18 = (undefined8 *)FUN_1401f9320(&local_78,&local_68);
          lVar12 = *(longlong *)(param_1 + 0x20c0);
          uVar16 = puVar18[1];
          *(undefined8 *)(lVar12 + 0xb8) = *puVar18;
          *(undefined8 *)(lVar12 + 0xc0) = uVar16;
          FUN_1401f9440(puVar18);
          lVar12 = FUN_140294a40(&DAT_140a16fd0);
          local_74 = *(undefined4 *)(param_1 + 0x26e0);
          local_res18 = *(float *)(*(longlong *)(lVar12 + 0x68) + 0x26f8);
          local_78 = *puVar2;
          local_70 = *(undefined4 *)(param_1 + 0x26e4);
          local_res1c = local_res18;
          FUN_14012f570(*(undefined8 *)(param_1 + 0x20c0),&local_78,0,&local_res18,fVar23);
          FUN_14012fba0(*(undefined8 *)(param_1 + 0x20c0),1);
          FUN_1401f9440(&local_68);
        }
      }
      if (*(longlong *)(param_1 + 0x2ae8) != 0) {
        iVar10 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if (((iVar10 == 1) && (DAT_140b15418 != (longlong *)0x0)) &&
           (lVar12 = DAT_140b15418[8], lVar12 != 0)) {
          FUN_1401f9340(&local_68,0,fVar23,0,fVar23);
          uVar9 = FUN_140294a70(&DAT_140a16fd0);
          cVar7 = FUN_140528a70(lVar12,uVar9,puVar2);
          if (cVar7 == '\0') {
            puVar18 = (undefined8 *)FUN_1401f9340(&local_78,fVar23,0,0,fVar23);
            local_68 = *puVar18;
            uStack_60 = puVar18[1];
            FUN_1401f9440(&local_78);
          }
          puVar18 = (undefined8 *)FUN_1401f9320(&local_78,&local_68);
          lVar12 = *(longlong *)(param_1 + 0x2ae8);
          uVar16 = puVar18[1];
          *(undefined8 *)(lVar12 + 0xb8) = *puVar18;
          *(undefined8 *)(lVar12 + 0xc0) = uVar16;
          FUN_1401f9440(puVar18);
          lVar12 = FUN_140294a40(&DAT_140a16fd0);
          local_74 = *(undefined4 *)(param_1 + 0x26e0);
          local_res1c = *(float *)(*(longlong *)(lVar12 + 0x68) + 0x2720);
          local_res18 = local_res1c * DAT_1408007d4;
          local_res1c = local_res1c * DAT_1408007d4;
          local_78 = *puVar2;
          local_70 = *(undefined4 *)(param_1 + 0x26e4);
          FUN_14012f570(*(undefined8 *)(param_1 + 0x2ae8),&local_78,0,&local_res18,fVar23);
          FUN_14012fba0(*(undefined8 *)(param_1 + 0x2ae8),1);
          FUN_1401f9440(&local_68);
        }
      }
      goto LAB_14030cb28;
    }
    FUN_14031d360(param_1);
    plVar13 = (longlong *)FUN_140347b50(0x239);
    (**(code **)(*plVar13 + 0x78))(plVar13,1);
    cVar7 = FUN_14028af60(&DAT_140b153e0);
    if (cVar7 != '\0') {
      plVar13 = (longlong *)FUN_140347b50(0x26d);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
      uVar16 = 0x26c;
      goto LAB_14030cb81;
    }
  }
LAB_14030cb9a:
  iVar10 = FUN_14025e960();
  *(float *)(param_1 + 0x1000) = *(float *)(param_1 + 0xff8) / (float)iVar10;
  iVar10 = FUN_14025e930();
  *(float *)(param_1 + 0x1004) = *(float *)(param_1 + 0xffc) / (float)iVar10;
LAB_14030cbda:
  if (*(int *)(param_1 + 0x211c) == 1) {
    lVar12 = *(longlong *)(param_1 + 0x2030);
    if (((lVar12 != 0) && (*(char *)(lVar12 + 0x335) != -1)) &&
       (*(int *)(DAT_140b2eea0 + 0x44) == 0)) {
      FUN_1403a8820(lVar12,1);
      lVar12 = *(longlong *)(param_1 + 0x2030);
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(undefined4 *)(*(longlong *)(lVar12 + 0x1b0) + 0xc) = 0x40000000;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(undefined4 *)(*(longlong *)(lVar12 + 0x1b0) + 0x10) = 0x3f000000;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(float *)(*(longlong *)(lVar12 + 0x1b0) + 0x14) =
             (float)DAT_140b0a320 / (float)DAT_140b0a340;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      *(longlong *)(param_1 + 0xf98) = lVar12;
    }
    plVar13 = (longlong *)FUN_140347b50(0);
    lVar12 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2708);
    lVar15 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2728);
    lVar14 = FUN_140294a40(&DAT_140a16fd0);
    if (lVar12 == *(longlong *)(lVar14 + 0x68)) {
      (**(code **)(*plVar13 + 0x300))(plVar13,1);
    }
    else {
      if (lVar15 == *(longlong *)(lVar14 + 0x68)) {
        uVar16 = 0;
      }
      else {
        uVar16 = 2;
      }
      (**(code **)(*plVar13 + 0x300))(plVar13,uVar16);
    }
    FUN_140264c80(*(undefined8 *)(param_1 + 0x27a0),1);
    if (*(longlong *)(param_1 + 0x2160) != 0) {
      FUN_1403e1500(param_1);
      FUN_1403e1790(param_1,*(undefined8 *)(param_1 + 0x2160));
    }
  }
  else if (*(int *)(param_1 + 0x211c) != 0) {
    FUN_14031b590(param_1);
    FUN_14031acd0(param_1);
    FUN_140314360(param_1);
    FUN_14031a450(param_1);
  }
  if ((*(int *)(param_1 + 0x211c) != 0) &&
     (iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar10 == 0)) {
    FUN_1402fca50(param_1);
    iVar10 = iVar8;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar10 = (int)DAT_140b15418[2];
    }
    lVar12 = DAT_140b15418[0x6d];
    iVar11 = FUN_1404b1ff0();
    plVar13 = (longlong *)(param_1 + 0x21c8);
    iVar11 = (int)(((float)(iVar10 - (int)lVar12) / (float)iVar11) * DAT_140805898);
    iVar10 = iVar8;
    if (-1 < iVar11) {
      iVar10 = iVar11;
    }
    if (9 < iVar10) {
      iVar10 = 9;
    }
    lVar12 = 10;
    do {
      plVar3 = (longlong *)*plVar13;
      lVar15 = *plVar3;
      if (iVar10 < lVar20) {
        uVar16 = FUN_14034c200(plVar3,0);
        (**(code **)(lVar15 + 0xa0))(*plVar13,uVar16,0);
LAB_14030ce57:
        lVar15 = *plVar13;
        *(byte *)(lVar15 + 0xc0) = *(byte *)(lVar15 + 0xc0) & 0xef;
        *(undefined4 *)(lVar15 + 0xac) = 0;
        *(undefined4 *)(lVar15 + 0xb4) = 0;
      }
      else {
        FUN_14034c200(plVar3,1);
        (**(code **)(lVar15 + 0xa0))(*plVar13);
        if ((lVar20 != iVar10) || (*(int *)(param_1 + 0x2968) == 0)) goto LAB_14030ce57;
        (**(code **)(*(longlong *)*plVar13 + 0x120))((longlong *)*plVar13,0,1,0,0);
      }
      lVar20 = lVar20 + 1;
      plVar13 = plVar13 + 1;
      lVar12 = lVar12 + -1;
    } while (lVar12 != 0);
  }
  *(bool *)(param_1 + 0x2120) = *(char *)(param_1 + 0x2120) == '\0';
  if (*(longlong *)(param_1 + 0xf88) != 0) {
    cVar7 = FUN_1403d6b70(param_1);
    lVar20 = *(longlong *)(param_1 + 0xf88);
    if (cVar7 == '\0') {
      local_78 = *(undefined4 *)(lVar20 + 0x78);
      local_74 = *(undefined4 *)(lVar20 + 0x7c);
      local_70 = *(undefined4 *)(lVar20 + 0x80);
      FUN_140448c20(&DAT_140b30350,&local_78);
    }
    else {
      puVar18 = (undefined8 *)(*(longlong *)(lVar20 + 0x298) + 0xf8);
      if (0xf < *(ulonglong *)(*(longlong *)(lVar20 + 0x298) + 0x110)) {
        puVar18 = (undefined8 *)*puVar18;
      }
      FUN_140025760("\n## Delayed selection of planet %s\n",puVar18);
      FUN_1403de570(param_1,*(undefined8 *)(param_1 + 0xf88),0);
      *(undefined8 *)(param_1 + 0xf88) = 0;
    }
  }
  iVar10 = *(int *)(param_1 + 0x2144) - uVar19;
  *(int *)(param_1 + 0x2128) = param_2;
  if (iVar10 < 0) {
    iVar10 = iVar8;
  }
  *(int *)(param_1 + 0x598) = *(int *)(param_1 + 0x598) - uVar19;
  *(int *)(param_1 + 0x2144) = iVar10;
  if (DAT_140b30392 != '\0') {
    *(undefined2 *)(param_1 + 0x2218) = 0;
    *(undefined2 *)(param_1 + 0x2188) = 0;
    FUN_1403def00(param_1,0);
    if ((*(code **)(param_1 + 0x27d0) != (code *)0x0) && (*(char *)(param_1 + 0x21a8) != '\0')) {
      (**(code **)(param_1 + 0x27d0))();
      *(undefined1 *)(param_1 + 0x21a8) = 0;
    }
  }
  if (*(int *)(param_1 + 0x211c) - 2U < 2) {
    plVar13 = (longlong *)FUN_140347b50(0xe3);
    uVar16 = (**(code **)(*DAT_140b15418 + 0x1e8))();
    cVar7 = FUN_1402ce420(uVar16,0);
    if (cVar7 != '\0') {
      (**(code **)(*plVar13 + 0x1d0))(plVar13,2);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x493f0  Name: FUN_1400493f0  Size: 1049 bytes
// ------------------------------------------------------------

void FUN_1400493f0(longlong param_1,uint param_2)

{
  float fVar1;
  longlong lVar2;
  bool bVar3;
  float fVar4;
  float fVar5;
  char cVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  int iVar10;
  longlong lVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined1 local_78 [12];
  undefined4 local_6c;
  
  iVar10 = *(int *)(param_1 + 0x40);
  while ((iVar10 != 0 && (plVar7 = *(longlong **)(param_1 + 0x38), *(uint *)(plVar7 + 1) <= param_2)
         )) {
    puVar8 = (undefined8 *)*plVar7;
    if (puVar8 != (undefined8 *)0x0) {
      (**(code **)*puVar8)(puVar8,1);
      plVar7 = *(longlong **)(param_1 + 0x38);
    }
    puVar8 = (undefined8 *)plVar7[3];
    if (puVar8 != (undefined8 *)0x0) {
      (**(code **)*puVar8)(puVar8,1);
    }
    if (0 < *(int *)(param_1 + 0x40)) {
      FUN_14004a6c0(param_1 + 0x30,0,1,*(int *)(param_1 + 0x40) + -1);
      *(int *)(param_1 + 0x40) = *(int *)(param_1 + 0x40) + -1;
    }
    iVar10 = *(int *)(param_1 + 0x40);
  }
  FUN_1401f92f0(&local_88);
  fVar5 = DAT_140800908;
  fVar4 = DAT_1408007cc;
  iVar10 = 0;
  if (0 < *(int *)(param_1 + 0x40)) {
    lVar11 = 0;
    do {
      fVar12 = (float)FUN_14025b880(*(undefined8 *)(param_1 + 0x28),0,1);
      fVar16 = *(float *)(param_1 + 0x48);
      if (*(char *)(param_1 + 0x58) == '\0') {
        fVar12 = (float)iVar10 * fVar12 + *(float *)(param_1 + 0x4c);
      }
      else {
        fVar12 = *(float *)(param_1 + 0x4c) - (float)iVar10 * fVar12;
      }
      lVar2 = *(longlong *)(param_1 + 0x38);
      fVar14 = fVar12;
      fVar15 = fVar16;
      if (((*(float *)(lVar11 + 0xc + lVar2) != fVar16) ||
          (*(float *)(lVar11 + 0x10 + lVar2) != fVar12)) &&
         ((fVar16 != *(float *)(param_1 + 0x50) ||
          (*(float *)(param_1 + 0x4c) != *(float *)(param_1 + 0x54))))) {
        fVar15 = *(float *)(lVar11 + 0xc + lVar2);
        fVar14 = *(float *)(lVar11 + 0x10 + lVar2);
        bVar3 = ABS(fVar15 - fVar16) < *(float *)(param_1 + 0x5c) * *(float *)(param_1 + 0x60);
        if (bVar3) {
          fVar15 = fVar16;
        }
        fVar13 = *(float *)(param_1 + 0x5c) * *(float *)(param_1 + 0x60);
        fVar1 = ABS(fVar14 - fVar12);
        if (fVar1 < fVar13) {
          fVar14 = fVar12;
        }
        if ((!bVar3) || (fVar13 <= fVar1)) {
          fVar14 = (fVar12 - *(float *)(lVar11 + 0x10 + lVar2)) * *(float *)(param_1 + 0x5c) +
                   *(float *)(lVar11 + 0x10 + lVar2);
          fVar15 = (fVar16 - *(float *)(lVar11 + 0xc + lVar2)) * *(float *)(param_1 + 0x5c) +
                   *(float *)(lVar11 + 0xc + lVar2);
        }
      }
      if (((((fVar15 != fVar16) || (fVar14 != fVar12)) ||
           (fVar15 != *(float *)(lVar11 + 0xc + lVar2))) ||
          (fVar14 != *(float *)(lVar11 + 0x10 + lVar2))) &&
         (((**(code **)(**(longlong **)(lVar11 + lVar2) + 0x10))(),
          *(char *)(param_1 + 0x59) != '\0' &&
          (fVar16 = ABS(*(float *)(param_1 + 0x4c) - *(float *)(param_1 + 0x54)), fVar16 != 0.0))))
      {
        fVar16 = ABS(*(float *)(param_1 + 0x54) - fVar14) / fVar16;
        puVar8 = (undefined8 *)
                 FUN_1401ce750(*(undefined8 *)(lVar11 + *(longlong *)(param_1 + 0x38)));
        local_88 = *puVar8;
        uStack_80._0_4_ = (undefined4)puVar8[1];
        if (fVar5 <= fVar16) {
          uStack_80 = CONCAT44(fVar16,(undefined4)uStack_80);
        }
        else {
          uStack_80 = CONCAT44(0x3e99999a,(undefined4)uStack_80);
        }
        cVar6 = FUN_14030a6a0(&DAT_140b27f60);
        if (cVar6 != '\0') {
          uStack_80 = uStack_80 & 0xffffffff;
        }
        (**(code **)(**(longlong **)(lVar11 + *(longlong *)(param_1 + 0x38)) + 0x28))();
      }
      lVar2 = *(longlong *)(param_1 + 0x38);
      *(float *)(lVar11 + 0xc + lVar2) = fVar15;
      *(float *)(lVar11 + 0x10 + lVar2) = fVar14;
      lVar2 = *(longlong *)(param_1 + 0x38);
      if (*(longlong *)(lVar11 + 0x18 + lVar2) != 0) {
        *(undefined4 *)(lVar11 + 0x20 + lVar2) = *(undefined4 *)(lVar11 + 0xc + lVar2);
        *(undefined4 *)(lVar11 + 0x24 + lVar2) = *(undefined4 *)(lVar11 + 0x10 + lVar2);
        lVar2 = *(longlong *)(param_1 + 0x38);
        *(float *)(lVar11 + 0x20 + lVar2) =
             *(float *)(lVar11 + 0x20 + lVar2) - *(float *)(lVar11 + 0x2c + lVar2) * fVar4;
        lVar2 = *(longlong *)(param_1 + 0x38);
        plVar7 = *(longlong **)(lVar11 + 0x18 + lVar2);
        (**(code **)(*plVar7 + 0x10))(plVar7,lVar2,*(undefined4 *)(lVar11 + 0x24 + lVar2));
        uVar9 = FUN_1401ce750(*(undefined8 *)(lVar11 + 0x18 + *(longlong *)(param_1 + 0x38)));
        FUN_1401f9320(local_78,uVar9);
        local_6c = uStack_80._4_4_;
        cVar6 = FUN_14030a6a0(&DAT_140b27f60);
        if (cVar6 != '\0') {
          local_6c = 0;
        }
        (**(code **)(**(longlong **)(lVar11 + 0x18 + *(longlong *)(param_1 + 0x38)) + 0x28))();
        FUN_1401f9440(local_78);
      }
      iVar10 = iVar10 + 1;
      lVar11 = lVar11 + 0x40;
    } while (iVar10 < *(int *)(param_1 + 0x40));
  }
  FUN_1401f9440(&local_88);
  return;
}



// ------------------------------------------------------------
// RVA: 0x1c8420  Name: FUN_1401c8420  Size: 2679 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1401c8420(longlong param_1,int param_2)

{
  undefined8 *puVar1;
  int *piVar2;
  int *******pppppppiVar3;
  char cVar4;
  int iVar5;
  longlong *plVar6;
  longlong *plVar7;
  ulonglong uVar8;
  int ******ppppppiVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  undefined8 ******ppppppuVar12;
  wchar_t *pwVar13;
  void *pvVar14;
  undefined8 *******pppppppuVar15;
  longlong lVar16;
  int *******pppppppiVar17;
  ulonglong uVar18;
  int iVar19;
  int *******pppppppiVar20;
  wchar_t *pwVar21;
  undefined8 *******pppppppuVar22;
  longlong lVar23;
  undefined8 *******pppppppuVar24;
  size_t sVar25;
  char local_res8 [8];
  int local_res10;
  int local_res18 [2];
  int local_res20;
  undefined8 *******local_188 [2];
  size_t local_178;
  ulonglong local_170;
  int local_168 [2];
  undefined8 *******local_160 [2];
  longlong local_150;
  ulonglong local_148;
  ulonglong local_140;
  undefined8 *******local_138 [2];
  size_t local_128;
  ulonglong local_120;
  undefined8 *******local_118 [2];
  longlong local_108;
  ulonglong local_100;
  int *******local_f8 [2];
  ulonglong local_e8;
  ulonglong local_e0;
  undefined4 local_d8 [2];
  undefined8 *******local_d0 [2];
  ulonglong local_c0;
  ulonglong local_b8;
  undefined1 local_b0;
  undefined8 local_ac;
  undefined8 local_a4;
  undefined8 *******local_98 [2];
  undefined8 local_88;
  ulonglong uStack_80;
  undefined8 *******local_78;
  int *piStack_70;
  undefined1 local_68 [40];
  
  local_res10 = param_2;
  if ((*(longlong *)(param_1 + 0x98) != 0) &&
     (plVar6 = (longlong *)**(longlong **)(param_1 + 0x90), plVar6 != *(longlong **)(param_1 + 0x90)
     )) {
    do {
      lVar23 = plVar6[2];
      if ((*(char *)(lVar23 + 0xe0) == '\x01') && (*(float *)(lVar23 + 0x138) == 0.0)) {
        plVar7 = (longlong *)*plVar6;
        *(longlong **)plVar6[1] = plVar7;
        *(longlong *)(*plVar6 + 8) = plVar6[1];
        *(longlong *)(param_1 + 0x98) = *(longlong *)(param_1 + 0x98) + -1;
        thunk_FUN_1407864b8(plVar6,0x18);
        FUN_1401c1380(param_1,lVar23);
      }
      else {
        plVar7 = (longlong *)*plVar6;
      }
      plVar6 = plVar7;
    } while (plVar7 != *(longlong **)(param_1 + 0x90));
  }
  if ((uint)(param_2 - *(int *)(param_1 + 0x78)) < 0x21) {
    return;
  }
  FUN_14026cc00(local_68,&DAT_140b0a158);
  lVar23 = *(longlong *)(param_1 + 0x98);
  if (lVar23 == 0) goto LAB_1401c8d71;
  pppppppiVar17 = (int *******)0x0;
  plVar6 = (longlong *)**(longlong **)(param_1 + 0x90);
  pppppppiVar20 = pppppppiVar17;
  if (plVar6 != *(longlong **)(param_1 + 0x90)) {
    do {
      lVar16 = plVar6[2];
      if (*(char *)(lVar16 + 0xe0) == '\0') {
        if ((*(char *)(lVar16 + 0x108) != '\0') || ((int)pppppppiVar20 == (int)lVar23 + -1)) {
          FUN_1401e34e0(lVar16,param_2,local_68);
        }
      }
      else {
        FUN_1401e3b00();
      }
      plVar6 = (longlong *)*plVar6;
      pppppppiVar20 = (int *******)(ulonglong)((int)pppppppiVar20 + 1);
    } while (plVar6 != (longlong *)*(longlong *)(param_1 + 0x90));
  }
  local_138[0] = (undefined8 *******)0x0;
  local_120 = 0xf;
  local_188[0] = (undefined8 *******)0x0;
  local_170 = 0xf;
  local_res18[0] = -1;
  local_128 = 0;
  local_178 = 0;
  FUN_14026cc00(local_res8,&DAT_140b0a158);
  if (*(longlong *)(param_1 + 0x98) == 0) {
LAB_1401c85e6:
    FUN_1401c0e60(param_1);
  }
  else {
    plVar6 = *(longlong **)(param_1 + 0x90);
    plVar7 = (longlong *)*plVar6;
    do {
      if (plVar6 == plVar7) goto LAB_1401c85e6;
      plVar6 = (longlong *)plVar6[1];
    } while (*(char *)(plVar6[2] + 0xe0) != '\0');
    cVar4 = FUN_1401e1f80(plVar6[2],local_res8,local_res18,local_138,local_188);
    sVar25 = local_178;
    if (((cVar4 != '\x01') || (local_178 == 0)) || (local_res18[0] == -1)) goto LAB_1401c85e6;
    local_140 = 0x7fffffffffffffff;
    if (0x7fffffffffffffff - local_178 < 0x18) {
                    /* WARNING: Subroutine does not return */
      FUN_140022610();
    }
    uVar18 = 0xf;
    local_f8[0] = (int *******)0x0;
    pppppppuVar15 = local_188;
    if (0xf < local_170) {
      pppppppuVar15 = local_188[0];
    }
    uVar11 = local_178 + 0x18;
    local_e8 = 0;
    local_e0 = 0;
    uVar8 = 0x8000000000000027;
    pppppppiVar20 = (int *******)local_f8;
    pppppppiVar3 = local_f8[0];
    if (0xf < uVar11) {
      uVar18 = uVar11 | 0xf;
      if (uVar18 < 0x8000000000000000) {
        if (uVar18 < 0x16) {
          uVar18 = 0x16;
        }
        if (uVar18 + 1 < 0x1000) {
          pppppppiVar20 = pppppppiVar17;
          pppppppiVar3 = pppppppiVar17;
          if (uVar18 != 0xffffffffffffffff) {
            pppppppiVar20 = (int *******)FUN_140769c58();
            pppppppiVar3 = pppppppiVar20;
          }
          goto LAB_1401c86ed;
        }
        uVar8 = uVar18 + 0x28;
        if (uVar8 <= uVar18 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
      }
      else {
        uVar18 = 0x7fffffffffffffff;
      }
      ppppppiVar9 = (int ******)FUN_140769c58(uVar8);
      if (ppppppiVar9 == (int ******)0x0) goto LAB_1401c8e98;
      pppppppiVar20 = (int *******)((longlong)ppppppiVar9 + 0x27U & 0xffffffffffffffe0);
      pppppppiVar20[-1] = ppppppiVar9;
      pppppppiVar3 = pppppppiVar20;
    }
LAB_1401c86ed:
    local_f8[0] = pppppppiVar3;
    local_e8 = uVar11;
    local_e0 = uVar18;
    *pppppppiVar20 = (int ******)s_TEXT_GUI_DIALOG_TOOLTIP__14084ea98._0_8_;
    pppppppiVar20[1] = (int ******)s_TEXT_GUI_DIALOG_TOOLTIP__14084ea98._8_8_;
    pppppppiVar20[2] = (int ******)s_TEXT_GUI_DIALOG_TOOLTIP__14084ea98._16_8_;
    FUN_14076bd80(pppppppiVar20 + 3,pppppppuVar15,sVar25);
    *(undefined1 *)((longlong)pppppppiVar20 + uVar11) = 0;
    pppppppiVar20 = pppppppiVar17;
    if (*(longlong *)(param_1 + 0x98) != 0) {
      for (puVar1 = (undefined8 *)**(undefined8 **)(param_1 + 0x90); pppppppiVar20 = pppppppiVar17,
          puVar1 != *(undefined8 **)(param_1 + 0x90); puVar1 = (undefined8 *)*puVar1) {
        pppppppiVar20 = (int *******)puVar1[2];
        if ((*(char *)(pppppppiVar20 + 0x1c) == '\0') && (*(int *)pppppppiVar20 == local_res18[0]))
        {
          if ((((pppppppiVar20 != (int *******)0x0) &&
               ((lVar23 = FUN_1401e1ef0(pppppppiVar20,local_188), lVar23 != 0 &&
                (*(int *)(lVar23 + 0x30) == 0xf)))) &&
              (pppppppiVar17 = (int *******)(lVar23 + 0x2b0), *(longlong *)(lVar23 + 0x2c0) != 0))
             && (local_f8 != (int ********)pppppppiVar17)) {
            if (0xf < *(ulonglong *)(lVar23 + 0x2c8)) {
              pppppppiVar17 = (int *******)*pppppppiVar17;
            }
            FUN_140022730(local_f8,pppppppiVar17);
          }
          break;
        }
      }
    }
    if (*(longlong *)(param_1 + 0x98) != 0) {
      for (puVar1 = (undefined8 *)**(undefined8 **)(param_1 + 0x90);
          puVar1 != *(undefined8 **)(param_1 + 0x90); puVar1 = (undefined8 *)*puVar1) {
        piVar2 = (int *)puVar1[2];
        if (((char)piVar2[0x38] == '\0') && (*piVar2 == local_res18[0])) {
          if ((piVar2 != (int *)0x0) && (lVar23 = FUN_1401e1ef0(piVar2,local_188), lVar23 != 0)) {
            local_res8[0] = *(char *)(lVar23 + 0x78);
            goto LAB_1401c87a4;
          }
          break;
        }
      }
    }
    local_res8[0] = '\0';
LAB_1401c87a4:
    uVar10 = FUN_1401fa680(&DAT_140a7bc58,local_f8,0);
    FUN_14007a960(local_160,uVar10);
    sVar25 = local_178;
    pwVar21 = L"[MISSING]";
    pppppppuVar24 = (undefined8 *******)0x0;
    local_res20 = 0;
    lVar23 = 9;
    pppppppuVar15 = local_160[0];
    if (local_150 == 0) {
LAB_1401c8828:
      if (pppppppiVar20[0x1d] != (int ******)0x0) {
        pppppppuVar15 = local_188;
        if (0xf < local_170) {
          pppppppuVar15 = local_188[0];
        }
        local_d0[0] = (undefined8 *******)0x0;
        local_b8 = 0xf;
        local_98[0] = (undefined8 *******)0x0;
        uStack_80 = 7;
        local_b0 = 0;
        local_ac = 0;
        local_a4 = 0;
        local_78 = (undefined8 *******)0x0;
        piStack_70 = (int *)0x0;
        local_88 = 0;
        local_118[0] = (undefined8 *******)0x0;
        local_108 = 0;
        local_100 = 7;
        local_168[0] = 0;
        local_d8[0] = 0x12;
        if (local_178 < 0x10) {
          local_c0 = local_178;
          FUN_14076bd80(local_d0,pppppppuVar15,local_178);
          *(undefined1 *)((longlong)local_d0 + sVar25) = 0;
        }
        else {
          if (0x7fffffffffffffff < local_178) {
                    /* WARNING: Subroutine does not return */
            FUN_140022610();
          }
          uVar18 = local_178 | 0xf;
          if (uVar18 < 0x8000000000000000) {
            if (uVar18 < 0x16) {
              uVar18 = 0x16;
            }
            local_140 = uVar18;
            if (0xfff < uVar18 + 1) {
              uVar11 = uVar18 + 0x28;
              if (uVar11 <= uVar18 + 1) {
                    /* WARNING: Subroutine does not return */
                FUN_1400225f0();
              }
              goto LAB_1401c896c;
            }
            if (uVar18 != 0xffffffffffffffff) {
              pppppppuVar24 = (undefined8 *******)FUN_140769c58();
            }
          }
          else {
            uVar11 = 0x8000000000000027;
LAB_1401c896c:
            ppppppuVar12 = (undefined8 ******)FUN_140769c58(uVar11);
            if (ppppppuVar12 == (undefined8 ******)0x0) goto LAB_1401c8e92;
            pppppppuVar24 =
                 (undefined8 *******)((longlong)ppppppuVar12 + 0x27U & 0xffffffffffffffe0);
            pppppppuVar24[-1] = ppppppuVar12;
          }
          local_c0 = sVar25;
          local_b8 = local_140;
          FUN_14076bd80(pppppppuVar24,pppppppuVar15,sVar25);
          *(undefined1 *)((longlong)pppppppuVar24 + sVar25) = 0;
          local_d0[0] = pppppppuVar24;
        }
        local_78 = local_118;
        piStack_70 = local_168;
        (*(code *)pppppppiVar20[0x1d])(local_d8);
        if (local_108 != 0) {
          pppppppuVar15 = local_118;
          if (7 < local_100) {
            pppppppuVar15 = local_118[0];
          }
          FUN_1400228a0(local_160,pppppppuVar15);
          local_res20 = local_168[0];
        }
        pppppppuVar15 = local_160[0];
        if (7 < local_100) {
          uVar18 = local_100 * 2 + 2;
          pppppppuVar24 = local_118[0];
          if (0xfff < uVar18) {
            pppppppuVar24 = (undefined8 *******)local_118[0][-1];
            uVar18 = local_100 * 2 + 0x29;
            if (0x1f < (ulonglong)((longlong)local_118[0] + (-8 - (longlong)pppppppuVar24)))
            goto LAB_1401c8e92;
          }
          thunk_FUN_1407864b8(pppppppuVar24,uVar18);
        }
        local_108 = 0;
        pppppppuVar24 = local_d0;
        if (0xf < local_b8) {
          pppppppuVar24 = local_d0[0];
        }
        local_100 = 7;
        local_118[0] = (undefined8 *******)((ulonglong)local_118[0] & 0xffffffffffff0000);
        local_d8[0] = 0xffffffff;
        local_c0 = 0;
        *(undefined1 *)pppppppuVar24 = 0;
        local_b0 = 0;
        pppppppuVar24 = local_98;
        if (7 < uStack_80) {
          pppppppuVar24 = local_98[0];
        }
        local_ac = 0;
        local_a4 = 0;
        local_78 = (undefined8 *******)0x0;
        piStack_70 = (int *)0x0;
        local_88 = 0;
        *(undefined2 *)pppppppuVar24 = 0;
        if (7 < uStack_80) {
          uVar18 = uStack_80 * 2 + 2;
          pppppppuVar24 = local_98[0];
          if (0xfff < uVar18) {
            pppppppuVar24 = (undefined8 *******)local_98[0][-1];
            uVar18 = uStack_80 * 2 + 0x29;
            if (0x1f < (ulonglong)((longlong)local_98[0] + (-8 - (longlong)pppppppuVar24)))
            goto LAB_1401c8e92;
          }
          thunk_FUN_1407864b8(pppppppuVar24,uVar18);
        }
        local_88 = _DAT_1407fffd0;
        uStack_80 = _UNK_1407fffd8;
        local_98[0] = (undefined8 *******)((ulonglong)local_98[0] & 0xffffffffffff0000);
        if (0xf < local_b8) {
          uVar18 = local_b8 + 1;
          if ((0xfff < uVar18) &&
             (uVar18 = local_b8 + 0x28,
             (undefined1 *)0x1f <
             (undefined1 *)((longlong)local_d0[0] + (-8 - (longlong)local_d0[0][-1])))) {
LAB_1401c8e92:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
          thunk_FUN_1407864b8(_DAT_1407fffd0,uVar18);
        }
      }
    }
    else {
      pppppppuVar22 = local_160;
      if (7 < local_148) {
        pppppppuVar22 = local_160[0];
      }
      if (local_150 == 9) {
        pwVar13 = L"[MISSING]";
        lVar16 = lVar23;
        do {
          if (*(wchar_t *)((longlong)pwVar13 + (longlong)(pppppppuVar22 + -0x28109d57)) != *pwVar13)
          goto LAB_1401c8b6c;
          pwVar13 = pwVar13 + 1;
          lVar16 = lVar16 + -1;
        } while (lVar16 != 0);
        goto LAB_1401c8828;
      }
    }
LAB_1401c8b6c:
    sVar25 = local_128;
    lVar16 = local_150;
    iVar19 = local_res18[0];
    if (*(int *)(param_1 + 0x1300) == local_res18[0]) {
      pvVar14 = (void *)(param_1 + 0x1308);
      pppppppuVar24 = local_138;
      if (0xf < local_120) {
        pppppppuVar24 = local_138[0];
      }
      if (0xf < *(ulonglong *)(param_1 + 0x1320)) {
        pvVar14 = *(void **)(param_1 + 0x1308);
      }
      if ((*(size_t *)(param_1 + 0x1318) != local_128) ||
         (iVar5 = memcmp(pvVar14,pppppppuVar24,*(size_t *)(param_1 + 0x1318)), iVar5 != 0))
      goto LAB_1401c8c14;
      pvVar14 = (void *)(param_1 + 0x1328);
      pppppppuVar24 = local_188;
      if (0xf < local_170) {
        pppppppuVar24 = local_188[0];
      }
      if (0xf < *(ulonglong *)(param_1 + 0x1340)) {
        pvVar14 = *(void **)(param_1 + 0x1328);
      }
      if ((((*(size_t *)(param_1 + 0x1338) != local_178) ||
           (iVar5 = memcmp(pvVar14,pppppppuVar24,*(size_t *)(param_1 + 0x1338)), iVar5 != 0)) ||
          (local_res20 != *(int *)(param_1 + 0x1304))) || (*(char *)(param_1 + 0x1368) != '\0'))
      goto LAB_1401c8c14;
    }
    else {
LAB_1401c8c14:
      uVar18 = local_148;
      if ((local_res8[0] != '\0') && (lVar16 != 0)) {
        pppppppuVar24 = local_160;
        if (7 < local_148) {
          pppppppuVar24 = pppppppuVar15;
        }
        if (local_150 != 9) {
LAB_1401c8d9b:
          if (*(char *)(param_1 + 0x1368) == '\0') {
            FUN_1401c0e60(param_1);
            sVar25 = local_128;
            iVar19 = local_res18[0];
          }
          *(int *)(param_1 + 0x1300) = iVar19;
          if ((undefined8 ********)(param_1 + 0x1308) != local_138) {
            pppppppuVar24 = local_138;
            if (0xf < local_120) {
              pppppppuVar24 = local_138[0];
            }
            FUN_140022730((undefined8 *******)(param_1 + 0x1308),pppppppuVar24,sVar25);
          }
          if ((undefined8 ********)(param_1 + 0x1328) != local_188) {
            pppppppuVar24 = local_188;
            if (0xf < local_170) {
              pppppppuVar24 = local_188[0];
            }
            FUN_140022730((undefined8 *******)(param_1 + 0x1328),pppppppuVar24,local_178);
          }
          if ((undefined8 ********)(param_1 + 0x1348) != local_160) {
            pppppppuVar24 = local_160;
            if (7 < uVar18) {
              pppppppuVar24 = pppppppuVar15;
            }
            FUN_1400228a0((undefined8 *******)(param_1 + 0x1348),pppppppuVar24,local_150);
            pppppppuVar15 = local_160[0];
          }
          *(int *)(param_1 + 0x1304) = local_res20;
          *(undefined1 *)(param_1 + 0x1368) = 0;
          if ((*(code **)(param_1 + 0x1370) != (code *)0x0) &&
             (cVar4 = (**(code **)(param_1 + 0x1370))(), cVar4 == '\0')) {
            *(undefined1 *)(param_1 + 0x1368) = 1;
          }
          goto LAB_1401c8c61;
        }
        do {
          if (*(wchar_t *)((longlong)(pppppppuVar24 + -0x28109d57) + (longlong)pwVar21) != *pwVar21)
          goto LAB_1401c8d9b;
          pwVar21 = pwVar21 + 1;
          lVar23 = lVar23 + -1;
        } while (lVar23 != 0);
      }
      FUN_1401c0e60(param_1);
    }
LAB_1401c8c61:
    if (7 < local_148) {
      uVar18 = local_148 * 2 + 2;
      pppppppuVar24 = pppppppuVar15;
      if (0xfff < uVar18) {
        pppppppuVar24 = (undefined8 *******)pppppppuVar15[-1];
        uVar18 = local_148 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)pppppppuVar15 + (-8 - (longlong)pppppppuVar24)))
        goto LAB_1401c8e98;
      }
      thunk_FUN_1407864b8(pppppppuVar24,uVar18);
    }
    param_2 = local_res10;
    if (0xf < local_e0) {
      uVar18 = local_e0 + 1;
      pppppppiVar20 = local_f8[0];
      if (0xfff < uVar18) {
        pppppppiVar20 = (int *******)local_f8[0][-1];
        uVar18 = local_e0 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_f8[0] + (-8 - (longlong)pppppppiVar20))) {
LAB_1401c8e98:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(pppppppiVar20,uVar18);
      param_2 = local_res10;
    }
  }
  if (0xf < local_170) {
    uVar18 = local_170 + 1;
    pppppppuVar15 = local_188[0];
    if (0xfff < uVar18) {
      pppppppuVar15 = (undefined8 *******)local_188[0][-1];
      uVar18 = local_170 + 0x28;
      if (0x1f < (ulonglong)((longlong)local_188[0] + (-8 - (longlong)pppppppuVar15)))
      goto LAB_1401c8e74;
    }
    thunk_FUN_1407864b8(pppppppuVar15,uVar18);
  }
  local_178 = 0;
  local_170 = 0xf;
  local_188[0] = (undefined8 *******)((ulonglong)local_188[0] & 0xffffffffffffff00);
  if (0xf < local_120) {
    if (0xfff < local_120 + 1) {
      pppppppuVar15 = (undefined8 *******)local_138[0][-1];
      uVar18 = local_120 + 0x28;
      if (0x1f < (ulonglong)((longlong)local_138[0] + (-8 - (longlong)pppppppuVar15))) {
LAB_1401c8e74:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(pppppppuVar15,uVar18);
      }
    }
    thunk_FUN_1407864b8();
  }
LAB_1401c8d71:
  *(int *)(param_1 + 0x78) = param_2;
  return;
}



