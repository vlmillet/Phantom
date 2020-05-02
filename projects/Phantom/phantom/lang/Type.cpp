// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Type.h"

#include "Application.h"
#include "Array.h"
#include "ConstVolatileType.h"
#include "DataElement.h"
#include "LValueReference.h"
#include "Module.h"
#include "Pointer.h"
#include "RValueReference.h"
#include "Source.h"
#include "TemplateSpecialization.h"

#include <phantom/detail/core_internal.h>
#include <phantom/utils/Delegate.h>
#include <phantom/utils/crc64.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
extern bool g_ReleasingPhantomModule;

Type::Type(TypeKind a_eTypeKind, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_Modifiers, a_uiFlags), m_eTypeKind(a_eTypeKind)
{
}

Type::Type(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_eTypeKind(a_eTypeKind)
{
}
Type::Type(TypeKind a_eTypeKind, Type* a_pUnderlyingType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_Modifiers, a_uiFlags), m_pUnderlyingType(a_pUnderlyingType), m_eTypeKind(a_eTypeKind)
{
}
Type::Type(TypeKind a_eTypeKind, Type* a_pUnderlyingType, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
           uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_pUnderlyingType(a_pUnderlyingType), m_eTypeKind(a_eTypeKind)
{
    addReferencedElement(m_pUnderlyingType);
}

Type::Type(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
           Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_eTypeKind(a_eTypeKind),
      m_uiSize(uint32_t(a_uiSize)),
      m_uiAlignment(uint16_t(a_uiAlignment))
{
}

Type::Type(TypeKind a_eTypeKind, Type* a_pUnderlyingType, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
           Modifiers a_Modifiers, uint a_uiFlags)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_pUnderlyingType(a_pUnderlyingType),
      m_eTypeKind(a_eTypeKind),
      m_uiSize(uint32_t(a_uiSize)),
      m_uiAlignment(uint16_t(a_uiAlignment))
{
    addReferencedElement(m_pUnderlyingType);
}

Type::~Type()
{
}

void Type::terminate()
{
	if (m_pExtendedTypes)
	{
		delete_<Types>(m_pExtendedTypes);
		m_pExtendedTypes = nullptr;
	}
    Symbol::terminate();
}

size_t Type::getSize() const
{
    PHANTOM_ASSERT(isVoid() || isTemplateDependant() || m_uiSize, "accessing size on incomplete type");
    return m_uiSize;
}

void Type::setSize(size_t a_uiSize)
{
    PHANTOM_ASSERT(m_uiSize == 0);
    m_uiSize = uint32_t(a_uiSize);
}

void Type::getFlattenedAggregateFields(AggregateFields& _aggregateFields) const
{
    AggregateField this_;
    this_.offset = 0;
    this_.type = const_cast<Type*>(this);
    _aggregateFields.push_back(this_);
}

bool Type::isEquivalent(Type* a_pOther) const
{
    if (isSame(a_pOther))
        return true;
    if (m_uiSize != a_pOther->m_uiSize || m_uiAlignment != a_pOther->m_uiAlignment)
        return false;
    AggregateFields thisFields;
    getFlattenedAggregateFields(thisFields);
    AggregateFields otherFields;
    getFlattenedAggregateFields(otherFields);
    size_t count = thisFields.size();
    if (count != otherFields.size())
        return false;
    for (size_t i = 0; i < count; ++i)
    {
        AggregateField fieldOther = otherFields[i];
        AggregateField fieldThis = thisFields[i];
        if (fieldOther.offset != fieldThis.offset || !fieldOther.type->isEquivalent(fieldOther.type))
            return false;
    }
    return true;
}

size_t Type::getAlignment() const
{
    return m_uiAlignment;
}

bool Type::isDefaultInstanciable() const
{
    return !(isAbstract()) && isDefaultConstructible();
}

bool Type::isA(Type* a_pType) const
{
    return isSame(a_pType) || a_pType->isTemplateDependant();
}

bool Type::isAuto() const
{
    return Application::Get()->getBuiltInType("auto") == const_cast<Type*>(this);
}

bool Type::hasAuto() const
{
    return removeEverything()->isAuto();
}

Type::TypeRelation Type::getRelationWith(Type* a_pType) const
{
    if (a_pType == this)
        return TypeRelation::Equal;
    return TypeRelation::None;
}

bool Type::equal(void const* a_pSrc0, void const* a_pSrc1) const
{
    return memcmp(a_pSrc0, a_pSrc1, m_uiSize) == 0;
}

Types& Type::_extTypes() const
{
    if (m_pExtendedTypes == nullptr)
		m_pExtendedTypes = new_<Types>(getAllocator());
    return *m_pExtendedTypes;
}

Type* Type::createPointer()
{
    switch (m_eTypeKind)
    {
    case TypeKind::Function:
        return (Type*)getSource()->functionPointerType((FunctionType*)this, ABI::CDecl);

    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<Pointer>(const_cast<Type*>(this));
    }
}

Array* Type::createArray(size_t a_uiCount)
{
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<Array>(const_cast<Type*>(this), a_uiCount);
    }
}

LValueReference* Type::createLValueReference()
{
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<LValueReference>(const_cast<Type*>(this));
    }
}

RValueReference* Type::createRValueReference()
{
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<RValueReference>(const_cast<Type*>(this));
    }
}

