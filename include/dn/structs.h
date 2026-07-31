#pragma once

#include "internal/structs.h"

// == ARRAY ================================================================= //

#define DnArray(Type) \
  struct { \
    Type* data; \
    u64 capacity; \
    u64 length; \
  }

#define DnArray_Init(allocator, array, initialCapacity) ({ \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Init(allocator, &_array, initialCapacity); \
  })

#define DnArray_Deinit(allocator, array) ({ \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Deinit(allocator, &_array); \
  })

#define DnArray_Reserve(allocator, array, neededCapacity) ({ \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Reserve(allocator, &_array, neededCapacity); \
  })

#define DnArray_Resize(allocator, array, elementCount) ({ \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Resize(allocator, &_array, elementCount); \
  })

#define DnArray_Append(allocator, array, element) ({ \
    DN_ASSERT(typeof(*array->data) == typeof(element)); \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Append(allocator, array, (const void*)&element); \
  })

#define DnArray_Clear(array) ({ \
    auto _array = DN_ARRAY_INTERNAL_ERASURE(array); \
    DnArrayInternal_Clear(array); \
  })
