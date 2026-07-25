#include <dn/shared.h>
#include <dn/main.h>

extern bool DnTest_Memory();
extern bool DnTest_Utility();

DN_DEFINE_MAIN_ENTRY() {
  if (!DnTest_Memory()) {
    return 1;
  }

  if (!DnTest_Utility()) {
    return 1;
  }

  return 0;
}
