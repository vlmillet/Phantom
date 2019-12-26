// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ContainerClass.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Base base for sequential containers class representation (vectors, dynamic arrays, ...).
class PHANTOM_EXPORT_PHANTOM SequentialContainerClass : public ContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(SequentialContainerClass);

    struct RTData
    {
        Method* m_pFunc_push_back = nullptr;
    };

protected:
    SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize,
                             size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);

    SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers = 0,
                             uint a_uiFlags = 0);

public:
    ~SequentialContainerClass() override;

    SequentialContainerClass* asSequentialContainerClass() const override
    {
        return const_cast<SequentialContainerClass*>(this);
    }

    virtual void push_back(void* a_pContainer, void const* a_pValue) const;

private:
    mutable RTData* m_Data = nullptr;
};

} // namespace reflection
} // namespace phantom
