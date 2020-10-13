#pragma once

// haunt {

// clang-format off

#include "Main.h"

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
#include <phantom/enum_class>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/Functor.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.lang.registration")
    PHANTOM_SOURCE("Main")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MessageReportFunc>("MessageReportFunc"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<LogFunc>("LogFunc"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Main")
PHANTOM_END("phantom.lang.registration")
namespace lang {
PHANTOM_PACKAGE("phantom.lang.registration")
    PHANTOM_SOURCE("Main")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<ClassHookOp>().values({
            {"InstanceRegistered",ClassHookOp::InstanceRegistered},
            {"InstanceUnregistering",ClassHookOp::InstanceUnregistering},
            {"KindCreated",ClassHookOp::KindCreated},
            {"KindDestroying",ClassHookOp::KindDestroying},
            {"ClassDestroying",ClassHookOp::ClassDestroying}});
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ClassHookFunc>("ClassHookFunc"); }
        PHANTOM_CLASS(Main)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .staticMethod<::phantom::lang::Main *()>("Get", &_::Get)
            .constructor<void(int (*)(int, char**), StringView, int, char**, CustomAllocator, ClassHookFunc, StringView, uint)>()["0"]["nullptr"]["CustomAllocator::Default()"]["ClassHookFunc()"]["\"\""]["0"]
            .constructor<void(int (*)(), StringView, int, char**, CustomAllocator, ClassHookFunc, StringView, uint)>()["0"]["nullptr"]["CustomAllocator::Default()"]["ClassHookFunc()"]["\"\""]["0"]
            .constructor<void(void*, StringView, int, char**, CustomAllocator, ClassHookFunc, StringView, uint)>()["0"]["nullptr"]["CustomAllocator::Default()"]["ClassHookFunc()"]["\"\""]["0"]
            .constructor<void(size_t, StringView, int, char**, CustomAllocator, ClassHookFunc, StringView, uint)>()["0"]["nullptr"]["CustomAllocator::Default()"]["ClassHookFunc()"]["\"\""]["0"]
            .method<void(MessageReportFunc)>("setAssertFunc", &_::setAssertFunc)
            .method<void(MessageReportFunc)>("setErrorFunc", &_::setErrorFunc)
            .method<void(LogFunc const&)>("setLogFunc", &_::setLogFunc)
            .method<void(MessageReportFunc)>("setWarningFunc", &_::setWarningFunc)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Main")
PHANTOM_END("phantom.lang.registration")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
