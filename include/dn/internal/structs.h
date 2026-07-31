#pragma once

#include "dn/shared.h"
#include "dn/memory.h"

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
void DnArrayInternal_Resize(const DnMemAllocator* allocator, DnArrayInternal* array, u64 elementCount);
void DnArrayInternal_Append(const DnMemAllocator* allocator, DnArrayInternal* array, const void* element);
void DnArrayInternal_Clear(DnArrayInternal* array);
