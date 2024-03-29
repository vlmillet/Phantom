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
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Template*, TemplateSignature*, const LanguageElements&, uint)>()({"a_pTemplate","a_pSignature","arguments","a_Flags"})
            .constructor<void(Template*, TemplateSignature*, const LanguageElements&, Symbol*, uint)>()({"a_pTemplate","a_pSignature","arguments","a_pTemplated","a_Flags"})
            .constructor<void(TemplateSpecialization*, const LanguageElements&, TemplateSubstitution const&)>()({"a_pInstantiationSpecialization","arguments","a_ArgumentSubstitutions"})
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_>("terminate", &_::terminate)
            .method<TemplateSpecialization*() const, virtual_|override_>("asTemplateSpecialization", &_::asTemplateSpecialization)
            .method<::phantom::lang::TemplateSpecialization *() const, virtual_>("getEnclosingTemplateSpecialization", &_::getEnclosingTemplateSpecialization)
            .method<LanguageElement*() const, virtual_|override_>("getNamingScope", &_::getNamingScope)
            .method<size_t() const>("getArgumentCount", &_::getArgumentCount)
            .method<Type*(StringView) const>("getArgumentAsType", &_::getArgumentAsType)({"a_strParameterName"})
            .method<LanguageElement*(StringView) const>("getArgument", &_::getArgument)({"a_strParameterName"})
            .method<LanguageElement*(size_t) const>("getArgument", &_::getArgument)({"i"})
            .method<size_t(StringView) const>("getArgumentIndex", &_::getArgumentIndex)({"a_strParameterName"})
            .method<LanguageElement*(size_t) const>("getDefaultArgument", &_::getDefaultArgument)({"i"})
            .method<LanguageElement*(StringView) const>("getDefaultArgument", &_::getDefaultArgument)({"a_strParameterName"})
            .method<const LanguageElements&() const>("getArguments", &_::getArguments)
            .method<::phantom::SmallVector<LanguageElement *, 20>::const_iterator() const>("beginArguments", &_::beginArguments)
            .method<::phantom::SmallVector<LanguageElement *, 20>::const_iterator() const>("endArguments", &_::endArguments)
            .method<void(size_t, LanguageElement*)>("setArgument", &_::setArgument)({"a_uiIndex","a_pElement"})
            .method<void(size_t, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)({"i","a_pElement"})
            .method<void(StringView, LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)({"a_strParameterName","a_pElement"})
            .method<TemplateParameters const&() const>("getTemplateParameters", &_::getTemplateParameters)
            .method<bool() const>("isVariadic", &_::isVariadic)
            .method<void(StringBuffer&) const>("getDecoration", &_::getDecoration)({"a_Buf"})
            .method<hash64() const>("getDecorationHash", &_::getDecorationHash)
            .method<void(LanguageElement*, StringBuffer&) const>("getRelativeDecoration", &_::getRelativeDecoration)({"a_pTo","a_Buf"})
            .method<void(StringBuffer&) const>("getQualifiedDecoration", &_::getQualifiedDecoration)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<void() const, virtual_|override_>("checkCompleteness", &_::checkCompleteness)
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<bool() const>("isEmpty", &_::isEmpty)
            .method<bool() const>("isPartial", &_::isPartial)
            .method<bool() const>("isFull", &_::isFull)
            .method<bool(TemplateParameter*) const>("isSpecializingParameter", &_::isSpecializingParameter)({"a_pParameter"})
            .method<bool(LanguageElementsView) const>("matches", &_::matches)({"a_Arguments"})
            .method<bool(TemplateSpecialization*) const>("isSame", &_::isSame)({"a_pTemplateSpecialization"})
            .using_("Symbol::isSame")
            .using_("Symbol::partialAccepts")
            .method<bool(const LanguageElements&, size_t&, PlaceholderMap&) const>("partialAccepts", &_::partialAccepts)({"a_Arguments","a_Score","a_Deductions"})
            .method<void(Symbol*)>("setTemplated", &_::setTemplated)({"a_pTemplated"})
            .method<Symbol*() const>("getTemplated", &_::getTemplated)
            .method<TemplateSpecialization*(LanguageElement*, uint) const>("clone", &_::clone)({"a_pOwner","a_Flags"})["0"]
            .method<void(TemplateSpecialization*)>("setExtendedSpecialization", &_::setExtendedSpecialization)({"a_pExtended"})
            .method<TemplateSpecialization*() const>("getExtendedSpecialization", &_::getExtendedSpecialization)
            .method<TemplateSpecialization*() const>("getInstantiationSpecialization", &_::getInstantiationSpecialization)
            .method<TemplateSubstitution const&() const>("getArgumentSubstitution", &_::getArgumentSubstitution)
            .method<Source*() const, virtual_|override_>("getCodeLocationSource", &_::getCodeLocationSource)
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
