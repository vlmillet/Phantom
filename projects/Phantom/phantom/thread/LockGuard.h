// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
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
