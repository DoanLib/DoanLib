#include <dn/shared.h>
#include <dn/main.h>

extern void DnTest_Memory();
extern void DnTest_String();
extern void DnTest_Structs();
extern void DnTest_Utility();

DN_DEFINE_MAIN_ENTRY() {
  DnTest_Memory();
  DnTest_String();
  DnTest_Structs();
  DnTest_Utility();

  return DnExitCode_Success;
}
