#pragma once

// haunt {

// clang-format off

#include "Variant.h"

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
#include <phantom/static_field>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "Delegate.hxx"
#include "Generic.hxx"
#include "SmallString.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Variant")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Variant)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TypeConverter = typedef_<_::TypeConverter>;
            this_()
        .public_()
            .typedef_<TypeConverter>("TypeConverter")
            .staticMethod<void(TypeConverter)>("SetTypeConverter", &_::SetTypeConverter)({"a_TC"})
            .staticMethod<TypeConverter()>("GetTypeConverter", &_::GetTypeConverter)
            .staticMethod<::phantom::Variant(::phantom::Generic::Param<>, ::phantom::Generic::Arg<Generic::ParamType<0> *, void *>)>("From", &_::From)({"a_Param0","a_Arg0"})
        
        .public_()
            .staticField("null", &_::null)
            .constructor<void()>()
            .constructor<void(const char*)>()({"a_Str"})
            .constructor<void(const Variant&)>()({"a_Other"})
            .constructor<void(Variant&&)>()({"a_Other"})
            .constructor<void(::phantom::lang::Type *, void*, bool)>()({"a_pType","a_pValue","a_bMove"})["false"]
            .constructor<void(::phantom::lang::Type *, void const*)>()({"a_pType","a_pValue"})
            .method<::phantom::Variant &(const Variant&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .method<::phantom::Variant &(Variant&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .method<::phantom::Variant &(const char*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Str"})
            .method<void()>("clear", &_::clear)
            .method<void(::phantom::lang::Type *)>("setType", &_::setType)({"a_pType"})
            .method<bool(const Variant&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const Variant&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(StringView) const>("operator==", &_::operator==)({"other"})
            .method<bool(StringView) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const char*) const>("operator==", &_::operator==)({"other"})
            .method<bool(const char*) const>("operator!=", &_::operator!=)({"other"})
            .method<void(const void*)>("copyAssign", &_::copyAssign)({"a_pValue"})
            .method<void(void*)>("moveAssign", &_::moveAssign)({"a_pValue"})
            .method<void(::phantom::lang::Type *, const void*)>("copyConstruct", &_::copyConstruct)({"a_pType","a_pValue"})
            .method<void(::phantom::lang::Type *, void*)>("moveConstruct", &_::moveConstruct)({"a_pType","a_pValue"})
            .method<size_t() const>("size", &_::size)
            .method<::phantom::lang::Type *() const>("type", &_::type)
            .method<const char*() const>("c_str", &_::c_str)
            .method<bool() const>("isString", &_::isString)
            .method<const void*() const>("data", &_::data)
            .method<void*()>("data", &_::data)
            .method<bool() const>("isValid", &_::isValid)
            .method<bool() const>("isNull", &_::isNull)
            .method<bool(::phantom::Generic::Param<>, ::phantom::Generic::Arg<Generic::ParamType<0> *, void *>) const>("as", &_::as)({"a_Param0","a_Arg0"})
            .method<::phantom::Variant(::phantom::lang::Type *) const>("as", &_::as)({"a_pType"})
            .method<void(StringBuffer&) const>("toLiteral", &_::toLiteral)({"a_Buf"})
            .method<void(StringBuffer&) const>("toString", &_::toString)({"a_Buf"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Variant")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
