// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/lang/SequentialContainerClassT.h>
#include <phantom/lang/StringClass.h>

namespace phantom
{
namespace lang
{
template<typename T, typename Base = StringClass>
class StringClassT : public SequentialContainerClassT<T, Base>
{
    using SelfType = StringClassT<T, Base>;
    using BaseType = SequentialContainerClassT<T, Base>;
    using StringType = T;
    using StringIterator = PHANTOM_TYPENAME StringType::iterator;
    using CharType = PHANTOM_TYPENAME       StringType::value_type;

public:
    StringClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(CharType));
    }

    const void* c_str(const void* a_pString) const override
    {
        const T* pContainer = reinterpret_cast<const T*>(a_pString);
        return pContainer->c_str();
    }

    void assign(void* a_pString, const void* a_pChars, size_t a_Len) const override
    {
        T* pContainer = reinterpret_cast<T*>(a_pString);
        pContainer->assign((const CharType*)a_pChars, a_Len);
    }

    void append(void* a_pString, const void* a_pChars) const override
    {
        T* pContainer = reinterpret_cast<T*>(a_pString);
        pContainer->append((const CharType*)a_pChars);
    }

    bool equal(const void* a_pS0, const void* a_pS1) const override
    {
        return *reinterpret_cast<T const*>(a_pS0) == *reinterpret_cast<T const*>(a_pS1);
    }

    void* data(void* a_pContainer) const override
    {
        T* pContainer = static_cast<T*>(a_pContainer);
        return (void*)pContainer->data();
    }

    const void* data(const void* a_pContainer) const override
    {
        const T* pContainer = static_cast<const T*>(a_pContainer);
        return (const void*)pContainer->data();
    }
};

} // namespace lang
} // namespace phantom
