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

void DnArrayInternal_Reserve(const DnMemAllocator* allocator, DnArrayInternal* array, u64 neededCapacity) {
  DN_ASSERT(allocator != nullptr);
  DN_ASSERT(array != nullptr);

  if (*array->capacity < neededCapacity) {
    *array->data = DN_MEM_REALLOC(allocator, *array->data, neededCapacity * array->typeSize, array->typeAlignment);
    *array->capacity = neededCapacity;
  }
}
