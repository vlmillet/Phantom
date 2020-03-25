#pragma once

// haunt {

// clang-format off

#include "LanguageElementVisitor.h"

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
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LanguageElementVisitor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(VisitorData)
        {
            this_()
            .constructor<void()>()
            .constructor<void(int, const void**, void**, int)>()["nullptr"]["nullptr"]["0"]
            .field("id", &_::id)
            .field("in", &_::in)
            .field("out", &_::out)
            .field("flags", &_::flags)
            .method<bool(int) const>("hasFlag", &_::hasFlag)
            ;
        }
        PHANTOM_CLASS(LanguageElementVisitor)
        {
            this_()
        .public_()
            .method<void(Alias*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousSection*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(AnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Application*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Array*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Class*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ClassType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Constant*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Constructor*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ConstVolatileType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ContainerClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Field*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FieldPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Pointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Destructor*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Ellipsis*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Enum*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Function*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FunctionPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(FunctionType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(InitializerListType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LanguageElement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LocalVariable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(LValueReference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MapClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousSection*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousStruct*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberAnonymousUnion*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Method*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MethodPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(MemberPointer*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Module*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Namespace*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Package*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PackageFolder*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Parameter*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Placeholder*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderClassType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderConstant*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderTemplate*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PlaceholderType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PointerType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(PrimitiveType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Property*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Reference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(RValueReference*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Scope*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SequentialContainerClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(SetClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VectorClass*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Signal*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Signature*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Source*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Structure*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Subroutine*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Symbol*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Template*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantArray*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantElement*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantTemplateInstance*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateDependantType*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateParameter*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateSignature*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(TemplateSpecialization*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Type*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Union*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(ValueMember*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(Variable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VirtualMethodTable*, VisitorData), virtual_>("visit", &_::visit)
            .method<void(VolatileType*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FundamentalTypeT<void>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (std::nullptr_t, phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::phantom::lang::FundamentalTypeT<std::nullptr_t> *, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<bool>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<uchar>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<schar>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<short>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ushort>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<int>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<uint>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<long>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ulong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<longlong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<ulonglong>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<float>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<double>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::FloatingPointTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(FloatingPointTypeT<longdouble>*, VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<wchar_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char16_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            /// missing symbol(s) reflection (phantom::lang::IntegralTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(IntegralTypeT<char32_t>*, ::phantom::lang::VisitorData), virtual_>("visit", &_::visit)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElementVisitor")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
