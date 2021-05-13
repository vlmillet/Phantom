#pragma once

// haunt {

// clang-format off

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

#include <phantom/utils/Functor.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctorProperty)
        {
            using GetFunctor = typedef_<_::GetFunctor>;
            using SetFunctor = typedef_<_::SetFunctor>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::ValueMember>()
        .public_()
            .typedef_<SetFunctor>("SetFunctor")
            .typedef_<GetFunctor>("GetFunctor")
            .constructor<void(Type*, StringView, uint)>()({"a_pType","a_Name","a_uiFilterMask"})["PHANTOM_R_FILTER_PROPERTY"]
            .constructor<void(Type*, StringView, GetFunctor const&, SetFunctor const&, uint)>()({"a_pType","a_Name","a_Get","a_Set","a_uiFilterMask"})["PHANTOM_R_FILTER_PROPERTY"]
            .constructor<void(Type*, StringView, GetFunctor const&, uint)>()({"a_pType","a_Name","a_Get","a_uiFilterMask"})["PHANTOM_R_FILTER_PROPERTY"]
            .method<SetFunctor const&() const>("getSet", &_::getSet)
            .method<GetFunctor const&() const>("getGet", &_::getGet)
            .method<void(SetFunctor const&)>("setSet", &_::setSet)({"a_Set"})
            .method<void(GetFunctor const&)>("setGet", &_::setGet)({"a_Get"})
            .method<void(void const*, void*) const, virtual_|override_>("getValue", &_::getValue)({"a_pObject","a_pDest"})
            .method<void(void*, void const*) const, virtual_|override_>("setValue", &_::setValue)({"a_pObject","a_pSrc"})
            .method<bool() const, virtual_|override_>("isWrittable", &_::isWrittable)
            .method<bool() const, virtual_|override_>("isReadable", &_::isReadable)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
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