bool Type::convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const
{
    if (isSame(a_pDstType) && isCopyable())
    {
        copyConstruct(a_pDst, a_pSrc);
        return true;
    }
    return false;
}

void* Type::allocate() const
{
    // allocate m_uiSize bytes
    void* memory = PHANTOM_MALLOC(m_uiSize);
    return memory;
}

void Type::deallocate(void* a_pInstance) const
{
    PHANTOM_FREE(a_pInstance);
}

void* Type::allocate(size_t a_uiCount) const
{
    void* pAllocation = PHANTOM_MALLOC(a_uiCount * m_uiSize);
    return pAllocation;
}

void Type::deallocate(void* a_pChunk, size_t) const
{
    PHANTOM_FREE(a_pChunk);
}

uint64_t Type::hash(const void* a_pInstance) const
{
    return crc64(0, reinterpret_cast<const unsigned char*>(a_pInstance), m_uiSize);
}

void* Type::newInstance() const
{
    void* pInstance = allocate(m_uiSize);
    construct(pInstance);
    return pInstance;
}

void Type::deleteInstance(void* a_pInstance) const
{
    destroy(a_pInstance);
    deallocate(a_pInstance);
}

void* Type::placementNewInstance(void* a_pMemory) const
{
    memset(a_pMemory, 0, m_uiSize);
    return a_pMemory;
}

void Type::placementDeleteInstance(void*) const {}

Scope* Type::getScope() const
{
    return getOwner() ? getOwner()->asScope() : nullptr;
}

Type* Type::getOwnerType() const
{
    return getOwner() ? getOwner()->asType() : nullptr;
}

void Type::AlignmentComputer::alignStruct(DataElements const& a_DataElements, size_t& a_OutSize, size_t& a_OutAlignment)
{
    for (auto pDataElement : a_DataElements)
    {
        pDataElement->setOffset(push(pDataElement->getSize(), pDataElement->getAlignment()));
    }
    a_OutSize = align();
    if (maxAlignment() == 0 && a_OutSize == 0)
    {
        setMaxAlignement(1);
    }
    if (a_OutSize == 0)
    {
        a_OutSize = 1; // Default class size
    }
    a_OutAlignment = maxAlignment();
    PHANTOM_ASSERT(a_OutAlignment);
}

