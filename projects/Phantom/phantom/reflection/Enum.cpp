// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Enum.h"

#include "Constant.h"

#include <phantom/alignof>
#include <phantom/utils/random.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
namespace
{
String GenerateAnonymousName()
{
    char buffer[64];
    phantom::random::str(buffer, "enum_%%%%%%%%_%%%%_%%%%_%%%%%%%%%%%%");
    return buffer;
}

} // namespace

Enum::Enum()
    : PrimitiveType(TypeKind::Enum, GenerateAnonymousName(), sizeof(int), PHANTOM_ALIGNOF(int)), m_pUnderlyingIntType(IntType())
{
}

Enum::Enum(PrimitiveType* a_pIntType)
    : PrimitiveType(TypeKind::Enum, GenerateAnonymousName(), a_pIntType->getSize(), a_pIntType->getAlignment()),
      m_pUnderlyingIntType(a_pIntType)
{
}

Enum::Enum(StringView a_strName, PrimitiveType* a_pIntType)
    : PrimitiveType(TypeKind::Enum, a_strName.size() ? a_strName : StringView(GenerateAnonymousName()),
                    a_pIntType->getSize(), a_pIntType->getAlignment()),
      m_pUnderlyingIntType(a_pIntType)
{
}

Enum::Enum(StringView a_strName)
    : PrimitiveType(TypeKind::Enum, a_strName.size() ? a_strName : StringView(GenerateAnonymousName()), sizeof(int),
        PHANTOM_ALIGNOF(int)),
      m_pUnderlyingIntType(IntType())
{
}

// for native
Enum::Enum(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers /*= 0*/,
           uint a_uiFlags /*= 0*/)
    : PrimitiveType(TypeKind::Enum, a_strName.size() ? a_strName : StringView(GenerateAnonymousName()), a_uiSize,
                    a_uiAlignment, a_Modifiers, a_uiFlags),
      m_pUnderlyingIntType(IntType())
{
    if (m_pUnderlyingIntType == nullptr)
    {
        switch (a_uiSize)
        {
        case 8:
            m_pUnderlyingIntType = (PrimitiveType*)PHANTOM_TYPEOF(long long);
            break;
        default:
            m_pUnderlyingIntType = (PrimitiveType*)PHANTOM_TYPEOF(int);
        }
    }
    addReferencedElement(m_pUnderlyingIntType);
}

Enum::~Enum()
{
}

void Enum::addConstant(Constant* a_pConstant)
{
    PHANTOM_ASSERT(getConstant(a_pConstant->getName()) == nullptr);
    PHANTOM_ASSERT(a_pConstant->getValueType() == this OR m_strName == "");
    if (m_pUnderlyingIntType->getSize() < a_pConstant->getValueType()->getSize())
    {
        PHANTOM_ASSERT(m_strName == "");
        removeReferencedElement(m_pUnderlyingIntType);
        PHANTOM_ASSERT(getSize() == sizeof(int));
        PHANTOM_ASSERT(a_pConstant->getValueType()->getSize() == sizeof(long long));
        m_pUnderlyingIntType = (PrimitiveType*)PHANTOM_TYPEOF(long long);
        m_uiSize = sizeof(long long);
        m_uiAlignment = PHANTOM_ALIGNOF(long long);
        addReferencedElement(m_pUnderlyingIntType);
    }
    PHANTOM_ASSERT(a_pConstant->getValueType()->getSize() == m_pUnderlyingIntType->getSize());
    addElement(a_pConstant);
    m_Constants.push_back(a_pConstant);
}

void Enum::addConstant(StringView a_strCode)
{
    PHANTOM_ASSERT(!isNative());
    if (a_strCode.front() == ' ')
    {
        addConstant(a_strCode.substr(1));
        return;
    }
    size_t pos = a_strCode.find_first_of("=");
    if (pos != String::npos)
    {
        String name = a_strCode.substr(0, pos);
        while (name.back() == ' ')
            name.pop_back();
        String valuestr = a_strCode.substr(pos + 1);
        while (valuestr.front() == ' ')
            valuestr = valuestr.substr(1);
        while (valuestr.back() == ' ')
            valuestr.pop_back();
        ulonglong value = lexical_cast<ulonglong>(valuestr);
        addConstant(m_pUnderlyingIntType->createConstant(&value, name, this));
    }
    else
    {
        String name = a_strCode;
        while (name.back() == ' ')
            name.pop_back();
        ulonglong value = 0;
        m_Constants.back()->getValue(&value);
        value++;
        addConstant(m_pUnderlyingIntType->createConstant(&value, name, this));
    }
}

