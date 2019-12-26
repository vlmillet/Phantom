// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Method.h"

#include "ConstType.h"
#include "LValueReference.h"
#include "Parameter.h"
#include "Pointer.h"
#include "RValueReference.h"
#include "Signature.h"
#include "VirtualMethodTable.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Method::Method(ABI a_eABI, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/)
    : Subroutine(a_eABI, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
}

Method::Method(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_pSignature, ABI::MethodCall, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
    if (testModifiers(PHANTOM_R_CONST) AND testModifiers(PHANTOM_R_SLOT_METHOD))
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "Slots cannot be const");
    }
}

Method::Method(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers /*= 0*/,
               uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_pSignature, a_eABI, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
    if (testModifiers(PHANTOM_R_CONST) AND testModifiers(PHANTOM_R_SLOT_METHOD))
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "Slots cannot be const");
    }
}

Method::Method(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature,
               Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_pScope, a_strName, a_strSignature, ABI::MethodCall, (a_Modifiers & ~PHANTOM_R_NOCONST), a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
    if (testModifiers(PHANTOM_R_CONST) AND testModifiers(PHANTOM_R_SLOT_METHOD))
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "Slots cannot be const");
    }
}

Method::Method(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature, ABI a_eABI,
               Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_pScope, a_strName, a_strSignature, a_eABI, (a_Modifiers & ~PHANTOM_R_NOCONST), a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
    if (testModifiers(PHANTOM_R_CONST) AND testModifiers(PHANTOM_R_SLOT_METHOD))
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "Slots cannot be const");
    }
}

size_t Method::getVirtualTableIndex(size_t a_uiVtableIndex) const
{
    Class* pClass = getOwnerClass();
    if (pClass)
    {
        if (a_uiVtableIndex < pClass->getVirtualMethodTables().size())
        {
            return pClass->getVirtualMethodTables()[a_uiVtableIndex]->getMethodIndex(const_cast<Method*>(this));
        }
    }
    return ~size_t(0);
}

Method::~Method()
{
    PHANTOM_DELETE(SmallMap<size_t, void*>) m_pVTableClosures;
}

reflection::ClassType* Method::getOwnerClassType() const
{
    return static_cast<reflection::ClassType*>(getOwner());
}

reflection::Class* Method::getOwnerClass() const
{
    return static_cast<reflection::ClassType*>(getOwner())->asClass();
}

bool Method::canOverride(Method* a_pMethod) const
{
    if (NOT(a_pMethod->isVirtual()))
        return false;
    ESignatureRelation r = getSignatureRelationWith(a_pMethod);
    return (r == e_SignatureRelation_Covariant) OR(r == e_SignatureRelation_Equal);
}

bool Method::canOverride(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/) const
{
    ESignatureRelation r = getSignatureRelationWith(a_strName, a_pSignature, a_Modifiers);
    return (r == e_SignatureRelation_Covariant) OR(r == e_SignatureRelation_Equal);
}

bool Method::isOverridableBy(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/) const
{
    ESignatureRelation r = getSignatureRelationWith(a_strName, a_pSignature, a_Modifiers);
    return (r == e_SignatureRelation_Contravariant) OR(r == e_SignatureRelation_Equal);
}

bool Method::isOverridableBy(Method* a_pMethod) const
{
    return a_pMethod->canOverride((Method*)this);
}

void Method::safeInvoke(void* a_pCallerAddress, void** a_pArgs, void* a_pReturnAddress) const
{
    reflection::Class* pOwnerClass = getOwner()->asClass();
    if (pOwnerClass)
    {
        const RttiMapData& rttiData = Rtti::Find(a_pCallerAddress);
        invoke(rttiData.cast(pOwnerClass), a_pArgs, a_pReturnAddress);
    }
    else
    {
        invoke(a_pCallerAddress, a_pArgs, a_pReturnAddress);
    }
}

void Method::safePlacementInvoke(void* a_pCallerAddress, void** a_pArgs, void* a_pReturnAddress) const
{
    reflection::Class* pOwnerClass = getOwner()->asClass();
    if (pOwnerClass)
    {
        const RttiMapData& rttiData = Rtti::Find(a_pCallerAddress);
        placementInvoke(rttiData.cast(pOwnerClass), a_pArgs, a_pReturnAddress);
    }
    else
    {
        placementInvoke(a_pCallerAddress, a_pArgs, a_pReturnAddress);
    }
}

void Method::safeInvoke(void* a_pCallerAddress, void** a_pArgs) const
{
    reflection::Class* pOwnerClass = getOwner()->asClass();
    if (pOwnerClass)
    {
        const RttiMapData& rttiData = Rtti::Find(a_pCallerAddress);
        invoke(rttiData.cast(pOwnerClass), a_pArgs);
    }
    else
    {
        invoke(a_pCallerAddress, a_pArgs);
    }
}

/*
jit_function Method::getVTableFunction( size_t a_uiThisOffset ) const
{
    PHANTOM_ASSERT(m_jit_function.function, "Closure not yet created, you must add this Method to a
Class"); if(a_uiThisOffset == 0) return m_jit_function; auto found =
m_VTableOffsetFixFunctions.find(a_uiThisOffset); if(found != m_VTableOffsetFixFunctions.end())
    {
        return found->second;
    }
    jit_function func = compileThisOffsetShiftFunction(a_uiThisOffset);
    // UNCOMMENT TO TEST APPLY
//     void* nul = nullptr;
//     void* null[] = { &nul };
//     jit_function_apply(func, null, nullptr);
    //a_pMethod->setVTableOffsetFixClosure[a_uiThisOffset] = func;
    return func;
}*/

