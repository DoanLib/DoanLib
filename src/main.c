#include "dn/main.h"
#include "dn/memory.h"

// == MAIN ENTRY ============================================================ //

static DnCmdLine g_commandLine = {};

DnExitCode DnMainWrapper(int argc, char* argv[]) {
  DnExitCode result = DnExitCode_UnknownFailure;

  if (!DnCmdLine_InitFromMain(&g_commandLine, argc, argv)) {
    result = DnExitCode_CommandLineFailure;
    goto error;
  }

  if (!DnLib_Init()) {
    result = DnExitCode_LibInitFailure;
    goto error;
  }

  DnMemTempScope tempScope = DnMemTemp_PushScope();
  result = DnMain();
  DnMemTemp_PopScope(&tempScope);

error:
  DnLib_Deinit();
  return result;
}

// == LIBRARY INITIALIZATION ================================================ //

bool DnLib_Init() {
  if (!DnMem_Init()) {
    return false;
  }

  return true;
}

void DnLib_Deinit() {
  DnMem_Deinit();
}

// == COMMAND LINE ========================================================== //

DnCmdLine* DnCmdLine_GetMain() {
  return &g_commandLine;
}

bool DnCmdLine_InitFromMain(DnCmdLine* commandLine, int argc, char* argv[]) {
  DN_ASSERT(commandLine != nullptr);
  
  if (argc <= 0 || argv == nullptr) {
    return false;
  }

  commandLine->program = DnStrView_FromCStr(argv[0]);

  DnArray_Init(DnMem_DefaultAllocator(),
    &commandLine->arguments, (u64)(argc - 1));

  for (u64 i = 1; i < (u64)argc; ++i) {
    DnArray_Append(DnMem_DefaultAllocator(),
      &commandLine->arguments, DnStrView_FromCStr(argv[i]));
  }

  return true;
}

void DnCmdLine_Deinit(DnCmdLine* commandLine) {
  DN_ASSERT(commandLine != nullptr);
  DnArray_Deinit(DnMem_DefaultAllocator(), &commandLine->arguments);
}

bool DnCmdLine_HasArgument(const DnCmdLine* commandLine, DnStrView argument) {
  DN_ASSERT(commandLine != nullptr);

  // #todo: String comparison should be case-insensitive.
  for (u64 i = 0; i < commandLine->arguments.length; ++i) {
    if (DnStrView_Compare(commandLine->arguments.data[i], argument)) {
      return true;
    }
  }

  return false;
}
