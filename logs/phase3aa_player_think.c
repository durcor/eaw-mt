
// ================================================================
// FUN_140288980 — per-player dispatch (called from slot-3 per player)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x288980  Name: FUN_140288980  Size: 510 bytes
// ------------------------------------------------------------

undefined4 FUN_140288980(longlong param_1,int param_2,char param_3,char param_4)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  int iVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar11;
  ulonglong uVar10;
  
  uVar11 = 0;
  if (param_2 < 1) {
    param_2 = 0;
  }
  iVar3 = *(int *)(param_1 + 0x88);
  if (param_2 < *(int *)(param_1 + 0x88)) {
    iVar3 = param_2;
  }
  *(int *)(param_1 + 0x84) = iVar3;
  FUN_14032e8c0(DAT_140b15418 + 0xa0,iVar3,param_1);
  bVar1 = false;
  if ((param_3 == '\x01') && (iVar3 = FUN_140408150(&DAT_140b2f020), bVar1 = false, 0 < iVar3)) {
    uVar10 = uVar11;
    do {
      lVar5 = FUN_140408130(&DAT_140b2f020,uVar10);
      if ((((lVar5 != 0) &&
           (cVar2 = FUN_1403751a0(lVar5,*(undefined8 *)(param_1 + 0x68)), cVar2 != '\0')) &&
          (*(int *)(lVar5 + 0x89c) < *(int *)(param_1 + 0x84))) && (*(char *)(lVar5 + 0xc1) != '\0')
         ) {
        iVar4 = *(int *)(param_1 + 0x1b8);
        if (0 < iVar4) {
          plVar6 = *(longlong **)(param_1 + 0x1b0);
          uVar8 = uVar11;
          do {
            if (*plVar6 == lVar5) goto LAB_140288b02;
            uVar8 = uVar8 + 1;
            plVar6 = plVar6 + 1;
          } while ((longlong)uVar8 < (longlong)iVar4);
        }
        if (*(char *)(lVar5 + 0xc2) != '\0') {
          if (0 < iVar4) {
            plVar6 = *(longlong **)(param_1 + 0x1b0);
            uVar8 = uVar11;
            do {
              if (*plVar6 == lVar5) goto LAB_140288aff;
              uVar8 = uVar8 + 1;
              plVar6 = plVar6 + 1;
            } while ((longlong)uVar8 < (longlong)iVar4);
          }
          iVar4 = *(int *)(param_1 + 0x1b8);
          iVar7 = *(int *)(param_1 + 0x1bc) * 2 >> 1;
          if (iVar7 <= iVar4) {
            iVar4 = iVar7 / 10;
            if (iVar7 / 10 < 10) {
              iVar4 = 10;
            }
            FUN_140047aa0(param_1 + 0x1a8,iVar4 + iVar7,0,1);
            iVar4 = *(int *)(param_1 + 0x1b8);
          }
          *(longlong *)(*(longlong *)(param_1 + 0x1b0) + (longlong)iVar4 * 8) = lVar5;
          *(int *)(param_1 + 0x1b8) = *(int *)(param_1 + 0x1b8) + 1;
LAB_140288aff:
          bVar1 = true;
        }
      }
LAB_140288b02:
      uVar9 = (int)uVar10 + 1;
      uVar10 = (ulonglong)uVar9;
    } while ((int)uVar9 < iVar3);
  }
  if (((param_4 == '\x01') && (bVar1)) &&
     ((iVar3 = FUN_140294a70(&DAT_140a16fd0), *(int *)(param_1 + 0x4c) == iVar3 &&
      ((*(longlong *)(param_1 + 0x68) != 0 &&
       (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x670), lVar5 != 0)))))) {
    FUN_1402d5290(&DAT_140b27e60,lVar5,0,0,0);
  }
  return *(undefined4 *)(param_1 + 0x84);
}




// ================================================================
// FUN_1404d9cd0 — per-player service (called from slot-4)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d9cd0  Name: FUN_1404d9cd0  Size: 100 bytes
// ------------------------------------------------------------

void FUN_1404d9cd0(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
  if (0 < iVar3) {
    do {
      lVar1 = FUN_140294bc0(&DAT_140a16fd0,iVar2);
      if (lVar1 != 0) {
        FUN_14027ff80(lVar1,param_2);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < iVar3);
  }
  return;
}




// ================================================================
// FUN_140245690 — used in FUN_1404889d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x245690  Name: FUN_140245690  Size: 8 bytes
// ------------------------------------------------------------

longlong FUN_140245690(longlong param_1)

{
  return param_1 + 0x88;
}




// ================================================================
// CALLERS OF FUN_140288980
// ================================================================

// Caller: RVA 0x292590  Name: FUN_140292590  Size: 294 bytes
// Caller: RVA 0x391310  Name: FUN_140391310  Size: 3493 bytes
// Caller: RVA 0x44a3f0  Name: FUN_14044a3f0  Size: 1593 bytes
// Caller: RVA 0x498170  Name: FUN_140498170  Size: 1321 bytes
// Caller: RVA 0x294fa0  Name: FUN_140294fa0  Size: 3394 bytes
// Caller: RVA 0x295cf0  Name: FUN_140295cf0  Size: 5635 bytes
// Caller: RVA 0x35cb10  Name: FUN_14035cb10  Size: 246 bytes
// Caller: RVA 0x402840  Name: FUN_140402840  Size: 2550 bytes
// Caller: RVA 0x461fc0  Name: FUN_140461fc0  Size: 427 bytes
// Caller: RVA 0x604480  Name: FUN_140604480  Size: 192 bytes
// Caller: RVA 0x282190  Name: FUN_140282190  Size: 30 bytes


// ================================================================
// CALLEES OF FUN_140288980 (what it calls)
// ================================================================



// ================================================================
// CALLEES OF FUN_1404d9cd0 (slot-4 per-player service)
// ================================================================


