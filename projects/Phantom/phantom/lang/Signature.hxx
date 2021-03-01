#pragma once

// haunt {

// clang-format off

#include "Signature.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/using>
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
    PHANTOM_SOURCE("Signature")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Signature)
        {
            using EState = typedef_<_::EState>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Parameters = typedef_< phantom::lang::Parameters>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using StringViews = typedef_< phantom::StringViews>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .enum_<EState>().values({
                {"eState_Virgin",_::eState_Virgin},
                {"eState_UnderConstruction",_::eState_UnderConstruction},
                {"eState_Final",_::eState_Final}})
            .end()
            .staticMethod<::phantom::lang::Signature *(LanguageElement*, Type*, TypesView, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<::phantom::lang::Signature *(LanguageElement*, Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, const Parameters&, Modifiers, uint)>()["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<Signature*() const, virtual_|override_>("asSignature", &_::asSignature)
            .method<bool() const>("isRVOCandidate", &_::isRVOCandidate)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<void(StringBuffer&) const>("getUniqueParameterTypeNameList", &_::getUniqueParameterTypeNameList)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Signature*) const>("isSame", &_::isSame)
            .method<bool(TypesView, Modifiers) const>("matches", &_::matches)["Modifiers()"]
            .method<bool(Signature*) const>("matches", &_::matches)
            .method<Signature*(LanguageElement*) const>("clone", &_::clone)
            .method<size_t() const>("getParameterCount", &_::getParameterCount)
            .method<Parameter*(size_t) const>("getParameter", &_::getParameter)
            .method<Parameter*(StringView) const>("getParameter", &_::getParameter)
            .method<Type*(size_t) const>("getParameterType", &_::getParameterType)
            .method<Types() const>("getParameterTypes", &_::getParameterTypes)
            .method<Parameters const&() const>("getParameters", &_::getParameters)
            .method<StringView(size_t) const>("getParameterName", &_::getParameterName)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(size_t) const>("getParameterDefaultValueExpression", &_::getParameterDefaultValueExpression)
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getRelativeDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<size_t() const>("getRequiredArgumentCount", &_::getRequiredArgumentCount)
            .method<Subroutine*() const>("getOwnerSubroutine", &_::getOwnerSubroutine)
            .method<void(ArrayView<StringView>)>("setNativeDefaultArgumentStrings", &_::setNativeDefaultArgumentStrings)
            .method<StringViews() const>("getNativeDefaultArgumentStrings", &_::getNativeDefaultArgumentStrings)
            .method<FunctionType*() const>("getFunctionType", &_::getFunctionType)
            .method<void()>("setVariadic", &_::setVariadic)
        
        .protected_()
            .field("m_pReturnType", &_::m_pReturnType)
            .field("m_Parameters", &_::m_Parameters)
            .field("m_bVariadic", &_::m_bVariadic)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Signature")
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
