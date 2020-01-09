// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Class.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM ContainerClass : public Class
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ContainerClass);

    struct RTData
    {
        Method* m_pFunc_size = nullptr;
        Method* m_pFunc_clear = nullptr;
        Method* m_pFunc_begin = nullptr;
        Method* m_pFunc_end = nullptr;
        Method* m_pFunc_beginc = nullptr;
        Method* m_pFunc_endc = nullptr;
        Method* m_pFunc_eraseAt = nullptr;
    };

protected:
    ContainerClass(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                   Modifiers a_Modifiers, uint a_uiFlags);
    ContainerClass(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    ~ContainerClass() override;

    inline Type* getValueType() const { return m_pValueType; }
    void         setValueType(Type* a_pValueType);

    ContainerClass* asContainerClass() const override { return const_cast<ContainerClass*>(this); }

    virtual void   begin(void* a_pContainer, void* a_pOutIt) const;
    virtual void   begin(void const* a_pContainer, void* a_pOutIt) const;
    virtual void   end(void* a_pContainer, void* a_pOutIt) const;
    virtual void   end(void const* a_pContainer, void* a_pOutIt) const;
    virtual void*  dereferenceIterator(void* a_pIt) const;
    virtual void   advanceIterator(void* a_pIt, size_t a_N = 1) const;
    virtual bool   compareIterators(void* a_pIt1, void* a_pIt2) const;
    virtual size_t size(void const* a_pContainer) const;
    virtual void   erase(void* a_pContainer, void const* a_pIt) const;
    virtual void   clear(void* a_pContainer) const;

    virtual void        eraseAt(void* a_pContainer, size_t a_uiIndex) const;
    virtual const void* referenceAt(void const* a_pContainer, size_t a_uiIndex) const;
    virtual void*       referenceAt(void* a_pContainer, size_t a_uiIndex) const;

    virtual Type* getIteratorType() const;
    virtual Type* getConstIteratorType() const;

    Property* getSizeProperty() const;

protected:
    virtual Property* createSizeProperty() const;

protected:
    Type*             m_pValueType = nullptr;
    mutable RTData*   m_Data = nullptr;
    mutable Property* m_pSizeProperty = nullptr;

private:
    void _initBegin() const;
    void _initBeginC() const;
};

} // namespace lang
} // namespace phantom
