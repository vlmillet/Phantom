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
#include <phantom/enum_class>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/alias_template>

#include <phantom/template-only-push>

#include "Delegate.hxx"
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
            using DelegateType = typedef_<PHANTOM_TYPENAME _::DelegateType>;
            using DynDelegateType = typedef_<PHANTOM_TYPENAME _::DynDelegateType>;
            using StringView = typedef_< phantom::StringView>;
            using ThisType = typedef_<PHANTOM_TYPENAME _::ThisType>;
            this_()
        .public_()
            .PHANTOM_T typedef_<ThisType>("ThisType")
            .PHANTOM_T typedef_<DelegateType>("DelegateType")
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<StdFuncType>("StdFuncType")
            .PHANTOM_T typedef_<DynDelegateType>("DynDelegateType")
            // EnableIfCustomArg : nested alias templates not suppported
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(ThisType const&)>()({"a_Other"})
            .PHANTOM_T constructor<void(ThisType&&)>()({"a_Temp"})
            .PHANTOM_T method<ThisType&(ThisType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .PHANTOM_T method<ThisType&(ThisType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Temp"})
            .PHANTOM_T constructor<void(DelegateType const&)>()({"a_In"})
            .PHANTOM_T constructor<void(DelegateType&&)>()({"a_In"})
            .PHANTOM_T constructor<void(R (*)(Params...))>()({"a_Func"})
            .PHANTOM_T method<ThisType&(R (*)(Params...))>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<ThisType&(DelegateType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<ThisType&(DelegateType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(StdFuncType const&, void*)>()({"a_In","a_pLambdaDiscriminant"})["nullptr"]
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T constructor<void(StdFuncType&&, void*)>()({"a_In","a_pLambdaDiscriminant"})["nullptr"]
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(StdFuncType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<ThisType&(StdFuncType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T constructor<void(DynDelegateType const&)>()({"a_In"})
            .PHANTOM_T constructor<void(DynDelegateType&&)>()({"a_In"})
            .PHANTOM_T constructor<void(::phantom::lang::Function *)>()({"a_pFunction"})
            .PHANTOM_T constructor<void(void*, ::phantom::lang::Class *, ::phantom::lang::Method *)>()({"a_pInstance","a_pClass","a_pMethod"})
            .PHANTOM_T constructor<void(void*, ::phantom::lang::Class *, StringView)>()({"a_pInstance","a_pClass","a_MethodName"})
            .PHANTOM_T constructor<void(OpaqueDynDelegate)>()({"a_OpaqueDynDelegate"})
            .PHANTOM_T method<phantom::Functor<R(Params...)>&(OpaqueDynDelegate)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_OpaqueDynDelegate"})
            .PHANTOM_T method<ThisType&(DynDelegateType const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<ThisType&(OpaqueDynDelegate const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<ThisType&(DynDelegateType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_In"})
            .PHANTOM_T method<bool() const>("isEmpty", &_::isEmpty)
            .PHANTOM_T method<FunctorID() const>("getID", &_::getID)
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())({"a_Args"})
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(ThisType const&) const>("operator==", &_::operator==)({"a_Other"})
            .PHANTOM_T method<bool(ThisType const&) const>("operator!=", &_::operator!=)({"a_Other"})
            .PHANTOM_T method<bool(DelegateType&)>("as", &_::as)({"a_Out"})
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(StdFuncType&)>("as", &_::as)({"a_Out"})
            .PHANTOM_T method<bool(DynDelegateType&)>("as", &_::as)({"a_Out"})
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
