#pragma once

// haunt {

#include "TemplateSignature.h"

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
#include <phantom/using>
#include <phantom/friend>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("TemplateSignature")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateSignature)
        {
            using LanguageElements = typedef_< phantom::reflection::LanguageElements>;
            using StringView = typedef_< phantom::StringView>;
            using TemplateParameters = typedef_< phantom::reflection::TemplateParameters>;
            this_()
            .inherits<::phantom::reflection::LanguageElement>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<TemplateSignature*(uint)>("Create", &_::Create)["PHANTOM_R_NONE"]
            .constructor<void(uint)>()["PHANTOM_R_NONE"]
            .constructor<void(const TemplateParameters&, bool, uint)>()["false"]["PHANTOM_R_NONE"]
            .staticMethod<TemplateSignature*(StringView, StringView, LanguageElement*, uint)>("Parse", &_::Parse)["PHANTOM_R_NONE"]
            .method<TemplateSignature*() const, virtual_|override_>("asTemplateSignature", &_::asTemplateSignature)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateParameter*(Type*, StringView)>("addTemplateValueParameter", &_::addTemplateValueParameter)
            .method<TemplateParameter*(StringView)>("addTemplateTypeParameter", &_::addTemplateTypeParameter)
            .method<TemplateParameter*(TemplateSignature*, StringView)>("addTemplateTemplateParameter", &_::addTemplateTemplateParameter)
            .method<size_t(StringView) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)
            .method<size_t(Placeholder*) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)
            .method<size_t(TemplateParameter*) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)
            .method<void(StringView, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<void(size_t, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<LanguageElement*(StringView) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<LanguageElement*(size_t) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<size_t() const>("getDefaultArgumentCount", &_::getDefaultArgumentCount)
            .method<void(TemplateParameter*)>("addTemplateParameter", &_::addTemplateParameter)
            .method<void(size_t, StringView)>("addTemplateParameterAliasName", &_::addTemplateParameterAliasName)
            .method<const LanguageElements&() const>("getPlaceholders", &_::getPlaceholders)
            .method<const TemplateParameters&() const>("getTemplateParameters", &_::getTemplateParameters)
            .method<TemplateParameter*(StringView) const>("getTemplateParameter", &_::getTemplateParameter)
            .method<bool(const LanguageElements&) const>("acceptsArguments", &_::acceptsArguments)
            .method<bool(Symbol*) const, virtual_>("isSame", &_::isSame)
            .using_("LanguageElement::isSame")
            .method<bool(TemplateSignature*) const>("isSame", &_::isSame)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<void(bool)>("setVariadic", &_::setVariadic)
            .method<TemplateSignature*(uint) const>("clone", &_::clone)["0"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateSignature")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
