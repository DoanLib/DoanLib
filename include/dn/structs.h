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
    auto _internal = DN_ARRAY_TO_INTERNAL(array); \
    DnArrayInternal_Init(allocator, &_internal, initialCapacity); \
  })

#define DnArray_Deinit(allocator, array) ({ \
    auto _internal = DN_ARRAY_TO_INTERNAL(array); \
    DnArrayInternal_Deinit(allocator, &_internal); \
  })

#define DnArray_Reserve(allocator, array, exactCapacity) ({ \
    auto _internal = DN_ARRAY_TO_INTERNAL(array); \
    DnArrayInternal_Reserve(allocator, &_internal, exactCapacity); \
  })

#define DnArray_Resize(allocator, array, elementCount) ({ \
    auto _internal = DN_ARRAY_TO_INTERNAL(array); \
    DnArrayInternal_Resize(allocator, &_internal, elementCount); \
  })

#define DnArray_Append(allocator, array, element) ({ \
    auto _array = array; \
    auto _element = element; \
    auto _internal = DN_ARRAY_TO_INTERNAL(_array); \
    static_assert(DN_TYPE_COMPARE(_array->data, &_element), "Incompatible array element type"); \
    DnArrayInternal_Append(allocator, &_internal, (const void*)&_element); \
  })

#define DnArray_Clear(array) ({ \
    auto _internal = DN_ARRAY_TO_INTERNAL(array); \
    DnArrayInternal_Clear(&_internal); \
  })
