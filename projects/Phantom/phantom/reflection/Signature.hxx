#pragma once

// haunt {

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
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Signature")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Signature)
        {
            using EState = typedef_<_::EState>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using Parameters = typedef_< phantom::reflection::Parameters>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using StringViews = typedef_< phantom::StringViews>;
            using Types = typedef_< phantom::reflection::Types>;
            using TypesView = typedef_< phantom::reflection::TypesView>;
            this_()
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<Signature*(Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<Signature*(Type*, uint)>("Create", &_::Create)
            .staticMethod<Signature*(Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<Signature*(Type*, Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<Signature*(Type*, Type*, Type*, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<Signature*(Type*, TypesView, Modifiers, uint)>("Create", &_::Create)["0"]["0"]
            .staticMethod<Signature*(StringView, LanguageElement*, Modifiers, uint)>("Create", &_::Create)["nullptr"]["0"]["0"]
            .enum_<EState>().values({
                {"eState_Virgin",_::eState_Virgin},
                {"eState_UnderConstruction",_::eState_UnderConstruction},
                {"eState_Final",_::eState_Final}})
            .end()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, LanguageElement*, Modifiers, uint)>()["nullptr"]["0"]["0"]
            .constructor<void(Type*, Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, const Parameters&, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, TypesView, Modifiers, uint)>()["0"]["0"]
            .method<Signature*() const, virtual_|override_>("asSignature", &_::asSignature)
            .method<bool() const>("isRVOCandidate", &_::isRVOCandidate)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<void(StringBuffer&) const>("getUniqueParameterTypeNameList", &_::getUniqueParameterTypeNameList)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Signature*) const>("isSame", &_::isSame)
            .method<bool(TypesView, Modifiers) const>("matches", &_::matches)["Modifiers()"]
            .method<bool(Signature*) const>("matches", &_::matches)
            .method<Signature*() const>("clone", &_::clone)
            .method<void(StringView, LanguageElement*)>("parse", &_::parse)["nullptr"]
            .method<size_t() const>("getParameterCount", &_::getParameterCount)
            .method<Parameter*(size_t) const>("getParameter", &_::getParameter)
            .method<Parameter*(StringView) const>("getParameter", &_::getParameter)
            .method<Parameter*(Type*, StringView)>("addParameter", &_::addParameter)["\"\""]
            .method<void(Parameter*)>("addParameter", &_::addParameter)
            .method<Type*(size_t) const>("getParameterType", &_::getParameterType)
            .method<Types() const>("getParameterTypes", &_::getParameterTypes)
            .method<Parameters const&() const>("getParameters", &_::getParameters)
            .method<StringView(size_t) const>("getParameterName", &_::getParameterName)
            /// missing symbol(s) reflection (phantom::reflection::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(size_t) const>("getParameterDefaultValueExpression", &_::getParameterDefaultValueExpression)
            .method<void(Type*)>("setReturnType", &_::setReturnType)
            .method<void(size_t, StringView)>("setParameterName", &_::setParameterName)
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<size_t() const>("getRequiredArgumentCount", &_::getRequiredArgumentCount)
            .method<Subroutine*() const>("getOwnerSubroutine", &_::getOwnerSubroutine)
            .method<void(ArrayView<StringView>)>("setNativeDefaultArgumentStrings", &_::setNativeDefaultArgumentStrings)
            .method<StringViews() const>("getNativeDefaultArgumentStrings", &_::getNativeDefaultArgumentStrings)
            .method<FunctionType*() const>("getFunctionType", &_::getFunctionType)
            .method<void()>("setVariadic", &_::setVariadic)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
        
        .protected_()
            .field("m_pReturnType", &_::m_pReturnType)
            .field("m_Parameters", &_::m_Parameters)
            .field("m_pReturnTypeName", &_::m_pReturnTypeName)
            .field("m_bVariadic", &_::m_bVariadic)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Signature")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
