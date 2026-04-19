
// ================================================================
// VTABLE SYMBOL SEARCH
// ================================================================

// GameObjectManagerClass::vftable @ RVA 0x85b9a8
// GameObjectClass::vftable @ RVA 0x8661b8
// ReferenceListClass::vftable NOT FOUND

// ================================================================
// GameObjectManagerClass VTABLE ENTRIES
// ================================================================

// GameObjectManagerClass vtable @ RVA 0x85b9a8
//   slot  0 (+0x000): RVA 0x29b990  FUN_14029b990
//   slot  1 (+0x008): RVA 0x90d0f0  (no function)
//   slot  2 (+0x010): RVA 0x2acf70  FUN_1402acf70
//   slot  3 (+0x018): RVA 0x29b960  FUN_14029b960
//   slot  4 (+0x020): RVA 0x90d170  (no function)
//   slot  5 (+0x028): RVA 0x2acf60  FUN_1402acf60
//   slot  6 (+0x030): RVA 0x29b960  FUN_14029b960
//   slot  7 (+0x038): RVA 0x6c61636834636154  (no function)
//   slot  8 (+0x040): RVA 0x415f65612972425f  (no function)
//   slot  9 (+0x048): RVA 0x7973296c6962  (no function)
//   slot 10 (+0x050): RVA 0x90d1f0  (no function)
//   slot 11 (+0x058): RVA 0x29e730  FUN_14029e730
//   slot 12 (+0x060): RVA 0x90cf08  (no function)
//   slot 13 (+0x068): RVA 0x29b3c0  FUN_14029b3c0
//   slot 14 (+0x070): RVA 0x53206d6e246e6152  (no function)
//   slot 15 (+0x078): RVA 0x3830251efa646565  (no function)
//   slot 16 (+0x080): RVA 0x8cd0a0d58  (no function)
//   slot 17 (+0x088): RVA 0x2520726439616c50  (no function)
//   slot 18 (+0x090): RVA 0xd29736ae5282064  (no function)
//   slot 19 (+0x098): RVA 0x746964643243090a  (no function)
//   slot 20 (+0x0a0): RVA 0x20090a0c26253a73  (no function)
//   slot 21 (+0x0a8): RVA 0x253a686225542020  (no function)
//   slot 22 (+0x0b0): RVA 0xa0d0a0bc00a0d64  (no function)
//   slot 23: NULL — end of vtable


// ================================================================
// GameObjectClass VTABLE ENTRIES
// ================================================================

// GameObjectClass vtable @ RVA 0x8661b8
//   slot  0 (+0x000): RVA 0x33fb50  FUN_14033fb50
//   slot  1 (+0x008): RVA 0x340890  FUN_140340890
//   slot  2 (+0x010): RVA 0x395ac0  FUN_140395ac0
//   slot  3 (+0x018): RVA 0x90fd40  (no function)
//   slot  4 (+0x020): RVA 0x33fb5c  FUN_14033fb5c
//   slot  5 (+0x028): RVA 0x34089c  FUN_14034089c
//   slot  6 (+0x030): RVA 0x38a250  FUN_14038a250
//   slot  7 (+0x038): RVA 0x391240  FUN_140391240
//   slot  8 (+0x040): RVA 0x90fd68  (no function)
//   slot  9 (+0x048): RVA 0x38a25c  FUN_14038a25c
//   slot 10 (+0x050): RVA 0x90fd90  (no function)
//   slot 11 (+0x058): RVA 0x38a268  FUN_14038a268
//   slot 12 (+0x060): RVA 0x395c00  FUN_140395c00
//   slot 13 (+0x068): RVA 0x90fdb8  (no function)
//   slot 14 (+0x070): RVA 0x38a274  FUN_14038a274
//   slot 15 (+0x078): RVA 0x90fb10  (no function)
//   slot 16 (+0x080): RVA 0x38a280  FUN_14038a280
//   slot 17 (+0x088): RVA 0x63697462216c6147  (no function)
//   slot 18 (+0x090): RVA 0x485f7464256c465f  (no function)
//   slot 19 (+0x098): RVA 0x6361707232657079  (no function)


// ================================================================
// GameObjectManagerClass VTABLE DECOMPILES (slots 0-15)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x29b990  Name: FUN_14029b990  Size: 52 bytes
// ------------------------------------------------------------

undefined8 FUN_14029b990(undefined8 param_1,ulonglong param_2)

{
  FUN_14029aaa0();
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0x640);
  }
  return param_1;
}



// No function at RVA 0x90d0f0

// ------------------------------------------------------------
// RVA: 0x2acf70  Name: FUN_1402acf70  Size: 8 bytes
// ------------------------------------------------------------

undefined * FUN_1402acf70(void)

{
  return &DAT_140a2bf50;
}



// ------------------------------------------------------------
// RVA: 0x29b960  Name: FUN_14029b960  Size: 43 bytes
// ------------------------------------------------------------

undefined8 * FUN_14029b960(undefined8 *param_1,ulonglong param_2)

{
  *param_1 = SignalDataClass::vftable;
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0x10);
  }
  return param_1;
}



