/* FUN_14029bfe0  RVA 0x29bfe0  size 50  params 0  ret undefined */


void FUN_14029bfe0(longlong param_1,undefined8 *param_2)

{
  char cVar1;
  
  if ((param_2 != (undefined8 *)0x0) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0xe8,param_2 + 3,1), cVar1 == '\x01')) {
    (**(code **)*param_2)(param_2);
  }
  return;
}


