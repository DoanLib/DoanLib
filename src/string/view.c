#include "dn/string.h"
#include "dn/memory.h"
#include <string.h>

// == STRING VIEW STRUCT ==================================================== //

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

bool DnStrView_IsValid(DnStrView view) {
  DN_ASSERT(view.length <= INT64_MAX);
  return view.data || view.length == 0;
}

const char* DnStrView_ToCStr(const DnMemAllocator* allocator, DnStrView view) {
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

DnStrView DnStrView_SubStr(DnStrView view, DnRange range) {
  DN_ASSERT(DnStrView_IsValid(view));
  DnRange_ToIndices(&range, view.length);

  return (DnStrView) {
    .data = view.data + range.start,
    .length = (u64)(range.end - range.start),
  };
}

bool DnStrView_Compare(DnStrView first, DnStrView second) {
  DN_ASSERT(DnStrView_IsValid(first));
  DN_ASSERT(DnStrView_IsValid(second));

  return memcmp(first.data, second.data, second.length) == 0;
}

i64 DnStrView_Find(DnStrView view, DnStrView occurrence) {
  DN_ASSERT(DnStrView_IsValid(view));
  DN_ASSERT(DnStrView_IsValid(occurrence));

  if (occurrence.length > 1) {
    for (i64 i = 0; i + (i64)occurrence.length - 1 < (i64)view.length; ++i) {
      if (memcmp(view.data + i, occurrence.data, occurrence.length) == 0)
        return i + 1;
    }
  }
  else if (occurrence.length == 1) {
    for (i64 i = 0; i < (i64)view.length; ++i) {
      if (view.data[i] == *occurrence.data) {
        return i + 1;
      }
    }
  }
  else if (occurrence.length == 0){
    return 1;
  }

  return 0;
}

i64 DnStrView_FindReverse(DnStrView view, DnStrView occurrence) {
  DN_ASSERT(DnStrView_IsValid(view));
  DN_ASSERT(DnStrView_IsValid(occurrence));

  if (occurrence.length > 1) {
    for (i64 i = (i64)view.length - (i64)occurrence.length; i >= 0; --i) {
      if (memcmp(view.data + i, occurrence.data, occurrence.length) == 0)
        return i + 1;
    }
  }
  else if (occurrence.length == 1) {
    for (i64 i = (i64)view.length - 1; i >= 0; --i) {
      if (view.data[i] == *occurrence.data) {
        return i + 1;
      }
    }
  }
  else if (occurrence.length == 0){
    return (i64)view.length;
  }

  return 0;
}
