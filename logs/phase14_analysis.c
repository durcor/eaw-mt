// ================================================================
// 1.4B — FUN_14021caf0 — GOM tick candidate
// ================================================================
// ------------------------------------------------------------
// RVA: 0x21caf0  Name: FUN_14021caf0  Size: 539 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14021caf0(char *param_1,int param_2,undefined4 *param_3,undefined4 *param_4,
                  undefined4 *param_5)

{
  undefined4 uVar1;
  char cVar2;
  
  if (((*param_1 != '\0') && (*(longlong *)(param_1 + 0xff0) != 0)) && (param_1[8] != '\0')) {
    if (*(int *)(param_1 + 4) < 1) {
      *(undefined4 *)(param_1 + 0x1000) = *param_3;
      *(undefined4 *)(param_1 + 0x1004) = param_3[1];
      *(undefined4 *)(param_1 + 0x1008) = param_3[2];
      *(undefined4 *)(param_1 + 0x100c) = *param_4;
      *(undefined4 *)(param_1 + 0x1010) = param_4[1];
      *(undefined4 *)(param_1 + 0x1014) = param_4[2];
      *(undefined4 *)(param_1 + 0x1018) = *param_5;
      *(undefined4 *)(param_1 + 0x101c) = param_5[1];
      *(undefined4 *)(param_1 + 0x1020) = param_5[2];
      AIL_set_listener_3D_position(*(undefined8 *)(param_1 + 0xff8),*param_3,param_3[1],param_3[2]);
      AIL_set_listener_3D_orientation
                (*(undefined8 *)(param_1 + 0xff8),*param_4,param_4[1],param_4[2],*param_5,param_5[1]
                 ,param_5[2]);
      FUN_140227f20(param_1 + 0x68,param_2);
      FUN_140229da0(param_1 + 0x490,param_2);
      FUN_14022bc20(param_1 + 0x10,param_2);
      FUN_14022c550(param_1 + 0xf40,param_2);
      FUN_14021d2f0(param_1,0,param_2);
      uVar1 = *(undefined4 *)(param_1 + 4000);
      cVar2 = FUN_14021d2f0(param_1,1,param_2);
      if (cVar2 == '\x01') {
        FUN_1402286e0(param_1 + 0x68,*(undefined4 *)(param_1 + 0xfb4),uVar1);
        FUN_14022a140(param_1 + 0x490,*(undefined4 *)(param_1 + 0xfb4),uVar1);
      }
      cVar2 = FUN_14021d2f0(param_1,2,param_2);
      if (cVar2 == '\x01') {
        FUN_14022c990(param_1 + 0xf40,*(undefined4 *)(param_1 + 0xfc8),uVar1);
      }
      cVar2 = FUN_14021d2f0(param_1,3,param_2);
      if (cVar2 == '\x01') {
        FUN_14022bf90(param_1 + 0x10,*(undefined4 *)(param_1 + 0xfdc),uVar1);
      }
      if (0x7d < (uint)(param_2 - _DAT_140a7e0d0)) {
        AIL_serve();
        _DAT_140a7e0d0 = param_2;
      }
    }
    *(int *)(param_1 + 0xc) = param_2;
  }
  return;
}




// ================================================================
// 1.4A — Render flush candidates
// (via xref to geometry-limit error string)
// ================================================================

// String " ERROR: render task for %s exceeded the dynamic geometry limits (%d pverts, %d lverts, %d indices)\n" @ 0x14081ae80
//   xref from FUN_0x180dc0 (call site 0x180fc9)
// String "*** WARNING *** alGraphicsDriver::Load_Texture - Failed to find texture (tga/dds) %s\n" @ 0x140819310
//   xref from FUN_0x13db20 (call site 0x13dc5a)
// String "*** WARNING *** alGraphicsDriver::Load_Texture - Failed to find texture %s\n" @ 0x140819370
//   xref from FUN_0x13dd10 (call site 0x13dd81)
// ------------------------------------------------------------
// RVA: 0x180dc0  Name: FUN_140180dc0  Size: 2078 bytes
// ------------------------------------------------------------

