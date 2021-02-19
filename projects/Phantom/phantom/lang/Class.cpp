// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Class.h"

#include "Application.h"
#include "Array.h"
#include "ConstType.h"
#include "Constructor.h"
#include "Field.h"
#include "InstanceCache.h"
#include "LValueReference.h"
#include "MapClass.h"
#include "Package.h"
#include "Parameter.h"
#include "Pointer.h"
#include "RValueReference.h"
#include "Signal.h"
#include "Signature.h"
#include "VirtualMethodTable.h"
#include "registration/registration.h"

#include <phantom/lang/Function.h> // phantom::lang::detail::pushModule
#include <phantom/lang/Module.h>   // phantom::lang::detail::pushModule
#include <phantom/lang/registration/Main.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/StringUtil.h>

namespace phantom
{
namespace detail
{
extern lang::ClassHookFunc g_InstanceHook_func;
}

namespace lang
{
static_assert(IsObject<Class>::value, "HasRtti<Class>::value");

Class* Class::metaClass;

Class* Class::MetaClass()
{
    return Class::metaClass;
}

Class::Class() : ClassType(TypeKind::Class) {}

Class::Class(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(a_eTypeKind, a_strName, a_Modifiers, a_uiFlags)
{
}

Class::Class(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ClassType(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

void Class::initialize()
{
    ClassType::initialize();
    if (!isNative())
        setExtraData(new_<ExtraData>());
}

void Class::terminate()
{
    if (m_pExtraData)
        delete_<ExtraData>(static_cast<ExtraData*>(m_pExtraData));
    if (m_pInstanceCache)
        delete_<InstanceCache>(m_pInstanceCache);

    if (phantom::detail::g_InstanceHook_func)
        phantom::detail::g_InstanceHook_func(ClassHookOp::ClassDestroying, this, nullptr);

    if (m_InstanceCount)
    {
        PHANTOM_LOG(Warning, "%s : %d instance(s) of this class have not been deleted",
                    getQualifiedDecoratedName().c_str(), int(m_InstanceCount));
    }

    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->removeDerivedClass(this);
    }
    m_BaseClasses.clear();

    ClassType::terminate();
}

Class* Class_getCommonBaseClass(Class const* a_pThis, Class* a_pClass)
{
    if (a_pThis == a_pClass)
        return a_pClass;
    if (a_pThis->hasBaseClassCascade(a_pClass))
        return a_pClass;
    for (Class* pBaseClass : a_pClass->getBaseClasses())
    {
        if (Class* pCommon = Class_getCommonBaseClass(a_pThis, pBaseClass))
            return pCommon;
    }
    return nullptr;
}

Class* Class::getCommonBaseClass(Class* a_pClass) const
{
    if (a_pClass == this)
        return a_pClass;

    if (a_pClass->hasBaseClassCascade((Class*)this))
        return (Class*)this;

    return Class_getCommonBaseClass(this, a_pClass);
}

Field* Class::getFieldCascade(StringView a_strName) const
{
    Field* pField = getField(a_strName);
    if (pField != NULL)
        return pField;
    for (Class* pClass : m_BaseClasses)
    {
        Field* pBaseField = pClass->getFieldCascade(a_strName);
        if (pBaseField != NULL)
        {
            if (pField != NULL)
            {
                // ambiguous access, so we return nothing
                return NULL;
            }
            pField = pBaseField;
        }
    }
    return pField;
}

Property* Class::getPropertyCascade(StringView a_strPropertyName) const
{
    Property* pProperty = getProperty(a_strPropertyName);
    if (pProperty != NULL)
        return pProperty;
    for (Class* pClass : m_BaseClasses)
    {
        Property* pBaseProperty = pClass->getPropertyCascade(a_strPropertyName);
        if (pBaseProperty != NULL)
        {
            if (pProperty != NULL)
            {
                // ambiguous access, so we return nothing
                return NULL;
            }
            pProperty = pBaseProperty;
        }
    }
    return pProperty;
}

StaticField* Class::getStaticFieldCascade(StringView a_strName) const
{
    StaticField* pField = getStaticField(a_strName);
    if (pField != NULL)
        return pField;
    for (Class* pClass : m_BaseClasses)
    {
        StaticField* pBaseField = pClass->getStaticFieldCascade(a_strName);
        if (pBaseField != NULL)
        {
            if (pField != NULL)
            {
                // ambiguous access, so we return nothing
                return NULL;
            }
            pField = pBaseField;
        }
    }
    return pField;
}

void Class::_addBaseClass(Class* a_pBaseClass, size_t a_uiOffset, Access a_Access /*= Access::Public*/)
{
    PHANTOM_ASSERT(a_pBaseClass);
    PHANTOM_ASSERT(!(a_pBaseClass->isSame(this)));
    PHANTOM_ASSERT(isTemplateDependant() || a_pBaseClass->isTemplateDependant() || !(a_pBaseClass->isA(this)));
    PHANTOM_ASSERT(isTemplateDependant() || a_pBaseClass->isTemplateDependant() || !(this->isA(a_pBaseClass)));
    PHANTOM_ASSERT(isNative() || !(a_pBaseClass->hasStrongDependencyOnType(this)), "cyclic class strong dependency");
    m_BaseClasses.push_back(BaseClass(a_pBaseClass, a_uiOffset, a_Access));
    a_pBaseClass->addDerivedClass(const_cast<Class*>(this));
    addReferencedElement(a_pBaseClass);
}

void Class::addBaseClass(Class* a_pClass, Access a_Access)
{
    ExtraData* pExtraData = getExtraData();
    PHANTOM_ASSERT(pExtraData);
    PHANTOM_ASSERT(!isSized(), "class has been sized, cannot add base class anymore");
    _addBaseClass(a_pClass, ~size_t(0), a_Access);
}

void Class::addBaseClass(Class* a_pBaseClass, size_t a_uiOffset, Access a_Access /*= Access::Public*/)
{
    _addBaseClass(a_pBaseClass, a_uiOffset, a_Access);
}

bool Class::hasBaseClass(Class* a_pClass, Access* a_pInheritanceAccess) const
{
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass == a_pClass)
        {
            if (a_pInheritanceAccess)
            {
                if (bc.access < *a_pInheritanceAccess)
                    *a_pInheritanceAccess = bc.access;
            }
            return true;
        }
    }
    return false;
}

bool Class::hasBaseClassCascade(Class* a_pClass) const
{
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass == a_pClass)
            return true;
        if (bc.baseClass->hasBaseClassCascade(a_pClass))
            return true;
    }
    return false;
}

void Class::addDerivedClass(Class* a_pType)
{
    m_DerivedClasses.push_back(a_pType);
#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
    _addDerivedClassRecursive(a_pType);
#endif
}

void Class::removeDerivedClass(Class* a_pType)
{
#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
    _removeDerivedClassRecursive(a_pType);
#endif
    m_DerivedClasses.erase(std::find(m_DerivedClasses.begin(), m_DerivedClasses.end(), a_pType));
}

