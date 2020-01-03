// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/detail/phantom.h>
#if PHANTOM_ACCURATE_NATIVE_VTABLE_INSPECTOR_SIGNATURES
#    include <phantom/traits/DefaultValue.h>
#endif
#include <phantom/traits/HasVirtualDestructor.h>
#include <vector>
HAUNT_STOP;
/* *********************************************** */

namespace phantom
{
template<typename t_Ty>
size_t virtualMethodCountOf();
}

namespace phantom
{
namespace reflection
{
class PHANTOM_EXPORT_PHANTOM NativeVTableInspector
{
public:
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    static void NativeVTableInspector_setResultIndex(size_t value)
    {
        sm_ResultIndex = value;
    }
#endif
    static size_t sm_ResultIndex;
    void*         m_vptr_impostor;
};

template<typename t_MemberFuncPtrTy>
PHANTOM_FORCEINLINE static void* extract_mfp_closure(t_MemberFuncPtrTy ptr)
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_GCC || PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
    union {
        struct
        {
            union {
                void*     fn;
                ptrdiff_t vtable_index;
            };
            ptrdiff_t delta;
        } wrap_fp;

        t_MemberFuncPtrTy fp;
    } u;
    u.fp = ptr;
    return u.wrap_fp.fn;

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO /// /vmg + /vmv must be defined
    struct __MicrosoftUnknownMFP
    {
        void*     m_func_address; // 64 bits for Itanium.
        ptrdiff_t m_delta;
        int       m_vtordisp;
        int       m_vtable_index; // or 0 if no virtual inheritance
    };
    union {
        t_MemberFuncPtrTy     original;
        __MicrosoftUnknownMFP generic;
    } un;
    un.original = ptr;
    return un.generic.m_func_address;
#else
#    error extract_closure not defined for your compiler, define it here
#endif
}

#define NativeVTableInspector_function_section_size 250

#if PHANTOM_DEBUG_LEVEL != PHANTOM_DEBUG_LEVEL_FULL
#    define NativeVTableInspector_setResultIndex(...) NativeVTableInspector::sm_ResultIndex = __VA_ARGS__
#endif

#define NativeVTableInspector_hack_member_function_0(z, count, signature)                                              \
    R hack_member_function_0_##count signature                                                                         \
    {                                                                                                                  \
        NativeVTableInspector_setResultIndex(count);                                                                   \
        return DefaultValue<R>::apply();                                                                               \
    }

#define NativeVTableInspector_hack_member_function_1(z, count, signature)                                              \
    R hack_member_function_1_##count signature                                                                         \
    {                                                                                                                  \
        NativeVTableInspector_setResultIndex(count + NativeVTableInspector_function_section_size);                     \
        return DefaultValue<R>::apply();                                                                               \
    }

#define NativeVTableInspector_hack_member_function_0_void(z, count, signature)                                         \
    void hack_member_function_0_##count signature                                                                      \
    {                                                                                                                  \
        NativeVTableInspector_setResultIndex(count);                                                                   \
    }

#define NativeVTableInspector_hack_member_function_1_void(z, count, signature)                                         \
    void hack_member_function_1_##count signature                                                                      \
    {                                                                                                                  \
        NativeVTableInspector_setResultIndex(count + NativeVTableInspector_function_section_size);                     \
    }

#define NativeVTableInspector_vtableentry_0(z, count, signature)                                                       \
    extract_mfp_closure(&self_type::hack_member_function_0_##count),

#define NativeVTableInspector_vtableentry_1(z, count, signature)                                                       \
    extract_mfp_closure(&self_type::hack_member_function_1_##count),

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define VTableInspector_commonT(call_args)                                                                         \
        PHANTOM_STATIC_ASSERT(sizeof(member_function_pointer_t) == 2 * sizeof(int) + 2 * sizeof(void*),                \
                              "member function pointer size must be 16 bytes on x86 and 24 bytes on x64, ensure /vmg " \
                              "and /vmv are added to your Visual Studio C++ compiler command line options");           \
        template<typename t_MemberFuncPtrTy>                                                                           \
        size_t getIndexOf(t_MemberFuncPtrTy ptr)                                                                       \
        {                                                                                                              \
            struct __MicrosoftUnknownMFP                                                                               \
            {                                                                                                          \
                void*     m_func_address;                                                                              \
                ptrdiff_t m_delta;                                                                                     \
                int       m_vtordisp;                                                                                  \
                int       m_vtable_index;                                                                              \
            };                                                                                                         \
            union {                                                                                                    \
                t_MemberFuncPtrTy     original;                                                                        \
                __MicrosoftUnknownMFP generic;                                                                         \
            } un;                                                                                                      \
            un.original = ptr;                                                                                         \
            if (un.generic.m_delta != 0)                                                                               \
            {                                                                                                          \
                int i = 0;                                                                                             \
                ++i;                                                                                                   \
            }                                                                                                          \
            byte* fakeInstance = (byte*)&m_vptr_impostor;                                                              \
            fakeInstance -= un.generic.m_delta;                                                                        \
            member_function_pointer_t mp = *reinterpret_cast<member_function_pointer_t*>(&ptr);                        \
            PHANTOM_ASSERT(sm_ResultIndex == ~size_t(0));                                                              \
            (reinterpret_cast<decltype(this)>(fakeInstance)->*mp) call_args;                                           \
            PHANTOM_ASSERT(sm_ResultIndex != ~size_t(0));                                                              \
            size_t result = sm_ResultIndex;                                                                            \
            sm_ResultIndex = ~size_t(0);                                                                               \
            return result;                                                                                             \
        }
#else

#    define VTableInspector_commonT(call_args)                                                                         \
        template<typename t_MemberFuncPtrTy>                                                                           \
        size_t getIndexOf(t_MemberFuncPtrTy ptr)                                                                       \
        {                                                                                                              \
            union {                                                                                                    \
                struct                                                                                                 \
                {                                                                                                      \
                    union {                                                                                            \
                        void (*fn)();                                                                                  \
                        ptrdiff_t vtable_index;                                                                        \
                    };                                                                                                 \
                    ptrdiff_t delta;                                                                                   \
                } wrap_fp;                                                                                             \
                                                                                                                       \
                t_MemberFuncPtrTy fp;                                                                                  \
            } u;                                                                                                       \
            u.fp = ptr;                                                                                                \
            return (u.wrap_fp.vtable_index - 1) / sizeof(u.wrap_fp.fn);                                                \
        }

#endif

template<typename t_Signature>
class VTableInspectorT;

class PHANTOM_EXPORT_PHANTOM VTableInspectorGeneric : public NativeVTableInspector
{
public:
    typedef VTableInspectorGeneric self_type;
    typedef void (VTableInspectorGeneric::*member_function_pointer_t)(void*);
    VTableInspectorGeneric();

    // -----------------------------------------------------
    // A CRASH HERE MEANS YOU PROBABLY DECLARED a non-virtual method as 'virtual' in your reflection
    VTableInspector_commonT((nullptr))
    // -----------------------------------------------------
};

class PHANTOM_EXPORT_PHANTOM NativeDestructorInspector : public VTableInspectorGeneric
{
public:
    template<typename t_Ty>
    size_t getDestructorIndex()
    {
        PHANTOM_ASSERT(sm_ResultIndex == ~size_t(0));
        reinterpret_cast<t_Ty*>(&m_vptr_impostor)->~t_Ty();
        PHANTOM_ASSERT(sm_ResultIndex != ~size_t(0));
        size_t result = sm_ResultIndex;
        sm_ResultIndex = ~size_t(0);
        return result;
    }
};

} // namespace reflection
} // namespace phantom

// Create easy access traits
namespace phantom
{
/// returns the vtable index of the given member_function or -1 if it's not a virtual
/// member_function throw static assert if the tested argument is not a member function pointer

template<typename t_Ty, bool t_has_virtual_destructor_recursive>
struct VirtualDtorIndexProviderH
{
    static size_t apply()
    {
        return phantom::reflection::NativeDestructorInspector().getDestructorIndex<t_Ty>();
    }
};

template<typename t_Ty>
struct VirtualDtorIndexProviderH<t_Ty, false>
{
    static size_t apply()
    {
        return ~size_t(0);
    }
};

template<typename t_Ty>
struct VirtualDtorIndexProvider : public VirtualDtorIndexProviderH<t_Ty, HasVirtualDestructor<t_Ty>::value>
{
};

template<typename t_Ty>
size_t virtualDestructorIndex()
{
    return VirtualDtorIndexProvider<t_Ty>::apply();
}

template<bool is_virtual = true>
struct VTableIndexOf
{
    template<typename t_MemberFuncPtrTy>
    static size_t apply(t_MemberFuncPtrTy ptr)
    {
        return ::phantom::reflection::VTableInspectorGeneric().getIndexOf(ptr);
    }
};

template<>
struct VTableIndexOf<false>
{
    template<typename t_MemberFuncPtrTy>
    static size_t apply(t_MemberFuncPtrTy ptr)
    {
        return ~size_t(0);
    }
};
} // namespace phantom
/// @endcond
