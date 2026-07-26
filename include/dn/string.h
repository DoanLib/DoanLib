#pragma once

#include "shared.h"
#include "memory.h"

// == STRING INDEXING ======================================================= //

// Most string functions accept and return positional integerts of characters
// that can be positive numbers from 1 to length, or negative numbers from
// -length to -1, where zero and length+1 both indicate end position past the
// last character. This can be visualized below for a string of 7 length
// (excluding null):
// +-----+----+----+----+----+----+----+----+----+
// | pos | +1 | +2 | +3 | +4 | +5 | +6 | +7 | +8 |
// +-----+----+----+----+----+----+----+----+----+
// | chr |  D |  o |  a |  n |  L |  i |  b | \0 |
// +-----+----+----+----+----+----+----+----+----+
// | neg | -7 | -6 | -5 | -4 | -3 | -2 | -1 |  0 |
// +-----+----+----+----+----+----+----+----+----+
// This might feel a bit confusing at first compared to the more common
// zero-based indexing, but it is a more natural way of representing string
// ranges that can start from the beginning or the end of a string (which
// zero-based indexing is awkward at due to zero having only one meaning for the
// first character). Here zero intuitivently always represents the end of the
// string. This allows ranges to be specified without need to know string length
// in advance (e.g. entire string is always denoted by 1:0 range). Below calls
// will produce identical results for the same
// string (returning "Lib"):
// DnStrView_Substr("DoanLib", 5, 8);
// DnStrView_Substr("DoanLib", 5, 0);
// DnStrView_Substr("DoanLib", -3, 8);
// DnStrView_Substr("DoanLib", -3, 0);

// Converts a string position to zero-based index.
void DnStr_PositionToIndex(i64* i, u64 length);

// Converts a string range to zero-based indices.
void DnStr_RangeToIndices(i64* i, i64* j, u64 length);

// == STRING MACROS ========================================================= //

// Printf format string for string view and its size.
// #todo: Create a custom printf format for string views to replace this.
#define DN_STR_VIEW_FMT "%.*s"

// String view arguments for printf format.
#define DN_STR_VIEW_ARG(view) (view.length), (view.data)

// Macros for creating string view from literal. Avoids calling strlen() by
// deducing length at compile time.
#define DN_STR_VIEW_LITERAL(text) ((DnStrView) { .data = text, .length = sizeof(text) - 1 })

// == STRING VIEW =========================================================== //

// String view struct represented as pointer to memory (which is not required to
// be null terminated) and length integer.
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
const char* DnStrView_AsCStr(const DnMemAllocator* allocator, DnStrView view);
