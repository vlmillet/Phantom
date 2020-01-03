#pragma once

// haunt {

#include "reflection.h"

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
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/enum_class>
#include <phantom/static_field>
#include <phantom/function>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Flags.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("reflection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<Access>().values({
            {"Undefined",Access::Undefined},
            {"Public",Access::Public},
            {"Protected",Access::Protected},
            {"Private",Access::Private}});
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Accesses>("Accesses"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<SymbolExtender>("SymbolExtender"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<SymbolExtenders>("SymbolExtenders"); }
        PHANTOM_STRUCT(Modifier)
        {
            using Enum = typedef_<_::Enum>;
            this_()
            .enum_<Enum>().values({
                {"None",_::None},
                {"Virtual",_::Virtual},
                {"VirtualGet",_::VirtualGet},
                {"VirtualSet",_::VirtualSet},
                {"Override",_::Override},
                {"Abstract",_::Abstract},
                {"PureVirtual",_::PureVirtual},
                {"Explicit",_::Explicit},
                {"Static",_::Static},
                {"Final",_::Final},
                {"Transient",_::Transient},
                {"Const",_::Const},
                {"NoConst",_::NoConst},
                {"Volatile",_::Volatile},
                {"Mutable",_::Mutable},
                {"Inline",_::Inline},
                {"Slot",_::Slot},
                {"NoExcept",_::NoExcept},
                {"Singleton",_::Singleton},
                {"LValueRef",_::LValueRef},
                {"RValueRef",_::RValueRef},
                {"RefQualifiersMask",_::RefQualifiersMask},
                {"MethodQualifiersMask",_::MethodQualifiersMask},
                {"Deleted",_::Deleted},
                {"Defaulted",_::Defaulted},
                {"UserFirst",_::UserFirst}})
            .end()
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Modifiers>("Modifiers"); }
        PHANTOM_REGISTER(Functions) { this_().function<::phantom::Flags<Modifier::Enum>(Modifier::Enum, Modifier::Enum)>("operator|", operator|); }
        PHANTOM_REGISTER(Functions) { this_().function<::phantom::Flags<Modifier::Enum>(Modifier::Enum, ::phantom::Flags<Modifier::Enum>)>( "operator|", operator|); }
        PHANTOM_REGISTER(Functions) { this_().function<::phantom::IncompatibleFlag(Modifier::Enum, int)>("operator|", operator|); }
        PHANTOM_STRUCT(BuiltInTypes)
        {
            this_()
        .public_()
            .staticField("TYPE_VOID", &_::TYPE_VOID)
            .staticField("TYPE_CHAR", &_::TYPE_CHAR)
            .staticField("TYPE_UNSIGNED_CHAR", &_::TYPE_UNSIGNED_CHAR)
            .staticField("TYPE_SIGNED_CHAR", &_::TYPE_SIGNED_CHAR)
            .staticField("TYPE_SHORT", &_::TYPE_SHORT)
            .staticField("TYPE_UNSIGNED_SHORT", &_::TYPE_UNSIGNED_SHORT)
            .staticField("TYPE_INT", &_::TYPE_INT)
            .staticField("TYPE_UNSIGNED_INT", &_::TYPE_UNSIGNED_INT)
            .staticField("TYPE_LONG", &_::TYPE_LONG)
            .staticField("TYPE_UNSIGNED_LONG", &_::TYPE_UNSIGNED_LONG)
            .staticField("TYPE_LONG_LONG", &_::TYPE_LONG_LONG)
            .staticField("TYPE_UNSIGNED_LONG_LONG", &_::TYPE_UNSIGNED_LONG_LONG)
            .staticField("TYPE_FLOAT", &_::TYPE_FLOAT)
            .staticField("TYPE_DOUBLE", &_::TYPE_DOUBLE)
            .staticField("TYPE_LONG_DOUBLE", &_::TYPE_LONG_DOUBLE)
            .staticField("TYPE_BOOL", &_::TYPE_BOOL)
            .staticField("TYPE_NULLPTR_T", &_::TYPE_NULLPTR_T)
            .staticField("TYPE_WCHAR_T", &_::TYPE_WCHAR_T)
            .staticField("TYPE_CHAR16_T", &_::TYPE_CHAR16_T)
            .staticField("TYPE_CHAR32_T", &_::TYPE_CHAR32_T)
            .staticField("TYPE_VOID_PTR", &_::TYPE_VOID_PTR)
            .staticField("TYPE_STD_STRING", &_::TYPE_STD_STRING)
            .staticField("TYPE_STRING", &_::TYPE_STRING)
            .staticMethod<void()>("Register", &_::Register)
            ;
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<TypeKind>().values({
            {"Void",TypeKind::Void},
            {"Bool",TypeKind::Bool},
            {"Char",TypeKind::Char},
            {"SChar",TypeKind::SChar},
            {"Int8",TypeKind::Int8},
            {"UChar",TypeKind::UChar},
            {"UInt8",TypeKind::UInt8},
            {"Short",TypeKind::Short},
            {"Int16",TypeKind::Int16},
            {"WChar",TypeKind::WChar},
            {"Char16",TypeKind::Char16},
            {"UShort",TypeKind::UShort},
            {"UInt16",TypeKind::UInt16},
            {"Int",TypeKind::Int},
            {"Int32",TypeKind::Int32},
            {"Char32",TypeKind::Char32},
            {"UInt",TypeKind::UInt},
            {"UInt32",TypeKind::UInt32},
            {"Long",TypeKind::Long},
            {"ULong",TypeKind::ULong},
            {"Int64",TypeKind::Int64},
            {"UInt64",TypeKind::UInt64},
            {"LongLong",TypeKind::LongLong},
            {"ULongLong",TypeKind::ULongLong},
            {"Float",TypeKind::Float},
            {"Double",TypeKind::Double},
            {"LongDouble",TypeKind::LongDouble},
            {"Enum",TypeKind::Enum},
            {"Pointer",TypeKind::Pointer},
            {"FunctionPointer",TypeKind::FunctionPointer},
            {"NullPtr",TypeKind::NullPtr},
            {"FieldPointer",TypeKind::FieldPointer},
            {"MethodPointer",TypeKind::MethodPointer},
            {"LValueReference",TypeKind::LValueReference},
            {"RValueReference",TypeKind::RValueReference},
            {"Array",TypeKind::Array},
            {"Structure",TypeKind::Structure},
            {"Union",TypeKind::Union},
            {"Class",TypeKind::Class},
            {"StlVectorClass",TypeKind::StlVectorClass},
            {"StlSetClass",TypeKind::StlSetClass},
            {"StlMapClass",TypeKind::StlMapClass},
            {"StringClass",TypeKind::StringClass},
            {"Placeholder",TypeKind::Placeholder},
            {"TemplateDependant",TypeKind::TemplateDependant},
            {"Function",TypeKind::Function},
            {"Unknown",TypeKind::Unknown},
            {"Custom0",TypeKind::Custom0},
            {"Custom1",TypeKind::Custom1},
            {"Custom2",TypeKind::Custom2},
            {"Custom3",TypeKind::Custom3},
            {"Custom4",TypeKind::Custom4},
            {"Custom5",TypeKind::Custom5},
            {"Custom6",TypeKind::Custom6},
            {"Custom7",TypeKind::Custom7},
            {"Custom8",TypeKind::Custom8},
            {"Custom9",TypeKind::Custom9}});
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<OperatorKind>().values({
            {"Binary",OperatorKind::Binary},
            {"UnaryPrefixed",OperatorKind::UnaryPrefixed},
            {"UnaryPostfixed",OperatorKind::UnaryPostfixed},
            {"Ternary",OperatorKind::Ternary},
            {"Postfixed",OperatorKind::Postfixed},
            {"Vararg",OperatorKind::Vararg}});
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<Operator>().values({
            {"Plus",Operator::Plus},
            {"Minus",Operator::Minus},
            {"Add",Operator::Add},
            {"Subtract",Operator::Subtract},
            {"Multiply",Operator::Multiply},
            {"Divide",Operator::Divide},
            {"Address",Operator::Address},
            {"Dereference",Operator::Dereference},
            {"Arrow",Operator::Arrow},
            {"ArrowStar",Operator::ArrowStar},
            {"PreDecrement",Operator::PreDecrement},
            {"PreIncrement",Operator::PreIncrement},
            {"PostDecrement",Operator::PostDecrement},
            {"PostIncrement",Operator::PostIncrement},
            {"Equal",Operator::Equal},
            {"NotEqual",Operator::NotEqual},
            {"Greater",Operator::Greater},
            {"Less",Operator::Less},
            {"GreaterEqual",Operator::GreaterEqual},
            {"LessEqual",Operator::LessEqual},
            {"LogicalAnd",Operator::LogicalAnd},
            {"LogicalOr",Operator::LogicalOr},
            {"XOr",Operator::XOr},
            {"Not",Operator::Not},
            {"BitAnd",Operator::BitAnd},
            {"BitOr",Operator::BitOr},
            {"Complement",Operator::Complement},
            {"Modulo",Operator::Modulo},
            {"Comma",Operator::Comma},
            {"ShiftLeft",Operator::ShiftLeft},
            {"ShiftRight",Operator::ShiftRight},
            {"Assignment",Operator::Assignment},
            {"AssignmentAdd",Operator::AssignmentAdd},
            {"AssignmentSubtract",Operator::AssignmentSubtract},
            {"AssignmentMultiply",Operator::AssignmentMultiply},
            {"AssignmentDivide",Operator::AssignmentDivide},
            {"AssignmentBitAnd",Operator::AssignmentBitAnd},
            {"AssignmentBitOr",Operator::AssignmentBitOr},
            {"AssignmentModulo",Operator::AssignmentModulo},
            {"AssignmentShiftLeft",Operator::AssignmentShiftLeft},
            {"AssignmentShiftRight",Operator::AssignmentShiftRight},
            {"AssignmentXOr",Operator::AssignmentXOr},
            {"Bracket",Operator::Bracket},
            {"Parenthesis",Operator::Parenthesis},
            {"Conversion",Operator::Conversion},
            {"COUNT",Operator::COUNT},
            {"Unknown",Operator::Unknown}});
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<UserDefinedFunctions>().values({
            {"None",UserDefinedFunctions::None},
            {"Implicits",UserDefinedFunctions::Implicits},
            {"Explicits",UserDefinedFunctions::Explicits}});
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<CastKind>().values({
            {"Implicit",CastKind::Implicit},
            {"Const",CastKind::Const},
            {"Reinterpret",CastKind::Reinterpret},
            {"Static",CastKind::Static},
            {"Explicit",CastKind::Explicit}});
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("reflection")
PHANTOM_END("phantom.reflection")
}
}
namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("reflection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(ReflectionMutex)
        {
            this_()
            .constructor<void()>()
            .method<void()>("lock", &_::lock)
            .method<void()>("unlock", &_::unlock)
            .field("m_private", &_::m_private)
            ;
        }
        PHANTOM_REGISTER(Functions) { this_().function<ReflectionMutex&()>("read_mutex", read_mutex);}
        PHANTOM_REGISTER(Functions) { this_().function<ReflectionMutex&()>("write_mutex", write_mutex);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("reflection")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
