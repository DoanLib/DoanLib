#pragma once

// == MAIN ENTRY INTERNAL =================================================== //

typedef enum DnExitCode : int DnExitCode;

DnExitCode DnMainWrapper(int argc, char* argv[]);
DnExitCode DnMain();
