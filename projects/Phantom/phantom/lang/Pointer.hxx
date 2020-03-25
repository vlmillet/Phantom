#pragma once

// haunt {

// clang-format off

#include "Pointer.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Pointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Pointer)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::PointerType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(Type*)>()
        
        .public_()
            .method<Type*() const>("getPointeeType", &_::getPointeeType)
            .method<Type*() const, virtual_|override_>("asClassAddressType", &_::asClassAddressType)
            .method<Pointer*() const, virtual_|override_>("asClassPointer", &_::asClassPointer)
            .method<Type*() const, virtual_|override_>("asConstClassAddressType", &_::asConstClassAddressType)
            .method<Pointer*() const, virtual_|override_>("asConstClassPointer", &_::asConstClassPointer)
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToLiteral", &_::valueToLiteral)
            .method<uint() const, virtual_|override_>("getDataPointerLevel", &_::getDataPointerLevel)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool(Type*) const, virtual_|override_>("isEquivalent", &_::isEquivalent)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
            .method<void(void**, void*)>("assignment", &_::assignment)
            .method<void(void**, void*)>("preIncrement", &_::preIncrement)
            .method<void(void**, void*)>("postIncrement", &_::postIncrement)
            .method<void(void**, void*)>("preDecrement", &_::preDecrement)
            .method<void(void**, void*)>("postDecrement", &_::postDecrement)
            .method<void(void**, void*)>("dereference", &_::dereference)
            .method<void(void**, void*)>("plus", &_::plus)
            .method<void(void**, void*)>("add", &_::add)
            .method<void(void**, void*)>("addRev", &_::addRev)
            .method<void(void**, void*)>("bracket", &_::bracket)
            .method<void(void**, void*)>("bracketRev", &_::bracketRev)
            .method<void(void**, void*)>("subtract", &_::subtract)
            .method<void(void**, void*)>("subtractPointer", &_::subtractPointer)
            .method<void(void**, void*)>("assignmentAdd", &_::assignmentAdd)
            .method<void(void**, void*)>("assignmentSubtract", &_::assignmentSubtract)
            .using_("LanguageElement::getUniqueName")
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Pointer")
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
