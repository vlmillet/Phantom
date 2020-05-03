// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Subroutine.h"

#include "Application.h"
#include "ExecutionContext.h"
#include "Parameter.h"
#include "Signature.h"

#include <phantom/utils/Variant.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Instructions m_EmptyInstructions;

Subroutine::Subroutine(ABI a_eABI, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers, a_uiFlags), m_eABI(a_eABI)
{
}

Subroutine::Subroutine(StringView a_strName, ABI a_eABI, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_eABI(a_eABI)
{
}

Subroutine::Subroutine(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers /*= 0*/,
                       uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_pSignature(a_pSignature), m_eABI(a_eABI)
{
    PHANTOM_ASSERT(m_pSignature);
    m_pSignature->setModifiers((getModifiers() & PHANTOM_R_METHOD_QUAL_MASK) | m_pSignature->getModifiers());
    setModifiers((m_pSignature->getModifiers() & PHANTOM_R_METHOD_QUAL_MASK) | getModifiers());
}

void Subroutine::terminate()
{
    if (m_pInstructions)
    {
        for (auto pIns : *m_pInstructions)
        {
            delete_<Instruction>(pIns);
        }
        delete_<Instructions>(m_pInstructions);
        m_pInstructions = nullptr;
    }
    Symbol::terminate();
}

void Subroutine::getQualifiedName(StringBuffer& a_Buf) const
{
    if (auto pNS = getNamingScope())
    {
        pNS->asLanguageElement()->getQualifiedDecoratedName(a_Buf);
        a_Buf += ':';
        a_Buf += ':';
        getName(a_Buf);
        return;
    }
    return getName(a_Buf);
}

void Subroutine::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    if (auto pNS = getNamingScope())
    {
        pNS->asLanguageElement()->getQualifiedDecoratedName(a_Buf);
        a_Buf += ':';
        a_Buf += ':';
        getName(a_Buf);
        m_pSignature->getQualifiedDecoratedName(a_Buf);
        return;
    }
    return getDecoratedName(a_Buf);
}

Types Subroutine::getParameterTypes() const
{
    return m_pSignature->getParameterTypes();
}

Type* Subroutine::getParameterType(size_t i) const
{
    return m_pSignature ? m_pSignature->getParameterType(i) : nullptr;
}

phantom::lang::Parameters const& Subroutine::getParameters() const
{
    return m_pSignature->getParameters();
}

Parameter* Subroutine::getParameter(StringView a_strName) const
{
    return m_pSignature ? m_pSignature->getParameter(a_strName) : nullptr;
}

size_t Subroutine::getRequiredArgumentCount() const
{
    return m_pSignature ? m_pSignature->getRequiredArgumentCount() : 0;
}

void Subroutine::getDecoratedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
    m_pSignature->getDecoratedName(a_Buf);
}

Instruction* Subroutine::createInstruction(int a_iOpCode, const CodeRangeLocation& a_Location,
                                           const MemoryLocation& a_MemoryLocation, void* a_pUserData /*= nullptr*/)
{
    if (m_pInstructions == nullptr)
    {
        m_pInstructions = new_<Instructions>();
    }
    Instruction* pNewInst = new_<Instruction>(a_iOpCode, a_Location.range, a_MemoryLocation, a_pUserData);
    m_pInstructions->push_back(pNewInst);
    pNewInst->m_pSubroutine = this;
    return pNewInst;
}

void Subroutine::deleteInstruction(Instruction* a_pInst)
{
    delete_<Instruction>(a_pInst);
}

Instructions const& Subroutine::getInstructions() const
{
    return m_pInstructions ? *m_pInstructions : m_EmptyInstructions;
}

Instruction* Subroutine::findInstructionAtCodePosition(const CodePosition&) const
{
    //     if(m_pInstructions == nullptr) return nullptr;
    //     if(!(containsCodePosition(position))) return nullptr;
    //     for(auto it = m_pInstructions->begin(); it != m_pInstructions->end(); ++it)
    //     {
    //         Instruction* pInstruction = *it;
    //         if(pInstruction->containsCodePosition(position))
    //             return pInstruction;
    //     }
    return nullptr;
}

