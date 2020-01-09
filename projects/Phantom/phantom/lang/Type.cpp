// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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

#include <phantom/detail/new.h>
#include <phantom/detail/core_internal.h>
#include <phantom/utils/Delegate.h>
#include <phantom/utils/crc64.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Type::Type(TypeKind a_eTypeKind, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_Modifiers, a_uiFlags), m_eTypeKind(a_eTypeKind)
{
}

Type::Type(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_eTypeKind(a_eTypeKind)
{
}

Type::Type(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
           Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_eTypeKind(a_eTypeKind),
      m_uiSize(a_uiSize),
      m_uiAlignment(a_uiAlignment)
{
}

Type::~Type()
{
    PHANTOM_ASSERT(m_pExtendedTypes == nullptr);
}

void Type::terminate()
{
    if (!isNative())
    {
        auto guard = m_ExtendedTypesMutex.autoLock();
        while (m_pExtendedTypes)
        {
            PHANTOM_DELETE_DYN m_pExtendedTypes->back();
        }
    }
    else
    {
        PHANTOM_DELETE(Types) m_pExtendedTypes;
        m_pExtendedTypes = nullptr;
    }
    Symbol::terminate();
    PHANTOM_ASSERT(m_pExtendedTypes == nullptr);
}

size_t Type::getSize() const
{
    PHANTOM_ASSERT(isVoid() OR isTemplateDependant() OR m_uiSize, "accessing size on incomplete type");
    return m_uiSize;
}

void Type::setSize(size_t a_uiSize)
{
    PHANTOM_ASSERT(m_uiSize == 0);
    m_uiSize = a_uiSize;
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
    return NOT(isAbstract()) AND isDefaultConstructible();
}

bool Type::isA(Type* a_pType) const
{
    return isSame(a_pType) OR a_pType->isTemplateDependant();
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

void Type::removeFromScope()
{
    PHANTOM_ASSERT(getOwner());
    getScope()->removeType(this);
}

Pointer* Type::createPointer() const
{
    return PHANTOM_DEFERRED_NEW(Pointer)(const_cast<Type*>(this));
}

Array* Type::createArray(size_t a_uiCount) const
{
    return PHANTOM_DEFERRED_NEW(Array)(const_cast<Type*>(this), a_uiCount);
}

LValueReference* Type::createLValueReference() const
{
    return PHANTOM_DEFERRED_NEW(LValueReference)(const_cast<Type*>(this));
}

RValueReference* Type::createRValueReference() const
{
    return PHANTOM_DEFERRED_NEW(RValueReference)(const_cast<Type*>(this));
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

void Type::placementDeleteInstance(void*) const
{
}

Scope* Type::getScope() const
{
    return getOwner() ? getOwner()->asScope() : nullptr;
}

Type* Type::getOwnerType() const
{
    return getOwner() ? getOwner()->asType() : nullptr;
}

void Type::onElementRemoved(LanguageElement* a_pElement)
{
    LanguageElement::onElementRemoved(a_pElement);
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes)
    {
        auto found = std::find(m_pExtendedTypes->begin(), m_pExtendedTypes->end(), a_pElement);
        if (found != m_pExtendedTypes->end())
        {
            m_pExtendedTypes->erase(found);
            if (m_pExtendedTypes->empty())
            {
                PHANTOM_DELETE(Types) m_pExtendedTypes;
                m_pExtendedTypes = nullptr;
            }
        }
    }
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
    if (m_Result == 0 OR m_MaxAlignment == 0)
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

Pointer* Type::makePointer() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        if (m_pExtendedTypes == nullptr)
            m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    Pointer* pType = getPointer();
    if (pType == nullptr)
    {
        pType = createPointer();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

LValueReference* Type::makeLValueReference() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    LValueReference* pType = getLValueReference();
    if (pType == nullptr)
    {
        pType = createLValueReference();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

RValueReference* Type::makeRValueReference() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    RValueReference* pType = getRValueReference();
    if (pType == nullptr)
    {
        pType = createRValueReference();
        if (!pType)
            return nullptr;
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

Array* Type::makeArray(size_t a_uiCount) const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    Array* pType = getArray(a_uiCount);
    if (pType == nullptr)
    {
        pType = createArray(a_uiCount);
        if (!pType)
            return nullptr;
        if (m_uiSize)
        {
            pType->setSize(m_uiSize * a_uiCount);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

ConstType* Type::makeConst() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    ConstType* pType = getConstType();
    if (pType == nullptr)
    {
        pType = createConstType();
        if (!pType)
            return nullptr;
        if (m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

VolatileType* Type::makeVolatile() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    VolatileType* pType = getVolatileType();
    if (pType == nullptr)
    {
        pType = createVolatileType();
        if (!pType)
            return nullptr;
        if (m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

ConstVolatileType* Type::makeConstVolatile() const
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    if (m_pExtendedTypes == nullptr)
    {
        m_pExtendedTypes = PHANTOM_NEW(Types);
    }
    ConstVolatileType* pType = getConstVolatileType();
    if (pType == nullptr)
    {
        pType = createConstVolatileType();
        if (!pType)
            return nullptr;
        if (m_uiSize)
        {
            pType->setSize(m_uiSize);
            pType->setAlignment(m_uiAlignment);
        }
        pType->addFlags(PHANTOM_R_FLAG_IMPLICIT);
        m_pExtendedTypes->push_back(pType);
        const_cast<Type*>(this)->addElement(pType);
    }
    return pType;
}

Type* Type::makePointer(size_t a_uiPointerLevel) const
{
    if (a_uiPointerLevel == 0)
        return (Type*)this;
    return makePointer()->makePointer(a_uiPointerLevel - 1);
}

void Type::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    LanguageElement::onReferencedElementRemoved(a_pElement);
}

void Type::onAncestorAboutToBeChanged(LanguageElement* a_pOwner)
{
    Symbol::onAncestorAboutToBeChanged(a_pOwner);
    if (!isNative())
    {
        if (a_pOwner ==
            getModule() AND(getTypeKind() == TypeKind::Class OR getTypeKind() == TypeKind::Union OR getTypeKind() ==
                            TypeKind::Structure OR getTypeKind() == TypeKind::Enum)
            AND((m_Modifiers & (PHANTOM_R_CONST | PHANTOM_R_VOLATILE)) == 0) AND !isTemplateDependant()
            AND NOT(getSource()->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)))
        {
            static_cast<Module*>(a_pOwner)->_unregisterType(m_Hash, this);
        }
    }
}

void Type::onAncestorChanged(LanguageElement* a_pOwner)
{
    Symbol::onAncestorChanged(a_pOwner);
    if (!isNative())
    {
        if (a_pOwner ==
            getModule() AND(getTypeKind() == TypeKind::Class OR getTypeKind() == TypeKind::Union OR getTypeKind() ==
                            TypeKind::Structure OR getTypeKind() == TypeKind::Enum)
            AND((m_Modifiers & (PHANTOM_R_CONST | PHANTOM_R_VOLATILE)) == 0) AND !isTemplateDependant()
            AND NOT(getSource()->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)))
        {
            auto nscope = getNamingScope();
            PHANTOM_ASSERT(nscope, "naming scope (namespace or class type) must be defined before inserting to module");
            static_cast<Module*>(a_pOwner)->_registerType(getHash(), this);
        }
    }
}

ConstType* Type::createConstType() const
{
    return PHANTOM_DEFERRED_NEW(ConstType)(const_cast<Type*>(this));
}

VolatileType* Type::createVolatileType() const
{
    return PHANTOM_DEFERRED_NEW(VolatileType)(const_cast<Type*>(this));
}

ConstVolatileType* Type::createConstVolatileType() const
{
    return PHANTOM_DEFERRED_NEW(ConstVolatileType)(const_cast<Type*>(this));
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

void Type::removeExtendedType(Type* a_pType)
{
    auto guard = m_ExtendedTypesMutex.autoLock();
    PHANTOM_ASSERT(m_pExtendedTypes);
    m_pExtendedTypes->erase(std::find(m_pExtendedTypes->begin(), m_pExtendedTypes->end(), a_pType));
    if (m_pExtendedTypes->empty())
    {
        PHANTOM_DELETE(Types) m_pExtendedTypes;
        m_pExtendedTypes = nullptr;
    }
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

Type* Type::addConst() const
{
    return makeConst();
}

Type* Type::addVolatile() const
{
    return makeVolatile();
}

Type* Type::addConstLValueReference() const
{
    return addConst()->addLValueReference();
}

Type* Type::addConstRValueReference() const
{
    return addConst()->addRValueReference();
}

Type* Type::addConstVolatile() const
{
    return makeConstVolatile();
}

Type* Type::addArray(size_t a_uiCount) const
{
    return makeArray(a_uiCount);
}

Type* Type::addLValueReference() const
{
    return makeLValueReference();
}

Type* Type::addRValueReference() const
{
    return makeRValueReference();
}

Type* Type::addPointer() const
{
    return makePointer();
}

Type* Type::addCustom(int) const
{
    return const_cast<Type*>(this);
}

Type* Type::removeConstLValueReference() const
{
    Type* pNoRef = removeLValueReference();
    if (pNoRef != this AND pNoRef->asConstType())
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

} // namespace reflection
} // namespace phantom
