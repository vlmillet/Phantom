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
    Members(LanguageElement* a_pOwner) : MembersBase(a_pOwner), m_data(nullptr) {}

    static t_Container* Empty()
    {
        static t_Container empty;
        return &empty;
    }

    ~Members() { phantom::Delete<t_Container>(m_data); }

    t_Container* operator->()
    {
        auto c = container();
        onAccess();
        return c;
    }

    t_Container* operator->() const
    {
        onAccess();
        return m_data ? m_data : Empty();
    }

    const t_Container& operator*() const
    {
        onAccess();
        return m_data ? *m_data : *Empty();
    }

    t_Container& operator*()
    {
        onAccess();
        return m_data ? *m_data : *Empty();
    }

    PHANTOM_TYPENAME t_Container::value_type const& operator[](size_t i) const
    {
        onAccess();
        PHANTOM_ASSERT(m_data);
        return (*m_data)[i];
    }

    size_t size() const
    {
        onAccess();
        return m_data ? m_data->size() : 0;
    }

    void push_back(PHANTOM_TYPENAME t_Container::value_type const& a_Value) { container()->push_back(a_Value); }

    void insert(PHANTOM_TYPENAME t_Container::iterator const& a_It,
                PHANTOM_TYPENAME t_Container::value_type const& a_Value)
    {
        container()->insert(a_It, a_Value);
    }

    PHANTOM_TYPENAME t_Container::const_iterator begin() const
    {
        onAccess();
        return container()->begin();
    }

    PHANTOM_TYPENAME t_Container::const_iterator end() const
    {
        onAccess();
        return container()->end();
    }

    t_Container* container() const
    {
        if (m_data == nullptr)
        {
            m_data = phantom::New<t_Container>();
        }
        return m_data;
    }

    t_Container* m_data;
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
