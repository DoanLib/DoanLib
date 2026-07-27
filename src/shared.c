#include "dn/shared.h"
#include <stdio.h>

// == STATIC ASSERTIONS ===================================================== //

static_assert(sizeof(i8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

// == LOGGING =============================================================== //

#if DN_LOG_ENABLED

void DnLog_Info(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  fflush(stdout);
  va_end(args);
}

void DnLog_Error(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  fflush(stdout);
  fflush(stderr);
  va_end(args);
}

#endif // DN_LOG_ENABLED

// == POSITIONAL INDEXING =================================================== //

void DnRange_ToIndex(i64* i, u64 length) {
  DN_ASSERT(i != nullptr);
  DN_ASSERT(length <= INT64_MAX);
  *i = *i <= 0 ? *i + (i64)length : *i - 1;
  DN_ASSERT(*i >= 0 && *i <= (i64)length);
}

void DnRange_FromIndex(i64* i) {
  DN_ASSERT(i != nullptr);
  DN_ASSERT(*i <= INT64_MAX);
  *i = *i + 1;
}

void DnRange_ToIndices(DnRange* range, u64 length) {
  DN_ASSERT(range != nullptr);
  DnRange_ToIndex(&range->start, length);
  DnRange_ToIndex(&range->end, length);

  if (range->start > range->end) {
    DN_SWAP(range->start, range->end);
  }
}

u64 DnRange_GetLength(DnRange range, u64 length) {
  DnRange_ToIndices(&range, length);
  return (u64)(range.end - range.start);
}
