// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "LanguageElementVisitor.h"

#include "Alias.h"
#include "AnonymousSection.h"
#include "AnonymousStruct.h"
#include "AnonymousUnion.h"
#include "Application.h"
#include "Array.h"
#include "Class.h"
#include "ClassType.h"
#include "ConstType.h"
#include "ConstVolatileType.h"
#include "Constant.h"
#include "Constructor.h"
#include "ContainerClass.h"
#include "Destructor.h"
#include "Enum.h"
#include "ExtendedType.h"
#include "Field.h"
#include "FieldPointer.h"
#include "Function.h"
#include "FunctionPointer.h"
#include "FunctionType.h"
#include "FundamentalsT.h"
#include "InitializerListType.h"
#include "LValueReference.h"
#include "LanguageElement.h"
#include "LocalVariable.h"
#include "MapClass.h"
#include "MemberAnonymousSection.h"
#include "MemberAnonymousStruct.h"
#include "MemberAnonymousUnion.h"
#include "MemberPointer.h"
#include "Method.h"
#include "MethodPointer.h"
#include "Module.h"
#include "Namespace.h"
#include "Package.h"
#include "PackageFolder.h"
#include "Parameter.h"
#include "Placeholder.h"
#include "PlaceholderClass.h"
#include "PlaceholderClassType.h"
#include "PlaceholderConstant.h"
#include "PlaceholderTemplate.h"
#include "PlaceholderType.h"
#include "Pointer.h"
#include "PointerType.h"
#include "PrimitiveType.h"
#include "Property.h"
#include "RValueReference.h"
#include "Reference.h"
#include "Scope.h"
#include "SequentialContainerClass.h"
#include "SetClass.h"
#include "Signal.h"
#include "Signature.h"
#include "Source.h"
#include "Structure.h"
#include "Subroutine.h"
#include "Symbol.h"
#include "Template.h"
#include "TemplateDependantArray.h"
#include "TemplateDependantElement.h"
#include "TemplateDependantTemplateInstance.h"
#include "TemplateDependantType.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
#include "Type.h"
#include "Union.h"
#include "ValueMember.h"
#include "Variable.h"
#include "VectorClass.h"
#include "VirtualMethodTable.h"
#include "VolatileType.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
void LanguageElementVisitor::visit(Alias* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(AnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(AnonymousStruct* a_pInput, VisitorData a_Data)
{
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(AnonymousUnion* a_pInput, VisitorData a_Data)
{
    visit(static_cast<AnonymousSection*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Application* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Array* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Class* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ClassType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Constant* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Constructor* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Method*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ConstType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ConstVolatileType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ContainerClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Field* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FieldPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberPointer*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Pointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Destructor* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Method*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Ellipsis*, VisitorData) {}
void LanguageElementVisitor::visit(Enum* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Function* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Subroutine*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FunctionPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FunctionType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(InitializerListType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(LanguageElement*, VisitorData) {}
void LanguageElementVisitor::visit(LocalVariable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(LValueReference* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MapClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Class*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MemberAnonymousSection* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberAnonymousSection*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Method* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Subroutine*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MethodPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<MemberPointer*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(MemberPointer* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Module* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Namespace* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Package* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PackageFolder* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Parameter* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LocalVariable*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Placeholder*, VisitorData) {}
void LanguageElementVisitor::visit(PlaceholderClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Class*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PlaceholderClassType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PlaceholderConstant* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Constant*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PlaceholderTemplate* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Template*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PlaceholderType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ExtendedType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PointerType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(PrimitiveType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Property* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ValueMember*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Reference* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ExtendedType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(RValueReference* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Reference*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Scope*, VisitorData) {}
void LanguageElementVisitor::visit(SequentialContainerClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Class*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(SetClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ContainerClass*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(VectorClass* a_pInput, VisitorData a_Data)
{
    visit(static_cast<SequentialContainerClass*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Signal* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Method*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Signature* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Source* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Structure* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Subroutine* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Symbol* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Template* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateDependantArray* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Array*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateDependantElement* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateDependantType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateParameter* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateSignature* a_pInput, VisitorData a_Data)
{
    visit(static_cast<LanguageElement*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(TemplateSpecialization* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Type* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Union* a_pInput, VisitorData a_Data)
{
    visit(static_cast<ClassType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(ValueMember* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(Variable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(VirtualMethodTable* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Symbol*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(VolatileType* a_pInput, VisitorData a_Data)
{
    visit(static_cast<Type*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}
void LanguageElementVisitor::visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data)
{
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
}

PHANTOM_IF_WCHAR_T(void LanguageElementVisitor::visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data) {
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
});
PHANTOM_IF_CHAR16_T(void LanguageElementVisitor::visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data) {
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
});
PHANTOM_IF_CHAR32_T(void LanguageElementVisitor::visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data) {
    visit(static_cast<PrimitiveType*>(a_pInput), a_Data);
});

} // namespace lang
} // namespace phantom
