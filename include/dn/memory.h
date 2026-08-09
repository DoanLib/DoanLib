#pragma once

#include "shared.h"
#include "math.h"

// == MEMORY DEFINES ======================================================== //

// Defines whether debug memory patterns are enabled to aid debugging. Such
// patterns are used to mark memory regions that have been e.g. allocated or
// freed with easily recognizable patterns. Very useful when debugging both live
// processes and also memory dumps.
#define DN_MEM_PATTERNS_ENABLED !DN_CONFIG_RELEASE

// == MEMORY COMMON ========================================================= //

// Converts a value in kilobytes (consisting of 1024 bytes each) to bytes.
// Returns an integer value.
#define DN_MEM_KB(kilobytes) ((u64)(kilobytes) << 10)

// Converts a value in megabytes (consisting of 1024 kilobytes each) to bytes.
// Returns an integer value.
#define DN_MEM_MB(megabytes) ((u64)(megabytes) << 20)

// Converts a value in gigabytes (consisting of 1024 megabytes each) to bytes.
// Returns an integer value.
#define DN_MEM_GB(gigabytes) ((u64)(gigabytes) << 30)

// Converts a value of bytes to kilobytes (consisting of 1024 bytes each).
// Returns a floating-point value which may represent fraction of a kilobyte.
#define DN_MEM_TO_KB(bytes) ((f64)(bytes) / (1ull << 10))

// Converts a value of bytes to megabytes (consisting of 1024 kilobytes each).
// Returns a floating-point value which may represent fraction of a megabyte.
#define DN_MEM_TO_MB(bytes) ((f64)(bytes) / (1ull << 20))

// Converts a value of bytes to gigabytes (consisting of 1024 megabytes each).
// Returns a floating-point value which may represent fraction of a gigabyte.
#define DN_MEM_TO_GB(bytes) ((f64)(bytes) / (1ull << 30))

// Check whether given value is aligned to multiple of the specified size.
// Assumes that the alignment is a power of 2. Can also be used with pointers.
#define DN_MEM_IS_ALIGNED(size, alignment) ({ \
    u64 _size = (u64)(size); \
    u64 _alignment = (u64)(alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    (_size & (_alignment - 1)) == 0; \
  })

// Aligns the given size up to the nearest multiple of the specified alignment.
// Assumes that the alignment is a power of 2. Can also be used with pointers.
#define DN_MEM_ALIGN_UP(size, alignment) ({ \
    u64 _size = (u64)(size); \
    u64 _alignment = (u64)(alignment); \
    DN_ASSERT(DN_IS_POW2(_alignment)); \
    ((_size) + (_alignment - 1)) & ~(_alignment - 1); \
  })

// == MEMORY CONSTANTS ====================================================== //

// Default memory alignment in bytes for memory allocations, imposed by C
// standard library, which corresponds to the alignment of the largest primitive
// scalar type available on the platform.
constexpr u64 DnMem_DefaultAlignment = alignof(max_align_t);

// Granularity of system virtual memory address reservations in bytes. If you
// reserve and manage large ranges of memory, there is no reason not to use this
// granularity as the multiply of range size. For commits you should use system
// page size instead to not waste available commit space (backed by physical
// memory and disk), which is usually smaller than this value.
constexpr u64 DnMem_ReservationGranularity = DN_MEM_KB(64);

// Expected system memory page size in bytes that is used to align virtual
// memory address reservations and commits. Determines the effective granularity
// of memory page faults against physical memory.
constexpr u64 DnMem_SystemPageSize = DN_MEM_KB(4);

// Common threshold size for large allocations.
constexpr u64 DnMem_LargeSizeThreshold = DN_MEM_KB(128);

// Debugging memory fill patterns.
#if DN_MEM_PATTERNS_ENABLED
  // Pattern used to fill new memory after allocation.
  constexpr u8 DnMem_PatternAllocated = 0xCD;

  // Pattern used to fill existing memory after deallocation.
  constexpr u8 DnMem_PatternFreed = 0xDD;

  // Pattern used to fill padding in memory that is not meant to be used.
  constexpr u8 DnMem_PatternPadding = 0xFD;
