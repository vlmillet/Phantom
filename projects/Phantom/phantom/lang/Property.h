// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ValueMember.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a get/set property.
class PHANTOM_EXPORT_PHANTOM Property : public ValueMember
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class ClassType;

public:
    PHANTOM_DECLARE_META_CLASS(Property);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a property.
    ///
    /// \param  a_pValueType            The property value type.
    /// \param  a_strName               The property name.
    /// \param  a_uiFilterMask   (optional) The database mask.
    /// \param  a_GetName               The get function name (if not specified, takes property name
    /// value). \param  a_SetName               The set function name. \param  a_SignalName The
    /// signal function name. \param  a_Modifiers             (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property(Type* a_pFunctionsType, StringView a_strName, uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY,
             Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property(Type* a_pFunctionsType, StringView a_strName, uint a_uiFilterMask, Modifiers a_Modifiers, uint a_uiFlags,
             int nativeTag);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the property value of the given object.
    ///
    /// \param  a_pObject       The object.
    /// \param [in,out] a_pDest The output buffer address receiving the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getValue(void const* a_pObject, void* a_pDest) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the property value of the given object.
    ///
    /// \param  a_pObject   The object.
    /// \param  a_pSrc      The input buffer address pointing to the source value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setValue(void* a_pObject, void const* a_pSrc) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value type.
    ///
    /// \return The value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getFunctionsType() const { return m_pFunctionsType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the effective value type (without const and ref qualifiers).
    ///
    /// \return The effective value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* asProperty() const override { return const_cast<Property*>(this); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'set' member function.
    ///
    /// \return The 'set' member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getSet() const { return m_pSet; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add the 'set' member function with an optional custom name.
    ///
    /// \param  a_strName   (optional) the name (if not specified, phantom will generated a name of
    ///                     the form &lt;property-name&gt;$set.
    ///
    /// \return The 'set' member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* addSet(StringView a_strName = "");

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add the 'get' member function with an optional custom name.
    ///
    /// \param  a_strName   (optional) the name (if not specified, phantom will generated a name of
    ///                     the form &lt;property-name&gt;$get.
    ///
    /// \return The 'get' member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* addGet(StringView a_strName = "");

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'get' member function.
    ///
    /// \return The 'get' member function
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getGet() const { return m_pGet; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the change notification signal function.
    ///
    /// \return The change notification signal function
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    Signal* getSignal() const { return m_pSignal; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the 'set' method.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setSet(Method* a_pFunc);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the 'get' method.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGet(Method* a_pFunc);

    bool isWrittable() const override { return m_pSet != nullptr; }
    bool isReadable() const override { return m_pGet != nullptr; }

    void              setTouchedFieldNames(StringViews _touchedFields);
    void              setTouchedFieldNames(ArrayView<String> _touchedFields);
    ArrayView<String> getTouchedFieldNames() const { return m_TouchedFieldNames; }

private:
    virtual void onAttachedToClass(ClassType*) {}

private:
    Type*   m_pFunctionsType = nullptr;
    Signal* m_pSignal = nullptr;
    Method* m_pSet = nullptr;
    Method* m_pGet = nullptr;
    Strings m_TouchedFieldNames;
};

} // namespace lang
} // namespace phantom
