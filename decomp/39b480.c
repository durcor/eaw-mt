/* FUN_14039b480  RVA 0x39b480  size 1094  params 0  ret undefined */


/* WARNING: Type propagation algorithm not settling */

bool FUN_14039b480(longlong *param_1,int param_2,char param_3,char param_4)

{
  char cVar1;
  undefined1 uVar2;
  char cVar3;
  byte bVar4;
  int iVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong *plVar8;
  int iVar9;
  bool bVar10;
  float extraout_XMM0_Da;
  
  if (0x4b < param_2 - 1U) {
    return false;
  }
  if (((param_4 != '\0') && (param_1[0x1e] != 0)) && (cVar1 = FUN_140380bb0(), cVar1 != '\0')) {
    return false;
  }
  if (((param_1[0x1e] != 0) && (cVar1 = FUN_140380b10(), cVar1 != '\0')) &&
     (cVar1 = FUN_14043fa30(&DAT_140b2f2e0,param_2), cVar1 != '\0')) {
    return false;
  }
  if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
    if (*(char *)((longlong)param_1 + 899) == -1) {
      lVar6 = param_1[0x56];
      if (lVar6 == 0) {
        return false;
      }
      if (*(char *)(lVar6 + 899) == -1) {
        return false;
      }
      if (*(longlong *)(lVar6 + 0x210) == 0) {
        return false;
      }
      iVar5 = *(int *)(*(longlong *)(lVar6 + 0x210) + 8);
    }
    else {
      if (param_1[0x42] == 0) {
        return false;
      }
      iVar5 = *(int *)(param_1[0x42] + 8);
    }
    if (iVar5 == 0) {
      return false;
    }
  }
  if ((param_1[0x20] != 0) && (*(char *)(param_1[0x20] + 0x28) != '\0')) {
    return false;
  }
  if (*(char *)((longlong)param_1 + 0x38a) != -1) {
    return false;
  }
  if ((((param_3 != '\0') && ((char)param_1[0x6f] != -1)) &&
      (lVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar6 != 0)) &&
     ((cVar1 = FUN_14042f8e0(lVar6,param_2), cVar1 == '\0' &&
      (cVar1 = FUN_14042f7a0(lVar6,param_2), cVar1 != '\0')))) {
    return false;
  }
  if ((char)param_1[0x69] != -1) {
    uVar7 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
    iVar9 = 0;
    iVar5 = FUN_140405300(uVar7);
    if (0 < iVar5) {
      do {
        lVar6 = FUN_1404052d0(uVar7,iVar9);
        if ((lVar6 != 0) && (cVar1 = FUN_14039b480(lVar6,param_2,1), cVar1 == '\0')) {
          return false;
        }
        iVar9 = iVar9 + 1;
        iVar5 = FUN_140405300(uVar7);
      } while (iVar9 < iVar5);
    }
  }
  if (((*(char *)((longlong)param_1 + 0x333) != -1) &&
      (plVar8 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar8 != (longlong *)0x0))
     && (cVar1 = (**(code **)(*plVar8 + 0x2b0))(plVar8), cVar1 != '\0')) {
    return false;
  }
  switch(param_2) {
  case 1:
    lVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x45);
    if ((lVar6 != 0) && (cVar1 = FUN_1404c3700(lVar6), cVar1 != '\0')) {
      return false;
    }
    if (*(char *)((longlong)param_1 + 0x342) == -1) {
      return false;
    }
    lVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x10);
    if (lVar6 == 0) {
      return false;
    }
    if (param_1[0x1e] != 0) {
      cVar1 = ShieldBehaviorClass::FUN_140380ad0();
      if (cVar1 != '\0') {
        return false;
      }
      cVar1 = FUN_140380b70(param_1[0x1e]);
      if (cVar1 != '\0') {
        return false;
      }
    }
    uVar2 = FUN_140549460(lVar6,param_1);
    return (bool)uVar2;
  default:
    goto switchD_14039b665_caseD_2;
  case 3:
    bVar4 = *(byte *)((longlong)param_1 + 0x3ca);
    goto joined_r0x00014039b8b9;
  case 6:
    bVar4 = FUN_14039ad40(param_1);
    goto joined_r0x00014039b8b9;
  case 0x12:
  case 0x13:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x4a:
    bVar10 = *(char *)((longlong)param_1 + 0x3a7) == '\x01';
    break;
  case 0x1e:
    iVar5 = FUN_140397c10(param_1,4);
    if (iVar5 == 0) {
      return false;
    }
  case 0x1f:
  case 0x22:
  case 0x37:
    bVar4 = *(byte *)(param_1 + 0x74) & 0x10;
    goto joined_r0x00014039b8b9;
  case 0x26:
    lVar6 = (**(code **)(*param_1 + 0x10))(param_1,0x1c);
    if (lVar6 == 0) {
      return false;
    }
    cVar1 = FUN_14058f840(lVar6);
    cVar3 = FUN_1404c3700(lVar6);
    return cVar3 == '\0' && cVar1 == '\0';
  case 0x2f:
    if (DAT_140b15418 == (longlong *)0x0) {
      return false;
    }
    lVar6 = (**(code **)(*DAT_140b15418 + 0x220))(DAT_140b15418,(int)param_1[0xb]);
    if (lVar6 == 0) {
      return false;
    }
    if (*(char *)(param_1[0x53] + 0x8b) != '\x01') {
      return true;
    }
    cVar1 = FUN_140527520(lVar6);
    bVar10 = cVar1 == '\0';
    break;
  case 0x34:
    if (param_1[0x20] == 0) {
      return true;
    }
    bVar4 = FUN_14037c520(param_1[0x20] + 8);
joined_r0x00014039b8b9:
    if (bVar4 != 0) {
      return false;
    }
    return true;
  case 0x3a:
  case 0x46:
    iVar5 = FUN_140397c10(param_1,10);
    bVar10 = iVar5 == 0;
    break;
  case 0x40:
    lVar6 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
    if (lVar6 == 0) {
      return true;
    }
    if (DAT_140b27938 <= *(int *)(lVar6 + 0x44c)) {
      return false;
    }
    return true;
  case 0x48:
    FUN_1403976f0(param_1);
    if (extraout_XMM0_Da <= 0.0) {
      return false;
    }
    return true;
  case 0x49:
    lVar6 = param_1[0x20];
    if (lVar6 == 0) {
      return true;
    }
    if (0 < *(int *)(lVar6 + 0x3a0)) {
      return false;
    }
    if (*(int *)(lVar6 + 0x39c) == 0x3fffff) {
      return true;
    }
    lVar6 = FUN_1402aca60(DAT_140b15418[3]);
    if (lVar6 == 0) {
      return true;
    }
    bVar10 = (*(byte *)(lVar6 + 0x3a0) & 0x42) == 0;
    break;
  case 0x4b:
    if (param_1[0x23] == 0) {
      return false;
    }
    if (*(int *)(param_1[0x23] + 0x50) != 4) {
      return false;
    }
    iVar5 = FUN_140397c10(param_1,3);
    bVar10 = iVar5 == 0;
  }
  if (bVar10) {
    return false;
  }
switchD_14039b665_caseD_2:
  return true;
}


