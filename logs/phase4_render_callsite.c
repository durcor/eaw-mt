// ================================================================
// RVA 0x180d90  Name: FUN_140180d90  Size: 32 bytes
// ================================================================

// Callers of FUN_140180d90:
//   call site RVA 0x13d3f0  in thunk_FUN_140180d90 (RVA 0x13d3f0)


void FUN_140180d90(undefined8 param_1)

{
  FUN_140180dc0(param_1,0xbff);
  FUN_140149650(param_1);
  return;
}



// ================================================================
// RVA 0x13d3f0  Name: thunk_FUN_140180d90  Size: 5 bytes
// ================================================================

// Callers of thunk_FUN_140180d90:
//   call site RVA 0x1b3057  in FUN_1401b2ae0 (RVA 0x1b2ae0)


void thunk_FUN_140180d90(undefined8 param_1)

{
  FUN_140180dc0(param_1,0xbff);
  FUN_140149650(param_1);
  return;
}



// ================================================================
// RVA 0x149650  Name: FUN_140149650  Size: 116 bytes
// ================================================================

// Callers of FUN_140149650:
//   call site RVA 0x180dab  in FUN_140180d90 (RVA 0x180d90)


void FUN_140149650(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong lVar4;
  undefined1 local_res10 [8];
  
  lVar4 = param_1 + 0x6c2e8;
  lVar3 = 0xc;
  param_1 = param_1 + 0x6c2f0;
  do {
    lVar1 = *(longlong *)(param_1 + 8);
    while (lVar1 != param_1) {
      puVar2 = (undefined8 *)FUN_14020ac80(lVar4,local_res10);
      if (puVar2 != (undefined8 *)0x0) {
        (**(code **)*puVar2)(puVar2,1);
      }
      lVar1 = *(longlong *)(param_1 + 8);
    }
    lVar4 = lVar4 + 0x48;
    param_1 = param_1 + 0x48;
    lVar3 = lVar3 + -1;
  } while (lVar3 != 0);
  return;
}



// ================================================================
// RVA 0x180dc0  Name: FUN_140180dc0  Size: 2078 bytes
// ================================================================

// Callers of FUN_140180dc0:
//   call site RVA 0x180d9e  in FUN_140180d90 (RVA 0x180d90)


