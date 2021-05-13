#pragma once

// haunt {

// clang-format off

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
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<::phantom::lang::PrimitiveType *()>("IntType", &_::IntType)
        
        .public_()
            .constructor<void()>()
            .constructor<void(PrimitiveType*)>()({"a_pIntType"})
            .constructor<void(StringView)>()({"a_strName"})
            .constructor<void(StringView, PrimitiveType*)>()({"a_strName","a_pIntType"})
            .method<bool() const>("isScoped", &_::isScoped)
            .method<void()>("setScoped", &_::setScoped)
            .method<::phantom::lang::Constant *(size_t) const>("getConstant", &_::getConstant)({"i"})
            .method<void(const void*, Constants&) const>("getConstants", &_::getConstants)({"a_pValue","a_Constants"})
            .method<Constants const&() const>("getConstants", &_::getConstants)
            .method<Constant*(const void*) const>("getFirstConstant", &_::getFirstConstant)({"a_pValue"})
            .method<Constant*(longlong) const>("getFirstConstant", &_::getFirstConstant)({"a_Value"})
            .method<void(void*) const>("getDefaultValue", &_::getDefaultValue)({"a_pBuffer"})
            .method<Constant*() const>("getDefaultConstant", &_::getDefaultConstant)
            .method<size_t() const>("getConstantCount", &_::getConstantCount)
            .method<Constant*(StringView) const>("getConstant", &_::getConstant)({"a_strKey"})
            .method<void(Constant*)>("addConstant", &_::addConstant)({"a_pConstant"})
            .method<void(StringView, int)>("addConstant", &_::addConstant)({"a_Name","a_Value"})
            .method<void(StringView, ::phantom::ArrayView<Pair<StringView, int> >)>("addConstants", &_::addConstants)({"a_Name","a_Values"})
            .method<void(StringView)>("addConstant", &_::addConstant)({"a_strCode"})
            .method<PrimitiveType*() const, virtual_|override_>("asIntegralType", &_::asIntegralType)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)({"a_strIn","a_pDest"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)({"a_Buf","a_pSrc"})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)({"n"})
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)({"a_pInstance","n"})
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)({"a_pInstance"})
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)({"a_pInstance"})
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)({"a_pInstance"})
            .method<void(void*, Constants&) const>("findConstantsWithValue", &_::findConstantsWithValue)({"a_pSrc","out"})
            .method<PrimitiveType*() const>("getUnderlyingIntType", &_::getUnderlyingIntType)
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)({"a_pDstType","a_pDst","a_pSrc"})
            .using_("PrimitiveType::createConstant")
            .method<Constant*(LanguageElement*, void*, StringView, PrimitiveType*) const, virtual_|override_>("createConstant", &_::createConstant)({"a_pOwner","a_pSrc","a_strName","a_pPrimitiveType"})["\"\""]["nullptr"]
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
        
        .protected_()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()({"a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
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

// clang-format on

// haunt }
