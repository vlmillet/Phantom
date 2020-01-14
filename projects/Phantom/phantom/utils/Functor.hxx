#pragma once

// haunt {

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
#include <phantom/enum_class>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/alias_template>

#include <phantom/template-only-push>

#include "DynDelegate.hxx"
#include "StringView.hxx"

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
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<_FunctorInternalType>().values({
            {"Delegate",_FunctorInternalType::Delegate},
            {"DynDelegate",_FunctorInternalType::DynDelegate},
            {"StdFunction",_FunctorInternalType::StdFunction}});
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS_TVS((class), (R), (class, Params), (R(Params...)), Functor)
        {
            using DynDelegateType = typedef_<PHANTOM_TYPENAME _::DynDelegateType>;
            using StringView = typedef_< phantom::StringView>;
            using ThisType = typedef_<PHANTOM_TYPENAME _::ThisType>;
            this_()
        .public_()
            .PHANTOM_T typedef_<ThisType>("ThisType")
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<DelegateType>("DelegateType")
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<StdFuncType>("StdFuncType")
            .PHANTOM_T typedef_<DynDelegateType>("DynDelegateType")
            // EnableIfCustomArg : nested alias templates not suppported
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(ThisType const&)>()
            .PHANTOM_T constructor<void(ThisType&&)>()
            .PHANTOM_T method<ThisType&(ThisType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<ThisType&(ThisType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(DelegateType const&)>()
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(DelegateType&&)>()
            .PHANTOM_T constructor<void(R (*)(Params...))>()
            .PHANTOM_T method<ThisType&(R (*)(Params...))>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(DelegateType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(DelegateType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(StdFuncType const&, void*)>()["nullptr"]
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(StdFuncType&&, void*)>()["nullptr"]
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(StdFuncType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(StdFuncType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T constructor<void(DynDelegateType const&)>()
            .PHANTOM_T constructor<void(DynDelegateType&&)>()
            .PHANTOM_T constructor<void(::phantom::lang::Function *)>()
            .PHANTOM_T constructor<void(void*, ::phantom::lang::Class *, ::phantom::lang::Method *)>()
            .PHANTOM_T constructor<void(void*, ::phantom::lang::Class *, StringView)>()
            .PHANTOM_T constructor<void(OpaqueDynDelegate)>()
            .PHANTOM_T method<ThisType&(DynDelegateType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<ThisType&(OpaqueDynDelegate const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<ThisType&(DynDelegateType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<bool() const>("isEmpty", &_::isEmpty)
            /// missing symbol(s) reflection (phantom::FunctorID) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<FunctorID() const>("getID", &_::getID)
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(ThisType const&) const>("operator==", &_::operator==)
            .PHANTOM_T method<bool(ThisType const&) const>("operator!=", &_::operator!=)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(DelegateType&)>("as", &_::as)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(StdFuncType&)>("as", &_::as)
            .PHANTOM_T method<bool(DynDelegateType&)>("as", &_::as)
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

// haunt }
