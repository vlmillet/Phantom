// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "FunctionPointer.h"

#include "Function.h"
#include "FunctionType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
FunctionPointer::FunctionPointer(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Modifiers /*= 0*/,
                                 uint a_uiFlags /*= 0*/)
    : PointerType(a_pFunctionType, "", sizeof(void (*)()), std::alignment_of<void (*)()>::value, a_Modifiers,
                  a_uiFlags | PHANTOM_R_FLAG_IMPLICIT),
      m_eABI(a_eABI)
{
}

FunctionPointer::~FunctionPointer()
{
}

void FunctionPointer::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
}

#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "cleanup these asserts")
#endif

void FunctionPointer::valueToLiteral(StringBuffer&, const void*) const
{
    PHANTOM_ASSERT(false, "valueToLiteral not defined for member function pointer types");
}

void FunctionPointer::valueToString(StringBuffer&, const void*) const
{
    PHANTOM_ASSERT(false, "valueToString not defined for member function pointer types");
}

void FunctionPointer::valueFromString(StringView, void*) const
{
    PHANTOM_ASSERT(false, "valueFromString not defined for member function pointer types");
}

/// Non native function pointers contains the meta function they point to

void FunctionPointer::call(void* a_pPointer, void** a_pArgs) const
{
    static_cast<Function*>(a_pPointer)->call(a_pArgs);
}

void FunctionPointer::call(void* a_pPointer, void** a_pArgs, void* a_pReturnAddress) const
{
    static_cast<Function*>(a_pPointer)->call(a_pArgs, a_pReturnAddress);
}

Closure FunctionPointer::getClosure(void* a_pPointer) const
{
    return static_cast<Function*>(a_pPointer)->getClosure();
}

FunctionPointer* FunctionPointer::Create(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Modifiers, uint a_Flags)
{
    return PHANTOM_DEFERRED_NEW_EX(FunctionPointer)(a_pFunctionType, a_eABI, a_Modifiers, a_Flags);
}

void FunctionPointer::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getQualifiedDecoratedName(a_Buf);
    a_Buf += "(*)(";
    for (auto it = pFunctionType->beginParameterTypes(); it != pFunctionType->endParameterTypes(); ++it)
    {
        if (it != pFunctionType->beginParameterTypes())
            a_Buf += ',';
        (*it)->getQualifiedDecoratedName(a_Buf);
    }
    a_Buf += ')';
}

void FunctionPointer::getQualifiedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getQualifiedName(a_Buf);
    a_Buf += "(*)(";
    for (auto it = pFunctionType->beginParameterTypes(); it != pFunctionType->endParameterTypes(); ++it)
    {
        if (it != pFunctionType->beginParameterTypes())
            a_Buf += ',';
        (*it)->getQualifiedName(a_Buf);
    }
    a_Buf += ')';
}

void FunctionPointer::getDecoratedName(StringBuffer& a_Buf) const
{
    FunctionType* pFunctionType = getFunctionType();
    pFunctionType->getReturnType()->getDecoratedName(a_Buf);
    a_Buf += "(*)(";
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