void Enum::addConstant(StringView a_Name, int a_Value)
{
    addConstant(Constant::Create<int>(a_Value, a_Name, this));
}

void Enum::addConstants(StringView, ArrayView<Pair<StringView, int>> a_Values)
{
    for (auto const& val : a_Values)
    {
        addConstant(val.first, val.second);
    }
}

void Enum::removeConstant(Constant* a_pConstant)
{
    PHANTOM_ASSERT(getConstant(a_pConstant->getName()) != nullptr);
    PHANTOM_ASSERT(a_pConstant->getValueType() == this);
    removeElement(a_pConstant);
}

Constant* Enum::getConstant(StringView a_strKey) const
{
    for (auto pCons : m_Constants)
    {
        if (pCons->getName() == a_strKey)
            return pCons;
    }
    return nullptr;
}

void Enum::getConstants(const void* a_pValue, Constants& a_Constants) const
{
    for (auto c : m_Constants)
    {
        if (c->hasValue(a_pValue))
            a_Constants.push_back(c);
    }
}

Constant* Enum::getFirstConstant(const void* a_pValue) const
{
    for (auto c : m_Constants)
    {
        if (c->hasValue(a_pValue))
            return c;
    }
    return nullptr;
}

Constant* Enum::getFirstConstant(longlong a_Value) const
{
    for (auto c : m_Constants)
    {
        if (c->hasValue(&a_Value))
            return c;
    }
    return nullptr;
}

void Enum::getDefaultValue(void* a_pBuffer) const
{
    if (m_Constants.size())
    {
        m_Constants.front()->getValue(a_pBuffer);
    }
    else
    {
        memset(a_pBuffer, 0, getSize());
    }
}

void Enum::valueFromString(StringView a_strIn, void* a_pDest) const
{
    size_t i = 0;
    size_t count = getConstantCount();
    for (; i < count; ++i)
    {
        reflection::Constant* pConstant = getConstant(i);
        if (pConstant->getName() == a_strIn)
        {
            pConstant->getValue(a_pDest);
            return;
        }
    }
}

void Enum::valueToString(StringBuffer& a_Buf, const void* a_pSrc) const
{
    size_t i = 0;
    size_t count = getConstantCount();
    for (; i < count; ++i)
    {
        size_t                constantValue = 0;
        reflection::Constant* pConstant = getConstant(i);
        pConstant->getValue(&constantValue);
        if (constantValue == *((size_t*)a_pSrc))
        {
            return pConstant->getQualifiedDecoratedName(a_Buf);
        }
    }
}

void Enum::onElementRemoved(LanguageElement* a_pElement)
{
    PrimitiveType::onElementRemoved(a_pElement);
    Constant* pConstant = a_pElement->asConstant();
    if (pConstant)
    {
        m_Constants.erase(std::find(m_Constants.begin(), m_Constants.end(), pConstant));
    }
}

void Enum::findConstantsWithValue(void* a_pSrc, Constants& out) const
{
    for (auto it = m_Constants.begin(); it != m_Constants.end(); ++it)
    {
        if ((*it)->hasValue(a_pSrc))
            out.push_back(*it);
    }
}

bool Enum::convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const
{
    return getUnderlyingType()->convert(a_pDstType, a_pDst, a_pSrc);
}

void Enum::onAncestorChanged(LanguageElement* a_pOwner)
{
    Type::onAncestorChanged(a_pOwner);
}

PrimitiveType* Enum::IntType()
{
    return (PrimitiveType*)PHANTOM_TYPEOF(int);
}

Constant* Enum::createConstant(void* a_pSrc, StringView a_strName /*= "" */,
                               PrimitiveType* a_pPrimitiveType /*= nullptr*/) const
{
    return m_pUnderlyingIntType->createConstant(a_pSrc, a_strName, a_pPrimitiveType ? a_pPrimitiveType : (Enum*)this);
}

} // namespace reflection
} // namespace phantom
