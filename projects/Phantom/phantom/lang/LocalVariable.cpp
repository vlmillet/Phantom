// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "LocalVariable.h"

#include "Method.h"
#include "Signature.h"
#include "Type.h"
#include "phantom/detail/new.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
LocalVariable::LocalVariable(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_pValueType(a_pValueType)
{
    PHANTOM_ASSERT(m_pValueType);
    if (m_pValueType->getOwner() == nullptr && !(m_pValueType->isNative()))
        addElement(m_pValueType);
    else
        addReferencedElement(m_pValueType);
    if (m_pValueType->isTemplateDependant())
        setTemplateDependant();
}

Block* LocalVariable::getBlock() const
{
    Block* pBlock = getOwner()->asBlock();
    if (pBlock)
        return pBlock;
    Signature* pSignature = getOwner()->asSignature();
    if (pSignature)
        return pSignature->getOwnerSubroutine()->getBlock();
    Subroutine* pSubroutine = getOwner()->asSubroutine();
    if (pSubroutine)
        return pSubroutine->getBlock();
    return nullptr;
}

Subroutine* LocalVariable::getSubroutine() const
{
    return getEnclosingSubroutine();
}

void LocalVariable::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement == m_pValueType)
    {
        m_pValueType = nullptr;
    }
}

void LocalVariable::setValueType(Type* a_pType)
{
    // TODO : check this assert
    // PHANTOM_ASSERT(getBlock() == nullptr ||
    // getBlock()->getLocalVariable(getBlock()->getLocalVariableCount() - 1) == this, "cannot change
    // value type of a local variable if any was added in the block after");
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(a_pType);
    PHANTOM_ASSERT(m_pValueType);
    if (getBlock())
    {
        size_t& frameSize = getSubroutine()->m_uiFrameSize;
        frameSize += ptrdiff_t(a_pType->getSize()) - ptrdiff_t(m_pValueType->getSize());
    }
    removeReferencedElement(m_pValueType);
    m_pValueType = a_pType;
    addReferencedElement(m_pValueType);
}

LocalVariable* LocalVariable::clone() const
{
    LocalVariable* pLV = cloneImpl();
    pLV->setCodeRange(getCodeRange());
    return pLV;
}

LocalVariable* LocalVariable::cloneImpl() const
{
    return PHANTOM_NEW(LocalVariable)(m_pValueType, m_strName, m_Modifiers);
}

void LocalVariable::getQualifiedName(StringBuffer& a_Buf) const
{
    return getQualifiedDecoratedName(a_Buf);
}

void LocalVariable::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return getName(a_Buf);
}

bool LocalVariable::isThis() const
{
    Method*            pMethod = getSubroutine()->asMethod();
    return pMethod && pMethod->getThis() == this;
}

} // namespace lang
} // namespace phantom
