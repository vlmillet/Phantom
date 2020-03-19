// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ClassType.h"

#include "Application.h"
#include "ConstType.h"
#include "Constructor.h"
#include "Destructor.h"
#include "Field.h"
#include "Function.h"
#include "LValueReference.h"
#include "MemberAnonymousStruct.h"
#include "MemberAnonymousUnion.h"
#include "Namespace.h"
#include "Pointer.h"
#include "Property.h"
#include "RValueReference.h"
#include "Signature.h"
#include "TemplateSpecialization.h"
#include "Variable.h"
#include "registration/registration.h"

#include <phantom/detail/VirtualDelete.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ClassType::ClassType(TypeKind a_eTypeKind, ExtraData* a_pExtraData, Modifiers a_Modifiers /*= 0*/,
                     uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_Modifiers, a_uiFlags), Scope(this), Aggregate(this), m_pExtraData(a_pExtraData)
{
}

ClassType::ClassType(TypeKind a_eTypeKind, ExtraData* a_pExtraData, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                     uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_strName, a_Modifiers, a_uiFlags), Scope(this), Aggregate(this), m_pExtraData(a_pExtraData)
{
}

ClassType::ClassType(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                     Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags), Scope(this), Aggregate(this)
{
}

ClassType::~ClassType()
{
    PHANTOM_DELETE_VIRTUAL m_pExtraData;
}

bool ClassType::matchesTemplateArguments(const LanguageElements& a_Elements) const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    return pSpec && pSpec->matches(a_Elements);
}

void ClassType::checkCompleteness() const
{
    if (getTemplateSpecialization())
        getTemplateSpecialization()->checkCompleteness();
}

Template* ClassType::getTemplate() const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    return pSpec ? pSpec->getTemplate() : nullptr;
}

void ClassType::onElementAdded(LanguageElement* a_pElement)
{
    if (Symbol* pSym = a_pElement->asSymbol())
    {
        if (pSym->getAccess() == Access::Undefined)
            pSym->setAccess(getDefaultAccess());
    }
    if (Field* pField = a_pElement->asField())
    {
        PHANTOM_ASSERT_DEBUG(getValueMember(pField->getName()) == nullptr);
        PHANTOM_ASSERT(isNative() || m_uiSize == 0,
                       "type sized, cannot add fields anymore or the memory consistency would be messed up");
        PHANTOM_ASSERT((asPOD() == nullptr ||(pField->getValueType()->asPOD() != nullptr)),
                       "POD structs can only store pod types");
        if (pField->getMemberAnonymousSection() == nullptr) // does not belong to an anonymous struct/union
            m_DataElements.push_back(pField);
        m_Fields.push_back(pField);
        // FIXME : hack (create a data() function to access directly to container)
        m_ValueMembers.push_back(nullptr);
        m_ValueMembers.m_data->pop_back();
        m_ValueMembers.insert(m_ValueMembers.m_data->begin() + (m_Fields.m_data->size() - 1), pField);
        PHANTOM_ASSERT(!(pField->getValueType()->removeQualifiers()->removeArray()->hasStrongDependencyOnType(this)),
                       "cyclic class strong dependency");
    }
    else if (Property* pProperty = a_pElement->asProperty())
    {
        PHANTOM_ASSERT_DEBUG(getValueMember(pProperty->getName()) == nullptr);
        PHANTOM_ASSERT(isNative() || pProperty->getSignal() == nullptr || m_uiSize == 0,
                       "type sized, cannot add property with signal anymore or the memory "
                       "consistency would be messed up");
        PHANTOM_ASSERT(isNative()
                       || pProperty->getGet() == nullptr || !(pProperty->getGet()->isVirtual()) || m_uiSize == 0,
                       "type sized, cannot add property with virtual method or the memory "
                       "consistency would be messed up");
        PHANTOM_ASSERT(isNative()
                       || pProperty->getSet() == nullptr || !(pProperty->getSet()->isVirtual()) || m_uiSize == 0,
                       "type sized, cannot add property with virtual method or the memory "
                       "consistency would be messed up");
        m_Properties.push_back(pProperty);
        m_ValueMembers.push_back(pProperty);
    }
    else if (Constructor* pConstructor = a_pElement->asConstructor())
    {
        m_Constructors.push_back(pConstructor);
    }
    else if (Method* pMethod = a_pElement->asMethod())
    {
        PHANTOM_ASSERT(isNative() || m_uiSize == 0 || !(pMethod->isVirtual()),
                       "type sized, cannot add virtual member functions anymore or the memory "
                       "consistency would be messed up");
        PHANTOM_ASSERT(isNative() || Scope::acceptsSubroutine(pMethod));
        m_Methods.push_back(pMethod);
    }
    else if (ValueMember* pValueMember = a_pElement->asValueMember())
    {
        PHANTOM_ASSERT_DEBUG(getValueMember(pValueMember->getName()) == nullptr);
        m_ValueMembers.push_back(pValueMember);
    }
    else if (MemberAnonymousSection* pMemberAnonymousSection = a_pElement->asMemberAnonymousSection())
    {
        PHANTOM_ASSERT(isNative() || m_uiSize == 0,
                       "type sized, cannot add anonymous types anymore or the memory consistency "
                       "would be messed up");
        m_MemberAnonymousSections.push_back(pMemberAnonymousSection);
        m_DataElements.push_back(pMemberAnonymousSection);
    }
    else
    {
        Type::onElementAdded(a_pElement);
        Scope::scopedElementAdded(a_pElement);
    }
}

