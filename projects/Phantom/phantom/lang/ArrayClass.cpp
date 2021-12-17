// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ArrayClass.h"

#include "Method.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
ArrayClass::ArrayClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                       uint a_uiFlags)
    : ArrayClass(TypeKind::ArrayClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

ArrayClass::ArrayClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ArrayClass(TypeKind::ArrayClass, a_strName, a_Modifiers, a_uiFlags)
{
}

ArrayClass::ArrayClass(TypeKind a_TypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                       Modifiers a_Modifiers, uint a_uiFlags)
    : Class(a_TypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

ArrayClass::ArrayClass(TypeKind a_TypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Class(a_TypeKind, a_strName, a_Modifiers, a_uiFlags)
{
}

void ArrayClass::initialize()
{
    Class::initialize();
    if (!isNative())
        m_pData = new_<RTData>();
}

void ArrayClass::terminate()
{
    if (m_pData)
        delete_<RTData>(m_pData);
    Class::terminate();
}

void const* ArrayClass::data(void const* a_pContainer) const
{
    if (!m_pData->m_pFunc_datac)
        m_pData->m_pFunc_datac = getMethod("data() const");
    PHANTOM_ASSERT(m_pData->m_pFunc_datac);
    void* ptr;
    m_pData->m_pFunc_datac->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void* ArrayClass::data(void* a_pContainer) const
{
    if (!m_pData->m_pFunc_data)
        m_pData->m_pFunc_data = getMethod("data()");
    PHANTOM_ASSERT(m_pData->m_pFunc_data);
    void* ptr;
    m_pData->m_pFunc_data->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

size_t ArrayClass::size(void const* a_pContainer) const
{
    if (!m_pData->m_pFunc_size)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_size = getMethod("size() const"));
    }
    PHANTOM_ASSERT(m_pData->m_pFunc_size);
    size_t result;
    m_pData->m_pFunc_size->invoke((void*)a_pContainer, nullptr, &result);
    return result;
}

void ArrayClass::_initBegin() const
{
    if (!m_pData->m_pFunc_begin)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_begin = getMethod("begin()"));
    }
}

void ArrayClass::_initBeginC() const
{
    if (!m_pData->m_pFunc_beginc)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_beginc = getMethod("begin() const"));
    }
}

Type* ArrayClass::getIteratorType() const
{
    _initBegin();
    return m_pData->m_pFunc_begin->getReturnType();
}

Type* ArrayClass::getConstIteratorType() const
{
    _initBeginC();
    return m_pData->m_pFunc_beginc->getReturnType();
}

void ArrayClass::begin(void* a_pContainer, void* a_pOutIt) const
{
    _initBegin();
    m_pData->m_pFunc_begin->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ArrayClass::begin(void const* a_pContainer, void* a_pOutIt) const
{
    _initBeginC();
    m_pData->m_pFunc_beginc->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ArrayClass::end(void* a_pContainer, void* a_pOutIt) const
{
    if (!m_pData->m_pFunc_end)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_end = getMethod("end()"));
    }
    m_pData->m_pFunc_end->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ArrayClass::end(void const* a_pContainer, void* a_pOutIt) const
{
    if (!m_pData->m_pFunc_endc)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_endc = getMethod("end() const"));
    }
    m_pData->m_pFunc_endc->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void const* ArrayClass::referenceAt(void const* a_pContainer, size_t a_uiIndex) const
{
    if (!m_pData->m_pFunc_refAtC)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_refAtC = getMethod("operator[]", {PHANTOM_TYPEOF(size_t)}, Modifier::Const));
    }
    void* args[] = {&a_uiIndex};
    void* ref;
    m_pData->m_pFunc_refAtC->invoke((void*)a_pContainer, args, &ref);
    return ref;
}

void* ArrayClass::referenceAt(void* a_pContainer, size_t a_uiIndex) const
{
    if (!m_pData->m_pFunc_refAt)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_refAt = getMethod("operator[]", {PHANTOM_TYPEOF(size_t)}));
    }
    void* args[] = {&a_uiIndex};
    void* ref;
    m_pData->m_pFunc_refAt->invoke(a_pContainer, args, &ref);
    return ref;
}

void* ArrayClass::dereferenceIterator(void* a_pIt) const
{
    return *(void**)a_pIt; // built in *pointer
}

void ArrayClass::advanceIterator(void* a_pIt, size_t a_N) const
{
    *(char**)a_pIt += a_N * m_pItemType->getSize(); // built in pointer ++
}

bool ArrayClass::compareIterators(void* a_pIt1, void* a_pIt2) const
{
    return *(void**)a_pIt1 == *(void**)a_pIt2; // built in *pointer
}

} // namespace lang
} // namespace phantom
