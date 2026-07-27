#include "dn/string.h"
#include <string.h>

// == STRING STRUCT ========================================================= //

DnStr DnStr_Empty() {
  return (DnStr) {
    .data = nullptr,
    .capacity = 0,
    .length = 0
  };
}

DnStr DnStr_Create(const DnMemAllocator* allocator, u64 capacity) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(capacity != 0);

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, capacity + 1);
  data[0] = '\0';

  return (DnStr) {
    .data = data,
    .capacity = capacity,
    .length = 0
  };
}

void DnStr_Destroy(const DnMemAllocator* allocator, DnStr* string) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  DN_ASSERT(DnStr_IsValid(*string));
  DN_MEM_FREE(allocator, string->data);
}

DnStr DnStr_Clone(const DnMemAllocator* allocator, const DnStr* string) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  if (string->data == nullptr)
    return DnStr_Empty();

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, string->length + 1);
  memcpy(data, string->data, string->length + 1);

  return (DnStr) {
    .data = data,
    .capacity = string->length + 1,
    .length = string->length
  };
}

DnStr DnStr_FromView(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(DnStrView_IsValid(view));

  if (DnStrView_IsEmpty(view))
    return DnStr_Empty();

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, view.length + 1);
  memcpy(data, view.data, view.length);
  data[view.length] = '\0';

  return (DnStr) {
    .data = data,
    .capacity = view.length + 1,
    .length = view.length
  };
}

DnStr DnStr_FromCStr(const DnMemAllocator* allocator, const char* string) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  u64 length = strlen(string);
  if (length == 0)
    return DnStr_Empty();
  
  char* data = DN_MEM_ALLOC_TYPES(allocator, char, length + 1);
  memcpy(data, string, length + 1);

  return (DnStr) {
    .data = data,
    .capacity = length + 1,
    .length = length
  };
}

DnStr DnStr_FromCStrLength(const DnMemAllocator* allocator, const char* string, u64 length) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  if (length == 0)
    return DnStr_Empty();

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, length + 1);
  memcpy(data, string, length);
  data[length] = '\0';

  return (DnStr) {
    .data = data,
    .capacity = length + 1,
    .length = length
  };
}

bool DnStr_IsEmpty(const DnStr* string) {
  return string->length == 0;
}

bool DnStr_IsValid(const DnStr* string) {
  DN_ASSERT(string != nullptr);
  DN_ASSERT(string->length <= INT64_MAX);

  if (string->data) {
    return string->capacity > 0;
  }
  else {
    return string->capacity == 0 && string->length == 0;
  }
}

DnStrView DnStr_AsView(const DnStr* string) {
  return (DnStrView) {
    .data = string->data,
    .length = string->length
  };
}

const char* DnStr_AsCStr(const DnStr* string) {
  return string->data ? string->data : "";
}

// == STRING FUNCTIONS ====================================================== //

void DnStr_EnsureCapacity(const DnMemAllocator* allocator, DnStr* string, u64 length) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(DnStr_IsValid(string));

  u64 newCapacity = length + 1;
  if (newCapacity > string->capacity) {
    string->data = DN_MEM_REALLOC_TYPES(allocator, string->data, char, newCapacity);
    string->capacity = newCapacity;
  }
}

void DnStr_Clear(DnStr* string) {
  DN_ASSERT(DnStr_IsValid(string));

  if (string->data) {
    string->data[0] = '\0';
    string->length = 0;
  }
}

void DnStr_Append(const DnMemAllocator* allocator, DnStr* string, DnStrView view) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(DnStr_IsValid(string));

  u64 newLength = string->length + view.length;
  DnStr_EnsureCapacity(allocator, string, newLength);

  memcpy(string->data + string->length, view.data, view.length);
  string->data[newLength] = '\0'; 
  string->length = newLength;
}

void DnStr_Concat(const DnMemAllocator* allocator, DnStr* string, ...) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(DnStr_IsValid(string));
  
  u64 length = 0;

  va_list args;
  va_start(args, string);
  while (true) {
    DnStrView view = va_arg(args, DnStrView);
    DN_ASSERT(DnStrView_IsValid(view));

    if (view.data == nullptr) {
      break;
    }

    length += view.length;
  }
  va_end(args);

  DnStr_Clear(string);
  DnStr_EnsureCapacity(allocator, string, length);

  va_start(args, string);
  while (true) {
    DnStrView view = va_arg(args, DnStrView);
    if (view.data == nullptr) {
      break;
    }

    memcpy(string->data + string->length, view.data, view.length);
    string->length += view.length;
  }
  DN_ASSERT(string->length == length);
  string->data[string->length] = '\0';
  va_end(args);
}

void DnStr_Map(DnStr* string, DnRange range, DnStrView from, DnStrView to) {
  DN_ASSERT(string != nullptr);
  DN_ASSERT(DnStrView_IsValid(from));
  DN_ASSERT(DnStrView_IsValid(to));
  DN_ASSERT(from.length == to.length);

  DnRange_ToIndices(&range, string->length);
  for (i64 i = range.start; i < range.end; ++i) {
    char* character = &string->data[i];
    for (i64 c = 0; c < (i64)from.length; ++c) {
      if (*character == from.data[c]) {
        *character = to.data[c];
      }
    }
  }
}
