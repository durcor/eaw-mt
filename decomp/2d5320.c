/* FUN_1402d5320  RVA 0x2d5320  size 279  params 0  ret undefined */


undefined4
FUN_1402d5320(undefined8 param_1,undefined8 *param_2,longlong param_3,undefined4 param_4,
             undefined1 param_5)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  undefined1 local_28 [32];
  
  if (param_2 == (undefined8 *)0x0) {
    uVar1 = 0;
  }
  else {
    if (*(char *)(param_2 + 0x1c) == '\x01') {
      FUN_1402d4250(param_1,param_3);
    }
    uVar1 = 0xffffffff;
    if (*(char *)((longlong)param_2 + 0x29) == '\x01') {
      uVar1 = FUN_1402d5440(param_1,param_2,3,local_28,param_3,0,param_4,param_5);
    }
    else if (*(char *)((longlong)param_2 + 0x2a) == '\x01') {
      uVar1 = FUN_1402d5440(param_1,param_2,1,local_28,param_3,0,param_4,param_5);
    }
    else {
      puVar2 = (undefined8 *)(*(longlong *)(param_3 + 0x298) + 0xf8);
      if (0xf < *(ulonglong *)(*(longlong *)(param_3 + 0x298) + 0x110)) {
        puVar2 = (undefined8 *)*puVar2;
      }
      if (0xf < (ulonglong)param_2[3]) {
        param_2 = (undefined8 *)*param_2;
      }
      FUN_140025760("ERROR: SFXEvent \'%s\' not 2D or 3D specified, object = %s\n",param_2,puVar2);
    }
  }
  return uVar1;
}


