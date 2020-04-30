// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/LanguageElement.h>
/* *********************************************** */

HAUNT_STOP;

namespace phantom
{
namespace lang
{
class MembersBase
{
protected:
    MembersBase(LanguageElement* a_pOwner) : m_pOwner(a_pOwner) {}

    inline void onAccess() const;

private:
    HAUNT_OFF mutable LanguageElement* m_pOwner;
};

template<typename t_Container>
class Members : public MembersBase
{
public:
    Members(LanguageElement* a_pOwner) : MembersBase(a_pOwner) {}

    ~Members() {}

    void setAllocator(CustomAllocator const* a_pAlloc) { m_container.setAllocator(a_pAlloc); }

    t_Container* operator->()
    {
        onAccess();
        return &m_container;
    }

    t_Container const* operator->() const
    {
        onAccess();
        return &m_container;
    }

    const t_Container& operator*() const
    {
        onAccess();
        return m_container;
    }

    t_Container& operator*()
    {
        onAccess();
        return m_container;
    }

    PHANTOM_TYPENAME t_Container::value_type const& operator[](size_t i) const
    {
        onAccess();
        return m_container[i];
    }

    size_t size() const
    {
        onAccess();
        return m_container.size();
    }

    bool empty() const
    {
        onAccess();
        return m_container.empty();
    }

    void push_back(PHANTOM_TYPENAME t_Container::value_type const& a_Value) { m_container.push_back(a_Value); }

    void insert(PHANTOM_TYPENAME t_Container::iterator const& a_It,
                PHANTOM_TYPENAME t_Container::value_type const& a_Value)
    {
        m_container.insert(a_It, a_Value);
    }

    PHANTOM_TYPENAME t_Container::const_iterator begin() const
    {
        onAccess();
        return m_container.begin();
    }

    PHANTOM_TYPENAME t_Container::const_iterator end() const
    {
        onAccess();
        return m_container.end();
    }

    t_Container& container() { return m_container; }

    t_Container m_container;
};

inline void MembersBase::onAccess() const
{
    if (m_pOwner)
    {
        auto pOwner = m_pOwner;
        m_pOwner = nullptr;
        pOwner->_onElementsAccess();
    }
}
} // namespace lang
} // namespace phantom
