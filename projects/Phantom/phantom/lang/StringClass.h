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
/// \brief  Base base for stl compliant String classes.
class PHANTOM_EXPORT_PHANTOM StringClass : public SequentialContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(StringClass);

    struct RTData
    {
        Method* m_pFunc_data = nullptr;
        Method* m_pFunc_datac = nullptr;
    };

protected:
    StringClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);

public:
    StringClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize();

    void terminate();

    virtual void const* c_str(void const* a_pString) const = 0;

    virtual void assign(void* a_pString, const void* a_pChars, size_t a_Len) const = 0;
    virtual void append(void* a_pString, const void* a_pChars) const = 0;

    virtual void const* data(void const* a_pContainer) const;
    virtual void*       data(void* a_pContainer) const;

private:
    RTData* m_pData;
};

} // namespace lang
} // namespace phantom
