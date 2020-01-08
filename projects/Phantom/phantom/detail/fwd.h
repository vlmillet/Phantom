// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <cstdarg> // for 'va_list'
#include <ctime>   // for 'size_t'
#include <haunt>
#include <phantom/plugin.h>
#include <utility>

#define PHANTOM_DEF_LIST(type, ...) typedef phantom::SmallVector<type*, ##__VA_ARGS__> type##s;
#define PHANTOM_DEF_LIST_S(type, ...) typedef phantom::SmallVector<type*, ##__VA_ARGS__> type##es;
#define PHANTOM_DECL_LISTABLE(type, ...)                                                                               \
    HAUNT_SOURCE(#type) HAUNT_OFF class type;                                                                          \
    PHANTOM_DEF_LIST(type, ##__VA_ARGS__) HAUNT_END()
#define PHANTOM_DECL_LISTABLE_S(type, ...)                                                                             \
    HAUNT_SOURCE(#type) HAUNT_OFF class type;                                                                          \
    PHANTOM_DEF_LIST_S(type, ##__VA_ARGS__) HAUNT_END()
#define PHANTOM_DECL_TD_LISTABLE(origin, type, ...)                                                                    \
    HAUNT_SOURCE(#type) typedef origin type;                                                                           \
    typedef origin##s                  type##s;                                                                        \
    HAUNT_END()

namespace phantom
{
HAUNT_PAUSE;

template<class T>
class BasicStringView;

template<class S>
class Delegate;

template<class T, ::size_t StaticAllocSize, ::size_t DynamicAllocInc>
class SmallString;

template<class>
struct Less;

template<class T, ::size_t StaticAllocSize = 4, ::size_t DynamicAllocInc = 4, class Pred = Less<T>>
class SmallSet;

template<class K, class V, ::size_t StaticAllocSize = 4, ::size_t DynamicAllocInc = 4, class Pred = Less<K>>
class SmallMap;

template<class K, class V, ::size_t StaticAllocSize = 4, ::size_t DynamicAllocInc = 4, class Pred = Less<K>>
class SmallMultimap;

template<class T, ::size_t StaticAllocSize = 4, ::size_t DynamicAllocInc = 4>
class SmallVector;

template<class T>
class ArrayView;

template<class T>
class Optional;

/// @cond ADVANCED
class Phantom;

template<class S>
class Delegate;

class Variant;
class DummyClass
{
};

class Object;

template<class>
struct Allocator;

template<class>
struct Copier;

template<class t_Ty>
struct DynamicDeleter;

template<class t_Ty>
struct DefaultAllocator;

template<class t_Ty>
struct DefaultConstructor;

template<class t_Ty>
struct DefaultInitializer;

template<class t_Ty>
struct DefaultInstaller;

template<class>
struct NewMetaH;

template<class>
struct DeleteMetaH;

template<class t_Ty>
struct DynamicDeleteExH;

template<class t_Ty>
struct DynamicDeleteMetaHelper;

namespace detail
{
class DynamicCppInitializerH;

template<class t_Ty, int>
struct DefaultInstallerH;

template<class t_Ty, int>
struct DefaultInitializerH;

} // namespace detail

struct Path;

HAUNT_RESUME;

typedef SmallVector<Variant, 10, 10> Variants;

/// @endcond

namespace reflection
{
HAUNT_PACKAGE("phantom.reflection");
// Listables

PHANTOM_DECL_LISTABLE(Package);
PHANTOM_DECL_LISTABLE(PackageFolder);
PHANTOM_DECL_LISTABLE(Module);
PHANTOM_DECL_LISTABLE(Plugin);
PHANTOM_DECL_LISTABLE(Application);
PHANTOM_DECL_LISTABLE(SourceFile);
PHANTOM_DECL_LISTABLE(SourceStream);
struct CodeRangeLocation;
PHANTOM_DECL_LISTABLE(Source);
PHANTOM_DECL_LISTABLE(LanguageElement, 20);
PHANTOM_DECL_LISTABLE(Signature);
PHANTOM_DECL_LISTABLE(Subroutine);
PHANTOM_DECL_LISTABLE(LocalVariable, 5);
PHANTOM_DECL_LISTABLE(Parameter, 3);
PHANTOM_DECL_LISTABLE(Function);
PHANTOM_DECL_LISTABLE(Constant);
PHANTOM_DECL_LISTABLE(Variable);
PHANTOM_DECL_LISTABLE(Constructor);
PHANTOM_DECL_LISTABLE(Destructor);
PHANTOM_DECL_LISTABLE(Method, 20); // member functions are generally several stored or required
PHANTOM_DECL_LISTABLE(Signal, 5);
PHANTOM_DECL_LISTABLE(Field, 10);
PHANTOM_DECL_LISTABLE(DataElement, 10);
PHANTOM_DECL_LISTABLE(AnonymousSection);
PHANTOM_DECL_LISTABLE(AnonymousStruct);
PHANTOM_DECL_LISTABLE(AnonymousUnion);
PHANTOM_DECL_LISTABLE(MemberAnonymousSection);
PHANTOM_DECL_LISTABLE(MemberAnonymousStruct);
PHANTOM_DECL_LISTABLE(MemberAnonymousUnion);

PHANTOM_DECL_LISTABLE(FunctionType);
PHANTOM_DECL_LISTABLE(MethodPointer);
PHANTOM_DECL_LISTABLE(FieldPointer);
PHANTOM_DECL_LISTABLE(FunctionPointer);
PHANTOM_DECL_LISTABLE(InitializerListType);

PHANTOM_DECL_LISTABLE(Placeholder);
PHANTOM_DECL_LISTABLE(PlaceholderType);
PHANTOM_DECL_LISTABLE(PlaceholderClassType);
PHANTOM_DECL_LISTABLE_S(PlaceholderClass);

PHANTOM_DECL_TD_LISTABLE(Variable, StaticField);
PHANTOM_DECL_TD_LISTABLE(Variable, StaticVariable);
PHANTOM_DECL_TD_LISTABLE(Function, StaticMethod);

class Property;

HAUNT_SOURCE("Property")
using Properties = SmallVector<Property*>;
HAUNT_END("Property")

/// @cond ADVANCED

HAUNT_PAUSE;
class ExecutionContext;

class PersistentElement;
struct VisitorData;
struct CodePosition;

class CodeGenerator;
class Debugger;
class Parser;
class Semantic;
class Translator;
class Interpreter;

class ContainerClass;
class SequentialContainerClass;
class MapClass;
class SetClass;
class VectorClass;
class StringClass;
class Ellipsis;
class PointerType;
class MemberPointer;
class PrimitiveType;
class ConstType;
class VolatileType;
class ConstVolatileType;
class Reference;
class LValueReference;
class RValueReference;
class ExtendedType;
class Pointer;
class Array;

class TemplateDependantArray;
class TemplateDependantElement;
class TemplateDependantTemplateInstance;
class TemplateDependantType;
class TemplateSignature;

struct BuiltInTypes;
class PlaceholderTemplate;
class PlaceholderConstant;
class Property;

template<class, class, class, class>
class PropertyT; ///< @off

template<class t_Ty>
struct TypeInfosOf;

template<class t_Ty>
struct TypeOf;

template<class t_Ty>
class EnumT;

template<class t_Ty>
class MethodPointerT;
template<class t_Ty>
class FieldPointerT;
template<class t_Ty>
class FunctionPointerT;
template<typename t_Signature>
class FunctionProviderT;

namespace detail
{
template<class t_Ty, bool t_is_default_constructible>
struct VTableSizeOfDefaultConstructibleH;
}

template<class>
class PrimitiveTypeT;
template<class>
class FundamentalTypeT;
template<class>
class ArithmeticTypeT;
template<class>
class IntegralTypeT;
template<class>
class FloatingPointTypeT;

template<class>
class FunctionT;
template<class, class>
class SignalMethodT;
template<class, class>
class ConstructorT;
template<class>
class DestructorT;
template<class>
class MethodT;
template<class>
struct SignatureH;

template<class T = size_t>
class FieldT;
template<class>
class VariableT;

template<class, class>
class ContainerClassT;
template<class, class>
class MapClassT;
template<class, class>
class SequentialContainerClassT;
template<class, class>
class VectorClassT;
template<class, class>
class SetClassT;
template<class>
class ConstantT;

namespace detail
{
struct TemplateRegistrer;

template<class t_Ty>
struct TypeOfBuilder;

template<class t_Ty, class t_STy>
struct BaseClassRegistrer;

template<class t_Ty, int t_Kind>
struct TypeRegistrerSelector;

} // namespace detail

template<class T, class Signature>
class SignalT;

HAUNT_RESUME;

HAUNT_END("phantom.reflection");

using CallDelegate = Delegate<void(ExecutionContext&, void**, ::size_t)>;
inline void SilentCallDelegate(ExecutionContext& /*unused*/, void** /*unused*/, ::size_t /*unused*/) {}

class Evaluable;
class Expression;
class Statement;
class Block;

/// @endcond

} // namespace reflection

} // namespace phantom

#include "fwd2.h"
