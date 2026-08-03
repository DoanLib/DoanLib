#include <dn/utility.h>
#include <dn/main.h>

void DnTest_Utility() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-utility"))) {
    return;
  }
}
