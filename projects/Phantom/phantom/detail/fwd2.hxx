#pragma once

// haunt {

#include "fwd2.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("State")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (phantom::State) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<States>("States"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("State")
    PHANTOM_SOURCE("Track")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (phantom::Track) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<Tracks>("Tracks"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Track")
PHANTOM_END("phantom")
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("fwd2")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(NullMutex)
        {
            this_()
            .method<void()>("lock", &_::lock)
            .method<void()>("unlock", &_::unlock)
            .method<bool()>("try_lock", &_::try_lock)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("fwd2")
PHANTOM_END("phantom")
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Alias")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Aliases>("Aliases"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Alias")
    PHANTOM_SOURCE("Class")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Classes>("Classes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Class")
    PHANTOM_SOURCE("ValueMember")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ValueMembers>("ValueMembers"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ValueMember")
    PHANTOM_SOURCE("LanguageElementVisitor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<LanguageElementVisitors>("LanguageElementVisitors"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElementVisitor")
    PHANTOM_SOURCE("Scope")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Scopes>("Scopes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Scope")
    PHANTOM_SOURCE("Type")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Types>("Types"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Type")
    PHANTOM_SOURCE("Callable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Callables>("Callables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Callable")
    PHANTOM_SOURCE("Enum")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Enums>("Enums"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Enum")
    PHANTOM_SOURCE("ClassType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ClassTypes>("ClassTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ClassType")
    PHANTOM_SOURCE("Structure")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Structures>("Structures"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Structure")
    PHANTOM_SOURCE("Union")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Unions>("Unions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Union")
    PHANTOM_SOURCE("VirtualMethodTable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<VirtualMethodTables>("VirtualMethodTables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VirtualMethodTable")
    PHANTOM_SOURCE("Namespace")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Namespaces>("Namespaces"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Namespace")
    PHANTOM_SOURCE("TemplateSpecialization")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<TemplateSpecializations>("TemplateSpecializations"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateSpecialization")
    PHANTOM_SOURCE("TemplateParameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<TemplateParameters>("TemplateParameters"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameter")
    PHANTOM_SOURCE("Template")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Templates>("Templates"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Template")
    PHANTOM_SOURCE("Symbol")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Symbols>("Symbols"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Symbol")
    PHANTOM_SOURCE("Instruction")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Instructions>("Instructions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Instruction")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctorPropertys>("FunctorPropertys"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
    PHANTOM_SOURCE("Aggregate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Aggregates>("Aggregates"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Aggregate")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctorProperties>("FunctorProperties"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<LanguageElementsView>("LanguageElementsView"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<TypesView>("TypesView"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.reflection")
}
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("fwd2")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(FunctorID)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(size_t, size_t)>()
            .constructor<void(void*, void*)>()
            .method<bool(FunctorID const&) const>("operator==", &_::operator==)
            .method<bool(FunctorID const&) const>("operator!=", &_::operator!=)
            .method<bool(FunctorID) const>("operator<", &_::operator<)
            .method<bool() const>("isNull", &_::isNull)
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("fwd2")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
