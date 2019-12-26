#pragma once

// haunt {

#include "FunctorProperty.h"

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
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/Functor.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctorProperty)
        {
            using GetFunctor = typedef_<_::GetFunctor>;
            using SetFunctor = typedef_<_::SetFunctor>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::ValueMember>()
        .public_()
            .typedef_<SetFunctor>("SetFunctor")
            .typedef_<GetFunctor>("GetFunctor")
            .constructor<void(Type*, StringView, uint)>()["PHANTOM_R_FILTER_PROPERTY"]
            .constructor<void(Type*, StringView, GetFunctor const&, SetFunctor const&, uint)>()["PHANTOM_R_FILTER_PROPERTY"]
            .constructor<void(Type*, StringView, GetFunctor const&, uint)>()["PHANTOM_R_FILTER_PROPERTY"]
            .method<SetFunctor const&() const>("getSet", &_::getSet)
            .method<GetFunctor const&() const>("getGet", &_::getGet)
            .method<void(SetFunctor const&)>("setSet", &_::setSet)
            .method<void(GetFunctor const&)>("setGet", &_::setGet)
            .method<void(void const*, void*) const, virtual_|override_>("getValue", &_::getValue)
            .method<void(void*, void const*) const, virtual_|override_>("setValue", &_::setValue)
            .method<bool() const, virtual_|override_>("isWrittable", &_::isWrittable)
            .method<bool() const, virtual_|override_>("isReadable", &_::isReadable)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
