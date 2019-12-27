// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Delegate.h"
#include "DynDelegate.h"
#include "SmallVector.h"

#include <functional>
#include <phantom/typeof>
#include <type_traits>
/* *********************************************** */

namespace phantom
{
template<class S>
class Functor
#if defined(__HAUNT__)
{
}
#endif
;

template<class T>
struct IsFunctor : std::false_type
{
};

template<class R, class... Params>
struct IsFunctor<Functor<R(Params...)>> : std::true_type
{
};

enum class _FunctorInternalType
{
    Delegate,
    DynDelegate,
    StdFunction
};

template<class R, class... Params>
class Functor<R(Params...)>
{
    using _Type = _FunctorInternalType;

public:
    using ThisType = Functor<R(Params...)>;
    using DelegateType = Delegate<R(Params...)>;
    using StdFuncType = std::function<R(Params...)>;
    using DynDelegateType = DynDelegate<R(Params...)>;
    template<class T>
    using EnableIfCustomArg = std::enable_if_t<!std::is_lvalue_reference<T>::value ||
                                               std::is_const<std::remove_reference_t<T>>::value // & => const&
                                               >;
    // Delegate

    /// @brief default constructor (holds a Delegate by default)
    Functor() : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType();
    }

    /// @brief copy constructor
    Functor(ThisType const& a_Other) : m_Type(a_Other.m_Type)
    {
        switch (m_Type)
        {
        case _Type::Delegate:
            new (&m_Delegate) DelegateType(a_Other._delegate());
            break;
        case _Type::StdFunction:
        {
            new (&m_StdFunction) StdFuncType(a_Other._stdFunction());
            m_pLambdaDiscriminant = a_Other.m_pLambdaDiscriminant;
            break;
        }
        default:
            new (&m_DynDelegate) DynDelegateType(a_Other._dynDelegate());
        }
    }

    /// @brief move constructor
    Functor(ThisType&& a_Temp) : m_Type(a_Temp.m_Type)
    {
        switch (m_Type)
        {
        case _Type::Delegate:
            new (&m_Delegate) DelegateType(std::move(a_Temp._delegate()));
            break;
        case _Type::StdFunction:
        {
            new (&m_StdFunction) StdFuncType(std::move(a_Temp._stdFunction()));
            m_pLambdaDiscriminant = a_Temp.m_pLambdaDiscriminant;
            break;
        }
        default:
            new (&m_DynDelegate) DynDelegateType(std::move(a_Temp._dynDelegate()));
        }
    }

    /// @brief copy assignment operator
    ThisType& operator=(ThisType const& a_Other)
    {
        if (m_Type == a_Other.m_Type)
        {
            switch (m_Type)
            {
            case _Type::Delegate:
                _delegate() = a_Other._delegate();
                break;
            case _Type::StdFunction:
            {
                _stdFunction() = a_Other._stdFunction();
                m_pLambdaDiscriminant = a_Other.m_pLambdaDiscriminant;
                break;
            }
            default:
                _dynDelegate() = a_Other._dynDelegate();
            }
        }
        else
        {
            _destroy();
            new (this) ThisType(a_Other);
        }
        return *this;
    }

    /// @brief move assignment operator
    ThisType& operator=(ThisType&& a_Temp)
    {
        if (m_Type == a_Temp.m_Type)
        {
            switch (m_Type)
            {
            case _Type::Delegate:
                _delegate() = std::move(a_Temp._delegate());
                break;
            case _Type::StdFunction:
            {
                _stdFunction() = std::move(a_Temp._stdFunction());
                m_pLambdaDiscriminant = a_Temp.m_pLambdaDiscriminant;
                break;
            }
            default:
                _dynDelegate() = std::move(a_Temp._dynDelegate());
            }
        }
        else
        {
            _destroy();
            new (this) ThisType(std::move(a_Temp));
        }
        return *this;
    }

    /// @brief destructor
    ~Functor()
    {
        _destroy();
    }

