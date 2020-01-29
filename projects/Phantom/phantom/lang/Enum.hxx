#pragma once

// haunt {

#include "Enum.h"

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

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Pair.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Enum")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Enum)
        {
            using Constants = typedef_< phantom::lang::Constants>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::PrimitiveType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<PrimitiveType*()>("IntType", &_::IntType)
        
        .public_()
            .constructor<void()>()
            .constructor<void(PrimitiveType*)>()
            .constructor<void(StringView)>()
            .constructor<void(StringView, PrimitiveType*)>()
            .method<bool() const>("isScoped", &_::isScoped)
            .method<void()>("setScoped", &_::setScoped)
            .method<Constant*(size_t) const>("getConstant", &_::getConstant)
            .method<void(const void*, Constants&) const>("getConstants", &_::getConstants)
            .method<Constants const&() const>("getConstants", &_::getConstants)
            .method<Constant*(const void*) const>("getFirstConstant", &_::getFirstConstant)
            .method<Constant*(longlong) const>("getFirstConstant", &_::getFirstConstant)
            .method<void(void*) const>("getDefaultValue", &_::getDefaultValue)
            .method<Constant*() const>("getDefaultConstant", &_::getDefaultConstant)
            .method<size_t() const>("getConstantCount", &_::getConstantCount)
            .method<Constant*(StringView) const>("getConstant", &_::getConstant)
            .method<void(Constant*)>("addConstant", &_::addConstant)
            .method<void(StringView, int)>("addConstant", &_::addConstant)
            .method<void(StringView, ::phantom::ArrayView<Pair<StringView, int> >)>("addConstants", &_::addConstants)
            .method<void(StringView)>("addConstant", &_::addConstant)
            .method<void(Constant*)>("removeConstant", &_::removeConstant)
            .method<Enum*() const, virtual_|override_>("asEnum", &_::asEnum)
            .method<PrimitiveType*() const, virtual_|override_>("asIntegralType", &_::asIntegralType)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void(void*, Constants&) const>("findConstantsWithValue", &_::findConstantsWithValue)
            .method<PrimitiveType*() const>("getUnderlyingIntType", &_::getUnderlyingIntType)
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)
            .method<Type*() const, virtual_|override_>("getUnderlyingType", &_::getUnderlyingType)
            .method<Constant*(void*, StringView, PrimitiveType*) const, virtual_|override_>("createConstant", &_::createConstant)["\"\""]["nullptr"]
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Enum")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
