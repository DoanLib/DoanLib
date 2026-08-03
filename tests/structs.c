#include <dn/structs.h>
#include <dn/main.h>

void DnTest_Structs() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-structs"))) {
    return;
  }

  DnArray(i64) array = {};
  DnArray_Init(DnMemAllocator_GetDefault(), &array, 4);
  DnArray_Deinit(DnMemAllocator_GetDefault(), &array);
}
