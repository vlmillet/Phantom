#pragma once

// haunt {

#include "FunctionPointer.h"

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("FunctionPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctionPointer)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::reflection::PointerType>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(FunctionType*, ABI, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            /// missing symbol(s) reflection (phantom::Closure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Closure(void*) const, virtual_>("getClosure", &_::getClosure)
            .method<FunctionPointer*() const, virtual_|override_>("asFunctionPointer", &_::asFunctionPointer)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToLiteral", &_::valueToLiteral)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<FunctionType*() const>("getFunctionType", &_::getFunctionType)
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<ABI() const>("getABI", &_::getABI)
            .method<void(void*, void**) const, virtual_>("call", &_::call)
            .method<void(void*, void**, void*) const, virtual_>("call", &_::call)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<FunctionPointer*(FunctionType*, ABI, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
        
        .protected_()
            /// missing symbol(s) reflection (phantom::reflection::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_eABI", &_::m_eABI)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionPointer")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
