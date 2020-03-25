// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Reference.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Reference::Reference(TypeKind a_eTypeKind, Type* a_pReferencedType, StringView a_strName)
    : Type(a_eTypeKind, a_pReferencedType, a_strName, sizeof(void*), std::alignment_of<void*>::value, 0,
           a_pReferencedType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
}

Reference::~Reference() {}

void Reference::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
    if (m_pUnderlyingType == a_pElement)
    {
        m_pUnderlyingType = nullptr;
    }
}

} // namespace lang
} // namespace phantom
