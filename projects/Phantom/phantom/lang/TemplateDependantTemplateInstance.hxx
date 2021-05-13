#pragma once

// haunt {

// clang-format off

#include "TemplateDependantTemplateInstance.h"

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
#include <phantom/constructor>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateDependantTemplateInstance")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantTemplateInstance)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(TemplateSpecialization*, const LanguageElements&, uint)>()({"a_pTemplateSpecialization","a_Arguments","a_uiFlags"})["0"]
            .constructor<void(Template*, const LanguageElements&, uint)>()({"a_pTemplate","a_Arguments","a_uiFlags"})["0"]
            .method<void()>("initialize", &_::initialize)
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)({""})
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<const LanguageElements&() const>("getArguments", &_::getArguments)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pOther"})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<void(StringBuffer&) const>("getDecoration", &_::getDecoration)({"a_Buf"})
            .method<void(StringBuffer&) const>("getQualifiedDecoration", &_::getQualifiedDecoration)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const>("getRelativeDecoration", &_::getRelativeDecoration)({"a_pTo","a_Buf"})
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getRelativeName")
            .using_("LanguageElement::getRelativeDecoratedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
            .method<hash64() const, virtual_|override_>("computeLocalHash", &_::computeLocalHash)
            .method<Class*()>("promoteAsClass", &_::promoteAsClass)
            ;
        }
        PHANTOM_CLASS(TemplateDependantClassPromotion)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Class>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(Type*)>()({"a_pBase"})
            .method<Type*() const>("getBase", &_::getBase)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
            .method<hash64() const, virtual_|override_>("computeLocalHash", &_::computeLocalHash)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pOther"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantTemplateInstance")
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