    Functor(DelegateType const& a_In) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(a_In);
    }

    Functor(DelegateType&& a_In) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(std::move(a_In));
    }

    Functor(R (*a_Func)(Params...)) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(a_Func);
    }

    template<class T>
    Functor(T* a_pThis, R (T::*a_MemberFunc)(Params...)) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(a_pThis, a_MemberFunc);
    }

    template<class T>
    Functor(T const* a_pThis, R (T::*a_MemberFunc)(Params...) const) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(a_pThis, a_MemberFunc);
    }

    template<class T>
    Functor(T* a_pThis, R (*a_PseudoMemberFunc)(T*, Params...)) : m_Type(_Type::Delegate)
    {
        new (&m_Delegate) DelegateType(a_pThis, a_PseudoMemberFunc);
    }

    ThisType& operator=(R (*a_In)(Params...))
    {
        if (m_Type == _Type::Delegate)
        {
            _delegate() = a_In;
        }
        else
        {
            _destroy();
            m_Type = _Type::Delegate;
            new (&m_Delegate) DelegateType(a_In);
        }
        return *this;
    }

    ThisType& operator=(DelegateType const& a_In)
    {
        if (m_Type == _Type::Delegate)
        {
            _delegate() = a_In;
        }
        else
        {
            _destroy();
            m_Type = _Type::Delegate;
            new (&m_Delegate) DelegateType(a_In);
        }
        return *this;
    }

    ThisType& operator=(DelegateType&& a_In)
    {
        if (m_Type == _Type::Delegate)
        {
            _delegate() = std::move(a_In);
        }
        else
        {
            _destroy();
            m_Type = _Type::Delegate;
            new (&m_Delegate) DelegateType(std::move(a_In));
        }
        return *this;
    }

    // std::function

    Functor(StdFuncType const& a_In, void* a_pLambdaDiscriminant = nullptr) : m_Type(_Type::StdFunction)
    {
        new (&m_StdFunction) StdFuncType(a_In);
        m_pLambdaDiscriminant = a_pLambdaDiscriminant;
    }

    Functor(StdFuncType&& a_In,
            void*         a_pLambdaDiscriminant = nullptr) // by default, set as delegate
        : m_Type(_Type::StdFunction)
    {
        new (&m_StdFunction) StdFuncType(std::move(a_In));
        m_pLambdaDiscriminant = a_pLambdaDiscriminant;
    }

    template<class Func, class = EnableIfCustomArg<Func>>
    Functor(Func&& a_In, void* a_pLambdaDiscriminant = nullptr) : m_Type(_Type::StdFunction)
    {
        new (&m_StdFunction) StdFuncType(std::forward<Func>(a_In));
        m_pLambdaDiscriminant = a_pLambdaDiscriminant;
    }

    ThisType& operator=(StdFuncType const& a_In)
    {
        if (m_Type == _Type::StdFunction)
        {
            _stdFunction() = a_In;
        }
        else
        {
            _destroy();
            m_Type = _Type::StdFunction;
            new (&m_StdFunction) StdFuncType(a_In);
        }
        m_pLambdaDiscriminant = nullptr;
        return *this;
    }

    ThisType& operator=(StdFuncType&& a_In)
    {
        if (m_Type == _Type::StdFunction)
        {
            _stdFunction() = std::move(a_In);
        }
        else
        {
            _destroy();
            m_Type = _Type::StdFunction;
            new (&m_StdFunction) StdFuncType(std::move(a_In));
        }
        m_pLambdaDiscriminant = nullptr;
        return *this;
    }

    template<class Func, class = EnableIfCustomArg<Func>>
    ThisType& operator=(Func&& a_In)
    {
        if (m_Type == _Type::StdFunction)
        {
            _stdFunction() = std::forward<Func>(a_In);
        }
        else
        {
            _destroy();
            m_Type = _Type::StdFunction;
            new (&m_StdFunction) StdFuncType(std::forward<Func>(a_In));
        }
        m_pLambdaDiscriminant = nullptr;
        return *this;
    }

    template<class Func, class = EnableIfCustomArg<Func>>
    ThisType& setLambdaOrStruct(Func&& a_In, void* a_pLambdaDiscriminant = nullptr)
    {
        if (m_Type == _Type::StdFunction)
        {
            _stdFunction() = std::forward<Func>(a_In);
        }
        else
        {
            _destroy();
            m_Type = _Type::StdFunction;
            new (&m_StdFunction) StdFuncType(std::forward<Func>(a_In));
        }
        m_pLambdaDiscriminant = a_pLambdaDiscriminant;
        return *this;
    }

    // DynDelegateType

    Functor(DynDelegateType const& a_In) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(a_In);
    }

    Functor(DynDelegateType&& a_In) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(std::move(a_In));
    }

    Functor(reflection::Function* a_pFunction) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(a_pFunction);
    }
    Functor(Object a_Object, reflection::Method* a_pMethod) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(a_Object, a_pMethod);
    }
    Functor(Object a_Object, StringView a_MethodName) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(a_Object, a_MethodName);
    }
    explicit Functor(OpaqueDynDelegate a_OpaqueDynDelegate) : m_Type(_Type::DynDelegate)
    {
        new (&m_DynDelegate) DynDelegateType(a_OpaqueDynDelegate);
    }

    ThisType& operator=(DynDelegateType const& a_In)
    {
        if (m_Type == _Type::DynDelegate)
        {
            _dynDelegate() = a_In;
        }
        else
        {
            _destroy();
            m_Type = _Type::DynDelegate;
            new (&m_Delegate) DynDelegateType(a_In);
        }
        return *this;
    }

    ThisType& operator=(DynDelegateType&& a_In)
    {
        if (m_Type == _Type::DynDelegate)
        {
            _dynDelegate() = std::move(a_In);
        }
        else
        {
            _destroy();
            m_Type = _Type::DynDelegate;
            new (&m_Delegate) DynDelegateType(std::move(a_In));
        }
        return *this;
    }

    bool isEmpty() const
    {
        return !operator bool();
    }

    FunctorID getID() const
    {
        if (isEmpty())
            return FunctorID();
        switch (m_Type)
        {
        case _Type::Delegate:
            return _delegate().getID();
        case _Type::StdFunction:
            return FunctorID(_stdFunction().target_type().hash_code(), size_t(m_pLambdaDiscriminant));
        default:
            return _dynDelegate().getID();
        }
    }

    R operator()(Params... a_Args) const
    {
        switch (m_Type)
        {
        case _Type::Delegate:
            return _delegate()(a_Args...);
        case _Type::StdFunction:
            return _stdFunction()(a_Args...);
        default:
            return _dynDelegate()(a_Args...);
        }
    }

    operator bool() const
    {
        switch (m_Type)
        {
        case _Type::Delegate:
            return bool(_delegate());
        case _Type::StdFunction:
            return bool(_stdFunction());
        default:
            return bool(_dynDelegate());
        }
    }

    bool operator==(ThisType const& a_Other) const
    {
        return m_Type == a_Other.m_Type && getID() == a_Other.getID();
    }

    bool operator!=(ThisType const& a_Other) const
    {
        return !operator==(a_Other);
    }

    bool as(DelegateType& a_Out)
    {
        if (m_Type == _Type::Delegate)
        {
            a_Out = _delegate();
            return true;
        }
        return false;
    }

    bool as(StdFuncType& a_Out)
    {
        if (m_Type == _Type::StdFunction)
        {
            a_Out = _stdFunction();
            return true;
        }
        return false;
    }

    bool as(DynDelegateType& a_Out)
    {
        if (m_Type == _Type::DynDelegate)
        {
            a_Out = _dynDelegate();
            return true;
        }
        return false;
    }

