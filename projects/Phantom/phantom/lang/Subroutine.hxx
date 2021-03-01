#pragma once

// haunt {

// clang-format off

#include "Subroutine.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Functor.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Subroutine")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<BlockBuilder>("BlockBuilder"); }
        PHANTOM_STRUCT(MemoryLocation)
        {
            this_()
            .constructor<void()>()
            .constructor<void(byte*, byte*)>()
            .method<bool(const MemoryLocation&) const>("operator==", &_::operator==)
            .method<bool(const MemoryLocation&) const>("operator<", &_::operator<)
            .method<bool() const>("isValid", &_::isValid)
            .method<byte*() const>("getStart", &_::getStart)
            .method<byte*() const>("getEnd", &_::getEnd)
            .method<void(byte*)>("setStart", &_::setStart)
            .method<void(byte*)>("setEnd", &_::setEnd)
            .method<bool(const byte*) const>("containsMemoryAddress", &_::containsMemoryAddress)
            .method<size_t() const>("getSize", &_::getSize)
        
        .protected_()
            .field("m_pStart", &_::m_pStart)
            .field("m_pEnd", &_::m_pEnd)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_TV((class), (R), (class, Args), SubroutineCallH)
        {
            this_()
            .PHANTOM_T staticMethod<R(Subroutine const*, Args&&...)>("call", &_::call)
            ;
        }
        PHANTOM_STRUCT_VS((class, Args), (void, Args...), SubroutineCallH)
        {
            this_()
            .PHANTOM_T staticMethod<void(Subroutine const*, Args&&...)>("call", &_::call)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Instruction)
        {
            this_()
        
        .public_()
            .constructor<void(int, const CodeRange&, const MemoryLocation&, void*)>()["MemoryLocation()"]["nullptr"]
            .method<bool(const Instruction&) const>("operator<", &_::operator<)
            .method<Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<void(void*) const>("setUserData", &_::setUserData)
            .method<void*() const>("getUserData", &_::getUserData)
            .method<int() const>("getOpCode", &_::getOpCode)
            .method<const ::phantom::lang::CodeRange &() const>("getCodeRange", &_::getCodeRange)
            .method<void(const CodeRange&)>("setCodeRange", &_::setCodeRange)
            .method<const ::phantom::lang::MemoryLocation &() const>("getMemoryLocation", &_::getMemoryLocation)
            .method<Source*() const>("getSource", &_::getSource)
            .method<byte*() const>("getMemoryStart", &_::getMemoryStart)
            .method<byte*() const>("getMemoryEnd", &_::getMemoryEnd)
            .method<bool(const CodePosition&)>("containsCodePosition", &_::containsCodePosition)
            .method<bool(const byte*)>("containsMemoryAddress", &_::containsMemoryAddress)
            .method<void(const MemoryLocation&)>("setMemoryLocation", &_::setMemoryLocation)
        
        .protected_()
            .field("m_pSubroutine", &_::m_pSubroutine)
            .field("m_iOpcode", &_::m_iOpcode)
            .field("m_CodeRange", &_::m_CodeRange)
            .field("m_MemoryLocation", &_::m_MemoryLocation)
            .field("m_pUserData", &_::m_pUserData)
            ;
        }
        PHANTOM_REGISTER(Enums) { this_().enum_<ESignatureRelation>().values({
            {"e_SignatureRelation_None",e_SignatureRelation_None},
            {"e_SignatureRelation_Equal",e_SignatureRelation_Equal},
            {"e_SignatureRelation_Covariant",e_SignatureRelation_Covariant},
            {"e_SignatureRelation_Contravariant",e_SignatureRelation_Contravariant},
            {"e_SignatureRelation_Forbidden",e_SignatureRelation_Forbidden}});
        }
        PHANTOM_CLASS(Subroutine)
        {
            using ApplyPointer = typedef_<_::ApplyPointer>;
            using Instructions = typedef_< phantom::lang::Instructions>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Parameters = typedef_< phantom::lang::Parameters>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using StringViews = typedef_< phantom::StringViews>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()
            .inherits<::phantom::lang::Symbol, ::phantom::lang::Callable>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<ApplyPointer>("ApplyPointer")
        
        .public_()
            .method<void(), virtual_>("terminate", &_::terminate)
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<ABI() const>("getABI", &_::getABI)
            .method<::phantom::lang::Callable *() const, virtual_>("asCallable", &_::asCallable)
            .method<Subroutine*() const, virtual_|override_>("asSubroutine", &_::asSubroutine)
            .method<::phantom::lang::Signature *() const>("getSignature", &_::getSignature)
            .method<ESignatureRelation(StringView, Signature*, Modifiers, uint) const>("getSignatureRelationWith", &_::getSignatureRelationWith)["0"]
            .method<ESignatureRelation(Subroutine*) const>("getSignatureRelationWith", &_::getSignatureRelationWith)
            .method<ESignatureRelation(Type*, StringView, TypesView, Modifiers, uint) const>("getSignatureRelationWith", &_::getSignatureRelationWith)["0"]
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getRelativeDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<Types() const, virtual_>("getParameterTypes", &_::getParameterTypes)
            .method<::phantom::lang::Type *(size_t) const, virtual_>("getParameterType", &_::getParameterType)
            .method<Parameters const&() const>("getParameters", &_::getParameters)
            .method<::phantom::lang::Parameter *(StringView) const, virtual_>("getParameter", &_::getParameter)
            .method<size_t() const, virtual_>("getRequiredArgumentCount", &_::getRequiredArgumentCount)
            .method<void(ArrayView<StringView>)>("setNativeDefaultArgumentStrings", &_::setNativeDefaultArgumentStrings)
            .method<StringViews() const>("getNativeDefaultArgumentStrings", &_::getNativeDefaultArgumentStrings)
            .method<bool(TypesView, Modifiers) const, virtual_>("matches", &_::matches)["Modifiers()"]
            .method<bool(const LanguageElements&, TypesView, Modifiers) const, virtual_>("matches", &_::matches)["Modifiers()"]
            .method<bool(StringView, TypesView, Modifiers) const>("matches", &_::matches)["Modifiers()"]
            .method<Type*() const>("getReturnType", &_::getReturnType)
            .method<::phantom::OpaqueDelegate() const, virtual_>("getOpaqueDelegate", &_::getOpaqueDelegate)
            .method<::phantom::OpaqueDelegate(void*) const, virtual_>("getOpaqueDelegate", &_::getOpaqueDelegate)
            .method<void(ExecutionContext&, void**) const, virtual_>("call", &_::call)
            .method<void(ExecutionContext&, void**, Types const&) const, virtual_>("callVarArg", &_::callVarArg)
            .method<void(void*, void**, void*) const, virtual_>("placementInvoke", &_::placementInvoke)
            .method<void(ExecutionContext&, void**) const, virtual_>("placementCall", &_::placementCall)
            .method<void(ExecutionContext&, void**, Types const&) const, virtual_>("placementCallVarArg", &_::placementCallVarArg)
            .method<void(void**) const, virtual_|override_>("call", &_::call)
            .method<void(void**, Types const&) const, virtual_>("callVarArg", &_::callVarArg)
            .method<void(void**, void*) const, virtual_|override_>("call", &_::call)
            .method<void(void**, Types const&, void*) const, virtual_>("callVarArg", &_::callVarArg)
            .method<void(void**, void*) const, virtual_>("placementCall", &_::placementCall)
            .method<void(void**, Types const&, void*) const, virtual_>("placementCallVarArg", &_::placementCallVarArg)
            .method<bool(void*) const, virtual_>("equalsPointer", &_::equalsPointer)
            .method<void(const MemoryLocation&)>("setMemoryLocation", &_::setMemoryLocation)
            .method<const ::phantom::lang::MemoryLocation &() const>("getMemoryLocation", &_::getMemoryLocation)
            .method<byte*() const>("getMemoryStart", &_::getMemoryStart)
            .method<byte*() const>("getMemoryEnd", &_::getMemoryEnd)
            .method<void(byte*)>("setMemoryStart", &_::setMemoryStart)
            .method<void(byte*)>("setMemoryEnd", &_::setMemoryEnd)
            .method<Instruction*(int, const CodeRangeLocation&, const MemoryLocation&, void*)>("createInstruction", &_::createInstruction)["MemoryLocation()"]["nullptr"]
            .method<void(Instruction*)>("deleteInstruction", &_::deleteInstruction)
            .method<Instruction*(size_t) const>("getInstruction", &_::getInstruction)
            .method<Instructions const&() const>("getInstructions", &_::getInstructions)
            .method<size_t() const>("getInstructionCount", &_::getInstructionCount)
            .method<Instruction*(const CodePosition&) const>("findInstructionAtCodePosition", &_::findInstructionAtCodePosition)
            .method<Instruction*(const byte*) const>("getInstructionAtMemoryAddress", &_::getInstructionAtMemoryAddress)
            .method<Instruction*(const byte*) const>("findInstructionStartingAtMemoryAddress", &_::findInstructionStartingAtMemoryAddress)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Block*() const>("getBlock", &_::getBlock)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Block*)>("setBlock", &_::setBlock)
            .method<bool()>("buildBlock", &_::buildBlock)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(BlockBuilder)>("setBlockBuilder", &_::setBlockBuilder)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<BlockBuilder() const>("getBlockBuilder", &_::getBlockBuilder)
            .method<bool(const byte*)>("containsMemoryAddress", &_::containsMemoryAddress)
            /// missing symbol(s) reflection (phantom::Closure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::Closure() const, virtual_>("getClosure", &_::getClosure)
            /// missing symbol(s) reflection (phantom::Closure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Closure)>("setClosure", &_::setClosure)
            .method<ApplyPointer() const>("getApplyPointer", &_::getApplyPointer)
            .method<void(ApplyPointer)>("setApplyPointer", &_::setApplyPointer)
            .method<size_t() const>("getFrameSize", &_::getFrameSize)
            .method<bool() const>("isRVOCandidate", &_::isRVOCandidate)
            .method<void(ExecutionContext&, void**, size_t) const>("call", &_::call)
            .method<void()>("sortInstructions", &_::sortInstructions)
            .method<void()>("clearInstructions", &_::clearInstructions)
            .method<void(void**, SmallVector<Variant, 10>&) const, virtual_|override_>("saveArgs", &_::saveArgs)
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(StringView, ABI, Modifiers, uint)>()["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(StringView, Signature*, ABI, Modifiers, uint)>()["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(ABI, Modifiers, uint)>()["0"]["0"]
            .method<void(void**, size_t, void*) const>("apply", &_::apply)
            .method<void(void**, size_t, void*) const>("placementApply", &_::placementApply)
            .method<void(void**, size_t) const>("apply", &_::apply)
            .method<void(void**, size_t) const>("placementApply", &_::placementApply)
            .method<uint64_t() const, virtual_|override_>("getUniqueID", &_::getUniqueID)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Subroutine")
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
