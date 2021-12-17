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
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .constructor<void(Type*)>()({"a_pPointeeType"})
            .method<Type*() const>("getPointeeType", &_::getPointeeType)
            .method<Type*() const, virtual_|override_>("asClassAddressType", &_::asClassAddressType)
            .method<Pointer*() const, virtual_|override_>("asClassPointer", &_::asClassPointer)
            .method<Type*() const, virtual_|override_>("asConstClassAddressType", &_::asConstClassAddressType)
            .method<Pointer*() const, virtual_|override_>("asConstClassPointer", &_::asConstClassPointer)
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)({"a_pDstType","a_pDst","a_pSrc"})
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)({"a_str","dest"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)({"a_Buf","src"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToLiteral", &_::valueToLiteral)({"a_Buf","src"})
            .method<uint() const, virtual_|override_>("getDataPointerLevel", &_::getDataPointerLevel)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool(Type*) const, virtual_|override_>("isEquivalent", &_::isEquivalent)({"a_pType"})
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)({"a_pType","a_Score","a_DeducedConstants"})
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pOther"})
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)({"a_pSource"})
            .method<void(void**, void*)>("assignment", &_::assignment)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("preIncrement", &_::preIncrement)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("postIncrement", &_::postIncrement)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("preDecrement", &_::preDecrement)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("postDecrement", &_::postDecrement)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("dereference", &_::dereference)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("plus", &_::plus)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("add", &_::add)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("addRev", &_::addRev)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("bracket", &_::bracket)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("bracketRev", &_::bracketRev)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("subtract", &_::subtract)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("subtractPointer", &_::subtractPointer)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("assignmentAdd", &_::assignmentAdd)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("assignmentSubtract", &_::assignmentSubtract)({"a_pArgs","a_pOutput"})
            .using_("LanguageElement::getUniqueName")
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<hash64() const, virtual_|override_>("computeLocalHash", &_::computeLocalHash)
            .method<LanguageElement*() const, virtual_|override_>("getNamingScope", &_::getNamingScope)
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
