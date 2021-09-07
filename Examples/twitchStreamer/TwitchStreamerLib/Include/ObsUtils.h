#pragma once

#include "Framework.h"

// Trick if your application is crashing only when started outside the debugger on startup.
// Add this function before the crash, then attach to the process. There's other ways to do
// this as well (regkeys, windbg), but this is reusable, quick dirty and easy to understand.
void TSL_EXPORT WaitForDebuggerToAttach();
