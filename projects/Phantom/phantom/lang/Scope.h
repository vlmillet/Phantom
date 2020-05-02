// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/core.h>
#include <phantom/lang/LanguageElement.h>
#include <phantom/lang/Members.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for non-abstract scopes (class scope, namespace scope, block...)
class PHANTOM_EXPORT_PHANTOM Scope
{
protected:
    Scope(LanguageElement* a_pThisElement, LanguageElement* a_pUnit);
    void initialize();
	void terminate();

public:
    virtual Scope* asScope() const = 0;

    PHANTOM_FORCEINLINE LanguageElement* asLanguageElement() const { return m_pThisElement; }

    PHANTOM_FORCEINLINE Symbol* asSymbol() const { return m_pThisElement->asSymbol(); }
    PHANTOM_FORCEINLINE Namespace* asNamespace() const { return m_pThisElement->asNamespace(); }

    bool isSame(Symbol* a_pSymbol) const { return m_pThisElement->isSame((LanguageElement*)a_pSymbol); }
    bool isSame(LanguageElement* a_pElement) const { return m_pThisElement->isSame(a_pElement); }
    bool isSame(Scope* a_pScope) const { return m_pThisElement->isSame(a_pScope->m_pThisElement); }

    virtual Scope* getNamedScope() const { return (Scope*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a type to this scope.
    ///
    /// \param [in,out] a_pType The type to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addType(Type* a_pType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the types declared in this scope.
    ///
    /// \return The type list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Types const& getTypes() const { return *m_Types; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type matching the given name.
    ///
    /// \param  a_strName   The type name.
    ///
    /// \return null if no type exists with the given name, else the type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getType(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the primitive type matching the given name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no type exists with the given name or its not a primitive type, else the
    /// primitive type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PrimitiveType* getPrimitiveType(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the class matching the given name.
    ///
    /// \param  a_strName   The class name.
    ///
    /// \return null if no class exists with the given name, else the class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getClass(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template to this scope.
    ///
    /// \param [in,out] a_pTemplate The template to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplate(Template* a_pTemplate);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template matching the given name.
    ///
    /// \param  a_strName   The template name.
    ///
    /// \return null if no template exists with the given name, else the template.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* getTemplate(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the templates declared in this scope.
    ///
    /// \return The template list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Templates const& getTemplates() const { return *m_Templates; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template specialization to this scope.
    ///
    /// \param a_pTemplateSpecialization  The template specialization to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create a PHANTOM_NEW(naked) template specialization.
    ///
    /// \param  a_pTemplate             The specialized template.
    /// \param  a_pTemplateSignature    The template signature.
    /// \param  a_Arguments             The template arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                      const LanguageElements& a_Arguments);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create a PHANTOM_NEW(template) specialization.
    ///
    /// \param  a_pTemplate             The specialized template.
    /// \param  a_pTemplateSignature    The template signature.
    /// \param  a_Arguments             The template arguments.
    /// \param  a_pBody                 The template body.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                      const LanguageElements& a_Arguments, Symbol* a_pBody);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Create a PHANTOM_NEW(template) instantiation.
    ///
    /// \param  a_pInstantiationSpecialization              The template specialization used to
    /// instantiate the template. \param  a_Arguments                                 The template
    /// arguments. \param  a_PartialSpecializationParameterDeductions  The template deductions
    /// (values of the partial specialization placeholders, which can differ from arguments, ex:
    /// placeholder T can be uses as T[5] in a partial specialization and if int[5] is the
    /// instantiation argument, T is int, not int[5]).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* addTemplateInstantiation(TemplateSpecialization* a_pInstantiationSpecialization,
                                                     const LanguageElements& a_Arguments,
                                                     const PlaceholderMap& a_PartialSpecializationParameterDeductions);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specializations declared in this scope.
    ///
    /// \return The template specialization list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecializations const& getTemplateSpecializations() const { return *m_TemplateSpecializations; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specialization matching given template name and arguments.
    ///
    /// \param  a_strName   The template name.
    /// \param  a_Arguments The arguments.
    ///
    /// \return The template specialization matching given template name and arguments.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getTemplateSpecialization(StringView a_strName, const LanguageElements& a_Arguments) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a variable to this scope.
    ///
    /// \param [in,out] The variable to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addVariable(Variable* a_pVariable);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the variable matching the given name.
    ///
    /// \param  a_strName   The variable name.
    ///
    /// \return null if no variable found with the given name, else the variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variable* getVariable(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the variables in this scope.
    ///
    /// \return the variable list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variables const& getVariables() const { return *m_Variables; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constant to this scope.
    ///
    /// \param [in,out] The constant to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addConstant(Constant* a_pConstant);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constant matching the given name.
    ///
    /// \param  a_strName   The constant name.
    ///
    /// \return null if no constant found with the given name, else the constant.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getConstant(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constants in this scope.
    ///
    /// \return the constant list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constants const& getConstants() const { return *m_Constants; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(alias) (typedef, using, etc...) to this scope.
    ///
    /// \param  a_pElement          The aliased element.
    /// \param  a_strAlias          The alias name.
    /// \param  a_Modifiers         (optional) the modifiers.
    ///
    /// \return null if it fails, else the PHANTOM_NEW(alias).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* addAlias(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an alias to this scope.
    ///
    /// \param [in,out] a_pAlias   The alias to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addAlias(Alias* a_pAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an alias template to this scope.
    ///
    /// \param a_TemplateTypes          The template signature types.
    /// \param a_TemplateParams         The template signature parameters.
    /// \param a_strAliasName           The alias name.
    /// \param a_TemplateDependantType  The template aliased type depending on template arguments.
    /// \param  a_Modifiers             (optional) the modifiers.
    /// \param  a_uiFlags               (optional) the flags.
    ///
    /// \return null if failed, else the added PHANTOM_NEW(template) alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* addAliasTemplate(StringView a_TemplateTypes, StringView a_TemplateParams, StringView a_strAliasName,
                               StringView a_TemplateDependantType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    /// \brief  Adds an alias template to this scope.
    ///
    /// \param a_pSignature             The template signature.
    /// \param a_strAliasName           The alias name.
    /// \param a_TemplateDependantType  The template aliased type depending on template arguments.
    /// \param  a_Modifiers             (optional) the modifiers.
    /// \param  a_uiFlags               (optional) the flags.
    ///
    /// \return null if failed, else the added PHANTOM_NEW(template) alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* addAliasTemplate(TemplateSignature* a_pSignature, StringView a_strAliasName,
                               StringView a_TemplateDependantType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an using to this scope.
    ///
    /// \param [in,out] a_pElement  If non-null, the element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addUsing(Symbol* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the alias matching the given name.
    ///
    /// \param  a_strName  The alias name.
    ///
    /// \return null if no alias found matching the given alias name, else the alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* getAlias(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the aliases in this scope.
    ///
    /// \return the alias list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Aliases const& getAliases() const { return *m_Aliases; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the element aliased with the given alias name.
    ///
    /// \param  a_strAlias  The alias name.
    ///
    /// \return null if no alias found matching the given alias name, else the aliased element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getAliased(StringView a_strAlias) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the typedef matching the given name.
    ///
    /// \param  a_strName    The typedef name.
    ///
    /// \return null if not typedef found matching the given name, else the typedef.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* getTypedef(StringView a_strTypedef) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a function to this scope.
    ///
    /// \param [in,out] a_pFunction The function to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addFunction(Function* a_pFunction);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the functions declared in this scope.
    ///
    /// \return The functions list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Functions const& getFunctions() const { return *m_Functions; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a function from given name and parameters' types.
    ///
    /// \param  a_strName   The name.
    /// \param  a_Types     The parameters' types.
    ///
    /// \return null if no function match the given name and parameters' types, else the function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Function* getFunction(StringView a_strName, TypesView a_Types) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find all the functions in this module matching the given optional name and/or
    /// signature
    ///
    /// \param [in,out] a_Classes   [in,out] The found functions.
    /// \param  a_pName             (optional) If non-empty, the name of the functions to look for.
    /// \param  a_pParameterTypes   (optional) If non-null, the parameter types of the functions to
    /// look for. \param  a_pReturnType       (optional) If non-null, the return type of the
    /// functions to look for.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findFunctions(Functions& a_Functions, StringView a_Name = StringView(),
                       const Types* a_pParameterTypes = nullptr, Type* a_pReturnType = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an anonymous section to this namespace.
    ///
    /// \param [in,out] a_pAnonymousSection The anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addAnonymousSection(AnonymousSection* a_pAnonymousSection);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the list of anonymous sections hold by this namespace.
    ///
    /// \return null if it fails, else the anonymous sections.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    AnonymousSections const& getAnonymousSections() const { return *m_AnonymousSections.operator->(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a subroutine from given name, parameters' types and optional modifiers.
    ///
    /// \param  a_strName           The name.
    /// \param  a_ParameterTypes    The parameters' types.
    /// \param  a_Modifiers         (optional) The modifiers.
    ///
    /// \return null if no subroutine found matching the given arguments, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Subroutine* getSubroutine(StringView a_strName, TypesView a_ParameterTypes,
                                      Modifiers a_Modifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a subroutine from given return type, name, parameters' types and optional
    /// modifiers.
    ///
    /// \param  a_pReturnType   The return type.
    /// \param  a_strName               The name.
    /// \param  a_ParameterTypes        The parameters' types.
    /// \param  a_Modifiers             (optional) The modifiers.
    ///
    /// \return null if no subroutine found matching the given arguments, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_ParameterTypes,
                              Modifiers a_Modifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a subroutine to this scope (should be overriden in derived classes).
    ///
    /// \param a_pSubroutine   The subroutine to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addSubroutine(Subroutine* a_pSubroutine);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this scope accepts the described subroutine.
    ///
    /// \param  a_pReturnType                   The subroutine return type.
    /// \param  a_strName                       The subroutine name.
    /// \param  a_ParameterTypes                The subroutine parameters' types.
    /// \param  a_Modifiers                     (optional) The subroutine modifiers.
    /// \param  a_Flags                         (optional) The subroutine flags.
    /// \param  a_pOutConflictingSubroutines    (optional) [in,out] If failed, will contain the
    /// conflicting subroutines forbidding this one to be accepted
    ///
    /// \return true if the described subroutine is accepted, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types,
                                   Modifiers a_Modifiers = 0, uint a_uiFlags = 0,
                                   Subroutines* a_pOutConflictingSubroutines = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this scope accepts the given subroutine.
    ///
    /// \param [in,out] a_pSubroutine   The subroutine to test.
    ///
    /// \return true if the given subroutine is accepted, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsSubroutine(Subroutine* a_pSubroutine, Subroutines* a_pOutConflictingSubroutines = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the given element's doubles contained in this scope.
    ///
    /// \param a_pElement  The element.
    /// \param [in,out] out         [in,out] The output doubles .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getElementDoubles(Symbol* a_pElement, Symbols& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches types and nested types recursively through this scope and its types.
    ///
    /// \param [in,out] a_Types The output types.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchTypesCascade(Types& a_Types) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given symbol has its name hidden by a higher scoped declaration.
    ///
    /// \param  a_pSymbol   The symbol to test.
    ///
    /// \return true if symbol hidden, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSymbolHidden(Symbol* a_pSymbol) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given symbols scoped in this Scope.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getScopedSymbolsWithName(StringView a_Name, Symbols& a_Symbols) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find all the classes in this source derived from the given base class into a vector
    ///         container.
    ///
    /// \param [in,out] a_Classes       [in,out] The found classes.
    /// \param  a_pBaseClass            (optional) If non-null, the base class used to filter the
    ///                                 searched classes. If null, every class in the module will be
    ///                                 collected.
    /// \param  a_bDefaultInstanciable  (optional) check if the classes must be default instanciable
    /// (not abstract + default constructible).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findClasses(Classes& a_Classes, Class* a_pBaseClass = nullptr, bool a_bDefaultInstanciable = false) const;

    void removeAlias(Alias* a_pAlias);
    void removeType(Type* a_pType);
    void removeFunction(Function* a_pFunction);
    void removeVariable(Variable* a_pVariable);
    void removeConstant(Constant* a_pConstant);
    void removeTemplate(Template* a_pTemplate);
    void removeTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization);

protected:
    virtual void onScopeSymbolAdded(Symbol* a_pElement) = 0;
    virtual void onScopeSymbolRemoving(Symbol* a_pElement) = 0;

protected:
    LanguageElement*                 m_pThisElement;
    LanguageElement*                 m_pUnit;
    Members<Functions>               m_Functions;
    Members<Variables>               m_Variables;
    Members<Constants>               m_Constants;
    Members<Types>                   m_Types;
    Members<Templates>               m_Templates;
    Members<TemplateSpecializations> m_TemplateSpecializations;
    Members<AnonymousSections>       m_AnonymousSections;
    Members<Aliases>                 m_Aliases;

public:
};

} // namespace lang
} // namespace phantom
