#pragma once
#include "wbx/atomic.h"
#include "wbx/Declarations.h"

namespace RBX
{

namespace Diagnostics
{

template<typename T>
class RBXBaseClass Countable
{
    static rbx::atomic<int> count;
public:
    static long getCount() { return count; }
    ~Countable()
    {
        --count;
    }
protected:
    Countable()
    {
        ++count;
    }
};

template<class T>
rbx::atomic<int> Countable<T>::count;

} // namespace Diagnostics

} // namespace RBX
