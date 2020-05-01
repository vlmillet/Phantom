// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Variable.h"

#include "Class.h"

#include <phantom/utils/LexicalCast.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Variable::Variable(Type* a_pValueType, void* a_pAddress, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(phantom::lexical_cast<String>(a_pAddress), a_Modifiers, a_uiFlags),
      m_pValueType(a_pValueType),
      m_pAddress(a_pAddress),
      m_pAnonymousSection(nullptr),
      m_pInitializationExpression(nullptr)
{
    PHANTOM_ASSERT(isNative());
    PHANTOM_ASSERT(m_pValueType);
    addReferencedElement(m_pValueType);
}

Variable::Variable(Type* a_pValueType, StringView a_strName, void* a_pAddress, Modifiers a_Modifiers /*= 0*/,
                   uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_pValueType(a_pValueType),
      m_pAddress(a_pAddress),
      m_pAnonymousSection(nullptr),
      m_pInitializationExpression(nullptr)
{
    PHANTOM_ASSERT(isNative());
    PHANTOM_ASSERT(m_pValueType);
    addReferencedElement(m_pValueType);
}

Variable::Variable(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_pValueType(a_pValueType),
      m_pAddress(a_pValueType ? a_pValueType->newInstance() : nullptr),
      m_pAnonymousSection(nullptr),
      m_pInitializationExpression(nullptr)
{
    PHANTOM_ASSERT(!isNative());
    if (m_pValueType)
    {
        addReferencedElement(m_pValueType);
    }
}

Variable::~Variable()
{
    if (!isNative() && m_pValueType)
    {
        m_pValueType->deleteInstance(m_pAddress);
    }
}

void Variable::setValue(void const* a_pInputBuffer) const
{
    m_pValueType->copyConstruct(m_pAddress, a_pInputBuffer);
}

void Variable::finalize()
{
    if (!isNative())
    {
        PHANTOM_ASSERT(m_pAddress == nullptr);
        m_pAddress = m_pValueType->newInstance();
    }
}

StaticField* Variable::asStaticField() const
{
    return (getOwner() == nullptr || getOwner()->asClassType()) ? (StaticField*)this : nullptr;
}

void Variable::setValueType(Type* a_pType)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(a_pType);
    PHANTOM_ASSERT(m_pValueType);
    removeReferencedElement(m_pValueType);
    m_pValueType = a_pType;
    addReferencedElement(m_pValueType);
}

void Variable::setInitializationExpression(Expression* a_pExpression)
{
    m_pInitializationExpression = a_pExpression;
}

void Variable::getValue(void* a_pOutputBuffer) const
{
    m_pValueType->copyConstruct(a_pOutputBuffer, m_pAddress);
}

} // namespace lang
} // namespace phantom
