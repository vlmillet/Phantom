// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/typedefs.h>

namespace phantom
{
template<class Map, class Iterator, class Derived>
class MapAdaptorIteratorBase
{
    using SelfType = MapAdaptorIteratorBase<Map, Iterator, Derived>;

protected:
    MapAdaptorIteratorBase(Iterator _it) : m_it(_it)
    {
    }

public:
    Derived& operator++()
    {
        ++m_it;
        return static_cast<Derived&>(*this);
    }
    Derived operator++(int)
    {
        Derived _this = static_cast<Derived&>(*this);
        m_it++;
        return _this;
    }

    bool operator==(SelfType const& _other) const
    {
        return m_it == _other.m_it;
    }

    bool operator!=(SelfType const& _other) const
    {
        return m_it != _other.m_it;
    }

protected:
    Iterator m_it;
};

template<class Map, class Iterator>
class MapKeysAdaptorIterator
    : public MapAdaptorIteratorBase<Map, Iterator, MapKeysAdaptorIterator<Map, Iterator>>
{
public:
    MapKeysAdaptorIterator(Iterator _it)
        : MapAdaptorIteratorBase<Map, Iterator, MapKeysAdaptorIterator<Map, Iterator>>(_it)
    {
    }
    auto operator*() const
    {
        return this->m_it->first;
    }
    auto operator-> () const
    {
        return this->m_it->first;
    }
};

template<class Map, class Iterator>
class MapValuesAdaptorIterator
    : public MapAdaptorIteratorBase<Map, Iterator, MapValuesAdaptorIterator<Map, Iterator>>
{
public:
    MapValuesAdaptorIterator(Iterator _it)
        : MapAdaptorIteratorBase<Map, Iterator, MapValuesAdaptorIterator<Map, Iterator>>(_it)
    {
    }
    auto operator*() const
    {
        return this->m_it->second;
    }
    auto operator-> () const
    {
        return this->m_it->second;
    }
};

template<class Map>
class ConstMapKeysAdaptor
{
public:
    using ConstIterator = MapKeysAdaptorIterator<Map, typename Map::const_iterator>;
    ConstMapKeysAdaptor(Map const& _map) : m_map(&_map)
    {
    }

    ConstIterator begin() const
    {
        return m_map->begin();
    }
    ConstIterator end() const
    {
        return m_map->end();
    }

    size_t size() const
    {
        return m_map->size();
    }
    bool empty() const
    {
        return m_map->empty();
    }

private:
    Map const* m_map;
};

/// @on
template<class Map>
class MapKeysAdaptor
{
public:
    using Iterator = MapKeysAdaptorIterator<Map, typename Map::iterator>;
    using ConstIterator = MapKeysAdaptorIterator<Map, typename Map::const_iterator>;
    MapKeysAdaptor(Map& _map) : m_map(&_map)
    {
    }

    ConstIterator begin() const
    {
        return m_map->begin();
    }
    ConstIterator end() const
    {
        return m_map->end();
    }

    Iterator begin()
    {
        return m_map->begin();
    }
    Iterator end()
    {
        return m_map->end();
    }

    size_t size() const
    {
        return m_map->size();
    }
    bool empty() const
    {
        return m_map->empty();
    }

private:
    Map* m_map;
};

/// @on
template<class Map>
class ConstMapValuesAdaptor
{
public:
    using ConstIterator = MapValuesAdaptorIterator<Map, typename Map::const_iterator>;
    ConstMapValuesAdaptor(Map const& _map) : m_map(&_map)
    {
    }

    ConstIterator begin() const
    {
        return m_map->begin();
    }
    ConstIterator end() const
    {
        return m_map->end();
    }

    size_t size() const
    {
        return m_map->size();
    }
    bool empty() const
    {
        return m_map->empty();
    }

private:
    Map const* m_map;
};

/// @on
template<class Map>
class MapValuesAdaptor
{
public:
    using Iterator = MapValuesAdaptorIterator<Map, typename Map::iterator>;
    using ConstIterator = MapValuesAdaptorIterator<Map, typename Map::const_iterator>;
    MapValuesAdaptor(Map& _map) : m_map(&_map)
    {
    }

    ConstIterator begin() const
    {
        return m_map->begin();
    }
    ConstIterator end() const
    {
        return m_map->end();
    }

    Iterator begin()
    {
        return m_map->begin();
    }
    Iterator end()
    {
        return m_map->end();
    }

    size_t size() const
    {
        return m_map->size();
    }
    bool empty() const
    {
        return m_map->empty();
    }

private:
    Map* m_map;
};

/// @on
template<class Map>
MapKeysAdaptor<Map> MapKeys(Map& _map)
{
    return MapKeysAdaptor<Map>(_map);
}
/// @on
template<class Map>
ConstMapKeysAdaptor<Map> MapKeys(Map const& _map)
{
    return ConstMapKeysAdaptor<Map>(_map);
}

/// @on
template<class Map>
MapValuesAdaptor<Map> MapValues(Map& _map)
{
    return MapValuesAdaptor<Map>(_map);
}
/// @on
template<class Map>
ConstMapValuesAdaptor<Map> MapValues(Map const& _map)
{
    return ConstMapValuesAdaptor<Map>(_map);
}

} // namespace phantom
