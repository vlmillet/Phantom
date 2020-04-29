#pragma once

#include "LanguageElement.h"

#include <phantom/utils/ForwardHeap.h>

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM LanguageElementUnit
{
public:
    void Delete(LanguageElement* a_pElem);
};

template<class Derived>
class LanguageElementUnitT : public LanguageElementUnit
{
    friend class LanguageElement;

protected:
    LanguageElementUnitT(ForwardHeapSequence* a_pAlloc) : m_pAlloc(a_pAlloc) {}

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
        T* ptr = new (m_pAlloc->allocate(sizeof(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewH(a_Owner.this_, ptr, T::MetaClass(), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _New(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _NewDeferred(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T))) T(std::forward<Args>(a_Args)...);
        this_()->_NewDeferredH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr,
                               lang::TypeInfosOf<T>::object().qualifiedDecoratedName());
        ptr->initialize();
        return ptr;
    }

protected:
    inline void setAllocator(ForwardHeapSequence* a_pAlloc) { m_pAlloc = a_pAlloc; }

private:
    inline Derived* this_() { return static_cast<Derived*>(this); }

private:
    ForwardHeapSequence* m_pAlloc;
};

} // namespace lang
} // namespace phantom