void ClassType::onElementRemoved(LanguageElement* a_pElement)
{
    Type::onElementRemoved(a_pElement);
    Scope::scopedElementRemoved(a_pElement);
    if (a_pElement->asField())
    {
        m_Fields->erase(std::find(m_Fields->begin(), m_Fields->end(), static_cast<Field*>(a_pElement)));
        m_ValueMembers->erase(
        std::find(m_ValueMembers->begin(), m_ValueMembers->end(), static_cast<Field*>(a_pElement)));
    }
    else if (a_pElement->asProperty())
    {
        m_Properties->erase(std::find(m_Properties->begin(), m_Properties->end(), static_cast<Property*>(a_pElement)));
        m_ValueMembers->erase(
        std::find(m_ValueMembers->begin(), m_ValueMembers->end(), static_cast<Property*>(a_pElement)));
    }
    else if (a_pElement->asConstructor())
    {
        m_Constructors->erase(
        std::find(m_Constructors->begin(), m_Constructors->end(), static_cast<Constructor*>(a_pElement)));
    }
    else if (a_pElement->asMethod())
    {
        m_Methods->erase(std::find(m_Methods->begin(), m_Methods->end(), static_cast<Method*>(a_pElement)));
    }
    else if (a_pElement->asValueMember())
    {
        m_ValueMembers->erase(std::find(m_ValueMembers->begin(), m_ValueMembers->end(), a_pElement->asValueMember()));
    }
    else if (a_pElement->asMemberAnonymousSection())
    {
        m_MemberAnonymousSections->erase(std::find(m_MemberAnonymousSections->begin(), m_MemberAnonymousSections->end(),
                                                   static_cast<MemberAnonymousSection*>(a_pElement)));
    }
}

Property* ClassType::getProperty(StringView a_strName) const
{
    ValueMember* pValueMember = getValueMember(a_strName);
    return pValueMember ? pValueMember->asProperty() : nullptr;
}

Property* ClassType::getProperty(size_t a_uiIndex) const
{
    return m_Properties[a_uiIndex];
}

Field* ClassType::getField(StringView a_strName) const
{
    auto it = m_ValueMembers->begin();
    auto end = m_ValueMembers->end();
    for (; it != end; ++it)
    {
        Field* pField = (*it)->asField();
        if (pField && pField->getName() == a_strName)
            return pField;
    }
    return nullptr;
}

Field* ClassType::getField(size_t a_uiIndex) const
{
    return m_Fields[a_uiIndex];
}

void ClassType::getAllFields(Fields& a_Out) const
{
    for (auto pDataElement : m_DataElements)
        pDataElement->fetchFields(a_Out);
}

Constructor* ClassType::getConstructor(StringView a_strParameterString) const
{
    Symbol* pElement = Application::Get()->findCppSymbol(a_strParameterString, const_cast<ClassType*>(this));
    return (pElement && pElement->getOwner() == this) ? pElement->asConstructor() : nullptr;
}

Constructor* ClassType::getConstructor(size_t a_uiIndex) const
{
    return m_Constructors[a_uiIndex];
}

Constructor* ClassType::getConstructor(Type* a_pType) const
{
    auto it = m_Constructors->begin();
    auto end = m_Constructors->end();
    for (; it != end; ++it)
    {
        Constructor* pConstructor = static_cast<Constructor*>((*it));
        if (pConstructor->getParameters().size() == 1 && pConstructor->getParameterType(0) == a_pType)
        {
            return pConstructor;
        }
    }
    return nullptr;
}

Method* ClassType::getConversionFunction(Type* a_pType) const
{
    for (auto it = m_Methods->begin(); it != m_Methods->end(); ++it)
    {
        Method* pMF = *it;
        if (pMF->getName().size() > 8 && pMF->getName()[8] == ' ' && pMF->getName().find("operator") == 0)
        {
            PHANTOM_ASSERT(pMF->getParameters().size() == 0);
            if (pMF->getReturnType() == a_pType)
                return pMF;
        }
    }
    return nullptr;
}

Method* ClassType::getCopyAssignmentOperator() const
{
    Type* pConstLValueRefType = makeConst()->makeLValueReference();
    for (auto it = m_Methods->begin(); it != m_Methods->end(); ++it)
    {
        Method* pMF = *it;
        if (pMF->getParameters().size() == 1 && pMF->getName() == "operator=")
        {
            if (pMF->getParameterType(0) == this || pMF->getParameterType(0) == (pConstLValueRefType))
                return pMF;
        }
    }
    return nullptr;
}

Method* ClassType::getMoveAssignmentOperator() const
{
    Type* pRValueRefType = makeRValueReference();
    for (auto it = m_Methods->begin(); it != m_Methods->end(); ++it)
    {
        Method* pMF = *it;
        if (pMF->getParameters().size() == 1 && pMF->getName() == "operator=")
        {
            if (pMF->getParameterType(0) == (pRValueRefType))
                return pMF;
        }
    }
    return nullptr;
}

