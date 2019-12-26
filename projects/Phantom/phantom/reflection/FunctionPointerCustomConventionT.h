// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/// @cond INTERNAL

HAUNT_STOP;

template<typename Signature>
class FunctionPointerT;

template<typename R, class... v_Params>
class FunctionPointerT<R(_PHNTM_CONV_KW*)(v_Params...)> : public FunctionPointer
{
public:
    typedef FunctionPointerT<R(_PHNTM_CONV_KW*)(v_Params...)> self_type;
    typedef typename FunctionPointerTypeFromFunctionType<
    phantom / convention_id, R(_PHNTM_CONV_KW*)(v_Params...)>::type function_pointer_t;

    FunctionPointerT(Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : FunctionPointer(SignatureH<self_type, R(v_Params...)>::CreateSignature(),
                          ABI::_PHNTM_CONV_ID, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE,
                          (int)0)
    {
    }
    FunctionPointerT(Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : FunctionPointer(a_pSignature, ABI::_PHNTM_CONV_ID, a_Modifiers,
                          a_uiFlags | PHANTOM_R_FLAG_NATIVE, (int)0)
    {
    }

    typedef PHANTOM_TYPENAME CallReturnTypeH<R>::type CallReturnType;

    void* getClosurePointer(void* a_pPointer) const
    {
        return (void*)(*static_cast<function_pointer_t*>(a_pPointer));
    }
    OpaqueDelegate getOpaqueDelegate(void* a_pPointer) const
    {
        return Delegate<R(v_Params...)>(*static_cast<function_pointer_t*>(a_pPointer)).getOpaque();
    }
    template<size_t... Is>
    inline void call(void* a_pPointer, void** a_pParams, Indices<Is...>) const
    {
        (*(*static_cast<function_pointer_t*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    virtual void call(void* a_pPointer, void** a_pParams) const
    {
        call(a_pPointer, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void call(void* a_pPointer, void** a_pParams, void* a_pReturnAddress,
                     Indices<Is...>) const
    {
        *static_cast<CallReturnType*>(a_pReturnAddress) =
        CallReturnConverter<R>::apply((*(*static_cast<function_pointer_t*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...));
    }

    virtual void call(void* a_pPointer, void** a_pParams, void* a_pReturnAddress) const
    {
        call(a_pPointer, a_pParams, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

protected:
};

/// @endcond
