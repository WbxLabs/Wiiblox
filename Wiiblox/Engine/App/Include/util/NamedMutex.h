#pragma once

#ifdef _WIN32
#include <windows.h>

namespace WBX
{
class ScopedNamedMutex
{
    HANDLE hMutex;

public:
    ScopedNamedMutex(const char* name);
    ~ScopedNamedMutex();
};
} // namespace WBX
#endif // #ifdef _WIN32 