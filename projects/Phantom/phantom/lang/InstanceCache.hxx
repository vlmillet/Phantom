#pragma once

// haunt {

#include "InstanceCache.h"

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("InstanceCache")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(InstanceCache)
        {
            using VoidPtrs = typedef_< phantom::VoidPtrs>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        
        .protected_()
            .constructor<void(Class*)>()
        
        .public_()
            .method<VoidPtrs const&() const>("getInstances", &_::getInstances)
        
        .protected_()
            .field("m_Instances", &_::m_Instances)
            .field("m_pClass", &_::m_pClass)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("InstanceCache")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
