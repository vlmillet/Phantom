// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Generic.h"
#include "SmallVector.h"
#include "StringView.h"
#include "phantom/detail/core.h"
#include "phantom/traits/CopyTraits.h"
#include "phantom/traits/MoveTraits.h"
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
class PHANTOM_EXPORT_PHANTOM Variant
{
    enum
    {
        StaticBufferSize = 8,
    };

public:
    typedef Delegate<bool(lang::Type*, void*, lang::Type*, const void*)> TypeConverter;
    static void                                                          SetTypeConverter(TypeConverter a_TC);
    static TypeConverter                                                 GetTypeConverter();

    static Variant From(Generic::Param<> a_Param0, Generic::Arg<Generic::ParamType<0>*, void*> a_Arg0)
    {
        return Variant(a_Param0.getType(), a_Arg0.getValue());
    }

    template<class T>
    static Variant From(T* a_pArg)
    {
        return Variant(PHANTOM_TYPEOF(T), a_pArg);
    }

public:
    static const Variant null;
    inline Variant() : m_pType(nullptr) {}

    inline Variant(const char* a_Str);

    inline Variant(const Variant& a_Other);
    inline Variant(Variant&& a_Other);

    template<typename t_Ty, typename = std::enable_if_t<phantom::IsCopyConstructible<t_Ty>::value, void>>
    inline Variant(const t_Ty& a_In);

    template<
    typename t_Ty,
    typename = std::enable_if_t<!std::is_same<std::remove_const_t<std::remove_reference_t<t_Ty>>, Variant>::value &&
                                phantom::IsMoveConstructible<t_Ty>::value,
                                void>>
    inline Variant(t_Ty&& a_In);

    Variant(lang::Type* a_pType, void* a_pValue, bool a_bMove = false);
    Variant(lang::Type* a_pType, void const* a_pValue);

    inline Variant& operator=(const Variant& a_Other);
    inline Variant& operator=(Variant&& a_Other);

    inline Variant& operator=(const char* a_Str);

    template<typename t_Ty,
             typename = std::enable_if_t<
             phantom::IsCopyConstructible<t_Ty>::value && phantom::IsCopyAssignable<t_Ty>::value, void>>
    inline Variant& operator=(t_Ty const& a_In);

    template<
    typename t_Ty,
    typename = std::enable_if_t<!std::is_same<std::remove_const_t<std::remove_reference_t<t_Ty>>, Variant>::value &&
                                phantom::IsMoveConstructible<t_Ty>::value && phantom::IsMoveAssignable<t_Ty>::value,
                                void>>
    inline Variant& operator=(t_Ty&& a_In);

    ~Variant() { clear(); }

    void clear()
    {
        if (m_pType)
            _release();
    }

    inline void setType(lang::Type* a_pType);

    inline bool operator==(const Variant& other) const;

    inline bool operator!=(const Variant& other) const;

    inline bool operator==(StringView other) const;

    inline bool operator!=(StringView other) const;

    inline bool operator==(const char* other) const;

    inline bool operator!=(const char* other) const;

    void copyAssign(const void* a_pValue);
    void moveAssign(void* a_pValue);

    void copyConstruct(lang::Type* a_pType, const void* a_pValue);
    void moveConstruct(lang::Type* a_pType, void* a_pValue);

    inline size_t size() const;

    inline lang::Type* type() const { return m_pType; }

    const char* c_str() const;

    bool isString() const;

    inline const void* data() const { return _buffer(); }

    inline void* data() { return _buffer(); }

    inline bool isValid() const { return m_pType != nullptr; }

    inline bool isNull() const { return m_pType == nullptr; }

    inline bool as(Generic::Param<> a_Param0, Generic::Arg<Generic::ParamType<0>*, void*> a_Arg0) const
    {
        return _as(a_Param0.getType(), a_Arg0.getValue());
    }

    inline phantom::Variant as(lang::Type* a_pType) const;

    template<typename t_Ty>
    inline bool as(t_Ty* a_pDest) const;

    template<typename t_Ty>
    inline t_Ty as(bool* a_pOK = nullptr) const;

    inline void toLiteral(StringBuffer& a_Buf) const;

    inline void toString(StringBuffer& a_Buf) const;

    template<typename T>
    inline T* fundamental();

    template<typename T>
    inline void fundamental(T value);

private:
    inline static byte* _Alloc(size_t _s, size_t _a)
    {
        auto b = reinterpret_cast<byte*>(PHANTOM_MALLOC_ALIGNED(_s, _a));
        // zero memory as we want everything to be safe inside a Variant (they are not designed for performances)
        memset(b, 0, _s);
        return b;
    }

    inline bool _as(lang::Type* a_pType, void* a_pDest) const;

    inline byte* _buffer() const
    {
        return (size() > StaticBufferSize) ? m_Buffer.dynamicBuffer : (byte*)m_Buffer.staticBuffer;
    }
    inline void _release();
    lang::Type* m_pType;
    union { // storage for small buffer or pointer to larger one
        byte  staticBuffer[StaticBufferSize];
        byte* dynamicBuffer;
    } m_Buffer;
};

} // namespace phantom

#include <phantom/lang/TypeOf.h>
#include <phantom/utils/Variant.inl>
