// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

// Inpired from Qt::Flags

#include <haunt>
#include <phantom/detail/core.h>

namespace phantom
{
class Flag
{
    int i;

public:
    inline Flag(int i);
    inline operator int() const
    {
        return i;
    }
};

inline Flag::Flag(int ai) : i(ai)
{
}

class IncompatibleFlag
{
    int i;

public:
    inline explicit IncompatibleFlag(int i);
    inline operator int() const
    {
        return i;
    }
};

inline IncompatibleFlag::IncompatibleFlag(int ai) : i(ai)
{
}

template<typename Enum>
class Flags;

template<typename Enum>
struct FlagsBit
{
    FlagsBit() : m_Flags(0), m_Where((Enum)0)
    {
    }
    FlagsBit(Flags<Enum>* a_Flags, Enum a_Where) : m_Flags(a_Flags), m_Where(a_Where)
    {
    }

    void setValue(bool a_bValue)
    {
        if (a_bValue)
            (*m_Flags) |= m_Where;
        else
            (*m_Flags) &= ~m_Where;
    }
    bool getValue() const
    {
        return ((*m_Flags) & m_Where) == m_Where;
    }

    HAUNT_PROPERTY(bool, value, getValue, setValue);

protected:
    Flags<Enum>* m_Flags;
    Enum         m_Where;
};

template<typename Enum>
struct ConstFlagsBit
{
    ConstFlagsBit() : m_Flags(0), m_Where((Enum)0)
    {
    }
    ConstFlagsBit(const Flags<Enum>* a_Flags, Enum a_Where) : m_Flags(a_Flags), m_Where(a_Where)
    {
    }

    bool getValue() const
    {
        return ((*m_Flags) & m_Where) == m_Where;
    }

protected:
    const Flags<Enum>* m_Flags;
    Enum               m_Where;
};

template<typename Enum>
class Flags
{
    int i;

public:
    using _Zero = void****;
    typedef Flags<Enum>         SelfType;
    typedef FlagsBit<Enum>      Bit;
    typedef ConstFlagsBit<Enum> ConstBit;
    typedef Enum                EnumType;
    inline Flags(const SelfType& f) : i(f.i)
    {
    }
    inline Flags(Enum f) : i(f)
    {
    }
    inline Flags(_Zero = 0) : i(0) // to allow 0 init but not any int init
    {
    }
    inline Flags(Flag f) : i(f)
    {
    }

    inline SelfType& operator=(const SelfType& f)
    {
        i = f.i;
        return *this;
    }
    inline SelfType& operator&=(int mask)
    {
        i &= mask;
        return *this;
    }
    inline SelfType& operator&=(unsigned int mask)
    {
        i &= mask;
        return *this;
    }
    inline SelfType& operator|=(SelfType f)
    {
        i |= f.i;
        return *this;
    }
    inline SelfType& operator|=(Enum f)
    {
        i |= f;
        return *this;
    }
    inline SelfType& operator^=(SelfType f)
    {
        i ^= f.i;
        return *this;
    }
    inline SelfType& operator^=(Enum f)
    {
        i ^= f;
        return *this;
    }

    inline operator int() const
    {
        return i;
    }

    inline SelfType operator|(SelfType f) const
    {
        return SelfType(Enum(i | f.i));
    }
    inline SelfType operator|(Enum f) const
    {
        return SelfType(Enum(i | f));
    }
    inline SelfType operator^(SelfType f) const
    {
        return SelfType(Enum(i ^ f.i));
    }
    inline SelfType operator^(Enum f) const
    {
        return SelfType(Enum(i ^ f));
    }
    inline SelfType operator&(int mask) const
    {
        return SelfType(Enum(i & mask));
    }
    inline SelfType operator&(unsigned int mask) const
    {
        return SelfType(Enum(i & mask));
    }
    inline SelfType operator&(Enum f) const
    {
        return SelfType(Enum(i & f));
    }
    inline SelfType operator~() const
    {
        return SelfType(Enum(~i));
    }

    inline bool operator!() const
    {
        return !i;
    }

    inline bool testFlag(Enum f) const
    {
        return (i & f) == f && (f != 0 || i == int(f));
    }

    Bit operator[](Enum f)
    {
        return Bit(this, f);
    }
    ConstBit operator[](Enum f) const
    {
        return ConstBit(this, f);
    }
};

} // namespace phantom

#define PHANTOM_DECLARE_FLAGS(FlagsName, _enum) typedef ::phantom::Flags<_enum> FlagsName;

#define PHANTOM_DECLARE_INCOMPATIBLE_FLAGS(flags_)                                                                     \
    inline ::phantom::IncompatibleFlag operator|(flags_::EnumType f1, int f2)                                          \
    {                                                                                                                  \
        return ::phantom::IncompatibleFlag(int(f1) | f2);                                                              \
    }

#define PHANTOM_DECLARE_OPERATORS_FOR_FLAGS(flags_)                                                                    \
    inline ::phantom::Flags<flags_::EnumType> operator|(flags_::EnumType f1, flags_::EnumType f2)                      \
    {                                                                                                                  \
        return ::phantom::Flags<flags_::EnumType>(f1) | f2;                                                            \
    }                                                                                                                  \
    inline ::phantom::Flags<flags_::EnumType> operator|(flags_::EnumType f1, ::phantom::Flags<flags_::EnumType> f2)    \
    {                                                                                                                  \
        return f2 | f1;                                                                                                \
    }                                                                                                                  \
    PHANTOM_DECLARE_INCOMPATIBLE_FLAGS(flags_)
