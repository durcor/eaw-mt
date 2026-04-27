/* FUN_1403729f0  RVA 0x3729f0 */


undefined8 FUN_1403729f0(longlong param_1,int param_2,longlong param_3)

{
  if (param_2 == -1) {
    param_2 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      param_2 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (param_2 == -1) {
      return 0;
    }
  }
  FUN_140375380(param_1,param_2);
  if (param_2 == 0) {
    if (((param_3 != 0) && (*(longlong *)(param_3 + 0xb8) != 0)) &&
       (*(char *)(*(longlong *)(param_3 + 0xb8) + 0x2c8) == '\x01')) {
      return *(undefined8 *)(param_1 + 0x298);
    }
    return *(undefined8 *)(param_1 + 0x288);
  }
  if (param_2 == 1) {
    return *(undefined8 *)(param_1 + 0x2a0);
  }
  if (param_2 == 2) {
    return *(undefined8 *)(param_1 + 0x2a8);
  }
  return 0;
}

