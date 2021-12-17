#pragma once

// haunt {

// clang-format off

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
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("FunctionPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctionPointer)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::PointerType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
        
        .public_()
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(FunctionType*, ABI, Modifiers, uint)>()({"a_pFunctionType","a_eABI","a_Modifiers","a_uiFlags"})["0"]["0"]
            /// missing symbol(s) reflection (phantom::Closure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::Closure(void*) const, virtual_>("getClosure", &_::getClosure)({"a_pPointer"})
            .method<FunctionPointer*() const, virtual_|override_>("asFunctionPointer", &_::asFunctionPointer)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)({"a_str","dest"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)({"a_Buf","src"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToLiteral", &_::valueToLiteral)({"a_Buf","src"})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<FunctionType*() const>("getFunctionType", &_::getFunctionType)
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<ABI() const>("getABI", &_::getABI)
            .method<void(void*, void**) const, virtual_>("call", &_::call)({"a_pPointer","a_pArgs"})
            .method<void(void*, void**, void*) const, virtual_>("call", &_::call)({"a_pPointer","a_pArgs","a_pReturnAddress"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
        
        .protected_()
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_eABI", &_::m_eABI)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionPointer")
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
