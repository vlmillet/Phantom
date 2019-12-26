// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet. 
// Distributed under the MIT license. Text available here at http://www.wiwila.com/tools/phantom/license/ 
// ]

#include <phantom/reflection/TypeOf.h>
#include "RttiMapData.h"
#include "Rtti.h"

namespace phantom {

    namespace detail
    {
        template<class t_Ty>
        struct VariantTypeOf
        {
            PHANTOM_STATIC_ASSERT(!(std::is_same<t_Ty, Variant>::value));
            static reflection::Type* object() { return PHANTOM_TYPEOF(t_Ty); }
        };

        template<class t_Ty>
        struct VariantTypeOf<t_Ty*>
        {
            static reflection::Type* object() 
            { 
                if (auto pType = PHANTOM_TYPEOF(t_Ty))
                    return pType->addPointer();
                return phantom::reflection::BuiltInTypes::TYPE_VOID_PTR;
            }
        };
    }

#define PHANTOM_VARIANT_TYPEOF(...)  phantom::detail::VariantTypeOf<__VA_ARGS__>::object()

    template<typename t_Ty, typename>
    inline Variant::Variant(const t_Ty& a_In) 
    {
        byte* pBuffer = (sizeof(t_Ty) > StaticBufferSize) 
            ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(t_Ty))) 
            : m_Buffer.staticBuffer;
        m_pType = PHANTOM_VARIANT_TYPEOF(t_Ty);
        new (pBuffer) t_Ty(a_In);
    }

    template<typename t_Ty, typename>
    inline Variant::Variant(t_Ty&& a_In)
    {
        byte* pBuffer = (sizeof(std::remove_reference_t<t_Ty>) > StaticBufferSize)
            ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(std::remove_reference_t<t_Ty>)))
            : m_Buffer.staticBuffer;
        m_pType = PHANTOM_VARIANT_TYPEOF(std::remove_reference_t<t_Ty>);
        new (pBuffer) std::remove_reference_t<t_Ty>(std::forward<t_Ty>(a_In));
    }

    inline Variant::Variant(const char* a_Str) 
    {
        new (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(String))) String(a_Str);
        PHANTOM_ASSERT(phantom::reflection::BuiltInTypes::TYPE_STRING);
        m_pType = (reflection::Type*)phantom::reflection::BuiltInTypes::TYPE_STRING;
    }

    Variant::Variant(Variant&& a_Other)
        : m_pType(a_Other.m_pType)
    {
        if (a_Other.isValid())
        {
            if ((a_Other.size() > StaticBufferSize))
            {
                m_Buffer.dynamicBuffer = a_Other.m_Buffer.dynamicBuffer;
                a_Other.m_Buffer.dynamicBuffer = nullptr;
                a_Other.m_pType = nullptr;
            }
            else
            {
                m_pType->construct(m_Buffer.staticBuffer);
                m_pType->copy(m_Buffer.staticBuffer, a_Other.m_Buffer.staticBuffer);
            }
        }
    }

    inline Variant::Variant(const Variant& a_Other) 
        : m_pType(a_Other.m_pType)
    {
        if(a_Other.isValid())
        {
            byte* pBuffer = (a_Other.size() > StaticBufferSize) 
                ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_Other.size())) 
                : m_Buffer.staticBuffer;
            m_pType->construct(pBuffer);
            m_pType->copy(pBuffer, a_Other._buffer());
        }
    }

    inline Variant& Variant::operator=(const Variant& a_Other)
    {
        if (m_pType)
        {
            _release();
        }
        if (a_Other.isValid())
        {
            byte* pBuffer = (a_Other.size() > StaticBufferSize)
                ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_Other.size()))
                : m_Buffer.staticBuffer;
            m_pType = a_Other.m_pType;
            m_pType->construct(pBuffer);
            m_pType->copy(pBuffer, a_Other._buffer());
        }
        else m_pType = nullptr;
        return *this;
    }

    inline Variant& Variant::operator=(Variant&& a_Other)
    {
        if (m_pType)
        {
            _release();
        }
        if (a_Other.isValid())
        {
            if (a_Other.size() > StaticBufferSize)
            {
                m_Buffer.dynamicBuffer = a_Other.m_Buffer.dynamicBuffer;
                a_Other.m_Buffer.dynamicBuffer = nullptr;
            }
            else
            {
                memcpy(m_Buffer.staticBuffer, a_Other.m_Buffer.staticBuffer, StaticBufferSize);
            }
            m_pType = a_Other.m_pType;
            a_Other.m_pType = nullptr;
        }
        else m_pType = nullptr;
        return *this;
    }

    inline Variant& Variant::operator=(const char* a_Str)
    {
        if (m_pType)
        {
            _release();
        }
        new (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(String))) String(a_Str);
        m_pType = (reflection::Type*)phantom::reflection::BuiltInTypes::TYPE_STRING;
        return *this;
    }

    template<typename t_Ty, class>
    inline Variant& Variant::operator=(const t_Ty& a_In)
    {
        auto pType = PHANTOM_VARIANT_TYPEOF(t_Ty);
        PHANTOM_ASSERT(pType);
        if (m_pType == pType)
        {
            *(t_Ty*)_buffer() = a_In;
        }
        else
        {
            if (m_pType)
                _release();
            byte* pBuffer = (sizeof(t_Ty) > StaticBufferSize)
                ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(t_Ty)))
                : m_Buffer.staticBuffer;
            m_pType = pType;
            new (pBuffer) t_Ty(a_In);
        }
        return *this;
    }

    template<typename t_Ty, class>
    inline Variant& Variant::operator=(t_Ty&& a_In)
    {
        auto pType = PHANTOM_VARIANT_TYPEOF(std::remove_reference_t<t_Ty>);
        PHANTOM_ASSERT(pType);
        if (m_pType == pType)
        {
            *(std::remove_reference_t<t_Ty>*)_buffer() = std::forward<t_Ty>(a_In);
        }
        else
        {
            if (m_pType)
                _release();
            byte* pBuffer = (sizeof(std::remove_reference_t<t_Ty>) > StaticBufferSize)
                ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(sizeof(std::remove_reference_t<t_Ty>)))
                : m_Buffer.staticBuffer;
            m_pType = pType;
            new (pBuffer) std::remove_reference_t<t_Ty>(std::forward<t_Ty>(a_In));
        }
        return *this;
    }

    inline void Variant::setType(reflection::Type* a_pType)
    {
        if (m_pType)
        {
            _release();
        }
        PHANTOM_ASSERT(a_pType->isCopyable() && a_pType->isDefaultInstanciable());
        byte* pBuffer = (a_pType->getSize() > StaticBufferSize) 
            ? (m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize())) 
            : m_Buffer.staticBuffer;
        m_pType = a_pType;
        m_pType->construct(pBuffer);
    }

    template<typename T>
    inline T* Variant::fundamental()
    {
        PHANTOM_STATIC_ASSERT(std::is_fundamental<T>::value, "T must be a fundamental type");
        PHANTOM_STATIC_ASSERT(sizeof(T) <= StaticBufferSize, "T size must be less than static buffer size");
        if (m_pType)
        {
            _release();
        }
        m_pType = PHANTOM_VARIANT_TYPEOF(T);
        return (T*)m_Buffer.staticBuffer;
    }

    template<typename T>
    inline void Variant::fundamental(T value)
    {
        PHANTOM_STATIC_ASSERT(std::is_fundamental<T>::value, "T must be a fundamental type");
        PHANTOM_STATIC_ASSERT(sizeof(T) <= StaticBufferSize, "T size must be less than static buffer size");
        if (m_pType)
        {
            _release();
        }
        m_pType = PHANTOM_VARIANT_TYPEOF(T);
        *(T*)m_Buffer.staticBuffer = value;
    }

    inline bool Variant::operator==(const char* other) const
    {
        return (m_pType == (reflection::Type*)phantom::reflection::BuiltInTypes::TYPE_STRING) AND(*((String*)_buffer()) == other);
    }

    inline bool Variant::operator==(StringView other) const
    {
        return (m_pType == (reflection::Type*)phantom::reflection::BuiltInTypes::TYPE_STRING) AND(*((String*)_buffer()) == other);
    }

    inline bool Variant::operator==(const Variant& other) const 
    {
        if (m_pType == nullptr)
            return other.m_pType == nullptr;
        else if (other.m_pType == nullptr)
            return m_pType == nullptr;
        if (m_pType->isSame((reflection::Type*)phantom::reflection::BuiltInTypes::TYPE_STRING))
        {
            return (m_pType->isSame(other.m_pType)) AND (*((String*)_buffer()) == *((String*)other._buffer()));
        }
        return (m_pType->isSame(other.m_pType)) AND m_pType->equal(_buffer(), other._buffer());
    }

    inline bool Variant::operator!=(const char* other) const
    {
        return !operator==(other);
    }

    inline bool Variant::operator!=(StringView other) const
    {
        return !operator==(other);
    }

    inline bool Variant::operator!=(const Variant& other) const
    {
        return !operator==(other);
    }

    inline size_t Variant::size() const 
    {
        return m_pType ? m_pType->getSize() : 0; 
    }

    inline const char* Variant::c_str() const
    {
        PHANTOM_ASSERT(isString());
        return (*(String*)_buffer()).c_str();
    }

    inline bool Variant::isString() const {
        PHANTOM_ASSERT(phantom::reflection::BuiltInTypes::TYPE_STRING);
        return m_pType == phantom::reflection::BuiltInTypes::TYPE_STRING;
    }

    inline bool Variant::_as(reflection::Type* a_pType, void* a_pDest) const
    {
        if(a_pType == nullptr OR m_pType == nullptr)
        {
            return false;
        }
        if(m_pType->removeAllQualifiers()->isSame(a_pType->removeAllQualifiers()))
        {
            m_pType->copy(a_pDest, _buffer());
            return true;
        }
        if (!GetTypeConverter().empty())
            return GetTypeConverter()(a_pType, a_pDest, m_pType, _buffer());
        else
            return m_pType->convert(a_pType, a_pDest, _buffer());
    }

    inline Variant Variant::as(reflection::Type* a_pType) const
    {
        if(a_pType == nullptr OR m_pType == nullptr)
        {
            return phantom::Variant();
        }
        Variant result;
        byte* pBuffer = (a_pType->getSize() > StaticBufferSize) 
            ? (result.m_Buffer.dynamicBuffer = (byte*)PHANTOM_MALLOC(a_pType->getSize()))
            : result.m_Buffer.staticBuffer;
        result.m_pType = a_pType;

        if(m_pType->removeAllQualifiers()->isSame(a_pType->removeAllQualifiers()))
        {
            m_pType->copy(pBuffer, _buffer());
            return result;
		}
        if (!GetTypeConverter().empty())
        {
            if(GetTypeConverter()(a_pType, pBuffer, m_pType, _buffer()))
                return result;
        }
        else
        {
            if (m_pType->convert(a_pType, pBuffer, _buffer()))
                return result;
        }
        return Variant();
    }

    inline void Variant::_release()
    {
        if(size() > StaticBufferSize)
        {
            m_pType->destroy(m_Buffer.dynamicBuffer);
			PHANTOM_FREE(m_Buffer.dynamicBuffer);
        }
        else 
        {
            m_pType->destroy(m_Buffer.staticBuffer);
        }
    }          

    template<typename t_Ty>
    inline bool Variant::as(t_Ty* a_pDest) const
    {
        return as(PHANTOM_VARIANT_TYPEOF(t_Ty), a_pDest);
    }

    template<typename t_Ty>
    inline t_Ty Variant::as(bool* a_pOK) const
    {
        byte temp[sizeof(t_Ty)];
        PHANTOM_STATIC_ASSERT(!std::is_class<t_Ty>::value, 
            "template return type is a class, it must be a fundamental type"
            ", use 'as(t_Ty* a_pDest)' instead for class to have proper copy to pointed memory");
        auto pType = PHANTOM_VARIANT_TYPEOF(t_Ty);
        pType->construct(&temp);
        if(a_pOK)
        {
            *a_pOK = as(pType, &temp);
        }
        else 
        {
            as(pType, &temp);
        }
        return *((t_Ty*)&temp);
    }

    inline void Variant::toLiteral(StringBuffer& a_Buf) const { if(m_pType) m_pType->valueToLiteral(a_Buf, _buffer()); }

    inline void Variant::toString(StringBuffer& a_Buf) const { if(m_pType) m_pType->valueToString(a_Buf, _buffer()); }

} // namespace phantom