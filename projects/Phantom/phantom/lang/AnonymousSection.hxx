#pragma once

// haunt {

#include "AnonymousSection.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("AnonymousSection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(AnonymousSection)
        {
            using AnonymousSections = typedef_< phantom::reflection::AnonymousSections>;
            using LanguageElements = typedef_< phantom::reflection::LanguageElements>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using Variables = typedef_< phantom::reflection::Variables>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .method<AnonymousSection*() const, virtual_|override_>("asAnonymousSection", &_::asAnonymousSection)
            .method<void(Variable*)>("addVariable", &_::addVariable)
            .method<Variable*(size_t) const>("getVariable", &_::getVariable)
            .method<size_t() const>("getVariableCount", &_::getVariableCount)
            .method<LanguageElements const&() const>("getDataElements", &_::getDataElements)
            .method<Variables const&() const>("getVariables", &_::getVariables)
            .method<ClassType*() const>("getRootOwner", &_::getRootOwner)
            .method<AnonymousSection*() const>("getOwnerSection", &_::getOwnerSection)
            .method<AnonymousSection*() const>("getRootSection", &_::getRootSection)
            .method<void(AnonymousSection*)>("addAnonymousSection", &_::addAnonymousSection)
            .method<void(AnonymousSection*)>("removeAnonymousSection", &_::removeAnonymousSection)
            .method<AnonymousSection*(size_t) const>("getAnonymousSection", &_::getAnonymousSection)
            .method<size_t() const>("getAnonymousSectionCount", &_::getAnonymousSectionCount)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
        .protected_()
            .field("m_Variables", &_::m_Variables)
            .field("m_AnonymousSections", &_::m_AnonymousSections)
            .field("m_DataElements", &_::m_DataElements)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousSection")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
