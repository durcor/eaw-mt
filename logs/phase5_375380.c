/* FUN_140375380  RVA 0x375380 */


void FUN_140375380(longlong param_1,int param_2)

{
  undefined4 *puVar1;
  longlong lVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  double dVar6;
  double dVar7;
  char cVar8;
  int iVar9;
  undefined4 uVar10;
  longlong lVar11;
  longlong *plVar12;
  undefined8 uVar13;
  longlong *plVar14;
  ulonglong uVar15;
  longlong *plVar16;
  int *piVar17;
  longlong lVar18;
  HANDLE pvVar19;
  LPVOID pvVar20;
  ulonglong uVar21;
  ulonglong *puVar22;
  longlong *plVar23;
  int iVar24;
  longlong *plVar25;
  undefined1 *puVar26;
  int *piVar27;
  ulonglong *puVar28;
  char *pcVar29;
  longlong *plVar30;
  int iVar31;
  uint uVar32;
  ulonglong *puVar33;
  uint uVar34;
  undefined8 *puVar35;
  int local_res10;
  longlong local_res18;
  ulonglong local_res20;
  undefined4 local_208;
  undefined4 local_204;
  undefined4 local_200;
  undefined4 local_1fc;
  undefined4 local_1f8;
  undefined4 local_1f4;
  longlong local_1f0 [2];
  longlong local_1e0;
  undefined1 uStack_1d9;
  char local_1d8 [272];
  undefined1 local_c8 [144];
  
  local_res10 = param_2;
  if (param_2 == -1) {
    local_res10 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      local_res10 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (local_res10 == -1) {
      return;
    }
  }
  if ((DAT_140b15490 == '\0') && (*(char *)(param_1 + 0x11) == '\x01')) {
    return;
  }
  plVar23 = (longlong *)0x0;
  plVar30 = (longlong *)0x0;
  iVar9 = -1;
  plVar12 = plVar30;
  if (local_res10 == 0) {
    if (*(char *)(param_1 + 0x31) == '\0') {
      if (*(longlong *)(param_1 + 400) == 0) {
        if (*(longlong **)(param_1 + 0x288) != (longlong *)0x0) {
          (**(code **)(**(longlong **)(param_1 + 0x288) + 8))();
          *(undefined8 *)(param_1 + 0x288) = 0;
        }
      }
      else {
        pcVar29 = PTR_s_Data_Art_Models__140a28548;
        do {
          cVar8 = *pcVar29;
          pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
          pcVar29 = pcVar29 + 1;
        } while (cVar8 != '\0');
        puVar35 = (undefined8 *)(param_1 + 0x180);
        if (0xf < *(ulonglong *)(param_1 + 0x198)) {
          puVar35 = (undefined8 *)*puVar35;
        }
        puVar26 = &uStack_1d9;
        do {
          pcVar29 = puVar26 + 1;
          puVar26 = puVar26 + 1;
        } while (*pcVar29 != '\0');
        lVar11 = 0;
        do {
          cVar8 = *(char *)((longlong)puVar35 + lVar11);
          puVar26[lVar11] = cVar8;
          lVar11 = lVar11 + 1;
        } while (cVar8 != '\0');
        plVar14 = (longlong *)FUN_14025ec10(local_1d8,0);
        plVar12 = *(longlong **)(param_1 + 0x288);
        if (plVar12 != plVar14) {
          if (plVar12 != (longlong *)0x0) {
            (**(code **)(*plVar12 + 8))();
          }
          *(longlong **)(param_1 + 0x288) = plVar14;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
          }
        }
      }
      if (*(longlong *)(param_1 + 0x1b0) == 0) {
        if (*(longlong **)(param_1 + 0x290) != (longlong *)0x0) {
          (**(code **)(**(longlong **)(param_1 + 0x290) + 8))();
          *(undefined8 *)(param_1 + 0x290) = 0;
        }
      }
      else {
        pcVar29 = PTR_s_Data_Art_Models__140a28548;
        do {
          cVar8 = *pcVar29;
          pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
          pcVar29 = pcVar29 + 1;
        } while (cVar8 != '\0');
        puVar35 = (undefined8 *)(param_1 + 0x1a0);
        if (0xf < *(ulonglong *)(param_1 + 0x1b8)) {
          puVar35 = (undefined8 *)*puVar35;
        }
        puVar26 = &uStack_1d9;
        do {
          pcVar29 = puVar26 + 1;
          puVar26 = puVar26 + 1;
        } while (*pcVar29 != '\0');
        lVar11 = 0;
        do {
          cVar8 = *(char *)((longlong)puVar35 + lVar11);
          puVar26[lVar11] = cVar8;
          lVar11 = lVar11 + 1;
        } while (cVar8 != '\0');
        plVar14 = (longlong *)FUN_14025ec10(local_1d8,0);
        plVar12 = *(longlong **)(param_1 + 0x290);
        if (plVar12 != plVar14) {
          if (plVar12 != (longlong *)0x0) {
            (**(code **)(*plVar12 + 8))();
          }
          *(longlong **)(param_1 + 0x290) = plVar14;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
          }
        }
      }
      if (*(longlong *)(param_1 + 0x1d0) == 0) {
        if (*(longlong **)(param_1 + 0x298) != (longlong *)0x0) {
          (**(code **)(**(longlong **)(param_1 + 0x298) + 8))();
          *(undefined8 *)(param_1 + 0x298) = 0;
        }
      }
      else {
        pcVar29 = PTR_s_Data_Art_Models__140a28548;
        do {
          cVar8 = *pcVar29;
          pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
          pcVar29 = pcVar29 + 1;
        } while (cVar8 != '\0');
        puVar35 = (undefined8 *)(param_1 + 0x1c0);
        if (0xf < *(ulonglong *)(param_1 + 0x1d8)) {
          puVar35 = (undefined8 *)*puVar35;
        }
        puVar26 = &uStack_1d9;
        do {
          pcVar29 = puVar26 + 1;
          puVar26 = puVar26 + 1;
        } while (*pcVar29 != '\0');
        lVar11 = 0;
        do {
          cVar8 = *(char *)((longlong)puVar35 + lVar11);
          puVar26[lVar11] = cVar8;
          lVar11 = lVar11 + 1;
        } while (cVar8 != '\0');
        plVar14 = (longlong *)FUN_14025ec10(local_1d8,0);
        plVar12 = *(longlong **)(param_1 + 0x298);
        if (plVar12 != plVar14) {
          if (plVar12 != (longlong *)0x0) {
            (**(code **)(*plVar12 + 8))();
          }
          *(longlong **)(param_1 + 0x298) = plVar14;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
            *(undefined1 *)(param_1 + 0x31) = 1;
            plVar30 = plVar23;
            goto LAB_14037689d;
          }
        }
      }
      *(undefined1 *)(param_1 + 0x31) = 1;
      goto LAB_14037689d;
    }
  }
  else if (local_res10 == 1) {
    if (*(char *)(param_1 + 0x32) == '\0') {
      iVar31 = *(int *)(param_1 + 0x23c);
      if (iVar31 == 0) {
        if (*(longlong *)(param_1 + 0x1f0) == 0) {
          if (*(longlong **)(param_1 + 0x2a0) != (longlong *)0x0) {
            (**(code **)(**(longlong **)(param_1 + 0x2a0) + 8))();
            *(undefined8 *)(param_1 + 0x2a0) = 0;
          }
          if (*(longlong **)(param_1 + 0x2b0) != (longlong *)0x0) {
            (**(code **)(**(longlong **)(param_1 + 0x2b0) + 8))();
            *(undefined8 *)(param_1 + 0x2b0) = 0;
          }
        }
        else {
          if (DAT_140b15418 != (longlong *)0x0) {
            iVar9 = (int)DAT_140b15418[1];
          }
          if (((*(longlong *)(param_1 + 0x2a0) == 0) ||
              (lVar18 = *(longlong *)(param_1 + 0x228) - *(longlong *)(param_1 + 0x220),
              lVar11 = lVar18 >> 0x3f, lVar18 / 0x28 + lVar11 == lVar11)) ||
             (iVar9 != *(int *)(param_1 + 0x238))) {
            if ((*(int *)(*(longlong *)
                           ((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) + 0xd4)
                 < DAT_140b2c250) && (_Init_thread_header(&DAT_140b2c250), DAT_140b2c250 == -1)) {
              atexit(FUN_1407f03c0);
              _Init_thread_footer(&DAT_140b2c250);
            }
            puVar35 = (undefined8 *)(param_1 + 0x1e0);
            if ((undefined8 **)puVar35 != &DAT_140a285e0) {
              if (0xf < *(ulonglong *)(param_1 + 0x1f8)) {
                puVar35 = (undefined8 *)*puVar35;
              }
              FUN_140022730(&DAT_140a285e0,puVar35,*(undefined8 *)(param_1 + 0x1f0));
            }
            piVar27 = *(int **)(param_1 + 0x220);
            iVar31 = (int)((*(longlong *)(param_1 + 0x228) - (longlong)piVar27) / 0x28);
            piVar17 = piVar27;
            plVar12 = plVar23;
            if (0 < iVar31) {
              do {
                if (*piVar17 == iVar9) {
                  piVar27 = piVar27 + ((longlong)(int)plVar12 * 5 + 1) * 2;
                  if ((undefined8 **)piVar27 != &DAT_140a285e0) {
                    if (0xf < *(ulonglong *)(piVar27 + 6)) {
                      piVar27 = *(int **)piVar27;
                    }
                    FUN_140022730(&DAT_140a285e0,piVar27);
                  }
                  break;
                }
                uVar32 = (int)plVar12 + 1;
                piVar17 = piVar17 + 10;
                plVar12 = (longlong *)(ulonglong)uVar32;
              } while ((int)uVar32 < iVar31);
            }
            *(int *)(param_1 + 0x238) = iVar9;
            plVar12 = plVar23;
            if ((*(longlong *)(param_1 + 600) != 0) &&
               (plVar12 = (longlong *)(param_1 + 0x248), 0xf < *(ulonglong *)(param_1 + 0x260))) {
              plVar12 = (longlong *)*plVar12;
            }
            pcVar29 = PTR_s_Data_Art_Models__140a28548;
            do {
              cVar8 = *pcVar29;
              pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
              pcVar29 = pcVar29 + 1;
            } while (cVar8 != '\0');
            puVar35 = &DAT_140a285e0;
            if (0xf < DAT_140a285f8) {
              puVar35 = DAT_140a285e0;
            }
            puVar26 = &uStack_1d9;
            do {
              pcVar29 = puVar26 + 1;
              puVar26 = puVar26 + 1;
            } while (*pcVar29 != '\0');
            lVar11 = 0;
            do {
              cVar8 = *(char *)((longlong)puVar35 + lVar11);
              puVar26[lVar11] = cVar8;
              lVar11 = lVar11 + 1;
            } while (cVar8 != '\0');
            plVar30 = (longlong *)FUN_14025ec10(local_1d8,plVar12);
            plVar12 = *(longlong **)(param_1 + 0x2a0);
            if (plVar12 != plVar30) {
              if (plVar12 != (longlong *)0x0) {
                (**(code **)(*plVar12 + 8))();
              }
              *(longlong **)(param_1 + 0x2a0) = plVar30;
              if (plVar30 != (longlong *)0x0) {
                (**(code **)*plVar30)(plVar30);
              }
            }
            plVar30 = *(longlong **)(param_1 + 0x2a0);
            plVar12 = plVar23;
            if (plVar30 != (longlong *)0x0) {
              (**(code **)*plVar30)(plVar30);
              plVar12 = plVar30;
            }
            lVar11 = FUN_1403718f0(param_1,1,0xffffffffffffffff);
            if ((lVar11 != 0) && (*(longlong *)(lVar11 + 0x1f0) != 0)) {
              pcVar29 = PTR_s_Data_Art_Models__140a28548;
              do {
                cVar8 = *pcVar29;
                pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8
                ;
                pcVar29 = pcVar29 + 1;
              } while (cVar8 != '\0');
              puVar35 = (undefined8 *)(lVar11 + 0x1e0);
              if (0xf < *(ulonglong *)(lVar11 + 0x1f8)) {
                puVar35 = (undefined8 *)*puVar35;
              }
              puVar26 = &uStack_1d9;
              do {
                pcVar29 = puVar26 + 1;
                puVar26 = puVar26 + 1;
              } while (*pcVar29 != '\0');
              lVar11 = 0;
              do {
                cVar8 = *(char *)((longlong)puVar35 + lVar11);
                puVar26[lVar11] = cVar8;
                lVar11 = lVar11 + 1;
              } while (cVar8 != '\0');
              plVar14 = (longlong *)FUN_14025ec10(local_1d8,0);
              plVar30 = *(longlong **)(param_1 + 0x2b0);
              if (plVar30 != plVar14) {
                if (plVar30 != (longlong *)0x0) {
                  (**(code **)(*plVar30 + 8))();
                }
                *(longlong **)(param_1 + 0x2b0) = plVar14;
                if (plVar14 != (longlong *)0x0) {
                  (**(code **)*plVar14)(plVar14);
                }
              }
            }
          }
        }
      }
      else if (iVar31 == 1) {
        lVar11 = FUN_140769c58(400);
        plVar30 = plVar23;
        if (lVar11 != 0) {
          local_res18 = *(longlong *)(param_1 + 0x46c);
          plVar30 = (longlong *)
                    FUN_1407a8860(lVar11,*(undefined4 *)(param_1 + 0x464),
                                  *(undefined4 *)(param_1 + 0x468),&local_res18);
        }
        lVar11 = FUN_140769c58(0x150);
        plVar14 = plVar23;
        if (lVar11 != 0) {
          plVar14 = (longlong *)FUN_140263a20(lVar11);
        }
        plVar25 = *(longlong **)(param_1 + 0x2a0);
        if (plVar25 != plVar14) {
          if (plVar25 != (longlong *)0x0) {
            (**(code **)(*plVar25 + 8))();
          }
          *(longlong **)(param_1 + 0x2a0) = plVar14;
          plVar25 = (longlong *)0x0;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
            plVar25 = *(longlong **)(param_1 + 0x2a0);
          }
        }
        FUN_140265d10(plVar25,plVar30);
        plVar30 = *(longlong **)(param_1 + 0x2a0);
        if (plVar30 != (longlong *)0x0) {
          (**(code **)*plVar30)(plVar30);
          plVar12 = plVar30;
        }
      }
      else {
        plVar12 = plVar23;
        if (iVar31 == 2) {
          lVar11 = FUN_140769c58(0x178);
          if (lVar11 != 0) {
            local_res18 = *(longlong *)(param_1 + 0x46c);
            plVar12 = (longlong *)
                      FUN_1407a9fc0(lVar11,*(undefined4 *)(param_1 + 0x464),
                                    *(undefined4 *)(param_1 + 0x468),&local_res18);
          }
          uVar13 = FUN_1401f9320(&local_208,param_1 + 0x454);
          FUN_1407aa210(plVar12,uVar13);
          lVar11 = FUN_140769c58(0x150);
          plVar14 = plVar23;
          if (lVar11 != 0) {
            plVar14 = (longlong *)FUN_140263a20(lVar11);
          }
          plVar25 = *(longlong **)(param_1 + 0x2a0);
          if (plVar25 != plVar14) {
            if (plVar25 != (longlong *)0x0) {
              (**(code **)(*plVar25 + 8))();
            }
            *(longlong **)(param_1 + 0x2a0) = plVar14;
            plVar25 = (longlong *)0x0;
            if (plVar14 != (longlong *)0x0) {
              (**(code **)*plVar14)(plVar14);
              plVar25 = *(longlong **)(param_1 + 0x2a0);
            }
          }
          FUN_140265d10(plVar25,plVar12);
          plVar14 = *(longlong **)(param_1 + 0x2a0);
          plVar12 = plVar30;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
            plVar12 = plVar14;
          }
        }
      }
      *(bool *)(param_1 + 0x32) =
           (*(longlong *)(param_1 + 0x228) - *(longlong *)(param_1 + 0x220)) / 0x28 == 0;
    }
  }
  else {
    plVar12 = plVar23;
    if ((local_res10 == 2) && (plVar12 = plVar30, *(char *)(param_1 + 0x33) == '\0')) {
      iVar9 = *(int *)(param_1 + 0x23c);
      if (iVar9 == 0) {
        if (*(longlong *)(param_1 + 0x210) == 0) {
          if (*(longlong **)(param_1 + 0x2a8) != (longlong *)0x0) {
            (**(code **)(**(longlong **)(param_1 + 0x2a8) + 8))();
            *(undefined8 *)(param_1 + 0x2a8) = 0;
          }
          if (*(longlong **)(param_1 + 0x2b8) != (longlong *)0x0) {
            (**(code **)(**(longlong **)(param_1 + 0x2b8) + 8))();
            *(undefined8 *)(param_1 + 0x2b8) = 0;
          }
        }
        else {
          pcVar29 = PTR_s_Data_Art_Models__140a28548;
          do {
            cVar8 = *pcVar29;
            pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
            pcVar29 = pcVar29 + 1;
          } while (cVar8 != '\0');
          puVar35 = (undefined8 *)(param_1 + 0x200);
          if (0xf < *(ulonglong *)(param_1 + 0x218)) {
            puVar35 = (undefined8 *)*puVar35;
          }
          puVar26 = &uStack_1d9;
          do {
            pcVar29 = puVar26 + 1;
            puVar26 = puVar26 + 1;
          } while (*pcVar29 != '\0');
          lVar11 = 0;
          do {
            cVar8 = *(char *)((longlong)puVar35 + lVar11);
            puVar26[lVar11] = cVar8;
            lVar11 = lVar11 + 1;
          } while (cVar8 != '\0');
          plVar12 = plVar23;
          if ((*(longlong *)(param_1 + 0x278) != 0) &&
             (plVar12 = (longlong *)(param_1 + 0x268), 0xf < *(ulonglong *)(param_1 + 0x280))) {
            plVar12 = (longlong *)*plVar12;
          }
          plVar30 = (longlong *)FUN_14025ec10(local_1d8,plVar12);
          plVar12 = *(longlong **)(param_1 + 0x2a8);
          if (plVar12 != plVar30) {
            if (plVar12 != (longlong *)0x0) {
              (**(code **)(*plVar12 + 8))();
            }
            *(longlong **)(param_1 + 0x2a8) = plVar30;
            if (plVar30 != (longlong *)0x0) {
              (**(code **)*plVar30)(plVar30);
            }
          }
          plVar30 = *(longlong **)(param_1 + 0x2a8);
          plVar12 = plVar23;
          if (plVar30 != (longlong *)0x0) {
            (**(code **)*plVar30)(plVar30);
            plVar12 = plVar30;
          }
          lVar11 = FUN_1403718f0(param_1,1,0xffffffff);
          if ((lVar11 != 0) && (*(longlong *)(lVar11 + 0x210) != 0)) {
            pcVar29 = PTR_s_Data_Art_Models__140a28548;
            do {
              cVar8 = *pcVar29;
              pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
              pcVar29 = pcVar29 + 1;
            } while (cVar8 != '\0');
            puVar35 = (undefined8 *)(lVar11 + 0x200);
            if (0xf < *(ulonglong *)(lVar11 + 0x218)) {
              puVar35 = (undefined8 *)*puVar35;
            }
            puVar26 = &uStack_1d9;
            do {
              pcVar29 = puVar26 + 1;
              puVar26 = puVar26 + 1;
            } while (*pcVar29 != '\0');
            lVar11 = 0;
            do {
              cVar8 = *(char *)((longlong)puVar35 + lVar11);
              puVar26[lVar11] = cVar8;
              lVar11 = lVar11 + 1;
            } while (cVar8 != '\0');
            plVar14 = (longlong *)FUN_14025ec10(local_1d8,0);
            plVar30 = *(longlong **)(param_1 + 0x2b8);
            if (plVar30 != plVar14) {
              if (plVar30 != (longlong *)0x0) {
                (**(code **)(*plVar30 + 8))();
              }
              *(longlong **)(param_1 + 0x2b8) = plVar14;
              if (plVar14 != (longlong *)0x0) {
                (**(code **)*plVar14)(plVar14);
              }
            }
          }
          cVar8 = FUN_14028afb0(&DAT_140b153e0);
          if ((cVar8 == '\x01') && (*(longlong *)(param_1 + 0x8c0) != 0)) {
            FUN_140375380(*(longlong *)(param_1 + 0x8c0),2);
          }
        }
      }
      else if (iVar9 == 1) {
        lVar11 = FUN_140769c58(400);
        plVar12 = plVar23;
        if (lVar11 != 0) {
          local_res18 = *(longlong *)(param_1 + 0x46c);
          plVar12 = (longlong *)
                    FUN_1407a8860(lVar11,*(undefined4 *)(param_1 + 0x464),
                                  *(undefined4 *)(param_1 + 0x468),&local_res18);
        }
        lVar11 = FUN_140769c58(0x150);
        plVar14 = plVar23;
        if (lVar11 != 0) {
          plVar14 = (longlong *)FUN_140263a20(lVar11);
        }
        plVar25 = *(longlong **)(param_1 + 0x2a8);
        if (plVar25 != plVar14) {
          if (plVar25 != (longlong *)0x0) {
            (**(code **)(*plVar25 + 8))();
          }
          *(longlong **)(param_1 + 0x2a8) = plVar14;
          plVar25 = (longlong *)0x0;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
            plVar25 = *(longlong **)(param_1 + 0x2a8);
          }
        }
        FUN_140265d10(plVar25,plVar12);
        plVar14 = *(longlong **)(param_1 + 0x2a8);
        plVar12 = plVar30;
        if (plVar14 != (longlong *)0x0) {
          (**(code **)*plVar14)(plVar14);
          plVar12 = plVar14;
        }
      }
      else {
        plVar12 = plVar23;
        if (iVar9 == 2) {
          lVar11 = FUN_140769c58(0x178);
          if (lVar11 != 0) {
            local_res18 = *(longlong *)(param_1 + 0x46c);
            plVar12 = (longlong *)
                      FUN_1407a9fc0(lVar11,*(undefined4 *)(param_1 + 0x464),
                                    *(undefined4 *)(param_1 + 0x468),&local_res18);
          }
          uVar13 = FUN_1401f9320(&local_208,param_1 + 0x454);
          FUN_1407aa210(plVar12,uVar13);
          lVar11 = FUN_140769c58(0x150);
          plVar14 = plVar23;
          if (lVar11 != 0) {
            plVar14 = (longlong *)FUN_140263a20(lVar11);
          }
          plVar25 = *(longlong **)(param_1 + 0x2a8);
          if (plVar25 != plVar14) {
            if (plVar25 != (longlong *)0x0) {
              (**(code **)(*plVar25 + 8))();
            }
            *(longlong **)(param_1 + 0x2a8) = plVar14;
            plVar25 = (longlong *)0x0;
            if (plVar14 != (longlong *)0x0) {
              (**(code **)*plVar14)(plVar14);
              plVar25 = *(longlong **)(param_1 + 0x2a8);
            }
          }
          FUN_140265d10(plVar25,plVar12);
          plVar14 = *(longlong **)(param_1 + 0x2a8);
          plVar12 = plVar30;
          if (plVar14 != (longlong *)0x0) {
            (**(code **)*plVar14)(plVar14);
            plVar12 = plVar14;
          }
        }
      }
      *(undefined1 *)(param_1 + 0x33) = 1;
    }
  }
  plVar30 = plVar12;
  if (plVar12 != (longlong *)0x0) {
    if (*(char *)(param_1 + 0x34) == '\0') {
      lVar11 = (longlong)*(int *)(param_1 + 0xe40);
      plVar14 = plVar23;
      local_res18 = lVar11;
      if (0 < lVar11) {
        do {
          lVar18 = *(longlong *)(*(longlong *)(param_1 + 0xe38) + (longlong)plVar14 * 8);
          uVar15 = FUN_1402648b0(plVar12);
          local_res20 = uVar15;
          if (*(longlong *)(lVar18 + 0x70) != 0) {
            pcVar29 = PTR_s_Data_Art_Models__140a28548;
            do {
              cVar8 = *pcVar29;
              pcVar29[(longlong)(local_1d8 + -(longlong)PTR_s_Data_Art_Models__140a28548)] = cVar8;
              pcVar29 = pcVar29 + 1;
            } while (cVar8 != '\0');
            puVar35 = (undefined8 *)(lVar18 + 0x60);
            if (0xf < *(ulonglong *)(lVar18 + 0x78)) {
              puVar35 = (undefined8 *)*puVar35;
            }
            puVar26 = &uStack_1d9;
            do {
              pcVar29 = puVar26 + 1;
              puVar26 = puVar26 + 1;
            } while (*pcVar29 != '\0');
            lVar11 = 0;
            do {
              cVar8 = *(char *)((longlong)puVar35 + lVar11);
              puVar26[lVar11] = cVar8;
              lVar11 = lVar11 + 1;
            } while (cVar8 != '\0');
            lVar11 = *(longlong *)(param_1 + 0x2c8);
            plVar16 = (longlong *)FUN_14025ec10(local_1d8,0);
            plVar25 = *(longlong **)(lVar11 + (longlong)plVar14 * 8);
            if (plVar25 != plVar16) {
              if (plVar25 != (longlong *)0x0) {
                (**(code **)(*plVar25 + 8))();
              }
              *(longlong **)(lVar11 + (longlong)plVar14 * 8) = plVar16;
              if (plVar16 != (longlong *)0x0) {
                (**(code **)*plVar16)(plVar16);
              }
            }
            uVar15 = local_res20;
            lVar11 = local_res18;
            if (*(longlong *)(*(longlong *)(param_1 + 0x2c8) + (longlong)plVar14 * 8) != 0) {
              plVar25 = (longlong *)(lVar18 + 0x80);
              if (0xf < *(ulonglong *)(lVar18 + 0x98)) {
                plVar25 = (longlong *)*plVar25;
              }
              iVar9 = FUN_14012d2a0(local_res20,plVar25);
              lVar11 = local_res18;
              if (-1 < iVar9) {
                uVar13 = FUN_140264a30(*(undefined8 *)
                                        (*(longlong *)(param_1 + 0x2c8) + (longlong)plVar14 * 8));
                FUN_14012d770(uVar15,uVar13,iVar9);
                lVar11 = local_res18;
              }
            }
          }
          if (*(longlong *)(lVar18 + 0xd0) != 0) {
            plVar25 = (longlong *)(lVar18 + 0xc0);
            if (0xf < *(ulonglong *)(lVar18 + 0xd8)) {
              plVar25 = (longlong *)*plVar25;
            }
            uVar10 = FUN_14012d520(uVar15,plVar25);
            lVar18 = FUN_14012d6f0(uVar15,uVar10);
            if (lVar18 != 0) {
              *(byte *)(lVar18 + 0x30) = *(byte *)(lVar18 + 0x30) | 1;
            }
          }
          plVar14 = (longlong *)((longlong)plVar14 + 1);
        } while ((longlong)plVar14 < lVar11);
      }
      *(undefined1 *)(param_1 + 0x34) = 1;
    }
    FUN_14001f910(param_1 + 0xe60);
    plVar14 = plVar23;
    if (0 < *(int *)(param_1 + 0xe58)) {
      do {
        puVar35 = (undefined8 *)(*(longlong *)(param_1 + 0xe50) + (longlong)plVar14);
        if (0xf < (ulonglong)puVar35[3]) {
          puVar35 = (undefined8 *)*puVar35;
        }
        uVar13 = FUN_1402648b0(plVar12);
        iVar9 = FUN_14012d2a0(uVar13,puVar35);
        if (-1 < iVar9) {
          iVar31 = *(int *)(param_1 + 0xe70);
          iVar24 = *(int *)(param_1 + 0xe74) * 2 >> 1;
          if (iVar24 <= iVar31) {
            iVar31 = iVar24 / 10;
            if (iVar24 / 10 < 10) {
              iVar31 = 10;
            }
            FUN_140021ec0(param_1 + 0xe60,iVar31 + iVar24,0,1);
            iVar31 = *(int *)(param_1 + 0xe70);
          }
          *(int *)(*(longlong *)(param_1 + 0xe68) + (longlong)iVar31 * 4) = iVar9;
          *(int *)(param_1 + 0xe70) = *(int *)(param_1 + 0xe70) + 1;
        }
        uVar32 = (int)plVar23 + 1;
        plVar23 = (longlong *)(ulonglong)uVar32;
        plVar14 = plVar14 + 4;
      } while ((int)uVar32 < *(int *)(param_1 + 0xe58));
    }
    puVar33 = (ulonglong *)0x0;
    pvVar20 = *(LPVOID *)(param_1 + 0x2390);
    if (pvVar20 != (LPVOID)0x0) {
      if ((*(uint *)(param_1 + 0x239c) & 0x80000000) == 0) {
        thunk_FUN_1407864b8(pvVar20);
      }
      else {
        pvVar19 = GetProcessHeap();
        HeapFree(pvVar19,0,pvVar20);
      }
      *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) & 0x80000000;
      *(undefined8 *)(param_1 + 0x2390) = 0;
      *(undefined4 *)(param_1 + 0x2398) = 0;
    }
    if (0 < *(int *)(param_1 + 0x2380)) {
      local_res20 = 0;
      puVar28 = puVar33;
      do {
        uVar15 = local_res20;
        puVar35 = (undefined8 *)(*(longlong *)(param_1 + 0x2378) + local_res20);
        if (0xf < (ulonglong)puVar35[3]) {
          puVar35 = (undefined8 *)*puVar35;
        }
        uVar13 = FUN_1402648b0(plVar12);
        iVar9 = FUN_14012d2a0(uVar13,puVar35);
        local_res18 = CONCAT44(local_res18._4_4_,iVar9);
        if (-1 < iVar9) {
          iVar9 = *(int *)(param_1 + 0x239c);
          uVar32 = *(uint *)(param_1 + 0x2398);
          iVar31 = iVar9 * 2 >> 1;
          if (iVar31 <= (int)uVar32) {
            iVar24 = iVar31 / 10;
            if (iVar31 / 10 < 10) {
              iVar24 = 10;
            }
            pvVar20 = *(LPVOID *)(param_1 + 0x2390);
            uVar34 = iVar31 + iVar24;
            if (uVar34 == 0) {
              puVar22 = (ulonglong *)(ulonglong)uVar32;
              if (pvVar20 != (LPVOID)0x0) {
                if (iVar9 < 0) {
                  pvVar19 = GetProcessHeap();
                  HeapFree(pvVar19,0,pvVar20);
                }
                else {
                  thunk_FUN_1407864b8(pvVar20);
                }
                *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) & 0x80000000;
                *(undefined8 *)(param_1 + 0x2390) = 0;
                *(undefined4 *)(param_1 + 0x2398) = 0;
                puVar22 = puVar33;
              }
            }
            else {
              uVar15 = (ulonglong)(int)uVar34;
              if (pvVar20 == (LPVOID)0x0) {
                if (iVar9 < 0) {
                  pvVar19 = GetProcessHeap();
                  pvVar20 = HeapAlloc(pvVar19,0,uVar15 * 4);
                }
                else {
                  auVar3._8_8_ = 0;
                  auVar3._0_8_ = uVar15;
                  uVar13 = SUB168(ZEXT816(4) * auVar3,0);
                  if (SUB168(ZEXT816(4) * auVar3,8) != 0) {
                    uVar13 = 0xffffffffffffffff;
                  }
                  pvVar20 = (LPVOID)thunk_FUN_140769c58(uVar13);
                }
                *(LPVOID *)(param_1 + 0x2390) = pvVar20;
                *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) & 0x80000000;
                puVar22 = (ulonglong *)0x0;
                *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) | uVar34 & 0x7fffffff;
                *(undefined4 *)(param_1 + 0x2398) = 0;
              }
              else {
                if (iVar9 < 0) {
                  pvVar19 = GetProcessHeap();
                  pvVar20 = HeapReAlloc(pvVar19,0,pvVar20,uVar15 * 4);
                }
                else {
                  auVar4._8_8_ = 0;
                  auVar4._0_8_ = uVar15;
                  uVar13 = SUB168(ZEXT816(4) * auVar4,0);
                  if (SUB168(ZEXT816(4) * auVar4,8) != 0) {
                    uVar13 = 0xffffffffffffffff;
                  }
                  pvVar20 = (LPVOID)thunk_FUN_140769c58(uVar13);
                  uVar32 = *(int *)(param_1 + 0x239c) * 2 >> 1;
                  if ((int)uVar34 < (int)uVar32) {
                    uVar32 = uVar34;
                  }
                  puVar22 = puVar33;
                  if (0 < (int)uVar32) {
                    do {
                      *(undefined4 *)((longlong)pvVar20 + (longlong)puVar22 * 4) =
                           *(undefined4 *)(*(longlong *)(param_1 + 0x2390) + (longlong)puVar22 * 4);
                      puVar22 = (ulonglong *)((longlong)puVar22 + 1);
                    } while ((longlong)puVar22 < (longlong)(int)uVar32);
                  }
                  thunk_FUN_1407864b8(*(undefined8 *)(param_1 + 0x2390));
                }
                *(LPVOID *)(param_1 + 0x2390) = pvVar20;
                *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) & 0x80000000;
                *(uint *)(param_1 + 0x239c) = *(uint *)(param_1 + 0x239c) | uVar34 & 0x7fffffff;
                if ((int)*(uint *)(param_1 + 0x2398) < (int)uVar34) {
                  uVar34 = *(uint *)(param_1 + 0x2398);
                }
                puVar22 = (ulonglong *)(ulonglong)uVar34;
                *(uint *)(param_1 + 0x2398) = uVar34;
              }
            }
            uVar32 = (uint)puVar22;
            uVar15 = local_res20;
          }
          *(undefined4 *)(*(longlong *)(param_1 + 0x2390) + (longlong)(int)uVar32 * 4) =
               (undefined4)local_res18;
          *(int *)(param_1 + 0x2398) = *(int *)(param_1 + 0x2398) + 1;
        }
        local_res20 = uVar15 + 0x20;
        uVar32 = (int)puVar28 + 1;
        puVar28 = (ulonglong *)(ulonglong)uVar32;
      } while ((int)uVar32 < *(int *)(param_1 + 0x2380));
    }
    if (*(int *)(param_1 + 0x23a8) != 0) {
      uVar13 = FUN_1402648b0(plVar12);
      iVar9 = FUN_14012dca0(uVar13);
      if (0 < iVar9) {
        uVar15 = (ulonglong)iVar9;
        auVar5._8_8_ = 0;
        auVar5._0_8_ = uVar15;
        uVar21 = SUB168(ZEXT816(0x18) * auVar5,0);
        if (SUB168(ZEXT816(0x18) * auVar5,8) != 0) {
          uVar21 = 0xffffffffffffffff;
        }
        lVar11 = uVar21 + 8;
        if (0xfffffffffffffff7 < uVar21) {
          lVar11 = -1;
        }
        local_res20 = uVar15;
        puVar22 = (ulonglong *)thunk_FUN_140769c58(lVar11);
        puVar28 = puVar33;
        if (puVar22 != (ulonglong *)0x0) {
          puVar28 = puVar22 + 1;
          *puVar22 = uVar15;
          puVar22 = puVar28;
          uVar21 = uVar15;
          do {
            *puVar22 = (ulonglong)DynamicVectorClass<int>::vftable;
            puVar22[1] = 0;
            puVar22[2] = 0;
            puVar22 = puVar22 + 3;
            uVar21 = uVar21 - 1;
          } while (uVar21 != 0);
        }
        *(ulonglong **)(param_1 + 0x23a0) = puVar28;
        if (0 < (longlong)uVar15) {
          local_res18 = 0;
          do {
            lVar11 = local_res18;
            iVar9 = 0;
            if (0 < *(int *)(param_1 + 0x23a8)) {
              do {
                pcVar29 = "FP_0%d_ALT%d";
                if (9 < iVar9) {
                  pcVar29 = "FP_%d_ALT%d";
                }
                FUN_140048ec0(local_c8,pcVar29,iVar9,puVar33);
                uVar13 = FUN_1402648b0(plVar12);
                uVar10 = FUN_14012d2a0(uVar13,local_c8);
                lVar18 = *(longlong *)(param_1 + 0x23a0) + lVar11;
                iVar31 = *(int *)(lVar18 + 0x10);
                iVar24 = *(int *)(lVar18 + 0x14) * 2 >> 1;
                if (iVar24 <= iVar31) {
                  iVar31 = iVar24 / 10;
                  if (iVar24 / 10 < 10) {
                    iVar31 = 10;
                  }
                  FUN_140021ec0(lVar18,iVar31 + iVar24,0,1);
                  iVar31 = *(int *)(lVar18 + 0x10);
                }
                iVar9 = iVar9 + 1;
                *(undefined4 *)(*(longlong *)(lVar18 + 8) + (longlong)iVar31 * 4) = uVar10;
                *(int *)(lVar18 + 0x10) = *(int *)(lVar18 + 0x10) + 1;
                uVar15 = local_res20;
              } while (iVar9 < *(int *)(param_1 + 0x23a8));
            }
            local_res18 = local_res18 + 0x18;
            puVar33 = (ulonglong *)(ulonglong)((int)puVar33 + 1);
            uVar15 = uVar15 - 1;
            local_res20 = uVar15;
          } while (uVar15 != 0);
        }
      }
    }
    lVar11 = 0;
    FUN_140265950(plVar12,PTR_s_STEALTH_140a15d60,1);
    FUN_140265950(plVar12,PTR_s_buildup_140a15d68,1);
    FUN_140265950(plVar12,PTR_s_charged_140a15d70,1);
    if ((((*(byte *)(*(longlong *)(param_1 + 0x760) + 4) & 0x40) != 0) && (local_res10 == 1)) &&
       (*(char *)(param_1 + 0x1b) != '\0')) {
      local_1e0 = 0;
      local_1f0[0] = 0;
      local_1f0[1] = 0;
      uVar13 = FUN_1402648b0(plVar12);
      iVar9 = FUN_14012d5d0(uVar13,PTR_s_CRUSHER_140a28558,local_1f0);
      lVar18 = lVar11;
      if (0 < (longlong)iVar9) {
        do {
          uVar10 = *(undefined4 *)(local_1f0[0] + lVar18 * 4);
          uVar13 = FUN_1402648b0(plVar12);
          plVar23 = (longlong *)FUN_14012d6f0(uVar13,uVar10);
          if (plVar23 != (longlong *)0x0) {
            (**(code **)(*plVar23 + 0x50))(plVar23,&local_208);
            plVar23 = (longlong *)(param_1 + 0x2d8);
            puVar1 = *(undefined4 **)(param_1 + 0x2e0);
            if (puVar1 == *(undefined4 **)(param_1 + 0x2e8)) {
              FUN_140366da0(plVar23,puVar1,&local_208);
            }
            else {
              *puVar1 = local_208;
              puVar1[1] = local_204;
              puVar1[2] = local_200;
              puVar1[3] = local_1fc;
              puVar1[4] = local_1f8;
              puVar1[5] = local_1f4;
              *(undefined4 **)(param_1 + 0x2e0) = puVar1 + 6;
            }
            lVar2 = *plVar23;
            dVar6 = (double)FUN_1407760bc(SUB84((double)((*(float *)(lVar2 + 0x10 + lVar11) +
                                                         *(float *)(lVar2 + 4 + lVar11)) -
                                                        (*(float *)(lVar2 + 4 + lVar11) -
                                                        *(float *)(lVar2 + 0x10 + lVar11))),0));
            lVar2 = *plVar23;
            dVar7 = (double)FUN_1407760bc(SUB84((double)((*(float *)(lVar2 + 0xc + lVar11) +
                                                         *(float *)(lVar2 + lVar11)) -
                                                        (*(float *)(lVar2 + lVar11) -
                                                        *(float *)(lVar2 + 0xc + lVar11))),0));
            plVar23 = (longlong *)(param_1 + 0xf8);
            if (0xf < *(ulonglong *)(param_1 + 0x110)) {
              plVar23 = (longlong *)*plVar23;
            }
            FUN_140025760("%s: Crusher width = %.2f, length = %.2f\n",plVar23,(double)(float)dVar7,
                          (double)(float)dVar6);
          }
          lVar18 = lVar18 + 1;
          lVar11 = lVar11 + 0x18;
        } while (lVar18 < iVar9);
      }
      if (local_1f0[0] != 0) {
        uVar15 = local_1e0 - local_1f0[0] & 0xfffffffffffffffc;
        if (0xfff < uVar15) {
          if (0x1f < (local_1f0[0] - *(longlong *)(local_1f0[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_1f0[0] + -8),uVar15 + 0x27);
          }
        }
        thunk_FUN_1407864b8();
      }
    }
  }