void ClassType::getFullConversionTypes(Types& out, bool a_bImplicits /*= true*/) const
{
    Types types;
    for (auto it = m_Constructors->begin(); it != m_Constructors->end(); ++it)
    {
        Constructor* pCtor = *it;
        if (pCtor->getParameters().size() == 1 &&(!(a_bImplicits) || !(pCtor->testModifiers(PHANTOM_R_EXPLICIT))))
        {
            types.push_back(pCtor->getParameterType(0)->removeConstLValueReference());
        }
    }
    for (auto it = types.begin(); it != types.end();)
    {
        Method* pMF = getConversionFunction(*it);
        if (pMF &&(!(a_bImplicits) || !(pMF->testModifiers(PHANTOM_R_EXPLICIT))))
        {
            ++it;
        }
        else
        {
            it = types.erase(it);
        }
    }
    out.insert(out.end(), types.begin(), types.end());
}

Constructor* ClassType::getDefaultConstructor() const
{
    Types types;
    return getConstructor(Types());
}

Constructor* ClassType::getConstructor(TypesView a_Types) const
{
    auto it = m_Constructors->begin();
    auto end = m_Constructors->end();
    for (; it != end; ++it)
    {
        Constructor* pConstructor = static_cast<Constructor*>((*it));
        if (pConstructor->matches(getName(), a_Types))
        {
            return pConstructor;
        }
    }
    return nullptr;
}

Method* ClassType::getMethod(StringView a_strIdentifierString) const
{
    if (a_strIdentifierString.empty())
        return nullptr;
    if (a_strIdentifierString.find_last_of(" )") == String::npos) // not a complete signature, just the method name
    {
        for (auto& pMethod : *m_Methods)
        {
            if (pMethod->getName() == a_strIdentifierString)
                return pMethod;
        }
        return nullptr;
    }
    else
    {
        Symbol* pElement = Application::Get()->findCppSymbol(a_strIdentifierString, const_cast<ClassType*>(this));
        return (pElement && pElement->getOwner() == this) ? pElement->asMethod() : nullptr;
    }
}

Method* ClassType::getMethod(StringView a_strName, TypesView a_Types, Modifiers a_Qualifiers /*= 0*/) const
{
    StringBuffer buffer;
    conversionOperatorNameNormalizer(a_strName, buffer, const_cast<ClassType*>(this));
    String name(buffer.data(), buffer.size());
    {
        auto it = m_Methods->begin();
        auto end = m_Methods->end();
        for (; it != end; ++it)
        {
            Method* pMethod = static_cast<Method*>((*it));
            if (pMethod->getSignature()->getModifiers() == a_Qualifiers &&
                pMethod->matches(name, a_Types, a_Qualifiers))
            {
                return pMethod;
            }
        }
    }
    return nullptr;
}

phantom::lang::Method* ClassType::getMethod(Type* a_pReturnType, StringView a_strName, TypesView a_ParameterTypes,
                                            Modifiers a_Qualifiers /*= 0*/) const
{
    if (Method* pMethod = getMethod(a_strName, a_ParameterTypes, a_Qualifiers))
    {
        if (pMethod->getReturnType()->isSame(a_pReturnType))
            return pMethod;
    }
    return nullptr;
}

phantom::lang::Method* ClassType::getMethodByPtr(void* a_ppMethodPtr) const
{
    for (auto pMethod : *m_Methods)
    {
        if (pMethod->equalsPointer(a_ppMethodPtr))
            return pMethod;
    }
    return nullptr;
}

StaticMethod* ClassType::getStaticMethod(StringView a_strName, TypesView a_Types) const
{
    return Scope::getFunction(a_strName, a_Types);
}

StaticMethod* ClassType::getStaticMethod(StringView a_strIdentifierString) const
{
    if (a_strIdentifierString.empty())
        return nullptr;
    if (a_strIdentifierString.find_last_of(" )") == String::npos) // not a complete signature, just the method name
    {
        auto& functions = getFunctions();
        for (auto& pFunc : functions)
        {
            if (pFunc->getName() == a_strIdentifierString)
                return pFunc;
        }
        return nullptr;
    }
    else
    {
        Symbol* pElement = Application::Get()->findCppSymbol(a_strIdentifierString, const_cast<ClassType*>(this));
        return (pElement && pElement->getOwner() == this) ? pElement->asStaticMethod() : nullptr;
    }
}

Subroutine* ClassType::getSubroutine(StringView a_strIdentifierString) const
{
    Symbol* pElement = Application::Get()->findCppSymbol(a_strIdentifierString, const_cast<ClassType*>(this));
    return (pElement && pElement->getOwner() == this) ? pElement->asSubroutine() : nullptr;
}

Subroutine* ClassType::getSubroutine(StringView a_strName, TypesView a_Types, Modifiers a_Modifiers) const
{
    if ((a_Modifiers & PHANTOM_R_STATIC))
    {
        return getStaticMethod(a_strName, a_Types);
    }
    if (a_strName == getName() && a_Modifiers == Modifiers())
    {
        return getConstructor(a_Types);
    }
    Method* pMethod = getMethod(a_strName, a_Types, a_Modifiers);
    if (pMethod != nullptr)
        return pMethod;
    if (a_Modifiers == 0)
    {
        return getStaticMethod(a_strName, a_Types);
    }
    return nullptr;
}
/*

void ClassType::getQualifiedName(StringBuffer& a_Buf) const
{
    String        str ;
    if(m_pNamespace != nullptr)
    {
        m_pNamespace->getHierarchicalNameNoRoot(&str);
        if(!(str.empty()))
            str+=PHANTOM_CC('.');
    }
    str += getName().substr(0, getName().find_first_of(PHANTOM_CC('<'))) ;
    return str;
}*/

