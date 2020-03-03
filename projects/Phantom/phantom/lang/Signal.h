// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Method.h>
/* ************* Class Declarations ************** */

/* *********************************************** */
namespace phantom
{
namespace connection
{
struct PIMPL;
}
namespace lang
{
/// \brief  Represents a signal function.
class PHANTOM_EXPORT_PHANTOM Signal : public Method
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Signal);

    friend class phantom::lang::Class;
    friend class Property;

public:
    Signal() = default;
    Signal(StringView a_strName, Types a_Types, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Signal(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the slot list associated with this signal inside the given object.
    ///
    /// \param  a_pObject The object.
    ///
    /// \return null if it fails, else the slot list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getInstance(void* a_pThis) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Blocks this signal for the given object.
    ///
    /// \param  a_pObject   The object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void block(void*
#if PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING
                      a_pObject
#endif
    )
    {
#if PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING
        getInstance(a_pObject)->block();
#endif
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Unblocks this signal for the given object.
    ///
    /// \param  a_pObject   The object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void unblock(void*
#if PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING
                        a_pObject
#endif
    )
    {
#if PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING
        getInstance(a_pObject)->unblock();
#endif
    }

    Signal* asSignal() const override
    {
        return (Signal*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the signal field offset inside the owner class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getFieldOffset() const
    {
        return m_FieldOffset;
    }

protected:
    /// \internal
    Signal(StringView a_strName, Signature* a_pSignature, size_t a_FieldOffset, Modifiers a_Modifiers, uint a_uiFlags);
    Signal(ClassType* a_pClassType, size_t a_uiFieldOffset, StringView a_strName, StringView a_strSignature,
           Modifiers a_Modifiers, uint a_uiFlags = 0);
    /// \internal
    void onAncestorChanged(LanguageElement* a_pLanguageElement) override;

protected:
    size_t m_FieldOffset = ~size_t(0);
};

} // namespace lang
} // namespace phantom
