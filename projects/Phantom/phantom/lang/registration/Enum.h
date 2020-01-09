// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "Namespace.h"
#include "Type.h"

#include <phantom/lang/ConstantT.h>
#include <phantom/lang/EnumT.h>
#include <phantom/lang/FundamentalsT.h>
#include <phantom/register>

namespace phantom
{
namespace lang
{
template<class T, class Top>
struct EnumBuilderT : TypeBuilderT<T, Top, EnumBuilderT<T, Top>, EnumT<T>>
{
    _PHNTM_REG_FRIENDS;

public:
    using SelfType = EnumBuilderT<T, Top>;
    using BaseType = TypeBuilderT<T, Top, SelfType, EnumT<T>>;
    using MetaType = PHANTOM_TYPENAME BaseType::MetaType;
    using MostDerived = SelfType;
    using TopType = Top;
    using BuilderProxyType = PHANTOM_TYPENAME Top::_PHNTM_Proxy;

    PHANTOM_DECL_OVERRIDE_DELETE_METHOD(SelfType);

    EnumBuilderT(Top* a_pTop) : BaseType(a_pTop, nullptr)
    {
        m_pEnum = this->_PHNTM_getMeta();
    }

    MostDerived& value(StringView a_Name, T a_Val)
    {
        _addConstant(a_Name, a_Val);
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& values(std::initializer_list<Pair<StringView, T>> a_NameValues)
    {
        for (Pair<StringView, T> const& pair : a_NameValues)
        {
            _addConstant(pair.first, pair.second);
        }
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& values(StringView a_NameList, std::initializer_list<T> a_Vals)
    {
        StringViews names;
        this->_splitEnumValues(a_NameList, names);
        PHANTOM_ASSERT(names.size() == a_Vals.size());
        for (size_t i = 0; i < a_Vals.size(); ++i)
        {
            _addConstant(names[i], a_Vals.begin()[i]);
        }
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& values(std::initializer_list<StringView> a_Names, std::initializer_list<T> a_Vals)
    {
        PHANTOM_ASSERT(a_Names.size() == a_Vals.size());
        for (size_t i = 0; i < a_Vals.size(); ++i)
        {
            _addConstant(a_Names.begin()[i], a_Vals.begin()[i]);
        }
        return static_cast<MostDerived&>(*this);
    }

    // meta data
    MostDerived& operator()(lang::MetaDatas&& a_MD)
    {
        if (m_pEnum->getConstants().size())
        {
            m_pEnum->getConstants().back()->addMetaDatas(a_MD);
        }
        else
        {
            BaseType::operator()(std::move(a_MD));
        }
        return static_cast<MostDerived&>(*this);
    }

    // meta data
    MostDerived& operator()(StringView a_Name, Variant&& a_Value)
    {
        if (m_pEnum->getConstants().size())
        {
            m_pEnum->getConstants().back()->setMetaData(a_Name, std::move(a_Value));
        }
        else
        {
            BaseType::operator()(a_Name, std::move(a_Value));
        }
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(StringView a_Annot)
    {
        if (m_pEnum->getConstants().size())
        {
            m_pEnum->getConstants().back()->addAnnotation(a_Annot);
        }
        else
        {
            BaseType::operator()(a_Annot);
        }
        return static_cast<MostDerived&>(*this);
    }

private:
    void _addConstant(StringView a_Nme, T a_Val)
    {
        m_pEnum->addConstant(
        PHANTOM_META_NEW(lang::ConstantT<T>)(m_pEnum, a_Nme, a_Val, 0, PHANTOM_R_FLAG_NATIVE));
    }

private:
    MetaType* m_pEnum;
    Top*      m_pTop;
};
} // namespace lang
} // namespace phantom

#define PHANTOM_ENUM(Name)                                                                                             \
    PHANTOM_REGISTER(ClassTypes, End)                                                                                  \
    {                                                                                                                  \
        if (PHANTOM_REGISTRATION_STEP == phantom::RegistrationStep::ClassTypes)                                        \
        (this_()).enum_<Name>().values _PHNTM_ENUM_VALUES

#define PHANTOM_ENUM_PAIRS(...)                                                                                        \
#    __VA_ARGS__,                                                                                                      \
    {                                                                                                                  \
        __VA_ARGS__                                                                                                    \
    }

#define _PHNTM_ENUM_VALUES(...)                                                                                        \
    (PHANTOM_ENUM_PAIRS(__VA_ARGS__));                                                                                 \
    }                                                                                                                  \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_2

#define _PHNTM_MENUM_VALUES(...)                                                                                       \
    (PHANTOM_ENUM_PAIRS(__VA_ARGS__));                                                                                 \
    _PHNTM_MTRAILING_METADATA // _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_2

#define PHANTOM_MENUM(Name) (this_()).enum_<_::Name>().values _PHNTM_MENUM_VALUES
