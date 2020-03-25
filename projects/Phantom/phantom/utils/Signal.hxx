#pragma once

// haunt {

// clang-format off

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
#include <phantom/function>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/thread/LockGuard.hxx>
#include "Functor.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Signal")

        PHANTOM_CLASS_T((class, class), (Sign, SignalMutexType), Signal)
        {
            this_()
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void*()>("Emitter", Emitter);}
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS_TVS((class), (SignalMutexType), (class, Parms), (void(Parms...), SignalMutexType), Signal)
        {
            using FunctorType = typedef_<PHANTOM_TYPENAME _::FunctorType>;
            using SignalLockGuardType = typedef_<PHANTOM_TYPENAME _::SignalLockGuardType>;
            using ThisType = typedef_<PHANTOM_TYPENAME _::ThisType>;
            this_()
            .PHANTOM_T inherits</* ::phantom::_Signal::SignalImpl<Signal<void (Parms...)>, typename _Signal::SignaturePopBack<void (Parms...)>::type, typename _Signal::SignaturePrepend<typename _Signal::SignatureBack<void (Parms...)>::type, void ()>::type> */>()
        .public_()
            .PHANTOM_T typedef_<SignalLockGuardType>("SignalLockGuardType")
            .PHANTOM_T typedef_<ThisType>("ThisType")
            /// missing symbol(s) reflection (phantom::_Signal::SignalImpl) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<BaseType>("BaseType")
            /// missing symbol(s) reflection (phantom::_Signal::Slot) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<SlotType>("SlotType")
            .PHANTOM_T typedef_<FunctorType>("FunctorType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<FunctorID(OpaqueDynDelegate&&)>("connect", &_::connect)
            .PHANTOM_T method<FunctorID(OpaqueDynDelegate const&)>("connect", &_::connect)
            .PHANTOM_T method<FunctorID(FunctorType const&)>("connect", &_::connect)
            .PHANTOM_T method<FunctorID(FunctorType&&)>("connect", &_::connect)
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<FunctorID(const Delegate<void (Parms...)>&)>("connect", &_::connect)
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<FunctorID(Delegate<void (Parms...)>&&)>("connect", &_::connect)
            .PHANTOM_T method<void(OpaqueDynDelegate&&)>("disconnect", &_::disconnect)
            .PHANTOM_T method<void(OpaqueDynDelegate const&)>("disconnect", &_::disconnect)
            .PHANTOM_T method<void(FunctorID)>("disconnect", &_::disconnect)
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<void(const Delegate<void (Parms...)>&)>("disconnect", &_::disconnect)
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<void(Delegate<void (Parms...)>&&)>("disconnect", &_::disconnect)
            .PHANTOM_T method<void(Parms...) const>("operator()", &_::operator())
            .PHANTOM_T method<void(void*, Parms...) const>("emit", &_::emit)
            .PHANTOM_T method<SignalMutexType&()>("mutex", &_::mutex)
            ;
        }
    PHANTOM_END("Signal")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
