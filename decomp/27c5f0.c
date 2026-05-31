/* FUN_14027c5f0  RVA 0x27c5f0  size 938  params 0  ret undefined */


undefined8 FUN_14027c5f0(uint *param_1,uint param_2,int *param_3,undefined1 *param_4)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  ulonglong uVar7;
  longlong lVar8;
  uint uVar9;
  longlong *plVar10;
  uint uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  int local_res8;
  
  uVar12 = 0;
  *param_3 = 0;
  local_res8 = -1;
  *param_4 = 0;
  param_1[0x321] = 0xffffffff;
  if ((char)param_1[0xd] != '\0') {
    return 0;
  }
  if (DAT_140b153f8 == 0) {
LAB_14027c8ec:
    if (param_1[6] != 0) {
      uVar11 = param_1[6];
      if (*(char *)((longlong)param_1 + 0xb0b5) != '\0') {
        uVar11 = 0x78;
      }
      if (*(char *)((longlong)param_1 + 0x37) != '\0') {
        uVar11 = 0x1e;
      }
      uVar11 = (uint)(1000 / (longlong)(int)uVar11);
      if (param_2 - param_1[0x14] < uVar11) {
        *param_3 = (param_1[0x14] - param_2) + uVar11;
        return 0;
      }
    }
    if (((char)param_1[0x2c2c] != '\0') && (*(char *)((longlong)param_1 + 0xb0b1) == '\0')) {
      return 0;
    }
    *(undefined1 *)((longlong)param_1 + 0xb0b1) = 0;
    if (*(char *)((longlong)param_1 + 0xb0b2) != '\0') {
      if ((*(char *)((longlong)param_1 + 0xb0b3) != '\0') &&
         (cVar4 = FUN_1402c1250(&DAT_140b15690,*param_1 + 1), cVar4 != '\0')) {
        *param_4 = 1;
        return 0;
      }
      if (*(char *)((longlong)param_1 + 0xb0b2) != '\0') {
        *(undefined1 *)((longlong)param_1 + 0xb0b3) = 1;
      }
    }
  }
  else {
    if ((DAT_140b153f8 != 1) && (DAT_140b153f8 != 2)) {
      if (DAT_140b153f8 == 3) goto LAB_14027c8ec;
      if (DAT_140b153f8 != 4) {
        return 0;
      }
    }
    iVar5 = FUN_140294b80(&DAT_140a16fd0);
    if (iVar5 < 2) {
      return 1;
    }
    uVar11 = 0xffffffff;
    uVar13 = 1;
    uVar7 = uVar12;
    uVar14 = uVar12;
    if ((int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3) < 1) {
      return 0;
    }
    do {
      iVar6 = (int)uVar7;
      uVar15 = uVar14;
      uVar9 = uVar11;
      iVar5 = local_res8;
      if ((((iVar6 != DAT_140a17000) && (uVar7 = FUN_140294be0(&DAT_140a16fd0,uVar7), uVar7 != 0))
          && (*(char *)(uVar7 + 0x62) != '\0')) && (*(char *)(uVar7 + 0x180) != '\0')) {
        iVar1 = *(int *)(uVar7 + 0x4c);
        lVar8 = FUN_140208290(&DAT_140a7c4e0,iVar1);
        if ((lVar8 != 0) &&
           (lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x31a) + (longlong)iVar1 * 8), lVar8 != 0)
           ) {
          uVar2 = *(uint *)(lVar8 + 0xc00);
          if (uVar2 == 0xffffffff) {
            return 0;
          }
          uVar15 = uVar7;
          uVar9 = uVar2;
          iVar5 = iVar6;
          if ((uVar11 <= uVar2) &&
             (uVar15 = uVar14, uVar9 = uVar11, iVar5 = local_res8, *param_1 <= uVar2)) {
            uVar13 = uVar12;
          }
        }
      }
      local_res8 = iVar5;
      uVar11 = uVar9;
      uVar7 = (ulonglong)(iVar6 + 1U);
      uVar14 = uVar15;
    } while ((int)(iVar6 + 1U) < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
    if (uVar11 == 0xffffffff) {
      return 0;
    }
    if (((local_res8 != -1) && (uVar9 = *param_1, uVar11 < uVar9)) && (param_1[2] <= uVar9 - uVar11)
       ) {
      param_1[0x321] = *(uint *)(uVar15 + 0x4c);
      if (DAT_140a7c479 == '\0') {
        return 0;
      }
      plVar10 = (longlong *)(uVar15 + 0x28);
      if (7 < *(ulonglong *)(uVar15 + 0x40)) {
        plVar10 = (longlong *)*plVar10;
      }
      FUN_140025760("FrameSynchronizer waiting for player %ls. My frame = %d, their frame = %d, MaxAhead = %d\n"
                    ,plVar10,uVar9,uVar11,param_1[2]);
      return 0;
    }
    uVar11 = param_1[6];
    if (uVar11 != 0) {
      uVar9 = (uint)(1000 / (longlong)(int)(uVar11 + 5));
      if ((char)uVar13 == '\0') {
        uVar9 = (uint)(1000 / (longlong)(int)(uVar11 + 10));
      }
      if (param_2 - param_1[0x14] < uVar9) {
        return 0;
      }
    }
    if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
      uVar7 = (ulonglong)((*param_1 - param_1[2]) + 1 & 0xff);
      do {
        if ((((int)uVar12 != DAT_140a17000) &&
            (lVar8 = FUN_140294be0(&DAT_140a16fd0,uVar12), lVar8 != 0)) &&
           ((*(char *)(lVar8 + 0x62) != '\0' && (*(char *)(lVar8 + 0x180) != '\0')))) {
          lVar3 = *(longlong *)
                   (*(longlong *)(param_1 + 0x31a) + (longlong)*(int *)(lVar8 + 0x4c) * 8);
          if (lVar3 != 0) {
            uVar11 = *(uint *)(lVar3 + uVar7 * 0xc);
            if (((uVar11 != 0xffffffff) && (param_1[uVar7 * 3 + 0x15] != 0xffffffff)) &&
               (uVar11 != param_1[uVar7 * 3 + 0x15])) {
              return 0;
            }
            iVar5 = *(int *)(lVar3 + uVar7 * 0xc + 8);
            iVar6 = FUN_1404b3af0(&DAT_140b15690,*(int *)(lVar8 + 0x4c));
            if (iVar6 < iVar5) {
              plVar10 = (longlong *)(lVar8 + 0x28);
              if (7 < *(ulonglong *)(lVar8 + 0x40)) {
                plVar10 = (longlong *)*plVar10;
              }
              FUN_140025760("FrameSynchronizer waiting for player %ls. received_commands = %d, their_commands = %d\n"
                            ,plVar10,iVar6,iVar5);
              return 0;
            }
          }
        }
        uVar11 = (int)uVar12 + 1;
        uVar12 = (ulonglong)uVar11;
        if ((int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3) <= (int)uVar11) {
          param_1[0x14] = param_2;
          return 1;
        }
      } while( true );
    }
  }
  param_1[0x14] = param_2;
  return 1;
}


