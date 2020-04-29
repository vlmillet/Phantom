// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \Brief For conversion and overload resolution purpose only, it is not a real type !

class PHANTOM_EXPORT_PHANTOM InitializerListType : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(InitializerListType);

    friend class Source;

private:
    InitializerListType(TypesView a_Types);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the types of the initializers expressions.
    ///
    /// \return return the types of the initializers expressions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TypesView getTypes() const { return m_Types; }

    bool matches(TypesView a_Types) const;

    InitializerListType* asInitializerListType() const override { return const_cast<InitializerListType*>(this); }

    void getUniqueName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

protected:
    Types m_Types;
};

} // namespace lang
} // namespace phantom
