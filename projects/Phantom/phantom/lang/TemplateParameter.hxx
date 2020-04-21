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
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Placeholder*, LanguageElement*, uint)>()["nullptr"]["0"]
            .method<TemplateParameter*() const, virtual_|override_>("asTemplateParameter", &_::asTemplateParameter)
            .method<size_t() const>("getIndex", &_::getIndex)
            .method<TemplateParameter*() const>("clone", &_::clone)
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<LanguageElement*() const>("getDefaultArgument", &_::getDefaultArgument)
            .method<void(LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<Placeholder*() const>("getPlaceholder", &_::getPlaceholder)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(LanguageElement*) const>("acceptsArgument", &_::acceptsArgument)
            .method<bool() const>("isPack", &_::isPack)
            .method<TemplateParameter*(uint) const>("clone", &_::clone)["0"]
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeName", &_::getRelativeName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
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
