#include "dn/string.h"

// == STRING INDEXING ======================================================= //

void DnStr_PositionToIndex(i64* i, u64 length) {
  DN_ASSERT(i != nullptr);
  DN_ASSERT(length <= INT64_MAX);
  *i = *i <= 0 ? *i + (i64)length : *i - 1;
}

void DnStr_RangeToIndices(i64* i, i64* j, u64 length) {
  DnStr_PositionToIndex(i, length);
  DnStr_PositionToIndex(j, length);

  if (*i > *j) {
    DN_SWAP(*i, *j);
  }

  DN_ASSERT(*i >= 0 && *j <= (i64)length);
}