StaticField* ClassType::getStaticField(StringView a_strName) const
{
    return Scope::getVariable(a_strName);
}

ValueMember* ClassType::getValueMember(StringView a_strName) const
{
    auto it = m_ValueMembers->begin();
    auto end = m_ValueMembers->end();
    for (; it != end; ++it)
    {
        ValueMember* pValueMember = static_cast<ValueMember*>((*it));
        if (pValueMember->getName() == a_strName)
            return pValueMember;
    }
    return nullptr;
}

void ClassType::addConstructor(Constructor* a_pConstructor)
{
    addSymbol(a_pConstructor);
}

Constructor* ClassType::addConstructor(StringView a_strParametersString)
{
    // TODO add modifiers
    Signature* pSignature = Signature::Create();
    pSignature->parse((String("void(") + a_strParametersString + ")").c_str(), this);
    if (pSignature == nullptr)
        return nullptr;
    Constructor* pConstructor = PHANTOM_NEW(Constructor)(getName(), pSignature, 0);
    addConstructor(pConstructor);
    return pConstructor;
}

Constructor* ClassType::addConstructor(const Parameters& a_Parameters, Modifiers a_Modifiers /*= 0*/,
                                       uint a_uiFlags /*= 0*/)
{
    Signature*   pSignature = PHANTOM_NEW(Signature)(PHANTOM_TYPEOF(void), a_Parameters);
    Constructor* pConstructor = PHANTOM_NEW(Constructor)(getName(), pSignature, a_Modifiers, a_uiFlags);
    pConstructor->setAccess(getDefaultAccess());
    addConstructor(pConstructor);
    return pConstructor;
}

Constructor* ClassType::addConstructor(Type* a_pSingleParameterType, Modifiers a_Modifiers /*= 0*/,
                                       uint a_uiFlags /*= 0*/)
{
    Signature*   pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(PHANTOM_TYPEOF(void), a_pSingleParameterType);
    Constructor* pConstructor = PHANTOM_DEFERRED_NEW_EX(Constructor)(getName(), pSignature, a_Modifiers, a_uiFlags);
    addConstructor(pConstructor);
    return pConstructor;
}

Constructor* ClassType::addConstructor(Modifiers a_Modifiers, uint a_uiFlags)
{
    Signature*   pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(PHANTOM_TYPEOF(void));
    Constructor* pConstructor = PHANTOM_DEFERRED_NEW_EX(Constructor)(getName(), pSignature, a_Modifiers, a_uiFlags);
    addConstructor(pConstructor);
    return pConstructor;
}

void ClassType::addValueMember(ValueMember* a_pValueMember)
{
    addSymbol(a_pValueMember);
}

void ClassType::addProperty(Property* a_pProperty)
{
    addSymbol(a_pProperty);
}

Property* ClassType::addProperty(StringView, uint)
{
    PHANTOM_ASSERT(false, "not implemented");
    return nullptr;
}

void ClassType::addField(Field* a_pField)
{
    addSymbol(a_pField);
}

Field* ClassType::addField(Type* a_pValueType, StringView a_strName, uint a_uiFilterFlag, Modifiers a_Modifiers /*= 0*/,
                           uint a_uiFlags /*= 0*/)
{
    if (hasSymbol(a_strName))
    {
        PHANTOM_LOG(Error, "a symbol with name '%s' already exists in this class type");
        return nullptr;
    }
    Field* pField = PHANTOM_DEFERRED_NEW_EX(Field)(a_pValueType, a_strName, a_uiFilterFlag, a_Modifiers, a_uiFlags);
    addField(pField);
    return pField;
}

void ClassType::addMethod(Method* a_pMethod)
{
    addSymbol(a_pMethod);
}

void ClassType::addStaticMethod(StaticMethod* a_pStaticMethod)
{
    PHANTOM_ASSERT(a_pStaticMethod->testModifiers(PHANTOM_R_STATIC),
                   "static member function requires 'PHANTOM_R_STATIC' modifier");
    Scope::addFunction(a_pStaticMethod);
}

void ClassType::removeConstructor(Constructor* a_pConstructor)
{
    removeElement(a_pConstructor);
}

void ClassType::removeValueMember(ValueMember* a_pValueMember)
{
    removeElement(a_pValueMember);
}

void ClassType::removeProperty(Property* a_pProperty)
{
    removeElement(a_pProperty);
}

void ClassType::removeField(Field* a_pField)
{
    removeElement(a_pField);
}

void ClassType::removeStaticField(StaticField* a_pField)
{
    removeElement(a_pField);
}

void ClassType::removeMethod(Method* a_pMethod)
{
    removeElement(a_pMethod);
}

void ClassType::removeStaticMethod(StaticMethod* a_pStaticMethod)
{
    removeElement(a_pStaticMethod);
}

void* ClassType::newInstance() const
{
    void* pInstance = allocate(m_uiSize);
    construct(pInstance);
    return pInstance;
}

