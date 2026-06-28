#pragma once

#include "wbx/Debug.h"

// Engine assertions often cause the game to stop running.
// Until we can fix these, turn them off.
//#define WBX_DEBUGENGINE

#ifdef WBX_DEBUGENGINE
#define WBX_ENGINE_ASSERT(expr) RBXASSERT(expr)
#else
#define WBX_ENGINE_ASSERT(expr) ((void)0)
#endif