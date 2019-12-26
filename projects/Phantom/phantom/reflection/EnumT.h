// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include "TypeT.h"

#include <phantom/alignof>
#include <phantom/reflection/Enum.h>
#include <phantom/traits/IsEnumClass.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace reflection
{
template<typename t_Ty>
class EnumT : public TypeT<t_Ty, Enum>
{
public:
    using BaseType = TypeT<t_Ty, Enum>;
    EnumT(StringView a_strName, Modifiers a_Modifiers = 0)
        : BaseType(a_strName, a_Modifiers, PHANTOM_R_FLAG_NATIVE)
    {
        this->m_pUnderlyingIntType =
        (PrimitiveType*)PHANTOM_PRECISE_TYPEOF(PHANTOM_TYPENAME std::underlying_type<t_Ty>::type);
        if (IsEnumClass<t_Ty>::value)
            this->setScoped();
    }
};

#define _PHNTM_SPEC_ANON_ENUMT(intType)                                                            \
    template<>                                                                                     \
    class EnumT<intType> : public TypeT<intType, Enum>                                             \
    {                                                                                              \
    public:                                                                                        \
        using BaseType = TypeT<intType, Enum>;                                                     \
        EnumT(Modifiers a_Modifiers = 0) : EnumT("", a_Modifiers)                                  \
        {                                                                                          \
            this->m_pUnderlyingIntType = (PrimitiveType*)PHANTOM_PRECISE_TYPEOF(intType);          \
        }                                                                                          \
        EnumT(StringView a_Name, Modifiers a_Modifiers = 0)                                        \
            : BaseType(a_Name, a_Modifiers, PHANTOM_R_FLAG_NATIVE)                                 \
        {                                                                                          \
        }                                                                                          \
        virtual hash64 computeHash() const override                                                \
        {                                                                                          \
            return Enum::computeHash();                                                            \
        }                                                                                          \
        virtual String getQualifiedDecoratedName() const override                                  \
        {                                                                                          \
            return Enum::getQualifiedDecoratedName();                                              \
        }                                                                                          \
        virtual String getQualifiedName() const override                                           \
        {                                                                                          \
            return Enum::getQualifiedName();                                                       \
        }                                                                                          \
        virtual String getDecoratedName() const override                                           \
        {                                                                                          \
            return Enum::getDecoratedName();                                                       \
        }                                                                                          \
        virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const override                 \
        {                                                                                          \
            Enum::getQualifiedDecoratedName(a_Buf);                                                \
        }                                                                                          \
        virtual void getQualifiedName(StringBuffer& a_Buf) const override                          \
        {                                                                                          \
            Enum::getQualifiedName(a_Buf);                                                         \
        }                                                                                          \
        virtual void getDecoratedName(StringBuffer& a_Buf) const override                          \
        {                                                                                          \
            Enum::getDecoratedName(a_Buf);                                                         \
        }                                                                                          \
    };

_PHNTM_SPEC_ANON_ENUMT(int8_t);
_PHNTM_SPEC_ANON_ENUMT(int16_t);
_PHNTM_SPEC_ANON_ENUMT(int32_t);
_PHNTM_SPEC_ANON_ENUMT(int64_t);
_PHNTM_SPEC_ANON_ENUMT(uint8_t);
_PHNTM_SPEC_ANON_ENUMT(uint16_t);
_PHNTM_SPEC_ANON_ENUMT(uint32_t);
_PHNTM_SPEC_ANON_ENUMT(uint64_t);

} // namespace reflection
} // namespace phantom

/// @endcond
