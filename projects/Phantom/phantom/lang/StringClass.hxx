#pragma once

// haunt {

// clang-format off

#include "StringClass.h"

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
    PHANTOM_SOURCE("StringClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(StringClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::SequentialContainerClass>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()({"a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_>("terminate", &_::terminate)
            .method<const void*(void const*) const, pure_virtual>("c_str", &_::c_str)({"a_pString"})
            .method<void(void*, const void*, size_t) const, pure_virtual>("assign", &_::assign)({"a_pString","a_pChars","a_Len"})
            .method<void(void*, const void*) const, pure_virtual>("append", &_::append)({"a_pString","a_pChars"})
            .method<const void*(void const*) const, virtual_>("data", &_::data)({"a_pContainer"})
            .method<void*(void*) const, virtual_>("data", &_::data)({"a_pContainer"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringClass")
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
