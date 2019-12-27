#pragma once

// haunt {

#include "Array.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Array")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Array)
        {
            using AggregateFields = typedef_< phantom::reflection::AggregateFields>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .inherits<::phantom::reflection::ExtendedType, ::phantom::reflection::Aggregate>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(Type*, size_t)>()
        
        .public_()
            .method<bool() const, virtual_|override_>("isDefaultConstructible", &_::isDefaultConstructible)
            .method<Type*() const, virtual_|override_>("asPOD", &_::asPOD)
            .method<bool() const, virtual_|override_>("isPOD", &_::isPOD)
            .method<Array*() const, virtual_|override_>("asArray", &_::asArray)
            .method<size_t() const>("getItemCount", &_::getItemCount)
            .method<Type*() const>("getItemType", &_::getItemType)
            .method<void*(void*, size_t) const>("getItemAddress", &_::getItemAddress)
            .method<const void*(const void*, size_t) const>("getItemAddress", &_::getItemAddress)
            .method<void(const void*, size_t, void*) const>("getItemValue", &_::getItemValue)
            .method<void(void*, size_t, const void*) const>("setItemValue", &_::setItemValue)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<Type*() const, virtual_|override_>("addConst", &_::addConst)
            .method<Type*() const, virtual_|override_>("addVolatile", &_::addVolatile)
            .method<Type*() const, virtual_>("addConstVolatile", &_::addConstVolatile)
            .method<Type*() const, virtual_|override_>("addPointer", &_::addPointer)
            .method<Type*() const, virtual_|override_>("removePointerOrArray", &_::removePointerOrArray)
            .method<Type*() const, virtual_|override_>("removeArray", &_::removeArray)
            .method<Type*() const, virtual_|override_>("removeAllConst", &_::removeAllConst)
            .method<Type*() const, virtual_|override_>("removeAllQualifiers", &_::removeAllQualifiers)
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)
            .method<void(AggregateFields&) const, virtual_|override_>("getFlattenedAggregateFields", &_::getFlattenedAggregateFields)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
        
        .protected_()
            .method<size_t() const>("getElementCount", &_::getElementCount)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
        .protected_()
            .field("m_uiCount", &_::m_uiCount)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Array")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
