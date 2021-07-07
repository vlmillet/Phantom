#pragma once

// haunt {

// clang-format off

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Template")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Template)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using LanguageElementsView = typedef_< phantom::lang::LanguageElementsView>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringView = typedef_< phantom::StringView>;
            using TemplateParameters = typedef_< phantom::lang::TemplateParameters>;
            using TemplateSpecializations = typedef_< phantom::lang::TemplateSpecializations>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<::phantom::lang::Template *(LanguageElement*, StringView, StringView, StringView, LanguageElement*, Modifiers, uint)>("Parse", &_::Parse)({"a_pOwner","a_strTemplateTypes","a_strTemplateParam","a_strName","a_pContextScope","a_Modifiers","a_uiFlags"})
            .constructor<void(Modifiers, uint)>()({"a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(TemplateSignature*, StringView, Modifiers, uint)>()({"a_pSignature","a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<Scope*() const>("getScope", &_::getScope)
            .method<TemplateSpecializations const&() const>("getTemplateSpecializations", &_::getTemplateSpecializations)
            .method<TemplateParameters const&() const>("getTemplateParameters", &_::getTemplateParameters)
            .method<size_t(StringView) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)({"a_strName"})
            .method<size_t(TemplateParameter*) const>("getTemplateParameterIndex", &_::getTemplateParameterIndex)({"a_pTemplateParameter"})
            .method<void(StringView, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)({"a_strParameterName","a_pElement"})
            .method<void(size_t, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)({"a_uiIndex","a_pElement"})
            .method<LanguageElement*(StringView) const>("getDefaultArgument", &_::getDefaultArgument)({"a_strParameterName"})
            .method<LanguageElement*(size_t) const>("getDefaultArgument", &_::getDefaultArgument)({"i"})
            .method<size_t() const>("getDefaultArgumentCount", &_::getDefaultArgumentCount)
            .method<void(TemplateParameter*)>("addTemplateParameter", &_::addTemplateParameter)({"a_pTemplateParameter"})
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<TemplateSpecialization*(const LanguageElements&, ClassType*, TemplateSignature*)>("createTemplateSpecialization", &_::createTemplateSpecialization)({"a_Arguments","a_pTemplated","a_pTemplateSignature"})["nullptr"]["nullptr"]
            .method<TemplateSpecialization*(LanguageElement*, ClassType*, TemplateSignature*)>("createTemplateSpecialization", &_::createTemplateSpecialization)({"a_pArgument","a_pBody","a_pTemplateSignature"})["nullptr"]["nullptr"]
            .method<void(size_t, StringView)>("addTemplateParameterAliasName", &_::addTemplateParameterAliasName)({"a_uiIndex","a_strAlias"})
            .method<TemplateSpecialization*(LanguageElementsView) const>("getTemplateSpecialization", &_::getTemplateSpecialization)({"a_Arguments"})
            .method<TemplateSpecialization*(LanguageElementsView, Module*, bool) const>("getTemplateSpecializationForModule", &_::getTemplateSpecializationForModule)({"a_Arguments","a_pModule","_acceptsDependencies"})["true"]
            .method<TemplateSpecialization*(LanguageElementsView) const>("getTemplateInstantiation", &_::getTemplateInstantiation)({"a_Arguments"})
            .method<TemplateSpecialization*(const PlaceholderMap&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)({"arguments"})
            .method<TemplateSpecialization*(TemplateSpecialization*) const>("getTemplateSpecialization", &_::getTemplateSpecialization)({"a_pTemplateSpecialization"})
            .method<TemplateSpecialization*() const>("getEmptyTemplateSpecialization", &_::getEmptyTemplateSpecialization)
            .method<bool(const LanguageElements&) const>("acceptsArguments", &_::acceptsArguments)({"a_Arguments"})
            .method<bool(const LanguageElements&, PlaceholderMap&) const>("mapArguments", &_::mapArguments)({"a_Arguments","a_Out"})
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<Template*() const, virtual_|override_>("asTemplate", &_::asTemplate)
            .method<void(const char**, LanguageElement*)>("setNativeDefaultArgumentStrings", &_::setNativeDefaultArgumentStrings)({"a_ppArgs","a_pScope"})
        
        .protected_()
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Template")
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
