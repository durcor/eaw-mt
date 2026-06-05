/* FUN_14012d430  RVA 0x12d430  size 30  params 0  ret undefined */


void FUN_14012d430(longlong param_1,int param_2,char param_3)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x10);
  *(byte *)(lVar1 + param_2) = *(byte *)(lVar1 + param_2) & 0xfd | param_3 * '\x02';
  return;
}


