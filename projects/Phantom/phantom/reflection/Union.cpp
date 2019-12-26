// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Union.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Union::Union(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(TypeKind::Union, PHANTOM_NEW(ExtraData), a_strName, a_Modifiers, a_uiFlags)
{
}

} // namespace reflection
} // namespace phantom
