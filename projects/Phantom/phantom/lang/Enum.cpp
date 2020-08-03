// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Enum.h"

#include "Constant.h"

#include <phantom/alignof>
#include <phantom/utils/random.h>
/* *********************************************** */
namespace phantom
{
namespace lang
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

Enum::Enum() : PrimitiveType(TypeKind::Enum, IntType(), GenerateAnonymousName(), sizeof(int), PHANTOM_ALIGNOF(int)) {}

Enum::Enum(PrimitiveType* a_pIntType)
    : PrimitiveType(TypeKind::Enum, a_pIntType, GenerateAnonymousName(), a_pIntType->getSize(),
                    a_pIntType->getAlignment())
{
}

Enum::Enum(StringView a_strName, PrimitiveType* a_pIntType)
    : PrimitiveType(TypeKind::Enum, a_pIntType, a_strName.size() ? a_strName : StringView(GenerateAnonymousName()),
                    a_pIntType->getSize(), a_pIntType->getAlignment())
{
}

Enum::Enum(StringView a_strName)
    : PrimitiveType(TypeKind::Enum, IntType(), a_strName.size() ? a_strName : StringView(GenerateAnonymousName()),
                    sizeof(int), PHANTOM_ALIGNOF(int))
{
}

// for native
Enum::Enum(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers /*= 0*/,
           uint a_uiFlags /*= 0*/)
    : PrimitiveType(TypeKind::Enum, IntType(), a_strName.size() ? a_strName : StringView(GenerateAnonymousName()),
                    a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

Enum::~Enum() {}

void Enum::addConstant(Constant* a_pConstant)
{
    auto pUT = getUnderlyingType();
    PHANTOM_ASSERT(getConstant(a_pConstant->getName()) == nullptr);
    PHANTOM_ASSERT(a_pConstant->getValueType() == this || getName() == "");
    if (pUT->getSize() < a_pConstant->getValueType()->getSize())
    {
        PHANTOM_ASSERT(getName() == "");
        removeReferencedElement(pUT);
        PHANTOM_ASSERT(getSize() == sizeof(int));
        PHANTOM_ASSERT(a_pConstant->getValueType()->getSize() == sizeof(long long));
        pUT = (PrimitiveType*)PHANTOM_TYPEOF(long long);
        setSizeAndAlignemnt(sizeof(long long), PHANTOM_ALIGNOF(long long));
        addReferencedElement(pUT);
    }
    PHANTOM_ASSERT(a_pConstant->getValueType()->getSize() == getUnderlyingIntType()->getSize());
    a_pConstant->setOwner(this);
    a_pConstant->setVisibility(Visibility::Public);
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
        addConstant(getUnderlyingIntType()->createConstant(&value, name, this));
    }
    else
    {
        String name = a_strCode;
        while (name.back() == ' ')
            name.pop_back();
        ulonglong value = 0;
        m_Constants.back()->getValue(&value);
        value++;
        addConstant(getUnderlyingIntType()->createConstant(&value, name, this));
    }
}

void Enum::addConstant(StringView a_Name, int a_Value)
{
    addConstant(Constant::Create<int>(this, a_Value, a_Name, this));
}

void Enum::addConstants(StringView, ArrayView<Pair<StringView, int>> a_Values)
{
    for (auto const& val : a_Values)
    {
        addConstant(val.first, val.second);
    }
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
        lang::Constant* pConstant = getConstant(i);
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
        long long       constantValue = 0;
        lang::Constant* pConstant = getConstant(i);
        pConstant->getValue(&constantValue);
        switch (getUnderlyingIntType()->getSize())
        {
        case 1:
            if (constantValue == *((char*)a_pSrc))
                return pConstant->getQualifiedDecoratedName(a_Buf);
            break;
        case 2:
            if (constantValue == *((short*)a_pSrc))
                return pConstant->getQualifiedDecoratedName(a_Buf);
            break;
        case 4:
            if (constantValue == *((int*)a_pSrc))
                return pConstant->getQualifiedDecoratedName(a_Buf);
            break;
        case 8:
            if (constantValue == *((long long*)a_pSrc))
                return pConstant->getQualifiedDecoratedName(a_Buf);
            break;
        }
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

PrimitiveType* Enum::IntType()
{
    return (PrimitiveType*)PHANTOM_TYPEOF(int);
}

Constant* Enum::createConstant(LanguageElement* a_pOwner, void* a_pSrc, StringView a_strName /*= "" */,
                               PrimitiveType* a_pPrimitiveType /*= nullptr*/) const
{
    return getUnderlyingIntType()->createConstant(a_pOwner, a_pSrc, a_strName,
                                                  a_pPrimitiveType ? a_pPrimitiveType : (Enum*)this);
}

} // namespace lang
} // namespace phantom
