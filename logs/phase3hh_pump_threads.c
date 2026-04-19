
// ================================================================
// FUN_140247a90 — Pump_Threads (Lua AI coroutine execution)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x247a90  Name: FUN_140247a90  Size: 953 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x000140247d8c) */
/* WARNING: Removing unreachable block (ram,0x000140247c59) */

void FUN_140247a90(longlong param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  int iVar3;
  undefined *puVar4;
  longlong lVar5;
  ulonglong uVar6;
  longlong *plVar7;
  longlong *plVar8;
  undefined8 *puVar9;
  ulonglong uVar10;
  uint uVar11;
  ulonglong uVar12;
  longlong local_58 [2];
  undefined8 local_48;
  ulonglong local_40;
  undefined4 local_38;
  undefined1 local_34;
  longlong *local_30;
  
  if (*(char *)(param_1 + 0x121) == '\0') {
    lVar5 = *(longlong *)(param_1 + 0x40);
    uVar10 = 0;
    uVar12 = uVar10;
    if (0 < (int)(*(longlong *)(param_1 + 0x48) - lVar5 >> 6)) {
      do {
        plVar7 = (longlong *)0x0;
        if (*(longlong *)(uVar10 + lVar5) != 0) {
          FUN_140247700(*(longlong *)(uVar10 + lVar5),*(undefined8 *)(uVar10 + 8 + lVar5));
          lVar5 = *(longlong *)(param_1 + 0x40);
          plVar8 = plVar7;
          if (*(longlong *)(uVar10 + 0x38 + lVar5) != 0) {
            plVar8 = (longlong *)&DAT_00000001;
            FUN_140247700(*(undefined8 *)(uVar10 + lVar5));
            lVar5 = *(longlong *)(param_1 + 0x40);
            plVar1 = *(longlong **)(uVar10 + 0x38 + lVar5);
            if (plVar1 != (longlong *)0x0) {
              (**(code **)(*plVar1 + 8))(plVar1);
              *(undefined8 *)(uVar10 + lVar5 + 0x38) = 0;
              lVar5 = *(longlong *)(param_1 + 0x40);
            }
          }
          iVar3 = thunk_FUN_1407bc310(*(undefined8 *)(uVar10 + lVar5),plVar8,
                                      (longlong)*(int *)(uVar10 + 0x30 + lVar5));
          if (iVar3 == 0) {
            plVar8 = (longlong *)
                     FUN_1402473e0(*(undefined8 *)(*(longlong *)(param_1 + 0x40) + uVar10),0,1,0);
            if (plVar8 != (longlong *)0x0) {
              (**(code **)*plVar8)(plVar8);
              for (puVar4 = (undefined *)(**(code **)(*plVar8 + 0x20))(plVar8);
                  puVar4 != (undefined *)0x0; puVar4 = *(undefined **)(puVar4 + 8)) {
                if (puVar4 == &DAT_140a157b0) {
                  (**(code **)*plVar8)(plVar8);
                  plVar7 = plVar8;
                  if ((char)plVar8[2] != '\0') goto LAB_140247d94;
                  break;
                }
              }
            }
            FUN_140248d10(param_1,*(undefined8 *)(uVar10 + *(longlong *)(param_1 + 0x40)));
            local_58[0] = 0;
            local_48 = 0;
            local_38 = 0;
            local_30 = (longlong *)0x0;
            puVar9 = (undefined8 *)(*(longlong *)(param_1 + 0x40) + uVar10);
            local_40 = 0xf;
            local_34 = 0;
            *puVar9 = 0;
            if ((longlong *)puVar9[1] != (longlong *)0x0) {
              (**(code **)(*(longlong *)puVar9[1] + 8))();
              puVar9[1] = 0;
            }
            FUN_1400582a0(puVar9 + 2,local_58);
            plVar2 = local_30;
            *(undefined4 *)(puVar9 + 6) = local_38;
            *(undefined1 *)((longlong)puVar9 + 0x34) = local_34;
            plVar1 = (longlong *)puVar9[7];
            if (plVar1 == local_30) {
LAB_140247d3a:
              if (plVar2 != (longlong *)0x0) {
                (**(code **)(*plVar2 + 8))(plVar2);
              }
            }
            else {
              if (plVar1 != (longlong *)0x0) {
                (**(code **)(*plVar1 + 8))();
              }
              puVar9[7] = plVar2;
              if (plVar2 != (longlong *)0x0) {
                (**(code **)*plVar2)(plVar2);
                goto LAB_140247d3a;
              }
            }
            if (0xf < local_40) {
              uVar6 = local_40 + 1;
              lVar5 = local_58[0];
              if (0xfff < uVar6) {
                lVar5 = *(longlong *)(local_58[0] + -8);
                uVar6 = local_40 + 0x28;
                if (0x1f < (local_58[0] - lVar5) - 8U) goto LAB_140247e44;
              }
              thunk_FUN_1407864b8(lVar5,uVar6);
            }
LAB_140247d94:
            if (*(char *)(param_1 + 0x121) != '\0') {
              if (plVar7 != (longlong *)0x0) {
                (**(code **)(*plVar7 + 8))(plVar7);
              }
              if (plVar8 != (longlong *)0x0) {
                (**(code **)(*plVar8 + 8))(plVar8);
              }
              break;
            }
            if (plVar7 != (longlong *)0x0) {
              (**(code **)(*plVar7 + 8))(plVar7);
            }
            if (plVar8 != (longlong *)0x0) {
              (**(code **)(*plVar8 + 8))(plVar8);
            }
          }
          else {
            FUN_140246940();
            FUN_140248d10(param_1,*(undefined8 *)(uVar10 + *(longlong *)(param_1 + 0x40)));
            puVar9 = (undefined8 *)(*(longlong *)(param_1 + 0x40) + uVar10);
            local_58[0] = 0;
            local_48 = 0;
            local_40 = 0xf;
            *puVar9 = 0;
            local_38 = 0;
            local_34 = 0;
            local_30 = (longlong *)0x0;
            if ((longlong *)puVar9[1] != (longlong *)0x0) {
              (**(code **)(*(longlong *)puVar9[1] + 8))();
              puVar9[1] = 0;
            }
            FUN_1400582a0(puVar9 + 2,local_58);
            plVar8 = local_30;
            *(undefined4 *)(puVar9 + 6) = local_38;
            *(undefined1 *)((longlong)puVar9 + 0x34) = local_34;
            plVar7 = (longlong *)puVar9[7];
            if (plVar7 == local_30) {
LAB_140247c03:
              if (plVar8 != (longlong *)0x0) {
                (**(code **)(*plVar8 + 8))(plVar8);
              }
            }
            else {
              if (plVar7 != (longlong *)0x0) {
                (**(code **)(*plVar7 + 8))();
              }
              puVar9[7] = plVar8;
              if (plVar8 != (longlong *)0x0) {
                (**(code **)*plVar8)(plVar8);
                goto LAB_140247c03;
              }
            }
            if (0xf < local_40) {
              if (0xfff < local_40 + 1) {
                lVar5 = *(longlong *)(local_58[0] + -8);
                uVar6 = local_40 + 0x28;
                if (0x1f < (local_58[0] - lVar5) - 8U) {
LAB_140247e44:
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(lVar5,uVar6);
                }
              }
              thunk_FUN_1407864b8();
            }
          }
        }
        lVar5 = *(longlong *)(param_1 + 0x40);
        uVar11 = (int)uVar12 + 1;
        uVar10 = uVar10 + 0x40;
        uVar12 = (ulonglong)uVar11;
      } while ((int)uVar11 < (int)(*(longlong *)(param_1 + 0x48) - lVar5 >> 6));
    }
    if (*(char *)(param_1 + 0x121) == '\0') {
      return;
    }
  }
  FUN_1402488e0(param_1);
  return;
}




