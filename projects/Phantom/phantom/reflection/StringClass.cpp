// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "StringClass.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
StringClass::StringClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                         Modifiers a_Modifiers, uint a_uiFlags)
    : StlVectorClass(TypeKind::StringClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers,
                     a_uiFlags)
{
}

StringClass::StringClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : StlVectorClass(TypeKind::StringClass, a_strName, a_Modifiers, a_uiFlags)
{
}

} // namespace reflection
} // namespace phantom
