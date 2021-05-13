#pragma once

// haunt {

// clang-format off

#include "SetClass.h"

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
    PHANTOM_SOURCE("SetClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SetClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ContainerClass>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()({"a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Type*() const, virtual_|final_|override_>("getKeyType", &_::getKeyType)
            .method<void(void*, void const*) const, virtual_>("insert", &_::insert)({"a_pContainer","a_pKey"})
            .method<void(void*, void const*) const, virtual_|override_>("eraseKey", &_::eraseKey)({"a_pContainer","a_pKey"})
            .method<void(void const*, void const*, void*) const, virtual_>("find", &_::find)({"a_pContainer","a_pKey","a_pIt"})
        
        .protected_()
            .field("m_pKeyType", &_::m_pKeyType)
            .field("m_pMappedType", &_::m_pMappedType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SetClass")
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
