
// ================================================================
// FUN_14004b250 — WINDOW lines 200-450 (seeking FUN_1404889d0 call)
// ================================================================

// Total lines: 1509, showing 200 to 450
      if (plVar8 == (longlong *)0x0) {
        pppppppuVar12 = local_58;
        if (0xf < local_40) {
          pppppppuVar12 = local_58[0];
        }
        FUN_140025760("Failed to load splash screen %s\n",pppppppuVar12);
      }
      else {
        (**(code **)*plVar8)(plVar8);
        lVar14 = FUN_140769c58(0x90);
        uVar9 = uVar15;
        if (lVar14 != 0) {
          uVar6 = CONCAT44((int)((ulonglong)uVar6 >> 0x20),DAT_1407ffaf8);
          plVar19 = plVar8;
          uVar9 = FUN_14025ff10(lVar14,0,0,DAT_1407ffaf8,uVar6,plVar8);
        }
        DAT_140a61a50 = uVar9;
        (**(code **)(*plVar8 + 8))(plVar8);
        FUN_14025c080(DAT_140a619f0,DAT_140a61a50);
      }
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      if (0xf < local_40) {
        if (0xfff < local_40 + 1) {
          if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      pcVar11 = (char *)FUN_14004fba0();
      if (*pcVar11 == '\x01') {
        uVar7 = FUN_14004fba0();
        FUN_140263300(uVar7);
      }
      iVar5 = FUN_14048b800(&DAT_140b31330);
      uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
      uVar9 = uVar15;
      if (0 < iVar5) {
        do {
          puVar10 = (undefined8 *)FUN_14048b7d0(&DAT_140b31330,uVar9);
          uVar20 = (undefined4)((ulonglong)uVar6 >> 0x20);
          uVar21 = (undefined4)((ulonglong)plVar19 >> 0x20);
          if (puVar10 != (undefined8 *)0x0) {
            plVar8 = puVar10 + 5;
            if (0xf < (ulonglong)puVar10[8]) {
              plVar8 = (longlong *)*plVar8;
            }
            puVar17 = puVar10;
            if (0xf < (ulonglong)puVar10[3]) {
              puVar17 = (undefined8 *)*puVar10;
            }
            uVar7 = FUN_14004fba0();
            plVar19 = (longlong *)CONCAT44(uVar21,*(undefined4 *)(puVar10 + 10));
            uVar6 = CONCAT44(uVar20,*(undefined4 *)((longlong)puVar10 + 0x4c));
            FUN_140262a00(uVar7,puVar17,plVar8,*(undefined4 *)(puVar10 + 9),uVar6,plVar19);
          }
          uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
          uVar13 = (int)uVar9 + 1;
          uVar9 = (ulonglong)uVar13;
        } while ((int)uVar13 < iVar5);
      }
      uVar6 = FUN_14004fba0();
      FUN_1402625a0(uVar6);
      uVar6 = FUN_14004fba0();
      FUN_140262490(uVar6,0);
      FUN_14020e010();
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","KeyboardClass::Init",uVar7,uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      FUN_140242cd0("Data/Scripts/Library/");
      FUN_140242cd0("Data/Scripts/GameObject/");
      FUN_140242cd0("Data/Scripts/FreeStore/");
      FUN_140242cd0("Data/Scripts/Miscellaneous/");
      FUN_140242cd0("Data/Scripts/Story/");
      uVar6 = FUN_14001fb80();
      cVar3 = FUN_14007d100(uVar6);
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheConsole::Get().System_Initialize",uVar7,
                    uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      if (cVar3 == '\0') {
        pcVar11 = "Error: Unable to initialize TheConsole.";
      }
      else {
        cVar3 = FUN_14021c2f0(&DAT_140a7d0a0,DAT_140a7c090,DAT_140a61ab6);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudio.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          FUN_1402149d0("Error: Unable to successfully initialize the TheAudio system.");
        }
        cVar3 = FUN_1401f9a60(&DAT_140a7bc58,DAT_140a7c090,
                              PTR_s___Data_Text_MasterTextFile_txt_140a10768);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameText.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          pcVar11 = "Error: Unable to initialize TheGameText.";
        }
        else {
          FUN_14004f150();
          cVar3 = FUN_1401f9a60(&DAT_140a7bc28,DAT_140a7c090,
                                PTR_s___Data_Text_CreditsText_txt_140a10770);
          DVar4 = timeGetTime();
          uVar9 = FUN_140216790();
          uVar6 = FUN_140216710();
          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheCreditsText.System_Initialize",uVar6,
                        uVar9,CONCAT44(uVar20,DVar4));
          if (cVar3 == '\0') {
            pcVar11 = "Error: Unable to initialize TheCreditsText.";
          }
          else {
            uVar20 = 1;
            cVar3 = FUN_1401c7bb0(&DAT_140a78510,PTR_s___Data_Resources_GUIDialog_GUIDi_140a10750,
                                  PTR_s___Data_Resources_GUIDialog_Resou_140a10758,
                                  PTR_s___Data_XML_GUIDialogs_xml_140a10760,
                                  uVar9 & 0xffffffffffffff00,FUN_140037840);
            DVar4 = timeGetTime();
            uVar6 = FUN_140216790();
            uVar7 = FUN_140216710();
            uVar18 = CONCAT44(uVar20,DVar4);
            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGUIDialogManager.System_Initialize",
                          uVar7,uVar6,uVar18);
            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
            if (cVar3 == '\0') {
              pcVar11 = "Error: Unable to initialize TheGUIDialogManager.";
            }
            else {
              _DAT_140a79880 = FUN_140037eb0;
              lVar14 = -1;
              local_58[0] = (undefined8 *******)0x0;
              local_48 = 0;
              local_40 = 0xf;
              do {
                lVar14 = lVar14 + 1;
              } while (PTR_s___Data_XML_SFXEventFiles_xml_140a10780[lVar14] != '\0');
              FUN_140022730(local_58);
              cVar3 = FUN_1402d5df0(&DAT_140b27e60,local_58,0);
              if (0xf < local_40) {
                if (0xfff < local_40 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])))
                  {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
              DVar4 = timeGetTime();
              uVar6 = FUN_140216790();
              uVar7 = FUN_140216710();
              uVar18 = CONCAT44(uVar20,DVar4);
              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheSFXEventManager.System_Initialize",
                            uVar7,uVar6,uVar18);
              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
              if (cVar3 == '\0') {
                pcVar11 = "Error: Unable to initialize TheSFXEventManager.";
              }
              else {
                lVar14 = -1;
                local_58[0] = (undefined8 *******)0x0;
                local_48 = 0;
                local_40 = 0xf;
                do {
                  lVar14 = lVar14 + 1;
                } while (PTR_s___Data_XML_TerrainDecalFX_xml_140a10790[lVar14] != '\0');
                FUN_140022730(local_58);
                cVar3 = FUN_140470ef0(&DAT_140a2a020,local_58,0);
                if (0xf < local_40) {
                  if (0xfff < local_40 + 1) {
                    if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))
                       ) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                    }
                  }
                  thunk_FUN_1407864b8();
                }
                DVar4 = timeGetTime();
                uVar6 = FUN_140216790();
                uVar7 = FUN_140216710();
                uVar18 = CONCAT44(uVar20,DVar4);
                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                              "TheTerrainDecalFXManager.System_Initialize",uVar7,uVar6,uVar18);
                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                if (cVar3 == '\0') {
                  pcVar11 = "Error: Unable to initialize TheTerrainDecalFXManager.";
                }
                else {
                  lVar14 = -1;
                  local_58[0] = (undefined8 *******)0x0;
                  local_48 = 0;
                  local_40 = 0xf;
                  do {
                    lVar14 = lVar14 + 1;
                  } while (PTR_s___Data_XML_DynamicTrackFX_xml_140a107a0[lVar14] != '\0');
                  FUN_140022730(local_58);
                  cVar3 = FUN_14046ffe0(&DAT_140a29f60,local_58,0);
                  if (0xf < local_40) {
                    if (0xfff < local_40 + 1) {
                      if (0x1f < (ulonglong)
                                 ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                      }
                    }
                    thunk_FUN_1407864b8();
                  }
                  DVar4 = timeGetTime();
                  uVar6 = FUN_140216790();
                  uVar7 = FUN_140216710();
                  uVar18 = CONCAT44(uVar20,DVar4);
                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                "TheDynamicTrackFXManager.System_Initialize",uVar7,uVar6,uVar18);
                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                  if (cVar3 == '\0') {
                    pcVar11 = "Error: Unable to initialize TheDynamicTrackFXManager.";
                  }
                  else {
                    lVar14 = -1;
                    local_58[0] = (undefined8 *******)0x0;
                    local_48 = 0;
                    local_40 = 0xf;
                    do {
                      lVar14 = lVar14 + 1;
                    } while (PTR_s___Data_XML_StarWars3DTextCrawl_x_140a107c0[lVar14] != '\0');
                    FUN_140022730(local_58);
                    cVar3 = FUN_140489d30(&DAT_140a2a9c0,local_58,0);


// ================================================================
// FUN_14004b250 — WINDOW lines 450-700
// ================================================================

// Total lines: 1509, showing 450 to 700
                    if (0xf < local_40) {
                      if (0xfff < local_40 + 1) {
                        if (0x1f < (ulonglong)
                                   ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                          FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                        }
                      }
                      thunk_FUN_1407864b8();
                    }
                    DVar4 = timeGetTime();
                    uVar6 = FUN_140216790();
                    uVar7 = FUN_140216710();
                    uVar18 = CONCAT44(uVar20,DVar4);
                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                  "TheDraw3DTextCrawlManager.System_Initialize",uVar7,uVar6,uVar18);
                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                    if (cVar3 == '\0') {
                      pcVar11 = "Error: Unable to initialize TheDraw3DTextCrawlManager.";
                    }
                    else {
                      lVar14 = -1;
                      local_58[0] = (undefined8 *******)0x0;
                      local_48 = 0;
                      local_40 = 0xf;
                      do {
                        lVar14 = lVar14 + 1;
                      } while (PTR_s___Data_XML_SurfaceFX_xml_140a10788[lVar14] != '\0');
                      FUN_140022730(local_58);
                      cVar3 = FUN_140470750(&DAT_140a29fa0,local_58,0);
                      if (0xf < local_40) {
                        if (0xfff < local_40 + 1) {
                          if (0x1f < (ulonglong)
                                     ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                          }
                        }
                        thunk_FUN_1407864b8();
                      }
                      DVar4 = timeGetTime();
                      uVar6 = FUN_140216790();
                      uVar7 = FUN_140216710();
                      uVar18 = CONCAT44(uVar20,DVar4);
                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                    "TheSurfaceFXEventManager.System_Initialize",uVar7,uVar6,uVar18)
                      ;
                      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                      if (cVar3 == '\0') {
                        pcVar11 = "Error: Unable to initialize TheSurfaceFXEventManager.";
                      }
                      else {
                        lVar14 = -1;
                        local_58[0] = (undefined8 *******)0x0;
                        local_48 = 0;
                        local_40 = 0xf;
                        do {
                          lVar14 = lVar14 + 1;
                        } while (PTR_s___Data_XML_ShadowBlobMaterials_x_140a107a8[lVar14] != '\0');
                        FUN_140022730(local_58);
                        cVar3 = FUN_1404280b0(&DAT_140b2f170,local_58,0);
                        if (0xf < local_40) {
                          if (0xfff < local_40 + 1) {
                            if (0x1f < (ulonglong)
                                       ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                            }
                          }
                          thunk_FUN_1407864b8();
                        }
                        DVar4 = timeGetTime();
                        uVar6 = FUN_140216790();
                        uVar7 = FUN_140216710();
                        uVar18 = CONCAT44(uVar20,DVar4);
                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                      "TheShadowBlobManager.System_Initialize",uVar7,uVar6,uVar18);
                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                        if (cVar3 == '\0') {
                          pcVar11 = "Error: Unable to initialize TheShadowBlobManager.";
                        }
                        else {
                          FUN_140427e20(&DAT_140b2f170,0);
                          cVar3 = FUN_14047ffe0(PTR_s___Data_XML_TacticalCameras_xml_140a107b0);
                          DVar4 = timeGetTime();
                          uVar6 = FUN_140216790();
                          uVar7 = FUN_140216710();
                          uVar18 = CONCAT44(uVar20,DVar4);
                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                        "TacticalCameraConstantsManager::System_Initialize",uVar7,
                                        uVar6,uVar18);
                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                          if (cVar3 == '\0') {
                            pcVar11 = "Error: Unable to initialize TacticalCameraConstantsManager.";
                          }
                          else {
                            lVar14 = -1;
                            local_58[0] = (undefined8 *******)0x0;
                            local_48 = 0;
                            local_40 = 0xf;
                            do {
                              lVar14 = lVar14 + 1;
                            } while (PTR_s___Data_XML_RadarMap_xml_140a10838[lVar14] != '\0');
                            FUN_140022730(local_58);
                            cVar3 = FUN_140359390(&DAT_140b2bcb0,local_58,0);
                            if (0xf < local_40) {
                              if (0xfff < local_40 + 1) {
                                if (0x1f < (ulonglong)
                                           ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])
                                           )) {
                    /* WARNING: Subroutine does not return */
                                  FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                }
                              }
                              thunk_FUN_1407864b8();
                            }
                            DVar4 = timeGetTime();
                            uVar6 = FUN_140216790();
                            uVar7 = FUN_140216710();
                            uVar18 = CONCAT44(uVar20,DVar4);
                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                          "TheRadarMap.System_Initialize",uVar7,uVar6,uVar18);
                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                            if (cVar3 == '\0') {
                              pcVar11 = "Error: Unable to initialize TheRadarMap.";
                            }
                            else {
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_UnitAbilityTypes_xml_140a10878[lVar14] !=
                                       '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_14043fab0(&DAT_140b2f2e0,local_58,0);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheUnitAbilityTypeManager.System_Initialize",uVar7,
                                            uVar6,uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                FUN_1402149d0(
                                             "Error: Unable to successfully initialize the TheUnitAbilityTypeManager system."
                                             );
                              }
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_MusicEvents_xml_140a107c8[lVar14] != '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_1402d2370(&DAT_140b27dc0,local_58);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheMusicEventManager.System_Initialize",uVar7,uVar6,
                                            uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                pcVar11 = "Error: Unable to initialize TheMusicEventManager.";
                              }
                              else {
                                lVar14 = -1;
                                local_58[0] = (undefined8 *******)0x0;
                                local_48 = 0;
                                local_40 = 0xf;
                                do {
                                  lVar14 = lVar14 + 1;
                                } while (PTR_s___Data_XML_SpeechEvents_xml_140a107d0[lVar14] != '\0'
                                        );
                                FUN_140022730(local_58);
                                cVar3 = FUN_140321450(&DAT_140b2b300,local_58);
                                if (0xf < local_40) {
                                  if (0xfff < local_40 + 1) {
                                    if (0x1f < (ulonglong)
                                               ((longlong)local_58[0] +
                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                    }
                                  }
                                  thunk_FUN_1407864b8();
                                }
                                DVar4 = timeGetTime();
                                uVar6 = FUN_140216790();
                                uVar7 = FUN_140216710();
                                uVar18 = CONCAT44(uVar20,DVar4);
                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                              "TheSpeechEventManager.System_Initialize",uVar7,uVar6,
                                              uVar18);
                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                if (cVar3 == '\0') {
                                  pcVar11 = "Error: Unable to initialize TheSpeechEventManager.";
                                }
                                else {
                                  cVar3 = FUN_1403226e0(&DAT_140b2b3a8);
                                  DVar4 = timeGetTime();
                                  uVar6 = FUN_140216790();
                                  uVar7 = FUN_140216710();
                                  uVar18 = CONCAT44(uVar20,DVar4);
                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                "TheSpeechConversationManager.System_Initialize",
                                                uVar7,uVar6,uVar18);
                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                  if (cVar3 == '\0') {
                                    pcVar11 = 
                                    "Error: Unable to initialize TheSpeechConversationManager.";
                                  }
                                  else {
                                    cVar3 = FUN_140320760(&DAT_140b2b2f0);
                                    DVar4 = timeGetTime();
                                    uVar6 = FUN_140216790();
                                    uVar7 = FUN_140216710();
                                    uVar18 = CONCAT44(uVar20,DVar4);
                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                  "TheSFXConversationManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                    if (cVar3 == '\0') {


// ================================================================
// FUN_14004b250 — WINDOW lines 700-950
// ================================================================

// Total lines: 1509, showing 700 to 950
                                      pcVar11 = 
                                      "Error: Unable to initialize TheSFXConversationManager.";
                                    }
                                    else {
                                      local_res10 = FUN_14032e230;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      local_res10 = (code *)&LAB_140107ed0;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      lVar14 = -1;
                                      local_58[0] = (undefined8 *******)0x0;
                                      local_48 = 0;
                                      local_40 = 0xf;
                                      do {
                                        lVar14 = lVar14 + 1;
                                      } while (PTR_s___Data_XML_AnimationSFXMaps_txt_140a107d8
                                               [lVar14] != '\0');
                                      FUN_140022730(local_58);
                                      cVar3 = FUN_1404770a0(&DAT_140a2a120,local_58,0);
                                      if (0xf < local_40) {
                                        if (0xfff < local_40 + 1) {
                                          if (0x1f < (ulonglong)
                                                     ((longlong)local_58[0] +
                                                     (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                          }
                                        }
                                        thunk_FUN_1407864b8();
                                      }
                                      DVar4 = timeGetTime();
                                      uVar6 = FUN_140216790();
                                      uVar7 = FUN_140216710();
                                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                    "TheAnimSFXMapper.System_Initialize",uVar7,uVar6
                                                    ,CONCAT44(uVar20,DVar4));
                                      if (cVar3 == '\0') {
                                        pcVar11 = "Error: Unable to initialize TheAnimSFXMapper.";
                                      }
                                      else {
                                        lVar14 = -1;
                                        local_58[0] = (undefined8 *******)0x0;
                                        local_48 = 0;
                                        local_40 = 0xf;
                                        do {
                                          lVar14 = lVar14 + 1;
                                        } while (PTR_s___Data_XML_CommandBarComponentFi_140a10808
                                                 [lVar14] != '\0');
                                        FUN_140022730(local_58);
                                        uVar21 = 1;
                                        uVar20 = 1;
                                        cVar3 = FUN_14030ae50(&DAT_140b27f60,local_58,DAT_140a619f0,
                                                              FUN_14009cec0,&LAB_14009db00,
                                                              FUN_1400374b0,FUN_1400375c0,
                                                              FUN_1400a7aa0,FUN_140037670,
                                                              FUN_140037720,&LAB_1400a3540,
                                                              &LAB_140037210,&LAB_140037230,
                                                              FUN_140037250,FUN_140037410,
                                                              FUN_140037760,FUN_140037450,
                                                              &LAB_140037470,&LAB_140037810,
                                                              FUN_1400d6770,&LAB_1400d63f0,
                                                              &LAB_1400378e0,&LAB_140037900,
                                                              &LAB_140037920,FUN_140037940,
                                                              FUN_140037ae0,FUN_140037c30,
                                                              FUN_140037a00,FUN_14009d2a0,
                                                              FUN_1400d3c20,FUN_1400dc410,
                                                              FUN_140037cb0,&LAB_140037ce0,
                                                              &LAB_140037cf0,0);
                                        if (0xf < local_40) {
                                          if (0xfff < local_40 + 1) {
                                            if (0x1f < (ulonglong)
                                                       ((longlong)local_58[0] +
                                                       (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                            }
                                          }
                                          thunk_FUN_1407864b8();
                                        }
                                        DVar4 = timeGetTime();
                                        uVar6 = FUN_140216790();
                                        uVar7 = FUN_140216710();
                                        uVar18 = CONCAT44(uVar20,DVar4);
                                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                      "TheCommandBar.System_Initialize",uVar7,uVar6,
                                                      uVar18);
                                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                        if (cVar3 == '\0') {
                                          pcVar11 = "Error: Unable to initialize TheCommandBar.";
                                        }
                                        else {
                                          cVar3 = FUN_140447190(&DAT_140b30350,FUN_140123d70,
                                                                FUN_14005f6b0,DAT_140a61a10);
                                          DVar4 = timeGetTime();
                                          uVar6 = FUN_140216790();
                                          uVar7 = FUN_140216710();
                                          uVar18 = CONCAT44(uVar20,DVar4);
                                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                        "TheTutorial.Init",uVar7,uVar6,uVar18);
                                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                          if (cVar3 == '\0') {
                                            pcVar11 = "Error: Unable to initialize TheTutorial.";
                                          }
                                          else {
                                            cVar3 = FUN_14044ce30(&DAT_140a28c10,DAT_140a619f0);
                                            DVar4 = timeGetTime();
                                            uVar6 = FUN_140216790();
                                            uVar7 = FUN_140216710();
                                            uVar18 = CONCAT44(uVar20,DVar4);
                                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                          "CameraFXManager.System_Initialize",uVar7,
                                                          uVar6,uVar18);
                                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                            if (cVar3 == '\0') {
                                              pcVar11 = 
                                              "Error: Unable to initialize CameraFXManager.";
                                            }
                                            else {
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (PTR_s___Data_XML_FactionFiles_xml_140a10830
                                                       [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              FUN_140331d70(&DAT_140a172d0,local_58);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                            "FactionList.Initialize",uVar7,uVar6,
                                                            uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (
                                                  PTR_s___Data_XML_TradeRouteLines_xml_140a10818
                                                  [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              cVar3 = FUN_140467740(&DAT_140a29f20,local_58,0);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                        
                                                  "TheTradeRouteLineManager.System_Initialize",uVar7
                                                  ,uVar6,uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              if (cVar3 == '\0') {
                                                pcVar11 = "Error: Unable to initialize FactionList."
                                                ;
                                              }
                                              else {
                                                if (DAT_140b31008 == 0) {
LAB_14004cc39:
                                                  bVar1 = false;
                                                }
                                                else {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_TargetingPrioritySetF_140a10858
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  uVar15 = 1;
                                                  cVar3 = FUN_140483f10(DAT_140b31008,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004cc39;
                                                  bVar1 = true;
                                                }
                                                if ((uVar15 != 0) && (0xf < local_40)) {
                                                  if (0xfff < local_40 + 1) {
                                                    if (0x1f < (ulonglong)
                                                               ((longlong)local_58[0] +
                                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28)
                                                      ;
                                                    }
                                                  }
                                                  thunk_FUN_1407864b8();
                                                }
                                                bVar2 = false;
                                                DVar4 = timeGetTime();
                                                uVar6 = FUN_140216790();
                                                uVar7 = FUN_140216710();
                                                uVar18 = CONCAT44(uVar20,DVar4);
                                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                            
                                                  "TheTargetingPrioritySetManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                if (bVar1) {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_HardPointDataFiles_xm_140a10820
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404738b0(&DAT_140a2a0b0,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();


// ================================================================
// FUN_14004b250 — WINDOW lines 950-1200
// ================================================================

// Total lines: 1509, showing 950 to 1200
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HardPointDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HardPointDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_1404814d0(
                                                  PTR_s___Data_XML_HeroClash_xml_140a10800);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HeroClashTypeClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HeroClashTypeClass.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140408830(&DAT_140b2f020,
                                                                                                                                                    
                                                  PTR_s___Data_XML_GameObjectFiles_xml_140a107e0);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "GameObjectTypeManager.Read_Parse_Game_Object_Databases"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize GameObjectTypeManager."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_TradeRouteFiles_xml_140a10810
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404715e0(&DAT_140a2a060,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TradeRouteManager.System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TradeRouteManager.";
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_CampaignFiles_xml_140a10828
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140479fe0(&DAT_140b31160,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "CampaignDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize CampaignDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_14048f1b0(&LAB_1402e0550,
                                                                          FUN_1400f5200,
                                                                          FUN_1400d1f00);
                                                    DVar4 = timeGetTime();
                                                    uVar6 = FUN_140216790();
                                                    uVar7 = FUN_140216710();
                                                    uVar18 = CONCAT44(uVar20,DVar4);
                                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                    
                                                  "SpecialAbilityClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (PTR_s___Data_XML_Movies_xml_140a10848
                                                           [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140480560(&DAT_140a2a810,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "MovieManager.System_Initialize",
                                                                uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_LightningEffectTypes__140a10850
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140482490(&DAT_140a2a850,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheLightningTypeManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_WeatherScenarios_xml_140a10868
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140489660(&DAT_140a2a980,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheWeatherScenarioManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  uVar6 = FUN_14004fde0();
                                                  cVar3 = FUN_140477c10(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheMovementClassManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {


// ================================================================
// ALL VTABLE SYMBOLS with 'Scoring', 'Think', 'Service', 'GameObj'
// ================================================================


