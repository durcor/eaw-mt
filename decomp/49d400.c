/* FUN_14049d400  RVA 0x49d400  size 28  params 0  ret undefined */


undefined4 FUN_14049d400(undefined8 param_1,uint param_2)

{
  if (param_2 < 0x96) {
    return *(undefined4 *)(&DAT_140b31440 + (longlong)(int)param_2 * 4);
  }
  return 0;
}


