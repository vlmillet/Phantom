#pragma once

// haunt {

#include "Signal.h"

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

#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Signal")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Signal)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::reflection::Types>;
            this_()
            .inherits<::phantom::reflection::Method>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(), default_>()
            .constructor<void(StringView, Types, Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Signature*, Modifiers, uint)>()["0"]["0"]
            .method<void*(void*) const, virtual_>("getInstance", &_::getInstance)
            .method<void(void*)>("block", &_::block)
            .method<void(void*)>("unblock", &_::unblock)
            .method<Signal*() const, virtual_|override_>("asSignal", &_::asSignal)
            .method<size_t() const>("getFieldOffset", &_::getFieldOffset)
        
        .protected_()
            .constructor<void(StringView, Signature*, size_t, Modifiers, uint)>()
            .constructor<void(ClassType*, size_t, StringView, StringView, Modifiers, uint)>()["0"]
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
        
        .protected_()
            .field("m_FieldOffset", &_::m_FieldOffset)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Signal")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
