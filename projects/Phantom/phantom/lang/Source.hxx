#pragma once

// haunt {

// clang-format off

#include "Source.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Source")

        PHANTOM_STRUCT_T((class), (T), ObjectDtor)
        {
            this_()
            .PHANTOM_T staticMethod<void(void*)>("apply", &_::apply)({"a_pBase"})
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Source)
        {
            using Import = typedef_<_::Import>;
            using Imports = typedef_<_::Imports>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol, ::phantom::lang::Scope>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .struct_<Import>()
                .field("symbol", &_::Import::symbol)
                .field("isPublic", &_::Import::isPublic)
                .field("isStatic", &_::Import::isStatic)
                .field("alias", &_::Import::alias)
            .end()
            .typedef_<Imports>("Imports")
        
        .public_()
            .enum_<>().values({
                {"InvalidVersionNumber",_::InvalidVersionNumber}})
            .end()
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<void*(Class*, void*)>("PlacementInit", &_::PlacementInit)({"a_pClass","a_pInstance"})
            .method<void(LanguageElement*)>("Delete", &_::Delete)({"a_pElem"})
            .method<Module*() const>("getModule", &_::getModule)
            .method<Package*() const>("getPackage", &_::getPackage)
            .method<FunctionType*(Type*, TypesView, Modifiers, uint)>("functionType", &_::functionType)({"a_pReturnType","a_ParameterTypes","a_Modifiers","a_Flags"})["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(Type*, ABI, TypesView, Modifiers, uint)>("functionPointerType", &_::functionPointerType)({"a_pReturnType","a_eABI","a_ParameterTypes","a_Qualifiers","a_Flags"})["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(FunctionType*, ABI, Modifiers, uint)>("functionPointerType", &_::functionPointerType)({"a_pFunctionType","a_eABI","a_Qualifiers","a_Flags"})["0"]["0"]
            .method<InitializerListType*(TypesView)>("initializerListType", &_::initializerListType)({"a_Types"})
            .method<MethodPointer*(ClassType*, Type*, TypesView, Modifiers, uint)>("methodPointerType", &_::methodPointerType)({"a_pObjectType","a_pReturnType","a_ParameterTypes","a_RefQualifiers","a_uiFlags"})["0"]["0"]
            .method<FieldPointer*(ClassType*, Type*, Modifiers, uint)>("fieldPointerType", &_::fieldPointerType)({"a_pObjectType","a_pValueType","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void(SourceStream*)>("setSourceStream", &_::setSourceStream)({"a_pStream"})
            .method<SourceStream*() const>("getSourceStream", &_::getSourceStream)
            .method<Imports const&() const>("getImports", &_::getImports)
            .method<void(Symbols&) const>("getImported", &_::getImported)({"a_Imports"})
            .method<bool(Symbol*, bool, bool)>("addImport", &_::addImport)({"a_pSource","a_bStatic","a_bPublic"})["false"]["false"]
            .method<bool(StringView, bool, bool)>("addImport", &_::addImport)({"a_strName","a_bStatic","a_bPublic"})["false"]["false"]
            .method<bool(Symbol*) const>("hasImport", &_::hasImport)({"a_pSource"})
            .method<bool(Symbol*) const>("hasImported", &_::hasImported)({"a_pSource"})
            .method<bool(Symbol*, Access, Modifiers, uint, SmallMap<Symbol*, Symbols>*) const>("canImport", &_::canImport)({"a_pSymbol","a_eAccess","a_Modifiers","a_uiFlags","a_pCollidingSymbols"})["Access::Public"]["0"]["0"]["nullptr"]
            .method<Source*() const, virtual_|override_>("asSource", &_::asSource)
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)({"a_Buf"})
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<Source*() const, virtual_|override_>("getCodeLocationSource", &_::getCodeLocationSource)
            .method<CustomAllocator const*() const, virtual_|override_>("getAllocator", &_::getAllocator)
            .method<void(Source*)>("addDependency", &_::addDependency)({"a_pSource"})
            .method<ArrayView<Source*>() const>("getDependencies", &_::getDependencies)
        
        .protected_()
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolAdded", &_::onScopeSymbolAdded)({"a_pSymbol"})
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)({"a_pSymbol"})
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
        
        .public_()
            .field("sourceStreamChanged", &_::sourceStreamChanged)
            .field("buildSucceeded", &_::buildSucceeded)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Source")
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
