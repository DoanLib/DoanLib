#include "dn/string.h"
#include "dn/memory.h"
#include <string.h>

// == STRING VIEW =========================================================== //

DnStrView DnStrView_FromCStr(const char* string) {
  return (DnStrView) {
    .data = string,
    .length = strlen(string),
  };
}

DnStrView DnStrView_FromCStrLength(const char* string, u64 length) {
  DN_ASSERT(string || length == 0);
  return (DnStrView) {
    .data = string,
    .length = length,
  };
}

bool DnStrView_IsEmpty(DnStrView view) {
  DN_ASSERT(view.data || view.length == 0);
  return view.data == nullptr || view.length == 0;
}

const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator);

  char* result = (char*)DN_MEM_ALLOC(allocator, view.length + 1, DnMem_DefaultAlignment);
  if (!DnStrView_IsEmpty(view)) {
    memcpy(result, view.data, view.length);
  }

  result[view.length] = '\0';
  return result;
}
