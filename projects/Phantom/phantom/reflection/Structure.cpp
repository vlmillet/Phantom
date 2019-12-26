// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Structure.h"
#include "ValueMember.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Structure::Structure(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(TypeKind::Structure, PHANTOM_NEW(ExtraData), a_strName, a_Modifiers, a_uiFlags)
{
}

Structure::Structure(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                     Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(TypeKind::Structure, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

} // namespace reflection
} // namespace phantom
