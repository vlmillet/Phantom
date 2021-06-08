#pragma once

// haunt {

// clang-format off

#include "Functor.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/alias_template>

#include <phantom/template-only-push>

#include "Delegate.hxx"
#include "DynDelegate.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Functor")

        PHANTOM_CLASS_T((class), (S), Functor)
        {
            this_()
            ;
        }
        PHANTOM_STRUCT_T((class), (T), IsFunctor)
        {
            this_()
            .PHANTOM_T inherits</* ::std::false_type */>()
            ;
        }
        PHANTOM_STRUCT_TVS((class), (R), (class, Params), (Functor<R(Params...)>), IsFunctor)
        {
            this_()
            .PHANTOM_T inherits</* ::std::true_type */>()
            ;
        }
        PHANTOM_STRUCT_T((class), (T), _Functor_proxy_funcs)
        {
            this_()
            .PHANTOM_T method<void()>("destructor", &_::destructor)
            .PHANTOM_T method<void*(void*, bool)>("copyOrMoveConstructor", &_::copyOrMoveConstructor)({"_mem","_move"})
            ;
        }
        PHANTOM_CLASS_TVS((class), (R), (class, Params), (R(Params...)), Functor)
        {
            using DelegateT = typedef_<PHANTOM_TYPENAME _::DelegateT>;
            using DynDelegateT = typedef_<PHANTOM_TYPENAME _::DynDelegateT>;
            using ThisType = typedef_<PHANTOM_TYPENAME _::ThisType>;
            this_()
        .public_()
            .PHANTOM_T typedef_<ThisType>("ThisType")
            .PHANTOM_T typedef_<DelegateT>("DelegateT")
            .PHANTOM_T typedef_<DynDelegateT>("DynDelegateT")
            // EnableIfCustomArg : nested alias templates not suppported
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(std::nullptr_t)>()({""})
            .PHANTOM_T constructor<void(ThisType const&)>()({"_other"})
            .PHANTOM_T constructor<void(ThisType&&)>()({"_other"})
            .PHANTOM_T method<ThisType&(ThisType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T method<ThisType&(ThisType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T constructor<void(DelegateT const&)>()({"_dgt"})
            .PHANTOM_T constructor<void(DelegateT&&)>()({"_dgt"})
            .PHANTOM_T method<ThisType&(DelegateT const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T method<ThisType&(DelegateT&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T constructor<void(DynDelegateT const&)>()({"_dgt"})
            .PHANTOM_T constructor<void(DynDelegateT&&)>()({"_dgt"})
            .PHANTOM_T method<ThisType&(DynDelegateT const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T method<ThisType&(DynDelegateT&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T constructor<void(OpaqueDynDelegate const&)>()({"_dgt"})
            .PHANTOM_T constructor<void(OpaqueDynDelegate&&)>()({"_dgt"})
            .PHANTOM_T method<ThisType&(OpaqueDynDelegate const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T method<ThisType&(OpaqueDynDelegate&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .PHANTOM_T constructor<void(R (*)(Params...))>()({"a_In"})
            .PHANTOM_T method<ThisType&(R (*)(Params...))>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())({"Args"})
            .PHANTOM_T method<bool(ThisType const&) const>("operator==", &_::operator==)({"_other"})
            .PHANTOM_T method<bool(ThisType const&) const>("operator!=", &_::operator!=)({"_other"})
            .PHANTOM_T method<bool(std::nullptr_t) const>("operator==", &_::operator==)({""})
            .PHANTOM_T method<bool(std::nullptr_t) const>("operator!=", &_::operator!=)({""})
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<FunctorID() const>("getID", &_::getID)
            ;
        }
    PHANTOM_END("Functor")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
