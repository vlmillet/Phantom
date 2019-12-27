// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/Constructor.h>
#include <phantom/detail/EmbeddedRtti.h>
#include <phantom/detail/newImpl.h>
#include <phantom/detail/phantom.h>
#include <phantom/reflection/CodeLocation.h>
#include <phantom/reflection/LanguageElementVisitor.h>
#include <phantom/reflection/reflection.h>
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/StringBuffer.h>
/* *********************************************** */

namespace phantom
{
namespace reflection
{
class MembersBase;

template<typename t_Scope>
class Members;

class Source;

class Ellipsis
{
};

#define PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT PHANTOM_REGISTER_FOR_VISIT(phantom::reflection::LanguageElementVisitor)

#define PHANTOM_REGISTER_FOR_VISIT(Visitor_)                                                                           \
public:                                                                                                                \
    friend class Visitor_;                                                                                             \
    virtual void visit(phantom::reflection::LanguageElementVisitor* a_pVisitor,                                        \
                       phantom::reflection::VisitorData             a_Data) override                                               \
    {                                                                                                                  \
        static_cast<Visitor_*>(a_pVisitor)->visit(this, a_Data);                                                       \
    }                                                                                                                  \
                                                                                                                       \
private:

typedef SmallMap<Placeholder*, LanguageElement*> PlaceholderMap;
typedef SmallSet<Module*>                        ModuleSet;

/// \brief  Base implementation of every language element.
class PHANTOM_EXPORT_PHANTOM LanguageElement
{
public:
    friend class phantom::reflection::LanguageElementVisitor;
    virtual void visit(phantom::reflection::LanguageElementVisitor* a_pVisitor, VisitorData a_Data);

    PHANTOM_DECLARE_META_CLASS(LanguageElement);

    friend class phantom::reflection::ClassType;
    friend class phantom::reflection::Class;
    friend class phantom::reflection::Scope;
    friend class phantom::reflection::TemplateSpecialization;
    friend class phantom::reflection::Module;
    friend class phantom::reflection::Application;
    friend class phantom::reflection::Source;
    friend class phantom::reflection::Namespace;
    friend class phantom::reflection::Symbol;
    friend class phantom::reflection::Semantic;
    friend class phantom::reflection::MembersBase;

public:
    typedef Delegate<Symbol*(Symbol*, bool)> SymbolFilter;
    static Symbol*                           NoFilter(Symbol* a_pSymbol, bool /*a_bUnamedSubSymbol*/)
    {
        return a_pSymbol;
    }
    static Symbol* PublicFilter(Symbol* a_pSymbol, bool a_bUnamedSubSymbol);
    static Symbol* PublicIfUnamedSubSymbolFilter(Symbol* a_pSymbol, bool a_bUnamedSubSymbol);

    virtual ~LanguageElement();

