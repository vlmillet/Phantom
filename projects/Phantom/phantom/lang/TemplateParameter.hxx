#pragma once

// haunt {

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateParameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateParameter)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Placeholder*, LanguageElement*, Modifiers, uint)>()["nullptr"]["0"]["0"]
            .method<TemplateParameter*() const, virtual_|override_>("asTemplateParameter", &_::asTemplateParameter)
            .method<size_t() const>("getIndex", &_::getIndex)
            .method<TemplateParameter*() const>("clone", &_::clone)
            .method<TemplateSignature*() const>("getTemplateSignature", &_::getTemplateSignature)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<LanguageElement*() const>("getDefaultArgument", &_::getDefaultArgument)
            .method<void(LanguageElement*)>("setDefaultArgument", &_::setDefaultArgument)
            .method<Placeholder*() const>("getPlaceholder", &_::getPlaceholder)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(LanguageElement*) const>("acceptsArgument", &_::acceptsArgument)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(TemplateParameter*) const>("isSame", &_::isSame)
            .using_("Symbol::isSame")
            .method<bool() const>("isPack", &_::isPack)
            .method<TemplateParameter*(uint) const>("clone", &_::clone)["0"]
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
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

// haunt }
