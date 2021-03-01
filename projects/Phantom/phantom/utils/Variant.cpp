// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Variant.h"
/* *********************************************** */
namespace phantom
{
static Variant::TypeConverter s_VariantTypeConverter;

void Variant::SetTypeConverter(TypeConverter a_TC)
{
    s_VariantTypeConverter = a_TC;
}

phantom::Variant::TypeConverter Variant::GetTypeConverter()
{
    return s_VariantTypeConverter;
}

Variant::Variant(lang::Type* a_pType, void const* a_pValue)
{
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = _Alloc(a_pType->getSize(), a_pType->getAlignment()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->copyConstruct(pBuffer, a_pValue);
}

Variant::Variant(lang::Type* a_pType, void* a_pValue, bool a_bMove)
{
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = _Alloc(a_pType->getSize(), a_pType->getAlignment()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    if (a_bMove)
    {
        a_pType->moveConstruct(pBuffer, a_pValue);
    }
    else
    {
        a_pType->copyConstruct(pBuffer, a_pValue);
    }
}

void Variant::copyAssign(const void* a_pValue)
{
    PHANTOM_ASSERT(m_pType);
    m_pType->copyAssign(data(), a_pValue);
}

void Variant::moveAssign(void* a_pValue)
{
    PHANTOM_ASSERT(m_pType);
    m_pType->moveAssign(data(), a_pValue);
}

void Variant::copyConstruct(lang::Type* a_pType, const void* a_pValue)
{
    if (m_pType)
    {
        if (m_pType == a_pType)
            return copyAssign(a_pValue);
        _release();
    }
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = _Alloc(a_pType->getSize(), a_pType->getAlignment()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->copyConstruct(pBuffer, a_pValue);
}

void Variant::moveConstruct(lang::Type* a_pType, void* a_pValue)
{
    if (m_pType)
    {
        if (m_pType == a_pType)
            return moveAssign(a_pValue);
        _release();
    }
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = _Alloc(a_pType->getSize(), a_pType->getAlignment()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->moveConstruct(pBuffer, a_pValue);
}

const Variant Variant::null;

} // namespace phantom
