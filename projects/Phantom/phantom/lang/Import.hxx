#pragma once

// haunt {

// clang-format off

#include "Import.h"

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

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Import")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Import)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Source*, StringView, Modifiers, uint)>()["0"]["0"]
            .method<Import*() const, virtual_>("asImport", &_::asImport)
            .method<Source*() const>("getImportedSource", &_::getImportedSource)
        
        .public_()
            .field("m_pImportedSource", &_::m_pImportedSource)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Import")
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
