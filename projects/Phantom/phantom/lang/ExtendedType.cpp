// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ExtendedType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
ExtendedType::ExtendedType(Type* a_pUnderlyingType, TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize,
                           size_t a_uiAlignment, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags),
      m_pUnderlyingType(a_pUnderlyingType)
{
    PHANTOM_ASSERT(m_pUnderlyingType);
    addReferencedElement(m_pUnderlyingType);
}

void ExtendedType::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pUnderlyingType == a_pElement)
    {
        m_pUnderlyingType = nullptr;
    }
}

} // namespace lang
} // namespace phantom
