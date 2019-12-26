#pragma once

namespace phantom
{
	using nullptr_t = std::nullptr_t;
}

// haunt {

#include "StringConverter.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/static_method>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallString.hxx"

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
namespace detail {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StringConverter")

        PHANTOM_CLASS_T((class), (t_Ty), MetaTypeIdOf)
        {
            this_()
            ;
        }
    PHANTOM_END("StringConverter")
PHANTOM_END("phantom")
}
}
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StringConverter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<string_converter_id>().values({
            {"string_converter_default",string_converter_default},
            {"string_converter_array",string_converter_array},
            {"string_converter_enum",string_converter_enum},
            {"string_converter_class",string_converter_class},
            {"string_converter_classtype",string_converter_classtype}});
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (t_Ty), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .PHANTOM_T staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((unsigned long), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((unsigned long long), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((long), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((long long), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((float), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_S((wchar_t), literal_suffix)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .staticMethod<void(StringBuffer&)>("suffix", &_::suffix)
            ;
        }
        PHANTOM_STRUCT_T((class, int), (t_Ty, t_MetaClassId), StringConverterH)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const t_Ty*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const t_Ty*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringView, t_Ty*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_TS((class), (t_Ty), (t_Ty, string_converter_classtype), StringConverterH)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::ClassType *, StringBuffer&, const t_Ty*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::ClassType *, StringBuffer&, const t_Ty*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::ClassType *, StringView, t_Ty*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_TS((class), (t_Ty), (t_Ty, string_converter_class), StringConverterH)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Class *, StringBuffer&, const t_Ty*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Class *, StringBuffer&, const t_Ty*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Class *, StringView, t_Ty*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_TS((class), (t_Ty), (t_Ty, string_converter_enum), StringConverterH)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Enum *, StringBuffer&, const t_Ty*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Enum *, StringBuffer&, const t_Ty*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Enum *, StringView, t_Ty*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_TS((class), (t_Ty), (t_Ty, string_converter_array), StringConverterH)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Array *, StringBuffer&, const t_Ty*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Array *, StringBuffer&, const t_Ty*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Array *, StringView, t_Ty*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_T((class), (t_Ty), StringConverter)
        {
            this_()
            .PHANTOM_T inherits<::phantom::StringConverterH<t_Ty, std::is_array<t_Ty>::value ? string_converter_array : std::is_enum<t_Ty>::value ? string_converter_enum : (std::is_void<t_Ty>::value) ? string_converter_default : phantom::IsDataPointer<t_Ty>::value ? string_converter_default : std::is_pointer<t_Ty>::value ? string_converter_default : ::std::is_floating_point<t_Ty>::value ? string_converter_default : ::phantom::IsNullptrT<t_Ty>::value ? string_converter_default : ::std::is_integral<t_Ty>::value ? string_converter_default : ::std::is_member_function_pointer<t_Ty>::value ? string_converter_default : ::std::is_member_object_pointer<t_Ty>::value ? string_converter_default : std::is_union<t_Ty>::value ? string_converter_classtype : std::is_class<t_Ty>::value ? IsStructure<t_Ty>::value ? string_converter_classtype : string_converter_class : string_converter_default>>()
            ;
        }
        PHANTOM_STRUCT_S((void), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const void*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const void*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, void*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((nullptr_t), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            /// missing symbol(s) reflection (std::nullptr_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const std::nullptr_t *)>("toLiteral", &_::toLiteral)
            /// missing symbol(s) reflection (std::nullptr_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const std::nullptr_t *)>("to", &_::to)
            /// missing symbol(s) reflection (std::nullptr_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<void(const ::phantom::reflection::Type *, StringView, std::nullptr_t *)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((char), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, char*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((bool), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const bool*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const bool*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, bool*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((unsigned char), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const uchar*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const uchar*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, uchar*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((signed char), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const schar*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const schar*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, schar*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((wchar_t), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const wchar_t*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const wchar_t*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, wchar_t*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((char16_t), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char16_t*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char16_t*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, char16_t*)>("from", &_::from)
            ;
        }
        PHANTOM_STRUCT_S((char32_t), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char32_t*)>("toLiteral", &_::toLiteral)
            .staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const char32_t*)>("to", &_::to)
            .staticMethod<void(const ::phantom::reflection::Type *, StringView, char32_t*)>("from", &_::from)
            ;
        }
        /// missing symbol(s) reflection (std::char_traits, std::basic_string) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class), (t_Alloc), (std::basic_string<char, std::char_traits<char>, t_Alloc>), StringConverter)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            /// missing symbol(s) reflection (std::char_traits, std::basic_string) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<self_type>("self_type")
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const self_type*)>("toLiteral", &_::toLiteral)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const self_type*)>("to", &_::to)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringView, self_type*)>("from", &_::from)
            ;
        } */
        PHANTOM_STRUCT_TS((size_t, size_t), (S, D), (SmallString<char, S, D>), StringConverter)
        {
            using InputType = typedef_<PHANTOM_TYPENAME _::InputType>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .PHANTOM_T typedef_<InputType>("InputType")
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const InputType*)>("toLiteral", &_::toLiteral)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringBuffer&, const InputType*)>("to", &_::to)
            .PHANTOM_T staticMethod<void(const ::phantom::reflection::Type *, StringView, InputType*)>("from", &_::from)
            ;
        }
    PHANTOM_END("StringConverter")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
