// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#include <phantom/detail/Allocator.h>
#include <phantom/detail/Constructor.h>
#include <phantom/detail/Copier.h>
#include <phantom/detail/Hasher.h>
#include <phantom/detail/Mover.h>
#include <phantom/detail/StringConverter.h>
#include <phantom/lang/ClassType.h>
#include <phantom/lang/Promoter.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/ModifiersAndFlags.h>
#include <phantom/traits/MoveTraits.h>
#include <phantom/traits/SafeAlignOf.h>
#include <phantom/traits/SafeSizeOf.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty, typename t_Base>
class TypeT : public t_Base
{
public:
    typedef TypeT<t_Ty, t_Base>        SelfType;
    typedef t_Base                     BaseType;
    typedef phantom::Allocator<t_Ty>   AllocatorType;
    typedef phantom::Constructor<t_Ty> ConstructorType;
    typedef TypeInfosOf<t_Ty>          TypeInfosOfType;
    typedef phantom::Hasher<t_Ty>      HasherType;

protected:
    TypeT(StringView a_TypeName, Modifiers a_Modifiers, uint a_uiFlags)
        : BaseType(a_TypeName, SafeSizeOf<t_Ty>::value, SafeAlignOf<t_Ty>::value,
                   a_Modifiers | Modifiers(MetaModifiers<t_Ty>::value),
                   a_uiFlags | PHANTOM_R_FLAG_NATIVE | MetaFlags<t_Ty>::value)
    {
    }

public:
    ~TypeT() {}

    virtual uint64_t hash(const void* a_pInstance) const override
    {
        return HasherType::apply(reinterpret_cast<const t_Ty*>(a_pInstance));
    }

    virtual void copyAssign(void* a_pDest, void const* a_pSrc) const override
    {
        phantom::Copier<t_Ty>::copy(reinterpret_cast<t_Ty*>(a_pDest), reinterpret_cast<t_Ty const*>(a_pSrc));
    }

    virtual void moveAssign(void* a_pDest, void* a_pSrc) const override
    {
        phantom::Mover<t_Ty>::move(reinterpret_cast<t_Ty*>(a_pDest), reinterpret_cast<t_Ty*>(a_pSrc));
    }

    virtual void* allocate() const override { return AllocatorType::allocate(); }
    virtual void* allocate(size_t a_uiCount) const override { return AllocatorType::allocate(a_uiCount); }
    virtual void  deallocate(void* a_pAddress) const override
    {
        AllocatorType::deallocate(reinterpret_cast<t_Ty*>(a_pAddress));
    }
    virtual void deallocate(void* a_pAddress, size_t a_uiCount) const override
    {
        AllocatorType::deallocate(reinterpret_cast<t_Ty*>(a_pAddress), a_uiCount);
    }

    // Construction
    virtual void construct(void* a_pInstance) const override
    {
        ConstructorType::construct(reinterpret_cast<t_Ty*>(a_pInstance));
    }

    virtual void destroy(void* a_pInstance) const override
    {
        ConstructorType::destroy(reinterpret_cast<t_Ty*>(a_pInstance));
    }

    virtual void* newInstance() const override
    {
        t_Ty* pInstance = AllocatorType::allocate();
        ConstructorType::construct(pInstance);
        return pInstance;
    }

    virtual void deleteInstance(void* a_pInstance) const override
    {
        ConstructorType::destroy(reinterpret_cast<t_Ty*>(a_pInstance));
        AllocatorType::deallocate(reinterpret_cast<t_Ty*>(a_pInstance));
    }

    virtual bool isCopyable() const override { return IsCopyable<t_Ty>::value; }
    virtual bool isCopyAssignable() const override { return IsCopyAssignableAndNotDisabled<t_Ty>::value; }
    virtual bool isCopyConstructible() const override { return IsCopyConstructibleAndNotDisabled<t_Ty>::value; }
    virtual bool isMoveable() const override { return IsMoveable<t_Ty>::value; }
    virtual bool isMoveAssignable() const override { return IsMoveAssignableAndNotDisabled<t_Ty>::value; }
    virtual bool isMoveConstructible() const override { return IsMoveConstructibleAndNotDisabled<t_Ty>::value; }
    virtual bool isTriviallyMoveAssignable() const override
    {
        return IsTriviallyMoveAssignableAndNotDisabled<t_Ty>::value;
    }
    virtual bool isTriviallyMoveConstructible() const override
    {
        return IsTriviallyMoveConstructibleAndNotDisabled<t_Ty>::value;
    }
    virtual bool isTriviallyCopyAssignable() const override
    {
        return IsTriviallyCopyAssignableAndNotDisabled<t_Ty>::value;
    }
    virtual bool isTriviallyCopyConstructible() const override
    {
        return IsTriviallyCopyConstructibleAndNotDisabled<t_Ty>::value;
    }

    virtual String getQualifiedDecoratedName() const override
    {
        return TypeInfosOfType::object().qualifiedDecoratedName();
    }
    virtual String getQualifiedName() const override { return TypeInfosOfType::object().qualifiedName(); }
    virtual String getDecoratedName() const override { return TypeInfosOfType::object().decoratedName(); }
    virtual void   getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        TypeInfosOfType::object().qualifiedDecoratedName(a_Buf);
    }
    virtual void getQualifiedName(StringBuffer& a_Buf) const override
    {
        TypeInfosOfType::object().qualifiedName(a_Buf);
    }
    virtual void getDecoratedName(StringBuffer& a_Buf) const override
    {
        TypeInfosOfType::object().decoratedName(a_Buf);
    }
    virtual hash64 computeHash() const override { return TypeInfosOf<t_Ty>::object().hash; }
};

template<typename t_Ty, typename t_Base>
struct TypeOf<TypeT<t_Ty, t_Base> >
{
    static Type* object() { return Class::metaClass; }
};

template<typename t_Ty, typename t_Base>
struct MetaTypeOf<TypeT<t_Ty, t_Base> >
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang

} // namespace phantom
