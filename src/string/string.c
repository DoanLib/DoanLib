#include "dn/string.h"
#include <string.h>

// == STRING INDEXING ======================================================= //

void DnStr_PositionToIndex(i64* i, u64 length) {
  DN_ASSERT(i != nullptr);
  DN_ASSERT(length <= INT64_MAX);
  *i = *i <= 0 ? *i + (i64)length : *i - 1;
  DN_ASSERT(*i >= 0 && *i <= (i64)length);
}

void DnStr_RangeToIndices(i64* i, i64* j, u64 length) {
  DnStr_PositionToIndex(i, length);
  DnStr_PositionToIndex(j, length);

  if (*i > *j) {
    DN_SWAP(*i, *j);
  }
}

// == STRING STRUCT ========================================================= //

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

  DN_MEM_FREE(allocator, string->data);
}

DnStr DnStr_Clone(const DnMemAllocator* allocator, DnStr string) {
  DN_ASSERT(allocator != nullptr);

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, string.length + 1);
  memcpy(data, string.data, string.length + 1);

  return (DnStr) {
    .data = data,
    .capacity = string.length + 1,
    .length = string.length
  };
}

DnStr DnStr_FromView(const DnMemAllocator* allocator, DnStrView view) {
  DN_ASSERT(allocator != nullptr);

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

  char* data = DN_MEM_ALLOC_TYPES(allocator, char, length + 1);
  memcpy(data, string, length);
  data[length] = '\0';

  return (DnStr) {
    .data = data,
    .capacity = length + 1,
    .length = length
  };
}

bool DnStr_IsEmpty(DnStr string) {
  return string.length == 0;
}

DnStrView DnStr_AsView(DnStr string) {
  return (DnStrView) {
    .data = string.data,
    .length = string.length
  };
}

const char* DnStr_AsCStr(DnStr string) {
  return string.data;
}

// == STRING FUNCTIONS ====================================================== //

void DnStr_EnsureCapacity(const DnMemAllocator* allocator, DnStr* string, u64 length) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  u64 newCapacity = length + 1;
  if (newCapacity > string->capacity) {
    string->data = DN_MEM_REALLOC_TYPES(allocator, string->data, char, newCapacity);
    string->capacity = newCapacity;
  }
}

void DnStr_Append(const DnMemAllocator* allocator, DnStr* string, DnStrView view) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(string != nullptr);

  u64 newLength = string->length + view.length;
  DnStr_EnsureCapacity(allocator, string, newLength);

  memcpy(string->data + string->length, view.data, view.length);
  string->data[newLength] = '\0'; 
  string->length = newLength;
}

DnStr DnStr_Concat(const DnMemAllocator* allocator, ...) {
  u64 length = 0;

  va_list args;
  va_start(args, allocator);
  while (true) {
    DnStrView view = va_arg(args, DnStrView);
    if (view.data == nullptr) {
      break;
    }

    DN_ASSERT(DnStrView_IsValid(view));
    length += view.length;
  }
  va_end(args);

  DnStr result = DnStr_Create(allocator, length);

  va_start(args, allocator);
  while (true) {
    DnStrView view = va_arg(args, DnStrView);
    if (view.data == nullptr) {
      break;
    }

    memcpy(result.data + result.length, view.data, view.length);
    result.length += view.length;
  }
  DN_ASSERT(result.length == length);
  result.data[result.length] = '\0';
  va_end(args);

  return result;
}

void DnStr_Reverse(DnStr* string) {
  DN_ASSERT(string != nullptr);

  for (u64 i = 0; i < string->length / 2; i++) {
    DN_SWAP(string->data[i], string->data[string->length - 1 - i]);
  }
}

DnStr DnStr_Reversed(const DnMemAllocator* allocator, DnStrView view) {
  DnStr result = DnStr_Create(allocator, view.length);
  for (u64 i = 0; i < view.length; i++) {
    result.data[i] = view.data[view.length - 1 - i];
  }
  result.data[view.length] = '\0';
  result.length = view.length;

  return result;
}
