#pragma once

// haunt {

// clang-format off

#include "Aggregate.h"

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
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Aggregate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(AggregateField)
        {
            this_()
            .field("type", &_::type)
            .field("offset", &_::offset)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<AggregateFields>("AggregateFields"); }
        PHANTOM_CLASS(Aggregate)
        {
            this_()
        .protected_()
            .constructor<void(Type*)>()({"a_pThisType"})
        
        .public_()
            .method<void(AggregateFields&) const, pure_virtual>("getFields", &_::getFields)({"_fields"})
            .method<void(AggregateFields&) const>("getFlattenedFields", &_::getFlattenedFields)({"_flattenedfields"})
            .method<Type*() const>("asType", &_::asType)
            .method<Class*() const>("asClass", &_::asClass)
            .method<Array*() const>("asArray", &_::asArray)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Aggregate")
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