void* ClassType::placementNewInstance(void* a_pMemory) const
{
    construct(a_pMemory);
    return a_pMemory;
}

void* ClassType::placementNewInstance(void* a_pMemory, Constructor* a_pConstructor, void** a_pArgs /*= NULL*/) const
{
    a_pConstructor->construct(a_pMemory, a_pArgs);
    return a_pMemory;
}

void ClassType::deleteInstance(void* a_pInstance) const
{
    destroy(a_pInstance);
    deallocate(a_pInstance);
}

void ClassType::placementDeleteInstance(void* a_pInstance) const
{
    destroy(a_pInstance);
}

void ClassType::findPublicValueMembersPointingValueType(Type* a_pType, ValueMembers& out) const
{
    auto it = m_ValueMembers->begin();
    auto end = m_ValueMembers->end();
    for (; it != end; ++it)
    {
        ValueMember* pValueMember = static_cast<ValueMember*>((*it));
        Pointer*     pPointerType = pValueMember->getValueType()->asPointer();
        if (pPointerType && pValueMember->isPublic() && a_pType->isA(pPointerType->getPointeeType()))
        {
            out.push_back(pValueMember);
        }
    }
}

void ClassType::copyAssign(void* a_pDest, void const* a_pSrc) const
{
    Method* pOp = getCopyAssignmentOperator();
    PHANTOM_ASSERT(pOp);
    pOp->invoke(a_pDest, (void**)&a_pSrc);
}

void ClassType::moveAssign(void* a_pDest, void* a_pSrc) const
{
    if (Method* pMoveOp = getMoveAssignmentOperator())
    {
        pMoveOp->invoke(a_pDest, (void**)&a_pSrc);
        return;
    }
    copyAssign(a_pDest, a_pSrc);
}

void ClassType::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    Constructor* pCtor = getCopyConstructor();
    PHANTOM_ASSERT(pCtor);
    pCtor->construct(a_pDest, (void**)&a_pSrc);
}

void ClassType::moveConstruct(void* a_pDest, void* a_pSrc) const
{
    if (Constructor* pMoveCtor = getMoveConstructor())
    {
        pMoveCtor->construct(a_pDest, (void**)&a_pSrc);
        return;
    }
    copyConstruct(a_pDest, a_pSrc);
}

Method* ClassType::getUniqueMethodWithName(StringView a_strName) const
{
    Method* pMethod = nullptr;
    auto    it = m_Methods->begin();
    auto    end = m_Methods->end();
    for (; it != end; ++it)
    {
        if (static_cast<Method*>((*it))->getName() == a_strName)
        {
            if (pMethod)
                return nullptr;
            pMethod = static_cast<Method*>((*it));
        }
    }
    return pMethod;
}

void ClassType::fetchElements(LanguageElements& out, Class* a_pClass) const
{
    Type::fetchElements(out, a_pClass);
}

bool ClassType::canBeDestroyed() const
{
    return Type::canBeDestroyed();
}

const Constructors& ClassType::getConstructors() const
{
    return *m_Constructors;
}

void* ClassType::allocate() const
{
    PHANTOM_ASSERT(m_uiSize);
    return Type::allocate();
}

void* ClassType::allocate(size_t n) const
{
    PHANTOM_ASSERT(m_uiSize);
    return Type::allocate(n);
}

void ClassType::construct(void* a_pInstance) const
{
    PHANTOM_ASSERT(m_pExtraData);
    // Construct attributes

    PHANTOM_ASSERT(m_uiSize);
    Constructor* pConstructor = getDefaultConstructor();
    PHANTOM_ASSERT(pConstructor != nullptr, "no default constructor available for this class type");
    pConstructor->construct(a_pInstance, nullptr);
}

void ClassType::destroy(void* a_pInstance) const
{
    PHANTOM_ASSERT(m_pExtraData);

    Method* pDestructor = getDestructor();
    PHANTOM_ASSERT(pDestructor, "no destructor available for this class type");
    pDestructor->invoke(a_pInstance, nullptr);
}

