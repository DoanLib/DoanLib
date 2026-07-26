#pragma once

#include "shared.h"
#include "memory.h"

// == STRING INDEXING ======================================================= //

// Most string functions accept and return positional integerts of characters
// that can be positive range between 1:length, or negative range between
// -length:-1, where zero and length+1 both indicate end position past the last
// character. This can be visualized below for a string of 7 length:
// +-----+----+----+----+----+----+----+----+----+
// | pos | +1 | +2 | +3 | +4 | +5 | +6 | +7 | +8 |
// +-----+----+----+----+----+----+----+----+----+
// | chr |  D |  o |  a |  n |  L |  i |  b | \0 |
// +-----+----+----+----+----+----+----+----+----+
// | neg | -7 | -6 | -5 | -4 | -3 | -2 | -1 |  0 |
// +-----+----+----+----+----+----+----+----+----+
// This might feel a bit awkward at first compared to zero-based indexing, but
// it is a more natural way of representing string ranges relative to the
// beginning or the end of a string (where for zero-based indexing zero has only
// one meaning for the first character). Here zero intuitively always represents
// the end of the string, which ranges to be specified without need to know
// string length in advance (e.g. entire string is always denoted by 1:0 range).
// Below calls will produce identical results for the same string (returning
// "Lib"):
// DnStrView_Substr("DoanLib", 5, 8);
// DnStrView_Substr("DoanLib", 5, 0);
// DnStrView_Substr("DoanLib", -3, 8);
// DnStrView_Substr("DoanLib", -3, 0);

// Converts a string position to zero-based index.
void DnStr_PositionToIndex(i64* i, u64 length);

// Converts a string zero-based index to position.
void DnStr_IndexToPosition(i64* i);

// Converts a string range to zero-based indices.
void DnStr_RangeToIndices(i64* i, i64* j, u64 length);

// Returns length of a string range.
u64 DnStr_RangeLength(i64 i, i64 j, u64 length);

// == STRING MACROS ========================================================= //

// Printf format string for string view and its size.
// #todo: Create a custom printf format for string views to replace this.
#define DN_STR_VIEW_FMT "%.*s"

// String view arguments for printf format.
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)

// Creating string view from literal. Avoids calling strlen() by deducing length
// at compile time.
#define DN_STR_VIEW_LITERAL(text) DnStrView_FromCStrLength(text, sizeof(text) - 1)

// Creating string from literal. Avoids calling strlen() by deducing length at
// compile time. Must be destroyed with DnStr_Destroy() to free memory.
#define DN_STR_LITERAL(allocator, text) DnStr_FromCStrLength(allocator, text, sizeof(text) - 1)

// == STRING VIEW STRUCT ==================================================== //

// Immutable string view struct represented as pointer to memory (which is not
// required to be null terminated) and length integer.
typedef struct DnStrView {
  const char* data;
  u64 length;
} DnStrView;

// Create a string view from a null-terminated C string.
DnStrView DnStrView_FromCStr(const char* string);

// Create a string view from memory and known length.
DnStrView DnStrView_FromCStrLength(const char* string, u64 length);

// Checks whether string view is empty.
bool DnStrView_IsEmpty(DnStrView view);

// Converts string view to a null-terminated C string. This will always allocate
// memory for returned result, so it must be freed by the caller, even if the
// string view is empty or already null terminated. This is done to avoid
// ambiguitiy regarding onwnership of the returned memory. Use temporary
// allocator when possible to make temporary conversions to C string fast.
const char* DnStrView_ToCStr(const DnMemAllocator* allocator, DnStrView view);

#if DN_ASSERT_ENABLED

// Checks whether string is valid. Used for debug assertions.
bool DnStrView_IsValid(DnStrView view);

#endif // DN_ASSERT_ENABLED

// == STRING VIEW FUNCTIONS ================================================= //

// Returns substring of a string view.
DnStrView DnStrView_SubStr(DnStrView view, i64 i, i64 j);

// Compares two string views.
bool DnStrView_Compare(DnStrView first, DnStrView second);

// == STRING STRUCT ========================================================= //

// Mutable string struct with resizable memory buffer that is always
// null-terminated.
typedef struct DnStr {
  char* data;
  u64 capacity;
  u64 length;
} DnStr;

// Creates an empty string with specified capacity.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_Create(const DnMemAllocator* allocator, u64 capacity);

// Destroys a string and frees its memory.
void DnStr_Destroy(const DnMemAllocator* allocator, DnStr* string);

// Creates a string copy from another string.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_Clone(const DnMemAllocator* allocator, DnStr string);

// Creates a string copy from a string view.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_FromView(const DnMemAllocator* allocator, DnStrView view);

// Creates a string copy from a null-terminated C string.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_FromCStr(const DnMemAllocator* allocator, const char* string);

// Creates a string from memory and known length.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_FromCStrLength(const DnMemAllocator* allocator, const char* string, u64 length);

// Checks whether string is empty.
bool DnStr_IsEmpty(DnStr string);

// Returns string view of a string.
DnStrView DnStr_AsView(DnStr string);

// Returns null-terminated character buffer of a string. This function does not
// allocate since string buffer is internally always null-terminated.
const char* DnStr_AsCStr(DnStr string);

// == STRING FUNCTIONS ====================================================== //

// Ensures string capacity for specified length.
void DnStr_EnsureCapacity(const DnMemAllocator* allocator, DnStr* string, u64 length);

// Appends string view to a string.
void DnStr_Append(const DnMemAllocator* allocator, DnStr* string, DnStrView view);

// Concatenates multiple string views into a new string. Must be destroyed with
// DnStr_Destroy() to free memory.
DnStr DnStr_Concat(const DnMemAllocator* allocator, ...); 

// Create a reversed string from string view. Must be destroyed with
// DnStr_Destroy() to free memory.
DnStr DnStr_Reversed(const DnMemAllocator* allocator, DnStrView view);

// Reverses a string in place.
void DnStr_Reverse(DnStr* string, i64 i, i64 j);
