// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/VirtualDelete.h>
#include <phantom/detail/core.h>
#include <phantom/utils/SmallVector.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
class PHANTOM_EXPORT_PHANTOM ExecutionContext
{
public:
    PHANTOM_DECL_ABSTRACT_DELETE_METHOD(ExecutionContext);

    static void              Push(ExecutionContext* a_pExecutionContext);
    static void              Pop();
    static ExecutionContext* Current();

    ExecutionContext()
    {
        m_resultPointers[0] = nullptr;
    }
    ~ExecutionContext();
    enum
    {
        e_max_calls = 1024
    };
    void* resultPointer()
    {
        return m_resultPointers[m_resultPointersIndex];
    }
    void pushResultPointer(void* a_pAddress)
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
    struct TempDestruction
    {
        Evaluable*                            scope;
        SmallVector<std::pair<Type*, void*> > buffers;
    };
    SmallVector<TempDestruction, 20> m_Temporaries;
};
} // namespace reflection
} // namespace phantom
