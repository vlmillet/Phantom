#pragma once

// haunt {

#include "LocalVariable.h"

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
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LocalVariable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(LocalVariable)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*, StringView, Modifiers, uint)>()["0"]["0"]
            .method<LocalVariable*() const, virtual_|override_>("asLocalVariable", &_::asLocalVariable)
            .method<lang::Type*() const>("getValueType", &_::getValueType)
            .method<bool() const>("isThis", &_::isThis)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Block*() const>("getBlock", &_::getBlock)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<void(ptrdiff_t)>("setEvalFrameOffset", &_::setEvalFrameOffset)
            .method<void(ptrdiff_t)>("setNativeFrameOffset", &_::setNativeFrameOffset)
            .method<ptrdiff_t() const>("getEvalFrameOffset", &_::getEvalFrameOffset)
            .method<ptrdiff_t() const>("getNativeFrameOffset", &_::getNativeFrameOffset)
            .method<LocalVariable*() const>("clone", &_::clone)
            .method<LocalVariable*() const, virtual_>("cloneImpl", &_::cloneImpl)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(Type*)>("setValueType", &_::setValueType)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            .field("m_uiIndexInBlock", &_::m_uiIndexInBlock)
            .field("m_iEvalFrameOffset", &_::m_iEvalFrameOffset)
            .field("m_iNativeFrameOffset", &_::m_iNativeFrameOffset)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LocalVariable")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
