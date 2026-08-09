#include <dn/memory.h>
#include <dn/main.h>

typedef struct DnTest_MemoryStruct {
  alignas(32)
  u64 value;
  u64 padding[4];
} DnTest_MemoryStruct;

static_assert(sizeof(DnTest_MemoryStruct) == 64);
static_assert(alignof(DnTest_MemoryStruct) == 32);

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

void DnTest_MemoryVirtual() {
  void* reserved = DnMemVirtual_Reserve(DnMem_SystemPageSize + 1);
  DN_ASSERT(reserved != nullptr);

  u64 size = DnMemVirtual_QuerySize(reserved);
  DN_ASSERT(size == DnMem_SystemPageSize * 2);

  void* committed = DnMemVirtual_Commit(reserved, DnMem_SystemPageSize * 2);
  DN_ASSERT(committed == reserved);

  DnMemVirtual_Release(reserved);
}

void DnTest_MemoryAllocate(const DnMemAllocator* allocator) {
  void* allocation = DN_MEM_ALLOC(allocator, 128, 16);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, 16));

  DN_MEM_FREE(allocator, allocation);
}

void DnTest_MemoryReallocate(const DnMemAllocator* allocator) {
  void* allocation = nullptr;
  allocation = DN_MEM_REALLOC(allocator, allocation, 256, 32);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, 32));

  for (u64 i = 0; i < 256; ++i) {
    ((u8*)allocation)[i] = (u8)i;
  }

  allocation = DN_MEM_REALLOC(allocator, allocation, 512, 32);
  DN_ASSERT(allocation != nullptr);

  for (u64 i = 0; i < 256; ++i) {
    DN_ASSERT(((u8*)allocation)[i] == (u8)i);
  }

  allocation = DN_MEM_REALLOC(allocator, allocation, 128, 32);
  DN_ASSERT(allocation != nullptr);

  for (u64 i = 0; i < 128; ++i) {
    DN_ASSERT(((u8*)allocation)[i] == (u8)i);
  }

  allocation = DN_MEM_REALLOC(allocator, allocation, 0, 0);
  DN_ASSERT(allocation == nullptr);
}

void DnTest_MemoryAllocateType(const DnMemAllocator* allocator) {
  DnTest_MemoryStruct* allocation = DN_MEM_ALLOC_TYPE(allocator, DnTest_MemoryStruct);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  DN_MEM_FREE(allocator, allocation);
}

void DnTest_MemoryAllocateTypes(const DnMemAllocator* allocator) {
  DnTest_MemoryStruct* allocation = DN_MEM_ALLOC_TYPES(allocator, DnTest_MemoryStruct, 8);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  DN_MEM_FREE(allocator, allocation);
}

void DnTest_MemoryReallocateTypes(const DnMemAllocator* allocator) {
  DnTest_MemoryStruct* allocation = nullptr;
  allocation = DN_MEM_REALLOC_TYPES(allocator, allocation, DnTest_MemoryStruct, 32);
  DN_ASSERT(allocation != nullptr);
  DN_ASSERT(DN_MEM_IS_ALIGNED(allocation, alignof(DnTest_MemoryStruct)));

  for (u64 i = 0; i < 32; ++i) {
    allocation[i].value = i;
  }

  allocation = DN_MEM_REALLOC_TYPES(allocator, allocation, DnTest_MemoryStruct, 64);
  DN_ASSERT(allocation != nullptr);

  for (u64 i = 0; i < 32; ++i) {
    DN_ASSERT(allocation[i].value == i);
  }

  allocation = DN_MEM_REALLOC_TYPES(allocator, allocation, DnTest_MemoryStruct, 32);
  DN_ASSERT(allocation != nullptr);

  for (u64 i = 0; i < 32; ++i) {
    DN_ASSERT(allocation[i].value == i);
  }

  allocation = DN_MEM_REALLOC_TYPES(allocator, allocation, DnTest_MemoryStruct, 0);
  DN_ASSERT(allocation == nullptr);
}

void DnTest_MemoryAllocationSuite(const DnMemAllocator* allocator) {
  DnTest_MemoryAllocate(allocator);
  DnTest_MemoryReallocate(allocator);
  DnTest_MemoryAllocateType(allocator);
  DnTest_MemoryAllocateTypes(allocator);
  DnTest_MemoryReallocateTypes(allocator);
}

void DnTest_MemoryDefault() {
  const DnMemAllocator* allocator = DnMemAllocator_GetDefault();
  DN_ASSERT(allocator != nullptr);

  for (u64 i = 0; i < 128; ++i) {
    DnTest_MemoryAllocationSuite(allocator);
  }
}

void DnTest_MemoryMalloc() {
  const DnMemAllocator* allocator = DnMemMalloc_GetAllocator();
  DN_ASSERT(allocator != nullptr);

  for (u64 i = 0; i < 128; ++i) {
    DnTest_MemoryAllocationSuite(allocator);
  }
}

void DnTest_MemoryLarge() {
  const DnMemAllocator* allocator = DnMemLarge_GetAllocator();
  DN_ASSERT(allocator != nullptr);

  for (u64 i = 0; i < 32; ++i) {
    DnTest_MemoryAllocationSuite(allocator);
  }
}

void DnTest_MemoryTemp() {
  const DnMemAllocator* allocator = DnMemTemp_GetAllocator();
  DN_ASSERT(allocator != nullptr);

  for (u64 i = 0; i < 16; ++i) {
    DnMemTempScope scope = DnMemTemp_PushScope();
    for (u64 j = 0; j < 32; ++j) {
      DnTest_MemoryAllocationSuite(allocator);
    }
    DnMemTemp_PopScope(&scope);
  }
}

void DnTest_MemoryArena() {
  DnMemArena* arena = DnMemArena_Create(DnMem_SystemPageSize);
  DN_ASSERT(arena != nullptr);

  const DnMemAllocator* allocator = DnMemArena_GetAllocator(arena);
  DN_ASSERT(allocator != nullptr);

  for (u64 j = 0; j < 128; ++j) {
    DnTest_MemoryAllocationSuite(allocator);
  }

  DnMemArena_Destroy(arena);
}

void DnTest_Memory() {
  if (!DnCmdLine_HasArgument(DnCmdLine_GetMain(), DN_STR_VIEW_LITERAL("-memory"))) {
    return;
  }

  DnTest_MemoryMacros();
  DnTest_MemoryVirtual();
  DnTest_MemoryDefault();
  DnTest_MemoryMalloc();
  DnTest_MemoryLarge();
  DnTest_MemoryTemp();
  DnTest_MemoryArena();
}