void FUN_140180dc0(longlong *param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined8 *puVar4;
  bool bVar5;
  bool bVar6;
  undefined4 uVar7;
  char cVar8;
  longlong *plVar9;
  longlong lVar10;
  undefined *puVar11;
  int iVar12;
  int iVar13;
  char *pcVar14;
  ulonglong uVar15;
  ulonglong uVar16;
  longlong *plVar17;
  ulonglong uVar18;
  int iVar19;
  longlong *plVar20;
  int iVar21;
  uint uVar22;
  longlong *plVar23;
  uint local_res10;
  int local_res20;
  uint local_88;
  longlong *local_80;
  longlong local_68;
  undefined8 *local_60 [4];
  
  FUN_14013d820(*param_1);
  local_res20 = 0;
  iVar21 = 0;
  iVar13 = 0;
  iVar12 = 0;
  iVar19 = 0;
  DAT_140a62740 = 1;
  plVar23 = DAT_140a6fda0;
  if ((longlong)DAT_140a6fda0 - (longlong)DAT_140a6fd98 >> 3 != 0) {
    plVar23 = DAT_140a6fd98;
  }
  local_80 = param_1 + 0xd85e;
  local_88 = 1;
  local_68 = 0xc;
  plVar9 = local_80;
  plVar20 = local_80;
  DAT_140a6fda0 = plVar23;
  do {
    if (((param_2 & local_88) != 0) && ((longlong *)plVar20[1] != plVar20)) {
      plVar17 = (longlong *)plVar9[1];
      while (plVar17 != plVar9) {
        puVar4 = (undefined8 *)plVar17[3];
        iVar1 = *(int *)((longlong)puVar4 + 0x5c);
        iVar2 = *(int *)(puVar4 + 0xb);
        iVar3 = *(int *)(puVar4 + 0xc);
        local_60[0] = puVar4;
        if (((iVar1 < 1) && (iVar2 < 1)) && (iVar3 < 1)) {
LAB_140180f8d:
          plVar17 = (longlong *)plVar17[1];
          iVar19 = iVar12;
        }
        else {
          if (((0xfffd < iVar1 + iVar21) || (0xfffd < iVar2 + iVar13)) || (0xfffd < iVar3 + iVar19))
          {
            FUN_140183e80(param_1,iVar21,iVar13);
            if ((longlong)DAT_140a6fda0 - (longlong)DAT_140a6fd98 >> 3 != 0) {
              DAT_140a6fda0 = DAT_140a6fd98;
            }
            iVar19 = 0;
            iVar21 = 0;
            iVar12 = 0;
            iVar13 = 0;
            local_res20 = 0;
            plVar23 = DAT_140a6fda0;
          }
          if (((iVar1 < 0xffff) && (iVar2 < 0xffff)) && (iVar3 < 0xffff)) {
            if (plVar23 == DAT_140a6fda8) {
              FUN_140045580(&DAT_140a6fd98,plVar23,local_60);
            }
            else {
              *plVar23 = (longlong)puVar4;
              DAT_140a6fda0 = DAT_140a6fda0 + 1;
              iVar12 = iVar19;
            }
            local_res20 = iVar13 + iVar2;
            iVar21 = iVar21 + iVar1;
            iVar12 = iVar12 + iVar3;
            plVar23 = DAT_140a6fda0;
            iVar13 = local_res20;
            goto LAB_140180f8d;
          }
          pcVar14 = "unknown";
          if (((longlong *)puVar4[2] != (longlong *)0x0) &&
             (lVar10 = (**(code **)(*(longlong *)puVar4[2] + 0x30))(), lVar10 != 0)) {
            pcVar14 = (char *)(**(code **)(*(longlong *)puVar4[2] + 0x30))();
          }
          FUN_140025760(" ERROR: render task for %s exceeded the dynamic geometry limits (%d pverts, %d lverts, %d indices)\r\n"
                        ,pcVar14,iVar1,iVar2,iVar3);
          if (plVar17[3] != 0) {
            plVar17 = (longlong *)plVar17[1];
            FUN_14020abe0(plVar9 + -1);
          }
          (**(code **)*puVar4)(puVar4,1);
          plVar23 = DAT_140a6fda0;
          iVar13 = local_res20;
          iVar19 = iVar12;
        }
      }
    }
    plVar9 = plVar9 + 9;
    local_88 = local_88 << 1 | (uint)((int)local_88 < 0);
    plVar20 = plVar20 + 9;
    local_68 = local_68 + -1;
  } while (local_68 != 0);
  FUN_140183e80(param_1,iVar21,iVar13,iVar19);
  if ((longlong)DAT_140a6fda0 - (longlong)DAT_140a6fd98 >> 3 != 0) {
    DAT_140a6fda0 = DAT_140a6fd98;
  }
  FUN_140197540();
  iVar12 = thunk_FUN_1401789f0();
  uVar7 = DAT_1407ffaf8;
  local_res10 = param_2;
  if (iVar12 == 0) {
    local_res10 = param_2 & 0xffffffbf;
  }
  plVar23 = &DAT_140a6fd20;
  uVar15 = 0;
  uVar22 = 1;
  do {
    iVar12 = (int)uVar15;
    if ((local_res10 & uVar22) != 0) {
      if ((longlong *)local_80[1] != local_80) {
        *(int *)(param_1 + 3) = iVar12;
        plVar9 = (longlong *)*plVar23;
        if (plVar9 != (longlong *)0x0) {
          (**(code **)(*plVar9 + 0x40))(plVar9,0,0);
          (**(code **)(*(longlong *)*plVar23 + 0x48))();
        }
        if (iVar12 == 6) {
          *(undefined4 *)(*param_1 + 0x38) = 0x3a83126f;
          FUN_140141260();
          FUN_14013d820(*param_1);
LAB_14018113f:
          cVar8 = FUN_14013ebf0();
          plVar9 = (longlong *)param_1[uVar15 * 9 + 0xd85f];
          if (cVar8 == '\0') {
            for (; plVar9 != param_1 + uVar15 * 9 + 0xd85e; plVar9 = (longlong *)plVar9[1]) {
              (**(code **)(*(longlong *)plVar9[3] + 0x10))();
            }
          }
          else {
            for (; plVar9 != param_1 + uVar15 * 9 + 0xd85e; plVar9 = (longlong *)plVar9[1]) {
              lVar10 = plVar9[3];
              plVar20 = *(longlong **)(lVar10 + 0x18);
              if ((plVar20 == (longlong *)0x0) ||
                 (puVar11 = (undefined *)(**(code **)(*plVar20 + 0x20))(plVar20),
                 puVar11 != &DAT_140a12a70)) {
                if (DAT_140a6fe00 == DAT_140a6fe08) {
                  FUN_140045580(&DAT_140a6fdf8,DAT_140a6fe00);
                }
                else {
                  *DAT_140a6fe00 = lVar10;
                  DAT_140a6fe00 = DAT_140a6fe00 + 1;
                }
              }
              else {
                FUN_1401b03a0(plVar20[6],lVar10);
              }
            }
            FUN_14018a6c0(param_1);
            uVar15 = 0;
            if ((longlong)DAT_140a6fe00 - (longlong)DAT_140a6fdf8 >> 3 != 0) {
              do {
                (**(code **)(*(longlong *)DAT_140a6fdf8[uVar15] + 0x10))
                          ((longlong *)DAT_140a6fdf8[uVar15],param_1);
                puVar4 = (undefined8 *)DAT_140a6fdf8[uVar15];
                if (puVar4 != (undefined8 *)0x0) {
                  (**(code **)*puVar4)(puVar4,1);
                }
                uVar15 = (ulonglong)((int)uVar15 + 1);
              } while (uVar15 < (ulonglong)((longlong)DAT_140a6fe00 - (longlong)DAT_140a6fdf8 >> 3))
              ;
            }
            if ((longlong)DAT_140a6fe00 - (longlong)DAT_140a6fdf8 >> 3 != 0) {
              DAT_140a6fe00 = DAT_140a6fdf8;
            }
          }
        }
        else {
          if ((iVar12 != 8) && (iVar12 != 0xb)) goto LAB_14018113f;
          FUN_140181800(param_1);
        }
        *(int *)(param_1 + 3) = iVar12;
        if ((longlong *)*plVar23 != (longlong *)0x0) {
          (**(code **)(*(longlong *)*plVar23 + 0x50))();
          (**(code **)(*(longlong *)*plVar23 + 0x48))();
          (**(code **)(*(longlong *)*plVar23 + 0x50))();
          (**(code **)(*(longlong *)*plVar23 + 0x58))();
        }
        if (iVar12 == 6) {
          *(undefined4 *)(*param_1 + 0x38) = 0;
          FUN_140141260();
          FUN_14013d820(*param_1);
          FUN_140183580(param_1);
          goto LAB_1401813a0;
        }
      }
      if (iVar12 == 8) {
        cVar8 = FUN_14013ec70();
        if ((cVar8 == '\0') || ((char)param_1[4] == '\0')) {
          bVar5 = false;
        }
        else {
          bVar5 = true;
        }
        cVar8 = FUN_14013ecb0();
        if ((cVar8 == '\0') || (*(char *)((longlong)param_1 + 0x21) == '\0')) {
          bVar6 = false;
        }
        else {
          bVar6 = true;
        }
        if ((((bVar5) || (bVar6)) && (FUN_140198380(DAT_140a6fdd8), bVar5)) &&
           (cVar8 = FUN_14013ec90(), cVar8 == '\0')) {
          FUN_140178010(0,0,1,0x8080ff,uVar7,0);
        }
      }
    }
LAB_1401813a0:
    uVar16 = 0;
    local_80 = local_80 + 9;
    uVar15 = (ulonglong)(iVar12 + 1U);
    uVar22 = uVar22 << 1 | (uint)((int)uVar22 < 0);
    plVar23 = plVar23 + 1;
    if (0xb < (int)(iVar12 + 1U)) {
      cVar8 = FUN_14013ec70();
      if (((cVar8 != '\0') && ((char)param_1[4] != '\0')) &&
         (cVar8 = FUN_14013ec90(), cVar8 == '\0')) {
        FUN_140198380(DAT_140a6fd18,0);
        FUN_1401b0280(DAT_140a6fe20,"SceneTexture",*(undefined8 *)(DAT_140a6fdd8 + 0x38));
        FUN_1401b0280(DAT_140a6fe20,"DistortionTexture",*(undefined8 *)(DAT_140a6fd18 + 0x38));
        (**(code **)(*DAT_140a6fe20 + 0x40))(DAT_140a6fe20,0,0);
        (**(code **)(*DAT_140a6fe20 + 0x48))(DAT_140a6fe20,0);
        FUN_140177dd0(DAT_140a6fe98);
        FUN_140177df0(0,DAT_140a6fe90,0,8);
        FUN_140177e90(5,0,2);
        (**(code **)(*DAT_140a6fe20 + 0x50))();
        (**(code **)(*DAT_140a6fe20 + 0x58))();
      }
      FUN_140183a30(param_1);
      plVar23 = DAT_140a6fe68;
      if (DAT_140a6fe68 != (longlong *)0x0) {
        *(undefined4 *)(DAT_140a6fe68 + 3) = 0;
        uVar15 = 0;
        plVar23[4] = 0;
        *(undefined4 *)(plVar23 + 5) = 0;
        plVar23[6] = 0;
        if (plVar23[1] - *plVar23 >> 3 != 0) {
          do {
            FUN_1401ace00(*(undefined8 *)(*plVar23 + uVar15 * 8));
            uVar15 = (ulonglong)((int)uVar15 + 1);
          } while (uVar15 < (ulonglong)(plVar23[1] - *plVar23 >> 3));
        }
      }
      lVar10 = DAT_140a6fe70;
      if (DAT_140a6fe70 != 0) {
        *(undefined4 *)(DAT_140a6fe70 + 0x20) = 0;
        uVar15 = 0;
        *(undefined8 *)(lVar10 + 0x28) = 0;
        *(undefined4 *)(lVar10 + 0x30) = 0;
        *(undefined8 *)(lVar10 + 0x38) = 0;
        if (*(longlong *)(lVar10 + 0x10) - *(longlong *)(lVar10 + 8) >> 3 != 0) {
          do {
            uVar18 = (ulonglong)((int)uVar15 + 1);
            *(undefined1 *)(*(longlong *)(*(longlong *)(lVar10 + 8) + uVar15 * 8) + 0x14) = 1;
            uVar15 = uVar18;
          } while (uVar18 < (ulonglong)
                            (*(longlong *)(lVar10 + 0x10) - *(longlong *)(lVar10 + 8) >> 3));
        }
      }
      lVar10 = DAT_140a6fe78;
      if (DAT_140a6fe78 != 0) {
        *(undefined4 *)(DAT_140a6fe78 + 0x20) = 0;
        *(undefined8 *)(lVar10 + 0x28) = 0;
        *(undefined4 *)(lVar10 + 0x30) = 0;
        *(undefined8 *)(lVar10 + 0x38) = 0;
        if (*(longlong *)(lVar10 + 0x10) - *(longlong *)(lVar10 + 8) >> 3 != 0) {
          do {
            uVar15 = (ulonglong)((int)uVar16 + 1);
            *(undefined1 *)(*(longlong *)(*(longlong *)(lVar10 + 8) + uVar16 * 8) + 0x14) = 1;
            uVar16 = uVar15;
          } while (uVar15 < (ulonglong)
                            (*(longlong *)(lVar10 + 0x10) - *(longlong *)(lVar10 + 8) >> 3));
        }
      }
      DAT_140a62740 = 0;
      return;
    }
  } while( true );
}



