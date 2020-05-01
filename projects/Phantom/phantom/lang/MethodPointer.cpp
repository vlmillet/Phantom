// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "MethodPointer.h"

#include "ConstType.h"
#include "FunctionType.h"
#include "LValueReference.h"
#include "Method.h"
#include "RValueReference.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MethodPointer::MethodPointer(ClassType* a_pObjectType, FunctionType* a_pFunctionType, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
    : MemberPointer(TypeKind::MethodPointer, a_pObjectType, sizeof(Method*), std::alignment_of<Method*>::value,
                    a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_IMPLICIT),
      m_pFunctionType(a_pFunctionType)
{
    PHANTOM_ASSERT(!isNative());
    addReferencedElement(m_pFunctionType);
    if (m_pFunctionType->isTemplateDependant())
        setTemplateDependant();
}

MethodPointer::MethodPointer(ClassType* a_pObjectType, FunctionType* a_pFunctionType, size_t a_Size, size_t a_Alignment,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : MemberPointer(TypeKind::MethodPointer, a_pObjectType, a_Size, a_Alignment, a_Modifiers,
                    a_uiFlags | PHANTOM_R_FLAG_IMPLICIT),
      m_pFunctionType(a_pFunctionType)
{
    PHANTOM_ASSERT(isNative());
    addReferencedElement(m_pFunctionType);
    if (m_pFunctionType->isTemplateDependant())
        setTemplateDependant();
}

void MethodPointer::valueToLiteral(StringBuffer& a_Buf, const void* src) const
{
    MemberPointer::valueToLiteral(a_Buf, src);
}

void MethodPointer::valueToString(StringBuffer& a_Buf, const void* src) const
{
    MemberPointer::valueToString(a_Buf, src);
}

void MethodPointer::valueFromString(StringView a_str, void* dest) const
{
    MemberPointer::valueFromString(a_str, dest);
}

Closure MethodPointer::getClosure(void* a_pPointer) const
{
    return static_cast<Method*>(a_pPointer)->getClosure();
}

void MethodPointer::call(void* a_pPointer, void* a_pThis, void** a_pArgs) const
{
    PHANTOM_ASSERT(a_pPointer);
    static_cast<Method*>(a_pPointer)->invoke(a_pThis, a_pArgs);
}

void MethodPointer::call(void* a_pPointer, void* a_pThis, void** a_pArgs, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(a_pPointer);
    static_cast<Method*>(a_pPointer)->invoke(a_pThis, a_pArgs, a_pReturnAddress);
}

bool MethodPointer::matches(TypesView parameters, Modifiers a_Modifiers, uint a_uiFlags) const
{
    return m_pFunctionType->matches(parameters, a_Modifiers, a_uiFlags);
}

Type* MethodPointer::getReturnType() const
{
    return m_pFunctionType->getReturnType();
}

Type* MethodPointer::getParameterType(size_t i) const
{
    return m_pFunctionType->getParameterType(i);
}

size_t MethodPointer::getParameterTypeCount() const
{
    return m_pFunctionType->getParameterTypeCount();
}

TypesView MethodPointer::getParameterTypes() const
{
    return m_pFunctionType->getParameterTypes();
}

bool MethodPointer::acceptsCallerExpressionType(Type* a_pType) const
{
    Modifiers qualifiers = 0;
    a_pType = a_pType->removeQualifiers();
    if (a_pType->asLValueReference())
    {
        qualifiers |= PHANTOM_R_LVALUEREF;
        a_pType = static_cast<LValueReference*>(a_pType);
    }
    else
    {
        qualifiers |= PHANTOM_R_RVALUEREF;
        a_pType = static_cast<RValueReference*>(a_pType);
    }
    if (a_pType->asConstType())
    {
        qualifiers |= PHANTOM_R_CONST;
    }
    return acceptsCallerExpressionQualifiers(qualifiers);
}

bool MethodPointer::acceptsCallerExpressionQualifiers(Modifiers a_CallerQualifiers) const
{
    PHANTOM_ASSERT((a_CallerQualifiers & (~(PHANTOM_R_REFQUAL_MASK | PHANTOM_R_CONST))) == 0);
    PHANTOM_ASSERT(((a_CallerQualifiers & PHANTOM_R_LVALUEREF) == PHANTOM_R_LVALUEREF) ^
                   ((a_CallerQualifiers & PHANTOM_R_RVALUEREF) == PHANTOM_R_RVALUEREF));
    return (((a_CallerQualifiers & PHANTOM_R_CONST) == PHANTOM_R_CONST) <=
            ((m_Modifiers & PHANTOM_R_CONST) ==
             PHANTOM_R_CONST)) // caller must be equally or less const qualified than member
                               // function (every one can call a const member function but a
                               // const cannot call a non const member function)
    && (((m_Modifiers & (PHANTOM_R_REFQUAL_MASK)) == 0) ||
        (m_Modifiers & PHANTOM_R_REFQUAL_MASK) == (a_CallerQualifiers & PHANTOM_R_REFQUAL_MASK));
}

Type* MethodPointer::getImplicitObjectParameterType() const
{
    Type* pImplicitObjectParameterType = getOwner()->asClassType();
    if (m_Modifiers & PHANTOM_R_CONST)
    {
        pImplicitObjectParameterType = pImplicitObjectParameterType->makeConst();
    }
    if (m_Modifiers & PHANTOM_R_RVALUEREF)
    {
        return pImplicitObjectParameterType->makeRValueReference();
    }
    return pImplicitObjectParameterType->makeLValueReference();
}

void MethodPointer::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getQualifiedDecoratedName(a_Buf);
    a_Buf += '(';
    m_pObjectType->getQualifiedDecoratedName(a_Buf);
    a_Buf += "::*)(";
    for (auto it = pFunctionType->beginParameterTypes(); it != pFunctionType->endParameterTypes(); ++it)
    {
        if (it != pFunctionType->beginParameterTypes())
            a_Buf += ',';
        (*it)->getQualifiedDecoratedName(a_Buf);
    }
    a_Buf += ')';
}

void MethodPointer::getQualifiedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getQualifiedName(a_Buf);
    a_Buf += '(';
    m_pObjectType->getQualifiedName(a_Buf);
    a_Buf += "::*)(";
    for (auto it = pFunctionType->beginParameterTypes(); it != pFunctionType->endParameterTypes(); ++it)
    {
        if (it != pFunctionType->beginParameterTypes())
            a_Buf += ',';
        (*it)->getQualifiedName(a_Buf);
    }
    a_Buf += ')';
}

void MethodPointer::getDecoratedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getDecoratedName(a_Buf);
    a_Buf += '(';
    m_pObjectType->getDecoratedName(a_Buf);
    a_Buf += "::*)(";
    for (auto it = pFunctionType->beginParameterTypes(); it != pFunctionType->endParameterTypes(); ++it)
    {
        if (it != pFunctionType->beginParameterTypes())
            a_Buf += ',';
        (*it)->getDecoratedName(a_Buf);
    }
    a_Buf += ')';
}

} // namespace lang
} // namespace phantom
