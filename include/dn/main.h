#pragma once

#include "structs.h"
#include "string.h"
#include "internal/main.h"

// == EXIT CODES ============================================================ //

// List of known exit codes returned by the library.
typedef enum DnExitCode : int {
  DnExitCode_Success = 0,
  DnExitCode_UnknownFailure = 1,
  DnExitCode_LibInitFailure = 2,
  DnExitCode_AppInitFailure = 3,
  DnExitCode_TestFailure = 4,
  DnExitCode_CommandLineFailure = 5,
} DnExitCode;

// == MAIN ENTRY ============================================================ //

// Helps define the main entry point for the library. This is for convenience
// and for aid in correctness of initialization and deinitialization.
#define DN_DEFINE_MAIN_ENTRY() \
  int main(int argc, char* argv[]) { \
    return DnMainWrapper(argc, argv); \
  } \
  DnExitCode DnMain()

// == LIBRARY INITITIALIZATION ============================================== //

// Initializes the library. Should be called before any other library functions
// at the very beginning of main().
bool DnLib_Init();

// Deinitializes the library. Should be called after all other library functions
// have been called at the very end of main().
void DnLib_Deinit();

// == COMMAND LINE ========================================================== //

// Command line argument structure.
typedef struct DnCmdLine {
  DnStrView program;
  DnArray(DnStrView) arguments;
} DnCmdLine;

// Retrieves the main commandline for the current process.
DnCmdLine* DnCmdLine_GetMain();

// Initializes command line structure using arguments passed to main entry.
bool DnCmdLine_InitFromMain(DnCmdLine* commandLine, int argc, char* argv[]);

// Deinitializes command line structure and frees any allocated memory.
void DnCmdLine_Deinit(DnCmdLine* commandLine);

// Checks if command line contains an argument.
bool DnCmdLine_HasArgument(const DnCmdLine* commandLine, DnStrView argument);
