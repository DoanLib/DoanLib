#include <dn/memory.h>
#include <dn/main.h>

void DnTest_Memory() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-memory"))) {
    return;
  }
}