#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
void Class::_addDerivedClassRecursive(Class* a_pType)
{
    m_RecurseDerivedClasses.push_back(a_pType);
    for (auto pDerived : a_pType->m_RecurseDerivedClasses)
    {
        m_RecurseDerivedClasses.push_back(pDerived);
    }
    for (Class* pBase : m_BaseClasses)
    {
        pBase->_addDerivedClassRecursive(a_pType);
    }
}

void Class::_removeDerivedClassRecursive(Class* a_pType)
{
    for (Class* pBase : m_BaseClasses)
    {
        pBase->_removeDerivedClassRecursive(a_pType);
    }
    for (auto pDerived : a_pType->m_RecurseDerivedClasses)
    {
        m_RecurseDerivedClasses.erase(
        std::find(m_RecurseDerivedClasses.begin(), m_RecurseDerivedClasses.end(), pDerived));
    }
    m_RecurseDerivedClasses.erase(std::find(m_RecurseDerivedClasses.begin(), m_RecurseDerivedClasses.end(), a_pType));
}
#endif

bool Class::hasDerivedClass(Class* a_pClass, Access* a_pDerivedAccess /*= 0*/) const
{
    uint i = 0;
    for (; i < m_DerivedClasses.size(); ++i)
    {
        if (m_DerivedClasses[i] == a_pClass)
        {
            if (a_pDerivedAccess)
            {
                *a_pDerivedAccess = a_pClass->getBaseClassAccess(const_cast<Class*>(this));
            }
            return true;
        }
    }
    return false;
}

void Class::sortBaseAndRootMethods(VirtualMethodTable* a_pBaseVMT, Methods* a_OutBaseMethods,
                                   Methods* a_OutRootMethods) const
{
    uint i = 0;
    for (; i < a_pBaseVMT->getMethodCount(); ++i)
    {
        Method* pBaseMethod = a_pBaseVMT->getMethod(i);
        for (Method* pMethod : getMethods())
        {
            if (pMethod->isVirtual())
            {
                ESignatureRelation eIR = pMethod->getSignatureRelationWith(pBaseMethod);
                PHANTOM_ASSERT(eIR != e_SignatureRelation_Contravariant && eIR != e_SignatureRelation_Forbidden);
                if (eIR == e_SignatureRelation_None)
                {
                    a_OutRootMethods->push_back(pMethod);
                }
                else
                {
                    a_OutBaseMethods->push_back(pMethod);
                }
            }
        }
    }
}

Class::TypeRelation Class::getRelationWith(Type* a_pType) const
{
    if (a_pType == this)
        return TypeRelation::Equal;
    if (a_pType->asClass() == nullptr)
        return TypeRelation::None;
    if (this->isA((Class*)a_pType))
        return TypeRelation::Child;
    if (static_cast<Class*>(a_pType)->isA(const_cast<Class*>(this)))
        return TypeRelation::Parent;

    return TypeRelation::None;
}

void Class::findOverriddenMethods(StringView a_strName, Signature* a_pSignature, Methods& a_Result)
{
    for (auto pMethod : getMethods())
    {
        if (pMethod->isOverridableBy(a_strName, a_pSignature))
        {
            bool bOverloadedAlreadyAddedForCurrentVTable = false;
            auto it_result = a_Result.begin();
            auto end_result = a_Result.end();
            for (; it_result != end_result; ++it_result)
            {
                Method* pResult = (*it_result);
                PHANTOM_ASSERT(pResult->getOwnerClass());
                if (pResult->getOwnerClass()->getBaseClassOffsetCascade((Class*)this) == 0 // in the same vtable
                    && pMethod->isOverridableBy(pResult)) // a result member function can overload the current tested
                                                          // member function, which means that for the current VTable,
                                                          // we already have found an overloaded member function
                {
                    bOverloadedAlreadyAddedForCurrentVTable = true;
                    break;
                }
            }
            if (!(bOverloadedAlreadyAddedForCurrentVTable))
            {
                a_Result.push_back(pMethod);
            }
        }
    }
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->findOverriddenMethods(a_strName, a_pSignature, a_Result);
    }
}

void Class::findOverriddenMethods(Method* a_pOverridingCandidate, Methods& a_Result)
{
    for (Method* pMethod : getMethods())
    {
        if (pMethod == a_pOverridingCandidate)
            continue;
        if (a_pOverridingCandidate->canOverride(pMethod))
        {
            bool bOverloadedAlreadyAddedForCurrentVTable = false;
            for (Method* pResult : a_Result)
            {
                PHANTOM_ASSERT(pResult->getOwnerClass());
                if (pResult->getOwnerClass()->getBaseClassOffsetCascade((Class*)this) == 0 // in the same vtable
                    && pResult->canOverride(pMethod)) // a result member_function can overload the current tested
                                                      // member_function, which means that for the current VTable, we
                                                      // already have found an overloaded member_function
                {
                    bOverloadedAlreadyAddedForCurrentVTable = true;
                    break;
                }
            }
            if (!(bOverloadedAlreadyAddedForCurrentVTable))
            {
                a_Result.push_back(pMethod);
            }
        }
    }
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->findOverriddenMethods(a_pOverridingCandidate, a_Result);
    }
}

size_t Class::getVirtualMethodTableSize(size_t a_uiIndex) const
{
    return m_VirtualMethodTables[a_uiIndex]->getMethodCount();
}

bool Class::acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers a_Modifiers,
                              uint a_uiFlags, Subroutines* a_pOutConflictingMethods) const
{
    bool bResult =
    ClassType::acceptsSubroutine(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags, a_pOutConflictingMethods);
    if (!bResult)
        return false;
    for (auto& bc : m_BaseClasses)
    {
        if (!(bc.baseClass->acceptsDerivedClassSubroutine(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags,
                                                          a_pOutConflictingMethods)))
        {
            return false;
        }
    }
    return true;
}

bool Class::acceptsDerivedClassSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types,
                                          Modifiers a_Modifiers, uint a_uiFlags,
                                          Subroutines* a_pOutConflictingMethods) const
{
    Subroutines prevMethods;
    bool        bResult = true;
    {
        for (auto& bc : m_BaseClasses)
        {
            if (!(bc.baseClass->acceptsDerivedClassSubroutine(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags,
                                                              &prevMethods)))
            {
                bResult = false;
            }
        }
    }

    {
        for (Method* pMethod : getMethods())
        {
            if (!(pMethod->isVirtual()))
                continue;
            ESignatureRelation eOR =
            pMethod->getSignatureRelationWith(a_pReturnType, a_strName, a_Types, a_Modifiers, a_uiFlags);
            if (eOR == e_SignatureRelation_Forbidden ||
                eOR == e_SignatureRelation_Covariant) // if base member function has covariance with child
                                                      // one, it means child one has contravariant relation
                                                      // => not accepted in C++
            {
                if (a_pOutConflictingMethods)
                {
                    a_pOutConflictingMethods->push_back(pMethod);
                    bResult = false;
                }
                else
                    return false;
            }
        }
    }
    if (a_pOutConflictingMethods)
    {
        auto it = prevMethods.begin();
        for (; it != prevMethods.end(); ++it)
        {
            a_pOutConflictingMethods->push_back(*it);
        }
    }

    return bResult;
}

