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

#define DN_ARRAY_TO_INTERNAL(arrayPtr) ({ \
    auto _arrayPtr = arrayPtr; \
    (DnArrayInternal) { \
      .data = (void**)&_arrayPtr->data, \
      .capacity = &_arrayPtr->capacity, \
      .length = &_arrayPtr->length, \
      .typeSize = sizeof(typeof(*_arrayPtr->data)), \
      .typeAlignment = alignof(typeof(*_arrayPtr->data)), \
    }; \
  })

void DnArrayInternal_Init(const DnMemAllocator* allocator, DnArrayInternal* array, u64 initialCapacity);
void DnArrayInternal_Deinit(const DnMemAllocator* allocator, DnArrayInternal* array);
void DnArrayInternal_Reserve(const DnMemAllocator* allocator, DnArrayInternal* array, u64 exactCapacity);
void DnArrayInternal_Resize(const DnMemAllocator* allocator, DnArrayInternal* array, u64 elementCount);
void DnArrayInternal_Append(const DnMemAllocator* allocator, DnArrayInternal* array, const void* element);
void DnArrayInternal_Clear(DnArrayInternal* array);
