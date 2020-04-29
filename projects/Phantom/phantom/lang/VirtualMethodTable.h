// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a virtual member function table (for polymorphc OOP languages such as C++, C#
/// or Java).
class PHANTOM_EXPORT_PHANTOM VirtualMethodTable : public Symbol
{
    PHANTOM_DECLARE_META_CLASS(VirtualMethodTable);

    friend class Class;

public:
    /// \brief  Default constructor. Creates an empty table
    VirtualMethodTable();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a virtual member function table with given size.
    ///
    /// \param  a_uiSize    The size.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTable(size_t a_uiSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a virtual member function table with given native closures and size.
    ///
    /// \param  a_ppClosures    The native closures.
    /// \param  a_uiSize        The size.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTable(void** a_ppClosures, size_t a_uiSize);
    PHANTOM_DTOR ~VirtualMethodTable() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function count.
    ///
    /// \return The member function count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getMethodCount() const { return m_pMethods->size(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get index of the given member function.
    ///
    /// \param [in,out] a_pMethod   The member function to find the index of.
    ///
    /// \return The index of the given member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getIndexOf(Method* a_pMethod) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function at given position index.
    ///
    /// \param  a_uiIndex   The index.
    ///
    /// \return The member function at given position index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethod(size_t a_uiIndex) const { return (*m_pMethods)[a_uiIndex]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner class of this table.
    ///
    /// \return The owner class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getOwnerClass() const { return getOwner() ? getOwner()->asClass() : nullptr; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the original class of this table (if table sharing base class table).
    ///
    /// \return The original class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getOriginalClass() const
    {
        return m_pBaseTable == nullptr ? getOwnerClass() : m_pBaseTable->getOriginalClass();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset where is set the table pointer in an instance.
    ///
    /// \return The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getOffset() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Derives this virtual member function table with optional PHANTOM_NEW(size)
    ///
    /// \param  a_uiSize    (optional) the PHANTOM_NEW(size).
    ///
    /// \return null if it fails, else the derived virtual member function table.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTable* derive(size_t a_uiSize = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an instance, installing the vtable pointer.
    ///
    /// \param [in,out] a_pInstance The instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void construct(void* a_pInstance);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the root table of this table.
    ///
    /// \return null if it fails, else the root table.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTable* getRootTable() const
    {
        return m_pBaseTable ? m_pBaseTable->getRootTable() : const_cast<VirtualMethodTable*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets root member function (the first overridden one).
    ///
    /// \param  a_uiIndex   The index of the source member function in this table.
    ///
    /// \return The root member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getRootMethod(size_t a_uiIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets root member function (the first overridden one).
    ///
    /// \param [in,out] a_pMethod   The source member function.
    ///
    /// \return The root member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getRootMethod(Method* a_pMethod) const;

    VirtualMethodTable* asVirtualMethodTable() const override { return const_cast<VirtualMethodTable*>(this); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieve the index of this member function in this vtable.
    ///
    /// \param [in,out] a_pMethod   The required member function.
    ///
    /// \return The required member function index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getMethodIndex(Method* a_pMethod) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieve the index of this member function in this vtable.
    ///
    /// \param [in,out] a_pMethod   The required member function.
    ///
    /// \return The required member function index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void** getClosurePointers() const { return m_ppClosures; }

    bool insertMethod(Method* a_pMethod, bool a_bOnlyIfOverrides);

    void extractNativeVTable(void* a_pInstance);

private: // Derivation constructors
    VirtualMethodTable(VirtualMethodTable* a_pBaseTable);
    VirtualMethodTable(VirtualMethodTable* a_pBaseTable, size_t a_uiSize);

    void addMethod(Method* a_pMethod);
    void setMethod(size_t a_uiIndex, Method* a_pMethod);
    bool sharesMethods() const
    {
        return m_bShared; /* m_pBaseTable && m_pBaseTable->m_pMethods == m_pMethods*/
        ;
    }
    void copyOnWrite();
    bool canBeDestroyed() const override;
    void _construct(void* a_pInstance, SmallMap<void**, size_t, 16>& a_VTableSizeAtAddress);

private:
    Methods*            m_pMethods{};
    VirtualMethodTable* m_pBaseTable{};
    VirtualMethodTables m_DerivedTables;
    void*               m_pNativeRttiData{};
    void**              m_ppClosures{};
    bool                m_bShared{};
};

} // namespace lang
} // namespace phantom
