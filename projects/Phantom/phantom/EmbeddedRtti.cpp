// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "EmbeddedRtti.h"
#include "Object.h"

namespace phantom
{
void EmbeddedRtti::asObject(Object& a_Out) const
{
    a_Out = Object(instance, metaClass);
}
} // namespace phantom
