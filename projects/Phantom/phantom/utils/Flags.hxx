#pragma once

// haunt {

// clang-format off

#include "Flags.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/property>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Flags")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Flag)
        {
            this_()
        .public_()
            .constructor<void(int)>()({"i"})
            .method<int() const>("operator int", &_::operator notypedef<int>)
            ;
        }
        PHANTOM_CLASS(IncompatibleFlag)
        {
            this_()
        .public_()
            .constructor<void(int), explicit_>()({"i"})
            .method<int() const>("operator int", &_::operator notypedef<int>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (Enum), FlagsBit)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(Flags<Enum>*, Enum)>()({"a_Flags","a_Where"})
            .PHANTOM_T method<void(bool)>("setValue", &_::setValue)({"a_bValue"})
            .PHANTOM_T method<bool() const>("getValue", &_::getValue)
            .PHANTOM_T property<bool>("value", &_::getValue, &_::setValue)
        
        .protected_()
            .PHANTOM_T field("m_Flags", &_::m_Flags)
            .PHANTOM_T field("m_Where", &_::m_Where)
            ;
        }
        PHANTOM_STRUCT_T((class), (Enum), ConstFlagsBit)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(const Flags<Enum>*, Enum)>()({"a_Flags","a_Where"})
            .PHANTOM_T method<bool() const>("getValue", &_::getValue)
        
        .protected_()
            .PHANTOM_T field("m_Flags", &_::m_Flags)
            .PHANTOM_T field("m_Where", &_::m_Where)
            ;
        }
        PHANTOM_CLASS_T((class), (Enum), Flags)
        {
            using Bit = typedef_<PHANTOM_TYPENAME _::Bit>;
            using ConstBit = typedef_<PHANTOM_TYPENAME _::ConstBit>;
            using EnumType = typedef_<PHANTOM_TYPENAME _::EnumType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            using _Zero = typedef_<PHANTOM_TYPENAME _::_Zero>;
            this_()
        .public_()
            .PHANTOM_T typedef_<_Zero>("_Zero")
            .PHANTOM_T typedef_<SelfType>("SelfType")
            .PHANTOM_T typedef_<Bit>("Bit")
            .PHANTOM_T typedef_<ConstBit>("ConstBit")
            .PHANTOM_T typedef_<EnumType>("EnumType")
            .PHANTOM_T constructor<void(const SelfType&)>()({"f"})
            .PHANTOM_T constructor<void(Enum)>()({"f"})
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(_Zero)>()({""})["0"]
            )
            .PHANTOM_T constructor<void(Flag)>()({"f"})
            .PHANTOM_T method<SelfType&(const SelfType&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"f"})
            .PHANTOM_T method<SelfType&(int)>("operator&=", &_::operator&=)({"mask"})
            .PHANTOM_T method<SelfType&(unsigned int)>("operator&=", &_::operator&=)({"mask"})
            .PHANTOM_T method<SelfType&(SelfType)>("operator|=", &_::operator|=)({"f"})
            .PHANTOM_T method<SelfType&(Enum)>("operator|=", &_::operator|=)({"f"})
            .PHANTOM_T method<SelfType&(SelfType)>("operator^=", &_::operator^=)({"f"})
            .PHANTOM_T method<SelfType&(Enum)>("operator^=", &_::operator^=)({"f"})
            .PHANTOM_T method<int() const>("operator int", &_::operator notypedef<int>)
            .PHANTOM_T method<SelfType(SelfType) const>("operator|", &_::operator|)({"f"})
            .PHANTOM_T method<SelfType(Enum) const>("operator|", &_::operator|)({"f"})
            .PHANTOM_T method<SelfType(SelfType) const>("operator^", &_::operator^)({"f"})
            .PHANTOM_T method<SelfType(Enum) const>("operator^", &_::operator^)({"f"})
            .PHANTOM_T method<SelfType(int) const>("operator&", &_::operator&)({"mask"})
            .PHANTOM_T method<SelfType(unsigned int) const>("operator&", &_::operator&)({"mask"})
            .PHANTOM_T method<SelfType(Enum) const>("operator&", &_::operator&)({"f"})
            .PHANTOM_T method<SelfType() const>("operator~", &_::operator~)
            .PHANTOM_T method<bool() const>("operator!", &_::operator!)
            .PHANTOM_T method<bool(Enum) const>("testFlag", &_::testFlag)({"f"})
            .PHANTOM_T method<Bit(Enum)>("operator[]", &_::operator[])({"f"})
            .PHANTOM_T method<ConstBit(Enum) const>("operator[]", &_::operator[])({"f"})
            ;
        }
    PHANTOM_END("Flags")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
