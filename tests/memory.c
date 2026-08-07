#include <dn/memory.h>
#include <dn/main.h>

void DnTest_MemoryMacros() {
  DN_ASSERT(DN_MEM_KB(1) == 1024);
  DN_ASSERT(DN_MEM_MB(1) == 1024 * 1024);
  DN_ASSERT(DN_MEM_GB(1) == 1024 * 1024 * 1024);

  DN_ASSERT(DN_NEAR_EQUAL(DN_MEM_TO_KB(1024), 1.0, 1e-9));
  DN_ASSERT(DN_NEAR_EQUAL(DN_MEM_TO_MB(1024 * 1024), 1.0, 1e-9));
  DN_ASSERT(DN_NEAR_EQUAL(DN_MEM_TO_GB(1024 * 1024 * 1024), 1.0, 1e-9));

  DN_ASSERT(DN_MEM_IS_ALIGNED(256, 64));
  DN_ASSERT(!DN_MEM_IS_ALIGNED(255, 64));
  DN_ASSERT(DN_MEM_ALIGN_UP(255, 64) == 256);
}

void DnTest_MemoryAllocation() {
  void* allocation = DN_MEM_ALLOC(DnMemAllocator_GetDefault(), 128, 16);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, 16));

  allocation = DN_MEM_REALLOC(DnMemAllocator_GetDefault(), allocation, 256, 32);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, 32));

  allocation = DN_MEM_REALLOC(DnMemAllocator_GetDefault(), allocation, 0, 0);
  DN_ASSERT(allocation == nullptr);

  allocation = DN_MEM_ALLOC(DnMemAllocator_GetDefault(), 64, 8);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, 8));

  DN_MEM_FREE(DnMemAllocator_GetDefault(), allocation);
}

typedef struct DnTest_MemoryStruct {
  alignas(32)
  u32 a;
  u32 b;
  u64 c;
  u64 d;
  u64 e;
  u64 f;
} DnTest_MemoryStruct;

static_assert(sizeof(DnTest_MemoryStruct) == 64);
static_assert(alignof(DnTest_MemoryStruct) == 32);

void DnTest_MemoryAllocationTyped() {
  DnTest_MemoryStruct* allocation = DN_MEM_ALLOC_TYPE(DnMemAllocator_GetDefault(), DnTest_MemoryStruct);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  allocation = DN_MEM_REALLOC_TYPES(DnMemAllocator_GetDefault(), allocation, DnTest_MemoryStruct, 0);
  DN_ASSERT(allocation == nullptr);

  allocation = DN_MEM_REALLOC_TYPES(DnMemAllocator_GetDefault(), allocation, DnTest_MemoryStruct, 4);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  DN_MEM_FREE(DnMemAllocator_GetDefault(), allocation);

  allocation = DN_MEM_ALLOC_TYPES(DnMemAllocator_GetDefault(), DnTest_MemoryStruct, 8);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  DN_MEM_FREE(DnMemAllocator_GetDefault(), allocation);
}

void DnTest_Memory() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-memory"))) {
    return;
  }

  DnTest_MemoryMacros();
  DnTest_MemoryAllocation();
  DnTest_MemoryAllocationTyped();
}