LAB_14037689d:
  uVar21 = 0;
  uVar15 = uVar21;
  if (0 < *(int *)(param_1 + 0xd28)) {
    do {
      lVar11 = *(longlong *)(*(longlong *)(param_1 + 0xd20) + 8 + uVar21);
      if ((lVar11 != 0) && (lVar11 != param_1)) {
        plVar12 = (longlong *)(lVar11 + 0xf8);
        if (0xf < *(ulonglong *)(lVar11 + 0x110)) {
          plVar12 = (longlong *)*plVar12;
        }
        plVar23 = (longlong *)(param_1 + 0xf8);
        if (0xf < *(ulonglong *)(param_1 + 0x110)) {
          plVar23 = (longlong *)*plVar23;
        }
        FUN_140025760("%s: Loading additional assets for TSW replacement object type %s\n",plVar23,
                      plVar12);
        FUN_140375380(lVar11,local_res10);
      }
      uVar32 = (int)uVar15 + 1;
      uVar21 = uVar21 + 0x10;
      uVar15 = (ulonglong)uVar32;
    } while ((int)uVar32 < *(int *)(param_1 + 0xd28));
  }
  if (plVar30 != (longlong *)0x0) {
    (**(code **)(*plVar30 + 8))(plVar30);
  }
  return;
}

