// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
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

    TypesView getTypes() const
    {
        return m_Types;
    }

    bool matches(TypesView a_Types) const;

    InitializerListType* asInitializerListType() const override
    {
        return const_cast<InitializerListType*>(this);
    }

    void getUniqueName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

protected:
    void     onReferencedElementRemoved(LanguageElement* a_pElement) override;
    Pointer* createPointer() const override
    {
        return nullptr;
    }
    LValueReference* createLValueReference() const override
    {
        return nullptr;
    }
    RValueReference* createRValueReference() const override
    {
        return nullptr;
    }
    ConstVolatileType* createConstVolatileType() const override
    {
        return nullptr;
    }
    VolatileType* createVolatileType() const override
    {
        return nullptr;
    }

protected:
    Types m_Types;
};

} // namespace reflection
} // namespace phantom
