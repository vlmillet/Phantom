// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Reference.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Reference::Reference(TypeKind a_eTypeKind, Type* a_pReferencedType, StringView a_strName)
    : ExtendedType(a_pReferencedType, a_eTypeKind, a_strName, sizeof(void*), std::alignment_of<void*>::value,
                   a_pReferencedType->getModifiers(), a_pReferencedType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
}

Reference::~Reference()
{
}

void Reference::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
    if (m_pUnderlyingType == a_pElement)
    {
        m_pUnderlyingType = nullptr;
    }
}

} // namespace reflection
} // namespace phantom
