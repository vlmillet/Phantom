#pragma once

// haunt {

// clang-format off

#include "Scope.h"

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

#include <phantom/template-only-push>

#include "LanguageElement.hxx"
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Scope")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Scope)
        {
            using Aliases = typedef_< phantom::lang::Aliases>;
            using AnonymousSections = typedef_< phantom::lang::AnonymousSections>;
            using Classes = typedef_< phantom::lang::Classes>;
            using Constants = typedef_< phantom::lang::Constants>;
            using Functions = typedef_< phantom::lang::Functions>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Subroutines = typedef_< phantom::lang::Subroutines>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using TemplateSpecializations = typedef_< phantom::lang::TemplateSpecializations>;
            using Templates = typedef_< phantom::lang::Templates>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using Variables = typedef_< phantom::lang::Variables>;
            this_()
        .protected_()
            .constructor<void(LanguageElement*, LanguageElement*)>()({"a_pThisElement","a_pUnit"})
            .method<void()>("initialize", &_::initialize)
            .method<void()>("terminate", &_::terminate)
        
        .public_()
            .method<::phantom::lang::Scope *() const, pure_virtual>("asScope", &_::asScope)
            .method<::phantom::lang::LanguageElement *() const>("asLanguageElement", &_::asLanguageElement)
            .method<::phantom::lang::Symbol *() const>("asSymbol", &_::asSymbol)
            .method<::phantom::lang::Namespace *() const>("asNamespace", &_::asNamespace)
            .method<bool(Symbol*) const>("isSame", &_::isSame)({"a_pSymbol"})
            .method<bool(LanguageElement*) const>("isSame", &_::isSame)({"a_pElement"})
            .method<bool(Scope*) const>("isSame", &_::isSame)({"a_pScope"})
            .method<::phantom::lang::Scope *() const, virtual_>("getNamedScope", &_::getNamedScope)
            .method<void(Type*)>("addType", &_::addType)({"a_pType"})
            .method<Types const&() const>("getTypes", &_::getTypes)
            .method<Type*(StringView) const>("getType", &_::getType)({"a_strName"})
            .method<PrimitiveType*(StringView) const>("getPrimitiveType", &_::getPrimitiveType)({"a_strName"})
            .method<Class*(StringView) const>("getClass", &_::getClass)({"a_strName"})
            .method<void(Template*)>("addTemplate", &_::addTemplate)({"a_pTemplate"})
            .method<Template*(StringView) const>("getTemplate", &_::getTemplate)({"a_strName"})
            .method<Templates const&() const>("getTemplates", &_::getTemplates)
            .method<void(TemplateSpecialization*)>("addTemplateSpecialization", &_::addTemplateSpecialization)({"a_pTemplateSpecialization"})
            .method<TemplateSpecialization*(Template*, TemplateSignature*, const LanguageElements&)>("addTemplateSpecialization", &_::addTemplateSpecialization)({"a_pTemplate","a_pTemplateSignature","a_Arguments"})
            .method<TemplateSpecialization*(Template*, TemplateSignature*, const LanguageElements&, Symbol*)>("addTemplateSpecialization", &_::addTemplateSpecialization)({"a_pTemplate","a_pTemplateSignature","a_Arguments","a_pBody"})
            .method<TemplateSpecialization*(TemplateSpecialization*, const LanguageElements&, const TemplateSubstitution&)>("addTemplateInstantiation", &_::addTemplateInstantiation)({"a_pInstantiationSpecialization","a_Arguments","a_Substitutions"})
            .method<TemplateSpecializations const&() const>("getTemplateSpecializations", &_::getTemplateSpecializations)
            .method<TemplateSpecialization*(StringView, const LanguageElements&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)({"a_strName","a_Arguments"})
            .method<void(Variable*)>("addVariable", &_::addVariable)({"a_pVariable"})
            .method<Variable*(StringView) const>("getVariable", &_::getVariable)({"a_strName"})
            .method<Variables const&() const>("getVariables", &_::getVariables)
            .method<void(Constant*)>("addConstant", &_::addConstant)({"a_pConstant"})
            .method<Constant*(StringView) const>("getConstant", &_::getConstant)({"a_strName"})
            .method<Constants const&() const>("getConstants", &_::getConstants)
            .method<Alias*(Symbol*, StringView, Modifiers, uint)>("addAlias", &_::addAlias)({"a_pSymbol","a_strAlias","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void(Alias*)>("addAlias", &_::addAlias)({"a_pAlias"})
            .method<Template*(StringView, StringView, StringView, StringView, Modifiers, uint)>("addAliasTemplate", &_::addAliasTemplate)({"a_TemplateTypes","a_TemplateParams","a_strAliasName","a_TemplateDependantType","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<Template*(TemplateSignature*, StringView, StringView, Modifiers, uint)>("addAliasTemplate", &_::addAliasTemplate)({"a_pSignature","a_strAliasName","a_TemplateDependantType","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void(Symbol*)>("addUsing", &_::addUsing)({"a_pElement"})
            .method<Alias*(StringView) const>("getAlias", &_::getAlias)({"a_strName"})
            .method<Aliases const&() const>("getAliases", &_::getAliases)
            .method<LanguageElement*(StringView) const>("getAliased", &_::getAliased)({"a_strAlias"})
            .method<Alias*(StringView) const>("getTypedef", &_::getTypedef)({"a_strTypedef"})
            .method<void(Function*)>("addFunction", &_::addFunction)({"a_pFunction"})
            .method<Functions const&() const>("getFunctions", &_::getFunctions)
            .method<Function*(StringView, TypesView) const>("getFunction", &_::getFunction)({"a_strName","a_Types"})
            .method<void(Functions&, StringView, const Types*, Type*) const>("findFunctions", &_::findFunctions)({"a_Functions","a_Name","a_pParameterTypes","a_pReturnType"})["StringView()"]["nullptr"]["nullptr"]
            .method<void(AnonymousSection*), virtual_>("addAnonymousSection", &_::addAnonymousSection)({"a_pAnonymousSection"})
            .method<AnonymousSections const&() const>("getAnonymousSections", &_::getAnonymousSections)
            .method<::phantom::lang::Subroutine *(StringView, TypesView, Modifiers) const, virtual_>("getSubroutine", &_::getSubroutine)({"a_strName","a_ParameterTypes","a_Modifiers"})["0"]
            .method<Subroutine*(Type*, StringView, TypesView, Modifiers) const>("getSubroutine", &_::getSubroutine)({"a_pReturnType","a_strName","a_ParameterTypes","a_Modifiers"})["0"]
            .method<void(Subroutine*), virtual_>("addSubroutine", &_::addSubroutine)({"a_pSubroutine"})
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_>("acceptsSubroutine", &_::acceptsSubroutine)({"a_pReturnType","a_strName","a_Types","a_Modifiers","a_uiFlags","a_pOutConflictingSubroutines"})["0"]["0"]["nullptr"]
            .method<bool(Subroutine*, Subroutines*) const>("acceptsSubroutine", &_::acceptsSubroutine)({"a_pSubroutine","a_pOutConflictingSubroutines"})["nullptr"]
            .method<void(Symbol*, Symbols&) const, virtual_>("getElementDoubles", &_::getElementDoubles)({"a_pElement","out"})
            .method<void(Types&) const>("fetchTypesCascade", &_::fetchTypesCascade)({"a_Types"})
            .method<bool(Symbol*) const>("isSymbolHidden", &_::isSymbolHidden)({"a_pSymbol"})
            .method<void(StringView, Symbols&) const, virtual_>("getScopedSymbolsWithName", &_::getScopedSymbolsWithName)({"a_Name","a_Symbols"})
            .method<void(Classes&, Class*, bool) const>("findClasses", &_::findClasses)({"a_Classes","a_pBaseClass","a_bDefaultInstanciable"})["nullptr"]["false"]
            .method<void(Alias*)>("removeAlias", &_::removeAlias)({"a_pAlias"})
            .method<void(Type*)>("removeType", &_::removeType)({"a_pType"})
            .method<void(Function*)>("removeFunction", &_::removeFunction)({"a_pFunction"})
            .method<void(Variable*)>("removeVariable", &_::removeVariable)({"a_pVariable"})
            .method<void(Constant*)>("removeConstant", &_::removeConstant)({"a_pConstant"})
            .method<void(Template*)>("removeTemplate", &_::removeTemplate)({"a_pTemplate"})
            .method<void(TemplateSpecialization*)>("removeTemplateSpecialization", &_::removeTemplateSpecialization)({"a_pTemplateSpecialization"})
        
        .protected_()
            .method<void(Symbol*), pure_virtual>("onScopeSymbolAdded", &_::onScopeSymbolAdded)({"a_pElement"})
            .method<void(Symbol*), pure_virtual>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)({"a_pElement"})
        
        .protected_()
            .field("m_pThisElement", &_::m_pThisElement)
            .field("m_pUnit", &_::m_pUnit)
            .field("m_Functions", &_::m_Functions)
            .field("m_Variables", &_::m_Variables)
            .field("m_Constants", &_::m_Constants)
            .field("m_Types", &_::m_Types)
            .field("m_Templates", &_::m_Templates)
            .field("m_TemplateSpecializations", &_::m_TemplateSpecializations)
            .field("m_AnonymousSections", &_::m_AnonymousSections)
            .field("m_Aliases", &_::m_Aliases)
        
        .public_()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Scope")
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
