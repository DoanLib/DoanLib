#include <dn/string.h>
#include <dn/main.h>

void DnTest_String() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-string"))) {
    return;
  }
}
