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
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
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
            .constructor<void(LanguageElement*, LanguageElement*)>()
        
        .public_()
            .method<Scope*() const, pure_virtual>("asScope", &_::asScope)
            .method<LanguageElement*() const>("asLanguageElement", &_::asLanguageElement)
            .method<Symbol*() const>("asSymbol", &_::asSymbol)
            .method<Namespace*() const>("asNamespace", &_::asNamespace)
            .method<bool(Symbol*) const>("isSame", &_::isSame)
            .method<bool(LanguageElement*) const>("isSame", &_::isSame)
            .method<bool(Scope*) const>("isSame", &_::isSame)
            .method<Scope*() const, virtual_>("getNamedScope", &_::getNamedScope)
            .method<void(Type*)>("addType", &_::addType)
            .method<Types const&() const>("getTypes", &_::getTypes)
            .method<Type*(StringView) const>("getType", &_::getType)
            .method<PrimitiveType*(StringView) const>("getPrimitiveType", &_::getPrimitiveType)
            .method<Class*(StringView) const>("getClass", &_::getClass)
            .method<void(Template*)>("addTemplate", &_::addTemplate)
            .method<Template*(StringView) const>("getTemplate", &_::getTemplate)
            .method<Templates const&() const>("getTemplates", &_::getTemplates)
            .method<void(TemplateSpecialization*)>("addTemplateSpecialization", &_::addTemplateSpecialization)
            .method<TemplateSpecialization*(Template*, TemplateSignature*, const LanguageElements&)>("addTemplateSpecialization", &_::addTemplateSpecialization)
            .method<TemplateSpecialization*(Template*, TemplateSignature*, const LanguageElements&, Symbol*)>("addTemplateSpecialization", &_::addTemplateSpecialization)
            .method<TemplateSpecialization*(TemplateSpecialization*, const LanguageElements&, const PlaceholderMap&)>("addTemplateInstantiation", &_::addTemplateInstantiation)
            .method<TemplateSpecializations const&() const>("getTemplateSpecializations", &_::getTemplateSpecializations)
            .method<TemplateSpecialization*(StringView, const LanguageElements&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<void(Variable*)>("addVariable", &_::addVariable)
            .method<Variable*(StringView) const>("getVariable", &_::getVariable)
            .method<Variables const&() const>("getVariables", &_::getVariables)
            .method<void(Constant*)>("addConstant", &_::addConstant)
            .method<Constant*(StringView) const>("getConstant", &_::getConstant)
            .method<Constants const&() const>("getConstants", &_::getConstants)
            .method<Alias*(Symbol*, StringView, Modifiers, uint)>("addAlias", &_::addAlias)["0"]["0"]
            .method<void(Alias*)>("addAlias", &_::addAlias)
            .method<Template*(StringView, StringView, StringView, StringView, Modifiers, uint)>("addAliasTemplate", &_::addAliasTemplate)["0"]["0"]
            .method<Template*(TemplateSignature*, StringView, StringView, Modifiers, uint)>("addAliasTemplate", &_::addAliasTemplate)["0"]["0"]
            .method<void(Symbol*)>("addUsing", &_::addUsing)
            .method<Alias*(StringView) const>("getAlias", &_::getAlias)
            .method<Aliases const&() const>("getAliases", &_::getAliases)
            .method<LanguageElement*(StringView) const>("getAliased", &_::getAliased)
            .method<Alias*(StringView) const>("getTypedef", &_::getTypedef)
            .method<void(Function*)>("addFunction", &_::addFunction)
            .method<Functions const&() const>("getFunctions", &_::getFunctions)
            .method<Function*(StringView, TypesView) const>("getFunction", &_::getFunction)
            .method<void(Functions&, StringView, const Types*, Type*) const>("findFunctions", &_::findFunctions)["StringView()"]["nullptr"]["nullptr"]
            .method<void(AnonymousSection*), virtual_>("addAnonymousSection", &_::addAnonymousSection)
            .method<AnonymousSections const&() const>("getAnonymousSections", &_::getAnonymousSections)
            .method<Subroutine*(StringView, TypesView, Modifiers) const, virtual_>("getSubroutine", &_::getSubroutine)["0"]
            .method<Subroutine*(Type*, StringView, TypesView, Modifiers) const>("getSubroutine", &_::getSubroutine)["0"]
            .method<void(Subroutine*), virtual_>("addSubroutine", &_::addSubroutine)
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_>("acceptsSubroutine", &_::acceptsSubroutine)["0"]["0"]["nullptr"]
            .method<bool(Subroutine*, Subroutines*) const>("acceptsSubroutine", &_::acceptsSubroutine)["nullptr"]
            .method<void(Symbol*, Symbols&) const, virtual_>("getElementDoubles", &_::getElementDoubles)
            .method<void(Types&) const>("fetchTypesCascade", &_::fetchTypesCascade)
            .method<bool(Symbol*) const>("isSymbolHidden", &_::isSymbolHidden)
            .method<void(StringView, Symbols&) const, virtual_>("getScopedSymbolsWithName", &_::getScopedSymbolsWithName)
            .method<void(Classes&, Class*, bool) const>("findClasses", &_::findClasses)["nullptr"]["false"]
            .method<void(Alias*)>("removeAlias", &_::removeAlias)
            .method<void(Type*)>("removeType", &_::removeType)
            .method<void(Function*)>("removeFunction", &_::removeFunction)
            .method<void(Variable*)>("removeVariable", &_::removeVariable)
            .method<void(Constant*)>("removeConstant", &_::removeConstant)
            .method<void(Template*)>("removeTemplate", &_::removeTemplate)
            .method<void(TemplateSpecialization*)>("removeTemplateSpecialization", &_::removeTemplateSpecialization)
        
        .protected_()
            .method<void(Symbol*), pure_virtual>("onScopeSymbolAdded", &_::onScopeSymbolAdded)
            .method<void(Symbol*), pure_virtual>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)
        
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
