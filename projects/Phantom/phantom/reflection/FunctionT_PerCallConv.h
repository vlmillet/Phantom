
HAUNT_STOP;

template<typename t_ReturnType, class... v_Params>
class FunctionT<t_ReturnType(_PHNTM_CALL_CONV*)(v_Params...)> : public Function
{
public:
    typedef FunctionT<t_ReturnType(_PHNTM_CALL_CONV*)(v_Params...)> self_type;
    typedef t_ReturnType(_PHNTM_CALL_CONV* FunctionPtrType)(v_Params...);
    typedef PHANTOM_TYPENAME CallReturnTypeH<t_ReturnType>::type CallReturnType;

    FunctionT(StringView a_strName, Signature* a_pSignature, FunctionPtrType a_pFunc, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Function(a_strName, a_pSignature, ABI::_PHNTM_CALL_CONV_ABI, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual Closure getClosure() const override
    {
        Closure c;
        c.address = (void*)m_Pointer;
        return c;
    }

    virtual OpaqueDelegate getOpaqueDelegate() const override
    {
        return OpaqueDelegateCaster<ABI::_PHNTM_CALL_CONV_ABI, t_ReturnType(v_Params...)>::cast(m_Pointer);
    }

    template<size_t... Is>
    inline void call(void** a_pParams, Indices<Is...>) const
    {
        (*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    void call(void** a_pParams) const override
    {
        call(a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void call(void** a_pParams, void* a_pReturnAddress, Indices<Is...>) const
    {
        CallReturner<t_ReturnType>::apply(
        a_pReturnAddress,
        (*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...));
    }

    void call(void** a_pParams, void* a_pReturnAddress) const override
    {
        call(a_pParams, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void placementCall(void** a_pParams, void* a_pReturnAddress, Indices<Is...>) const
    {
        new (a_pReturnAddress) CallReturnType(CallReturnConverter<t_ReturnType>::apply((*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...)));
    }

    void placementCall(void** a_pParams, void* a_pReturnAddress) const override
    {
        placementCall(a_pParams, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        call(a_pArgs, a_Context.resultPointer(), BuildIndices<sizeof...(v_Params)>{});
    }

    void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        placementCall(a_pArgs, a_Context.resultPointer(), BuildIndices<sizeof...(v_Params)>{});
    }

private:
    FunctionPtrType m_Pointer;
};

template<class... v_Params>
class FunctionT<void(_PHNTM_CALL_CONV*)(v_Params...)> : public Function
{
public:
    typedef FunctionT<void(_PHNTM_CALL_CONV*)(v_Params...)> self_type;
    typedef void(_PHNTM_CALL_CONV* FunctionPtrType)(v_Params...);

    FunctionT(StringView a_strName, Signature* a_pSignature, FunctionPtrType a_pFunc, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Function(a_strName, a_pSignature, ABI::_PHNTM_CALL_CONV_ABI, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual Closure getClosure() const override
    {
        Closure c;
        c.address = (void*)m_Pointer;
        return c;
    }

    virtual OpaqueDelegate getOpaqueDelegate() const override
    {
        return OpaqueDelegateCaster<ABI::_PHNTM_CALL_CONV_ABI, void(v_Params...)>::cast(m_Pointer);
    }

    template<size_t... Is>
    inline void call(void** a_pParams, Indices<Is...>) const
    {
        (*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        call(a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    void placementCall(ExecutionContext&, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(false, "expecting return address from a void function placement call");
        call(a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    void call(void** a_pParams) const override
    {
        call(a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

private:
    FunctionPtrType m_Pointer;
};