Instruction* Subroutine::getInstructionAtMemoryAddress(const byte* a_pAddress) const
{
    if (m_pInstructions == nullptr)
        return nullptr;
    for (auto it = m_pInstructions->begin(); it != m_pInstructions->end(); ++it)
    {
        Instruction* pInstruction = *it;
        if (pInstruction->getMemoryStart() == a_pAddress)
            return pInstruction;
    }
    return nullptr;
}

Instruction* Subroutine::findInstructionStartingAtMemoryAddress(const byte* a_pAddress) const
{
    if (m_pInstructions == nullptr)
        return nullptr;
    for (auto it = m_pInstructions->begin(); it != m_pInstructions->end(); ++it)
    {
        Instruction* pInstruction = *it;
        if (pInstruction->getMemoryStart() == a_pAddress)
            return pInstruction;
    }
    return nullptr;
}

void Subroutine::setBlock(Block* a_pBlock)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(!m_pBlock);
    m_pBlock = a_pBlock;
    reinterpret_cast<LanguageElement*>(m_pBlock)->setOwner(this);
}

bool Subroutine::containsMemoryAddress(const byte* a_pAddress)
{
    return m_MemoryLocation.containsMemoryAddress(a_pAddress);
}

bool Subroutine::matches(StringView a_strName, TypesView a_FunctionSignature, Modifiers a_Modifiers) const
{
    return getName() == a_strName && matches(a_FunctionSignature, a_Modifiers);
}

bool Subroutine::matches(TypesView a_ParameterTypes, Modifiers a_Modifiers) const
{
    return m_pSignature->matches(a_ParameterTypes, a_Modifiers & (PHANTOM_R_METHOD_QUAL_MASK));
}

bool Subroutine::matches(const LanguageElements&, TypesView, Modifiers a_Modifiers) const
{
    PHANTOM_ASSERT(a_Modifiers == (a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK));
    PHANTOM_ASSERT_NO_IMPL(); /// template function not supported yet
    return false;
}

Type* Subroutine::getReturnType() const
{
    return m_pSignature ? m_pSignature->getReturnType() : nullptr;
}

OpaqueDelegate Subroutine::getOpaqueDelegate(void*) const
{
    return OpaqueDelegate();
}

OpaqueDelegate Subroutine::getOpaqueDelegate() const
{
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

void Subroutine::call(void** a_pArgs) const
{
    PHANTOM_ASSERT(asMethod() == nullptr);
    Type* pRetType = getReturnType();
    if (auto applyPointer = getApplyPointer())
    {
        if (isRVOCandidate()) // apply pointer must respect RVO convention
        {
            void*    retAddress = alloca(pRetType->getSize());
            auto     sc = pRetType->scopedConstruct(retAddress);
            size_t   argCount = m_pSignature->getParameters().size() + 1; // +1 is RV
            TempArgs newArgs;
            newArgs.resize(argCount);
            newArgs[0] = &retAddress;
            if (a_pArgs && argCount > 1)
            {
                memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
            }
            return applyPointer(newArgs.data(), nullptr);
        }
        else
        {
            // if method does not return a class (no rvo), we use a dummy buffer to welcome an eventual return value
            PHANTOM_ASSERT(pRetType->getSize() < 16);
            unsigned char dummy[16];
            return applyPointer(a_pArgs, dummy);
        }
    }
    return apply(a_pArgs, m_pSignature->getParameterCount());
}

void Subroutine::call(void** a_pArgs, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(a_pReturnAddress);
    if (auto applyPointer = getApplyPointer())
    {
        if (isRVOCandidate())
        {
            size_t   argCount = m_pSignature->getParameters().size() + 1; // +1 is RV
            TempArgs newArgs;
            newArgs.resize(argCount);
            newArgs[0] = &a_pReturnAddress;
            if (a_pArgs && argCount > 1)
            {
                memcpy(&newArgs[1], a_pArgs, (argCount - 1) * sizeof(void*));
            }
            applyPointer(newArgs.data(), nullptr);
        }
        else
        {
            applyPointer(a_pArgs, a_pReturnAddress);
        }
    }
    else
    {
        apply(a_pArgs, m_pSignature->getParameterCount(), a_pReturnAddress);
    }
}

void Subroutine::call(ExecutionContext& a_Context, void** a_pArgs) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_Context.resultPointer());
    size_t argCount =
    m_pSignature->getParameterCount() + (asMethod() != nullptr); // TODO : optimize by storing this value
    a_Context.call(const_cast<Subroutine*>(this), a_pArgs, argCount);
}

