#pragma once

// == MAIN ENTRY INTERNAL =================================================== //

typedef enum DnExitCode : int DnExitCode;
typedef struct DnCmdLine DnCmdLine;

DnExitCode DnMainWrapper(int argc, char* argv[]);
DnExitCode DnMain();
