#include <dn/structs.h>
#include <dn/main.h>

void DnTest_Structs() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-structs"))) {
    return;
  }

  DnArray(i64) array = {};
  DnArray_Init(DnMem_DefaultAllocator(), &array, 4);
  DnArray_Deinit(DnMem_DefaultAllocator(), &array);
}
