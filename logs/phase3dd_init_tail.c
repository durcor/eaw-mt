// Total lines: 1509

// === lines 60-200 ===
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheGameConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_1404457e0(&DAT_140b30160,&DAT_140a108e8);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudioConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheAudioConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_14048b8a0(&DAT_140b31330,&DAT_140a10928,0);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheMousePointerDataManager.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_140261280(&DAT_140b0a130);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","FontManager.System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize FontManager.");
    FUN_14004da50();
    return 0;
  }
  uVar6 = FUN_14004faa0();
  FUN_1404766a0(uVar6);
  FUN_14047fd00(&DAT_1409cf314);
  FUN_14047fcf0(FUN_14004f320);
  uVar6 = FUN_14004fcc0();
  cVar3 = FUN_14047fd10(uVar6,DAT_140a619d8,DAT_140a619d0);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                "TheDebugWindowManagerClass::Get().System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize DebugWindowManager.");
  }
  lVar16 = -1;
  lVar14 = -1;
  local_58[0] = (undefined8 *******)0x0;
  local_48 = 0;
  local_40 = 0xf;
  do {
    lVar14 = lVar14 + 1;
  } while (PTR_s___Data_XML_GraphicDetails_xml_140a10798[lVar14] != '\0');
  FUN_140022730(local_58);
  cVar3 = FUN_14048aa00(&DAT_140a2aa40,local_58,0);
  if (0xf < local_40) {
    if (0xfff < local_40 + 1) {
      if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGraphicDetailManager.System_Initialize",uVar7,
                uVar6,CONCAT44(uVar20,DVar4));
  if (cVar3 == '\0') {
    pcVar11 = "Error: Unable to initialize GraphicDetailManager.";
  }
  else {
    local_res8[0] = 0;
    DAT_140a61a30 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    if (DAT_140a61a30 == 0) {
      local_res8[0] = 0;
      DAT_140a61a30 =
           FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    }
    local_res8[0] = 0;
    DAT_140a61a38 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409cf410,0x1064c,0,local_res8,"EmpireAtWar-Light");
    local_res8[0] = 0;
    DAT_140a61a40 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409efb00,0x10414,0,local_res8,"EmpireAtWar-Medium");
    local_res8[0] = 0;
    uVar20 = 1;
    DAT_140a61a48 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409fff20,0x10830,0,local_res8,"EmpireAtWar-Stencil");
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    uVar18 = CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","EAW Fonts Loaded",uVar7,uVar6,uVar18);
    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
    cVar3 = FUN_14004df10(DAT_140a61aac,DAT_1409cf308,DAT_1409cf30c,DAT_1409cf310,DAT_140a61ac0);
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    plVar19 = (longlong *)CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","Init_3D_Game",uVar7,uVar6,plVar19);
    if (cVar3 == '\0') {
      pcVar11 = "Error: Unable to initialize via Init_3D_Game().";
    }
    else {
      uVar7 = FUN_1402c8210(&DAT_140b15800,DAT_140a7c090,DAT_140a13ed8);
      FUN_14001e680(local_58,uVar7);
      if (local_48 == 0) {
        lVar14 = -1;
        do {
          lVar14 = lVar14 + 1;
        } while (PTR_s_Splash_TGA_140a10ba8[lVar14] != '\0');
        FUN_140022730(local_58);
      }
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      FUN_140025760("Loading splash screen %s\n",pppppppuVar12);
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      plVar8 = (longlong *)FUN_14025e990(pppppppuVar12,1,0);

// === lines 1200-end ===
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheMovementClassManager."
                                                  ;
                                                  goto LAB_14004b60c;
                                                  }
                                                  FUN_140248c60();
                                                  FUN_140248be0();
                                                  DAT_140b09d30 = 0;
                                                  if (DAT_140b31018 == 0) {
LAB_14004d373:
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
                                                  PTR_s___Data_XML_DifficultyAdjustments_140a10860
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  bVar2 = true;
                                                  cVar3 = FUN_140484e30(DAT_140b31018,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004d373;
                                                  bVar1 = true;
                                                  }
                                                  if ((bVar2) && (0xf < local_40)) {
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
                                                                                                                                
                                                  "TheDifficultyAdjustmentManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (!bVar1) goto LAB_14004b611;
                                                  DAT_140b31150 = &DAT_140a619ef;
                                                  uVar6 = FUN_14004fc30();
                                                  cVar3 = FUN_1404753b0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheAIDataManagerClass::Get().System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    FUN_1402149d0(
                                                  "Error: Unable to initialize TheAIDataManager.");
                                                  }
                                                  uVar6 = FUN_14004fd50();
                                                  cVar3 = FUN_1404889d0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheGameScoringManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheGameScoringManagerClass."
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
                                                  PTR_s___Data_XML_WeatherModifiers_xml_140a10840
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_14046a350(&DAT_140b30b40,local_58);
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
                                                                "TheWeatherSystem.System_Initialize"
                                                                ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheWeatherSystem.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140488ec0(
                                                  PTR_s___Data_XML_LightSources_xml_140a107b8);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LightTypeManager::System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LightTypeManager.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140482d30(
                                                  PTR_s___Data_XML_LensFlares_xml_140a10778);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LensFlareTypeManager::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LensFlareTypeManager."
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
                                                  PTR_s___Data_XML_WeatherAudio_xml_140a107f8
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140324e70(&DAT_140b2b3b0,local_58);
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
                                                                                                                                
                                                  "TheWeatherAudioManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 != '\0') {
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar16 = lVar16 + 1;
                                                    } while (PTR_s_BlackMarketItems_xml_140a10880
                                                             [lVar16] != '\0');
                                                    FUN_140022730(local_58,
                                                  PTR_s_BlackMarketItems_xml_140a10880,lVar16);
                                                  FUN_14048f5b0(&PTR_vftable_140a2aa78,local_58);
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
                                                  uVar15 = FUN_140216790();
                                                  uVar6 = FUN_140216710();
                                                  uVar7 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "BlackMarketItemList.Initialize",
                                                                uVar6,uVar15,uVar7);
                                                  FUN_140451cd0();
                                                  DAT_140b2b5d0 = &DAT_1409cf314;
                                                  FUN_1404737c0(&DAT_140a2a0b0);
                                                  FUN_140479400(&DAT_140b31160);
                                                  FUN_140330450(&DAT_140a172d0);
                                                  FUN_1402c79d0(&DAT_140b15800);
                                                  _guard_check_icall(&DAT_140b30160);
                                                  FUN_140483ea0(DAT_140b31008);
                                                  FUN_1403239f0(&DAT_140b2b3b0);
                                                  FUN_140084a70();
                                                  FUN_1400a9800();
                                                  FUN_140086ff0();
                                                  FUN_140105240();
                                                  FUN_140107b70();
                                                  FUN_14008e880();
                                                  FUN_140085380();
                                                  FUN_140463c80(FUN_1400dca70,&LAB_14004fe70);
                                                  FUN_14047cb40(&DAT_140b311a8);
                                                  FUN_1402a73a0();
                                                  _DAT_140b15480 = FUN_140056480;
                                                  uVar9 = CONCAT71((int7)((ulonglong)uVar7 >> 8),1);
                                                  local_res10 = (code *)0x3d99999a3c23d70a;
                                                  local_res18 = 0x3c23d70a;
                                                  local_res1c = 0x3d99999a;
                                                  FUN_140049110(&DAT_1409cf320,&local_res18,
                                                                &local_res10,0,
                                                                uVar15 & 0xffffffffffffff00,uVar9,
                                                                "Arial Bold",CONCAT44(uVar21,9));
                                                  local_res1c = DAT_1408022fc;
                                                  local_res10 = (code *)0x3c23d70a3ccccccd;
                                                  local_res18 = 0x3ccccccd;
                                                  FUN_140049110(&DAT_1409cf390,&local_res18,
                                                                &local_res10,1,1,
                                                                uVar9 & 0xffffffffffffff00,"Arial",7
                                                               );
                                                  FUN_140049270(&DAT_1409cf390,DAT_1408007b4,
                                                                DAT_1408022f8,30000,0x18);
                                                  DAT_140b15668 = FUN_140038100;
                                                  FUN_140297630(&DAT_140a16fd0,0);
                                                  FUN_140297630(&DAT_140a17008,0);
                                                  FUN_140293390(&DAT_140a17008,1);
                                                  FUN_1403d4490(&DAT_140b27f60);
                                                  return 1;
                                                  }
                                                  pcVar11 = 
                                                  "Error: Unable to initialize the TheWeatherAudioManager."
                                                  ;
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                                else {
                                                  pcVar11 = 
                                                  "Error: Unable to initialize TargetingPrioritySetManager."
                                                  ;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_14004b60c:
  FUN_1402149d0(pcVar11);
LAB_14004b611:
  FUN_14004da50();
  return 0;
}