void Class::filtersNonOverloadedPureVirtualMethodsCascade(Methods& a_Result) const
{
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->filtersNonOverloadedPureVirtualMethodsCascade(a_Result);
    }
    SmallVector<Methods::iterator> toRemove;
    Methods::iterator              it_abs = a_Result.begin();
    for (; it_abs != a_Result.end();)
    {
        bool    anyRemoved = false;
        Method* pPureVirtualMethod = *it_abs;
        for (Method* pMethod : getMethods())
        {
            if (pMethod->isAbstract())
                continue;
            ESignatureRelation eOR = pMethod->getSignatureRelationWith(pPureVirtualMethod);
            PHANTOM_ASSERT(eOR != e_SignatureRelation_Forbidden);
            if (eOR != e_SignatureRelation_None)
            {
                anyRemoved = true;
                it_abs = a_Result.erase(it_abs);
                break;
            }
        }
        if (!anyRemoved)
            ++it_abs;
    }

    findPureVirtualMethods(a_Result);
}

void Class::findPureVirtualMethods(Methods& a_Result) const
{
    for (auto pMethod : getMethods())
    {
        if (pMethod->isAbstract())
        {
            a_Result.push_back(pMethod);
        }
    }
}

StaticMethod* Class::getStaticMethodCascade(StringView a_strIdentifierString) const
{
    StaticMethod* pMethod = getStaticMethod(a_strIdentifierString);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getStaticMethodCascade(a_strIdentifierString);
        if (pMethod != NULL)
            return pMethod;
    }
    return NULL;
}

StaticMethod* Class::getStaticMethodCascade(StringView a_strName, TypesView a_FunctionSignature) const
{
    StaticMethod* pMethod = getStaticMethod(a_strName, a_FunctionSignature);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getStaticMethodCascade(a_strName, a_FunctionSignature);
        if (pMethod != NULL)
            return pMethod;
    }
    return nullptr;
}

Method* Class::getMethodCascade(StringView a_strIdentifierString) const
{
    Method* pMethod = getMethod(a_strIdentifierString);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getMethodCascade(a_strIdentifierString);
        if (pMethod != NULL)
            return pMethod;
    }
    return NULL;
}

Method* Class::getMethodCascade(StringView a_strName, TypesView a_ParameterTypes, Modifiers a_Modifiers /*= 0*/) const
{
    Method* pMethod = getMethod(a_strName, a_ParameterTypes, a_Modifiers);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getMethodCascade(a_strName, a_ParameterTypes, a_Modifiers);
        if (pMethod != NULL)
            return pMethod;
    }
    return NULL;
}

Method* Class::getMethodCascade(StringView a_strName, Type* a_pSingleParameterType,
                                Modifiers a_Qualifiers /*= 0*/) const
{
    return getMethodCascade(a_strName, TypesView{&a_pSingleParameterType, 1}, a_Qualifiers);
}

Subroutine* Class::getSubroutineCascade(StringView a_strIdentifierString) const
{
    Subroutine* pMethod = getSubroutine(a_strIdentifierString);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getSubroutineCascade(a_strIdentifierString);
        if (pMethod != NULL)
            return pMethod;
    }
    return NULL;
}

Subroutine* Class::getSubroutineCascade(StringView a_strName, TypesView a_FunctionSignature,
                                        Modifiers a_Modifiers /*= 0*/) const
{
    Subroutine* pMethod = getSubroutine(a_strName, a_FunctionSignature, a_Modifiers);
    if (pMethod != NULL)
        return pMethod;
    for (auto& bc : m_BaseClasses)
    {
        pMethod = bc.baseClass->getSubroutineCascade(a_strName, a_FunctionSignature, a_Modifiers);
        if (pMethod != NULL)
            return pMethod;
    }
    return NULL;
}

ValueMember* Class::getValueMemberCascade(StringView a_strIdentifierString) const
{
    ValueMember* pFound = getValueMember(a_strIdentifierString);
    if (pFound != NULL)
        return pFound;

    for (auto& bc : m_BaseClasses)
    {
        ValueMember* pValueMember = bc.baseClass->getValueMemberCascade(a_strIdentifierString);
        if (pValueMember != nullptr)
            return pValueMember;
    }
    return nullptr;
}

void Class::findPublicValueMembersPointingValueTypeCascade(Type* a_pType, ValueMembers& out) const
{
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->findPublicValueMembersPointingValueTypeCascade(a_pType, out);
    }
    findPublicValueMembersPointingValueType(a_pType, out);
}

void Class::getValueMembersCascade(ValueMembers& out) const
{
    out.insert(out.end(), getValueMembers().begin(), getValueMembers().end());
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->getValueMembersCascade(out);
    }
}

/* PHANTOM SPECIFIC EXTENSION */

void* Class::upcast(Class* a_pBaseClass, void* a_pInstance) const
{
    if (a_pInstance == nullptr)
        return nullptr;
    if (a_pBaseClass == this)
        return a_pInstance;
    ptrdiff_t offset = getBaseClassOffsetCascade(a_pBaseClass);
    if (offset == -1)
        return nullptr;
    return static_cast<byte*>(a_pInstance) + offset;
}

void const* Class::upcast(Class* a_pBaseClass, void const* a_pInstance) const
{
    return const_cast<void const*>(upcast(a_pBaseClass, const_cast<void*>(a_pInstance)));
}

void* Class::downcast(Class* a_pDerivedClass, void* a_pInstance) const
{
    if (a_pInstance == nullptr)
        return nullptr;
    if (a_pDerivedClass == this)
        return a_pInstance;
    ptrdiff_t rev_offset = a_pDerivedClass->getBaseClassOffsetCascade(const_cast<Class*>(this));
    if (rev_offset == -1)
        return nullptr;
    return static_cast<byte*>(a_pInstance) - rev_offset;
}

void const* Class::downcast(Class* a_pDerivedClass, void const* a_pInstance) const
{
    return const_cast<void const*>(downcast(a_pDerivedClass, const_cast<void*>(a_pInstance)));
}

void* Class::cast(Class* a_pBaseOrDerivedClass, void* a_pInstance) const
{
    if (a_pInstance == nullptr)
        return nullptr;
    if (a_pBaseOrDerivedClass == this)
        return a_pInstance;
    ptrdiff_t offset = getBaseClassOffsetCascade(a_pBaseOrDerivedClass);
    if (offset == -1)
    {
        ptrdiff_t rev_offset = a_pBaseOrDerivedClass->getBaseClassOffsetCascade(const_cast<Class*>(this));
        if (rev_offset == -1)
            return nullptr;
        return static_cast<byte*>(a_pInstance) - rev_offset;
    }
    return static_cast<byte*>(a_pInstance) + offset;
}

void const* Class::cast(Class* a_pBaseOrDerivedClass, void const* a_pInstance) const
{
    return const_cast<void const*>(cast(a_pBaseOrDerivedClass, const_cast<void*>(a_pInstance)));
}

void* Class::cast(Type* a_pTarget, void* a_pBase) const
{
    if (a_pTarget->asClass())
        return cast(static_cast<Class*>(a_pTarget), a_pBase);
    return nullptr;
}

void const* Class::cast(Type* a_pTargetType, void const* a_pInstance) const
{
    return const_cast<void const*>(cast(a_pTargetType, const_cast<void*>(a_pInstance)));
}

