// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Constant.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Constant::Constant(Type* a_pValueType, Modifiers a_Modifiers /*= 0*/, uint a_Flags /*= 0*/)
    : Symbol("", a_Modifiers, a_Flags), m_pValueType(a_pValueType)
{
    addReferencedElement(m_pValueType);
}

Constant::Constant(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_Flags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_Flags), m_pValueType(a_pValueType)
{
    addReferencedElement(m_pValueType);
}

void Constant::getQualifiedName(StringBuffer& a_Buf) const
{
    if (m_strName.size())
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
    if (m_strName.size())
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
    if (m_strName.size())
    {
        return Symbol::getUniqueName(a_Buf);
    }
    void* pValue = m_pValueType->allocate();
    getValue(pValue);
    m_pValueType->valueToLiteral(a_Buf, pValue);
    m_pValueType->deallocate(pValue);
}

Constant* Constant::clone() const
{
    Constant* pC = cloneImpl();
    pC->setCodeRange(getCodeRange());
    return pC;
}

} // namespace reflection
} // namespace phantom
