#pragma once

// haunt {

// clang-format off

#include "ExecutionContext.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ExecutionContext")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ExecutionContext)
        {
            this_()
        .public_()
            .staticMethod<void(ExecutionContext*)>("Push", &_::Push)
            .staticMethod<void()>("Pop", &_::Pop)
            .staticMethod<::phantom::lang::ExecutionContext *()>("Current", &_::Current)
            .constructor<void()>()
            .enum_<>().values({
                {"e_max_calls",_::e_max_calls}})
            .end()
            .method<void*()>("resultPointer", &_::resultPointer)
            .method<void(void*)>("pushResultPointer", &_::pushResultPointer)
            .method<void()>("popResultPointer", &_::popResultPointer)
            /// missing symbol(s) reflection (phantom::lang::Evaluable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Evaluable*, Type*, void*)>("pushTempDestruction", &_::pushTempDestruction)
            /// missing symbol(s) reflection (phantom::lang::Evaluable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Evaluable*)>("releaseTemporaries", &_::releaseTemporaries)
            .method<void()>("releaseTemporaries", &_::releaseTemporaries)
            .method<bool(Subroutine*, void**, size_t), pure_virtual>("call", &_::call)
            /// missing symbol(s) reflection (phantom::lang::Statement) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Statement*), pure_virtual>("setNextStatement", &_::setNextStatement)
            .method<byte*() const, pure_virtual>("getStackPointer", &_::getStackPointer)
            .method<byte*() const, pure_virtual>("getBasePointer", &_::getBasePointer)
            .method<void*() const, pure_virtual>("getReturnAddress", &_::getReturnAddress)
            .method<byte*() const, pure_virtual>("addressOfThis", &_::addressOfThis)
            .method<byte*(LocalVariable*) const, pure_virtual>("addressOf", &_::addressOf)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ExecutionContext")
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