void* Class::newInstance() const
{
    void* pInstance = allocate();
    construct(pInstance);
    return pInstance;
}

void Class::deleteInstance(void* a_pInstance) const
{
    destroy(a_pInstance);
    deallocate(a_pInstance);
}

void* Class::upcast(Type* a_pTarget, void* a_pBase) const
{
    if (a_pTarget->asClass())
        return upcast(static_cast<Class*>(a_pTarget), a_pBase);
    return nullptr;
}

void* Class::downcast(Type* a_pTarget, void* a_pBase) const
{
    if (a_pTarget->asClass())
        return downcast(static_cast<Class*>(a_pTarget), a_pBase);
    return nullptr;
}

void* Class::placementNewInstance(void* a_pMemory) const
{
    construct(a_pMemory);
    return a_pMemory;
}

void* Class::placementNewInstance(void* a_pMemory, Constructor* a_pConstructor, void** a_pArgs) const
{
    a_pConstructor->construct(a_pMemory, a_pArgs);
    return a_pMemory;
}

void Class::placementDeleteInstance(void* a_pInstance) const
{
    destroy(a_pInstance);
}

bool Class::isA(Class* a_pType) const
{
    if (a_pType == this)
        return true;
    if (getTemplateSpecialization() && a_pType->getTemplateSpecialization() && isSame(a_pType))
        return true;
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass->isA(a_pType))
            return true;
    }
    return false;
}

const Variant& Class::getMetaDataIncludingBases(StringWithHash a_Hash) const
{
    static Variant null;
    const Variant& v = getMetaData(a_Hash);
    if (v.isValid())
        return v;
    for (auto& bc : m_BaseClasses)
    {
        const Variant& v2 = bc.baseClass->getMetaDataIncludingBases(a_Hash);
        if (v2.isValid())
            return v2;
    }
    return null;
}

const Variant& Class::getMetaDataIncludingBases(StringView a_Name) const
{
    return getMetaDataIncludingBases(StringWithHash(a_Name));
}

void Class::getMetaDatasIncludingBases(StringView a_Name, Variants& a_MetaDatas) const
{
    getMetaDatasIncludingBases(StringWithHash(a_Name), a_MetaDatas);
}

void Class::getMetaDatasIncludingBases(StringWithHash a_Hash, Variants& a_MetaDatas) const
{
    auto& meta = getMetaData(a_Hash);
    if (meta.isValid())
    {
        a_MetaDatas.push_back(meta);
    }
    for (auto& bc : m_BaseClasses)
    {
        bc.baseClass->getMetaDatasIncludingBases(a_Hash, a_MetaDatas);
    }
}

bool Class::canBeDestroyed() const
{
    return ClassType::canBeDestroyed() && m_DerivedClasses.size() == 0 &&
    (getInstanceCount() == 0 || (this == PHANTOM_TYPEOF(Class) && getInstanceCount() == 1));
}

void Class::getFields(AggregateFields& a_OutFields) const
{
    size_t firstField = a_OutFields.size();
    for (BaseClass const& bc : getBaseClasses())
    {
        size_t i = 0;
        bc.baseClass->getFields(a_OutFields);
        for (; i < a_OutFields.size(); ++i)
        {
            a_OutFields[i].offset += bc.offset;
        }
    }
    if (isPolymorphic())
    {
        if (a_OutFields.size() && a_OutFields[firstField].type == nullptr) // vtable already set
        {
            PHANTOM_ASSERT(a_OutFields[firstField].offset == 0);
        }
        else
        {
            a_OutFields.insert(a_OutFields.begin(), AggregateField()); // insert vtable descriptor
        }
    }
    ClassType::getFields(a_OutFields);
}

void Class::_onNativeElementsAccess()
{
    // TODO : check this is this worthy
    // 	Module* pModule;
    // 	if (Application::Get() && !(Application::Get()->testFlags(PHANTOM_R_FLAG_TERMINATED)) && (pModule = getModule())
    // &&
    // 		!(pModule->testFlags(PHANTOM_R_FLAG_TERMINATED)))
    // 	{
    // 		onElementsAccess();
    // 	}
    if (!(m_OnDemandMembersFunc.empty()) /*&& !isFinalized()*/ &&
        ((m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATING) == 0))
    {
        auto guard = m_OnDemandMutex.autoLock();
        if (!(m_OnDemandMembersFunc.empty()) /*&& !isFinalized()*/ &&
            ((m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATING) == 0))
        {
            Module* pThisModule = getModule();
            PHANTOM_ASSERT(pThisModule);
            phantom::lang::detail::pushModule(pThisModule);
            _onNativeElementsAccessImpl();
            phantom::lang::detail::popModule();
        }
    }
}

void Class::_onNativeElementsAccessImpl()
{
    auto func = m_OnDemandMembersFunc;
    m_OnDemandMembersFunc = nullptr;
    for (Class* pBaseClass : m_BaseClasses)
    {
        pBaseClass->onElementsAccess();
    }
    m_OnDemandMembersFunc = func;
    ClassType::_onNativeElementsAccessImpl();

    if (!isFinal() && isPolymorphic() &&
        (m_BaseClasses.empty() ||
         m_BaseClasses[0].offset)) // is polymorphic + no base class at offset 0 => we need our own base class
    {
        addNewVirtualMethodTable();
    }

    for (Class* pBaseClass : m_BaseClasses)
    {
        for (auto pTable : pBaseClass->getVirtualMethodTables())
        {
            VirtualMethodTable* pDerivedVTable = deriveVirtualMethodTable(pTable);
            pDerivedVTable->setFlag(PHANTOM_R_FLAG_NATIVE);
            pDerivedVTable->m_strName = '$' + StringUtil::ToString(m_VirtualMethodTables.size());
            pDerivedVTable->setOwner(this);
            m_VirtualMethodTables.push_back(pDerivedVTable);
        }
    }
    for (auto pMethod : getMethods())
    {
        if (pMethod->isVirtual() && m_VirtualMethodTables.empty())
            addNewVirtualMethodTable();

        size_t i = m_VirtualMethodTables.size();
        bool   inserted = false;
        while (i--)
        {
            // if not the zero-vtable (the one at offset-0 from the instance memory address), we
            // only insert the function if it overrides an existing if the zero vtable, if already
            // inserted, we only insert the function if it overrides existing, else we insert
            bool insertOnlyIfOverrides = (i != 0) || inserted;
            inserted = m_VirtualMethodTables[i]->insertMethod(pMethod, insertOnlyIfOverrides) || inserted;
        }
    }
}

bool Class::hasMetaDataIncludingBases(StringWithHash a_strName) const
{
    if (hasMetaData(a_strName))
        return true;
    for (auto it = m_BaseClasses.begin(); it != m_BaseClasses.end(); ++it)
    {
        if (it->baseClass->hasMetaDataIncludingBases(a_strName))
            return true;
    }
    return false;
}

bool Class::hasMetaDataIncludingBases(StringView a_strName) const
{
    return hasMetaDataIncludingBases(StringWithHash(a_strName));
}

