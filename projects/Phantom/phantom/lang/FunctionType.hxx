#pragma once

// haunt {

// clang-format off

#include "FunctionType.h"

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
#include <phantom/field>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("FunctionType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FunctionType)
        {
            using EState = typedef_<_::EState>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .enum_<EState>().values({
                {"eState_Virgin",_::eState_Virgin},
                {"eState_UnderConstruction",_::eState_UnderConstruction},
                {"eState_Final",_::eState_Final}})
            .end()
            .constructor<void(Modifiers, uint)>()({"a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(Type*, Modifiers, uint)>()({"a_pReturnType","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(Type*, TypesView, Modifiers, uint)>()({"a_pType","a_Types","a_Modifiers","a_uiFlags"})["0"]["0"]
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<FunctionType*() const, virtual_|override_>("asFunctionType", &_::asFunctionType)
            .method<bool() const>("isRVOCandidate", &_::isRVOCandidate)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pOther"})
            .method<bool(FunctionType*) const>("isSame", &_::isSame)({"a_pOther"})
            .using_("Type::isSame")
            .method<bool(TypesView, Modifiers, uint) const>("matches", &_::matches)({"parameterTypes","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<bool(FunctionType*) const>("matches", &_::matches)({"a_pOther"})
            .method<void(StringView, LanguageElement*)>("parse", &_::parse)({"a_strFunctionType","a_pContextScope"})["nullptr"]
            .method<size_t() const>("getParameterTypeCount", &_::getParameterTypeCount)
            .method<Type*(size_t) const>("getParameterType", &_::getParameterType)({"i"})
            .method<void(Type*)>("addParameterType", &_::addParameterType)({"a_pType"})
            .method<TypesView() const>("getParameterTypes", &_::getParameterTypes)
            .method<void(Type*)>("setReturnType", &_::setReturnType)({"a_pType"})
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)({"a_Buf"})
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<bool() const>("hasEllipsis", &_::hasEllipsis)
            .method<::phantom::SmallVector<Type *, 4>::const_iterator() const>("beginParameterTypes", &_::beginParameterTypes)
            .method<::phantom::SmallVector<Type *, 4>::const_iterator() const>("endParameterTypes", &_::endParameterTypes)
        
        .protected_()
            .field("m_pReturnType", &_::m_pReturnType)
            .field("m_ParameterTypes", &_::m_ParameterTypes)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionType")
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