void Subroutine::call(ExecutionContext& a_Context, void** a_pArgs, size_t a_uiCount) const
{
    PHANTOM_VERIFY(a_Context.call(const_cast<Subroutine*>(this), a_pArgs, a_uiCount));
}

void Subroutine::placementCall(ExecutionContext& a_Context, void** a_pArgs) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_Context.resultPointer());
    size_t argCount =
    m_pSignature->getParameterCount() + (asMethod() != nullptr); // TODO : optimize by storing this value
    PHANTOM_VERIFY(a_Context.call(const_cast<Subroutine*>(this), a_pArgs, argCount));
}

void Subroutine::placementCall(void** a_pArgs, void* a_pReturnAddress) const
{
    call(a_pArgs, a_pReturnAddress);
}

void Subroutine::placementCallVarArg(ExecutionContext& a_Context, void** a_pArgs, Types const& a_VarArgTypes) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_Context.resultPointer());
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    size_t argCount = m_pSignature->getParameterCount() + (asMethod() != nullptr) +
    a_VarArgTypes.size(); // TODO : optimize by storing this value
    a_Context.call(const_cast<Subroutine*>(this), a_pArgs, argCount);
}

void Subroutine::placementCallVarArg(void** a_pArgs, Types const& a_VarArgTypes, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_pReturnAddress != nullptr);
    placementApply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr) + a_VarArgTypes.size(),
                   a_pReturnAddress);
}

void Subroutine::callVarArg(void** a_pArgs, Types const& a_VarArgTypes) const
{
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void));
    apply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr) + a_VarArgTypes.size());
}

void Subroutine::callVarArg(void** a_pArgs, Types const& a_VarArgTypes, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_pReturnAddress != nullptr);
    apply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr) + a_VarArgTypes.size(),
          a_pReturnAddress);
}

void Subroutine::callVarArg(ExecutionContext& a_Context, void** a_pArgs, Types const& a_VarArgTypes) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) || a_Context.resultPointer());
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    size_t argCount = m_pSignature->getParameterCount() + (asMethod() != nullptr) +
    a_VarArgTypes.size(); // TODO : optimize by storing this value
    a_Context.call(const_cast<Subroutine*>(this), a_pArgs, argCount);
}

bool Subroutine::equalsPointer(void* a_pPtrOnPtr) const
{
    return getClosure().address == *(void**)a_pPtrOnPtr;
}

void Subroutine::placementInvoke(void* a_pCallerAddress, void** a_pArgs, void* a_pReturnAddress) const
{
    invoke(a_pCallerAddress, a_pArgs, a_pReturnAddress);
}

void Subroutine::apply(void** a_pArgs, size_t a_uiCount, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(ExecutionContext::Current());
    ExecutionContext::Current()->pushResultPointer(a_pReturnAddress);
    ExecutionContext::Current()->call(const_cast<Subroutine*>(this), a_pArgs, a_uiCount);
    ExecutionContext::Current()->popResultPointer();
}

void Subroutine::apply(void** a_pArgs, size_t a_uiCount) const
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(!m_ApplyPointer);
    PHANTOM_ASSERT(ExecutionContext::Current());
    ExecutionContext::Current()->call(const_cast<Subroutine*>(this), a_pArgs, a_uiCount);
}

void Subroutine::placementApply(void** a_pArgs, size_t a_uiCount, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(ExecutionContext::Current());
    /// placement calls are only interpreted
    ExecutionContext::Current()->pushResultPointer(a_pReturnAddress);
    ExecutionContext::Current()->call(const_cast<Subroutine*>(this), a_pArgs, a_uiCount);
    ExecutionContext::Current()->popResultPointer();
}

