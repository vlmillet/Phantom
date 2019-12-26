// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Subroutine.h"

#include "Application.h"
#include "ExecutionContext.h"
#include "Parameter.h"
#include "Signature.h"
#include "phantom/new.h"

#include <phantom/Variant.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
static Instructions m_EmptyInstructions;

Subroutine::Subroutine()
{
}

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
    if (m_pSignature->isShared())
    {
        addReferencedElement(a_pSignature);
    }
    else
    {
        addElement(m_pSignature);
    }
}

Subroutine::Subroutine(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature, ABI a_eABI,
                       Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_pSignature(Signature::Create(a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK, a_uiFlags & PHANTOM_R_FLAG_NATIVE)),
      m_eABI(a_eABI)
{
    PHANTOM_ASSERT(a_pScope);
    PHANTOM_ASSERT(m_pSignature);
    m_pSignature->setModifiers((getModifiers() & (PHANTOM_R_METHOD_QUAL_MASK)) | m_pSignature->getModifiers());
    a_pScope->addScopedElement(this);
    m_pSignature->parse(a_strSignature, this);
    a_pScope->removeScopedElement(this);
    if (m_pSignature->isShared())
    {
        addReferencedElement(m_pSignature);
    }
    else
    {
        addElement(m_pSignature);
    }
}

Subroutine::~Subroutine()
{
}

void Subroutine::terminate()
{
    if (m_pInstructions)
    {
        for (auto pIns : *m_pInstructions)
        {
            PHANTOM_DELETE(Instruction) pIns;
        }
        PHANTOM_DELETE(Instructions) m_pInstructions;
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
        m_pSignature->getQualifiedName(a_Buf);
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

phantom::reflection::Parameters const& Subroutine::getParameters() const
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
        m_pInstructions = PHANTOM_NEW(Instructions);
    }
    Instruction* pNewInst = PHANTOM_NEW(Instruction)(a_iOpCode, a_Location.range, a_MemoryLocation, a_pUserData);
    m_pInstructions->push_back(pNewInst);
    pNewInst->m_pSubroutine = this;
    return pNewInst;
}

void Subroutine::deleteInstruction(Instruction* a_pInst)
{
    PHANTOM_DELETE(Instruction) a_pInst;
}

Instructions const& Subroutine::getInstructions() const
{
    return m_pInstructions ? *m_pInstructions : m_EmptyInstructions;
}

Instruction* Subroutine::findInstructionAtCodePosition(const CodePosition&) const
{
    //     if(m_pInstructions == nullptr) return nullptr;
    //     if(NOT(containsCodePosition(position))) return nullptr;
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
    if (m_pBlock)
    {
        addElement(reinterpret_cast<LanguageElement*>(m_pBlock));
    }
}

bool Subroutine::containsMemoryAddress(const byte* a_pAddress)
{
    return m_MemoryLocation.containsMemoryAddress(a_pAddress);
}

void Subroutine::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    LanguageElement::onReferencedElementRemoved(a_pElement);
    if (reinterpret_cast<LanguageElement*>(m_pBlock) == a_pElement)
        m_pBlock = nullptr;
    else if (m_pSignature == a_pElement)
        m_pSignature = nullptr;
}

bool Subroutine::matches(StringView a_strName, TypesView a_FunctionSignature, Modifiers a_Modifiers) const
{
    return m_strName == a_strName AND matches(a_FunctionSignature, a_Modifiers);
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

phantom::OpaqueDelegate Subroutine::getOpaqueDelegate(void*) const
{
    return OpaqueDelegate(); // empty by default
}

void Subroutine::call(void** a_pArgs) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void));
    apply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr));
}

void Subroutine::call(void** a_pArgs, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_pReturnAddress != nullptr);
    apply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr), a_pReturnAddress);
}

