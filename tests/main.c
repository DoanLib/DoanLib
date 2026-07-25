#include <dn/shared.h>
#include <dn/main.h>

extern bool DnTest_Memory();
extern bool DnTest_Utility();

DN_DEFINE_MAIN_ENTRY() {
  bool success = true;

  if (!DnTest_Memory()) {
    success = false;
  }

  if (!DnTest_Utility()) {
    success = false;
  }

  if (!success) {
    return DnExitCode_TestFailure;
  }

  return DnExitCode_Success;
}