bool Class::isCopyable() const
{
    if (!(isCopyConstructible() && !hasCopyDisabled()))
        return false;
    for (auto pField : getFields())
    {
        if (!(pField->getValueType()->isCopyable()))
            return false;
    }
    for (auto& bc : m_BaseClasses)
    {
        if (!(bc.baseClass->isCopyable()))
            return false;
    }
    return true;
}

bool Class::isPolymorphic() const
{
    return m_VirtualMethodTables.size() != 0;
}

void* Class::allocate() const
{
    PHANTOM_ASSERT(getSize());
    void* pAllocation = Type::allocate();
    memset(pAllocation, 0, getSize());
    return pAllocation;
}

void Class::deallocate(void* a_pInstance) const
{
    PHANTOM_ASSERT(getSize());
    Type::deallocate(a_pInstance);
}
void* Class::allocate(size_t a_uiCount) const
{
    PHANTOM_ASSERT(getSize());
    return Type::allocate(a_uiCount);
}
void Class::deallocate(void* a_pInstance, size_t a_uiCount) const
{
    PHANTOM_ASSERT(getSize());
    Type::deallocate(a_pInstance, a_uiCount);
}

void Class::construct(void* a_pInstance) const
{
    ClassType::construct(a_pInstance);
}

void Class::destroy(void* a_pObject) const
{
    ClassType::destroy(a_pObject);
}

void Class::setOverriddenDefaultExpression(ValueMember* a_pValueMember, Expression* a_pExpression)
{
    if (m_pOverriddenDefaultExpressions == nullptr)
    {
        m_pOverriddenDefaultExpressions = new_<SmallMap<ValueMember*, Expression*>>();
    }
    auto& pExp = (*m_pOverriddenDefaultExpressions)[a_pValueMember];
    if (pExp)
    {
        Delete(reinterpret_cast<LanguageElement*>(pExp));
    }
    pExp = a_pExpression;
}

Expression* Class::getOverriddenDefaultExpression(ValueMember* a_pValueMember) const
{
    PHANTOM_ASSERT(a_pValueMember->getOwner());
    auto pOwnerClass = a_pValueMember->getOwner()->asClass();
    PHANTOM_ASSERT(pOwnerClass && isA(pOwnerClass));
    if (m_pOverriddenDefaultExpressions)
    {
        auto found = m_pOverriddenDefaultExpressions->find(a_pValueMember);
        if (found != m_pOverriddenDefaultExpressions->end())
        {
            return found->second;
        }
    }
    if (pOwnerClass == this)
    {
        return a_pValueMember->getDefaultExpression();
    }
    return nullptr;
}

Expression* Class::getOverriddenDefaultExpressionCascade(ValueMember* a_pValueMember) const
{
    PHANTOM_ASSERT(a_pValueMember->getOwner());
    auto pOwnerClass = a_pValueMember->getOwner()->asClass();
    PHANTOM_ASSERT(pOwnerClass && isA(pOwnerClass));
    Expression* pExpression = getOverriddenDefaultExpression(a_pValueMember);
    if (pExpression)
        return pExpression;
    for (Class* pClass : m_BaseClasses)
    {
        if (pClass->isA(pOwnerClass))
        {
            return pClass->getOverriddenDefaultExpression(a_pValueMember);
        }
    }
    return nullptr;
}

VirtualMethodTable* Class::deriveVirtualMethodTable(VirtualMethodTable* a_pVirtualMethodTable)
{
    VirtualMethodTable* pVMT = a_pVirtualMethodTable->derive(this);
    pVMT->setOwner(this);
    m_VirtualMethodTables.push_back(pVMT);
    return pVMT;
}

SymbolExtension* Class::getExtensionCascade(Class* a_pSymbolExtensionClass, size_t a_Num /*= 0*/) const
{
    if (SymbolExtension* ext = Symbol::getExtension(a_pSymbolExtensionClass, a_Num))
        return ext;
    for (Class* pClass : m_BaseClasses)
    {
        if (SymbolExtension* ext = pClass->getExtension(a_pSymbolExtensionClass, a_Num))
            return ext;
    }
    return nullptr;
}

VirtualMethodTable* Class::DeriveVirtualMethodTable(Class* a_pOwner, VirtualMethodTable* a_pBase, size_t a_MethodCount)
{
    return a_pBase->derive(a_pOwner, a_MethodCount);
}

VirtualMethodTable* Class::DeriveVirtualMethodTable(Class* a_pOwner, VirtualMethodTable* a_pBase)
{
    return a_pBase->derive(a_pOwner);
}

Class* Class::VTablePrimaryClass(VirtualMethodTable* a_pVTable)
{
    return a_pVTable->getOriginalClass();
}

VirtualMethodTable* Class::createVirtualMethodTable()
{
    return New<VirtualMethodTable>();
}

size_t Class::getVirtualMethodCount(size_t a_uiOffset) const
{
    for (auto pVT : m_VirtualMethodTables)
    {
        if (pVT->getOffset() == a_uiOffset)
            return pVT->getMethodCount();
    }
    return 0;
}

void Class::addNewVirtualMethodTable()
{
    PHANTOM_ASSERT(m_VirtualMethodTables.empty() &&
                   ((m_BaseClasses.size() == 0) || !(getBaseClass(0)->isPolymorphic()) || isNative()));
    VirtualMethodTable* pVMT = createVirtualMethodTable();
    pVMT->setOwner(this);
    pVMT->setFlag(getFlags() & PHANTOM_R_FLAG_NATIVE);
    m_VirtualMethodTables.push_back(pVMT);
}

bool Class::hasNewVTable() const
{
    return getExtraData()->m_bHasVTablePtr;
}

Classes const& Class::getDerivedClasses() const
{
    return m_DerivedClasses;
}

void Class::getDerivedClassesCascade(Classes& out) const
{
    out.insert(out.end(), m_DerivedClasses.begin(), m_DerivedClasses.end());
    for (auto it = m_DerivedClasses.begin(); it != m_DerivedClasses.end(); ++it)
    {
        (*it)->getDerivedClassesCascade(out);
    }
}

#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
phantom::lang::Classes const& Class::getDerivedClassesCascade() const
{
    return m_RecurseDerivedClasses;
}
#endif

Strings Class::getBaseClasseNames() const
{
    Strings table;
    for (auto& bc : m_BaseClasses)
    {
        table.push_back(bc.baseClass->getQualifiedDecoratedName());
    }
    return table;
}

void Class::setBaseClasseNames(Strings table)
{
    for (auto it = table.begin(); it != table.end(); ++it)
    {
        addBaseClass(Application::Get()->findCppClass(*it));
    }
}

Field* Class::getFieldByOffset(size_t a_uiOffset) const
{
    Field* pField = ClassType::getFieldByOffset(a_uiOffset);
    if (pField)
        return pField;
    for (auto& bc : m_BaseClasses)
    {
        pField = bc.baseClass->getFieldByOffset(a_uiOffset - (bc.offset));
        if (pField)
            return pField;
    }
    return nullptr;
}

Field* Class::getFieldAtOffset(size_t a_uiOffset) const
{
    Field* pField = ClassType::getFieldAtOffset(a_uiOffset);
    if (pField)
        return pField;
    for (auto& bc : m_BaseClasses)
    {
        pField = bc.baseClass->getFieldAtOffset(a_uiOffset - (bc.offset));
        if (pField)
            return pField;
    }
    return nullptr;
}

