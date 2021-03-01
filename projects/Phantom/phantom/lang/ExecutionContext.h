// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/reflection.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallVector.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM ExecutionContext
{
public:
    static void              Push(ExecutionContext* a_pExecutionContext);
    static void              Pop();
    static ExecutionContext* Current();

    ExecutionContext() { m_resultPointers[0] = nullptr; }
    virtual ~ExecutionContext();
    enum
    {
        e_max_calls = 1024
    };
    void* resultPointer() { return m_resultPointers[m_resultPointersIndex]; }
    void  pushResultPointer(void* a_pAddress)
    {
        PHANTOM_ASSERT(m_resultPointersIndex < e_max_calls - 1);
        m_resultPointers[++m_resultPointersIndex] = (a_pAddress);
    }
    void popResultPointer()
    {
        PHANTOM_ASSERT(m_resultPointersIndex != ~size_t(0));
        --m_resultPointersIndex;
    }
    void pushTempDestruction(Evaluable* a_pScope, Type* a_pType, void* a_pBuffer);
    void releaseTemporaries(Evaluable* a_pScope);
    void releaseTemporaries();

    // must be reimplemented
    virtual bool  call(Subroutine* a_pSubroutine, void** a_ppArgs, size_t a_uiCount) = 0;
    virtual void  setNextStatement(Statement* a_pStatement) = 0;
    virtual byte* getStackPointer() const = 0;
    virtual byte* getBasePointer() const = 0;
    virtual void* getReturnAddress() const = 0;
    virtual byte* addressOfThis() const = 0;
    virtual byte* addressOf(LocalVariable* a_pLocalVariable) const = 0;

private:
    void*  m_resultPointers[e_max_calls];
    size_t m_resultPointersIndex = 0;

    SmallMap<Evaluable*, SmallVector<std::pair<Type*, void*> >, 20> m_Temporaries;
};
} // namespace lang
} // namespace phantom
