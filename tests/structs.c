#include <dn/shared.h>
#include <dn/structs.h>

void DnTest_Structs() {
  DnArray(i64) array = {};
  DnArray_Init(DnMemAllocator_GetDefault(), &array, 4);
  DnArray_Deinit(DnMemAllocator_GetDefault(), &array);
}