void Method::setVirtual()
{
    if (isVirtual())
        return;
    if (getOwner())
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException,
                                "Member function cannot be set to virtual after being added to a class type")
    }
    m_Modifiers |= PHANTOM_R_VIRTUAL;
}

void* Method::getVTableClosure(size_t a_uiOffset) const
{
    if (m_pVTableClosures == nullptr)
        return nullptr;
    auto found = m_pVTableClosures->find(a_uiOffset);
    return (found == m_pVTableClosures->end()) ? nullptr : found->second;
}

void Method::setVTableClosure(size_t a_uiOffset, void* a_pClosure)
{
    if (m_pVTableClosures == nullptr)
    {
        m_pVTableClosures = PHANTOM_NEW(SmallMap<size_t, void*>);
    }
    (*m_pVTableClosures)[a_uiOffset] = a_pClosure;
}

void Method::getOriginalOverriddenMethods(Methods& a_Out) const
{
    Class* pClass = getOwnerClass();
    if (pClass == nullptr)
        return;
    for (auto p : pClass->getVirtualMethodTables())
    {
        Method* pRootMethod = p->getRootMethod(const_cast<Method*>(this));
        if (pRootMethod)
            a_Out.push_back(pRootMethod);
    }
}

bool Method::acceptsCallerExpressionType(Type* a_pType) const
{
    Modifiers qualifiers = 0;
    a_pType = a_pType->removeQualifiers();
    if (a_pType->asLValueReference())
    {
        qualifiers |= PHANTOM_R_LVALUEREF;
        a_pType = static_cast<LValueReference*>(a_pType);
    }
    else
    {
        qualifiers |= PHANTOM_R_RVALUEREF;
        a_pType = static_cast<RValueReference*>(a_pType);
    }
    if (a_pType->asConstType())
    {
        qualifiers |= PHANTOM_R_CONST;
    }
    return acceptsCallerExpressionQualifiers(qualifiers);
}

bool Method::acceptsCallerExpressionQualifiers(Modifiers a_CallerQualifiers) const
{
    Modifiers signModifiers = m_pSignature->getModifiers();
    PHANTOM_ASSERT((a_CallerQualifiers & (~(PHANTOM_R_REFQUAL_MASK | PHANTOM_R_CONST))) == 0);
    PHANTOM_ASSERT(((a_CallerQualifiers & PHANTOM_R_LVALUEREF) == PHANTOM_R_LVALUEREF) ^
                   ((a_CallerQualifiers & PHANTOM_R_RVALUEREF) == PHANTOM_R_RVALUEREF));
    return (((a_CallerQualifiers & PHANTOM_R_CONST) == PHANTOM_R_CONST) <=
            ((signModifiers & PHANTOM_R_CONST) ==
             PHANTOM_R_CONST)) // caller must be equally or less const qualified than member
                               // function (every one can call a const member function but a const
                               // cannot call a non const member function)
    AND(((signModifiers & (PHANTOM_R_REFQUAL_MASK)) == 0)
        OR(signModifiers & PHANTOM_R_REFQUAL_MASK) == (a_CallerQualifiers & PHANTOM_R_REFQUAL_MASK));
}

Type* Method::getImplicitObjectParameterType() const
{
    Type* pImplicitObjectParameterType = getOwner()->asClassType();
    if (m_pSignature->testModifiers(PHANTOM_R_CONST))
    {
        pImplicitObjectParameterType = pImplicitObjectParameterType->makeConst();
    }
    if (m_pSignature->testModifiers(PHANTOM_R_RVALUEREF))
    {
        return pImplicitObjectParameterType->makeRValueReference();
    }
    return pImplicitObjectParameterType->makeLValueReference();
}

LocalVariable* Method::createThis() const
{
    return PHANTOM_NEW(LocalVariable)(isConst() ? getEnclosingClassType()->makeConst()->makePointer()->makeConst()
                                                : getEnclosingClassType()->makePointer()->makeConst(),
                                      "this");
}

LocalVariable* Method::getThis() const
{
    return m_pThis;
}

void Method::onAncestorChanged(LanguageElement* a_pAncestor)
{
    Subroutine::onAncestorChanged(a_pAncestor);
    if (a_pAncestor == getOwner())
    {
        if (NOT(isNative()))
        {
            if (getAccess() == Access::Undefined)
                setAccess(static_cast<ClassType*>(a_pAncestor)->getDefaultAccess());
            m_pThis = createThis();
            addElement(m_pThis);
        }
    }
}

typedef SmallVector<void*, 7> TempArgs;

void Method::invoke(void* a_pObject, void** a_pArgs) const
{
    size_t   argCount = m_pSignature->getParameters().size() + 1;
    TempArgs newArgs;
    newArgs.resize(argCount);
    newArgs[0] = &a_pObject;
    if (a_pArgs && argCount > 1)
    {
        memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
    }
    apply(newArgs.data(), argCount);
}

void Method::invoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const
{
    size_t   argCount = m_pSignature->getParameters().size() + 1;
    TempArgs newArgs;
    newArgs.resize(argCount);
    newArgs[0] = &a_pObject;
    if (a_pArgs && argCount > 1)
    {
        memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
    }
    apply(newArgs.data(), argCount, a_pReturnAddress);
}

void Method::placementInvoke(void*, void**, void*) const
{
}

} // namespace reflection
} // namespace phantom