size_t Class::getInheritanceLevel(size_t i) const
{
    return (i < m_BaseClasses.size()) ? m_BaseClasses[i].baseClass->getInheritanceLevel(i) + 1 : 0;
}

size_t Class::getInheritanceLevelFromBase(Class* a_pClass) const
{
    if (this == a_pClass)
        return 0;
    for (auto& bc : m_BaseClasses)
    {
        size_t b = bc.baseClass->getInheritanceLevelFromBase(a_pClass);
        if (b != ~size_t(0))
        {
            return b + 1;
        }
    }
    return ~size_t(0);
}

bool Class::hasDerivedClassCascade(Class* a_pClass, Access* a_pDerivedAccess) const
{
    if (hasDerivedClass(a_pClass, a_pDerivedAccess))
        return true;
    for (auto it = m_DerivedClasses.begin(); it != m_DerivedClasses.end(); ++it)
    {
        if ((*it)->hasDerivedClassCascade(a_pClass, a_pDerivedAccess))
            return true;
    }
    return false;
}

Access Class::getBaseClassAccess(Class* a_pClass) const
{
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass == a_pClass)
            return bc.access;
    }
    PHANTOM_ASSERT(false);
    return Access::Undefined;
}

bool Class::getBaseClassAccess(Class* a_pClass, Access* a_pInheritanceAccess) const
{
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass == a_pClass)
        {
            *a_pInheritanceAccess = std::min(*a_pInheritanceAccess, bc.access);
            return true;
        }
    }
    return false;
}

bool Class::getBaseClassAccessCascade(Class* a_pClass, Access* a_pInheritanceAccess) const
{
    if (getBaseClassAccess(a_pClass, a_pInheritanceAccess))
        return true;
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass->getBaseClassAccessCascade(a_pClass, a_pInheritanceAccess))
        {
            return true;
        }
    }
    return false;
}

Access Class::getBaseClassAccessCascade(Class* a_pClass) const
{
    Access access = Access::Public;
    if (getBaseClassAccessCascade(a_pClass, &access))
        return access;
    return Access::Undefined;
}

bool Class::hasPublicMember(Symbol* a_pLanguageElement) const
{
    Class* pClass = a_pLanguageElement->getOwner() ? a_pLanguageElement->getOwner()->asClass() : nullptr;
    if (pClass)
    {
        if (pClass == this)
            return a_pLanguageElement->isPublic();
        Access superAccess = getBaseClassAccessCascade(pClass);
        return (superAccess != Access::Undefined) &&
        (std::max(a_pLanguageElement->getAccess(), superAccess) == Access::Public);
    }
    return false;
}

bool Class::hasProtectedMember(Symbol* a_pSymbol) const
{
    Class* pClass = a_pSymbol->getOwner() ? a_pSymbol->getOwner()->asClass() : nullptr;
    if (pClass)
    {
        if (pClass == this)
            return a_pSymbol->isProtected();
        Access superAccess = getBaseClassAccessCascade(pClass);
        return (superAccess != Access::Undefined) &&
        (std::max(a_pSymbol->getAccess(), superAccess) == Access::Protected);
    }
    return false;
}

bool Class::hasPrivateMember(Symbol* a_pSymbol) const
{
    Class* pClass = a_pSymbol->getOwner() ? a_pSymbol->getOwner()->asClass() : nullptr;
    if (pClass)
    {
        if (pClass == this)
            return a_pSymbol->isPrivate();
        Access superAccess = getBaseClassAccessCascade(pClass);
        return (superAccess != Access::Undefined) && (std::max(a_pSymbol->getAccess(), superAccess) == Access::Private);
    }
    return false;
}

bool Class::isListInitializable() const
{
    if (!ClassType::isListInitializable())
        return false;
    if (m_VirtualMethodTables.size())
        return false;
    for (auto baseClass : m_BaseClasses)
    {
        if (baseClass.access != Access::Public)
        {
            return false;
        }
    }
    return true;
}

ptrdiff_t Class::getPointerAdjustmentOffset(Class* a_pClass) const
{
    ptrdiff_t o0 = getBaseClassOffsetCascade(a_pClass);
    if (o0 != -1)
        return (int)o0;
    ptrdiff_t o1 = a_pClass->getBaseClassOffsetCascade(const_cast<Class*>(this));
    return o1 != -1 ? -ptrdiff_t(o1) : std::numeric_limits<ptrdiff_t>::min();
}

ptrdiff_t Class::getPointerAdjustmentOffset(Type* a_pType) const
{
    Class* pClass = a_pType->asClass();
    if (pClass == nullptr)
        return -1;
    return getPointerAdjustmentOffset(pClass);
}

bool Class::canHaveImplicitDefaultConstructor() const
{
    if (getDefaultConstructor())
        return false;
    if (getConstructors().empty()) /// we can only have a default constructor is no constructor has
                                   /// already been declared
    {
        for (auto& bc : m_BaseClasses)
        {
            if (!(bc.baseClass->isDefaultConstructible()))
                return false;
        }
        for (auto pField : getFields())
        {
            Type* pType = pField->getValueType()->removeAllExtents()->removeQualifiers();
            if (ClassType* pClassType = pType->asClassType())
            {
                Constructor* pCtor;
                if (pField->getDefaultExpression() == nullptr &&
                    (!(pClassType->isDefaultConstructible()) ||
                     ((pCtor = pClassType->getDefaultConstructor()) == nullptr) ||
                     pCtor->testModifiers(Modifier::Deleted)))
                    return false;
            }
            else if (pType->asReference())
                return false;
        }
        return true;
    }
    return false;
}