void Type::AlignmentComputer::alignUnion(DataElements const& a_DataElements, size_t& a_OutSize, size_t& a_OutAlignment)
{
    for (auto pDataElement : a_DataElements)
    {
        if (pDataElement->getSize() > a_OutSize)
            a_OutSize = pDataElement->getSize();
        if (pDataElement->getAlignment() > a_OutAlignment)
            a_OutAlignment = pDataElement->getAlignment();
    }
    size_t offset = push(a_OutSize, a_OutAlignment);
    for (auto pDataElement : a_DataElements)
    {
        pDataElement->setOffset(offset); // every data element has same offset
    }
    a_OutSize = align();
    if (maxAlignment() == 0 && a_OutSize == 0)
    {
        setMaxAlignement(1);
    }
    if (a_OutSize == 0)
    {
        a_OutSize = 1; // Default class size
    }
    a_OutAlignment = maxAlignment();
    PHANTOM_ASSERT(a_OutAlignment);
}

size_t Type::AlignmentComputer::push(Type* a_pType)
{
    return push(a_pType->getSize(), a_pType->getAlignment());
}

size_t Type::AlignmentComputer::push(size_t a_uiSize, size_t a_uiAlignment)
{
    if (a_uiAlignment > m_MaxAlignment)
    {
        m_MaxAlignment = a_uiAlignment;
    }

    size_t modulo = m_Result % a_uiAlignment;
    if (modulo)
        m_Result += (a_uiAlignment - modulo);

    size_t offset = m_Result;
    m_Result += a_uiSize;
    return offset;
}

size_t Type::AlignmentComputer::align()
{
    if (m_Result == 0 || m_MaxAlignment == 0)
        return 0;
    size_t modulo = m_Result % m_MaxAlignment;
    if (modulo)
        m_Result += (m_MaxAlignment - modulo);

    return m_Result;
}

void Type::fetchElements(LanguageElements& out, Class* a_pClass) const
{
    LanguageElement::fetchElements(out, a_pClass);
}

void Type::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    memcpy(a_pDest, a_pSrc, m_uiSize);
}

void Type::moveConstruct(void* a_pDest, void* a_pSrc) const
{
    copyConstruct(a_pDest, a_pSrc);
}

Type* Type::getCommonBaseAncestor(Type* a_pType) const
{
    PHANTOM_ASSERT(a_pType);
    if (a_pType == this)
        return a_pType;
    return nullptr;
}

Pointer* Type::getPointer() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        Pointer* pPointer = (*it)->asPointer();
        if (pPointer)
            return pPointer;
    }
    return nullptr;
}

LValueReference* Type::getLValueReference() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        LValueReference* pPointer = (*it)->asLValueReference();
        if (pPointer)
            return pPointer;
    }
    return nullptr;
}

RValueReference* Type::getRValueReference() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        RValueReference* pRValueRT = (*it)->asRValueReference();
        if (pRValueRT)
            return pRValueRT;
    }
    return nullptr;
}

Array* Type::getArray(size_t a_uiCount) const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        Array* pArray = (*it)->asArray();
        if (pArray && pArray->getItemCount() == a_uiCount)
            return pArray;
    }
    return nullptr;
}

ConstType* Type::getConstType() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        ConstType* pConstType = (*it)->asConstType();
        if (pConstType)
            return pConstType;
    }
    return nullptr;
}

VolatileType* Type::getVolatileType() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        VolatileType* pVolatileType = (*it)->asVolatileType();
        if (pVolatileType)
            return pVolatileType;
    }
    return nullptr;
}

ConstVolatileType* Type::getConstVolatileType() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
        return nullptr;
    for (auto it = m_pExtendedTypes->begin(); it != m_pExtendedTypes->end(); ++it)
    {
        ConstVolatileType* pConstVolatileType = (*it)->asConstVolatileType();
        if (pConstVolatileType)
            return pConstVolatileType;
    }
    return nullptr;
}

