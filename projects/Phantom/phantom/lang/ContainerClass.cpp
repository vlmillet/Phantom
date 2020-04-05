// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ContainerClass.h"

#include "Method.h"
#include "Property.h"
#include "phantom/detail/new.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
ContainerClass::ContainerClass(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                               Modifiers a_Modifiers, uint a_uiFlags)
    : Class(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

ContainerClass::ContainerClass(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                               uint a_uiFlags /*= 0*/)
    : Class(a_eTypeKind, a_strName, a_Modifiers, a_uiFlags), m_Data(PHANTOM_NEW(RTData))
{
}

ContainerClass::~ContainerClass()
{
    PHANTOM_DELETE(RTData) m_Data;
}

void ContainerClass::setValueType(Type* a_pValueType)
{
    PHANTOM_ASSERT(a_pValueType, "container value type not reflected");
    m_pValueType = a_pValueType;
}

size_t ContainerClass::size(void const* a_pContainer) const
{
    if (!m_Data->m_pFunc_size)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_size = getMethod("size() const"));
    }
    PHANTOM_ASSERT(m_Data->m_pFunc_size);
    size_t result;
    m_Data->m_pFunc_size->invoke((void*)a_pContainer, nullptr, &result);
    return result;
}

void ContainerClass::clear(void* a_pContainer) const
{
    if (!m_Data->m_pFunc_clear)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_clear = getMethod("clear()"));
    }
    m_Data->m_pFunc_clear->invoke((void*)a_pContainer, nullptr);
}

void ContainerClass::insert(void* a_pContainer, void const* a_pIt, void const* a_pValue, void* a_pOutIt) const
{
    if (!m_Data->m_pFunc_insert)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_insert =
                       getMethod("insert", TypesView{getIteratorType(), getValueType()->addConstLValueReference()}));
    }
    void* args[2]{(void*)a_pIt, (void*)a_pValue};
    return m_Data->m_pFunc_insert->invoke((void*)a_pContainer, args, a_pOutIt);
}

void ContainerClass::eraseKey(void* a_pContainer, void const* a_pKey) const
{
    eraseAt(a_pContainer, *reinterpret_cast<size_t const*>(a_pKey));
}

void ContainerClass::begin(void* a_pContainer, void* a_pOutIt) const
{
    _initBegin();
    m_Data->m_pFunc_begin->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ContainerClass::begin(void const* a_pContainer, void* a_pOutIt) const
{
    _initBeginC();
    m_Data->m_pFunc_beginc->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ContainerClass::end(void* a_pContainer, void* a_pOutIt) const
{
    if (!m_Data->m_pFunc_end)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_end = getMethod("end()"));
    }
    m_Data->m_pFunc_end->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

void ContainerClass::end(void const* a_pContainer, void* a_pOutIt) const
{
    if (!m_Data->m_pFunc_endc)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_endc = getMethod("end() const"));
    }
    m_Data->m_pFunc_endc->invoke((void*)a_pContainer, (void**)nullptr, a_pOutIt);
}

Property* ContainerClass::getSizeProperty() const
{
    if (!m_pSizeProperty)
    {
        if ((m_pSizeProperty = createSizeProperty()))
            const_cast<ContainerClass*>(this)->addProperty(m_pSizeProperty);
    }
    if (!m_pSizeProperty)
    {
        PHANTOM_LOG(Error, "container has no size property");
    }
    return m_pSizeProperty;
}

void* ContainerClass::dereferenceIterator(void* a_pIt) const
{
    Type*   pIteratorType = getIteratorType();
    Method* pMethodDeref = nullptr;

    if (Class* pItClass = pIteratorType->asClass())
    {
        PHANTOM_VERIFY(pMethodDeref = pItClass->getMethodCascade("operator*()"));
    }
    else
    {
        PHANTOM_ASSERT(pIteratorType->removeAllQualifiers() == m_pValueType->removeAllQualifiers()->addPointer());
    }

    if (pMethodDeref)
        return pMethodDeref->invoke<void*>(a_pIt, nullptr);
    else
        return *(void**)a_pIt; // built in *pointer
}

