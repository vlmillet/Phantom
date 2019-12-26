// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

namespace phantom
{
template<class MutexT>
struct LockGuard
{
    inline LockGuard(MutexT& _m) : m(&_m)
    {
        m->lock();
    }
    inline ~LockGuard()
    {
        m->unlock();
    }

private:
    MutexT* m;
};
} // namespace phantom
