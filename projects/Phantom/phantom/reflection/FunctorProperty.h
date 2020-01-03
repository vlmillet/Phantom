// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ValueMember.h>
#include <phantom/utils/Functor.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Represents a get/set property using functors.
class PHANTOM_EXPORT_PHANTOM FunctorProperty : public ValueMember
{
public:
    using SetFunctor = phantom::Functor<void(void*, void const*)>;
    using GetFunctor = phantom::Functor<void(void const*, void*)>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a functor property based on two get/set functors.
    ///
    /// \param  a_pValueType    The functor value type.
    /// \param  a_strName       The functor property name.
    /// \param  a_uiFilterMask  (optional) The filter mask.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctorProperty(Type* a_pType, StringView a_Name, uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);
    FunctorProperty(Type* a_pType, StringView a_Name, GetFunctor const& a_Get, SetFunctor const& a_Set,
                    uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);
    FunctorProperty(Type* a_pType, StringView a_Name, GetFunctor const& a_Get,
                    uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'set' functor.
    ///
    /// \return The 'set' functor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SetFunctor const& getSet() const
    {
        return m_Set;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'get' functor.
    ///
    /// \return The 'get' functor
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    GetFunctor const& getGet() const
    {
        return m_Get;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the 'set' functor.
    ///
    /// \return The 'set' functor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setSet(SetFunctor const& a_Set)
    {
        m_Set = a_Set;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the 'get' functor.
    ///
    /// \return The 'get' functor
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGet(GetFunctor const& a_Get)
    {
        m_Get = a_Get;
    }

    void getValue(void const* a_pObject, void* a_pDest) const override;
    void setValue(void* a_pObject, void const* a_pSrc) const override;
    bool isWrittable() const override
    {
        return m_Set;
    }
    bool isReadable() const override
    {
        return m_Get;
    }

private:
    SetFunctor m_Set;
    GetFunctor m_Get;
};

} // namespace reflection
} // namespace phantom
