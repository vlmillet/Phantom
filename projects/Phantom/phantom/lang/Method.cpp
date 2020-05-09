// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Method.h"

#include "ConstType.h"
#include "LValueReference.h"
#include "Parameter.h"
#include "Pointer.h"
#include "RValueReference.h"
#include "Signature.h"
#include "VirtualMethodTable.h"
/* *********************************************** */
namespace phantom
{
namespace lang
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
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "remove Slot concept from Phantom (get more bloat free)")
#endif
}

Method::Method(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers /*= 0*/,
               uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_pSignature, a_eABI, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags),
      m_pThis(nullptr),
      m_uiVirtualTableIndex(~size_t(0)),
      m_pVTableClosures(nullptr),
      m_pProperty(nullptr)
{
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
    if (m_pVTableClosures)
        delete_<SmallMap<size_t, void*>>(m_pVTableClosures);
}

lang::ClassType* Method::getOwnerClassType() const
{
    return static_cast<lang::ClassType*>(getOwner());
}

lang::Class* Method::getOwnerClass() const
{
    return static_cast<lang::ClassType*>(getOwner())->asClass();
}

bool Method::canOverride(Method* a_pMethod) const
{
    if (!(a_pMethod->isVirtual()))
        return false;
    ESignatureRelation r = getSignatureRelationWith(a_pMethod);
    return (r == e_SignatureRelation_Covariant) || (r == e_SignatureRelation_Equal);
}

bool Method::canOverride(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/) const
{
    ESignatureRelation r = getSignatureRelationWith(a_strName, a_pSignature, a_Modifiers);
    return (r == e_SignatureRelation_Covariant) || (r == e_SignatureRelation_Equal);
}

bool Method::isOverridableBy(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/) const
{
    ESignatureRelation r = getSignatureRelationWith(a_strName, a_pSignature, a_Modifiers);
    return (r == e_SignatureRelation_Contravariant) || (r == e_SignatureRelation_Equal);
}

bool Method::isOverridableBy(Method* a_pMethod) const
{
    return a_pMethod->canOverride((Method*)this);
}

void Method::setVirtual()
{
    if (isVirtual())
        return;
    PHANTOM_ASSERT(getOwner(), "method cannot be set to virtual after being added to a class type");
    addModifiers(PHANTOM_R_VIRTUAL);
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
        m_pVTableClosures = new_<SmallMap<size_t, void*>>(getAllocator());
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
    && (((signModifiers & (PHANTOM_R_REFQUAL_MASK)) == 0) ||
        (signModifiers & PHANTOM_R_REFQUAL_MASK) == (a_CallerQualifiers & PHANTOM_R_REFQUAL_MASK));
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

void Method::_onAttachingToClass(ClassType* a_pClass)
{
    // normalize conversion function name (only if native)
    if (isNative())
    {
        StringBuffer buffer;
        conversionOperatorNameNormalizer(getName(), buffer, a_pClass);
        setName(buffer);
    }
}

void Method::_onAttachedToClass(ClassType* a_pClass)
{
    // create this
    PHANTOM_ASSERT(m_pThis == nullptr); // ASSERT_DEBUG
    m_pThis = New<LocalVariable>(
    isConst() ? a_pClass->makeConst()->makePointer()->makeConst() : a_pClass->makePointer()->makeConst(), "this");
}

LocalVariable* Method::getThis() const
{
    return m_pThis;
}

OpaqueDelegate Method::getOpaqueDelegate(void* a_pThis) const
{
    PHANTOM_ASSERT(a_pThis);
    if (void* addr = getClosure().address)
    {
        struct
        {
            void* thisOrFunc_;
            void* meth_;
        } s;
        s.thisOrFunc_ = a_pThis;
        s.meth_ = addr;
        return *reinterpret_cast<OpaqueDelegate*>(&s); // empty by default
    }
    return OpaqueDelegate();
}

OpaqueDelegate Method::getOpaqueDelegate() const
{
    // universal calling convention delegate

    if (void* addr = getClosure().address)
    {
        struct
        {
            void* thisOrFunc_;
            void* meth_;
        } s;
        s.thisOrFunc_ = addr;
        s.meth_ = nullptr;
        return *reinterpret_cast<OpaqueDelegate*>(&s); // empty by default
    }
    return OpaqueDelegate();
}

typedef SmallVector<void*, 7> TempArgs;

void Method::invoke(void* a_pObject, void** a_pArgs) const
{
    TempArgs newArgs;
    if (auto applyPointer = getApplyPointer())
    {
        Type*     pRetType = getReturnType();
        bool      bRVO = isRVOCandidate();
        const int extraArgs = 1 + bRVO;
        size_t    argCount = m_pSignature->getParameters().size() + extraArgs;
        newArgs.resize(argCount);
        newArgs[0] = &a_pObject;
        unsigned char dummy[16];
        void*         ret{};
        if (bRVO)
        {
            const int align = 32;
            void*     p = _alloca(pRetType->getSize() + align - 1);
            ret = (void*)((((intptr_t)p + align - 1) / align) * align);
            pRetType->construct(ret);
            newArgs[1] = &ret;
        }
        else
        {
            PHANTOM_ASSERT(pRetType->getSize() < 16);
            ret = dummy;
        }
        if (a_pArgs && argCount > extraArgs)
            memcpy(&newArgs[extraArgs], a_pArgs, (argCount - extraArgs) * sizeof(void*));
        applyPointer(newArgs.data(), ret);
        if (bRVO)
            pRetType->destroy(ret);
    }
    else
    {
        size_t argCount = m_pSignature->getParameters().size() + 1;
        newArgs.resize(argCount);
        newArgs[0] = &a_pObject;
        if (a_pArgs && argCount > 1)
        {
            memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
        }
        apply(newArgs.data(), argCount);
    }
}

void Method::invoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(a_pReturnAddress);
    TempArgs newArgs;
    if (auto applyPointer = getApplyPointer())
    {
        bool      bRVO = isRVOCandidate();
        const int extraArgs = 1 + bRVO;
        size_t    argCount = m_pSignature->getParameters().size() + extraArgs;
        newArgs.resize(argCount);
        newArgs[bRVO] = &a_pReturnAddress;
        newArgs[0] = &a_pObject;
        if (a_pArgs && argCount > extraArgs)
            memcpy(&newArgs[extraArgs], a_pArgs, (argCount - extraArgs) * sizeof(void*));
        applyPointer(newArgs.data(), reinterpret_cast<void*>(!bRVO * uint64_t(a_pReturnAddress)));
    }
    else
    {
        size_t argCount = m_pSignature->getParameters().size() + 1;
        newArgs.resize(argCount);
        newArgs[0] = &a_pObject;
        if (a_pArgs && argCount > 1)
            memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
        apply(newArgs.data(), argCount, a_pReturnAddress);
    }
}

void Method::placementInvoke(void*, void**, void*) const {}

void Method::call(void** a_pArgs) const
{
    void* pThis = *(void**)(*a_pArgs++);
    Method::invoke(pThis, a_pArgs);
}

void Method::call(void** a_pArgs, void* a_pReturnAddress) const
{
    void* pThis = *(void**)(*a_pArgs++);
    Method::invoke(pThis, a_pArgs, a_pReturnAddress);
}

} // namespace lang
} // namespace phantom
