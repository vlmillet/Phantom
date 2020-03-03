// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Union.h"

#include "phantom/detail/new.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Union::Union(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(TypeKind::Union, PHANTOM_NEW(ExtraData), a_strName, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
