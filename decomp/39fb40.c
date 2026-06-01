/* NebulaBehaviorClass::FUN_14039fb40  RVA 0x39fb40  size 251  params 0  ret undefined */


void NebulaBehaviorClass::FUN_14039fb40(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong lVar4;
  undefined **local_18;
  int local_10;
  
  lVar4 = *(longlong *)(param_1 + 0x2b0);
  local_18 = AbilitySignalDataClass::vftable;
  local_10 = 0;
  if ((lVar4 == 0) || (*(char *)(lVar4 + 0x348) == -1)) {
    lVar4 = param_1;
  }
  local_10 = FUN_140374b50(*(undefined8 *)(param_1 + 0x298),0);
  if (local_10 != 0) {
    cVar1 = FUN_14039b480(param_1,local_10,1);
    if (cVar1 != '\0') {
      lVar3 = lVar4 + 0x38;
      if (lVar4 == 0) {
        lVar3 = 0;
      }
      uVar2 = FUN_140058570();
      FUN_140220ed0(uVar2,lVar3,0x2c,&local_18);
    }
  }
  local_10 = FUN_140374b50(*(undefined8 *)(param_1 + 0x298),1);
  if (local_10 != 0) {
    cVar1 = FUN_14039b480(param_1,local_10,1);
    if (cVar1 != '\0') {
      lVar3 = lVar4 + 0x38;
      if (lVar4 == 0) {
        lVar3 = 0;
      }
      uVar2 = FUN_140058570();
      FUN_140220ed0(uVar2,lVar3,0x2c,&local_18);
    }
  }
  return;
}