private:
    DelegateType& _delegate()
    {
        PHANTOM_ASSERT(m_Type == _Type::Delegate);
        return (DelegateType&)m_Delegate;
    }
    StdFuncType& _stdFunction()
    {
        PHANTOM_ASSERT(m_Type == _Type::StdFunction);
        return (StdFuncType&)m_StdFunction;
    }
    DynDelegateType& _dynDelegate()
    {
        PHANTOM_ASSERT(m_Type == _Type::DynDelegate);
        return (DynDelegateType&)m_DynDelegate;
    }
    DelegateType const& _delegate() const
    {
        PHANTOM_ASSERT(m_Type == _Type::Delegate);
        return (DelegateType&)m_Delegate;
    }
    StdFuncType const& _stdFunction() const
    {
        PHANTOM_ASSERT(m_Type == _Type::StdFunction);
        return (StdFuncType&)m_StdFunction;
    }
    DynDelegateType const& _dynDelegate() const
    {
        PHANTOM_ASSERT(m_Type == _Type::DynDelegate);
        return (DynDelegateType&)m_DynDelegate;
    }
    void _destroy()
    {
        switch (m_Type)
        {
        case _Type::Delegate:
            _delegate().~DelegateType();
            break;
        case _Type::StdFunction:
            _stdFunction().~StdFuncType();
            break;
        default:
            _dynDelegate().~DynDelegateType();
        }
    }

private:
    union {
        std::aligned_storage_t<sizeof(DelegateType), alignof(DelegateType)> m_Delegate;
        struct
        {
            std::aligned_storage_t<sizeof(StdFuncType), alignof(StdFuncType)> m_StdFunction;
            void* m_pLambdaDiscriminant; // this serves as a discriminant to build the functor ID
                                         // and compare them in case of lambdas
        };
        std::aligned_storage_t<sizeof(DynDelegateType), alignof(DynDelegateType)> m_DynDelegate;
    };
    _Type m_Type;
};

template<class T, class U, class R, class... Params>
auto makeFunctor(T* a_pThis, R (U::*a_Func)(Params...))
{
    return Functor<R(Params...)>(a_pThis, a_Func);
}

template<class T, class R, class... Params>
auto makeFunctor(R (*a_Func)(Params...))
{
    return Functor<R(Params...)>(a_Func);
}

} // namespace phantom
