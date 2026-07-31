#pragma once

#include "shared.h"
#include "memory.h"

// == ARRAY INTERNAL ======================================================== //

typedef struct DnArrayInternal {
  void** data;
  u64* capacity;
  u64* length;
  u64 typeSize;
  u64 typeAlignment;
} DnArrayInternal;

#define DN_ARRAY_INTERNAL_ERASURE(array) ({ \
    auto _array = array; \
    (DnArrayInternal) { \
      .data = (void**)&_array->data, \
      .capacity = &_array->capacity, \
      .length = &_array->length, \
      .typeSize = sizeof(typeof(*_array->data)), \
      .typeAlignment = alignof(typeof(*_array->data)), \
    }; \
  })

void DnArrayInternal_Init(const DnMemAllocator* allocator, DnArrayInternal* array, u64 initialCapacity);
void DnArrayInternal_Deinit(const DnMemAllocator* allocator, DnArrayInternal* array);
void DnArrayInternal_Reserve(const DnMemAllocator* allocator, DnArrayInternal* array, u64 neededCapacity);

// == ARRAY TYPED =========================================================== //

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

#define DnArray_Resize
#define DnArray_Append
#define DnArray_Insert
#define DnArray_Erase
#define DnArray_Clear