#endif

// == MEMORY INITIALIZATION ================================================= //

// Initializes the memory system with the given configuration. Should be called
// only once at the very beginning of the program, before any memory operations.
bool DnMem_Init();

// Deinitializes the memory system. Should be called only once at the very end
// of the program, after all memory operations. With debugging enabled, may
// perform additional checks to validate memory integrity (e.g. checking for
// memory leaks).
void DnMem_Deinit();

// == MEMORY VIRTUAL ======================================================== //

// Reserves virtual address space of the given size without committing. The size
// is rounded up to a multiple of system page size. Returns the base address of
// the reserved region, or null if the reservation failed. Reserved memory
// cannot be accessed until it is committed via DnMemVirtual_Commit(). This
// syscall function is expensive and should be called as rarely as possible.
void* DnMemVirtual_Reserve(u64 size);

// Commits previously reserved region, making the specified range usable. The
// page address and size are expected to align with the system page size. Null
// page address can be specified to automatically reserve address space using
// single syscall. This syscall function is expensive and should be called as
// rarely as possible.
void* DnMemVirtual_Commit(void* page, u64 size);

// Decommits previously committed region, releasing the backing memory (either
// physical or on disk) while keeping the address space reserved. The page
// address and size are expected to align with the system page size. The memory
// region may be recommitted later via DnMemVirtual_Commit(). This syscall
// function is expensive and should be called as rarely as possible.
void DnMemVirtual_Decommit(void* page, u64 size);

// Releases an entire region of virtual address space that was previously
// reserved, freeing both the reservation and any committed memory. The page
// address must be the base address returned by the original reservation. This
// syscall function is expensive and should be called as rarely as possible.
void DnMemVirtual_Release(void* page);

// Returns the size of the virtual memory region starting at the given page
// address, or 0 if the page address is invalid. This syscall function is
// expensive and should be called as rarely as possible.
u64 DnMemVirtual_QuerySize(void* page);

// == MEMORY ALLOCATION ====================================================== //

// Shorthand macro for allocating memory from an allocator.
#define DN_MEM_ALLOC(allocator, size, alignment) ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->alloc(_allocator, size, alignment); \
  })

// Shorthand macro for reallocating memory from an allocator. When null
// allocation is passed, a new memory block is allocated. When zero size is
// passed, the memory block is freed. Returns a pointer to a memory block that
// may have been shrunk or expanded in-place, or to a new memory block with
// content copied into it if the existing one could not be resized. You should
// always assume that pointers to the reallocated memory are invalidated.
#define DN_MEM_REALLOC(allocator, allocation, size, alignment) ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->realloc(_allocator, allocation, size, alignment); \
  })

// Shorthand macro for freeing memory from an allocator.
#define DN_MEM_FREE(allocator, allocation) ({ \
    const DnMemAllocator* _allocator = allocator; \
    _allocator->free(_allocator, allocation); \
  })

// Shorthand macro for allocating type instance from an allocator.
#define DN_MEM_ALLOC_TYPE(allocator, type) \
  DN_MEM_ALLOC(allocator, sizeof(type), alignof(type))

// Shorthand macro for allocating array of types from an allocator.
#define DN_MEM_ALLOC_TYPES(allocator, type, count) \
  DN_MEM_ALLOC(allocator, sizeof(type) * (count), alignof(type))

// Shorthand macro for reallocating array of types from an allocator.
#define DN_MEM_REALLOC_TYPES(allocator, allocation, type, count) \
  DN_MEM_REALLOC(allocator, allocation, sizeof(type) * (count), alignof(type))

// == MEMORY ALLOCATOR ====================================================== //

typedef struct DnMemAllocator DnMemAllocator;

// Function pointer for memory allocator allocation method.
typedef void* (DnMemAllocatorAllocFunc)(const DnMemAllocator* allocator, u64 size, u64 alignment);

