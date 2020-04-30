#pragma once

#include "LanguageElement.h"

#include <phantom/CustomAllocator.h>
#include <phantom/utils/ForwardHeap.h>

namespace phantom
{
namespace lang
{
template<class Derived>
class LanguageElementUnitT
{
    friend class LanguageElement;
    using ThisType = LanguageElementUnitT<Derived>;

protected:
    LanguageElementUnitT(ForwardHeapSequence* a_pAlloc) : m_pAlloc(a_pAlloc) { _updateCustomAllocator(); }

private:
    template<typename T>
    static void _AssertSpecialSymbols()
    {
        // special symbols
        PHANTOM_STATIC_ASSERT(!(std::is_same<Module, T>::value), "use Application::newModule() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<Package, T>::value), "use Module::newPackage() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<Source, T>::value), "use Package::newSource() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<PackageFolder, T>::value),
                              "use PackageFolder::newPackageFolder() instead");
    }

    template<typename T, class... Args>
    T* _NewMeta(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewH(a_Owner.this_, ptr, T::MetaClass(), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _New(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _NewDeferred(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewDeferredH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr,
                               lang::TypeInfosOf<T>::object().qualifiedDecoratedName());
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _new(Args&&... a_Args)
    {
        return new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
    }

    template<class T>
    void _delete(TypeIndentityT<T*> a_p)
    {
        a_p->~T();
        m_pAlloc->deallocate(a_p);
    }

    CustomAllocator const* getUnitAllocator() const { return &m_CustomAlloc; }

protected:
    inline void setAllocator(ForwardHeapSequence* a_pAlloc)
    {
        m_pAlloc = a_pAlloc;
        _updateCustomAllocator();
    }

    void _updateCustomAllocator()
    {
        m_CustomAlloc.allocFunc = CustomAllocator::AllocFunc(this, &ThisType::_alloc);
        m_CustomAlloc.reallocFunc = CustomAllocator::ReallocFunc(this, &ThisType::_relloc);
        m_CustomAlloc.deallocFunc = CustomAllocator::DeallocFunc(this, &ThisType::_dealloc);
    }

    void* _alloc(size_t size, size_t align) { return m_pAlloc->allocate(size, align); }

    void* _relloc(void* ptr, size_t size, size_t align)
    {
        m_pAlloc->deallocate(ptr);
        return m_pAlloc->allocate(size, align);
    }

    void _dealloc(void* ptr) { m_pAlloc->deallocate(ptr); }

private:
    inline Derived* this_() { return static_cast<Derived*>(this); }

private:
    ForwardHeapSequence* m_pAlloc;
    CustomAllocator      m_CustomAlloc;
};

} // namespace lang
} // namespace phantom
