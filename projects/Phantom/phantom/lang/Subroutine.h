// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Callable.h"
#include "Symbol.h"
#include "Type.h"

#include <phantom/utils/Delegate.h>
#include <phantom/utils/StringViews.h>
#include <phantom/utils/Variant.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM MemoryLocation
{
    /// \brief  Default constructor.
    MemoryLocation() : m_pStart(0), m_pEnd(0) {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a memory location from a start address and an end address.
    ///
    /// \param [in,out] start   The start address.
    /// \param [in,out] end     The end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemoryLocation(byte* start, byte* end) : m_pStart(start), m_pEnd(end)
    {
        PHANTOM_ASSERT(m_pStart && m_pEnd && (m_pStart < m_pEnd));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Equality operator.
    ///
    /// \param  other   The other memory location to be compared to.
    ///
    /// \return true if the two memory locations are isSame.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool operator==(const MemoryLocation& other) const { return m_pStart == other.m_pStart && m_pEnd == other.m_pEnd; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Less-than comparison operator.
    ///
    /// \param  other   The other memory location to be compared to.
    ///
    /// \return true if this memory location is before the given other one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool operator<(const MemoryLocation& other) const { return m_pStart < other.m_pStart; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine is valid.
    ///
    /// \return true if valid, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isValid() const { return m_pStart && m_pEnd; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the start address.
    ///
    /// \return The start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getStart() const { return m_pStart; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the end address.
    ///
    /// \return The end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getEnd() const { return m_pEnd; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the start address.
    ///
    /// \param a_pAddress The start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setStart(byte* a_pAddress);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the end address.
    ///
    /// \param a_pAddress The end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setEnd(byte* a_pAddress);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if it contains the given memory address.
    ///
    /// \param  a_pAddress  The address.
    ///
    /// \return true if it contains the memory address, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool containsMemoryAddress(const byte* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the size of this memory location.
    ///
    /// \return The calculated size of this memory location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline size_t getSize() const { return m_pEnd - m_pStart; }

protected:
    byte* m_pStart;
    byte* m_pEnd;
};

template<class R, class... Args>
struct SubroutineCallH
{
    static R call(Subroutine const* a_pSubroutine, Args&&... a_Args);
};

template<class... Args>
struct SubroutineCallH<void, Args...>
{
    static void call(Subroutine const* a_pSubroutine, Args&&... a_Args);
};

class PHANTOM_EXPORT_PHANTOM Instruction
{
    friend class Subroutine;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an instruction from an op code, a code location and a memory location.
    ///
    /// \param  a_iOpcode           The opcode.
    /// \param  a_CodeRange         The code location.
    /// \param  a_MemoryLocation    The memory location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction(int a_iOpcode, const CodeRange& a_CodeRange, const MemoryLocation& a_MemoryLocation = MemoryLocation(),
                void* a_pUserData = nullptr)
        : m_pSubroutine(nullptr),
          m_iOpcode(a_iOpcode),
          m_CodeRange(a_CodeRange),
          m_MemoryLocation(a_MemoryLocation),
          m_pUserData(a_pUserData)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Less-than comparison operator.
    ///
    /// \param  other   The other instruction to be compared to.
    ///
    /// \return true if this instruction is before the other given one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool operator<(const Instruction& other) const { return m_MemoryLocation < other.m_MemoryLocation; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine holding this instruction.
    ///
    /// \return The subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine() const { return m_pSubroutine; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets user data.
    ///
    /// \param  a_pUserData The data.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setUserData(void* a_pUserData) const { m_pUserData = a_pUserData; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the user data.
    ///
    /// \return The user data.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getUserData() const { return m_pUserData; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the operation code of this instruction.
    ///
    /// \return The operation code.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int getOpCode() const { return m_iOpcode; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the code location of this instruction.
    ///
    /// \return The code location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const CodeRange& getCodeRange() const { return m_CodeRange; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the code location of this instruction.
    ///
    /// \return The code location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setCodeRange(const CodeRange& a_CodeRange) { m_CodeRange = a_CodeRange; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory location of this instruction.
    ///
    /// \return The memory location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const MemoryLocation& getMemoryLocation() const { return m_MemoryLocation; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source containing this instruction.
    ///
    /// \return The source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory start address of this instruction.
    ///
    /// \return The memory start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getMemoryStart() const { return m_MemoryLocation.getStart(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory end address of this instruction.
    ///
    /// \return The memory end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getMemoryEnd() const { return m_MemoryLocation.getEnd(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this instruction contains the given code position.
    ///
    /// \param  a_CodePosition    The code position.
    ///
    /// \return true if it contains the cdoe position, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline bool containsCodePosition(const CodePosition& a_CodePosition)
    {
        return m_CodeRange.containsCodePosition(a_CodePosition);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this instruction contains the given memory address.
    ///
    /// \param  a_pAddress  The address.
    ///
    /// \return true if it contains the memory address, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline bool containsMemoryAddress(const byte* a_pAddress)
    {
        return m_MemoryLocation.containsMemoryAddress(a_pAddress);
    }

    void setMemoryLocation(const MemoryLocation& a_MemoryLocation) { m_MemoryLocation = a_MemoryLocation; }

protected:
    Subroutine*    m_pSubroutine;
    int            m_iOpcode;
    CodeRange      m_CodeRange;
    MemoryLocation m_MemoryLocation;
    mutable void*  m_pUserData;
};

/// \brief  Represents the return type relation of two subroutines.
enum ESignatureRelation
{
    e_SignatureRelation_None = 0,
    e_SignatureRelation_Equal,
    e_SignatureRelation_Covariant,
    e_SignatureRelation_Contravariant,
    e_SignatureRelation_Forbidden,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Base class for procedural programming language subroutine representations. Used for C++
///         functions and member functions.
////////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM Subroutine : public Symbol, public Callable
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Subroutine);

    friend class Block;
    friend class LocalVariable;
    friend class Label;
    friend class ClassType;
    friend class Class;

public:
    using ApplyPointer = void (*)(void** /*args*/, void* /*ret*/);

public:
    void terminate();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Application Binary Interface (ABI) of this subroutine.
    ///
    /// \return The ABI.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ABI getABI() const { return m_eABI; }

    virtual Callable* asCallable() const { return const_cast<Subroutine*>(this); }
    Subroutine*       asSubroutine() const override { return const_cast<Subroutine*>(this); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the signature of this subroutine.
    ///
    /// \return The signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Signature* getSignature() const { return m_pSignature; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the signature relation with another described subroutine.
    ///
    /// \param  a_strName               The other subroutine's name.
    /// \param [in,out] a_pSignature    The other subroutine's signature.
    /// \param  a_Modifiers             The other subroutine's modifiers.
    ///
    /// \return The signature relation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ESignatureRelation getSignatureRelationWith(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers,
                                                uint a_uiFlags = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets signature relation with another given subroutine.
    ///
    /// \param [in,out] a_pSubroutine   The other subroutine.
    ///
    /// \return The signature relation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ESignatureRelation getSignatureRelationWith(Subroutine* a_pSubroutine) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the signature relation with another described subroutine.
    ///
    /// \param [in,out] a_pReturnType   The other subroutine's return type.
    /// \param  a_strName               The other subroutine's name.
    /// \param  a_Types                 The other subroutine's parameter types.
    /// \param  a_Modifiers             The other subroutine's modifiers.
    ///
    /// \return The signature relation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ESignatureRelation getSignatureRelationWith(Type* a_pReturnType, StringView a_strName, TypesView a_Types,
                                                Modifiers a_Modifiers, uint a_uiFlags = 0) const;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getUniqueName;

    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter types of this subroutine.
    ///
    /// \return The parameter types.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Types getParameterTypes() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter type at given position index.
    ///
    /// \param  i   The index.
    ///
    /// \return The parameter type at given position index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* getParameterType(size_t i) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameters of this subroutine.
    ///
    /// \return The parameters.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameters const& getParameters() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter matching the given name.
    ///
    /// \param  a_strName   The parameter name.
    ///
    /// \return null is no parameter found, else the parameter matching the given name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Parameter* getParameter(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the required argument count for calling this subroutine (skipping default
    /// arguments).
    ///
    /// \return The required argument count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual size_t getRequiredArgumentCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds default arguments to the signature of this subroutine.
    ///
    /// \param  a_ArgExps    The default arguments as an array view of string view (\see
    /// Signature::setNativeDefaultArgumentStrings).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setNativeDefaultArgumentStrings(ArrayView<StringView> a_ArgExps);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  collects the native default arguments of this signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringViews getNativeDefaultArgumentStrings() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine matched another described by its parameter types and
    /// modifiers.
    ///
    /// \param  parameterTypes  The other subroutine's parameter types.
    /// \param  a_Modifiers     The other subroutine's modifiers.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool matches(TypesView parameterTypes, Modifiers a_Modifiers = Modifiers()) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine matched another described by its template arguments,
    /// parameter types and
    ///         modifiers.
    ///
    /// \param  templateArguments   The other subroutine's template arguments.
    /// \param  parameterTypes      The other subroutine's parameter types.
    /// \param  a_Modifiers         The other subroutine's modifiers.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool matches(const LanguageElements& templateArguments, TypesView parameterTypes,
                         Modifiers a_Modifiers = Modifiers()) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine matched another described by its name, parameter types and
    ///         qualifiers.
    ///
    /// \param  a_strName           The other subroutine's name.
    /// \param  a_ParameterTypes    The other subroutine's parameter types.
    /// \param  a_Modifiers        (optional) the other subroutine's qualifiers.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(StringView a_strName, TypesView a_FunctionSignature, Modifiers a_Modifiers = Modifiers()) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the return type of this subroutine.
    ///
    /// \return The return type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReturnType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates an opaque delegate.
    ///
    /// \return an opaque delegate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual OpaqueDelegate getOpaqueDelegate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates an opaque delegate from a caller object.
    ///
    /// \param [in,out] a_pObject    The caller object.
    /// \return an opaque delegate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual OpaqueDelegate getOpaqueDelegate(void* a_pObject) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine, executing in a given execution context
    ///
    /// \param  a_Context   The execution context.
    /// \param  a_pArgs     The arguments addresses.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void call(ExecutionContext& a_Context, void** a_pArgs) const;
    virtual void callVarArg(ExecutionContext& a_Context, void** a_pArgs, Types const& a_VarArgTypes) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine, executing either interpreted or delegate code, and returning
    ///         value in place to the given area.
    ///
    /// \param  a_pObject    The caller address.
    /// \param  a_pArgs             The arguments addresses.
    /// \param  a_pReturnArea       The return area.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void placementInvoke(void* a_pCallerAddress, void** a_pArgs, void* a_pReturnAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine on a non-constructed return area, executing in a given
    /// execution context
    ///
    /// \param  a_Context   The execution context.
    /// \param  a_pArgs     The arguments addresses.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void placementCall(ExecutionContext& a_Context, void** a_pArgs) const;
    virtual void placementCallVarArg(ExecutionContext& a_Context, void** a_pArgs, Types const& a_VarArgTypes) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine, executing either interpreted or delegate code, and returning
    ///         value in place to the given area.
    ///
    /// \param  a_pArgs The arguments addresses.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void         call(void** a_pArgs) const override;
    virtual void callVarArg(void** a_pArgs, Types const& a_VarArgTypes) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine, executing either interpreted or delegate code, and returning
    ///         value to the given area.
    ///
    /// \param  a_pArgs             The arguments addresses.
    /// \param  a_pReturnArea       The return area.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void         call(void** a_pArgs, void* a_pReturnAddress) const override;
    virtual void callVarArg(void** a_pArgs, Types const& a_VarArgTypes, void* a_pReturnAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Calls the subroutine, executing either interpreted or delegate code, and returning
    ///         value in place to the given area.
    ///
    /// \param  a_pArgs             The arguments addresses.
    /// \param  a_pReturnArea       The return area.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void placementCall(void** a_pArgs, void* a_pReturnAddress) const;
    virtual void placementCallVarArg(void** a_pArgs, Types const& a_VarArgTypes, void* a_pReturnAddress) const;

    template<class... Args>
    Variant callV(Args&&... a_Args) const
    {
        void* addresses[] = {&a_Args..., 0};
        if (getReturnType()->getTypeKind() == TypeKind::Void)
        {
            call(addresses);
            return Variant();
        }
        else
        {
            Variant result;
            result.setType(getReturnType());
            call(addresses, result.data());
            return result;
        }
    }

    template<class R, class... Args>
    R call(Args&&... a_Args) const
    {
        return SubroutineCallH<R, Args...>::call(this, std::forward<Args>(a_Args)...);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Checks if the underlying native pointer is isSame the pointer behing the address we
    /// give
    ///
    /// \param  a_pPtrOnPtr             The pointer on the function pointer.
    /// \return                         true if the pointers are equal.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool equalsPointer(void* a_pPtrOnPtr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the memory location of this subroutine (commonly used by JIT compilation for
    ///         debugging purpose).
    ///
    /// \param  a_MemoryLocation    The memory location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setMemoryLocation(const MemoryLocation& a_MemoryLocation) { m_MemoryLocation = a_MemoryLocation; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory location of this subroutine.
    ///
    /// \return The memory location.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const MemoryLocation& getMemoryLocation() const { return m_MemoryLocation; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory start address of this subroutine.
    ///
    /// \return The memory start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getMemoryStart() const { return m_MemoryLocation.getStart(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory end address of this subroutine.
    ///
    /// \return The memory end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getMemoryEnd() const { return m_MemoryLocation.getEnd(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the memory start address of this subroutine.
    ///
    /// \param  a_pAddress   The memory start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setMemoryStart(byte* a_pAddress) { m_MemoryLocation.setStart(a_pAddress); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the memory end address of this subroutine.
    ///
    /// \param  a_pAddress   The memory end address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setMemoryEnd(byte* a_pAddress) { m_MemoryLocation.setEnd(a_pAddress); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an instruction representation to this subroutine for debugging and
    /// introspection purpose.
    ///
    /// \param [in,out] a_pInstruction  The instruction.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction* createInstruction(int a_iOpCode, const CodeRangeLocation& a_Location,
                                   const MemoryLocation& a_MemoryLocation = MemoryLocation(),
                                   void*                 a_pUserData = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an instruction representation to this subroutine for debugging and
    /// introspection purpose.
    ///
    /// \param [in,out] a_pInstruction  The instruction.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void deleteInstruction(Instruction* a_pInst);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an instruction at given position index.
    ///
    /// \param  i   The index of the instruction.
    ///
    /// \return The instruction.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction* getInstruction(size_t i) const
    {
        PHANTOM_ASSERT(m_pInstructions && m_pInstructions->size() > i);
        return (*m_pInstructions)[i];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all the instructions stored in this sub routine (use sortInstructions to sort
    /// them by memory location).
    ///
    /// \return The instructions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instructions const& getInstructions() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the instruction count.
    ///
    /// \return The instruction count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getInstructionCount() const { return m_pInstructions ? m_pInstructions->size() : 0; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for an instruction at given code position if any.
    ///
    /// \param  a_CodePosition    The code position.
    ///
    /// \return null if no instruction found, else the instruction found at given code position.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction* findInstructionAtCodePosition(const CodePosition& a_CodePosition) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the instruction at given memory address.
    ///
    /// \param  a_pAddress    The memory address.
    ///
    /// \return null if no instruction defined enclosing the given memory address, else the
    /// instruction.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction* getInstructionAtMemoryAddress(const byte* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for an instruction starting exactly at given memory address.
    ///
    /// \param [in,out] a_pAddress  If non-null, the address.
    ///
    /// \return null if no instruction found, else the instruction found starting at memory address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Instruction* findInstructionStartingAtMemoryAddress(const byte* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the code block of this subroutine.
    ///
    /// \return null if it fails, else the block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Block* getBlock() const { return m_pBlock; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a block to this subroutine. The subroutine must not be native.
    ///
    /// \return null if it fails, else the PHANTOM_NEW(code) block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setBlock(Block* a_pBlock);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine contains the given memory address.
    ///
    /// \param  a_pAddress  The address to test.
    ///
    /// \return true if it contains the memory address, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool containsMemoryAddress(const byte* a_pAddress);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the closure of this subroutine, if any defined by the user.
    ///
    /// \return null if it fails, else the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Closure getClosure() const { return m_Closure; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the closure of this subroutine.
    ///
    /// \param [in,out] a_pClosure  If non-null, the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setClosure(Closure a_Closure)
    {
        PHANTOM_ASSERT(!isNative());
        m_Closure = a_Closure;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the closure of this subroutine, if any defined by the user.
    ///
    /// \return null if it fails, else the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ApplyPointer getApplyPointer() const { return m_ApplyPointer; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the closure of this subroutine.
    ///
    /// \param [in,out] a_pClosure  If non-null, the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setApplyPointer(ApplyPointer a_ApplyPointer)
    {
        PHANTOM_ASSERT(!isNative());
        m_ApplyPointer = a_ApplyPointer;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the frame size of this subroutine (memory used by local variables).
    ///
    /// \return The frame size.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getFrameSize() const { return m_uiFrameSize; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this subroutine is a candidate to Return Value Optimisation (RVO).
    ///
    /// \return true if rvo candidate, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isRVOCandidate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Perform a call through the interpreter or JIT if any compilation has been done.
    ///
    /// \param  a_pArgs         The call arguments.
    /// \param  a_uiCount       The number of call arguments.
    /// \param  a_pReturnArea   If non-null, the return area.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void call(ExecutionContext& a_Context, void** a_pArgs, size_t a_uiCount) const;

    void sortInstructions();

    void clearInstructions();

    void saveArgs(void** a_pArgs, SmallVector<Variant, 10>& a_variants) const override;

protected:
    Subroutine();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a 'void()' subroutine with a name, an ABI and optional modifiers.
    ///
    /// \param  a_strName   The name.
    /// \param  a_eABI      The ABI.
    /// \param  a_Modifiers (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine(StringView a_strName, ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a subroutine with a name, a signature, an ABI and optional modifiers.
    ///
    /// \param  a_strName               The name.
    /// \param  a_pSignature    The signature.
    /// \param  a_eABI                  The ABI.
    /// \param  a_Modifiers             (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers = 0,
               uint a_uiFlags = 0);

    /// \internal
    Subroutine(ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    /// \internal
    void apply(void** a_pArgs, size_t a_uiCount, void* a_pReturnAddr) const;
    /// \internal
    void placementApply(void** a_pArgs, size_t a_uiCount, void* a_pReturnAddr) const;
    /// \internal
    void apply(void** a_pArgs, size_t a_uiCount) const;
    /// \internal
    void placementApply(void** a_pArgs, size_t a_uiCount) const;

    uint64_t getUniqueID() const override { return uint64_t(this); }

protected:
    Signature*     m_pSignature = nullptr;
    ABI            m_eABI = ABI::StdCall;
    Instructions*  m_pInstructions = nullptr;
    Block*         m_pBlock = nullptr;
    MemoryLocation m_MemoryLocation;
    CallDelegate   m_CallDelegate;
    ApplyPointer   m_ApplyPointer = nullptr;
    Closure        m_Closure;
    size_t         m_uiFrameSize = 0;
};

template<class R, class... Args>
R SubroutineCallH<R, Args...>::call(Subroutine const* a_pSubroutine, Args&&... a_Args)
{
    PHANTOM_ASSERT(a_pSubroutine->getReturnType()->removeQualifiers() == PHANTOM_TYPEOF(R));
    void* addresses[] = {(&a_Args)..., 0};
    R     result;
    a_pSubroutine->call(addresses, &result);
    return result;
}

template<class... Args>
void SubroutineCallH<void, Args...>::call(Subroutine const* a_pSubroutine, Args&&... a_Args)
{
    PHANTOM_ASSERT(a_pSubroutine->getReturnType()->getTypeKind() == TypeKind::Void);
    void* addresses[] = {(&a_Args)..., 0};
    a_pSubroutine->call(addresses);
}

} // namespace lang
} // namespace phantom
