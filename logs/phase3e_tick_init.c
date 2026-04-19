
// ================================================================
// TICK-SLOT INITIALIZERS
// ================================================================

// ------------------------------------------------------------
// RVA: 0x322780  Name: FUN_140322780  Size: 44 bytes
// ------------------------------------------------------------

void FUN_140322780(void)

{
  FUN_140321830(&DAT_140b2b300,1);
  DAT_140a17250 = 0;
  DAT_140b2b398 = 0;
  DAT_140b2b3a0 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x3207c0  Name: FUN_1403207c0  Size: 22 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403207c0(void)

{
  _DAT_140a171f0 = 0;
  _DAT_140b2b2e0 = 0;
  _DAT_140b2b2e8 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x321830  Name: FUN_140321830  Size: 1103 bytes
// ------------------------------------------------------------

void FUN_140321830(longlong param_1,char param_2)

{
  ulonglong *puVar1;
  undefined8 *puVar2;
  ulonglong *puVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined4 uVar6;
  char cVar7;
  uint uVar8;
  HANDLE pvVar9;
  ulonglong uVar10;
  longlong lVar11;
  ulonglong *puVar12;
  int iVar13;
  ulonglong *puVar14;
  longlong *plVar15;
  ulonglong *puVar16;
  undefined8 *puVar17;
  ulonglong uVar18;
  longlong lVar19;
  int iVar20;
  uint uVar21;
  undefined **local_68;
  ulonglong *local_60;
  undefined8 local_58;
  
  uVar6 = DAT_1407ffaf8;
  puVar17 = *(undefined8 **)(param_1 + 0x60);
  if (puVar17 != *(undefined8 **)(param_1 + 0x68)) {
    local_68 = DynamicVectorClass<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>_>
               ::vftable;
    local_60 = (ulonglong *)0x0;
    local_58 = 0;
    do {
      puVar16 = (ulonglong *)0x0;
      puVar2 = (undefined8 *)*puVar17;
      if (puVar2 != (undefined8 *)0x0) {
        if ((*(char *)(puVar2 + 1) == '\x01') &&
           (cVar7 = FUN_14022ccf0(&DAT_140a7dfe0,*(undefined4 *)(puVar2 + 2)), cVar7 == '\x01')) {
          plVar15 = (longlong *)*puVar2;
          if (0xf < (ulonglong)plVar15[3]) {
            plVar15 = (longlong *)*plVar15;
          }
          FUN_140025760("SpeechEventManager: Event %s: Physical playback killed by System_Kill_All_Active_Events().\n"
                        ,plVar15);
          FUN_14022cd40(&DAT_140a7dfe0,*(undefined4 *)(puVar2 + 2),uVar6);
        }
        plVar15 = (longlong *)*puVar2;
        if (0xf < (ulonglong)plVar15[3]) {
          plVar15 = (longlong *)*plVar15;
        }
        FUN_140025760("SpeechEventManager: Event %s: Adding to killed name list for possible Story Mode callbacks (callback flag = %d).\n"
                      ,plVar15,param_2);
        puVar3 = (ulonglong *)*puVar2;
        iVar20 = (int)(local_58._4_4_ * 2) >> 1;
        iVar13 = (uint)local_58;
        if (iVar20 <= (int)(uint)local_58) {
          iVar13 = iVar20 / 10;
          if (iVar20 / 10 < 10) {
            iVar13 = 10;
          }
          uVar21 = iVar13 + iVar20;
          if (uVar21 == 0) {
            puVar14 = (ulonglong *)(local_58 & 0xffffffff);
            if (local_60 != (ulonglong *)0x0) {
              if ((longlong)local_58 < 0) {
                pvVar9 = GetProcessHeap();
                HeapFree(pvVar9,0,local_60);
              }
              else {
                FUN_14001eef0(local_60,3);
              }
              local_58 = (ulonglong)(local_58._4_4_ & 0x80000000) << 0x20;
              local_60 = (ulonglong *)0x0;
              puVar14 = puVar16;
            }
          }
          else {
            uVar18 = (ulonglong)(int)uVar21;
            if (local_60 == (ulonglong *)0x0) {
              if ((longlong)local_58 < 0) {
                pvVar9 = GetProcessHeap();
                local_60 = HeapAlloc(pvVar9,0,uVar18 << 5);
              }
              else {
                auVar4._8_8_ = 0;
                auVar4._0_8_ = uVar18;
                uVar10 = SUB168(ZEXT816(0x20) * auVar4,0);
                if (SUB168(ZEXT816(0x20) * auVar4,8) != 0) {
                  uVar10 = 0xffffffffffffffff;
                }
                lVar11 = uVar10 + 8;
                if (0xfffffffffffffff7 < uVar10) {
                  lVar11 = -1;
                }
                puVar12 = (ulonglong *)thunk_FUN_140769c58(lVar11);
                if (puVar12 == (ulonglong *)0x0) {
                  local_60 = (ulonglong *)0x0;
                }
                else {
                  local_60 = puVar12 + 1;
                  *puVar12 = uVar18;
                  puVar12 = local_60;
                  do {
                    *puVar12 = 0;
                    puVar12[2] = 0;
                    puVar12[3] = 0xf;
                    puVar12 = puVar12 + 4;
                    uVar18 = uVar18 - 1;
                  } while (uVar18 != 0);
                }
              }
              local_58 = (ulonglong)(local_58._4_4_ ^ (local_58._4_4_ ^ uVar21) & 0x7fffffff) <<
                         0x20;
              puVar14 = puVar16;
            }
            else {
              if ((longlong)local_58 < 0) {
                pvVar9 = GetProcessHeap();
                puVar12 = HeapReAlloc(pvVar9,0,local_60,uVar18 << 5);
              }
              else {
                auVar5._8_8_ = 0;
                auVar5._0_8_ = uVar18;
                uVar10 = SUB168(ZEXT816(0x20) * auVar5,0);
                if (SUB168(ZEXT816(0x20) * auVar5,8) != 0) {
                  uVar10 = 0xffffffffffffffff;
                }
                lVar11 = uVar10 + 8;
                if (0xfffffffffffffff7 < uVar10) {
                  lVar11 = -1;
                }
                puVar14 = (ulonglong *)thunk_FUN_140769c58(lVar11);
                puVar12 = puVar16;
                if (puVar14 != (ulonglong *)0x0) {
                  puVar12 = puVar14 + 1;
                  *puVar14 = uVar18;
                  puVar14 = puVar12;
                  do {
                    *puVar14 = 0;
                    puVar14[2] = 0;
                    puVar14[3] = 0xf;
                    puVar14 = puVar14 + 4;
                    uVar18 = uVar18 - 1;
                  } while (uVar18 != 0);
                }
                uVar8 = (int)(local_58._4_4_ * 2) >> 1;
                if ((int)uVar21 < (int)uVar8) {
                  uVar8 = uVar21;
                }
                lVar11 = (longlong)(int)uVar8;
                puVar14 = local_60;
                if (0 < (int)uVar8) {
                  lVar19 = 0;
                  do {
                    puVar1 = (ulonglong *)(lVar19 + (longlong)puVar14);
                    if (puVar12 + (longlong)(int)puVar16 * 4 != puVar1) {
                      puVar14 = puVar1;
                      if (0xf < puVar1[3]) {
                        puVar14 = (ulonglong *)*puVar1;
                      }
                      FUN_140022730(puVar12 + (longlong)(int)puVar16 * 4,puVar14,puVar1[2]);
                      puVar14 = local_60;
                    }
                    puVar16 = (ulonglong *)(ulonglong)((int)puVar16 + 1);
                    lVar19 = lVar19 + 0x20;
                    lVar11 = lVar11 + -1;
                  } while (lVar11 != 0);
                }
                if (puVar14 != (ulonglong *)0x0) {
                  FUN_14001eef0(puVar14,3);
                }
              }
              local_58 = CONCAT44(local_58._4_4_ ^ (local_58._4_4_ ^ uVar21) & 0x7fffffff,
                                  (uint)local_58);
              if ((int)(uint)local_58 < (int)uVar21) {
                uVar21 = (uint)local_58;
              }
              puVar14 = (ulonglong *)(ulonglong)uVar21;
              local_60 = puVar12;
            }
          }
          iVar13 = (int)puVar14;
        }
        local_58 = CONCAT44(local_58._4_4_,iVar13 + 1);
        if (local_60 + (longlong)iVar13 * 4 != puVar3) {
          puVar16 = puVar3;
          if (0xf < puVar3[3]) {
            puVar16 = (ulonglong *)*puVar3;
          }
          FUN_140022730(local_60 + (longlong)iVar13 * 4,puVar16,puVar3[2]);
        }
        *puVar2 = 0;
        *(undefined8 *)((longlong)puVar2 + 0xc) = 0xffffffffffffffff;
        *(undefined1 *)(puVar2 + 1) = 0;
        *(undefined4 *)((longlong)puVar2 + 0x14) = 0xffffffff;
        thunk_FUN_1407864b8(puVar2,0x18);
        FUN_14076bd80(puVar17,puVar17 + 1,*(longlong *)(param_1 + 0x68) - (longlong)(puVar17 + 1));
        *(longlong *)(param_1 + 0x68) = *(longlong *)(param_1 + 0x68) + -8;
      }
      puVar17 = *(undefined8 **)(param_1 + 0x60);
    } while (puVar17 != *(undefined8 **)(param_1 + 0x68));
    FUN_14022c8e0(&DAT_140a7dfe0);
    if ((param_2 == '\x01') && (DAT_140b15418 != 0)) {
      FUN_14032e290(DAT_140b15418 + 0xa0,&local_68);
    }
    local_68 = DynamicVectorClass<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>_>
               ::vftable;
    if (local_60 != (ulonglong *)0x0) {
      if ((local_58 & 0x8000000000000000) != 0) {
        pvVar9 = GetProcessHeap();
        HeapFree(pvVar9,0,local_60);
        return;
      }
      FUN_14001eef0(local_60,3);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x321170  Name: FUN_140321170  Size: 162 bytes
// ------------------------------------------------------------

undefined1 FUN_140321170(longlong param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined8 *local_res10 [3];
  
  if (param_2 == (undefined8 *)0x0) {
    return 0;
  }
  local_res10[0] = (undefined8 *)FUN_140769c58(0x18);
  uVar2 = DAT_140a7c090;
  if (local_res10[0] != (undefined8 *)0x0) {
    *local_res10[0] = param_2;
    *(undefined1 *)(local_res10[0] + 1) = 0;
    *(undefined4 *)((longlong)local_res10[0] + 0xc) = uVar2;
    local_res10[0][2] = 0xffffffffffffffff;
  }
  puVar1 = *(undefined8 **)(param_1 + 0x68);
  if (puVar1 == *(undefined8 **)(param_1 + 0x70)) {
    FUN_14001ded0(param_1 + 0x60,puVar1,local_res10);
  }
  else {
    *puVar1 = local_res10[0];
    *(longlong *)(param_1 + 0x68) = *(longlong *)(param_1 + 0x68) + 8;
  }
  if (0xf < (ulonglong)param_2[3]) {
    param_2 = (undefined8 *)*param_2;
  }
  FUN_140025760("SpeechEventManager: Queue_Speech_Event() - Event %s queued, queue size now = %u\n",
                param_2,*(longlong *)(param_1 + 0x68) - *(longlong *)(param_1 + 0x60) >> 3);
  return 1;
}



// ------------------------------------------------------------
// RVA: 0x323660  Name: FUN_140323660  Size: 185 bytes
// ------------------------------------------------------------

void FUN_140323660(longlong param_1)

{
  longlong *plVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  longlong *plVar5;
  
  if (*(char *)(param_1 + 0x30) != '\0') {
    plVar1 = *(longlong **)(param_1 + 0x40);
    if (plVar1 != (longlong *)0x0) {
      iVar2 = (int)(plVar1[0xc] - plVar1[0xb] >> 5);
      iVar3 = (int)(plVar1[0xf] - plVar1[0xe] >> 5);
      iVar4 = (int)(plVar1[0x12] - plVar1[0x11] >> 5);
      if ((0 < iVar2 + iVar3 + iVar4) && (0 < iVar2 + iVar3 + iVar4)) {
        plVar5 = plVar1;
        if (0xf < (ulonglong)plVar1[3]) {
          plVar5 = (longlong *)*plVar1;
        }
        FUN_140025760("WeatherAudioManagerClass::Fade_Out_Active_Loop(): %s\n",plVar5);
        FUN_1402d5a30(&DAT_140b27e60,plVar1,DAT_140803520);
      }
    }
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined8 *)(param_1 + 0x40) = 0;
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 *)(param_1 + 0x50) = 0;
    *(undefined4 *)(param_1 + 0x58) = 0;
    *(undefined1 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
  }
  return;
}




// ================================================================
// PER-OBJECT DISPATCH (FUN_140265280)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x265280  Name: FUN_140265280  Size: 518 bytes
// ------------------------------------------------------------

void FUN_140265280(float param_1,undefined4 param_2,char param_3)

{
  bool bVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined *puVar5;
  longlong in_RCX;
  longlong *plVar6;
  float fVar7;
  
  if ((*(longlong *)(in_RCX + 0xe8) != 0) && (*(float *)(in_RCX + 0x104) == 0.0)) {
    param_1 = *(float *)(in_RCX + 0xd4);
    *(float *)(in_RCX + 0x104) = param_1;
    if (param_1 == 0.0) {
      *(undefined4 *)(in_RCX + 0x104) = 0x3f800000;
    }
  }
  cVar2 = FUN_1402650f0(param_1,in_RCX + 0xe8,param_2);
  cVar3 = FUN_1402650f0(in_RCX,in_RCX + 0xb8,param_2);
  cVar4 = FUN_1402650f0(in_RCX,in_RCX + 0x118,param_2);
  plVar6 = *(longlong **)(in_RCX + 0x48);
  if (plVar6 != (longlong *)0x0) {
    for (puVar5 = (undefined *)(**(code **)(*plVar6 + 0x20))(); puVar5 != (undefined *)0x0;
        puVar5 = *(undefined **)(puVar5 + 8)) {
      if (puVar5 == &DAT_140a12370) {
        bVar1 = true;
        goto LAB_140265359;
      }
    }
  }
  bVar1 = false;
LAB_140265359:
  if (!bVar1) {
    plVar6 = (longlong *)0x0;
  }
  if ((cVar3 == '\x01') || ((param_3 != '\0' && (*(longlong *)(in_RCX + 0xb8) != 0)))) {
    if (cVar2 == '\x01') {
      fVar7 = DAT_1407ffaf8;
      if (*(char *)(in_RCX + 0x108) == '\x01') {
        fVar7 = DAT_1407ffaf8 - *(float *)(in_RCX + 0x110) / *(float *)(in_RCX + 0x10c);
        *(float *)(in_RCX + 0x114) = fVar7;
      }
      if (plVar6 != (longlong *)0x0) {
        FUN_14012d0f0(plVar6,*(undefined8 *)(in_RCX + 0xe8),*(undefined4 *)(in_RCX + 0x100),
                      *(undefined8 *)(in_RCX + 0xb8),*(undefined4 *)(in_RCX + 0xd0),fVar7);
      }
    }
    else if (plVar6 != (longlong *)0x0) {
      FUN_14012d020(plVar6,*(undefined8 *)(in_RCX + 0xb8),*(undefined4 *)(in_RCX + 0xd0));
    }
  }
  if (((cVar4 != '\0') || (param_3 != '\0')) && (plVar6 != (longlong *)0x0)) {
    if (*(int *)(in_RCX + 0x128) == 2) {
      FUN_14012d220(plVar6,0,0);
      *(undefined4 *)(in_RCX + 0x124) = 0xffffffff;
    }
    else {
      FUN_14012d220(plVar6,*(undefined8 *)(in_RCX + 0x118),*(undefined4 *)(in_RCX + 0x130));
    }
  }
  *(undefined4 *)(in_RCX + 0x148) = param_2;
  return;
}




// ================================================================
// 0x10xxxx CLUSTER (appear in icall-slot xrefs)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x105190  Name: FUN_140105190  Size: 163 bytes
// ------------------------------------------------------------

void FUN_140105190(void)

{
  if (DAT_140a622e0 != '\0') {
    if (DAT_140a622f0 != 0) {
      FUN_140271a10();
    }
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_1401c1d20(&DAT_140a78510,DAT_140a121f0,1);
    DAT_140a121f0 = 0xffffffff;
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_14032edd0(DAT_140b15418 + 0xa0,"close_story_dialog");
    DAT_140a622e0 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x1053c0  Name: FUN_1401053c0  Size: 1988 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401053c0(uint param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined4 *puVar3;
  ulonglong uVar4;
  undefined1 *puVar5;
  uint uVar6;
  uint uVar7;
  longlong *plVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  longlong lVar11;
  undefined8 *****pppppuVar12;
  longlong lVar13;
  undefined8 *puVar14;
  uint in_stack_ffffffffffffef98;
  undefined8 ****local_1058;
  undefined4 uStack_1050;
  uint3 uStack_104c;
  undefined1 uStack_1049;
  ulonglong local_1048;
  ulonglong local_1040;
  short local_1038 [2056];
  
  uVar7 = DAT_1407ffaf8;
  lVar1 = DAT_140a622e8[1];
  lVar2 = *DAT_140a622e8;
  while ((DAT_140a62304 < (uint)(lVar1 - lVar2 >> 3) && (DAT_140a62300 <= param_1))) {
    puVar3 = *(undefined4 **)(*DAT_140a622e8 + (ulonglong)DAT_140a62304 * 8);
    switch(*puVar3) {
    case 0:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = 0;
      FUN_1402d5010(&DAT_140b27e60,&local_1058,0,0,0);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 1:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = in_stack_ffffffffffffef98 & 0xffffff00;
      FUN_1402d2070(&DAT_140b27dc0,&local_1058,0,0,in_stack_ffffffffffffef98);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 2:
      lVar13 = *(longlong *)(puVar3 + 2);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,1);
      lVar11 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar11 = lVar11 + 1;
      } while (*(char *)(lVar13 + lVar11) != '\0');
      FUN_140022730(&local_1058,lVar13);
      FUN_140320f80(&DAT_140b2b300,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 3:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),CONCAT71((uint7)(uint3)(DAT_140a62304 >> 8),1));
      break;
    case 4:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),0);
      break;
    case 5:
      if (DAT_140a622f0 != 0) {
        in_stack_ffffffffffffef98 = uVar7;
        FUN_140271700(DAT_140a622f0,**(undefined1 **)(puVar3 + 2),
                      CONCAT71((int7)((ulonglong)puVar3 >> 8),1),0,uVar7);
      }
      break;
    case 6:
      FUN_140105fd0(*(undefined8 *)(puVar3 + 2));
      break;
    case 7:
      plVar8 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      local_1058 = (undefined8 ****)0x0;
      local_1048 = 0;
      local_1040 = 0;
      uVar4 = plVar8[2];
      if (7 < (ulonglong)plVar8[3]) {
        plVar8 = (longlong *)*plVar8;
      }
      if (uVar4 < 8) {
        local_1058 = (undefined8 ****)*plVar8;
        uStack_1050 = *(undefined4 *)(plVar8 + 1);
        _uStack_104c = *(undefined4 *)((longlong)plVar8 + 0xc);
        uVar10 = 7;
      }
      else {
        uVar10 = uVar4 | 7;
        if (0x7ffffffffffffffe < uVar10) {
          uVar10 = 0x7ffffffffffffffe;
        }
        local_1058 = (undefined8 ****)FUN_1400226c0(&local_1058,uVar10 + 1);
        FUN_14076bd80(local_1058,plVar8,uVar4 * 2 + 2);
      }
      local_1048 = uVar4;
      local_1040 = uVar10;
      if (DAT_140a12208 != 0) {
        puVar14 = &DAT_140a121f8;
        if (7 < DAT_140a12210) {
          puVar14 = DAT_140a121f8;
        }
        pppppuVar12 = &local_1058;
        if (7 < uVar10) {
          pppppuVar12 = (undefined8 *****)local_1058;
        }
        FUN_140022b30(local_1038,0x800,pppppuVar12,puVar14);
        lVar13 = -1;
        do {
          lVar13 = lVar13 + 1;
        } while (local_1038[lVar13] != 0);
        FUN_1400228a0(&local_1058,local_1038);
      }
      FUN_140106360(&local_1058);
      if (7 < local_1040) {
        if (0xfff < local_1040 * 2 + 2) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 8:
      FUN_140106680();
      break;
    case 9:
      local_1040 = 0xf;
      local_1048 = 10;
      uStack_1050._2_1_ = '\0';
      uStack_1050._0_2_ = s_IDC_STATIC_1408181e0._8_2_;
      local_1058 = (undefined8 ****)s_IDC_STATIC_1408181e0._0_8_;
      uVar9 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      FUN_1401c8ea0(&DAT_140a78510,DAT_140a121f0,&local_1058,uVar9);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 10:
      local_1040 = 0xf;
      local_1048 = 0xe;
      uVar6 = (uint)_uStack_104c >> 0x18;
      uStack_1049 = (undefined1)uVar6;
      uStack_104c = (uint3)(ushort)s_IDC_STORY_TEXT_140818168._12_2_;
      local_1058 = (undefined8 ****)s_IDC_STORY_TEXT_140818168._0_8_;
      uStack_1050 = s_IDC_STORY_TEXT_140818168._8_4_;
      FUN_1401c40b0(&DAT_140a78510,DAT_140a121f0,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0xb:
      puVar5 = *(undefined1 **)(puVar3 + 2);
      in_stack_ffffffffffffef98 = CONCAT31((int3)(in_stack_ffffffffffffef98 >> 8),puVar5[0xc]);
      FUN_1401f9470(&DAT_140a62310,*puVar5,puVar5[4],puVar5[8],in_stack_ffffffffffffef98);
      break;
    case 0xc:
      FUN_140106bb0(**(undefined4 **)(puVar3 + 2));
      break;
    case 0xd:
      DAT_140a62300 = **(int **)(puVar3 + 2) + param_1;
      break;
    case 0xe:
      if (**(char **)(puVar3 + 2) == '\0') {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
      else {
        (**(code **)(*DAT_140b15418 + 0x38))();
      }
      break;
    case 0xf:
      FUN_140106e60(*(undefined8 *)(puVar3 + 2));
      break;
    case 0x10:
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._0_2_ = (ushort)(byte)s_IDC_IMAGE_1408002e8[8];
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      FUN_1401c3810(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._1_1_ = '\0';
      uStack_1050._0_1_ = s_IDC_IMAGE_1408002e8[8];
      FUN_1401c38d0(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0x11:
      if (DAT_140a622f0 != 0) {
        *(undefined1 *)(DAT_140a622f0 + 0x1d) = **(undefined1 **)(puVar3 + 2);
      }
      break;
    case 0x12:
      if (DAT_140b30690 != 0) {
        FUN_14044f130(&DAT_140b306dd);
      }
    }
    DAT_140a62304 = DAT_140a62304 + 1;
  }
  s_IDC_STORY_TEXT_140818168[8] = (char)s_IDC_STORY_TEXT_140818168._8_4_;
  s_IDC_STORY_TEXT_140818168[9] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,1);
  s_IDC_STORY_TEXT_140818168[10] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,2);
  s_IDC_STORY_TEXT_140818168[0xb] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,3);
  s_IDC_STORY_TEXT_140818168[0xc] = (char)s_IDC_STORY_TEXT_140818168._12_2_;
  s_IDC_STORY_TEXT_140818168[0xd] = (char)((ushort)s_IDC_STORY_TEXT_140818168._12_2_ >> 8);
  s_IDC_STATIC_1408181e0[8] = (char)s_IDC_STATIC_1408181e0._8_2_;
  s_IDC_STATIC_1408181e0[9] = (char)((ushort)s_IDC_STATIC_1408181e0._8_2_ >> 8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x105d30  Name: FUN_140105d30  Size: 190 bytes
// ------------------------------------------------------------

void FUN_140105d30(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x107900  Name: FUN_140107900  Size: 611 bytes
// ------------------------------------------------------------

void FUN_140107900(void)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  int iVar11;
  int iVar12;
  ulonglong uVar13;
  
  if (DAT_140a62320 != '\0') {
    FUN_1401c1d20(&DAT_140a78510,DAT_140a12218,1);
    DAT_140a12218 = 0xffffffff;
    uVar13 = (ulonglong)*(int *)(DAT_140a62328 + 0x10);
    iVar7 = *(int *)(DAT_140a62328 + 0x10) + -1;
    if (-1 < iVar7) {
      lVar9 = (longlong)iVar7 * 0x70;
      lVar4 = uVar13 * 0x70;
      lVar8 = DAT_140a62328;
      lVar10 = lVar9;
      do {
        lVar3 = *(longlong *)(lVar8 + 8);
        iVar12 = (int)uVar13;
        if ((((*(char *)(lVar9 + 0x6e + lVar3) != '\0') && (*(char *)(lVar9 + 0x6d + lVar3) != '\0')
             ) && (-1 < iVar7)) && (iVar11 = *(int *)(lVar8 + 0x10), iVar7 < iVar11)) {
          uVar5 = (iVar11 - iVar7) - 1;
          uVar13 = (ulonglong)uVar5;
          if (uVar5 != 0) {
            if ((*(uint *)(lVar8 + 0x14) & 0x80000000) == 0) {
              if (iVar12 < iVar7) {
                iVar6 = iVar7 + -1 + uVar5;
                if (0 < (int)uVar5) {
                  do {
                    iVar11 = iVar11 + -1;
                    lVar3 = (longlong)iVar6;
                    iVar6 = iVar6 + -1;
                    FUN_14010a170(lVar3 * 0x70 + *(longlong *)(lVar8 + 8),
                                  (longlong)iVar11 * 0x70 + *(longlong *)(lVar8 + 8));
                    uVar13 = uVar13 - 1;
                  } while (uVar13 != 0);
                }
              }
              else if (0 < (int)uVar5) {
                iVar11 = 0;
                do {
                  iVar2 = iVar12 + iVar11;
                  iVar6 = iVar11 + iVar7;
                  iVar11 = iVar11 + 1;
                  FUN_14010a170((longlong)iVar6 * 0x70 + *(longlong *)(lVar8 + 8),
                                (longlong)iVar2 * 0x70 + *(longlong *)(lVar8 + 8));
                } while (iVar11 < (int)uVar5);
              }
            }
            else {
              FUN_14076bd80(lVar3 + lVar10,lVar4 + lVar3,(longlong)(int)uVar5 * 0x70);
            }
          }
          *(int *)(lVar8 + 0x10) = *(int *)(lVar8 + 0x10) + -1;
          lVar8 = DAT_140a62328;
        }
        lVar4 = lVar4 + -0x70;
        lVar10 = lVar10 + -0x70;
        uVar13 = (ulonglong)(iVar12 - 1);
        lVar9 = lVar9 + -0x70;
        iVar7 = iVar7 + -1;
      } while (-1 < iVar7);
    }
    if (DAT_140a12221 == '\0') {
      FUN_140321830(&DAT_140b2b300,1);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
    }
    cVar1 = FUN_14028af60(&DAT_140b153e0);
    if (cVar1 == '\0') {
      iVar7 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (iVar7 == 0) {
        FUN_14028c720(&DAT_140b153e0,1);
      }
      else if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
    }
    FUN_14030ac40(&DAT_140b27f60);
    FUN_14032edd0(DAT_140b15418 + 0x14,"close_story_dialog");
    DAT_140a62320 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x1089e0  Name: FUN_1401089e0  Size: 1896 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401089e0(uint param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  longlong lVar9;
  undefined8 *******pppppppuVar10;
  longlong lVar11;
  char cVar12;
  undefined8 *puVar13;
  uint in_stack_ffffffffffffefa8;
  undefined8 ******local_1048;
  uint3 uStack_1040;
  char cStack_103d;
  undefined4 uStack_103c;
  ulonglong local_1038;
  ulonglong local_1030;
  short local_1028 [2048];
  
  if ((DAT_140a12224 != -1) && (DAT_140a12224 <= *(int *)(DAT_140a62328 + 0x10))) {
    puVar13 = (undefined8 *)((longlong)DAT_140a12224 * 0x70 + *(longlong *)(DAT_140a62328 + 8));
    *(undefined1 *)((longlong)puVar13 + 0x6d) = 1;
    puVar6 = puVar13;
    if (0xf < (ulonglong)puVar13[3]) {
      puVar6 = (undefined8 *)*puVar13;
    }
    plVar3 = (longlong *)FUN_140108430(puVar6,*(undefined4 *)(puVar13 + 0xd));
    cVar12 = '\x01';
    if (plVar3 != (longlong *)0x0) {
      lVar11 = *plVar3;
      uVar7 = (ulonglong)DAT_140a62340;
      if (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3)) {
        do {
          puVar6 = (undefined8 *)0x0;
          if (param_1 < DAT_140a6233c) {
            return;
          }
          if (cVar12 == '\0') {
            return;
          }
          puVar1 = *(undefined4 **)(lVar11 + uVar7 * 8);
          switch(*puVar1) {
          case 0:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = 0;
            FUN_1402d5010(&DAT_140b27e60,&local_1048,0,0,0);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
LAB_140108b67:
              thunk_FUN_1407864b8();
            }
            break;
          case 1:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = in_stack_ffffffffffffefa8 & 0xffffff00;
            FUN_1402d2070(&DAT_140b27dc0,&local_1048,0,0,in_stack_ffffffffffffefa8);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            break;
          case 2:
            lVar11 = *(longlong *)(puVar1 + 2);
            FUN_140322780(&DAT_140b2b3a8);
            FUN_1403207c0(&DAT_140b2b2f0);
            FUN_140321830(&DAT_140b2b300,1);
            lVar9 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar9 = lVar9 + 1;
            } while (*(char *)(lVar11 + lVar9) != '\0');
            FUN_140022730(&local_1048,lVar11);
            FUN_140320f80(&DAT_140b2b300,&local_1048,1);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            DAT_140a12221 = '\0';
            break;
          case 3:
            uVar5 = CONCAT71((int7)((ulonglong)lVar11 >> 8),1);
            goto LAB_140108cd1;
          case 4:
            uVar5 = 0;
LAB_140108cd1:
            FUN_1402fc7a0(&DAT_140b27f60,*(undefined8 *)(puVar1 + 2),uVar5);
            break;
          case 6:
            FUN_1401099f0(*(undefined8 *)(puVar1 + 2));
            break;
          case 7:
            if ((puVar13[6] != 0) && (puVar6 = puVar13 + 4, 0xf < (ulonglong)puVar13[7])) {
              puVar6 = (undefined8 *)*puVar6;
            }
            plVar4 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0;
            uVar7 = plVar4[2];
            if (7 < (ulonglong)plVar4[3]) {
              plVar4 = (longlong *)*plVar4;
            }
            if (uVar7 < 8) {
              local_1048 = (undefined8 ******)*plVar4;
              _uStack_1040 = *(undefined4 *)(plVar4 + 1);
              uStack_103c = *(uint *)((longlong)plVar4 + 0xc);
              uVar8 = 7;
            }
            else {
              uVar8 = uVar7 | 7;
              if (0x7ffffffffffffffe < uVar8) {
                uVar8 = 0x7ffffffffffffffe;
              }
              local_1048 = (undefined8 ******)FUN_1400226c0(&local_1048,uVar8 + 1);
              FUN_14076bd80(local_1048,plVar4,uVar7 * 2 + 2);
            }
            local_1038 = uVar7;
            local_1030 = uVar8;
            if (puVar6 != (undefined8 *)0x0) {
              pppppppuVar10 = &local_1048;
              if (7 < uVar8) {
                pppppppuVar10 = (undefined8 *******)local_1048;
              }
              FUN_140022b30(local_1028,0x800,pppppppuVar10,puVar6);
              lVar11 = -1;
              do {
                lVar11 = lVar11 + 1;
              } while (local_1028[lVar11] != 0);
              FUN_1400228a0(&local_1048,local_1028);
            }
            FUN_140108720(&local_1048);
            if (7 < local_1030) {
              if (0xfff < local_1030 * 2 + 2) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 * 2 + 0x29);
                }
              }
              thunk_FUN_1407864b8();
            }
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 8:
            FUN_1401093b0(puVar13[0xc]);
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 9:
            local_1048 = (undefined8 ******)s_IDC_STATIC_1408181e0._0_8_;
            local_1030 = 0xf;
            local_1038 = 10;
            uStack_1040._2_1_ = '\0';
            uStack_1040._0_1_ = s_IDC_STATIC_1408181e0[8];
            uStack_1040._1_1_ = s_IDC_STATIC_1408181e0[9];
            uVar5 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            FUN_1401c8ea0(&DAT_140a78510,DAT_140a12218,&local_1048,uVar5);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 0xb:
            puVar2 = *(undefined1 **)(puVar1 + 2);
            in_stack_ffffffffffffefa8 = CONCAT31((int3)(in_stack_ffffffffffffefa8 >> 8),puVar2[0xc])
            ;
            FUN_1401f9470(&DAT_140a62348,*puVar2,puVar2[4],puVar2[8],in_stack_ffffffffffffefa8);
            break;
          case 0xc:
            FUN_1401091b0(**(undefined4 **)(puVar1 + 2));
            break;
          case 0xd:
            DAT_140a6233c = **(int **)(puVar1 + 2) + param_1;
            break;
          case 0x10:
            FUN_140109bf0();
            break;
          case 0x12:
            FUN_14030ac40(&DAT_140b27f60);
            break;
          case 0x13:
            cVar12 = DAT_140a12221;
            if (DAT_140a12221 != '\0') break;
            goto LAB_1401090cc;
          }
          DAT_140a62340 = DAT_140a62340 + 1;
LAB_1401090cc:
          lVar11 = *plVar3;
          uVar7 = (ulonglong)DAT_140a62340;
        } while (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3));
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x1098e0  Name: FUN_1401098e0  Size: 197 bytes
// ------------------------------------------------------------

void FUN_1401098e0(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DAT_140a12221 = 0;
  return;
}




// ================================================================
// Start_Game helper (FUN_1402b09b0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2b09b0  Name: FUN_1402b09b0  Size: 101 bytes
// ------------------------------------------------------------

void FUN_1402b09b0(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong lVar2;
  
  lVar2 = *(longlong *)(param_1 + 0x48);
  if (lVar2 != param_1 + 0x40) {
    do {
      lVar1 = *(longlong *)(lVar2 + 0x18) + -0x18;
      if (*(longlong *)(lVar2 + 0x18) == 0) {
        lVar1 = 0;
      }
      FUN_140390a70(lVar1,param_2);
      lVar2 = *(longlong *)(lVar2 + 8);
    } while (lVar2 != param_1 + 0x40);
  }
  return;
}



