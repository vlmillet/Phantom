// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/SequentialContainerClass.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base base for stl compliant vector containers.
class PHANTOM_EXPORT_PHANTOM ArrayClass : public Class
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ArrayClass);

    struct RTData
    {
        Method* m_pFunc_data = nullptr;
        Method* m_pFunc_datac = nullptr;
        Method* m_pFunc_refAt = nullptr;
        Method* m_pFunc_refAtC = nullptr;
        Method* m_pFunc_size = nullptr;
        Method* m_pFunc_begin = nullptr;
        Method* m_pFunc_end = nullptr;
        Method* m_pFunc_beginc = nullptr;
        Method* m_pFunc_endc = nullptr;
    };

public:
    ArrayClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize();
    void terminate() override;

    Type*  getItemType() const { return m_pItemType; }
    size_t getItemCount() const { return m_ItemCount; }

    virtual void setItemValue(void* a_pContainer, size_t a_uiIndex, void const* a_pSrc)
    {
        getItemType()->copyAssign(referenceAt(a_pContainer, a_uiIndex), a_pSrc);
    }
    virtual void getItemValue(void const* a_pContainer, size_t a_uiIndex, void* a_pDst)
    {
        getItemType()->copyAssign(a_pDst, referenceAt(a_pContainer, a_uiIndex));
    }

    virtual void const* data(void const* a_pContainer) const;
    virtual void*       data(void* a_pContainer) const;
    virtual size_t      size(void const* a_pContainer) const;
    virtual const void* referenceAt(void const* a_pContainer, size_t a_uiIndex) const;
    virtual void*       referenceAt(void* a_pContainer, size_t a_uiIndex) const;
    virtual void        begin(void* a_pContainer, void* a_pOutIt) const;
    virtual void        begin(void const* a_pContainer, void* a_pOutIt) const;
    virtual void        end(void* a_pContainer, void* a_pOutIt) const;
    virtual void        end(void const* a_pContainer, void* a_pOutIt) const;
    virtual void*       dereferenceIterator(void* a_pIt) const;
    virtual void        advanceIterator(void* a_pIt, size_t a_N = 1) const;
    virtual bool        compareIterators(void* a_pIt1, void* a_pIt2) const;
    virtual Type*       getIteratorType() const;
    virtual Type*       getConstIteratorType() const;

protected:
    ArrayClass(TypeKind a_TypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    ArrayClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);
    ArrayClass(TypeKind a_TypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
               uint a_uiFlags);
    void setItemType(Type* a_pElemType)
    {
        PHANTOM_ASSERT(m_pItemType == nullptr);
        m_pItemType = a_pElemType;
    }
    void setItemCount(size_t a_C)
    {
        PHANTOM_ASSERT(m_ItemCount == 0);
        PHANTOM_ASSERT(a_C != 0);
        m_ItemCount = a_C;
    }

private:
    void _initBegin() const;
    void _initBeginC() const;

private:
    mutable RTData* m_pData = nullptr;
    mutable Type*   m_pItemType = nullptr;
    size_t          m_ItemCount{};
};

} // namespace lang
} // namespace phantom
