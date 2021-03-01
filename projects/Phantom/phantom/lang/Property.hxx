#pragma once

// haunt {

// clang-format off

#include "Property.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Property")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Property)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using StringViews = typedef_< phantom::StringViews>;
            this_()
            .inherits<::phantom::lang::ValueMember>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*, StringView, uint, Modifiers, uint)>()["PHANTOM_R_FILTER_PROPERTY"]["0"]["0"]
        
        .protected_()
            .constructor<void(Type*, StringView, uint, Modifiers, uint, int)>()
        
        .public_()
            .method<void(void const*, void*) const, virtual_|override_>("getValue", &_::getValue)
            .method<void(void*, void const*) const, virtual_|override_>("setValue", &_::setValue)
            .method<Type*() const>("getFunctionsType", &_::getFunctionsType)
            .method<Property*() const, virtual_|override_>("asProperty", &_::asProperty)
            .method<Method*() const>("getSet", &_::getSet)
            .method<Method*(StringView)>("addSet", &_::addSet)["\"\""]
            .method<Method*(StringView)>("addGet", &_::addGet)["\"\""]
            .method<Method*() const>("getGet", &_::getGet)
            /// missing symbol(s) reflection (phantom::lang::Signal) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Signal*() const>("getSignal", &_::getSignal)
            .method<void(Method*)>("setSet", &_::setSet)
            .method<void(Method*)>("setGet", &_::setGet)
            .method<bool() const, virtual_|override_>("isWrittable", &_::isWrittable)
            .method<bool() const, virtual_|override_>("isReadable", &_::isReadable)
            .method<void(StringViews)>("setTouchedFieldNames", &_::setTouchedFieldNames)
            .method<void(ArrayView<String>)>("setTouchedFieldNames", &_::setTouchedFieldNames)
            .method<ArrayView<String>() const>("getTouchedFieldNames", &_::getTouchedFieldNames)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Property")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
