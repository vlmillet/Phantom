#pragma once

// haunt {

#include "StdFunctionSignal.h"

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

#include "SmallVector.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StdFunctionSignal")

        PHANTOM_CLASS_T((class, class, size_t), (Signature, MutexT, t_StaticAllocCount), StdFunctionSignal)
        {
            this_()
            ;
        }
        PHANTOM_CLASS_TVS((class, class, size_t), (R, MutexT, t_StaticAllocCount), (class, Args), (R(Args...), MutexT, t_StaticAllocCount), StdFunctionSignal)
        {
            this_()
        .public_()
            /// missing symbol(s) reflection (std::function) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<StdFunction>("StdFunction")
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<StdFunctions>("StdFunctions")
        
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount> const&)>()
            .PHANTOM_T method<phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount>&(phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount> const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T constructor<void(phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount>&&)>()
            .PHANTOM_T method<phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount>&(phantom::StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount>&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<void(R (*)(Args...))>("connect", &_::connect)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<void(StdFunction const&)>("connect", &_::connect)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<void(StdFunction const&)>("disconnect", &_::disconnect)
            .PHANTOM_T method<void(Args...)>("operator()", &_::operator())
            ;
        }
    PHANTOM_END("StdFunctionSignal")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
