// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "RttiMapData.h"

#include <phantom/Object.h>
#include <phantom/reflection/Constant.h>
#include <phantom/reflection/Field.h>
#include <phantom/reflection/Pointer.h>

namespace phantom
{
void RttiMapData::asObject(Object& out) const
{
    out = Object(m_pObject, m_pObjectClass);
}

} // namespace phantom
