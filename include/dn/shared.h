#pragma once

// == STANDARD INCLUDES ===================================================== //

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <memory.h>

// == PREDEFINED TYPES ====================================================== //

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

// == GLOBAL DEFINES ======================================================== //

// Defines whether logging is enabled.
#define DN_LOG_ENABLED !DN_CONFIG_RELEASE

// Defines whether assertions are enabled.
#define DN_ASSERT_ENABLED !DN_CONFIG_RELEASE

// == COMPILER MACROS ======================================================= //

// Marks variable as intentionally unused to suppress compiler warnings.
#define DN_UNUSED(x) (void)(x)

// Marks an expression as likely to be true, for use in branch prediction.
#define DN_LIKELY(expression) __builtin_expect(!!(expression), 1)

// Marks an expression as unlikely to be true, for use in branch prediction.
#define DN_UNLIKELY(expression) __builtin_expect(!!(expression), 0)

// Triggers a breakpoint in the debugger.
#define DN_BREAK() __builtin_debugtrap()

// Aborts the process.
#define DN_ABORT() __builtin_trap()

// == COMMON MACROS ========================================================= //

// Returns the length of a statically allocated array.
#define DN_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

// Swaps the values of two variables of the same type.
#define DN_SWAP(a, b) ({ \
    auto _temp = a; \
    a = b; \
    b = _temp; \
  })

// == LOGGING MACROS ======================================================== //

#if DN_LOG_ENABLED
  void DnLog_Info(const char* format, ...);
  void DnLog_Error(const char* format, ...);

  // Logs an info message using printf-style formatting.
  #define DN_LOG_INFO(format, ...) DnLog_Info(format "\n" __VA_OPT__(,) __VA_ARGS__)

  // Logs an error message using printf-style formatting.
  #define DN_LOG_ERROR(format, ...) DnLog_Error(format "\n" __VA_OPT__(,) __VA_ARGS__)
#else
  #define DN_LOG_INFO(format, ...)
  #define DN_LOG_ERROR(format, ...)
#endif // DN_LOG_ENABLED

// == ASSERTION MACROS ====================================================== //

// Internal assert implementation function.
void DnAssert_Internal(const char* expression, const char* file, u64 line);

// Assertion macro that is always executed, even when assertions are disabled.
#define DN_ASSERT_ALWAYS(expression) ({ \
    if (DN_UNLIKELY(!(expression))) { \
      DnAssert_Internal(#expression, __FILE__, __LINE__); \
      DN_ABORT(); \
    } \
  })

#if DN_ASSERT_ENABLED
  // Assertion macro that is only executed when assertions are enabled.
  // Evaluates the expression and triggers fatal error if it fails.
  #define DN_ASSERT(expression) DN_ASSERT_ALWAYS(expression)

  // Assertion macro that is always evaluated, even when assertions are
  // disabled. Triggers a fatal error if it fails, but only when assertions are
  // enabled.
  #define DN_ASSERT_EVALUATE(expression) DN_ASSERT_ALWAYS(expression)
#else
  #define DN_ASSERT(expression)
  #define DN_ASSERT_EVALUATE(expression) (void)(expression)
#endif // DN_ASSERT_ENABLED

// Static assertion for comparing types with qualifiers.
#define DN_ASSERT_TYPES_EQUAL(left, right) \
  static_assert(_Generic(left, right: true, default: false), \
  "Types are not equal: " #left " and " #right);

// Static assertion for comparing types without qualifiers.
#define DN_ASSERT_TYPES_EQUAL_UNQUAL(left, right) \
  static_assert(_Generic(typeof_unqual(left), typeof_unqual(right): true, default: false), \
  "Unqualified types are not equal: " #left " and " #right);

// == POSITIONAL INDEXING =================================================== //

// Some functions can accept or return positional ranges instead of zero-based
// indexes that can be positive integers between 1:length, or negative integers
// between -length:-1, where zero and length+1 both indicate end position past
// the last element. This is most commonly used across strings and can be
// visualized below for a string of 7 length:
// +-----+----+----+----+----+----+----+----+----+
// | pos | +1 | +2 | +3 | +4 | +5 | +6 | +7 | +8 |
// +-----+----+----+----+----+----+----+----+----+
// | chr |  D |  o |  a |  n |  L |  i |  b | \0 |
// +-----+----+----+----+----+----+----+----+----+
// | neg | -7 | -6 | -5 | -4 | -3 | -2 | -1 |  0 |
// +-----+----+----+----+----+----+----+----+----+
// This might feel a bit awkward at first compared to zero-based indexing, but
// it is a more natural way of representing ranges relative to the beginning or
// the end of a collection (where for zero-based indexing zero has only one
// meaning for the first element). Here zero intuitively always represents the
// end of the collection (past the last element) and range for all elements can
// be specified without knowing collection length in advance (e.g. entire string
// can be denoted by 1:0 range). Below calls will produce identical results for
// the same string (returning "Lib"):
// DnStrView_Substr("DoanLib", DN_RANGE(5, 8));
// DnStrView_Substr("DoanLib", DN_RANGE(5, 0));
// DnStrView_Substr("DoanLib", DN_RANGE(-3, 8));
// DnStrView_Substr("DoanLib", DN_RANGE(-3, 0));
typedef struct DnRange {
  i64 start;
  i64 end;
} DnRange;

// Utility macro for shorter range struct creation.
#define DN_RANGE(start, end) (DnRange){ .start = start, .end = end }

// Utility macro for specifying full range.
#define DN_RANGE_ALL DN_RANGE(1, 0)

// Converts position to zero-based index.
void DnRange_ToIndex(i64* i, u64 length);

// Converts zero-based index to position.
void DnRange_ToPosition(i64* i);

// Converts range struct to zero-based indices.
void DnRange_ToIndices(DnRange* range, u64 length);

// Returns length of a positional range struct.
// Requires collection's length to calculate range's length.
u64 DnRange_GetLength(DnRange range, u64 length);
