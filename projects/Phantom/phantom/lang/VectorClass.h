// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
class PHANTOM_EXPORT_PHANTOM VectorClass : public SequentialContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(VectorClass);

    struct RTData
    {
        Method* m_pFunc_data = nullptr;
        Method* m_pFunc_datac = nullptr;
        Method* m_pFunc_resize = nullptr;
        Method* m_pFunc_refAt = nullptr;
        Method* m_pFunc_refAtC = nullptr;
    };

public:
    VectorClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ~VectorClass() override;

    virtual void const* data(void const* a_pContainer) const;
    virtual void*       data(void* a_pContainer) const;
    virtual void        resize(void* a_pContainer, size_t a_Size) const;
    const void*         referenceAt(void const* a_pContainer, size_t a_uiIndex) const override;
    void*               referenceAt(void* a_pContainer, size_t a_uiIndex) const override;

protected:
    VectorClass(TypeKind a_TypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    VectorClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);
    VectorClass(TypeKind a_TypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                uint a_uiFlags);

private:
    mutable RTData* m_pData = nullptr;
};

} // namespace lang
} // namespace phantom
