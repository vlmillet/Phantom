// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ContainerClass.h>
#include <phantom/traits/MoveArg.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base base for sequential containers class representation (vector, list, ...).
class PHANTOM_EXPORT_PHANTOM SequentialContainerClass : public ContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(SequentialContainerClass);

    struct RTData
    {
        Method* m_pFunc_push_back = nullptr;
        Method* m_pFunc_push_back_move = nullptr;
        Method* m_pFunc_pop_back = nullptr;
    };

protected:
    SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                             Modifiers a_Modifiers, uint a_uiFlags);

    SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    ~SequentialContainerClass() override;

    SequentialContainerClass* asSequentialContainerClass() const override
    {
        return const_cast<SequentialContainerClass*>(this);
    }

    virtual void push_back(void* a_pContainer, void const* a_pValue) const;
    virtual void push_back(void* a_pContainer, MoveArg a_pValue) const;
    virtual void pop_back(void* a_pContainer) const;

private:
    mutable RTData* m_Data = nullptr;
};

} // namespace lang
} // namespace phantom
