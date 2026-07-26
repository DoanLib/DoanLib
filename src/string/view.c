#include "dn/string.h"
#include "dn/memory.h"
#include <string.h>

// == STRING VIEW STRUCT ==================================================== //

static bool DnStrView_IsValid(DnStrView view) {
  return view.data || view.length == 0;
}

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
  DN_ASSERT(DnStrView_IsValid(view));
  return view.data == nullptr || view.length == 0;
}

const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(DnStrView_IsValid(view));

  char* result = (char*)DN_MEM_ALLOC(allocator, view.length + 1, DnMem_DefaultAlignment);
  if (!DnStrView_IsEmpty(view)) {
    memcpy(result, view.data, view.length);
  }

  result[view.length] = '\0';
  return result;
}

// == STRING VIEW FUNCTIONS ================================================= //

DnStrView DnStrView_SubStr(DnStrView view, i64 i, i64 j) {
  DN_ASSERT(DnStrView_IsValid(view));
  DnStr_RangeToIndices(&i, &j, view.length);

  return (DnStrView) {
    .data = view.data + i,
    .length = (u64)j - (u64)i,
  };
}
