// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/core.h>

#pragma warning(disable : 4700)

namespace phantom
{
/// @cond INTERNAL

HAUNT_PAUSE;

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
struct MethodClosure
{
    MethodClosure() = default;

    void setClosurePointer(void* ptr)
    {
        closure.address = ptr;
    }

    void setClosure(Closure ptr)
    {
        closure = ptr;
    }

    MethodClosure(std::nullptr_t)
    {
    }

    template<typename t_MemberPtr>
    MethodClosure(t_MemberPtr ptr)
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                void* address;
                int   delta; // #BYTES to be added to the 'this' pointer
            };
        } un;
        un.mptr = ptr;
        closure.address = un.address;
        closure.offset = un.delta;
    }
    template<typename t_MemberPtr>
    MethodClosure& operator=(t_MemberPtr ptr)
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                void* address;
                int   delta; // #BYTES to be added to the 'this' pointer
            };
        } un;
        un.mptr = ptr;
        closure.address = un.address;
        closure.offset = un.offset;
        return *this;
    }
    template<typename t_MemberPtr>
    operator t_MemberPtr() const
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                void* address;
                int   delta; // #BYTES to be added to the 'this' pointer
            };
        } un;
        un.address = closure.address;
        un.delta = closure.offset;
        return un.mptr;
    }
    operator Closure() const
    {
        return closure;
    }
    operator bool() const
    {
        return closure.address != nullptr;
    }

    void* getAddress() const
    {
        return closure.address;
    }
    int getOffset() const
    {
        return closure.offset;
    }

protected:
    Closure closure;
};

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC || PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG

struct MethodClosure
{
    MethodClosure() = default;

    void setClosurePointer(void* ptr)
    {
        funcaddress = ptr;
    }

    void setClosure(Closure ptr)
    {
        funcaddress = ptr.address;
        delta = ptr.offset;
    }

    template<typename t_MemberPtr>
    MethodClosure(t_MemberPtr ptr)
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                union {
                    void* funcaddress;    // always even
                    int   vtable_index_2; //  = vindex*2+1, always odd
                };
                int delta;
            };
        } un;
        //         adjustedthis = this + delta
        //             if (funcadr & 1) CALL (* ( *delta + (vindex+1)/2) + 4)
        //             else CALL funcadr
        un.mptr = ptr;
        funcaddress = un.funcaddress;
        vtable_index_2 = un.vtable_index_2;
        delta = un.delta;
    }
    template<typename t_MemberPtr>
    MethodClosure& operator=(t_MemberPtr ptr)
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                union {
                    void* funcaddress;    // always even
                    int   vtable_index_2; //  = vindex*2+1, always odd
                };
                int delta;
            };
        } un;
        //         adjustedthis = this + delta
        //             if (funcadr & 1) CALL (* ( *delta + (vindex+1)/2) + 4)
        //             else CALL funcadr
        un.mptr = ptr;
        funcaddress = un.funcaddress;
        vtable_index_2 = un.vtable_index_2;
        delta = un.delta;
        return *this;
    }
    template<typename t_MemberPtr>
    operator t_MemberPtr() const
    {
        PHANTOM_STATIC_ASSERT(PHANTOM_STD_NAMESPACE::is_member_function_pointer<t_MemberPtr>::value,
                              "must be a member function pointer");
        union {
            t_MemberPtr mptr;
            struct
            {
                union {
                    void* funcaddress;    // always even
                    int   vtable_index_2; //  = vindex*2+1, always odd
                };
                int delta;
            };
        } un;
        un.funcaddress = funcaddress;
        un.delta = delta;
        un.vtable_index_2 = vtable_index_2;
        return un.mptr;
    }

    operator Closure() const
    {
        Closure cl;
        cl.address = funcaddress;
        cl.offset = delta;
        return cl;
    }

    void* getAddress() const
    {
        return funcaddress;
    }
    int getOffset() const
    {
        return delta;
    }

protected:
    struct
    {
        union {
            void* funcaddress;    // always even
            int   vtable_index_2; //  = vindex*2+1, always odd
        };
        int delta;
    };
};

#else
#    error define MethodClosure for your compiler
#endif

HAUNT_RESUME;

/// @endcond

} // namespace phantom

#pragma warning(default : 4700)
