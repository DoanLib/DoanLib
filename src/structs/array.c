#include <dn/structs.h>

// == ARRAY INTERNAL ======================================================== //

void DnArrayInternal_Init(const DnMemAllocator* allocator, DnArrayInternal* array, u64 initialCapacity) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  *array->data = DN_MEM_ALLOC(allocator, initialCapacity * array->typeSize, array->typeAlignment);
  *array->capacity = initialCapacity;
  *array->length = 0;
}

void DnArrayInternal_Deinit(const DnMemAllocator* allocator, DnArrayInternal* array) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  DN_MEM_FREE(allocator, *array->data);
  *array->data = nullptr;
  *array->capacity = 0;
  *array->length = 0;
}

void DnArrayInternal_Reserve(const DnMemAllocator* allocator, DnArrayInternal* array, u64 exactCapacity) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  if (*array->capacity < exactCapacity) {
    *array->data = DN_MEM_REALLOC(allocator, *array->data, exactCapacity * array->typeSize, array->typeAlignment);
    *array->capacity = exactCapacity;
  }
}

void DnArrayInternal_Resize(const DnMemAllocator* allocator, DnArrayInternal* array, u64 elementCount) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  if (elementCount > *array->length) {
    DnArrayInternal_Reserve(allocator, array, elementCount);
    memset((u8*)*array->data + (*array->length * array->typeSize), 0, elementCount - *array->length);
  }

  *array->length = elementCount;
}

void DnArrayInternal_Append(const DnMemAllocator* allocator, DnArrayInternal* array, const void* element) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  DnArrayInternal_Reserve(allocator, array, *array->length + 1);
  memcpy((u8*)*array->data + (*array->length * array->typeSize), element, array->typeSize);
  *array->length += 1;
}

void DnArrayInternal_Clear(DnArrayInternal* array) {
  DN_ASSERT(array != nullptr);
  *array->length = 0;
}
