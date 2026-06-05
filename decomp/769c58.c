/* FUN_140769c58  RVA 0x769c58  size 58  params 0  ret undefined */


void FUN_140769c58(size_t param_1)

{
  longlong lVar1;
  int iVar2;
  
  do {
    lVar1 = _malloc_base(param_1);
    if (lVar1 != 0) {
      return;
    }
    iVar2 = _callnewh(param_1);
  } while (iVar2 != 0);
  if (param_1 == 0xffffffffffffffff) {
                    /* WARNING: Subroutine does not return */
    FUN_1400225f0();
  }
                    /* WARNING: Subroutine does not return */
  std::locale::_Locimp::FUN_1407408bc();
}


