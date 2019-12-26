// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
/* ** The Class Header must be the last #include * */
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

Variant::Variant(reflection::Type* a_pType, void const* a_pValue)
{
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->copyConstruct(pBuffer, a_pValue);
}

Variant::Variant(reflection::Type* a_pType, void* a_pValue, bool a_bMove)
{
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize()))
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

void Variant::copyConstruct(reflection::Type* a_pType, const void* a_pValue)
{
    if (m_pType)
    {
        if (m_pType == a_pType)
            return copyAssign(a_pValue);
        _release();
    }
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->copyConstruct(pBuffer, a_pValue);
}

void Variant::moveConstruct(reflection::Type* a_pType, void* a_pValue)
{
    if (m_pType)
    {
        if (m_pType == a_pType)
            return moveAssign(a_pValue);
        _release();
    }
    byte* pBuffer = (a_pType->getSize() > StaticBufferSize)
    ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize()))
    : m_Buffer.staticBuffer;
    m_pType = a_pType;
    a_pType->moveConstruct(pBuffer, a_pValue);
}

const Variant Variant::null;

} // namespace phantom