void ContainerClass::advanceIterator(void* a_pIt, size_t a_N) const
{
    Type*   pIteratorType = getIteratorType();
    Method* pMethodInc = nullptr;

    if (Class* pItClass = pIteratorType->asClass())
    {
        PHANTOM_VERIFY(pMethodInc = pItClass->getMethodCascade("operator++()"));
    }
    else
    {
        PHANTOM_ASSERT(pIteratorType->removeAllQualifiers() == m_pValueType->removeAllQualifiers()->addPointer());
    }

    while (a_N--)
    {
        if (pMethodInc)
            pMethodInc->invoke(a_pIt, nullptr);
        else
            *(char**)a_pIt += m_pValueType->getSize(); // built in pointer ++
    }
}

bool ContainerClass::compareIterators(void* a_pIt1, void* a_pIt2) const
{
    Type*   pIteratorType = getIteratorType();
    Method* pMethodEq = nullptr;

    if (Class* pItClass = pIteratorType->asClass())
    {
        PHANTOM_VERIFY(pMethodEq = pItClass->getMethodCascade("operator==", pIteratorType->addConstLValueReference()));
    }
    else
    {
        PHANTOM_ASSERT(pIteratorType->removeAllQualifiers() == m_pValueType->removeAllQualifiers()->addPointer());
    }

    if (pMethodEq)
    {
        bool result = false;
        pMethodEq->invoke(a_pIt1, &a_pIt2, &result);
        return result;
    }
    else
    {
        return *(void**)a_pIt1 == *(void**)a_pIt2; // built in *pointer
    }
}

const void* ContainerClass::referenceAt(void const* a_pContainer, size_t a_uiIndex) const
{
    return (const void*)referenceAt((void*)a_pContainer, a_uiIndex);
}

void* ContainerClass::referenceAt(void* a_pContainer, size_t a_uiIndex) const
{
    PHANTOM_ASSERT(m_pValueType);
    Type* pIteratorType = getIteratorType();
    PHANTOM_ASSERT(pIteratorType);
    auto pIt = PHANTOM_LOCAL(pIteratorType);
    begin((void*)a_pContainer, pIt);
    advanceIterator(pIt, a_uiIndex);
    return dereferenceIterator(pIt);
}

void ContainerClass::erase(void* a_pContainer, void const* a_pIt) const
{
    if (!m_Data->m_pFunc_eraseAt)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_eraseAt = getMethod("erase", Types{getIteratorType()}));
    }
    m_Data->m_pFunc_eraseAt->invoke((void*)a_pContainer, (void**)&a_pIt);
}

void ContainerClass::eraseAt(void* a_pContainer, size_t a_uiIndex) const
{
    PHANTOM_ASSERT(m_pValueType);
    Type* pIteratorType = getIteratorType();
    PHANTOM_ASSERT(pIteratorType);
    auto pIt = PHANTOM_LOCAL(pIteratorType);
    begin((void*)a_pContainer, pIt);
    advanceIterator(pIt, a_uiIndex);
    erase(a_pContainer, pIt);
}

void ContainerClass::_initBegin() const
{
    if (!m_Data->m_pFunc_begin)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_begin = getMethod("begin()"));
    }
}

void ContainerClass::_initBeginC() const
{
    if (!m_Data->m_pFunc_beginc)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_beginc = getMethod("begin() const"));
    }
}

Type* ContainerClass::getIteratorType() const
{
    _initBegin();
    return m_Data->m_pFunc_begin->getReturnType();
}

Type* ContainerClass::getConstIteratorType() const
{
    _initBeginC();
    return m_Data->m_pFunc_beginc->getReturnType();
}

Property* ContainerClass::createSizeProperty() const
{
    Property* pProp = nullptr;
    auto      pGetMethod = getMethod("size() const");
    if (pGetMethod)
    {
        pProp = PHANTOM_NEW(Property)(PHANTOM_TYPEOF(size_t), "Size", PHANTOM_R_FILTER_PROPERTY, PHANTOM_R_NONE,
                                      isNative() * PHANTOM_R_FLAG_NATIVE);
        pProp->setGet(pGetMethod);
        if (auto pSetMethod = getMethod("resize(size_t)"))
            pProp->setSet(pSetMethod);
    }
    return pProp;
}

} // namespace lang
} // namespace phantom
