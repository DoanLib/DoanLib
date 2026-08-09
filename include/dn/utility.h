#pragma once

#include "shared.h"
#include "string.h"

// == FILE UTILITY ========================================================== //

// Reads the contents of a file and returns allocated memory that contains data.
bool DnUtils_ReadFile(const DnMemAllocator* allocator, DnStrView path, u8** outData, u64* outSize);
