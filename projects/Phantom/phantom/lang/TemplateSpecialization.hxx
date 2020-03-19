#pragma once

// haunt {

// clang-format off

#include "TemplateSpecialization.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateSpecialization")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateSpecialization)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using LanguageElementsView = typedef_< phantom::lang::LanguageElementsView>;
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TemplateParameters = typedef_< phantom::lang::TemplateParameters>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .staticMethod<TemplateSpecialization*(Template*, TemplateSignature*, const LanguageElements&, Symbol*, uint)>("Create", &_::Create)
            .constructor<void(Template*, TemplateSignature*, const LanguageElements&, uint)>()
            .constructor<void(Template*, TemplateSignature*, const LanguageElements&, Symbol*, uint)>()
            .constructor<void(TemplateSpecialization*, const LanguageElements&, const PlaceholderMap&)>()
        
        .public_()
            .method<TemplateSpecialization*() const, virtual_|override_>("asTemplateSpecialization", &_::asTemplateSpecialization)
            .method<TemplateSpecialization*() const, virtual_>("getEnclosingTemplateSpecialization", &_::getEnclosingTemplateSpecialization)
            .method<Scope*() const, virtual_|override_>("getNamingScope", &_::getNamingScope)
            .method<size_t() const>("getArgumentCount", &_::getArgumentCount)
            .method<Type*(StringView) const>("getArgumentAsType", &_::getArgumentAsType)
            .method<LanguageElement*(StringView) const>("getArgument", &_::getArgument)
            .method<LanguageElement*(size_t) const>("getArgument", &_::getArgument)
            .method<size_t(StringView) const>("getArgumentIndex", &_::getArgumentIndex)
            .method<LanguageElement*(size_t) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<LanguageElement*(StringView) const>("getDefaultArgument", &_::getDefaultArgument)
            .method<const LanguageElements&() const>("getArguments", &_::getArguments)
            .method<LanguageElements::const_iterator() const>("beginArguments", &_::beginArguments)
            .method<LanguageElements::const_iterator() const>("endArguments", &_::endArguments)
            .method<void(size_t, LanguageElement*)>("setArgument", &_::setArgument)
            .method<void(size_t, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<void(StringView, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<void(LanguageElement*)>("removeArgument", &_::removeArgument)
            .method<TemplateParameters const&() const>("getTemplateParameters", &_::getTemplateParameters)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<void(StringBuffer&) const>("getDecoration", &_::getDecoration)
            .method<void(StringBuffer&) const>("getQualifiedDecoration", &_::getQualifiedDecoration)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<void() const, virtual_|override_>("checkCompleteness", &_::checkCompleteness)
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<bool() const>("isEmpty", &_::isEmpty)
            .method<bool() const>("isPartial", &_::isPartial)
            .method<bool() const>("isFull", &_::isFull)
            .method<bool(TemplateParameter*) const>("isSpecializingParameter", &_::isSpecializingParameter)
            .method<bool(LanguageElementsView) const>("matches", &_::matches)
            .method<bool(TemplateSpecialization*) const>("isSame", &_::isSame)
            .using_("Symbol::isSame")
            .using_("Symbol::partialAccepts")
            .method<bool(const LanguageElements&, size_t&, PlaceholderMap&) const>("partialAccepts", &_::partialAccepts)
            .method<void(Symbol*)>("setTemplated", &_::setTemplated)
            .method<Symbol*() const>("getTemplated", &_::getTemplated)
            .method<TemplateSpecialization*(uint) const>("clone", &_::clone)["0"]
            .method<void(TemplateSpecialization*)>("setExtendedSpecialization", &_::setExtendedSpecialization)
            .method<TemplateSpecialization*() const>("getExtendedSpecialization", &_::getExtendedSpecialization)
            .method<TemplateSpecialization*() const>("getInstantiationSpecialization", &_::getInstantiationSpecialization)
            .method<PlaceholderMap const&() const>("getPlaceholderSubstitutions", &_::getPlaceholderSubstitutions)
            .method<Source*() const, virtual_|override_>("getCodeLocationSource", &_::getCodeLocationSource)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementAdded", &_::onReferencedElementAdded)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateSpecialization")
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

