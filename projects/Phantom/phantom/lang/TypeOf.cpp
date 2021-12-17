// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "TypeOf.h"

#include "registration/registration.h"

#include <phantom/lang/Source.h>

namespace phantom
{
namespace lang
{
namespace
{
bool g_LogTypeOfByName = false;
}
PHANTOM_EXPORT_PHANTOM void TypeOfByNameLogFailed(StringView _qn)
{
    PHANTOM_LOG(Warning,
                "cannot find type '%.*s' in any module, ensure it has lang "
                "declared or, if it is a template instance, ensure the lang is "
                "available above in the compilation unit (.hxx included for example)",
                PHANTOM_STRING_AS_PRINTF_ARG(_qn));
}

PHANTOM_EXPORT_PHANTOM void TypeOfByNameLogModuleFailed(StringView _qn, StringView _foundModule, StringView _currModule)
{
    PHANTOM_LOG(Warning,
                "type '%.*s' was found in module '%.*s' which is not a dependency of the current "
                "module '%.*s'. This can cause undefined behavior when unloading modules.",
                PHANTOM_STRING_AS_PRINTF_ARG(_qn), PHANTOM_STRING_AS_PRINTF_ARG(_foundModule),
                PHANTOM_STRING_AS_PRINTF_ARG(_currModule));
}

PHANTOM_EXPORT_PHANTOM bool TypeOfByNameLogEnabled()
{
    return g_LogTypeOfByName;
}

PHANTOM_EXPORT_PHANTOM void TypeOfByNameLogEnabled(bool _enabled)
{
    g_LogTypeOfByName = _enabled;
}

PHANTOM_EXPORT_PHANTOM FunctionType* functionType(Type* a_pReturnType, TypesView a_ParameterTypes,
                                                  Modifiers a_Qualifiers)
{
    return detail::currentSource()->functionType(a_pReturnType, a_ParameterTypes, a_Qualifiers, PHANTOM_R_FLAG_NATIVE);
}

PHANTOM_EXPORT_PHANTOM FunctionPointer* functionPointerType(Type* a_pReturnType, int a_eABI, TypesView a_ParameterTypes)
{
    return detail::currentSource()->functionPointerType(a_pReturnType, (ABI)a_eABI, a_ParameterTypes, PHANTOM_R_NONE,
                                                        PHANTOM_R_FLAG_NATIVE);
}

PHANTOM_EXPORT_PHANTOM InitializerListType* initializerListType(TypesView a_Types)
{
    return detail::currentSource()->initializerListType(a_Types);
}

PHANTOM_EXPORT_PHANTOM MethodPointer* methodPointerType(ClassType* a_pObjectType, Type* a_pReturnType,
                                                        TypesView a_ParameterTypes, Modifiers a_Qualifiers)
{
    return detail::currentSource()->methodPointerType(a_pObjectType, a_pReturnType, a_ParameterTypes, a_Qualifiers,
                                                      PHANTOM_R_FLAG_NATIVE);
}

PHANTOM_EXPORT_PHANTOM FieldPointer* fieldPointerType(ClassType* a_pObjectType, Type* a_pValueType)
{
    return detail::currentSource()->fieldPointerType(a_pObjectType, a_pValueType, PHANTOM_R_NONE);
}

PHANTOM_EXPORT_PHANTOM FunctionPointer* functionPointerType(FunctionType* a_pFunctionType, ABI a_eABI)
{
    return detail::currentSource()->functionPointerType(a_pFunctionType, a_eABI, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);
}

#if defined(PHANTOM_SAFE_SYMBOL_CAST)
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Type* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(PrimitiveType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Namespace* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Enum* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Class* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Structure* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(MethodPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(FieldPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(FunctionPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Pointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(LValueReference* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(RValueReference* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(ConstType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(ConstVolatileType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(VolatileType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Array* a_pElement)
{
    return a_pElement;
}

PHANTOM_EXPORT_PHANTOM Type* type_cast(PrimitiveType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(Enum* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(Class* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(Structure* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(MethodPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(FieldPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(FunctionPointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(Pointer* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(LValueReference* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(RValueReference* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(ConstType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(ConstVolatileType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(VolatileType* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(Array* a_pElement)
{
    return a_pElement;
}

#endif
} // namespace lang
} // namespace phantom
