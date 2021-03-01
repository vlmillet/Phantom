// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "StringClass.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
StringClass::StringClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                         uint a_uiFlags)
    : VectorClass(TypeKind::StringClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

StringClass::StringClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : VectorClass(TypeKind::StringClass, a_strName, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