// ================================================================
// FUN_140246fb0 — read Lua variable (ServiceRate/LastService lookup)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x246fb0  Name: FUN_140246fb0  Size: 67 bytes
// ------------------------------------------------------------

void FUN_140246fb0(longlong param_1,undefined8 param_2,undefined1 param_3)

{
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(param_1 + 0x58);
  FUN_1407b9540(uVar1);
  FUN_1407b8e90(uVar1,0xffffd8ef);
  FUN_1402473e0(uVar1,param_3,1,0);
  return;
}




// ================================================================
// FUN_140247000 — set Lua 'LastService' variable
// ================================================================

// ------------------------------------------------------------
// RVA: 0x247000  Name: FUN_140247000  Size: 62 bytes
// ------------------------------------------------------------

void FUN_140247000(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(param_1 + 0x58);
  FUN_1407b9540(uVar1,param_3);
  FUN_140247700(uVar1,param_2);
  FUN_1407b9a60(uVar1,0xffffd8ef);
  return;
}




// ================================================================
// FUN_1402488e0 — Lua cleanup (called in shutdown)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2488e0  Name: FUN_1402488e0  Size: 754 bytes
// ------------------------------------------------------------

void FUN_1402488e0(longlong param_1)

{
  undefined8 uVar1;
  undefined **ppuVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined *puVar6;
  undefined **local_res8;
  
  if (*(longlong *)(param_1 + 0x58) != 0) {
    *(undefined1 *)(param_1 + 0x121) = 1;
    FUN_1402490c0(param_1 + 0x40,0);
    FUN_1407b94a0(*(undefined8 *)(param_1 + 0x58),"LuaThreadTable",0xe);
    FUN_1407b9510(*(undefined8 *)(param_1 + 0x58));
    FUN_1407b9a60(*(undefined8 *)(param_1 + 0x58),0xffffd8ef);
    if (DAT_140a10b50 == 0) {
      lVar4 = FUN_140769c58(0x78);
      if (lVar4 == 0) {
        DAT_140a10b50 = 0;
      }
      else {
        DAT_140a10b50 = FUN_140221820(lVar4);
      }
      local_res8 = &PTR_vftable_140a10b48;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_140a10b48;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140220ed0(DAT_140a10b50,param_1 + 0x28,0x2f);
    if (*(char *)(param_1 + 0x124) != '\0') {
      uVar1 = *(undefined8 *)(param_1 + 0x58);
      FUN_1407b9540(uVar1,"Flush_G");
      FUN_1407b8e90(uVar1,0xffffd8ef);
      plVar5 = (longlong *)FUN_1402473e0(uVar1,0,1,0);
      if (plVar5 != (longlong *)0x0) {
        for (puVar6 = (undefined *)(**(code **)(*plVar5 + 0x20))(plVar5); puVar6 != (undefined *)0x0
            ; puVar6 = *(undefined **)(puVar6 + 8)) {
          if (puVar6 == &DAT_140a15820) {
            (**(code **)*plVar5)(plVar5);
            FUN_140243640(param_1,&local_res8,plVar5,0,0);
            (**(code **)(*plVar5 + 8))(plVar5);
            if (local_res8 != (undefined **)0x0) {
              (**(code **)(*local_res8 + 8))();
            }
            break;
          }
        }
      }
      uVar1 = *(undefined8 *)(param_1 + 0x58);
      FUN_1407b9540(uVar1,"Base_Definitions");
      FUN_1407b8e90(uVar1,0xffffd8ef);
      plVar5 = (longlong *)FUN_1402473e0(uVar1,0,1,0);
      if (plVar5 != (longlong *)0x0) {
        for (puVar6 = (undefined *)(**(code **)(*plVar5 + 0x20))(plVar5); puVar6 != (undefined *)0x0
            ; puVar6 = *(undefined **)(puVar6 + 8)) {
          if (puVar6 == &DAT_140a15820) {
            (**(code **)*plVar5)(plVar5);
            FUN_140243640(param_1,&local_res8,plVar5,0,0);
            (**(code **)(*plVar5 + 8))(plVar5);
            if (local_res8 != (undefined **)0x0) {
              (**(code **)(*local_res8 + 8))();
            }
            break;
          }
        }
      }
      iVar3 = FUN_1407b8ef0(*(undefined8 *)(param_1 + 0x58));
      FUN_1407b9ab0(*(undefined8 *)(param_1 + 0x58),-1 - iVar3);
      *(undefined2 *)(param_1 + 0x122) = 0;
      FUN_140244000(param_1);
      return;
    }
    if (DAT_140b09d58 != (code *)0x0) {
      plVar5 = (longlong *)(param_1 + 0x88);
      if (0xf < *(ulonglong *)(param_1 + 0xa0)) {
        plVar5 = (longlong *)*plVar5;
      }
      (*DAT_140b09d58)(plVar5);
    }
    if (*(longlong **)(param_1 + 0xd8) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0xd8) + 8))();
      *(undefined8 *)(param_1 + 0xd8) = 0;
    }
    if (*(longlong *)(param_1 + 0x58) != 0) {
      FUN_1407b8890();
    }
    *(undefined8 *)(param_1 + 0x58) = 0;
    FUN_1401cacb0(&DAT_140b09dc0,&local_res8,param_1 + 0x130);
    ppuVar2 = local_res8;
    if (local_res8 != DAT_140b09dc8) {
      if (DAT_140b09e88 != (longlong *)0x0) {
        (**(code **)(*DAT_140b09e88 + 0x38))(DAT_140b09e88,param_1);
      }
      FUN_140242370(&DAT_140b09dc0,&local_res8,ppuVar2);
    }
  }
  return;
}



