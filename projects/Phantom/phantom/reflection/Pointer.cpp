// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Pointer.h"

#include <phantom/value.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Pointer::Pointer(Type* a_pPointeeType)
    : PointerType(a_pPointeeType, "*", sizeof(void*), std::alignment_of<void*>::value, a_pPointeeType->getModifiers(),
                  PHANTOM_R_FLAG_IMPLICIT | a_pPointeeType->getFlags())
{
}

Pointer::~Pointer()
{
}

void* Pointer::allocate() const
{
    return PHANTOM_ALLOCATE(void*);
}

void Pointer::deallocate(void* a_pInstance) const
{
    PHANTOM_DEALLOCATE(static_cast<void**>(a_pInstance), void*);
}

void* Pointer::allocate(size_t a_uiCount) const
{
    return PHANTOM_ALLOCATE_N(a_uiCount, void*);
}

void Pointer::deallocate(void* a_pChunk, size_t a_uiCount) const
{
    PHANTOM_DEALLOCATE_N(static_cast<void**>(a_pChunk), a_uiCount, void*);
}

void Pointer::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
    if (m_pUnderlyingType == a_pElement)
    {
        m_pUnderlyingType = nullptr;
    }
}

void Pointer::valueToLiteral(StringBuffer&, const void*) const
{
    PHANTOM_THROW_EXCEPTION(UnsupportedMethodException, "not implemented");
}

uint Pointer::getDataPointerLevel() const
{
    return m_pUnderlyingType->getDataPointerLevel() + 1;
}

Type* Pointer::removePointer() const
{
    return m_pUnderlyingType;
}

void Pointer::valueToString(StringBuffer& a_Buf, const void* src) const
{
    char buf[32];
    snprintf(buf, 32, "%zx", size_t(*((void**)(src))));
    a_Buf += (const char*)buf;
}

void Pointer::valueFromString(StringView a_str, void* dest) const
{
    *reinterpret_cast<void**>(dest) = ::phantom::lexical_cast<void*>(a_str);
}

Type* Pointer::asAddressType() const
{
    return const_cast<Pointer*>(this);
}

Pointer* Pointer::asPointer() const
{
    return const_cast<Pointer*>(this);
}

Type* Pointer::asClassAddressType() const
{
    return (m_pUnderlyingType AND m_pUnderlyingType->asClass()) ? const_cast<Pointer*>(this) : nullptr;
}

Pointer* Pointer::asClassPointer() const
{
    return (m_pUnderlyingType AND m_pUnderlyingType->asClass()) ? const_cast<Pointer*>(this) : nullptr;
}

Type* Pointer::asConstClassAddressType() const
{
    return (m_pUnderlyingType AND m_pUnderlyingType->asConstClass()) ? const_cast<Pointer*>(this) : nullptr;
}

Pointer* Pointer::asConstClassPointer() const
{
    return (m_pUnderlyingType AND m_pUnderlyingType->asConstClass()) ? const_cast<Pointer*>(this) : nullptr;
}

bool Pointer::convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const
{
    if (a_pDstType->getTypeKind() == TypeKind::Pointer)
    {
        *(void**)a_pDst = cast(static_cast<Pointer*>(a_pDstType)->getPointeeType(), *(void* const*)a_pSrc);
        return true;
    }
    return false;
}

Type* Pointer::removePointerOrArray() const
{
    return m_pUnderlyingType;
}

Type* Pointer::removeAddress() const
{
    return m_pUnderlyingType;
}

Type* Pointer::removeAllConst() const
{
    return m_pUnderlyingType->removeAllConst()->makePointer();
}

Type* Pointer::removeAllQualifiers() const
{
    return m_pUnderlyingType->removeAllQualifiers()->makePointer();
}

bool Pointer::isCopyable() const
{
    return true;
}

bool Pointer::isEquivalent(Type* a_pType) const
{
    if (isSame(a_pType))
        return true;
    if (Pointer* pPointer = a_pType->asPointer())
    {
        if (Class* pPointeeClass = pPointer->getPointeeType()->asClass())
        {
            if (Class* pThisPointeeClass = getPointeeType()->asClass())
            {
                return pThisPointeeClass->getBaseClassOffset(pPointeeClass) == 0 ||
                pPointeeClass->getBaseClassOffset(pThisPointeeClass) == 0;
            }
        }
        return pPointer->getPointeeType()->isEquivalent(getPointeeType());
    }
    return false;
}

bool Pointer::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const
{
    if (a_pType->asPointer())
    {
        a_Score += 10;
        return m_pUnderlyingType->partialAccepts(a_pType->removePointer(), a_Score, a_DeducedConstants);
    }
    return false;
}

bool Pointer::isSame(Symbol* a_pOther) const
{
    return a_pOther ==
    this OR(a_pOther->asPointer() AND m_pUnderlyingType->isSame(static_cast<Pointer*>(a_pOther)->m_pUnderlyingType));
}

Type* Pointer::getUnderlyingType() const
{
    return m_pUnderlyingType;
}

Type* Pointer::replicate(Type* a_pSource) const
{
    return m_pUnderlyingType->replicate(a_pSource->removePointer())->addPointer();
}

void Pointer::assignment(void** a_pArgs, void* a_pOutput)
{
    PointerType::assignment(a_pArgs, a_pOutput);
}

void Pointer::preIncrement(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pArgs[0] += m_pUnderlyingType->getSize();
    *(byte***)a_pOutput = (byte**)a_pArgs[0];
}

void Pointer::postIncrement(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = *(byte**)a_pArgs[0];
    *(byte**)a_pArgs[0] += m_pUnderlyingType->getSize();
}

void Pointer::preDecrement(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pArgs[0] -= m_pUnderlyingType->getSize();
    *(byte***)a_pOutput = (byte**)a_pArgs[0];
}

void Pointer::postDecrement(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = *(byte**)a_pArgs[0];
    *(byte**)a_pArgs[0] -= m_pUnderlyingType->getSize();
}

void Pointer::assignmentAdd(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pArgs[0] += m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[1]);
    *(byte***)a_pOutput = (byte**)a_pArgs[0];
}

void Pointer::assignmentSubtract(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pArgs[0] -= m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[1]);
    *(byte***)a_pOutput = (byte**)a_pArgs[0];
}

void Pointer::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += '*';
}

void Pointer::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += '*';
}

void Pointer::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += '*';
}

void Pointer::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += '*';
}

void Pointer::subtractPointer(void** a_pArgs, void* a_pOutput)
{
    *(ptrdiff_t*)a_pOutput = *(byte**)a_pArgs[0] - *(byte**)a_pArgs[1];
}

void Pointer::dereference(void** a_pArgs, void* a_pOutput)
{
    *(void**)a_pOutput = *(void**)a_pArgs[0];
}

void Pointer::plus(void** a_pArgs, void* a_pOutput)
{
    *(void**)a_pOutput = *(void**)a_pArgs[0];
}

void Pointer::add(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = *(byte**)a_pArgs[0] + m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[1]);
}

void Pointer::addRev(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[0]) + *(byte**)a_pArgs[1];
}

void Pointer::bracket(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = *(byte**)a_pArgs[0] + m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[1]);
}

void Pointer::bracketRev(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[0]) + *(byte**)a_pArgs[1];
}

void Pointer::subtract(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pOutput = *(byte**)a_pArgs[0] - m_pUnderlyingType->getSize() * (*(ptrdiff_t*)a_pArgs[1]);
}

} // namespace reflection
} // namespace phantom
