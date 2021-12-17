#pragma once

// haunt {

// clang-format off

#include "ArrayClass.h"

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
    PHANTOM_SOURCE("ArrayClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ArrayClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Class>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Type*() const>("getItemType", &_::getItemType)
            .method<size_t() const>("getItemCount", &_::getItemCount)
            .method<void(void*, size_t, void const*), virtual_>("setItemValue", &_::setItemValue)({"a_pContainer","a_uiIndex","a_pSrc"})
            .method<void(void const*, size_t, void*), virtual_>("getItemValue", &_::getItemValue)({"a_pContainer","a_uiIndex","a_pDst"})
            .method<const void*(void const*) const, virtual_>("data", &_::data)({"a_pContainer"})
            .method<void*(void*) const, virtual_>("data", &_::data)({"a_pContainer"})
            .method<size_t(void const*) const, virtual_>("size", &_::size)({"a_pContainer"})
            .method<const void*(void const*, size_t) const, virtual_>("referenceAt", &_::referenceAt)({"a_pContainer","a_uiIndex"})
            .method<void*(void*, size_t) const, virtual_>("referenceAt", &_::referenceAt)({"a_pContainer","a_uiIndex"})
            .method<void(void*, void*) const, virtual_>("begin", &_::begin)({"a_pContainer","a_pOutIt"})
            .method<void(void const*, void*) const, virtual_>("begin", &_::begin)({"a_pContainer","a_pOutIt"})
            .method<void(void*, void*) const, virtual_>("end", &_::end)({"a_pContainer","a_pOutIt"})
            .method<void(void const*, void*) const, virtual_>("end", &_::end)({"a_pContainer","a_pOutIt"})
            .method<void*(void*) const, virtual_>("dereferenceIterator", &_::dereferenceIterator)({"a_pIt"})
            .method<void(void*, size_t) const, virtual_>("advanceIterator", &_::advanceIterator)({"a_pIt","a_N"})["1"]
            .method<bool(void*, void*) const, virtual_>("compareIterators", &_::compareIterators)({"a_pIt1","a_pIt2"})
            .method<::phantom::lang::Type *() const, virtual_>("getIteratorType", &_::getIteratorType)
            .method<::phantom::lang::Type *() const, virtual_>("getConstIteratorType", &_::getConstIteratorType)
        
        .protected_()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()({"a_TypeKind","a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()({"a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()({"a_TypeKind","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
            .method<void(Type*)>("setItemType", &_::setItemType)({"a_pElemType"})
            .method<void(size_t)>("setItemCount", &_::setItemCount)({"a_C"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ArrayClass")
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