Type* Type::makePointer() const
{
    auto  guard = m_ExtendedTypesMutex.autoLock();
    Type* pType = getPointer();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createPointer();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

LValueReference* Type::makeLValueReference() const
{
    auto             guard = m_ExtendedTypesMutex.autoLock();
    LValueReference* pType = getLValueReference();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createLValueReference();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

RValueReference* Type::makeRValueReference() const
{
    auto             guard = m_ExtendedTypesMutex.autoLock();
    RValueReference* pType = getRValueReference();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createRValueReference();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

Array* Type::makeArray(size_t a_uiCount) const
{
    auto   guard = m_ExtendedTypesMutex.autoLock();
    Array* pType = getArray(a_uiCount);
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createArray(a_uiCount);
        if (!pType)
            return nullptr;
        if (!isNative() && a_uiCount && m_uiSize)
        {
            pType->setSize(m_uiSize * a_uiCount);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

ConstType* Type::makeConst() const
{
    auto       guard = m_ExtendedTypesMutex.autoLock();
    ConstType* pType = getConstType();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createConstType();
        if (!pType)
            return nullptr;
        if (!isNative() && m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

VolatileType* Type::makeVolatile() const
{
    auto          guard = m_ExtendedTypesMutex.autoLock();
    VolatileType* pType = getVolatileType();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createVolatileType();
        if (!pType)
            return nullptr;
        if (!isNative() && m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

ConstVolatileType* Type::makeConstVolatile() const
{
    auto               guard = m_ExtendedTypesMutex.autoLock();
    ConstVolatileType* pType = getConstVolatileType();
    if (pType == nullptr)
    {
        pType = const_cast<Type*>(this)->createConstVolatileType();
        if (!pType)
            return nullptr;
        if (!isNative() && m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        _extTypes().push_back(pType);
    }
    return pType;
}

Type* Type::makePointer(size_t a_uiPointerLevel) const
{
    if (a_uiPointerLevel == 0)
        return (Type*)this;
    return makePointer()->makePointer(a_uiPointerLevel - 1);
}

void Type::onNamespaceChanging(Namespace* /*a_pNamespace*/)
{
	if (!isNative())
	{
		if ((getTypeKind() == TypeKind::Class || getTypeKind() == TypeKind::Union ||
			getTypeKind() == TypeKind::Structure || getTypeKind() == TypeKind::Enum) &&
			((m_Modifiers & (PHANTOM_R_CONST | PHANTOM_R_VOLATILE)) == 0) && !isTemplateDependant() &&
			!(getSource()->getVisibility() == Visibility::Private))
		{
			getModule()->_unregisterType(m_Hash, this);
		}
	}
}

void Type::onNamespaceChanged(Namespace* /*a_pNamespace*/)
{
    if (!isNative())
    {
        if ((getTypeKind() == TypeKind::Class || getTypeKind() == TypeKind::Union ||
             getTypeKind() == TypeKind::Structure || getTypeKind() == TypeKind::Enum) &&
            ((m_Modifiers & (PHANTOM_R_CONST | PHANTOM_R_VOLATILE)) == 0) && !isTemplateDependant() &&
            !(getSource()->getVisibility() == Visibility::Private))
        {
            getModule()->_registerType(getHash(), this);
        }
    }
}

ConstType* Type::createConstType()
{
    if (isQualified())
        return nullptr;
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<ConstType>(this);
    }
}

VolatileType* Type::createVolatileType()
{
    if (isQualified())
        return nullptr;
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<VolatileType>(this);
    }
}

ConstVolatileType* Type::createConstVolatileType()
{
    if (isQualified())
        return nullptr;
    switch (m_eTypeKind)
    {
    case TypeKind::LValueReference:
    case TypeKind::RValueReference:
        return nullptr;
    default:
        return NewDeferred<ConstVolatileType>(this);
    }
}

void Type::valueToString(StringBuffer&, const void*) const
{
    PHANTOM_WARNING(false, "'valueToString' not available for '%s'", getQualifiedDecoratedName().c_str());
}

void Type::valueFromString(StringView, void*) const
{
    PHANTOM_WARNING(false, "'valueFromString' not available for '%s'", getQualifiedDecoratedName().c_str());
}

void Type::valueToLiteral(StringBuffer& a_Buf, const void* src) const
{
    return valueToString(a_Buf, src);
}

bool Type::isCopyable() const
{
    return true;
}

bool Type::isCopyConstructible() const
{
    return isCopyable();
}

bool Type::isCopyAssignable() const
{
    return isCopyable();
}

bool Type::isTriviallyCopyAssignable() const
{
    return isCopyable();
}

bool Type::isTriviallyCopyConstructible() const
{
    return isCopyable();
}

bool Type::isMoveable() const
{
    return true;
}

bool Type::partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score, PlaceholderMap& a_Deductions) const
{
    Type* pType = a_pLanguageElement->asType();
    if (pType)
    {
        return partialAccepts(pType, a_Score, a_Deductions);
    }
    return false;
}

bool Type::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap&) const
{
    if (a_pType == this)
    {
        a_Score = ~size_t(0);
        return true;
    }
    return false;
}

int Type::ConversionRank(TypeKind id)
{
    switch (id)
    {
    case TypeKind::Bool:
        return 1;

    case TypeKind::Char:
        return 2;
    case TypeKind::UChar:
        return 2;
    case TypeKind::SChar:
        return 2;

    case TypeKind::Short:
        return 3;
    case TypeKind::UShort:
        return 3;
        PHANTOM_IF_WCHAR_T(case TypeKind::WChar : return 3;)
        PHANTOM_IF_CHAR16_T(case TypeKind::Char16 : return 3;)

        PHANTOM_IF_CHAR32_T(case TypeKind::Char32 : return 4;)
    case TypeKind::Int:
        return 4;
    case TypeKind::UInt:
        return 4;

    case TypeKind::Long:
        return 5;
    case TypeKind::ULong:
        return 5;

    case TypeKind::LongLong:
        return 6;
    case TypeKind::ULongLong:
        return 6;
    default:
        break;
    }
    return 0;
}

bool Type::hasStrongDependencyOnType(Type*) const
{
    return false;
}

Type* Type::addConstLValueReference() const
{
    return addConst()->addLValueReference();
}

Type* Type::addConstRValueReference() const
{
    return addConst()->addRValueReference();
}

Type* Type::addCustom(int) const
{
    return const_cast<Type*>(this);
}

Type* Type::removeConstLValueReference() const
{
    Type* pNoRef = removeLValueReference();
    if (pNoRef != this && pNoRef->asConstType())
        return pNoRef->removeConst();
    return const_cast<Type*>(this);
}

Template* Type::getTemplate() const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    return pSpec ? pSpec->getTemplate() : nullptr;
}

ptrdiff_t Type::getPointerAdjustmentOffset(Type*) const
{
    return 0;
}

bool Type::isMoveConstructible() const
{
    return isMoveable();
}

bool Type::isMoveAssignable() const
{
    return isMoveable();
}

bool Type::isTriviallyMoveAssignable() const
{
    return isMoveable();
}

bool Type::isTriviallyMoveConstructible() const
{
    return isMoveable();
}

bool Type::hasCopyDisabled() const
{
    return false;
}

bool Type::hasMoveDisabled() const
{
    return false;
}

Type* Type::unsignedType() const
{
    return nullptr;
}

Type* Type::signedType() const
{
    return nullptr;
}

Type* Type::longType() const
{
    return nullptr;
}

bool Type::isSignedInteger() const
{
    return false;
}

bool Type::isUnsignedInteger() const
{
    return false;
}

Type* Type::promote() const
{
    return const_cast<Type*>(this);
}

void Type::copyAssign(void* a_pDest, void const* a_pSrc) const
{
    memcpy(a_pDest, a_pSrc, m_uiSize);
}

void Type::moveAssign(void* a_pDest, void* a_pSrc) const
{
    copyAssign(a_pDest, a_pSrc);
}

} // namespace lang
} // namespace phantom
