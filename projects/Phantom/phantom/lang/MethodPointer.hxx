#pragma once

// haunt {

// clang-format off

#include "MethodPointer.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("MethodPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MethodPointer)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::MemberPointer>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(ClassType*, FunctionType*, Modifiers, uint)>()({"a_pObjectType","a_pFunctionType","a_Modifiers","a_uiFlags"})["0"]["0"]
        
        .protected_()
            .constructor<void(ClassType*, FunctionType*, size_t, size_t, Modifiers, uint)>()({"a_pObjectType","a_pFunctionType","a_Size","a_Alignment","a_Modifiers","a_uiFlags"})
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<bool(TypesView, Modifiers, uint) const>("matches", &_::matches)({"parameters","a_Modifiers","a_uiFlags"})["0"]["0"]
            /// missing symbol(s) reflection (phantom::Closure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::Closure(void*) const, virtual_>("getClosure", &_::getClosure)({"a_pPointer"})
            .method<MethodPointer*() const, virtual_|override_>("asMethodPointer", &_::asMethodPointer)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)({"a_str","dest"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)({"a_Buf","src"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToLiteral", &_::valueToLiteral)({"a_Buf","src"})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<FunctionType*() const>("getFunctionType", &_::getFunctionType)
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .method<Type*(size_t) const>("getParameterType", &_::getParameterType)({"i"})
            .method<size_t() const>("getParameterTypeCount", &_::getParameterTypeCount)
            .method<TypesView() const>("getParameterTypes", &_::getParameterTypes)
            .method<Type*() const>("getImplicitObjectParameterType", &_::getImplicitObjectParameterType)
            .method<bool(Type*) const>("acceptsCallerExpressionType", &_::acceptsCallerExpressionType)({"a_pType"})
            .method<bool(Modifiers) const>("acceptsCallerExpressionQualifiers", &_::acceptsCallerExpressionQualifiers)({"a_CallerQualifiers"})
            .method<void(void*, void**) const>("call", &_::call)({"a_pPointer","a_pArgs"})
            .method<void(void*, void**, void*) const>("call", &_::call)({"a_pPointer","a_pArgs","a_pReturnAddress"})
            .method<void(void*, void*, void**) const, virtual_>("call", &_::call)({"a_pPointer","a_pThis","a_pArgs"})
            .method<void(void*, void*, void**, void*) const, virtual_>("call", &_::call)({"a_pPointer","a_pThis","a_pArgs","a_pReturnAddress"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
        
        .protected_()
            .field("m_pFunctionType", &_::m_pFunctionType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MethodPointer")
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
