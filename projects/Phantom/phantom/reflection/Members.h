// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/newImpl.h>
#include <phantom/reflection/LanguageElement.h>
/* *********************************************** */

HAUNT_STOP;

namespace phantom
{
namespace reflection
{
class MembersBase
{
protected:
    MembersBase(LanguageElement* a_pOwner) : m_pOwner(a_pOwner)
    {
    }

    inline void onAccess() const;

private:
    HAUNT_OFF mutable LanguageElement* m_pOwner;
};

template<typename t_Container>
class Members : public MembersBase
{
public:
    Members(LanguageElement* a_pOwner) : MembersBase(a_pOwner), m_data(nullptr)
    {
    }

    static t_Container* Empty()
    {
        static t_Container empty;
        return &empty;
    }

    ~Members()
    {
        phantom::Delete(m_data);
    }

    t_Container* operator->()
    {
        if (m_data == nullptr)
        {
            m_data = phantom::New<t_Container>();
            onAccess();
        }
        return m_data;
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

    void push_back(PHANTOM_TYPENAME t_Container::value_type const& a_Value)
    {
        if (m_data == nullptr)
        {
            m_data = phantom::New<t_Container>();
        }
        m_data->push_back(a_Value);
    }

    void insert(PHANTOM_TYPENAME t_Container::iterator const& a_It,
                PHANTOM_TYPENAME t_Container::value_type const& a_Value)
    {
        if (m_data == nullptr)
        {
            m_data = phantom::New<t_Container>();
        }
        m_data->insert(a_It, a_Value);
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
} // namespace reflection
} // namespace phantom
