#pragma once

#include "shared.h"
#include "memory.h"

// == STRING MACROS ========================================================= //

// Printf format string for string view and its size.
// #todo: Create a custom printf format for string views to replace this.
#define DN_STR_VIEW_FMT "%.*s"

// String view arguments for printf format.
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)

// Creating string view from literal. Avoids calling strlen() by deducing length
// at compile time. Returns struct directly to satisfy constexpr requirements.
#define DN_STR_VIEW_LITERAL(text) \
  (DnStrView) { \
    .data = text, \
    .length = sizeof(text) - 1, \
  }

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

// Checks whether string is valid. Used for debug assertions.
bool DnStrView_IsValid(DnStrView view);

// Converts string view to a null-terminated C string. This will always allocate
// memory for returned result, so it must be freed by the caller, even if the
// string view is empty or already null terminated. This is done to avoid
// ambiguitiy regarding onwnership of the returned memory. Use temporary
// allocator when possible to make temporary conversions to C string fast.
const char* DnStrView_ToCStr(const DnMemAllocator* allocator, DnStrView view);

// == STRING VIEW CONSTANTS ================================================= //

// Emptry string view.
extern const DnStrView DnStrView_Empty;

// String view consisting of letters (upper and lower case).
extern const DnStrView DnStrView_Letter;

// String view consisting of digits.
extern const DnStrView DnStrView_Digit;

// String view consisting of whitespaces.
extern const DnStrView DnStrView_WhiteSpace;

// String view consisting of letters and digits.
extern const DnStrView DnStrView_AlphaNumeric;

// String view consisting of upper case letters.
extern const DnStrView DnStrView_UpperCase;

// String view consistring of lower case letters.
extern const DnStrView DnStrView_LowerCase;

// == STRING VIEW FUNCTIONS ================================================= //

// Returns substring of a string view.
DnStrView DnStrView_SubStr(DnStrView view, DnRange range);

// Compares two string views.
bool DnStrView_Compare(DnStrView first, DnStrView second);

// Finds the first occurrence in a string view. Returns positive integer for
// position if found, zero if not found.
i64 DnStrView_Find(DnStrView view, DnStrView occurrence);

// Finds the last occurrence in a string view. Returns
// positive integer for position if found, zero if not found.
i64 DnStrView_FindReverse(DnStrView view, DnStrView occurrence);

// == STRING STRUCT ========================================================= //

// Mutable string struct with resizable memory buffer that is always
// null-terminated.
typedef struct DnStr {
  char* data;
  u64 capacity;
  u64 length;
} DnStr;

// Creates an empty string without any allocation. Even though not necessary, it
// is still a good practice to call DnStr_Destroy() on any string that might
// have been modified from initial empty construction.
DnStr DnStr_Empty();

// Creates an empty string with specified capacity.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_Create(const DnMemAllocator* allocator, u64 capacity);

// Destroys a string and frees its memory.
void DnStr_Destroy(const DnMemAllocator* allocator, DnStr* string);

// Creates a string copy from another string.
// Must be destroyed with DnStr_Destroy() to free memory.
DnStr DnStr_Clone(const DnMemAllocator* allocator, const DnStr* string);

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
bool DnStr_IsEmpty(const DnStr* string);

// Checks whether string is valid. Used for debug assertions.
bool DnStr_IsValid(const DnStr* string);

// Returns string view of a string.
DnStrView DnStr_AsView(const DnStr* string);

// Returns null-terminated character buffer of a string. This function does not
// allocate since string buffer is internally always null-terminated.
const char* DnStr_AsCStr(const DnStr* string);

// == STRING FUNCTIONS ====================================================== //

// Ensures string capacity for specified length.
void DnStr_EnsureCapacity(const DnMemAllocator* allocator, DnStr* string, u64 length);

// Clears string content without freeing memory.
void DnStr_Clear(DnStr* string);

// Appends string view to a string. It is undefined behavior to append string to
// itself, meaning pass view argument that points to memory of string argument.
void DnStr_Append(const DnMemAllocator* allocator, DnStr* string, DnStrView view);

// Concatenates multiple string views into a string. If existing string with
// capacity is passed, it is cleared before storing the result.
void DnStr_Concat(const DnMemAllocator* allocator, DnStr* string, ...);

// Maps characters of a string from to a set of new characters. From and to
// character set arguments must be of the exact same size.
void DnStr_Map(DnStr* string, DnRange range, DnStrView from, DnStrView to);