void Subroutine::placementApply(void** a_pArgs, size_t a_uiCount) const
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(ExecutionContext::Current());
    /// placement calls are only interpreted
    ExecutionContext::Current()->call(const_cast<Subroutine*>(this), a_pArgs, a_uiCount);
}

ESignatureRelation Subroutine::getSignatureRelationWith(StringView a_strName, Signature* a_pSignature,
                                                        Modifiers a_Modifiers, uint) const
{
	StringView name = getName();
    if (a_strName != name // not same name
        && (name.front() != '~' || a_strName.front() != '~'))
        return e_SignatureRelation_None;
    if ((a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK) != (getModifiers() & PHANTOM_R_METHOD_QUAL_MASK))
        return e_SignatureRelation_None;

    if (getSignature()->getParameterCount() != a_pSignature->getParameterCount())
    {
        return e_SignatureRelation_None;
    }
    uint i = 0;
    for (; i < getSignature()->getParameterCount(); ++i)
    {
        if (!(getSignature()->getParameterType(i)->isSame(a_pSignature->getParameterType(i))))
        {
            return e_SignatureRelation_None;
        }
    }

    // here, to sum, we have :
    // ¤ same name,
    // ¤ same parameter count
    // ¤ same parameter types,

    // We now look at the return types
    Type* thisReturnType = getSignature()->getReturnType()->removeConst();
    Type* otherReturnType = a_pSignature->getReturnType()->removeConst();
    if (thisReturnType->isSame(otherReturnType))
    {
        return e_SignatureRelation_Equal;
    }

    // Only (const) class pointer/reference types can postulate to covariance and contravariance
    // We filter them by using address type implicit conversion
    if ((thisReturnType->asPointer() && otherReturnType->asPointer()) ||
        (thisReturnType->asLValueReference() && otherReturnType->asLValueReference()) ||
        (thisReturnType->asRValueReference() && otherReturnType->asRValueReference()))
    {
        // Covariance and Contravariance test
        if (thisReturnType->getUnderlyingType()->isA(otherReturnType->getUnderlyingType()))
        {
            return e_SignatureRelation_Covariant;
        }
        else if (otherReturnType->getUnderlyingType()->isA(thisReturnType->getUnderlyingType()))
        {
            // In the majority of case contravariance would be a forbidden case
            // We return a specific value for this result in case of ...
            return e_SignatureRelation_Contravariant;
        }
    }

    // Return types don't have any inheritance relation
    return e_SignatureRelation_Forbidden;
}

ESignatureRelation Subroutine::getSignatureRelationWith(Type* a_pReturnType, StringView a_strName, TypesView a_Types,
                                                        Modifiers a_Modifiers, uint) const
{
    if (a_strName != getName())
        return e_SignatureRelation_None;
    if ((a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK) != (getModifiers() & PHANTOM_R_METHOD_QUAL_MASK))
        return e_SignatureRelation_None;

    if (getSignature()->getParameterCount() != a_Types.size())
    {
        return e_SignatureRelation_None;
    }
    uint i = 0;
    for (; i < getSignature()->getParameterCount(); ++i)
    {
        if (!(getSignature()->getParameterType(i)->removeQualifiers()->isSame(a_Types[i]->removeQualifiers())))
        {
            return e_SignatureRelation_None;
        }
    }

    // here, to sum, we have :
    // ¤ same name,
    // ¤ same parameter count
    // ¤ same parameter types,

    // We now look at the return types
    Type* thisReturnType = getSignature()->getReturnType();
    Type* otherReturnType = a_pReturnType;
    if (thisReturnType->isSame(otherReturnType))
    {
        return e_SignatureRelation_Equal;
    }

    // Only (const) class pointer/reference types can postulate to covariance and contravariance
    // We filter them by using address type implicit conversion
    if ((thisReturnType->asPointer() && otherReturnType->asPointer()) ||
        (thisReturnType->asLValueReference() && otherReturnType->asLValueReference()) ||
        (thisReturnType->asRValueReference() && otherReturnType->asRValueReference()))
    {
        // Covariance and Contravariance test
        if (thisReturnType->getUnderlyingType()->isA(otherReturnType->getUnderlyingType()))
        {
            return e_SignatureRelation_Covariant;
        }
        else if (otherReturnType->getUnderlyingType()->isA(thisReturnType->getUnderlyingType()))
        {
            // In the majority of case contravariance would be a forbidden case
            // We return a specific value for this result in case of ...
            return e_SignatureRelation_Contravariant;
        }
    }

    // Return types don't have any inheritance relation
    return e_SignatureRelation_Forbidden;
}