// No function at RVA 0x90d170

// ------------------------------------------------------------
// RVA: 0x2acf60  Name: FUN_1402acf60  Size: 8 bytes
// ------------------------------------------------------------

undefined * FUN_1402acf60(void)

{
  return &DAT_140a2bf70;
}



// ------------------------------------------------------------
// RVA: 0x29b960  Name: FUN_14029b960  Size: 43 bytes
// ------------------------------------------------------------

undefined8 * FUN_14029b960(undefined8 *param_1,ulonglong param_2)

{
  *param_1 = SignalDataClass::vftable;
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(param_1,0x10);
  }
  return param_1;
}



// No function at RVA 0x6c61636834636154

// No function at RVA 0x415f65612972425f

// No function at RVA 0x7973296c6962

// No function at RVA 0x90d1f0

// ------------------------------------------------------------
// RVA: 0x29e730  Name: FUN_14029e730  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14029e730(longlong param_1)

{
  FUN_140135140(*(undefined8 *)(param_1 + 8));
  return;
}



// No function at RVA 0x90cf08

// ------------------------------------------------------------
// RVA: 0x29b3c0  Name: FUN_14029b3c0  Size: 256 bytes
// ------------------------------------------------------------

longlong * FUN_14029b3c0(longlong *param_1,ulonglong param_2)

{
  longlong *plVar1;
  longlong *plVar2;
  LPVOID lpMem;
  longlong *plVar3;
  HANDLE hHeap;
  longlong lVar4;
  
  if ((param_2 & 2) == 0) {
    FUN_14029a810();
    if ((param_2 & 1) != 0) {
      thunk_FUN_1407864b8(param_1,0x18);
    }
    return param_1;
  }
  lVar4 = param_1[-1];
  plVar1 = param_1 + -1;
  plVar2 = param_1 + lVar4 * 3;
  while (plVar3 = plVar2, lVar4 != 0) {
    lpMem = (LPVOID)plVar3[-2];
    plVar2 = plVar3 + -3;
    lVar4 = lVar4 + -1;
    *plVar2 = (longlong)DynamicVectorClass<struct_CompanyStruct*___ptr64>::vftable;
    if (lpMem != (LPVOID)0x0) {
      if ((*(uint *)((longlong)plVar3 + -4) & 0x80000000) == 0) {
        thunk_FUN_1407864b8(lpMem);
      }
      else {
        hHeap = GetProcessHeap();
        HeapFree(hHeap,0,lpMem);
      }
      *(uint *)((longlong)plVar3 + -4) = *(uint *)((longlong)plVar3 + -4) & 0x80000000;
      plVar3[-2] = 0;
      *(undefined4 *)(plVar3 + -1) = 0;
    }
  }
  if ((param_2 & 1) != 0) {
    thunk_FUN_1407864b8(plVar1,*plVar1 * 0x18 + 8);
  }
  return plVar1;
}



// No function at RVA 0x53206d6e246e6152

// No function at RVA 0x3830251efa646565


// ================================================================
// GameObjectClass VTABLE DECOMPILES (slots 0-10)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x33fb50  Name: FUN_14033fb50  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14033fb50(longlong param_1)

{
  FUN_1401f92b0(param_1 + 8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x340890  Name: FUN_140340890  Size: 9 bytes
// ------------------------------------------------------------

void FUN_140340890(longlong param_1)

{
  FUN_1401f92c0(param_1 + 8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x395ac0  Name: FUN_140395ac0  Size: 43 bytes
// ------------------------------------------------------------

longlong FUN_140395ac0(longlong param_1,int param_2)

{
  char cVar1;
  
  if (param_2 == 0) {
    return param_1;
  }
  cVar1 = *(char *)((longlong)param_2 + 0x332 + param_1);
  if (cVar1 == -1) {
    return 0;
  }
  return *(longlong *)(*(longlong *)(param_1 + 0x278) + (longlong)cVar1 * 8);
}



// No function at RVA 0x90fd40

// ------------------------------------------------------------
// RVA: 0x33fb5c  Name: FUN_14033fb5c  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14033fb5c(longlong param_1)

{
  FUN_14033fb50(param_1 + -8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x34089c  Name: FUN_14034089c  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14034089c(longlong param_1)

{
  FUN_140340890(param_1 + -8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x38a250  Name: FUN_14038a250  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14038a250(longlong param_1)

{
  FUN_14038a310(param_1 + -8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x391240  Name: FUN_140391240  Size: 22 bytes
// ------------------------------------------------------------

void FUN_140391240(longlong *param_1)

{
  if (param_1 != (longlong *)&DAT_00000008) {
                    /* WARNING: Could not recover jumptable at 0x000140391251. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*param_1 + 0x10))(param_1,1);
    return;
  }
  return;
}



// No function at RVA 0x90fd68

// ------------------------------------------------------------
// RVA: 0x38a25c  Name: FUN_14038a25c  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14038a25c(longlong param_1)

{
  FUN_14038a310(param_1 + -0x18);
  return;
}



// No function at RVA 0x90fd90