// Function pointer for memory allocator reallocation method.
typedef void* (DnMemAllocatorReallocFunc)(const DnMemAllocator* allocator, void* pointer, u64 size, u64 alignment);

// Function pointer for memory allocator free method.
typedef void (DnMemAllocatorFreeFunc)(const DnMemAllocator* allocator, void* pointer);

// Memory allocator interface which can be invoked through family of
// DN_MEM_ALLOC/REALLOC/FREE() macros for ease of use.
typedef struct DnMemAllocator {
  DnMemAllocatorAllocFunc* alloc;
  DnMemAllocatorReallocFunc* realloc;
  DnMemAllocatorFreeFunc* free;
  void* context;
} DnMemAllocator;

// Default general purpose memory allocator. Used for common allocation cases
// when there are no specialized allocators available for given purpose.
const DnMemAllocator* DnMemAllocator_GetDefault();

// Standard C library malloc memory allocator. Should be used only when
// interfacing with external libraries or when paired with memory debugging
// tools such as Valgrin or ASAN.
const DnMemAllocator* DnMemMalloc_GetAllocator();

// Large allocator that puts individual allocations into separate dedicated
// system memory pages for simplicity and lower memory fragmentation.
const DnMemAllocator* DnMemLarge_GetAllocator();

// == MEMORY ARENA ========================================================== //

// Memory arena allocator that represents a set of memory regions of given chunk
// size. The arena behaves like a simple bump allocator, allowing for fast
// allocation and deallocation of all regions at once, at a cost of potentially
// worse memory space utilization when allocations are intended to be freed.
// Freeing allocations is a no-op due to performance reasons. However, scoping
// functionality is provided to allow reverting to a previous state after
// performing a series of temporary allocations. Struct is an opaque type that
// resides on the first page of reserved memory region and inherits allocator
// interface.
typedef struct DnMemArena DnMemArena;

// Allocates a new memory arena with the given chunk size. The arena will
// allocate memory in chunks of the specified size (which will be rounded up to
// next system page size). This size indicates virtual address space usage for
// each chunk and will not reflect actual physical memory usage. The size should
// be a balance between too high value resulting in pressure on address space
// and too low value resulting in frequent chunk allocations.
DnMemArena* DnMemArena_Create(u64 chunkSize);

// Destroys an arena instance, freeing all allocations associated with it.
void DnMemArena_Destroy(DnMemArena* arena);

// Retrieves allocator interface for the arena.
const DnMemAllocator* DnMemArena_GetAllocator(const DnMemArena* arena);

// Represents a scope within an arena, used to record the state of the arena so
// it can be reverted to later, so allocations done within it can be then freed
// all at once when the scope is popped.
typedef struct DnMemArenaScope {
  alignas(DnMem_DefaultAlignment)
  char opaque[32];
} DnMemArenaScope;

// Records the current state of the arena so it can be reverted to later, so
// allocations done within it can be then freed all at once when the scope is
// popped.
DnMemArenaScope DnMemArena_PushScope(DnMemArena* arena);

// Restores the previously recorded state of an arena, unwinding allocations
// that were made since the scope was pushed.
void DnMemArena_PopScope(DnMemArenaScope* scope);

// == MEMORY TEMPORARY ====================================================== //

// Temporary memory allocator that uses an arena for efficient allocation of
// short-lived memory. Very efficient when used in combination with
// DnMemTemp_PushScope()/PopScope() for scoped allocations.
const DnMemAllocator* DnMemTemp_GetAllocator();

// Alias for DnMemArena scope which temporary allocator is backed by.
typedef struct DnMemArenaScope DnMemTempScope;

// Shorthand for calling DnMemArena_PushScope() on the temporary allocator
// backed by an arena.
DnMemTempScope DnMemTemp_PushScope();

// Shorthand for calling DnMemArena_PopScope() on the temporary allocator backed
// by an arena.
void DnMemTemp_PopScope(DnMemTempScope* scope);
