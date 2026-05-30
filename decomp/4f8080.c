/* LandMovementCoordinatorClass::vfunc_5  RVA 0x4f8080  size 61  params 0  ret undefined */


void LandMovementCoordinatorClass::vfunc_5(longlong *param_1)

{
  char cVar1;
  
  (**(code **)(*param_1 + 0x50))();
  do {
    if ((int)param_1[0x15] == 0) {
      cVar1 = (**(code **)(*param_1 + 0x58))(param_1);
    }
    else {
      if ((int)param_1[0x15] != 1) {
        return;
      }
      cVar1 = (**(code **)(*param_1 + 0x60))(param_1);
    }
  } while (cVar1 == '\0');
  return;
}


