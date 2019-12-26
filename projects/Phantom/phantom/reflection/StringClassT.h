// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/new>
#include <phantom/reflection/StlVectorClassT.h>
#include <phantom/reflection/StringClass.h>

namespace phantom
{
namespace reflection
{
template<typename t_Ty, typename Base = StringClass>
class StringClassT : public StlVectorClassT<t_Ty, Base>
{
    using SelfType = StringClassT<t_Ty, Base>;
    using BaseType = StlVectorClassT<t_Ty, Base>;
    using StringType = t_Ty;
    using CharType = PHANTOM_TYPENAME t_Ty::value_type;

public:
    StringClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(CharType));
    }

    virtual const void* c_str(const void* a_pString) const override
    {
        const t_Ty* pContainer = static_cast<const t_Ty*>(a_pString);
        return pContainer->c_str();
    }

    virtual void assign(void* a_pString, const void* a_pChars, size_t a_Len) const override
    {
        t_Ty* pContainer = static_cast<t_Ty*>(a_pString);
        pContainer->assign((const CharType*)a_pChars, a_Len);
    }
};

} // namespace reflection
} // namespace phantom