void ClassType::addImplicitDefaultConstructor()
{
    PHANTOM_ASSERT(m_pExtraData);
    Signature*   pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(PHANTOM_TYPEOF(void));
    Constructor* pConstructor =
    PHANTOM_DEFERRED_NEW_EX(Constructor)(getName(), pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void ClassType::addImplicitCopyConstructor()
{
    PHANTOM_ASSERT(m_pExtraData);
    Signature* pSignature =
    PHANTOM_DEFERRED_NEW_EX(Signature)(PHANTOM_TYPEOF(void), makeConst()->makeLValueReference());
    Constructor* pConstructor =
    PHANTOM_DEFERRED_NEW_EX(Constructor)(getName(), pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void ClassType::addImplicitCopyAssignmentOperator()
{
    PHANTOM_ASSERT(m_pExtraData);
    Signature* pSignature =
    PHANTOM_DEFERRED_NEW_EX(Signature)(makeLValueReference(), makeConst()->makeLValueReference());
    Method* pFunc = PHANTOM_DEFERRED_NEW_EX(Method)("operator=", pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pFunc->setAccess(Access::Public);
    addMethod(pFunc);
}

void ClassType::addImplicitMoveConstructor()
{
    PHANTOM_ASSERT(m_pExtraData);
    Signature*   pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(PHANTOM_TYPEOF(void), makeRValueReference());
    Constructor* pConstructor =
    PHANTOM_DEFERRED_NEW_EX(Constructor)(getName(), pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void ClassType::addImplicitMoveAssignmentOperator()
{
    PHANTOM_ASSERT(m_pExtraData);
    Signature* pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(makeLValueReference(), makeRValueReference());
    Method* pFunc = PHANTOM_DEFERRED_NEW_EX(Method)("operator=", pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pFunc->setAccess(Access::Public);
    addMethod(pFunc);
}

void ClassType::addImplicitDestructor()
{
    PHANTOM_ASSERT(m_pExtraData);
    addDestructor(PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
}

void ClassType::_onNativeElementsAccessImpl()
{
    Source* pCurrentSource = phantom::lang::detail::currentSource();
    if (!pCurrentSource)
        phantom::lang::detail::pushSource(getSource());
    bool wasActive = phantom::lang::detail::isActive();
    if (!wasActive)
        phantom::lang::detail::pushInstallation();
    phantom::TypeInstallationDelegate func = m_OnDemandMembersFunc;
    m_OnDemandMembersFunc = nullptr;
    func(phantom::TypeInstallationStep::Members);
    if (!wasActive)
        phantom::lang::detail::popInstallation();
    if (!pCurrentSource)
        phantom::lang::detail::popSource();
}

void ClassType::_onNativeElementsAccess()
{
    if (!(m_OnDemandMembersFunc.empty()) /*&& !isFinalized()*/ && ((getFlags() & PHANTOM_R_FLAG_TERMINATED) == 0))
    {
        auto guard = m_OnDemandMutex.autoLock();
        if (!(m_OnDemandMembersFunc.empty()) /*&& !isFinalized()*/ && ((getFlags() & PHANTOM_R_FLAG_TERMINATED) == 0))
        {
            Module* pThisModule = getModule();
            PHANTOM_ASSERT(pThisModule);
            phantom::lang::detail::pushModule(pThisModule);
            _onNativeElementsAccessImpl();
            phantom::lang::detail::popModule();
        }
    }
}

void ClassType::onElementsAccess()
{
    PHANTOM_ASSERT(!isNative());
}

Destructor* ClassType::getDestructor() const
{
    return static_cast<Destructor*>(getMethod("~" + String(getName()), Types{}));
}

bool ClassType::isDefaultInstanciable() const
{
    return (getTemplateSpecialization() == nullptr || getTemplateSpecialization()->isFull());
}

size_t ClassType::getFieldIndex(Field* a_pField) const
{
    size_t i = 0;
    for (auto it = m_Fields->begin(); it != m_Fields->end(); ++it)
    {
        if (a_pField == *it)
            return i;
        ++i;
    }
    return ~size_t(0);
}

Field* ClassType::getFieldByOffset(size_t a_uiOffset) const
{
    for (auto pDM : *m_Fields)
    {
        if (pDM->getOffset() == a_uiOffset)
            return pDM;
    }
    return nullptr;
}

Field* ClassType::getFieldByAddress(const void* a_pBase, const void* a_pAddress) const
{
    return getFieldByOffset((size_t)((const byte*)a_pAddress - (const byte*)a_pBase));
}

Field* ClassType::getFieldAtOffset(size_t a_uiOffset) const
{
    for (auto pDM : *m_Fields)
    {
        if ((a_uiOffset >= pDM->getOffset()) &&(a_uiOffset < (pDM->getOffset() + pDM->getValueType()->getSize())))
            return pDM;
    }
    return nullptr;
}

Field* ClassType::getFieldAtAddress(const void* a_pBase, const void* a_pAddress) const
{
    return getFieldAtOffset((size_t)((const byte*)a_pAddress - (const byte*)a_pBase));
}

bool ClassType::isListInitializable() const
{
    for (auto pCtor : *m_Constructors)
    {
        if ((!(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pCtor->testModifiers(PHANTOM_R_EXPLICIT)
             || pCtor->testFlags(PHANTOM_R_FLAG_INHERITED))&& !(pCtor->testModifiers(Modifier::Deleted)))
        {
            return false;
        }
    }
    for (auto pDM : getFields())
    {
        if (pDM->isPrivate() || pDM->isProtected())
        {
            return false;
        }
    }
    return true;
}

void ClassType::setDefaultAccess(Access a_eAccess)
{
    PHANTOM_ASSERT(a_eAccess == Access::Public || a_eAccess == Access::Protected || a_eAccess == Access::Private);
    m_DefaultAccess = a_eAccess;
}

void ClassType::addSubroutine(Subroutine* a_pSubroutine)
{
    if (a_pSubroutine->asMethod())
        addMethod(a_pSubroutine->asMethod());
    else
        Scope::addSubroutine(a_pSubroutine);
}

bool ClassType::isSame(LanguageElement* a_pElement) const
{
    return Type::isSame(a_pElement);
}

bool ClassType::isSame(Symbol* a_pSymbol) const
{
    return Type::isSame(a_pSymbol);
}

bool ClassType::hasStrongDependencyOnType(Type* a_pType) const
{
    for (auto pField : getFields())
    {
        if (pField->getValueType()->removeQualifiers()->removeArray() == a_pType)
            return true;
    }
    return false;
}

void ClassType::finalizeNative()
{
    PHANTOM_ASSERT_NO_IMPL();
}

void ClassType::getFields(AggregateFields& _fields) const
{
    for (auto pField : getFields())
    {
        AggregateField aField;
        aField.type = pField->getValueType();
        aField.offset = pField->getOffset();
        _fields.push_back(aField);
    }
}

void ClassType::getFlattenedAggregateFields(AggregateFields& _aggregateFields) const
{
    Aggregate::getFlattenedFields(_aggregateFields);
}

void ClassType::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
    auto guard = m_OnDemandMutex.autoLock();
    if (!isNative() ||
        m_OnDemandMembersFunc.empty()) // if not native or if native, check if member installation has been done
    {
        auto friendFound = std::find(m_Friends->begin(), m_Friends->end(), static_cast<Symbol*>(a_pElement));
        if (friendFound != m_Friends->end())
        {
            m_Friends->erase(friendFound);
        }
    }
}

TemplateSpecialization* ClassType::getTemplateSpecialization() const
{
    return Type::getTemplateSpecialization();
}

TemplateSpecialization* ClassType::getTemplateSpecialization(StringView              a_strTemplateName,
                                                             const LanguageElements& a_Arguments) const
{
    return Scope::getTemplateSpecialization(a_strTemplateName, a_Arguments);
}

void ClassType::addFriend(Symbol* a_pFriend)
{
    PHANTOM_ASSERT(a_pFriend);
    PHANTOM_ASSERT(!hasFriend(a_pFriend));
    m_Friends.push_back(a_pFriend);
    addReferencedElement(a_pFriend);
}

bool ClassType::hasFriend(Symbol* a_pFriend) const
{
    return std::find(m_Friends->begin(), m_Friends->end(), a_pFriend) != m_Friends->end();
}

void ClassType::addStaticField(StaticField* a_pField)
{
    Scope::addVariable(a_pField);
}

void ClassType::addMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection)
{
    addSymbol(a_pMemberAnonymousSection);
}

void ClassType::removeMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection)
{
    removeElement(a_pMemberAnonymousSection);
}

// ex: union(m, struct(x, y, union(z, w)))
MemberAnonymousSection* ClassType::addMemberAnonymousSection(StringView a_strCode, Modifiers /*= 0*/, uint /*= 0*/)
{
    String str = a_strCode;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    enum state
    {
        state_union_or_struct,
        state_list,
        state_ended,
    };
    state                   s = state_union_or_struct;
    String                  word;
    MemberAnonymousSections sections;
    MemberAnonymousSection* pSection = nullptr;
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        char c = *it;
        switch (s)
        {
        case state_union_or_struct:
            if (PHANTOM_CHAR_IS_CPP_IDENTIFIER(c))
            {
                word += c;
            }
            else if (c == '(')
            {
                if (word == "union")
                {
                    sections.push_back(PHANTOM_DEFERRED_NEW_EX(MemberAnonymousUnion));
                }
                else if (word == "struct")
                {
                    sections.push_back(PHANTOM_NEW(MemberAnonymousStruct));
                }
                else
                {
                    PHANTOM_DELETE_DYN(sections.front());
                    return nullptr;
                }
                s = state_list;
                word.clear();
            }
            else
            {
                PHANTOM_DELETE_DYN(sections.front());
                return nullptr;
            }
            break;

        case state_list:
            if (PHANTOM_CHAR_IS_CPP_IDENTIFIER(c))
            {
                word += c;
            }
            else if (c == '(')
            {
                if (word == "union")
                {
                    sections.push_back(PHANTOM_NEW(MemberAnonymousUnion));
                }
                else if (word == "struct")
                {
                    sections.push_back(PHANTOM_NEW(MemberAnonymousStruct));
                }
                else
                {
                    if (sections.size())
                    {
                        PHANTOM_DELETE_DYN(sections.front());
                    }
                    return nullptr;
                }
                word.clear();
            }
            else if (c == ',')
            {
                if (word.size())
                {
                    if (sections.empty())
                        return nullptr;
                    Field* pElement = getField(word);
                    if (pElement == nullptr)
                    {
                        if (sections.size())
                        {
                            PHANTOM_DELETE_DYN(sections.front());
                        }
                        return nullptr;
                    }
                    sections.back()->addField(pElement);
                }
                // data member
                word.clear();
            }
            else if (c == ')')
            {
                if (sections.empty())
                    return nullptr;
                pSection = sections.back();
                sections.pop_back();
                if (sections.empty())
                {
                    addMemberAnonymousSection(pSection);
                    s = state_ended;
                }
                else
                {
                    sections.back()->addMemberAnonymousSection(pSection);
                    s = state_list;
                }
            }
            else
            {
                if (sections.size())
                {
                    PHANTOM_DELETE_DYN(sections.front());
                }
                return nullptr;
            }
            break;

        case state_ended:
            if (sections.size())
            {
                PHANTOM_DELETE_DYN(sections.front());
            }
            return nullptr;
        }
    }
    return pSection;
}

bool ClassType::acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers a_Modifiers,
                                  uint a_uiFlags, Subroutines* a_pOutConflictingSubroutines /*= NULL*/) const
{
    bool bResult = true;
    {
        auto it = m_Methods->begin();
        auto end = m_Methods->end();
        for (; it != end; ++it)
        {
            Method*            pMethod = *it;
            ESignatureRelation eOR =
            pMethod->getSignatureRelationWith(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags);
            if (eOR != e_SignatureRelation_None)
            {
                if (a_pOutConflictingSubroutines)
                {
                    a_pOutConflictingSubroutines->push_back(pMethod);
                    bResult = false;
                }
                else
                    return false;
            }
        }
    }
    return Scope::acceptsSubroutine(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags,
                                    a_pOutConflictingSubroutines) && bResult;
}

Constructor* ClassType::getCopyConstructor() const
{
    Types types;
    types.push_back(addConst()->addLValueReference());
    Constructor* pCopyCtor = getConstructor(types);
    if (pCopyCtor == nullptr)
    {
        types[0] = const_cast<ClassType*>(this);
        pCopyCtor = getConstructor(types);
    }
    return pCopyCtor;
}

Constructor* ClassType::getMoveConstructor() const
{
    Types types;
    types.push_back(addRValueReference());
    return getConstructor(types);
}

bool ClassType::isDefaultConstructible() const
{
    return ((getFlags() & PHANTOM_R_FLAG_NO_DEFAULT_CTOR) == 0) && getDefaultConstructor() != nullptr;
}

bool ClassType::canHaveImplicitDefaultConstructor() const
{
    if (m_Constructors->empty()) /// we can only have a default constructor is no constructor has
                                 /// already been declared
    {
        for (auto pDM : *m_Fields)
        {
            if (pDM->getValueType()->asReference() || !(pDM->getValueType()->isDefaultConstructible()))
                return false;
        }
        return true;
    }
    return false;
}

bool ClassType::canHaveImplicitCopyConstructor() const
{
    for (auto pDM : *m_Fields)
    {
        if (pDM->getValueType()->asReference() || !(pDM->getValueType()->isCopyable()))
            return false;
    }
    return true;
}

bool ClassType::canHaveImplicitCopyAssignmentOperator() const
{
    for (Field* pDM : *m_Fields)
    {
        if (pDM->getValueType()->asReference() || !(pDM->getValueType()->isCopyAssignable()))
            return false;
    }
    return true;
}

bool ClassType::canHaveImplicitMoveConstructor() const
{
    if (Constructor* pCtor = getCopyConstructor())
    {
        if (!pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Destructor* pDtor = getDestructor())
    {
        if (!pDtor->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Method* pCopyAssign = getCopyAssignmentOperator())
    {
        if (!pCopyAssign->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Method* pMoveAssign = getMoveAssignmentOperator())
    {
        if (!pMoveAssign->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    for (Field* pDM : *m_Fields)
    {
        if (pDM->getValueType()->asReference() || !(pDM->getValueType()->isMoveable()))
            return false;
    }
    return true;
}

bool ClassType::canHaveImplicitMoveAssignmentOperator() const
{
    if (Constructor* pCtor = getCopyConstructor())
    {
        if (!pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Destructor* pDtor = getDestructor())
    {
        if (!pDtor->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Method* pCopyAssign = getCopyAssignmentOperator())
    {
        if (!pCopyAssign->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    if (Method* pMoveAssign = getMoveAssignmentOperator())
    {
        if (!pMoveAssign->testFlags(PHANTOM_R_FLAG_IMPLICIT))
            return false;
    }
    for (Field* pDM : *m_Fields)
    {
        if (pDM->getValueType()->asReference() || !(pDM->getValueType()->isMoveAssignable()))
            return false;
    }
    return true;
}

bool ClassType::isCopyable() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getCopyConstructor() != nullptr && getCopyAssignmentOperator() != nullptr;
}

bool ClassType::isCopyAssignable() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getCopyAssignmentOperator() != nullptr;
}

bool ClassType::isCopyConstructible() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getCopyConstructor() != nullptr;
}

bool ClassType::isMoveable() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getMoveConstructor() != nullptr && getMoveAssignmentOperator() != nullptr;
}

bool ClassType::isMoveAssignable() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getMoveAssignmentOperator() != nullptr;
}

bool ClassType::isMoveConstructible() const
{
    PHANTOM_ASSERT(m_pExtraData);
    return getMoveConstructor() != nullptr;
}

bool ClassType::isTemplateInstance() const
{
    return Type::getTemplateSpecialization() != NULL;
}

Method* ClassType::addDestructor(Modifiers a_Modifiers /*= 0 */, uint a_uiFlags /*= 0*/)
{
    PHANTOM_ASSERT(!isNative(), "cannot add manually destructor to native types");
    PHANTOM_ASSERT(!getDestructor(), "destructor already added");
    Destructor* pDestructor =
    PHANTOM_NEW(Destructor)('~' + getName(), Signature::Create(PHANTOM_TYPEOF(void)), a_Modifiers, a_uiFlags);
    addMethod(pDestructor);
    pDestructor->setAccess(m_pExtraData ? getDefaultAccess() : Access::Public);
    return pDestructor;
}

void ClassType::ExtraData::PHANTOM_CUSTOM_VIRTUAL_DELETE()
{
    PHANTOM_DELETE(ExtraData) this;
}

} // namespace lang
} // namespace phantom
