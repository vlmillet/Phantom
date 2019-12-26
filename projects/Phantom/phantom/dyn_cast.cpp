// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "Rtti.h"
#include "dyn_cast.h"

namespace phantom
{
PHANTOM_EXPORT_PHANTOM void* dyn_cast(reflection::Class* a_pClass, void* a_pPointer)
{
    auto& rttiData = Rtti::Find(a_pPointer);
    if (rttiData.isNull())
        return nullptr;
    return rttiData.cast(a_pClass);
}
} // namespace phantom
