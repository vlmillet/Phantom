#include "Signal.h"

#include <phantom/thread/SpinMutex.h>
#include <type_traits>

namespace phantom
{
namespace _Signal
{
const size_t BucketSize = 256;

struct SlotMem
{
    typename IntTypeBySize<PHANTOM_ALIGNOF(Slot<>)>::type mem[sizeof(Slot<>) / PHANTOM_ALIGNOF(Slot<>)];
};

using SignalPoolBucket = SmallVector<SlotMem, BucketSize>;
struct SignalPool
{
    SignalPool() { buckets.emplace_back(&firstBucket); }
    ~SignalPool()
    {
        for (size_t i = 1; i < buckets.size(); ++i)
        {
            buckets[i]->~SignalPoolBucket();
            phantom::deallocate(buckets[i]);
        }
    }
    void* New()
    {
        if (freeList.size())
        {
            void* s = freeList.back();
            freeList.pop_back();
            return s;
        }
        if (buckets.back()->size() >= BucketSize)
            buckets.emplace_back(new (phantom::allocate(sizeof(SignalPoolBucket), PHANTOM_ALIGNOF(SignalPoolBucket)))
                                 SignalPoolBucket);

        return &buckets.back()->emplace_back();
    }
    void                                Delete(void* _slot) { freeList.push_back(_slot); }
    SignalPoolBucket                    firstBucket;
    SmallVector<SignalPoolBucket*, 256> buckets;
    SmallVector<void*, 256>             freeList;
};

StaticGlobal<SignalPool> g_SignalPool;
SpinMutex                g_SignalPoolMtx;

PHANTOM_EXPORT_PHANTOM void* _Signal::AllocateSlot()
{
    auto lock = g_SignalPoolMtx.autoLock();
    return g_SignalPool->New();
}

PHANTOM_EXPORT_PHANTOM void _Signal::DeallocateSlot(void* a_pSlot)
{
    auto lock = g_SignalPoolMtx.autoLock();
    g_SignalPool->Delete(a_pSlot);
}

} // namespace _Signal
} // namespace phantom
