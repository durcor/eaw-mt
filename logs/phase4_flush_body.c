// FUN_140180dc0 — render flush body (2078 bytes)


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

// ... (184 more lines)
