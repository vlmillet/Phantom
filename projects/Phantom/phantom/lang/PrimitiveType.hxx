#pragma once

// haunt {

// clang-format off

#include "PrimitiveType.h"

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PrimitiveType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PrimitiveType)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .method<PrimitiveType*() const, virtual_|override_>("asPrimitiveType", &_::asPrimitiveType)
            .method<Type*() const, virtual_|override_>("asPOD", &_::asPOD)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isSignedInteger", &_::isSignedInteger)
            .method<bool() const, virtual_|override_>("isUnsignedInteger", &_::isUnsignedInteger)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<::phantom::lang::Constant *(LanguageElement*, void*, StringView, PrimitiveType*) const, virtual_>("createConstant", &_::createConstant)({"a_pOwner","a_pSrc","a_strName","a_pPrimitiveType"})["\"\""]["nullptr"]
            .method<Constant*(void*, StringView, PrimitiveType*) const>("createConstant", &_::createConstant)({"a_pSrc","a_strName","a_pPrimitiveType"})["\"\""]["nullptr"]
        
        .protected_()
            .constructor<void(TypeKind)>()({"a_eTypeKind"})
            .constructor<void(TypeKind, Type*, StringView, size_t, size_t, Modifiers, uint)>()({"a_eTypeKind","a_pUnderlyingType","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()({"a_eTypeKind","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})["0"]["0"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PrimitiveType")
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
