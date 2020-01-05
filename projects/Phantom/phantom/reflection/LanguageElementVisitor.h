// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/core.h>
#include <phantom/reflection/reflection.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Visitor data (used in LanguageElementVisitor visitor functions).
struct VisitorData
{
    VisitorData() : id(0), in(0), out(0), flags(0)
    {
    }
    VisitorData(int aid, const void** ain = nullptr, void** aout = nullptr, int aflags = 0)
        : id(aid), in(ain), out(aout), flags(aflags)
    {
    }
    int          id;
    const void** in;
    void**       out;
    int          flags;
    bool         hasFlag(int f) const
    {
        return (flags & f) == f;
    }
};

/// \brief  A visitor base class to visit every native language elements (useful for
/// (pre)compilers).

class PHANTOM_EXPORT_PHANTOM LanguageElementVisitor
{
public:
    virtual void visit(Alias* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(AnonymousUnion* a_pInput, VisitorData a_Data);
    virtual void visit(Application* a_pInput, VisitorData a_Data);
    virtual void visit(Array* a_pInput, VisitorData a_Data);
    virtual void visit(Class* a_pInput, VisitorData a_Data);
    virtual void visit(ClassType* a_pInput, VisitorData a_Data);
    virtual void visit(Constant* a_pInput, VisitorData a_Data);
    virtual void visit(Constructor* a_pInput, VisitorData a_Data);
    virtual void visit(ConstType* a_pInput, VisitorData a_Data);
    virtual void visit(ConstVolatileType* a_pInput, VisitorData a_Data);
    virtual void visit(ContainerClass* a_pInput, VisitorData a_Data);
    virtual void visit(Field* a_pInput, VisitorData a_Data);
    virtual void visit(FieldPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Pointer* a_pInput, VisitorData a_Data);
    virtual void visit(Destructor* a_pInput, VisitorData a_Data);
    virtual void visit(Ellipsis* a_pInput, VisitorData a_Data);
    virtual void visit(Enum* a_pInput, VisitorData a_Data);
    virtual void visit(Function* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionPointer* a_pInput, VisitorData a_Data);
    virtual void visit(FunctionType* a_pInput, VisitorData a_Data);
    virtual void visit(InitializerListType* a_pInput, VisitorData a_Data);
    virtual void visit(LanguageElement* a_pInput, VisitorData a_Data);
    virtual void visit(LocalVariable* a_pInput, VisitorData a_Data);
    virtual void visit(LValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(StlMapClass* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousSection* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousStruct* a_pInput, VisitorData a_Data);
    virtual void visit(MemberAnonymousUnion* a_pInput, VisitorData a_Data);

    virtual void visit(Method* a_pInput, VisitorData a_Data);
    virtual void visit(MethodPointer* a_pInput, VisitorData a_Data);
    virtual void visit(MemberPointer* a_pInput, VisitorData a_Data);
    virtual void visit(Module* a_pInput, VisitorData a_Data);
    virtual void visit(Namespace* a_pInput, VisitorData a_Data);
    virtual void visit(Package* a_pInput, VisitorData a_Data);
    virtual void visit(PackageFolder* a_pInput, VisitorData a_Data);
    virtual void visit(Parameter* a_pInput, VisitorData a_Data);
    virtual void visit(Placeholder* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClass* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderClassType* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderConstant* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderTemplate* a_pInput, VisitorData a_Data);
    virtual void visit(PlaceholderType* a_pInput, VisitorData a_Data);
    virtual void visit(PointerType* a_pInput, VisitorData a_Data);
    virtual void visit(ExtendedType* a_pInput, VisitorData a_Data);
    virtual void visit(PrimitiveType* a_pInput, VisitorData a_Data);
    virtual void visit(Property* a_pInput, VisitorData a_Data);
    virtual void visit(Reference* a_pInput, VisitorData a_Data);
    virtual void visit(RValueReference* a_pInput, VisitorData a_Data);
    virtual void visit(Scope* a_pInput, VisitorData a_Data);
    virtual void visit(SequentialContainerClass* a_pInput, VisitorData a_Data);
    virtual void visit(StlSetClass* a_pInput, VisitorData a_Data);
    virtual void visit(StlVectorClass* a_pInput, VisitorData a_Data);
    virtual void visit(Signal* a_pInput, VisitorData a_Data);
    virtual void visit(Signature* a_pInput, VisitorData a_Data);
    virtual void visit(Source* a_pInput, VisitorData a_Data);
    virtual void visit(Structure* a_pInput, VisitorData a_Data);
    virtual void visit(Subroutine* a_pInput, VisitorData a_Data);
    virtual void visit(Symbol* a_pInput, VisitorData a_Data);
    virtual void visit(Template* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantArray* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantElement* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantTemplateInstance* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateDependantType* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateParameter* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSignature* a_pInput, VisitorData a_Data);
    virtual void visit(TemplateSpecialization* a_pInput, VisitorData a_Data);
    virtual void visit(Type* a_pInput, VisitorData a_Data);
    virtual void visit(Union* a_pInput, VisitorData a_Data);
    virtual void visit(ValueMember* a_pInput, VisitorData a_Data);
    virtual void visit(Variable* a_pInput, VisitorData a_Data);
    virtual void visit(VirtualMethodTable* a_pInput, VisitorData a_Data);
    virtual void visit(VolatileType* a_pInput, VisitorData a_Data);

    virtual void visit(FundamentalTypeT<void>* a_pInput, VisitorData a_Data);
    virtual void visit(FundamentalTypeT<std::nullptr_t>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<bool>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<char>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<uchar>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<schar>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<short>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ushort>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<int>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<uint>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<long>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ulong>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<longlong>* a_pInput, VisitorData a_Data);
    virtual void visit(IntegralTypeT<ulonglong>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<float>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<double>* a_pInput, VisitorData a_Data);
    virtual void visit(FloatingPointTypeT<longdouble>* a_pInput, VisitorData a_Data);

    PHANTOM_IF_WCHAR_T(virtual void visit(IntegralTypeT<wchar_t>* a_pInput, VisitorData a_Data);)
    PHANTOM_IF_CHAR16_T(virtual void visit(IntegralTypeT<char16_t>* a_pInput, VisitorData a_Data);)
    PHANTOM_IF_CHAR32_T(virtual void visit(IntegralTypeT<char32_t>* a_pInput, VisitorData a_Data);)
};

} // namespace reflection
} // namespace phantom