    void terminate();

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  most derived address of this LanguageElement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getMostDerived() const
    {
        return RTTI.instance;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  meta class of this LanguageElement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getMetaClass() const
    {
        return RTTI.metaClass;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  dynamic meta cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* as(Class* a_Class) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets elements owned by this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElements const& getElements() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets elements referenced by this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElements const& getReferencedElements() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets elements referencing this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElements const& getReferencingElements() const;

    bool isTemplateElement() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this element has a reference to the given element.
    ///
    /// \param a_pLanguageElement  The language element.
    ///
    /// \return true if the element is referenced, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasReferencedElement(LanguageElement* a_pLanguageElement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if 'a_pLanguageElement' has referencing element.
    ///
    /// \param a_pLanguageElement  If non-null, the language element.
    ///
    /// \return true if referencing element, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasReferencingElement(LanguageElement* a_pLanguageElement) const;

    virtual Type* asAddressType() const
    {
        return nullptr;
    }
    virtual Alias* asAlias() const
    {
        return nullptr;
    }
    virtual AnonymousSection* asAnonymousSection() const
    {
        return nullptr;
    }
    virtual AnonymousStruct* asAnonymousStruct() const
    {
        return nullptr;
    }
    virtual AnonymousUnion* asAnonymousUnion() const
    {
        return nullptr;
    }
    virtual PrimitiveType* asArithmeticType() const
    {
        return nullptr;
    }
    virtual Array* asArray() const
    {
        return nullptr;
    }
    virtual Block* asBlock() const
    {
        return nullptr;
    }
    virtual Class* asClass() const
    {
        return nullptr;
    }
    virtual Pointer* asClassPointer() const
    {
        return nullptr;
    }
    virtual LValueReference* asClassLValueReference() const
    {
        return nullptr;
    }
    virtual RValueReference* asClassRValueReference() const
    {
        return nullptr;
    }
    virtual Type* asClassAddressType() const
    {
        return nullptr;
    }
    virtual ClassType* asClassType() const
    {
        return nullptr;
    }
    virtual Constant* asConstant() const
    {
        return nullptr;
    }
    virtual ConstType* asConstClass() const
    {
        return nullptr;
    }
    virtual Pointer* asConstClassPointer() const
    {
        return nullptr;
    }
    virtual LValueReference* asConstClassLValueReference() const
    {
        return nullptr;
    }
    virtual RValueReference* asConstClassRValueReference() const
    {
        return nullptr;
    }
    virtual Type* asConstClassAddressType() const
    {
        return nullptr;
    }
    virtual Constructor* asConstructor() const
    {
        return nullptr;
    }
    virtual ConstType* asConstType() const
    {
        return nullptr;
    }
    Reference*                 asConstLValueReference() const;
    virtual ConstVolatileType* asConstVolatileType() const
    {
        return nullptr;
    }
    virtual ContainerClass* asContainerClass() const
    {
        return nullptr;
    }
    virtual DataElement* asDataElement() const
    {
        return nullptr;
    }
    virtual FieldPointer* asFieldPointer() const
    {
        return nullptr;
    }
    virtual Pointer* asPointer() const
    {
        return nullptr;
    }
    virtual Destructor* asDestructor() const
    {
        return nullptr;
    }
    virtual Ellipsis* asEllipsis() const
    {
        return nullptr;
    } // not yet implemented
    virtual Enum* asEnum() const
    {
        return nullptr;
    }
    virtual Evaluable* asEvaluable() const
    {
        return nullptr;
    }
    virtual Expression* asExpression() const
    {
        return nullptr;
    }
    virtual Field* asField() const
    {
        return nullptr;
    }
    virtual PrimitiveType* asFloatingPointType() const
    {
        return nullptr;
    }
    virtual Function* asFunction() const
    {
        return nullptr;
    }
    virtual FunctionPointer* asFunctionPointer() const
    {
        return nullptr;
    }
    virtual FunctionType* asFunctionType() const
    {
        return nullptr;
    }
    virtual PrimitiveType* asFundamentalType() const
    {
        return nullptr;
    }
    virtual InitializerListType* asInitializerListType() const
    {
        return nullptr;
    }
    virtual PrimitiveType* asIntegralType() const
    {
        return nullptr;
    }
    inline LanguageElement* asLanguageElement() const
    {
        return (LanguageElement*)this;
    }
    virtual LocalVariable* asLocalVariable() const
    {
        return nullptr;
    }
    virtual LValueReference* asLValueReference() const
    {
        return nullptr;
    }
    virtual StlMapClass* asMapContainerClass() const
    {
        return nullptr;
    }
    virtual MemberAnonymousSection* asMemberAnonymousSection() const
    {
        return nullptr;
    }
    virtual MemberAnonymousStruct* asMemberAnonymousStruct() const
    {
        return nullptr;
    }
    virtual MemberAnonymousUnion* asMemberAnonymousUnion() const
    {
        return nullptr;
    }
    virtual MethodPointer* asMethodPointer() const
    {
        return nullptr;
    }
    virtual MemberPointer* asMemberPointer() const
    {
        return nullptr;
    }
    virtual Method* asMethod() const
    {
        return nullptr;
    }
    virtual Module* asModule() const
    {
        return nullptr;
    }
    virtual Symbol* asSymbol() const
    {
        return nullptr;
    }
    virtual Namespace* asNamespace() const
    {
        return nullptr;
    }
    virtual Type* asNullptrType() const
    {
        return nullptr;
    }
    virtual Package* asPackage() const
    {
        return nullptr;
    }
    virtual PackageFolder* asPackageFolder() const
    {
        return nullptr;
    }
    virtual Parameter* asParameter() const
    {
        return nullptr;
    }
    virtual Placeholder* asPlaceholder() const
    {
        return nullptr;
    }
    virtual Type* asPOD() const
    {
        return nullptr;
    }
    virtual PointerType* asPointerType() const
    {
        return nullptr;
    }
    virtual ExtendedType* asExtendedType() const
    {
        return nullptr;
    }
    virtual PrimitiveType* asPrimitiveType() const
    {
        return nullptr;
    }
    virtual Property* asProperty() const
    {
        return nullptr;
    }
    virtual Reference* asReference() const
    {
        return nullptr;
    }
    virtual RValueReference* asRValueReference() const
    {
        return nullptr;
    }
    virtual Scope* asScope() const
    {
        return nullptr;
    }
    virtual SequentialContainerClass* asSequentialContainerClass() const
    {
        return nullptr;
    }
    virtual StlSetClass* asSetContainerClass() const
    {
        return nullptr;
    }
    virtual Signal* asSignal() const
    {
        return nullptr;
    }
    virtual Signature* asSignature() const
    {
        return nullptr;
    }
    virtual Method* asSlot() const
    {
        return nullptr;
    }
    virtual Source* asSource() const
    {
        return nullptr;
    }
    virtual Statement* asStatement() const
    {
        return nullptr;
    }
    virtual StaticField* asStaticField() const
    {
        return nullptr;
    }
    virtual StaticMethod* asStaticMethod() const
    {
        return nullptr;
    }
    virtual Structure* asStructure() const
    {
        return nullptr;
    }
    virtual Subroutine* asSubroutine() const
    {
        return nullptr;
    }
    virtual Template* asTemplate() const
    {
        return nullptr;
    }
    virtual TemplateParameter* asTemplateParameter() const
    {
        return nullptr;
    }
    virtual TemplateSignature* asTemplateSignature() const
    {
        return nullptr;
    }
    virtual TemplateSpecialization* asTemplateSpecialization() const
    {
        return nullptr;
    }
    virtual Type* asType() const
    {
        return nullptr;
    }
    virtual Union* asUnion() const
    {
        return nullptr;
    }
    virtual ValueMember* asValueMember() const
    {
        return nullptr;
    }
    virtual Variable* asVariable() const
    {
        return nullptr;
    }
    virtual VirtualMethodTable* asVirtualMethodTable() const
    {
        return nullptr;
    }
    virtual VolatileType* asVolatileType() const
    {
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Converts this element to type if possible.
    ///
    /// \param a_pOptExpression    If non-null, the optional 'left-hand-side' expression.
    ///
    /// \return The resulting expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* toType() const
    {
        return asType();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given element is friend of this element.
    ///
    /// \param a_pElement  The element to test.
    ///
    /// \return true if friend, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasFriend(Symbol*) const
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query the given element is a sub element of this element.
    ///
    /// \param a_pElement  The element to test.
    ///
    /// \return true if element, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasElement(LanguageElement* a_pElement) const
    {
        return a_pElement->m_pOwner == this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given element is friend of this element, recursively through all
    /// owner's hierarchy.
    ///
    /// \param a_pElement  The element to test.
    ///
    /// \return true if friend, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool hasFriendCascade(Symbol* a_pElement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given element is an ancestor/ascendant of this element.
    ///
    /// \param a_pElement  The element to test.
    ///
    /// \return true if owner cascade, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasOwnerCascade(LanguageElement* a_pElement) const
    {
        return (m_pOwner == a_pElement) OR(m_pOwner AND m_pOwner->hasOwnerCascade(a_pElement));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query the given element is a sub element of this element or of any of its sub
    /// element recursively.
    ///
    /// \param a_pElement  The element to test.
    ///
    /// \return true if element cascade, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasElementCascade(LanguageElement* a_pElement) const
    {
        return hasElement(a_pElement) OR(m_pOwner AND m_pOwner->hasElementCascade(a_pElement));
    }

    virtual bool isCompileTime() const
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Mark this element as invalid an generate error message describing the reason.
    ///
    /// \param  a_Format    The message formatting.
    ///
    /// \return The error message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Message* error(const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Mark this element as invalid an generate a sub error message describing the reason,
    /// attached to the main error message.
    ///
    /// \param  a_Format    The message formatting.
    ///
    /// \return The sub verror message.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    Message* subError(const char* a_Format, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner of this element.
    ///
    /// \return The owner.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE LanguageElement* getOwner() const
    {
        return m_pOwner;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets every sub elements inheriting a given optional class, recursively through all
    /// the sub elements.
    ///
    /// \param a_OutElements   The output elements.
    /// \param  a_pClass                (optional) The filtering class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchElementsDeep(LanguageElements& out, Class* a_pClass = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets every sub elements inheriting a given optional class.
    ///
    /// \param [in,out] a_OutElements   The output elements.
    /// \param  a_pClass                (optional) The filtering class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchElements(LanguageElements& out, Class* a_pClass = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all symbol with given name owned by this element.
    ///
    /// \param  a_strName               The symbol name.
    /// \param [in,out] a_OutSymbols    the output symbols.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getSymbolsWithName(StringView a_strName, Symbols& a_OutSymbols) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all symbol owned by this element.
    ///
    /// \param [in,out] a_OutSymbols    the output symbols.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getSymbols(Symbols& a_OutSymbols) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this element owns a symbol with the given name.
    ///
    /// \param  a_strName               The symbol name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasSymbol(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the module holding this element.
    ///
    /// \return The module holding this element or this if it is a module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getModule() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package holding this element.
    ///
    /// \return The package holding this element or this if it is a source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getPackage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source holding this element.
    ///
    /// \return The source holding this element or this if it is a source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this element can be destroyed safely.
    ///
    /// \return true if this element can be destroyed safely, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canBeDestroyed() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this element can be unloaded safely.
    ///
    /// \return true if this element can be unloaded safely, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canBeUnloaded() const;

    virtual void checkCompleteness() const;

    virtual bool isSame(LanguageElement* a_pOther) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Hatch, i.e. if any underlying element, destroys this element and return its
    /// underlying element (see removeExpression()).
    ///         It allows for example to extract a constant from a constant expression.
    ///
    /// \return this element if no hatched element, else the hatched element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LanguageElement* hatchExpression();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying element if any, for example the underlying element of a data
    /// member expression is the associated data member.
    ///
    /// \return The underlying element or null if none.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LanguageElement* removeExpression() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying element only if exists and is a symbol.
    ///
    /// \return The underlying element as symbol or null if none or not a symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* removeExpressionAsSymbol() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the class type enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing class type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType* getEnclosingClassType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the statement enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing statement.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Statement* getEnclosingStatement() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the block enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Block* getEnclosingBlock() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the scope enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing scope.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Scope* getEnclosingScope() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the namespace enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing namespace.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getEnclosingNamespace() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getEnclosingSubroutine() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specialization enclosing this element, if any.
    ///
    /// \return null if it fails, else the enclosing template specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getEnclosingTemplateSpecialization() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Partial accepts.
    ///
    /// \param a_pLanguageElement  If non-null, the language element.
    /// \param a_Score             The score.
    /// \param a_Deductions        If non-null, the deductions.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score,
                                PlaceholderMap& a_Deductions) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a referenced element.
    ///
    /// \param a_pElement  The referenced element to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addReferencedElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a referenced element but assert in case of element already referenced.
    ///
    /// \param a_pElement  The referenced element to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addUniquelyReferencedElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a referenced element.
    ///
    /// \param a_pElement  The referenced element to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeReferencedElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Adds a scoped element (allows to add an element partially only for scope
    /// resolution).
    ///
    /// \param a_pElement  The element to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addScopedElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Removes a scoped element.
    ///
    /// \param a_pElement  The element to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeScopedElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches modules referenced inside this element (i.e. modules managing elements
    /// referenced by this element).
    ///
    /// \param a_Modules   The output modules.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchReferencedModules(ModuleSet& a_Modules) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches modules referencing this element.
    ///
    /// \param a_Modules   The output modules.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchReferencingModules(ModuleSet& a_Modules) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches modules referenced inside this element or any sub element, recursively.
    ///
    /// \param a_Modules   The output modules.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchReferencedModulesDeep(ModuleSet& a_Modules) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches modules referencing this element or any sub element, recursively.
    ///
    /// \param a_Modules   The output modules.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchReferencingModulesDeep(ModuleSet& a_Modules) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches the elements contained in this element.
    ///
    /// \param a_Elements  The elements.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getElements(LanguageElements& a_Elements) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches the symbols contained in this element.
    ///
    /// \param a_Symbols  The symbols.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchSymbols(Symbols& a_Symbols) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetch/Collect the symbols owned by this element - and optionally the sub symbols of
    /// anonymous ones (enums for example) - which validate the given filter.
    ///
    /// \param  a_Symbols                   The accessible symbols.
    /// \param  a_Filter                    Specifies the filter discriminating symbol result.
    /// \param  a_bIncludeUnamedSubSymbols  (optional) True to include unamed sub symbols (symbols
    ///                                     contained by anonymous symbols).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchSymbols(Symbols& a_Symbols, SymbolFilter a_Filter, bool a_bIncludeUnamedSubSymbols = true,
                      bool a_bRecursive = false) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches all sub elements recursively.
    ///
    /// \param a_Elements  The elements.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getElementsDeep(LanguageElements& a_Elements) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub element.
    ///
    /// \param a_pElement  The element to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a named sub element.
    ///
    /// \param a_pElement  The element to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addSymbol(Symbol* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a sub element.
    ///
    /// \param a_pElement  The element to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeElement(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a unique element with the given name and modifiers. If multiple elements are
    /// found with the same name (overloads for example), return no one.
    ///
    /// \param  name        The element's name.
    /// \param  modifiers   (optional) The element'smodifiers.
    ///
    /// \return null if it fails, else the unique element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* getUniqueElement(StringView name, Modifiers modifiers = 0, uint a_uiFlags = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves given sub element's index in this element.
    ///
    /// \param a_pElement  The element.
    ///
    /// \return The element's index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getElementIndex(LanguageElement* a_pElement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Dumps this element sub element's list to an output stream for debug purpose.
    ///
    /// \param out The output stream.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void dumpElementList(std::basic_ostream<char>& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Dumps all sub elements recrusively to an output stream for debug purpose.
    ///
    /// \param out The output stream.
    /// /////////////////////////////////////////////////////////////////////////////////////////////////

    void dumpElementListCascade(std::basic_ostream<char>& out) const;

    virtual String getQualifiedDecoratedName() const;

    virtual String getQualifiedName() const;

    virtual String getDecoratedName() const;

    String getName() const;

    PHANTOM_FORCEINLINE String getUniqueName() const
    {
        StringBuffer cstr;
        getUniqueName(cstr);
        return String(cstr.data(), cstr.size());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this elemnet's qualified name (ex : namespace0::namespace1::element0).
    ///
    /// \return The qualified name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getName(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this elemnet's qualified name (ex : namespace0::namespace1::element0).
    ///
    /// \return The qualified name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getQualifiedName(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this elemnet's decorated name (ex : element0<typename0>).
    ///
    /// \return The decorated name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getDecoratedName(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this elemnet's qualified decorated name (ex :
    /// namespace0::namespace1::element0<typename0>).
    ///
    /// \return The qualified decorated name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this elements' unique name, i.e based on module/package/source structure (ex:
    /// package0.source0.class0).
    ///
    /// \return The unique name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void   getUniqueName(StringBuffer& a_Buf) const;
    virtual Scope* getNamingScope() const;

    bool hasNamingScopeCascade(Scope* a_pScope) const;

    /// \brief  Remove all elements from this one.
    void clear();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Steals every sub elements of the given element (useful for temporaries or proxy
    /// elements).
    ///
    /// \param [in,out] a_pInput    The element to steal the sub elements from.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void steal(LanguageElement* a_pInput);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Allows easy removal of the element from its owner, Equivalent to
    /// 'owner->removeElement(this)'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void detach();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given symbol has its name hidden by a higher scoped declaration.
    ///
    /// \param  a_pSymbol   The symbol to test.
    ///
    /// \return true if symbol hidden, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isSymbolHidden(Symbol* a_pSymbol) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the flags set on this element.
    ///
    /// \return The flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    uint getFlags() const
    {
        return m_uiFlags;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets a flag on this element.
    ///
    /// \param  flag    The flag.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setFlag(uint flag)
    {
        m_uiFlags |= flag;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add flags to this element.
    ///
    /// \param  flags    The flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addFlags(uint flags)
    {
        m_uiFlags |= flags;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Remove flags from this element.
    ///
    /// \param  flags    The flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeFlags(uint flags)
    {
        m_uiFlags &= ~flags;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the flags of this element.
    ///
    /// \param  flags    The flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setFlags(uint flags)
    {
        m_uiFlags = flags;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if given flags match this element's ones.
    ///
    /// \param  flags   The flags to test.
    ///
    /// \return true if the test passes, false if the test fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool testFlags(uint flags) const
    {
        return (m_uiFlags & flags) == flags;
    }

    PHANTOM_FORCEINLINE bool isNative() const
    {
        return testFlags(PHANTOM_R_FLAG_NATIVE);
    }
    PHANTOM_FORCEINLINE bool isInvalid() const
    {
        return testFlags(PHANTOM_R_FLAG_INVALID);
    }

    bool isTemplateDependant() const
    {
        return ((m_uiFlags & PHANTOM_R_FLAG_TEMPLATE_DEPENDANT) == PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
    }
    void setTemplateDependant()
    {
        m_uiFlags |= PHANTOM_R_FLAG_TEMPLATE_DEPENDANT;
    }

    void setInvalid();
    bool isIncomplete() const;
    void setIncomplete();

    PHANTOM_FORCEINLINE bool isAlwaysValid() const
    {
        return (m_uiFlags & PHANTOM_R_ALWAYS_VALID) == PHANTOM_R_ALWAYS_VALID;
    }

    virtual bool isTemplateInstance() const
    {
        return false;
    }

    /// \brief  Sets this element as 'shared'.
    PHANTOM_FORCEINLINE void setShared()
    {
        m_uiFlags |= PHANTOM_R_FLAG_SHARED;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this element is 'shared'.
    ///
    /// \return true if 'shared', false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE bool isShared() const
    {
        return ((m_uiFlags & PHANTOM_R_FLAG_SHARED) == PHANTOM_R_FLAG_SHARED);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the code position of this element (beginning)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setCodePosition(const CodePosition& a_CodePosition)
    {
        setCodeRange(CodeRange{a_CodePosition, a_CodePosition});
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the code range of this element as  {{line, col}, {line, col}}
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const CodeRange& getCodeRange() const
    {
        return m_CodeRange;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the code range of this element
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setCodeRange(const CodeRange& a_CodeRange);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the code position of this element as {line, col}
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const CodePosition& getCodePosition() const
    {
        return m_CodeRange.begin;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the code location of this element, which contains the source + a code position
    /// {line, col}
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CodeLocation getCodeLocation() const
    {
        return CodeLocation(getCodeLocationSource(), getCodePosition());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the code range location of this element, which container the source + a code range
    /// {{line, col}, {line, col}}
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CodeRangeLocation getCodeRangeLocation() const
    {
        return CodeRangeLocation(getCodeLocationSource(), getCodeRange());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the source which contains code for this element.
    ///         /!\ To not mix up with getSource() which gives the translation unit in which this symbol is stored.
    ///          (by default they are the same, but they might differ for template instantiations for example)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Source* getCodeLocationSource() const;

    LanguageElement* getElementAtCodePosition(const CodePosition& a_CodePosition) const;
    LanguageElement* getElementAtLine(uint16 a_Line) const;

    /// @cond INTERNAL
    EmbeddedProxyRtti RTTI;
    /// @endcond

    virtual int destructionPriority() const;

protected:
    LanguageElement(uint a_uiFlags = 0);

protected:
    virtual void onAncestorChanged(LanguageElement* a_pOwner);
    virtual void onAncestorAboutToBeChanged(LanguageElement* a_pOwner);
    virtual void onElementAdded(LanguageElement* a_pElement);
    virtual void onElementRemoved(LanguageElement* a_pElement);
    virtual void onReferencedElementAdded(LanguageElement* a_pElement);
    virtual void onReferencedElementRemoved(LanguageElement* a_pElement);
    virtual void onInvalidated();
    virtual void onElementsAccess()
    {
    }

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Reimplement this function to provide specific behaviors when elements of the scope
    /// are accessed.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void _onElementsAccess();

private:
    void setOwner(LanguageElement* a_pOwner);
    void _onAncestorChanged(LanguageElement* a_pAncestor);
    void _onAncestorAboutToBeChanged(LanguageElement* a_pOwner);
    void replaceElement(LanguageElement* a_pOld, LanguageElement* a_pNew);
    void registerReferencingElement(LanguageElement* a_pElement);
    void unregisterReferencingElement(LanguageElement* a_pElement);
    void _nativeDetachElementsFromModule();

private:
    LanguageElement* m_pOwner; /// Owner represents the real container of the element, not the
                               /// naming scope (both can still be equal). For example a global
                               /// function is owned by its source, not by the global namespace
    LanguageElements* m_pElements;
    LanguageElements* m_pReferencingElements;
    LanguageElements* m_pReferencedElements;
    CodeRange         m_CodeRange;
    uint              m_uiFlags;

private:
    static void             Register(LanguageElement*);
    static void             Unregister(LanguageElement*);
    static LanguageElements sm_Elements;
};

} // namespace reflection

template<class T, bool Is>
struct ConstructorIfLanguageElement
    : Constructor<T, ConstructorOverloadTag::Enum(ConstructorOverloadTag::MetaElement - 1)>
{
    static void destroy(T* a_pInstance)
    {
        a_pInstance->terminate(); // ensure terminate is called before destructor for every LanguageElement
        Constructor<T, ConstructorOverloadTag::Default>::destroy(a_pInstance);
    }
};

template<class T>
struct ConstructorIfLanguageElement<T, false>
    : Constructor<T, ConstructorOverloadTag::Enum(ConstructorOverloadTag::MetaElement - 1)>
{
};

template<class T>
struct Constructor<T, ConstructorOverloadTag::MetaElement>
    : ConstructorIfLanguageElement<T, std::is_base_of<reflection::LanguageElement, T>::value>
{
};

} // namespace phantom