bool Class::canHaveImplicitCopyConstructor() const
{
    if (getCopyConstructor())
        return false;
    for (auto& bc : m_BaseClasses)
    {
        auto pCtor = bc.baseClass->getCopyConstructor();
        if (pCtor == nullptr || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }

    for (auto pField : getFields())
    {
        Type* pTypeNoExt = pField->getValueType()->removeAllExtents();
        if (ClassType* pClassType = pTypeNoExt->asClassType())
        {
            if (auto pOp = pClassType->getCopyConstructor())
            {
                if (pOp->testModifiers(Modifier::Deleted))
                    return false;
            }
            else
                return false;
        }
        if (pTypeNoExt->asReference())
            return false;
    }
    return true;
}

bool Class::canHaveImplicitCopyAssignmentOperator() const
{
    if (getCopyAssignmentOperator())
        return false;
    for (auto& bc : m_BaseClasses)
    {
        auto pCtor = bc.baseClass->getCopyAssignmentOperator();
        if (pCtor == nullptr || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }
    for (auto pField : getFields())
    {
        Type* pTypeNoExt = pField->getValueType()->removeAllExtents();
        if (ClassType* pClassType = pTypeNoExt->asClassType())
        {
            if (auto pOp = pClassType->getCopyAssignmentOperator())
            {
                if (pOp->testModifiers(Modifier::Deleted))
                    return false;
            }
            else
                return false;
        }
        if (pTypeNoExt->asReference() || pTypeNoExt->isConst())
            return false;
    }
    return true;
}

bool Class::canHaveImplicitMoveConstructor() const
{
    if (getMoveConstructor())
        return false;
    if (Constructor* pCtor = getCopyConstructor())
    {
        if (!(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }
    if (Method* pOp = getCopyAssignmentOperator())
    {
        if (!(pOp->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pOp->testModifiers(Modifier::Deleted))
            return false;
    }
    for (auto& bc : m_BaseClasses)
    {
        auto pCtor = bc.baseClass->getMoveConstructor();
        if (pCtor == nullptr || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }

    for (auto pField : getFields())
    {
        Type* pTypeNoExt = pField->getValueType()->removeAllExtents();
        if (ClassType* pClassType = pTypeNoExt->asClassType())
        {
            if (auto pOp = pClassType->getMoveConstructor())
            {
                if (pOp->testModifiers(Modifier::Deleted))
                    return false;
            }
            else
                return false;
        }
        if (pTypeNoExt->asReference())
            return false;
    }
    return true;
}

bool Class::canHaveImplicitMoveAssignmentOperator() const
{
    if (getMoveAssignmentOperator())
        return false;
    if (Constructor* pCtor = getCopyConstructor())
    {
        if (!(pCtor->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }
    if (Method* pOp = getCopyAssignmentOperator())
    {
        if (!(pOp->testFlags(PHANTOM_R_FLAG_IMPLICIT)) || pOp->testModifiers(Modifier::Deleted))
            return false;
    }
    for (auto& bc : m_BaseClasses)
    {
        auto pCtor = bc.baseClass->getMoveAssignmentOperator();
        if (pCtor == nullptr || pCtor->testModifiers(Modifier::Deleted))
            return false;
    }

    for (auto pField : getFields())
    {
        Type* pTypeNoExt = pField->getValueType()->removeAllExtents();
        if (ClassType* pClassType = pTypeNoExt->asClassType())
        {
            if (auto pOp = pClassType->getMoveAssignmentOperator())
            {
                if (pOp->testModifiers(Modifier::Deleted))
                    return false;
            }
            else
                return false;
        }
        if (pTypeNoExt->asReference() || pTypeNoExt->isConst())
            return false;
    }
    return true;
}

VirtualMethodTable* Class::CreateVirtualMethodTable(Class* a_pOwner, void** a_ppAddr, size_t a_MethodCount)
{
    return a_pOwner->New<VirtualMethodTable>(a_ppAddr, a_MethodCount);
}

void Class::addImplicitDefaultConstructor()
{
    PHANTOM_ASSERT(!isNative());
    Signature*   pSignature = Signature::Create(this, PHANTOM_TYPEOF(void));
    Constructor* pConstructor = New<Constructor>(m_strName, pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void Class::addImplicitCopyConstructor()
{
    PHANTOM_ASSERT(!isNative());
    Signature*   pSignature = Signature::Create(this, PHANTOM_TYPEOF(void), makeConst()->makeLValueReference());
    Constructor* pConstructor = New<Constructor>(m_strName, pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void Class::addImplicitCopyAssignmentOperator()
{
    PHANTOM_ASSERT(!isNative());
    Signature* pSignature = Signature::Create(this, makeLValueReference(), makeConst()->makeLValueReference());
    Method*    pMethod = New<Method>("operator=", pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pMethod->setAccess(Access::Public);
    addMethod(pMethod);
}

void Class::addImplicitMoveConstructor()
{
    PHANTOM_ASSERT(!isNative());
    Signature*   pSignature = Signature::Create(this, PHANTOM_TYPEOF(void), makeRValueReference());
    Constructor* pConstructor = New<Constructor>(m_strName, pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pConstructor->setAccess(Access::Public);
    addConstructor(pConstructor);
}

void Class::addImplicitMoveAssignmentOperator()
{
    PHANTOM_ASSERT(!isNative());
    Signature* pSignature = Signature::Create(this, makeLValueReference(), makeRValueReference());
    Method*    pMethod = New<Method>("operator=", pSignature, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pMethod->setAccess(Access::Public);
    addMethod(pMethod);
}

void Class::addImplicitDestructor()
{
    PHANTOM_ASSERT(!isNative());
    Method* pDtor = addDestructor(PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    pDtor->setAccess(Access::Public);
    for (Class* pBaseClass : getBaseClasses())
    {
        if (pBaseClass->getDestructor()->isVirtual())
        {
            pDtor->setVirtual();
            break;
        }
    }
}

bool Class::hasMemberCascade(LanguageElement* a_pElement) const
{
    if (hasElement(a_pElement))
        return true;
    for (auto& bc : m_BaseClasses)
    {
        if (bc.baseClass->hasMemberCascade(a_pElement))
            return true;
    }
    return false;
}

void Class::setBaseClassOffset(Class* a_pClass, ptrdiff_t a_Offset)
{
    for (auto& baseClass : m_BaseClasses)
        if (baseClass.baseClass == a_pClass)
            baseClass.offset = a_Offset;
}

// TODO : remove
void Class::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    ClassType::copyConstruct(a_pDest, a_pSrc);
}

bool Class::equal(void const* a_pInstance0, void const* a_pInstance1) const
{
    if (!m_OpEquals.isInitialized())
    {
        Type* pThis = (Type*)this;
        m_OpEquals.emplace(nullptr);
        *m_OpEquals = getMethod("operator==", TypesView{&pThis, 1});
        if (!*m_OpEquals)
        {
            Type* pThisConstRef = addConst()->addLValueReference();
            *m_OpEquals = getMethod("operator==", TypesView{&pThisConstRef, 1});
            if (!*m_OpEquals)
            {
                *m_OpEquals = getEnclosingNamespace()->getFunction("operator==", Types{pThisConstRef, pThisConstRef});
            }
        }
    }
    if (*m_OpEquals)
    {
        if (Method* pMethod = (*m_OpEquals)->asMethod())
        {
            bool res = false;
            pMethod->invoke((void*)a_pInstance0, (void**)&a_pInstance1, &res);
            return res;
        }
        else
        {
            return static_cast<Function*>(*m_OpEquals)->call<bool>(*(int*)a_pInstance0, *(int*)a_pInstance1);
        }
    }
    return false;
}

bool Class::hasStrongDependencyOnType(Type* a_pType) const
{
    if (ClassType::hasStrongDependencyOnType(a_pType))
        return true;

    for (Class* pBaseClass : m_BaseClasses)
    {
        if (pBaseClass == a_pType || pBaseClass->hasStrongDependencyOnType(a_pType))
            return true;
    }

    return false;
}

void Class::registerInstance(void* a_pInstance)
{
    _registerKind(a_pInstance);
    ++m_InstanceCount;
    if (phantom::detail::g_InstanceHook_func)
        phantom::detail::g_InstanceHook_func(ClassHookOp::InstanceRegistered, this, a_pInstance);
}

void Class::unregisterInstance(void* a_pInstance)
{
    PHANTOM_ASSERT(m_InstanceCount);
    if (phantom::detail::g_InstanceHook_func)
        phantom::detail::g_InstanceHook_func(ClassHookOp::InstanceUnregistering, this, a_pInstance);
    --m_InstanceCount;
    _unregisterKind(a_pInstance);
}

InstanceCache* Class::getOrCreateInstanceCache()
{
    if (m_pInstanceCache)
        return m_pInstanceCache;
    return m_pInstanceCache = new_<InstanceCache>(this);
}

Type* Class::getCommonBaseAncestor(Type* a_pType) const
{
    if (Class* pClass = a_pType->asClass())
        return getCommonBaseClass(pClass);
    return nullptr;
}

void Class::_registerKind(void* a_pInstance)
{
    for (BaseClass const& bc : m_BaseClasses)
    {
        bc.baseClass->_registerKind(reinterpret_cast<byte*>(a_pInstance) + bc.offset);
    }
    if (isSingleton())
    {
        PHANTOM_ASSERT(m_InstanceCount == 0 && m_pSingleton == nullptr);
        m_pSingleton = a_pInstance;
    }

    if (phantom::detail::g_InstanceHook_func)
        phantom::detail::g_InstanceHook_func(ClassHookOp::KindCreated, this, a_pInstance);

#if PHANTOM_CUSTOM_ENABLE_INSTANTIATION_SIGNALS
    PHANTOM_EMIT kindCreated(a_pInstance);
#endif
}

void Class::_unregisterKind(void* a_pInstance)
{
#if PHANTOM_CUSTOM_ENABLE_INSTANTIATION_SIGNALS
    PHANTOM_EMIT kindDestroying(a_pInstance);
#endif
    if (phantom::detail::g_InstanceHook_func)
        phantom::detail::g_InstanceHook_func(ClassHookOp::KindDestroying, this, a_pInstance);
    if (isSingleton())
    {
        PHANTOM_ASSERT(m_InstanceCount == 0 && m_pSingleton == a_pInstance);
        m_pSingleton = nullptr;
    }
    for (BaseClass const& bc : m_BaseClasses)
    {
        bc.baseClass->_unregisterKind(reinterpret_cast<byte*>(a_pInstance) + bc.offset);
    }
}

bool Class::isCopyConstructible() const
{
    return !(hasCopyDisabled());
}

bool Class::isMoveConstructible() const
{
    return !(hasMoveDisabled());
}

namespace
{
const int anonymous_inc = 1;
}

ClassBuilder::ClassBuilder(Scope* a_pOwnerScope, Scope* a_pNamingScope, StringView a_Name, Access a_Access,
                           size_t a_MinAlign /*= 0*/)
    : m_Access(a_Access), m_MinAlign(a_MinAlign)
{
    if (a_pOwnerScope == nullptr)
        a_pOwnerScope = Application::Get()->getMainModule()->getDefaultPackage()->getOrCreateSource("default");

    if (a_pNamingScope == nullptr)
        a_pNamingScope = Namespace::Global();

    if (a_Name.size())
    {
        StringViews nameParts;
        StringUtil::Split(nameParts, a_Name, ":");
        if (nameParts.size() > 1)
        {
            Namespace* pNS = a_pNamingScope->asNamespace();
            PHANTOM_ASSERT(pNS);
            for (size_t i = 0; i < nameParts.size() - 1; ++i)
            {
                pNS = pNS->getOrCreateNamespace(nameParts[i]);
            }
            a_pNamingScope = pNS;
        }
        m_pClass = a_pOwnerScope->asLanguageElement()->New<lang::Class>(nameParts.back());
    }
    else
    {
        m_pClass = a_pOwnerScope->asLanguageElement()->New<lang::Class>(StringView());
    }
    m_pClass->setCurrentAccess(a_Access);

    if (a_pNamingScope != a_pOwnerScope)
        a_pNamingScope->addType(m_pClass);
    a_pOwnerScope->addType(m_pClass);
}

Scope* ClassBuilder::scope() const
{
    return m_pClass;
}

ClassBuilder& ClassBuilder::inherits(Class* _class)
{
    PHANTOM_ASSERT(m_pClass, "ClassBuilder : class already finalized");
    m_pClass->addBaseClass(_class);
    return *this;
}

ClassBuilder& ClassBuilder::field(Type* a_pType, StringView a_Name, size_t a_Align /*= 0*/, uint a_FilterMask /*= ~0u*/,
                                  StringView a_DefaultValue /* = {}*/)
{
    Expression* pExp = nullptr;
    if (!a_DefaultValue.empty())
    {
        pExp = Application::Get()->cppExpression(a_DefaultValue, m_pClass);
    }

    return field(a_pType, a_Name, a_Align, a_FilterMask, pExp);
}

ClassBuilder& ClassBuilder::field(Type* a_pType, StringView a_Name, size_t a_Align /*= 0*/, uint a_FilterMask /*= ~0u*/,
                                  Expression* a_pDefaultExp)
{
    PHANTOM_ASSERT(m_pClass, "ClassBuilder : class already finalized");
    if (Field* pField = m_pClass->addField(a_pType, a_Name, a_FilterMask))
    {
        if (a_Align)
            pField->setAlignment(std::max(a_pType->getAlignment(), a_Align));

        if (a_pDefaultExp)
        {
            pField->setDefaultExpression(a_pDefaultExp);
        }
    }
    return *this;
}

PHANTOM_EXPORT_PHANTOM Functor<void(Class*)> m_SemanticClassFinalizer;

Class* ClassBuilder::finalize()
{
    PHANTOM_ASSERT(m_pClass, "ClassBuilder : class already finalized");

    if (m_SemanticClassFinalizer)
    {
        m_SemanticClassFinalizer(m_pClass);
    }
    else
    {
        Type::AlignmentComputer computer(m_MinAlign);
        size_t                  size = 0;
        size_t                  align = 0;
        for (auto baseClass : m_pClass->getBaseClasses())
        {
            computer.push(baseClass.baseClass->getSize(), baseClass.baseClass->getAlignment());
        }
        computer.alignStruct(m_pClass->getDataElements(), size, align);
        m_pClass->setSize(size);
        m_pClass->setAlignment(align);
        if (m_pClass->getDefaultConstructor() == nullptr && m_pClass->canHaveImplicitDefaultConstructor())
        {
            m_pClass->addImplicitDefaultConstructor();
        }
        if (m_pClass->getCopyConstructor() == nullptr && m_pClass->canHaveImplicitCopyConstructor())
        {
            m_pClass->addImplicitCopyConstructor();
        }
        if (m_pClass->getCopyAssignmentOperator() == nullptr && m_pClass->canHaveImplicitCopyAssignmentOperator())
        {
            m_pClass->addImplicitCopyAssignmentOperator();
        }
        if (m_pClass->getMoveConstructor() == nullptr && m_pClass->canHaveImplicitMoveConstructor())
        {
            m_pClass->addImplicitMoveConstructor();
        }
        if (m_pClass->getMoveAssignmentOperator() == nullptr && m_pClass->canHaveImplicitMoveAssignmentOperator())
        {
            m_pClass->addImplicitMoveAssignmentOperator();
        }
        if (m_pClass->getDestructor() == nullptr)
        {
            m_pClass->addImplicitDestructor();
        }
    }
    auto pRes = m_pClass;
    m_pClass = nullptr;
    return pRes;
}

ClassBuilder& ClassBuilder::access(Access _access)
{
    m_pClass->setCurrentAccess(_access);
    return *this;
}

} // namespace lang
} // namespace phantom
