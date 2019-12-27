#pragma once

// haunt {

#include "Template.h"

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

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Template")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Template)
        {
            using LanguageElements = typedef_< phantom::reflection::LanguageElements>;
            using LanguageElementsView = typedef_< phantom::reflection::LanguageElementsView>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using TemplateParameters = typedef_< phantom::reflection::TemplateParameters>;
            using TemplateSpecializations = typedef_< phantom::reflection::TemplateSpecializations>;
            this_()
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<Template*(StringView, StringView, StringView, LanguageElement*, Modifiers, uint)>("Parse", &_::Parse)
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(TemplateSignature*, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Scope*() const>("getScope", &_::getScope)
            .method<TemplateSpecializations const&() const>("getTemplateSpecializations", &_::getTemplateSpecializations)
            .method<TemplateParameters const&() const>("getTemplateParameters", &_::getTemplateParameters)
            .method<size_t(StringView) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)
            .method<size_t(TemplateParameter*) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)
            .method<void(StringView, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<void(size_t, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<LanguageElement*(StringView) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<LanguageElement*(size_t) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<size_t() const>("getDefaultArgumentCount", &_::getDefaultArgumentCount)
            .method<void(TemplateParameter*)>("addTemplateParameter", &_::addTemplateParameter)
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<TemplateSpecialization*(const LanguageElements&, ClassType*, TemplateSignature*)>("createTemplateSpecialization", &_::createTemplateSpecialization)["nullptr"]["nullptr"]
            .method<TemplateSpecialization*(LanguageElement*, ClassType*, TemplateSignature*)>("createTemplateSpecialization", &_::createTemplateSpecialization)["nullptr"]["nullptr"]
            .method<void(size_t, StringView)>("addTemplateParameterAliasName", &_::addTemplateParameterAliasName)
            .method<TemplateSpecialization*(LanguageElementsView) const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<TemplateSpecialization*(LanguageElementsView) const>("getTemplateInstantiation", &_::getTemplateInstantiation)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<TemplateSpecialization*(const PlaceholderMap&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<TemplateSpecialization*(TemplateSpecialization*) const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<TemplateSpecialization*() const>("getEmptyTemplateSpecialization", &_::getEmptyTemplateSpecialization)
            .method<bool(const LanguageElements&) const>("acceptsArguments", &_::acceptsArguments)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<bool(const LanguageElements&, PlaceholderMap&) const>("mapArguments", &_::mapArguments)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<Template*() const, virtual_|override_>("asTemplate", &_::asTemplate)
            .method<void(const char**, LanguageElement*)>("setNativeDefaultArgumentStrings", &_::setNativeDefaultArgumentStrings)
        
        .protected_()
            .method<void(TemplateSpecialization*)>("addTemplateSpecialization", &_::addTemplateSpecialization)
        
        .protected_()
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
        .protected_()
            .field("m_TemplateSpecializations", &_::m_TemplateSpecializations)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Template")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