void Subroutine::call(ExecutionContext& a_Context, void** a_pArgs) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_Context.resultPointer());
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
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_Context.resultPointer());
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
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_Context.resultPointer());
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    size_t argCount = m_pSignature->getParameterCount() + (asMethod() != nullptr) +
    a_VarArgTypes.size(); // TODO : optimize by storing this value
    a_Context.call(const_cast<Subroutine*>(this), a_pArgs, argCount);
}

void Subroutine::placementCallVarArg(void** a_pArgs, Types const& a_VarArgTypes, void* a_pReturnAddress) const
{
    PHANTOM_ASSERT(m_pSignature->isVariadic());
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_pReturnAddress != nullptr);
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
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_pReturnAddress != nullptr);
    apply(a_pArgs, m_pSignature->getParameterCount() + (asMethod() != nullptr) + a_VarArgTypes.size(),
          a_pReturnAddress);
}

void Subroutine::callVarArg(ExecutionContext& a_Context, void** a_pArgs, Types const& a_VarArgTypes) const
{
    PHANTOM_ASSERT(getReturnType() == PHANTOM_TYPEOF(void) OR a_Context.resultPointer());
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
    if (a_strName !=
        m_strName // not same name
        AND(m_strName.front() != '~' OR a_strName.front() != '~'))
        return e_SignatureRelation_None;
    if ((a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK) != (m_Modifiers & PHANTOM_R_METHOD_QUAL_MASK))
        return e_SignatureRelation_None;

    if (getSignature()->getParameterCount() != a_pSignature->getParameterCount())
    {
        return e_SignatureRelation_None;
    }
    uint i = 0;
    for (; i < getSignature()->getParameterCount(); ++i)
    {
        if (NOT(getSignature()->getParameterType(i)->isSame(a_pSignature->getParameterType(i))))
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
    if ((thisReturnType->asPointer() AND otherReturnType->asPointer())OR(
        thisReturnType->asLValueReference() AND otherReturnType->asLValueReference())
        OR(thisReturnType->asRValueReference() AND otherReturnType->asRValueReference()))
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
    if ((a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK) != (m_Modifiers & PHANTOM_R_METHOD_QUAL_MASK))
        return e_SignatureRelation_None;

    if (getSignature()->getParameterCount() != a_Types.size())
    {
        return e_SignatureRelation_None;
    }
    uint i = 0;
    for (; i < getSignature()->getParameterCount(); ++i)
    {
        if (NOT(getSignature()->getParameterType(i)->removeQualifiers()->isSame(a_Types[i]->removeQualifiers())))
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
    if ((thisReturnType->asPointer() AND otherReturnType->asPointer())OR(
        thisReturnType->asLValueReference() AND otherReturnType->asLValueReference())
        OR(thisReturnType->asRValueReference() AND otherReturnType->asRValueReference()))
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

static const char* operatorIdToKeyword(Operator id)
{
    switch (id)
    {
    case Operator::Plus:
        return "@plus";
    case Operator::Minus:
        return "@minus";
    case Operator::Add:
        return "@add";
    case Operator::Subtract:
        return "@sub";
    case Operator::Multiply:
        return "@mul";
    case Operator::Divide:
        return "@div";
    case Operator::Address:
        return "@addr";
    case Operator::Dereference:
        return "@deref";
    case Operator::PreDecrement:
        return "@predec";
    case Operator::PreIncrement:
        return "@preinc";
    case Operator::PostDecrement:
        return "@postdec";
    case Operator::PostIncrement:
        return "@postinc";
    case Operator::Equal:
        return "@eq";
    case Operator::NotEqual:
        return "@ne";
    case Operator::Greater:
        return "@gt";
    case Operator::Less:
        return "@lt";
    case Operator::GreaterEqual:
        return "@ge";
    case Operator::LessEqual:
        return "@le";
    case Operator::LogicalAnd:
        return "@land";
    case Operator::LogicalOr:
        return "@lor";
    case Operator::XOr:
        return "@bxor";
    case Operator::Not:
        return "@not";
    case Operator::BitAnd:
        return "@band";
    case Operator::BitOr:
        return "@bor";
    case Operator::Complement:
        return "@compl";
    case Operator::Modulo:
        return "@mod";
    case Operator::Comma:
        return "@comma";
    case Operator::ShiftLeft:
        return "@shl";
    case Operator::ShiftRight:
        return "@shr";
    case Operator::Assignment:
        return "@assign";
    case Operator::AssignmentAdd:
        return "@aadd";
    case Operator::AssignmentSubtract:
        return "@asub";
    case Operator::AssignmentMultiply:
        return "@amul";
    case Operator::AssignmentDivide:
        return "@adiv";
    case Operator::AssignmentBitAnd:
        return "@aband";
    case Operator::AssignmentBitOr:
        return "@abor";
    case Operator::AssignmentModulo:
        return "@amod";
    case Operator::AssignmentShiftLeft:
        return "@ashl";
    case Operator::AssignmentShiftRight:
        return "@ashr";
    case Operator::AssignmentXOr:
        return "@abxor";
    case Operator::Bracket:
        return "@elem";
    case Operator::Arrow:
        return "@->";
    case Operator::ArrowStar:
        return "@->*";
    case Operator::Parenthesis:
        return "@()";
    default:
        PHANTOM_ASSERT_NO_IMPL();
    }
    return "";
}

void Subroutine::getUniqueName(StringBuffer& a_Buf) const
{
    StringBuffer baseName;
    // TODO optimize, lazy guy
    getName(baseName);
    const char* cstr = m_strName.c_str();
    if (m_strName.compare(0, 8, "operator") == 0)
    {
        cstr += 8;
        char c = *cstr;
        if (NOT((c >= 'a' AND c <= 'z') OR(c >= 'A' AND c <= 'Z') OR(c >= '0' AND c <= '9') OR c == '_'))
        {
            if (*cstr == ' ') // conversion function
            {
                baseName += "@operator#";
                getReturnType()->getUniqueName(baseName);
            }
            else
            {
                Operator id = Operator::Parenthesis;
                if (strcmp(cstr, "()") == 0)
                {
                    id = Operator::Parenthesis;
                }
                else
                    switch (getSignature()->getParameterCount())
                    {
                    case 0:
                        id = Application::Get()->getBuiltInPreUnaryOperatorId(cstr);
                        if (id == Operator::Unknown)
                            id = Application::Get()->getBuiltInPostUnaryOperatorId(cstr);
                        break;
                    case 1:
                        id = Application::Get()->getBuiltInBinaryOperatorId(cstr);
                        if (id == Operator::Unknown)
                            id = Application::Get()->getBuiltInPostUnaryOperatorId(cstr);
                        break;
                    default:
                        break;
                    }
                PHANTOM_ASSERT(id != Operator::Unknown);
                baseName += "@operator";
                baseName += operatorIdToKeyword(id);
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
        bool operator()(Instruction* a_pI0, Instruction* a_pI1) const
        {
            return *a_pI0 < *a_pI1;
        }
    };
    std::sort(m_pInstructions->begin(), m_pInstructions->end(), InstructionSorter());
}

void Subroutine::clearInstructions()
{
    while (m_pInstructions)
        deleteInstruction(m_pInstructions->back());
}

void Subroutine::saveArgs(void** a_pArgs, Variants& a_variants) const
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

phantom::reflection::Operator Subroutine::getOperatorId() const
{
    if (getName().find("operator") == 0)
    {
        StringView view = getName().substr(8);
        if (view.size() && view.front() == ' ')
            return Operator::Conversion;

        switch (getParameters().size())
        {
        case 2:
            return Application::Get()->getBuiltInBinaryOperatorId(getName().substr(8));
        case 1:
            return Application::Get()->getBuiltInPostUnaryOperatorId(getName().substr(8));
        case 0:
            return Application::Get()->getBuiltInPreUnaryOperatorId(getName().substr(8));
        }
    }
    return Operator::Unknown;
}

Source* Instruction::getSource() const
{
    return getSubroutine()->getSource();
}

} // namespace reflection
} // namespace phantom
