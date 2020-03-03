// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/lang/Type.h>

namespace phantom
{
namespace Generic
{
struct Marker
{
};
struct Auto : Marker
{
};

template<class Decoration = Auto>
struct Type
{
    Type() = default;
    Type(lang::Type* a_pType) : m_pType(a_pType)
    {
    }
    operator lang::Type*() const
    {
        return m_pType;
    }

    lang::Type* getType() const
    {
        return m_pType;
    }
    lang::Class* getClass() const
    {
        return m_pType ? m_pType->asClass() : nullptr;
    }

private:
    lang::Type* m_pType = nullptr;
};

template<class T = Auto>
struct Param : Type<T>
{
    Param() = default;
    Param(lang::Type* a_pType) : Type<T>(a_pType)
    {
    }
};

template<class TypeRef>
struct ConstructorParams
{
    ConstructorParams() = default;
    ConstructorParams(void** a_pArgs) : m_pArgs(a_pArgs)
    {
    }
    operator void**() const
    {
        return m_pArgs;
    }

private:
    void** m_pArgs = nullptr;
};

template<class TypePattern, class ReturnType>
struct Return : Marker
{
    Return() = default;
    Return(ReturnType a_Return) : m_Return(a_Return)
    {
    }
    operator ReturnType() const
    {
        return m_Return;
    }

private:
    ReturnType m_Return;
};

template<class TypePattern, class ArgType>
struct Arg
{
    Arg() = default;
    Arg(ArgType a_Arg) : m_Arg(a_Arg)
    {
    }
    operator ArgType() const
    {
        return m_Arg;
    }

    ArgType getValue() const
    {
        return m_Arg;
    }

private:
    ArgType m_Arg;
};

template<size_t ParamIndex>
struct ParamType : Marker
{
};

};    // namespace Generic
#if 0 // example
    Return<ParamType<0>*, void*> New(Param<> a_Return, ConstructorParams<ParamType<0>> a_Args); // is the generic version of T* New(Args...)
#endif
} // namespace phantom
