// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/core.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Callable
{
public:
    virtual void     call(void** a_pArgs) const = 0;
    virtual void     call(void** a_pArgs, void* a_pReturnAddress) const = 0;
    virtual void     invoke(void* a_pThis, void** a_pArgs) const = 0;
    virtual void     invoke(void* a_pThis, void** a_pArgs, void* a_pReturnAddress) const = 0;
    virtual uint64_t getUniqueID() const = 0;
    virtual void     saveArgs(void** a_pArgs, SmallVector<Variant, 10>& a_variants) const = 0;
    virtual void     onDisconnect() {}
};

} // namespace lang
} // namespace phantom
