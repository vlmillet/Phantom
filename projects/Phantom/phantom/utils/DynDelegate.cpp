// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "DynDelegate.h"

#include <phantom/reflection/Function.h>
#include <phantom/reflection/Method.h>
#include <phantom/reflection/Signature.h>
/* *********************************************** */
namespace phantom
{
OpaqueDynDelegate::OpaqueDynDelegate(void* a_pInstance, reflection::Class* a_pClass, reflection::Method* a_pMethod)
    : m_pMethod(a_pMethod)
{
    m_pThis = a_pClass->cast(a_pMethod->getOwnerClass(), a_pInstance);
    PHANTOM_ASSERT(m_pThis, "given object is an instance of the given method class");
}

OpaqueDynDelegate::OpaqueDynDelegate(reflection::Function* a_pFunction) : m_pFunction(a_pFunction)
{
}

OpaqueDynDelegate::OpaqueDynDelegate(void* a_pInstance, reflection::Class* a_pClass, StringView a_MethodName)
{
    reflection::Method* pMethod = a_pClass->getMethodCascade(a_MethodName);
    PHANTOM_ASSERT(pMethod, "no method '%.*s' found in class '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_MethodName),
                   PHANTOM_STRING_AS_PRINTF_ARG(a_pClass->getName()));
    m_pThis = a_pClass->cast(pMethod->getOwnerClass(), a_pInstance);
    m_pMethod = pMethod;
}

phantom::reflection::Subroutine* OpaqueDynDelegate::getSubroutine() const
{
    return m_pMethod ? static_cast<reflection::Subroutine*>(m_pMethod) : m_pFunction;
}

void OpaqueDynDelegate::call(void** a_pArgs) const
{
    if (m_pMethod)
        m_pMethod->invoke(m_pThis, a_pArgs);
    else
        m_pFunction->call(a_pArgs);
}

void OpaqueDynDelegate::call(void** a_pArgs, void* a_pReturnAddress) const
{
    if (m_pMethod)
        m_pMethod->invoke(m_pThis, a_pArgs, a_pReturnAddress);
    else
        m_pFunction->call(a_pArgs, a_pReturnAddress);
}

bool OpaqueDynDelegate::_CheckSignature(reflection::Type* a_pRetType, reflection::TypesView a_Types) const
{
    reflection::Signature* pSign = getSubroutine()->getSignature();
    if (pSign->getReturnType() != a_pRetType)
        return false;
    if (pSign->getParameterCount() != a_Types.size())
        return false;
    for (size_t i = 0; i < a_Types.size(); ++i)
    {
        reflection::Type* pArgT = a_Types[i];
        reflection::Type* pParamT = pSign->getParameterType(i);
        if (pArgT !=
            pSign->getParameterType(i) AND((pArgT->asPointer() == nullptr) OR(
            pParamT != PHANTOM_TYPEOF(void*) && pArgT != PHANTOM_TYPEOF(void*)))) // X* -> void* || void* -> X*
            return false;
    }
    return true;
}

} // namespace phantom