// ------------------------------------------------------------
// RVA: 0x13db20  Name: FUN_14013db20  Size: 489 bytes
// ------------------------------------------------------------

longlong * FUN_14013db20(LPCSTR param_1,char param_2,undefined1 param_3)

{
  char cVar1;
  longlong *plVar2;
  longlong lVar3;
  char *pcVar4;
  CHAR local_448 [272];
  char local_338 [272];
  char local_228 [272];
  char local_118 [272];
  
  pcVar4 = param_1;
  do {
    cVar1 = *pcVar4;
    pcVar4[(longlong)(local_338 + -(longlong)param_1)] = cVar1;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  pcVar4 = param_1;
  do {
    cVar1 = *pcVar4;
    pcVar4[(longlong)(local_228 + -(longlong)param_1)] = cVar1;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  cVar1 = FUN_14013f450(param_1,local_338,local_228);
  lVar3 = 0;
  do {
    pcVar4 = local_338 + lVar3;
    local_118[lVar3] = *pcVar4;
    lVar3 = lVar3 + 1;
  } while (*pcVar4 != '\0');
  if (cVar1 == '\0') {
    lstrcpyA(local_448,param_1);
    FUN_1407802c4(local_448,&DAT_14080329c);
    lstrcatA(local_448,".dds");
    lVar3 = 0;
    do {
      pcVar4 = local_448 + lVar3;
      local_338[lVar3] = *pcVar4;
      lVar3 = lVar3 + 1;
    } while (*pcVar4 != '\0');
    lVar3 = 0;
    do {
      pcVar4 = local_448 + lVar3;
      local_228[lVar3] = *pcVar4;
      lVar3 = lVar3 + 1;
    } while (*pcVar4 != '\0');
    cVar1 = FUN_14013f450(local_448,local_338,local_228);
    if (cVar1 == '\0') {
      FUN_140025760("*** WARNING *** alGraphicsDriver::Load_Texture - Failed to find texture (tga/dds) %s\n"
                    ,local_118);
      if (param_2 == '\0') {
        return (longlong *)0x0;
      }
      if (DAT_140a626b0 != (longlong *)0x0) {
        return DAT_140a626b0;
      }
      plVar2 = (longlong *)FUN_140186210();
      if (DAT_140a626b0 == plVar2) {
        return DAT_140a626b0;
      }
      if (DAT_140a626b0 != (longlong *)0x0) {
        (**(code **)(*DAT_140a626b0 + 8))(DAT_140a626b0);
      }
      if (plVar2 != (longlong *)0x0) {
        DAT_140a626b0 = plVar2;
        (**(code **)*plVar2)(plVar2);
        return DAT_140a626b0;
      }
      DAT_140a626b0 = plVar2;
      return (longlong *)0x0;
    }
  }
  lVar3 = FUN_140769c58(0x98);
  if (lVar3 == 0) {
    return (longlong *)0x0;
  }
  plVar2 = (longlong *)FUN_14018d090(lVar3,local_228,local_338,DAT_140a626c0,param_3);
  return plVar2;
}



// ------------------------------------------------------------
// RVA: 0x13dd10  Name: FUN_14013dd10  Size: 240 bytes
// ------------------------------------------------------------

longlong * FUN_14013dd10(longlong *param_1,char param_2,undefined1 param_3)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  
  cVar1 = (**(code **)(*param_1 + 8))(param_1,0,0);
  if (cVar1 == '\0') {
    uVar4 = (**(code **)(*param_1 + 0x50))(param_1);
    FUN_140025760("*** WARNING *** alGraphicsDriver::Load_Texture - Failed to find texture %s\n",
                  uVar4);
    if (param_2 != '\0') {
      if (DAT_140a626b0 != (longlong *)0x0) {
        return DAT_140a626b0;
      }
      plVar3 = (longlong *)FUN_140186210();
      if (DAT_140a626b0 == plVar3) {
        return DAT_140a626b0;
      }
      if (DAT_140a626b0 != (longlong *)0x0) {
        (**(code **)(*DAT_140a626b0 + 8))(DAT_140a626b0);
      }
      if (plVar3 == (longlong *)0x0) {
        DAT_140a626b0 = plVar3;
        return (longlong *)0x0;
      }
      DAT_140a626b0 = plVar3;
      (**(code **)*plVar3)(plVar3);
      return DAT_140a626b0;
    }
  }
  else {
    (**(code **)(*param_1 + 0x20))();
    lVar2 = FUN_140769c58(0x98);
    if (lVar2 != 0) {
      plVar3 = (longlong *)FUN_14018d3c0(lVar2,param_1,DAT_140a626c0,param_3);
      return plVar3;
    }
  }
  return (longlong *)0x0;
}




// ================================================================
// 1.4C — Lua thread model
// (via xref to 'LuaScriptThread:' string)
// ================================================================

// String "LuaScriptThread: Main State" @ 0x140855cd0
//   xref from FUN_0x248190 (call site 0x2482b1)
//   xref from FUN_0x248620 (call site 0x24872e)
// String "LuaScriptThread: %s" @ 0x140855cf0
//   xref from FUN_0x248190 (call site 0x24827f)
//   xref from FUN_0x248620 (call site 0x2486fc)
// String "LuaScriptThread: Main State\n" @ 0x1408567b8
//   xref from FUN_0x251840 (call site 0x2519c5)
//   xref from FUN_0x251840 (call site 0x251918)
// String "LuaScriptThread: " @ 0x1408567d8
//   xref from FUN_0x251840 (call site 0x251b9f)
// String "LuaScriptThread: Main State" @ 0x140855cd0
//   xref from FUN_0x248190 (call site 0x2482b1)
//   xref from FUN_0x248620 (call site 0x24872e)
// String "LuaScriptThread: %s" @ 0x140855cf0
//   xref from FUN_0x248190 (call site 0x24827f)
//   xref from FUN_0x248620 (call site 0x2486fc)
// String "LuaScriptThread: Main State\n" @ 0x1408567b8
//   xref from FUN_0x251840 (call site 0x2519c5)
//   xref from FUN_0x251840 (call site 0x251918)
// String "LuaScriptThread: " @ 0x1408567d8
//   xref from FUN_0x251840 (call site 0x251b9f)
// ------------------------------------------------------------
// RVA: 0x248190  Name: FUN_140248190  Size: 644 bytes
// ------------------------------------------------------------

void FUN_140248190(longlong param_1,char *param_2,undefined8 param_3,undefined8 param_4)

{
  undefined2 *puVar1;
  ulonglong *puVar2;
  undefined8 *****pppppuVar3;
  undefined8 ******ppppppuVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  longlong lVar7;
  undefined8 *puVar8;
  longlong *plVar9;
  undefined8 local_res18;
  undefined8 local_res20;
  undefined8 *****local_468 [2];
  ulonglong local_458;
  ulonglong local_450;
  longlong local_448 [2];
  undefined8 local_438;
  ulonglong local_430;
  char local_428 [1032];
  
  if (param_2 == (char *)0x0) {
    return;
  }
  if (DAT_140b09d40 == (code *)0x0) {
    *(undefined1 *)(param_1 + 0x121) = 1;
    return;
  }
  local_res18 = param_3;
  local_res20 = param_4;
  puVar2 = (ulonglong *)FUN_140022b20();
  common_vsprintf<class___crt_stdio_output::standard_base,char>
            (*puVar2 | 1,local_428,0x3ff,param_2,(__crt_locale_pointers *)0x0,(char *)&local_res18);
  local_468[0] = (undefined8 ******)0x0;
  pppppuVar3 = (undefined8 *****)FUN_140769c58(0x2037);
  if (pppppuVar3 == (undefined8 *****)0x0) goto LAB_14024840f;
  ppppppuVar4 = (undefined8 ******)((longlong)pppppuVar3 + 0x27U & 0xffffffffffffffe0);
  ppppppuVar4[-1] = pppppuVar3;
  local_450 = 0x200f;
  *(undefined1 *)ppppppuVar4 = local_468[0]._0_1_;
  local_458 = 0;
  local_468[0] = ppppppuVar4;
  if (*(int *)(param_1 + 0x60) == -1) {
LAB_1402482b1:
    (*DAT_140b09d40)("LuaScriptThread: Main State");
    uVar5 = *(undefined8 *)(param_1 + 0x58);
  }
  else {
    lVar7 = (longlong)*(int *)(param_1 + 0x60) * 0x40;
    if (*(longlong *)(*(longlong *)(param_1 + 0x40) + lVar7) == 0) goto LAB_1402482b1;
    puVar8 = (undefined8 *)(*(longlong *)(param_1 + 0x40) + 0x10 + lVar7);
    if (0xf < (ulonglong)puVar8[3]) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140200cd0(local_468,"LuaScriptThread: %s",puVar8);
    ppppppuVar4 = local_468;
    if (0xf < local_450) {
      ppppppuVar4 = (undefined8 ******)local_468[0];
    }
    (*DAT_140b09d40)(ppppppuVar4);
    uVar5 = *(undefined8 *)(lVar7 + *(longlong *)(param_1 + 0x40));
  }
  FUN_140252be0(uVar5,0);
  plVar9 = (longlong *)(param_1 + 0x68);
  if (0xf < *(ulonglong *)(param_1 + 0x80)) {
    plVar9 = (longlong *)*plVar9;
  }
  FUN_140200cd0(local_468,"LuaScript: \"%s\", Error: %s",plVar9,local_428);
  ppppppuVar4 = local_468;
  if (0xf < local_450) {
    ppppppuVar4 = (undefined8 ******)local_468[0];
  }
  (*DAT_140b09d40)(ppppppuVar4);
  local_430 = 0xf;
  local_438 = 1;
  local_448[0] = 10;
  if (local_450 == local_458) {
    FUN_14004f860(local_468,1,0,local_448,1);
    if (0xf < local_430) {
      uVar6 = local_430 + 1;
      lVar7 = local_448[0];
      if (0xfff < uVar6) {
        lVar7 = *(longlong *)(local_448[0] + -8);
        uVar6 = local_430 + 0x28;
        if (0x1f < (local_448[0] - lVar7) - 8U) goto LAB_14024840f;
      }
      thunk_FUN_1407864b8(lVar7,uVar6);
    }
  }
  else {
    ppppppuVar4 = local_468;
    if (0xf < local_450) {
      ppppppuVar4 = (undefined8 ******)local_468[0];
    }
    puVar1 = (undefined2 *)((longlong)ppppppuVar4 + local_458);
    local_458 = local_458 + 1;
    *puVar1 = 10;
  }
  FUN_140250040(local_468,0);
  *(undefined1 *)(param_1 + 0x121) = 1;
  if (0xf < local_450) {
    uVar6 = local_450 + 1;
    ppppppuVar4 = (undefined8 ******)local_468[0];
    if (0xfff < uVar6) {
      ppppppuVar4 = (undefined8 ******)local_468[0][-1];
      uVar6 = local_450 + 0x28;
      if (0x1f < (ulonglong)((longlong)local_468[0] + (-8 - (longlong)ppppppuVar4))) {
LAB_14024840f:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
    }
    thunk_FUN_1407864b8(ppppppuVar4,uVar6);
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x248620  Name: FUN_140248620  Size: 618 bytes
// ------------------------------------------------------------

void FUN_140248620(longlong param_1,char *param_2,undefined8 param_3,undefined8 param_4)

{
  undefined2 *puVar1;
  ulonglong *puVar2;
  undefined8 *****pppppuVar3;
  undefined8 ******ppppppuVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  longlong lVar7;
  undefined8 *puVar8;
  longlong *plVar9;
  undefined8 local_res18;
  undefined8 local_res20;
  undefined8 *****local_468 [2];
  ulonglong local_458;
  ulonglong local_450;
  longlong local_448 [2];
  undefined8 local_438;
  ulonglong local_430;
  char local_428 [1032];
  
  if (param_2 == (char *)0x0) {
    return;
  }
  if (DAT_140b09d48 == (code *)0x0) {
    return;
  }
  local_res18 = param_3;
  local_res20 = param_4;
  puVar2 = (ulonglong *)FUN_140022b20();
  common_vsprintf<class___crt_stdio_output::standard_base,char>
            (*puVar2 | 1,local_428,0x3ff,param_2,(__crt_locale_pointers *)0x0,(char *)&local_res18);
  local_468[0] = (undefined8 ******)0x0;
  pppppuVar3 = (undefined8 *****)FUN_140769c58(0x2037);
  if (pppppuVar3 == (undefined8 *****)0x0) goto LAB_140248885;
  ppppppuVar4 = (undefined8 ******)((longlong)pppppuVar3 + 0x27U & 0xffffffffffffffe0);
  ppppppuVar4[-1] = pppppuVar3;
  local_450 = 0x200f;
  *(undefined1 *)ppppppuVar4 = local_468[0]._0_1_;
  local_458 = 0;
  local_468[0] = ppppppuVar4;
  if (*(int *)(param_1 + 0x60) == -1) {
LAB_14024872e:
    (*DAT_140b09d48)("LuaScriptThread: Main State");
    uVar5 = *(undefined8 *)(param_1 + 0x58);
  }
  else {
    lVar7 = (longlong)*(int *)(param_1 + 0x60) * 0x40;
    if (*(longlong *)(lVar7 + *(longlong *)(param_1 + 0x40)) == 0) goto LAB_14024872e;
    puVar8 = (undefined8 *)(*(longlong *)(param_1 + 0x40) + 0x10 + lVar7);
    if (0xf < (ulonglong)puVar8[3]) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140200cd0(local_468,"LuaScriptThread: %s",puVar8);
    ppppppuVar4 = local_468;
    if (0xf < local_450) {
      ppppppuVar4 = (undefined8 ******)local_468[0];
    }
    (*DAT_140b09d48)(ppppppuVar4);
    uVar5 = *(undefined8 *)(lVar7 + *(longlong *)(param_1 + 0x40));
  }
  FUN_140252be0(uVar5,1);
  plVar9 = (longlong *)(param_1 + 0x68);
  if (0xf < *(ulonglong *)(param_1 + 0x80)) {
    plVar9 = (longlong *)*plVar9;
  }
  FUN_140200cd0(local_468,"LuaScript: \"%s\", Warning: %s",plVar9,local_428);
  ppppppuVar4 = local_468;
  if (0xf < local_450) {
    ppppppuVar4 = (undefined8 ******)local_468[0];
  }
  (*DAT_140b09d48)(ppppppuVar4);
  local_430 = 0xf;
  local_438 = 1;
  local_448[0] = 10;
  if (local_450 == local_458) {
    FUN_14004f860(local_468,1,0,local_448,1);
    if (0xf < local_430) {
      uVar6 = local_430 + 1;
      lVar7 = local_448[0];
      if (0xfff < uVar6) {
        lVar7 = *(longlong *)(local_448[0] + -8);
        uVar6 = local_430 + 0x28;
        if (0x1f < (local_448[0] - lVar7) - 8U) goto LAB_140248885;
      }
      thunk_FUN_1407864b8(lVar7,uVar6);
    }
  }
  else {
    ppppppuVar4 = local_468;
    if (0xf < local_450) {
      ppppppuVar4 = (undefined8 ******)local_468[0];
    }
    puVar1 = (undefined2 *)((longlong)ppppppuVar4 + local_458);
    local_458 = local_458 + 1;
    *puVar1 = 10;
  }
  FUN_140250040(local_468,0);
  if (0xf < local_450) {
    uVar6 = local_450 + 1;
    ppppppuVar4 = (undefined8 ******)local_468[0];
    if (0xfff < uVar6) {
      ppppppuVar4 = (undefined8 ******)local_468[0][-1];
      uVar6 = local_450 + 0x28;
      if (0x1f < (ulonglong)((longlong)local_468[0] + (-8 - (longlong)ppppppuVar4))) {
LAB_140248885:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
    }
    thunk_FUN_1407864b8(ppppppuVar4,uVar6);
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x251840  Name: FUN_140251840  Size: 1671 bytes
// ------------------------------------------------------------

undefined8 * FUN_140251840(longlong param_1)

{
  longlong *plVar1;
  undefined8 uVar2;
  char *****pppppcVar3;
  longlong lVar4;
  ulonglong uVar5;
  char ****ppppcVar6;
  undefined8 *****pppppuVar7;
  uint uVar8;
  ulonglong uVar9;
  char *pcVar10;
  char *pcVar11;
  ulonglong uVar12;
  undefined8 *puVar13;
  char *****pppppcVar14;
  undefined8 *****pppppuVar15;
  ulonglong uVar16;
  char ****ppppcVar17;
  uint local_res8;
  char ****local_88 [2];
  ulonglong local_78;
  ulonglong uStack_70;
  undefined8 ****local_68 [2];
  ulonglong local_58;
  ulonglong local_50;
  undefined8 ****local_48;
  char ***pppcStack_40;
  char ***local_38;
  char ***pppcStack_30;
  
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
               0xd4) < DAT_140b09f00) && (_Init_thread_header(&DAT_140b09f00), DAT_140b09f00 == -1))
  {
    atexit(FUN_1407ee120);
    _Init_thread_footer(&DAT_140b09f00);
  }
  pcVar10 = (char *)0x0;
  pcVar11 = (char *)&DAT_140a15838;
  if (0xf < DAT_140a15850) {
    pcVar11 = DAT_140a15838;
  }
  DAT_140a15848 = 0;
  *pcVar11 = '\0';
  uVar16 = DAT_140a15850;
  local_68[0] = (undefined8 *****)0x0;
  local_58 = 0;
  local_50 = 0xf;
  if (*(longlong *)(param_1 + 0x58) != 0) {
    if (DAT_140a15850 < 0x1c) {
      if (0x7fffffffffffffff - (DAT_140a15850 >> 1) < DAT_140a15850) {
        uVar12 = 0x7fffffffffffffff;
        uVar9 = 0x8000000000000027;
LAB_140251956:
        lVar4 = FUN_140769c58(uVar9);
        if (lVar4 == 0) goto LAB_140251e78;
        pcVar10 = (char *)(lVar4 + 0x27U & 0xffffffffffffffe0);
        *(longlong *)(pcVar10 + -8) = lVar4;
      }
      else {
        uVar9 = (DAT_140a15850 >> 1) + DAT_140a15850;
        uVar12 = 0x1f;
        if (0x1f < uVar9) {
          uVar12 = uVar9;
        }
        if (0xfff < uVar12 + 1) {
          uVar9 = uVar12 + 0x28;
          if (uVar9 <= uVar12 + 1) {
                    /* WARNING: Subroutine does not return */
            FUN_1400225f0();
          }
          goto LAB_140251956;
        }
        if (uVar12 != 0xffffffffffffffff) {
          pcVar10 = (char *)FUN_140769c58();
        }
      }
      uVar2 = s_LuaScriptThread__Main_State_1408567b8._8_8_;
      DAT_140a15848 = 0x1c;
      DAT_140a15850 = uVar12;
      *(undefined8 *)pcVar10 = s_LuaScriptThread__Main_State_1408567b8._0_8_;
      *(undefined8 *)(pcVar10 + 8) = uVar2;
      *(undefined8 *)(pcVar10 + 0x10) = s_LuaScriptThread__Main_State_1408567b8._16_8_;
      *(undefined4 *)(pcVar10 + 0x18) = s_LuaScriptThread__Main_State_1408567b8._24_4_;
      pcVar10[0x1c] = '\0';
      if (0xf < uVar16) {
        uVar9 = uVar16 + 1;
        pcVar11 = DAT_140a15838;
        if (0xfff < uVar9) {
          pcVar11 = *(char **)(DAT_140a15838 + -8);
          uVar9 = uVar16 + 0x28;
          if ((char *)0x1f < DAT_140a15838 + (-8 - (longlong)pcVar11)) goto LAB_140251e78;
        }
        thunk_FUN_1407864b8(pcVar11,uVar9);
      }
    }
    else {
      DAT_140a15848 = 0x1c;
      pcVar11 = (char *)&DAT_140a15838;
      if (0xf < DAT_140a15850) {
        pcVar11 = DAT_140a15838;
      }
      FUN_14076bd80(pcVar11,"LuaScriptThread: Main State\n");
      pcVar11[0x1c] = '\0';
      pcVar10 = DAT_140a15838;
    }
    DAT_140a15838 = pcVar10;
    FUN_140252a60(*(undefined8 *)(param_1 + 0x58),local_68);
    uVar16 = local_58;
    pppppuVar7 = local_68;
    if (0xf < local_50) {
      pppppuVar7 = (undefined8 *****)local_68[0];
    }
    if (DAT_140a15850 - DAT_140a15848 < local_58) {
      FUN_14004f860(&DAT_140a15838,local_58,0,pppppuVar7,local_58);
    }
    else {
      pcVar11 = (char *)&DAT_140a15838;
      if (0xf < DAT_140a15850) {
        pcVar11 = DAT_140a15838;
      }
      pcVar11 = pcVar11 + DAT_140a15848;
      DAT_140a15848 = DAT_140a15848 + local_58;
      FUN_14076bd80(pcVar11,pppppuVar7);
      pcVar11[uVar16] = '\0';
    }
  }
  uVar9 = 0;
  lVar4 = *(longlong *)(param_1 + 0x40);
  local_res8 = 0;
  uVar16 = uVar9;
  if (0 < (int)(*(longlong *)(param_1 + 0x48) - lVar4 >> 6)) {
    do {
      uVar8 = (uint)uVar9;
      if (*(longlong *)(lVar4 + uVar16) != 0) {
        puVar13 = (undefined8 *)(uVar16 + 0x10 + lVar4);
        plVar1 = puVar13 + 2;
        if (0x7fffffffffffffffU - *plVar1 < 0x11) {
                    /* WARNING: Subroutine does not return */
          FUN_140022610();
        }
        if (0xf < (ulonglong)puVar13[3]) {
          puVar13 = (undefined8 *)*puVar13;
        }
        uVar9 = *plVar1 + 0x11;
        local_88[0] = (char ****)0x0;
        uVar12 = 0xf;
        local_78 = 0;
        uStack_70 = 0;
        pppppcVar14 = local_88;
        pppppcVar3 = (char *****)local_88[0];
        if (0xf < uVar9) {
          uVar12 = uVar9 | 0xf;
          if (uVar12 < 0x8000000000000000) {
            if (uVar12 < 0x16) {
              uVar12 = 0x16;
            }
            if (uVar12 + 1 < 0x1000) {
              pppppcVar14 = (char *****)0x0;
              pppppcVar3 = (char *****)0x0;
              if (uVar12 != 0xffffffffffffffff) {
                pppppcVar14 = (char *****)FUN_140769c58();
                pppppcVar3 = pppppcVar14;
              }
              goto LAB_140251b9f;
            }
            uVar5 = uVar12 + 0x28;
            if (uVar5 <= uVar12 + 1) {
                    /* WARNING: Subroutine does not return */
              FUN_1400225f0();
            }
          }
          else {
            uVar12 = 0x7fffffffffffffff;
            uVar5 = 0x8000000000000027;
          }
          ppppcVar6 = (char ****)FUN_140769c58(uVar5);
          if (ppppcVar6 == (char ****)0x0) goto LAB_140251ec0;
          pppppcVar14 = (char *****)((longlong)ppppcVar6 + 0x27U & 0xffffffffffffffe0);
          pppppcVar14[-1] = ppppcVar6;
          pppppcVar3 = pppppcVar14;
        }
LAB_140251b9f:
        local_88[0] = (char ****)pppppcVar3;
        local_78 = uVar9;
        uStack_70 = uVar12;
        *pppppcVar14 = (char ****)s_LuaScriptThread__1408567d8._0_8_;
        pppppcVar14[1] = (char ****)s_LuaScriptThread__1408567d8._8_8_;
        *(char *)(pppppcVar14 + 2) = s_LuaScriptThread__1408567d8[0x10];
        FUN_14076bd80((char *)((longlong)pppppcVar14 + 0x11),puVar13);
        *(char *)((longlong)pppppcVar14 + uVar9) = '\0';
        uVar9 = local_78;
        if (uStack_70 == local_78) {
          pppppcVar14 = (char *****)FUN_14004f860(local_88,1,0,&DAT_140819c5c,1);
        }
        else {
          local_78 = local_78 + 1;
          pppppcVar14 = local_88;
          if (0xf < uStack_70) {
            pppppcVar14 = (char *****)local_88[0];
          }
          ((char *)((longlong)pppppcVar14 + uVar9))[0] = '\n';
          ((char *)((longlong)pppppcVar14 + uVar9))[1] = '\0';
          pppppcVar14 = local_88;
        }
        pppppuVar7 = (undefined8 *****)*pppppcVar14;
        ppppcVar6 = pppppcVar14[2];
        ppppcVar17 = pppppcVar14[3];
        pppppcVar14[2] = (char ****)0x0;
        pppppcVar14[3] = (char ****)0xf;
        *(undefined1 *)pppppcVar14 = 0;
        pppppuVar15 = &local_48;
        if ((char ****)0xf < ppppcVar17) {
          pppppuVar15 = pppppuVar7;
        }
        local_48 = pppppuVar7;
        pppcStack_40 = (char ***)pppppcVar14[1];
        local_38 = (char ***)ppppcVar6;
        pppcStack_30 = (char ***)ppppcVar17;
        if ((char ****)(DAT_140a15850 - DAT_140a15848) < ppppcVar6) {
          FUN_14004f860(&DAT_140a15838,ppppcVar6,0,pppppuVar15,ppppcVar6);
          pppppuVar7 = (undefined8 *****)local_48;
          ppppcVar17 = (char ****)pppcStack_30;
        }
        else {
          pcVar11 = (char *)&DAT_140a15838;
          if (0xf < DAT_140a15850) {
            pcVar11 = DAT_140a15838;
          }
          pcVar11 = pcVar11 + DAT_140a15848;
          DAT_140a15848 = DAT_140a15848 + (longlong)ppppcVar6;
          FUN_14076bd80(pcVar11,pppppuVar15);
          pcVar11[(longlong)ppppcVar6] = '\0';
        }
        if ((char ****)0xf < ppppcVar17) {
          ppppcVar6 = (char ****)((longlong)ppppcVar17 + 1);
          pppppuVar15 = pppppuVar7;
          if ((char ****)0xfff < ppppcVar6) {
            pppppuVar15 = (undefined8 *****)pppppuVar7[-1];
            ppppcVar6 = ppppcVar17 + 5;
            if (0x1f < (ulonglong)((longlong)pppppuVar7 + (-8 - (longlong)pppppuVar15)))
            goto LAB_140251ec0;
          }
          thunk_FUN_1407864b8(pppppuVar15,ppppcVar6);
        }
        if (0xf < uStack_70) {
          uVar9 = uStack_70 + 1;
          pppppcVar14 = (char *****)local_88[0];
          if (0xfff < uVar9) {
            pppppcVar14 = (char *****)local_88[0][-1];
            uVar9 = uStack_70 + 0x28;
            if ((char *)0x1f < (char *)((longlong)local_88[0] + (-8 - (longlong)pppppcVar14))) {
LAB_140251ec0:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
          }
          thunk_FUN_1407864b8(pppppcVar14,uVar9);
        }
        FUN_140252a60(*(undefined8 *)(uVar16 + *(longlong *)(param_1 + 0x40)),local_68);
        uVar9 = local_58;
        pppppuVar7 = local_68;
        if (0xf < local_50) {
          pppppuVar7 = (undefined8 *****)local_68[0];
        }
        uVar8 = local_res8;
        if (DAT_140a15850 - DAT_140a15848 < local_58) {
          FUN_14004f860(&DAT_140a15838,local_58,0,pppppuVar7,local_58);
        }
        else {
          pcVar11 = (char *)&DAT_140a15838;
          if (0xf < DAT_140a15850) {
            pcVar11 = DAT_140a15838;
          }
          pcVar11 = pcVar11 + DAT_140a15848;
          DAT_140a15848 = DAT_140a15848 + local_58;
          FUN_14076bd80(pcVar11,pppppuVar7);
          pcVar11[uVar9] = '\0';
        }
      }
      lVar4 = *(longlong *)(param_1 + 0x40);
      local_res8 = uVar8 + 1;
      uVar9 = (ulonglong)local_res8;
      uVar16 = uVar16 + 0x40;
    } while ((int)local_res8 < (int)(*(longlong *)(param_1 + 0x48) - lVar4 >> 6));
  }
  if (0xf < local_50) {
    uVar16 = local_50 + 1;
    pppppuVar7 = (undefined8 *****)local_68[0];
    if (0xfff < uVar16) {
      pppppuVar7 = (undefined8 *****)local_68[0][-1];
      uVar16 = local_50 + 0x28;
      if (0x1f < (ulonglong)((longlong)local_68[0] + (-8 - (longlong)pppppuVar7))) {
LAB_140251e78:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
    }
    thunk_FUN_1407864b8(pppppuVar7,uVar16);
  }
  return &DAT_140a15838;
}



