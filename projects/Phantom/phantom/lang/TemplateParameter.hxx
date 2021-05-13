#pragma once

// haunt {

// clang-format off

#include "TemplateParameter.h"

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

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateParameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateParameter)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Placeholder*, LanguageElement*, uint)>()({"a_pPlaceholder","a_pDefaultArgument","a_uiFlags"})["nullptr"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<TemplateParameter*() const, virtual_|override_>("asTemplateParameter", &_::asTemplateParameter)
            .method<size_t() const>("getIndex", &_::getIndex)
            .method<TemplateParameter*(LanguageElement*) const>("clone", &_::clone)({"a_pOwner"})
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<LanguageElement*() const>("getDefaultArgument", &_::getDefaultArgument)
            .method<void(LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)({"a_pElement"})
            .method<Placeholder*() const>("getPlaceholder", &_::getPlaceholder)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)({"a_pLanguageElement","a_Score","a_Deductions"})
            .method<bool(LanguageElement*) const>("acceptsArgument", &_::acceptsArgument)({"a_pLanguageElement"})
            .method<bool() const>("isPack", &_::isPack)
            .method<TemplateParameter*(LanguageElement*, uint) const>("clone", &_::clone)({"a_pOwner","a_Flags"})["0"]
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
        
        .protected_()
            .method<hash64() const, virtual_|override_>("computeLocalHash", &_::computeLocalHash)
        
        .protected_()
            .field("m_pPlaceholder", &_::m_pPlaceholder)
            .field("m_pDefaultArgument", &_::m_pDefaultArgument)
            .field("m_bPack", &_::m_bPack)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameter")
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
