#pragma once

// haunt {

// clang-format off

#include "Method.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Method")

        PHANTOM_STRUCT_TV((class), (R), (class, Args), MethodInvokeH)
        {
            this_()
            .PHANTOM_T staticMethod<R(Method const*, void*, Args&&...)>("invoke", &_::invoke)
            ;
        }
        PHANTOM_STRUCT_VS((class, Args), (void, Args...), MethodInvokeH)
        {
            this_()
            .PHANTOM_T staticMethod<void(Method const*, void*, Args&&...)>("invoke", &_::invoke)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Method)
        {
            using Methods = typedef_< phantom::lang::Methods>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Subroutine>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(ABI, Modifiers, uint)>()["ABI::MethodCall"]["0"]["0"]
            .constructor<void(StringView, Signature*, Modifiers, uint)>()["0"]["0"]
            .method<lang::ClassType*() const>("getOwnerClassType", &_::getOwnerClassType)
            .method<lang::Class*() const>("getOwnerClass", &_::getOwnerClass)
            .method<void(void*, void**) const, virtual_|override_>("invoke", &_::invoke)
            .method<void(void*, void**, void*) const, virtual_|override_>("invoke", &_::invoke)
            .method<void(void*, void**, void*) const, virtual_|override_>("placementInvoke", &_::placementInvoke)
            .method<void(ExecutionContext&, void**) const, virtual_|override_>("placementCall", &_::placementCall)
            .method<void(void**) const, virtual_|override_>("call", &_::call)
            .method<void(void**, void*) const, virtual_|override_>("call", &_::call)
            .method<bool(Method*) const>("canOverride", &_::canOverride)
            .method<bool(StringView, Signature*, Modifiers) const>("canOverride", &_::canOverride)["0"]
            .method<bool(Method*) const>("isOverridableBy", &_::isOverridableBy)
            .method<bool(StringView, Signature*, Modifiers) const>("isOverridableBy", &_::isOverridableBy)["0"]
            .method<Method*() const, virtual_|override_>("asSlot", &_::asSlot)
            .method<LanguageElement*() const, virtual_>("asLanguageElement", &_::asLanguageElement)
            .method<Subroutine*() const, virtual_|override_>("asSubroutine", &_::asSubroutine)
            .method<Method*() const, virtual_|override_>("asMethod", &_::asMethod)
            .method<size_t(size_t) const>("getVirtualTableIndex", &_::getVirtualTableIndex)
            .method<void()>("setVirtual", &_::setVirtual)
            .method<void*(size_t) const, virtual_>("getVTableClosure", &_::getVTableClosure)
            .method<void(size_t, void*)>("setVTableClosure", &_::setVTableClosure)
            .method<void(Methods&) const>("getOriginalOverriddenMethods", &_::getOriginalOverriddenMethods)
            .method<Property*() const>("getProperty", &_::getProperty)
            .method<bool(Type*) const>("acceptsCallerExpressionType", &_::acceptsCallerExpressionType)
            .method<bool(Modifiers) const>("acceptsCallerExpressionQualifiers", &_::acceptsCallerExpressionQualifiers)
            .method<Type*() const>("getImplicitObjectParameterType", &_::getImplicitObjectParameterType)
            .method<LocalVariable*() const>("getThis", &_::getThis)
        
        .protected_()
            .constructor<void(LanguageElement*, StringView, StringView, Modifiers, uint)>()["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(LanguageElement*, StringView, StringView, ABI, Modifiers, uint)>()["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(StringView, Signature*, ABI, Modifiers, uint)>()["0"]["0"]
            .method<size_t() const, virtual_>("computeNativeVirtualIndex", &_::computeNativeVirtualIndex)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
            .method<LocalVariable*() const, virtual_>("createThis", &_::createThis)
        
        .protected_()
            .field("m_pThis", &_::m_pThis)
            .field("m_uiVirtualTableIndex", &_::m_uiVirtualTableIndex)
            .field("m_pVTableClosures", &_::m_pVTableClosures)
            .field("m_pProperty", &_::m_pProperty)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Method")
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
