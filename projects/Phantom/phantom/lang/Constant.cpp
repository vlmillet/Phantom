// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Constant.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Constant::Constant(Type* a_pValueType, Modifiers a_Modifiers /*= 0*/, uint a_Flags /*= 0*/)
    : Symbol("", a_Modifiers, a_Flags), m_pValueType(a_pValueType)
{
}

Constant::Constant(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_Flags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_Flags), m_pValueType(a_pValueType)
{
}

void Constant::getQualifiedName(StringBuffer& a_Buf) const
{
    if (!getName().empty())
    {
        return Symbol::getQualifiedName(a_Buf);
    }
    void* pValue = m_pValueType->allocate();
    getValue(pValue);
    m_pValueType->valueToLiteral(a_Buf, pValue);
    m_pValueType->deallocate(pValue);
}

void Constant::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return Constant::getQualifiedName(a_Buf);
}

void Constant::getDecoratedName(StringBuffer& a_Buf) const
{
    if (!getName().empty())
    {
        return Symbol::getDecoratedName(a_Buf);
    }
    void* pValue = m_pValueType->allocate();
    getValue(pValue);
    m_pValueType->valueToLiteral(a_Buf, pValue);
    m_pValueType->deallocate(pValue);
}

void Constant::getUniqueName(StringBuffer& a_Buf) const
{
    if (!getName().empty())
    {
        return Symbol::getUniqueName(a_Buf);
    }
    void* pValue = m_pValueType->allocate();
    getValue(pValue);
    m_pValueType->valueToLiteral(a_Buf, pValue);
    m_pValueType->deallocate(pValue);
}

void Constant::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    getRelativeName(a_pTo, a_Buf);
}
void Constant::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (!getName().empty())
    {
        return Symbol::getRelativeName(a_pTo, a_Buf);
    }
    void* pValue = m_pValueType->allocate();
    getValue(pValue);
    m_pValueType->valueToLiteral(a_Buf, pValue);
    m_pValueType->deallocate(pValue);
}

Constant* Constant::clone(LanguageElement* a_pOwner) const
{
    Constant* pC = cloneImpl(a_pOwner);
    pC->setCodeRange(getCodeRange());
    return pC;
}

} // namespace lang
} // namespace phantom