ESignatureRelation Subroutine::getSignatureRelationWith(Subroutine* a_pSubroutine) const
{
    return getSignatureRelationWith(a_pSubroutine->getName(), a_pSubroutine->getSignature(),
                                    a_pSubroutine->getModifiers());
}

void Subroutine::getUniqueName(StringBuffer& a_Buf) const
{
    StringBuffer baseName;
    getName(baseName);
    if (baseName.compare(0, 8, "operator") == 0)
    {
        if (StringUtil::IsBlank(baseName[8]))
        {
            do
            {
                baseName.erase(8);
            } while (StringUtil::IsBlank(baseName[8]));

            char c = baseName[8];
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'))
            {
                // classical operator + - / * ! ? ...
                // => remain intact
            }
            else
            {
                // conversion function
                baseName.resize(9);
                baseName[8] = ' ';
                getReturnType()->getUniqueName(baseName);
            }
        }
    }
    LanguageElement* pOwner =
    getOwner() ? getOwner()->asTemplateSpecialization() ? getOwner()->getOwner() : getOwner() : nullptr;
    if (pOwner == nullptr)
    {
        a_Buf += '.';
        a_Buf += baseName;
        getUniqueTemplateDecoration(a_Buf);
        m_pSignature->getUniqueParameterTypeNameList(a_Buf);
        return;
    }
    // restart from beginning
    pOwner->getUniqueName(a_Buf);
    a_Buf += '.';
    a_Buf += baseName;
    getUniqueTemplateDecoration(a_Buf);
    m_pSignature->getUniqueParameterTypeNameList(a_Buf);
}

bool Subroutine::isRVOCandidate() const
{
    return getSignature()->isRVOCandidate();
}

void Subroutine::sortInstructions()
{
    if (m_pInstructions == nullptr)
        return;
    struct InstructionSorter
    {
        bool operator()(Instruction* a_pI0, Instruction* a_pI1) const { return *a_pI0 < *a_pI1; }
    };
    std::sort(m_pInstructions->begin(), m_pInstructions->end(), InstructionSorter());
}

void Subroutine::clearInstructions()
{
    while (m_pInstructions)
        deleteInstruction(m_pInstructions->back());
}

void Subroutine::saveArgs(void** a_pArgs, SmallVector<Variant, 10>& a_variants) const
{
    size_t c = getParameters().size();
    for (size_t i = 0; i < c; ++i)
    {
        Variant v;
        v.setType(getParameterType(i)->removeReference());
        getParameterType(i)->removeReference()->copyConstruct(v.data(), a_pArgs[i]);
        a_variants.push_back((Variant &&) v);
    }
}

void Subroutine::setNativeDefaultArgumentStrings(ArrayView<StringView> a_ArgExps)
{
    m_pSignature->setNativeDefaultArgumentStrings(a_ArgExps);
}

StringViews Subroutine::getNativeDefaultArgumentStrings() const
{
    return m_pSignature->getNativeDefaultArgumentStrings();
}

void MemoryLocation::setStart(byte* a_pAddress)
{
    PHANTOM_ASSERT(a_pAddress);
    PHANTOM_ASSERT(m_pEnd == 0 || a_pAddress < m_pEnd);
    m_pStart = a_pAddress;
}

void MemoryLocation::setEnd(byte* a_pAddress)
{
    PHANTOM_ASSERT(a_pAddress);
    PHANTOM_ASSERT(m_pStart == 0 || a_pAddress > m_pStart);
    m_pEnd = a_pAddress;
}

bool MemoryLocation::containsMemoryAddress(const byte* a_pAddress) const
{
    return a_pAddress >= m_pStart && a_pAddress < m_pEnd;
}

Source* Instruction::getSource() const
{
    return getSubroutine()->getSource();
}

} // namespace lang
} // namespace phantom
