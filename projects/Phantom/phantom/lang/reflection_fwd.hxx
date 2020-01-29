#pragma once

// haunt {

#include "reflection_fwd.h"

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
#include <phantom/function>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Package")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Packages>("Packages"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Package")
    PHANTOM_SOURCE("PackageFolder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PackageFolders>("PackageFolders"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PackageFolder")
    PHANTOM_SOURCE("Module")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Modules>("Modules"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Module")
    PHANTOM_SOURCE("Plugin")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Plugins>("Plugins"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Plugin")
    PHANTOM_SOURCE("Application")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Applications>("Applications"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Application")
    PHANTOM_SOURCE("SourceFile")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<SourceFiles>("SourceFiles"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SourceFile")
    PHANTOM_SOURCE("SourceStream")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<SourceStreams>("SourceStreams"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SourceStream")
    PHANTOM_SOURCE("Source")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Sources>("Sources"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Source")
    PHANTOM_SOURCE("LanguageElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<LanguageElements>("LanguageElements"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElement")
    PHANTOM_SOURCE("Signature")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Signatures>("Signatures"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Signature")
    PHANTOM_SOURCE("Subroutine")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Subroutines>("Subroutines"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Subroutine")
    PHANTOM_SOURCE("LocalVariable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<LocalVariables>("LocalVariables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LocalVariable")
    PHANTOM_SOURCE("Parameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Parameters>("Parameters"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Parameter")
    PHANTOM_SOURCE("Function")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Functions>("Functions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Function")
    PHANTOM_SOURCE("Constant")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Constants>("Constants"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Constant")
    PHANTOM_SOURCE("Variable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Variables>("Variables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Variable")
    PHANTOM_SOURCE("Constructor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Constructors>("Constructors"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Constructor")
    PHANTOM_SOURCE("Destructor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Destructors>("Destructors"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Destructor")
    PHANTOM_SOURCE("Method")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Methods>("Methods"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Method")
    PHANTOM_SOURCE("Signal")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Signals>("Signals"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Signal")
    PHANTOM_SOURCE("Field")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Fields>("Fields"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Field")
    PHANTOM_SOURCE("DataElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<DataElements>("DataElements"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DataElement")
    PHANTOM_SOURCE("AnonymousSection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<AnonymousSections>("AnonymousSections"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousSection")
    PHANTOM_SOURCE("AnonymousStruct")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<AnonymousStructs>("AnonymousStructs"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousStruct")
    PHANTOM_SOURCE("AnonymousUnion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<AnonymousUnions>("AnonymousUnions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousUnion")
    PHANTOM_SOURCE("MemberAnonymousSection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MemberAnonymousSections>("MemberAnonymousSections"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberAnonymousSection")
    PHANTOM_SOURCE("MemberAnonymousStruct")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MemberAnonymousStructs>("MemberAnonymousStructs"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberAnonymousStruct")
    PHANTOM_SOURCE("MemberAnonymousUnion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MemberAnonymousUnions>("MemberAnonymousUnions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberAnonymousUnion")
    PHANTOM_SOURCE("FunctionType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctionTypes>("FunctionTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionType")
    PHANTOM_SOURCE("MethodPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MethodPointers>("MethodPointers"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MethodPointer")
    PHANTOM_SOURCE("FieldPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FieldPointers>("FieldPointers"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldPointer")
    PHANTOM_SOURCE("FunctionPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctionPointers>("FunctionPointers"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctionPointer")
    PHANTOM_SOURCE("InitializerListType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<InitializerListTypes>("InitializerListTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("InitializerListType")
    PHANTOM_SOURCE("Placeholder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Placeholders>("Placeholders"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Placeholder")
    PHANTOM_SOURCE("PlaceholderType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PlaceholderTypes>("PlaceholderTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderType")
    PHANTOM_SOURCE("PlaceholderClassType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PlaceholderClassTypes>("PlaceholderClassTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderClassType")
    PHANTOM_SOURCE("PlaceholderClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PlaceholderClasses>("PlaceholderClasses"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderClass")
    PHANTOM_SOURCE("StaticField")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticField>("StaticField"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticFields>("StaticFields"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StaticField")
    PHANTOM_SOURCE("StaticVariable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticVariable>("StaticVariable"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticVariables>("StaticVariables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StaticVariable")
    PHANTOM_SOURCE("StaticMethod")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticMethod>("StaticMethod"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StaticMethods>("StaticMethods"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StaticMethod")
    PHANTOM_SOURCE("Property")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Properties>("Properties"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Property")
    PHANTOM_SOURCE("Alias")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Aliases>("Aliases"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Alias")
    PHANTOM_SOURCE("Class")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Classes>("Classes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Class")
    PHANTOM_SOURCE("ValueMember")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ValueMembers>("ValueMembers"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ValueMember")
    PHANTOM_SOURCE("LanguageElementVisitor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<LanguageElementVisitors>("LanguageElementVisitors"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElementVisitor")
    PHANTOM_SOURCE("Scope")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Scopes>("Scopes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Scope")
    PHANTOM_SOURCE("Type")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Types>("Types"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Type")
    PHANTOM_SOURCE("Callable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Callables>("Callables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Callable")
    PHANTOM_SOURCE("Enum")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Enums>("Enums"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Enum")
    PHANTOM_SOURCE("ClassType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ClassTypes>("ClassTypes"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ClassType")
    PHANTOM_SOURCE("Structure")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Structures>("Structures"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Structure")
    PHANTOM_SOURCE("Union")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Unions>("Unions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Union")
    PHANTOM_SOURCE("VirtualMethodTable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<VirtualMethodTables>("VirtualMethodTables"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VirtualMethodTable")
    PHANTOM_SOURCE("Namespace")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Namespaces>("Namespaces"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Namespace")
    PHANTOM_SOURCE("TemplateSpecialization")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<TemplateSpecializations>("TemplateSpecializations"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateSpecialization")
    PHANTOM_SOURCE("TemplateParameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<TemplateParameters>("TemplateParameters"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateParameter")
    PHANTOM_SOURCE("Template")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Templates>("Templates"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Template")
    PHANTOM_SOURCE("Symbol")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Symbols>("Symbols"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Symbol")
    PHANTOM_SOURCE("Instruction")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Instructions>("Instructions"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Instruction")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctorPropertys>("FunctorPropertys"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
    PHANTOM_SOURCE("Aggregate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Aggregates>("Aggregates"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Aggregate")
    PHANTOM_SOURCE("FunctorProperty")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<FunctorProperties>("FunctorProperties"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorProperty")
    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<LanguageElementsView>("LanguageElementsView"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<TypesView>("TypesView"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.lang")
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("reflection_fwd")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<CallDelegate>("CallDelegate"); }
        PHANTOM_REGISTER(Functions) { this_().function<void(ExecutionContext&, void**, ::size_t)>("SilentCallDelegate", SilentCallDelegate);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("reflection_fwd")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
