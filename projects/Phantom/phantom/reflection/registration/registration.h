#pragma once

#include <haunt>

HAUNT_STOP;

#include "Forward.h"
#include "phantom/VirtualDelete.h"

#include <phantom/phantom.h>
#include <phantom/reflection/Namespace.h>
#include <phantom/reflection/Source.h>
#include <phantom/reflection/TypeInfos.h>

#pragma warning(disable : 4868)

namespace phantom
{
namespace reflection
{
struct TypeInstallationInfo;

namespace detail
{
// redirections to avoid includes of Application essentially
PHANTOM_EXPORT_PHANTOM void newTemplateSpecialization(Template*               a_pTemplate,
                                                      const LanguageElements& arguments,
                                                      Symbol*                 a_pBody);
PHANTOM_EXPORT_PHANTOM Enum* newAnonEnum(PrimitiveType* a_pUnderlyingIntType);
PHANTOM_EXPORT_PHANTOM Alias* newAlias(Symbol* a_pElement, StringView a_strAlias,
                                       Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

PHANTOM_EXPORT_PHANTOM Type* findUsableType(size_t a_ModuleHandle, const TypeInfos& a_TypeInfos);
PHANTOM_EXPORT_PHANTOM Type* findUsableType(const TypeInfos& a_TypeInfos);
PHANTOM_EXPORT_PHANTOM Symbol* findCppSymbol(StringView a_Text, LanguageElement* a_pScope = nullptr,
                                             StringBuffer* a_pLastError = nullptr);
PHANTOM_EXPORT_PHANTOM bool    findCppSymbols(StringView a_Text, Symbols& a_Sym,
                                              LanguageElement* a_pScope = nullptr,
                                              StringBuffer*    a_pLastError = nullptr);
PHANTOM_EXPORT_PHANTOM Type* findCppType(StringView a_Text, LanguageElement* a_pScope = nullptr,
                                         StringBuffer* a_pLastError = nullptr);
PHANTOM_EXPORT_PHANTOM Class* findCppClass(StringView a_Text, LanguageElement* a_pScope = nullptr,
                                           StringBuffer* a_pLastError = nullptr);

// push/pop Source & Module during registration

PHANTOM_EXPORT_PHANTOM void pushModule(Module* a_pModule);
PHANTOM_EXPORT_PHANTOM void popModule();
PHANTOM_EXPORT_PHANTOM Module* currentModule();
PHANTOM_EXPORT_PHANTOM void    pushSource(Source* a_pSource);
PHANTOM_EXPORT_PHANTOM void    popSource();
PHANTOM_EXPORT_PHANTOM Source* currentSource();
PHANTOM_EXPORT_PHANTOM Source* currentSourceOrAnonymous();
PHANTOM_EXPORT_PHANTOM Source* currentSource();

PHANTOM_EXPORT_PHANTOM void use_dynamic_initializer();
PHANTOM_EXPORT_PHANTOM void pushSourceName(StringView a_strSource);
PHANTOM_EXPORT_PHANTOM void popSourceName();
PHANTOM_EXPORT_PHANTOM StringView currentSourceName();
PHANTOM_EXPORT_PHANTOM void       pushPackageName(StringView a_strPackage);
PHANTOM_EXPORT_PHANTOM void       popPackageName();
PHANTOM_EXPORT_PHANTOM StringView getCurrentPackageName();
PHANTOM_EXPORT_PHANTOM void       stepTypeInstallation(reflection::Type* a_pType);
PHANTOM_EXPORT_PHANTOM void       stepTemplateInstanceInstallation(size_t            a_ModuleHandle,
                                                                   reflection::Type* a_pType);
PHANTOM_EXPORT_PHANTOM reflection::Type* registeredTypeByHash(size_t a_ModuleHandle, hash64);
PHANTOM_EXPORT_PHANTOM void              registerModule(size_t a_ModuleHandle, StringView a_strName,
                                                        StringView a_strBinaryFileName, StringView a_strSource,
                                                        uint                              a_uiFlags,
                                                        std::initializer_list<StringView> a_Dependencies,
                                                        void (*onLoad)(), void (*onUnload)());
PHANTOM_EXPORT_PHANTOM void              unregisterModule(size_t a_ModuleHandle);
PHANTOM_EXPORT_PHANTOM bool              isActive();
PHANTOM_EXPORT_PHANTOM bool              isAutoRegistrationLocked();
PHANTOM_EXPORT_PHANTOM void              pushInstallation();
PHANTOM_EXPORT_PHANTOM void              popInstallation();
PHANTOM_EXPORT_PHANTOM void              setAutoRegistrationLocked(bool a_bLocked);
PHANTOM_EXPORT_PHANTOM                   reflection::Source*
                                         nativeSource(StringView a_strFile, StringView a_strPackage, StringView a_strSource);
PHANTOM_EXPORT_PHANTOM bool              installed();
PHANTOM_EXPORT_PHANTOM void              installModules();
PHANTOM_EXPORT_PHANTOM reflection::Module* mainModule();
PHANTOM_EXPORT_PHANTOM void                registerOrphanMemory(void* a_pMem);
PHANTOM_EXPORT_PHANTOM                     reflection::Symbol*
                                           symbolRegisteredAt(size_t a_ModuleHandle, StringView a_File, int a_Line, int a_Tag);
PHANTOM_EXPORT_PHANTOM void
                            registerTypeInstallationInfo(reflection::TypeInstallationInfo* a_pTypeInstallInfo);
PHANTOM_EXPORT_PHANTOM void registerTemplateInstance(size_t a_ModuleHandle,
                                                     reflection::TypeInstallationInfo* a_pTii);
PHANTOM_EXPORT_PHANTOM void registerType(size_t a_ModuleHandle, hash64 a_Hash,
                                         StringView a_ScopeName, reflection::Type* a_pType);
PHANTOM_EXPORT_PHANTOM void registerType(size_t a_ModuleHandle, hash64 a_Hash,
                                         reflection::Type* a_pType);
#define PHANTOM_ASSERT_ON_MAIN_THREAD() PHANTOM_ASSERT(phantom::isMainThread())
} // namespace detail

struct _PHNTM_RegistrerKeyWords
{
    using _PHNTM_Modifier = Modifier::Enum;
    using _PHNTM_MetaDatas = MetaDatas;

    static const int abstract_ = Modifier::PureVirtual;
    static const int pure_virtual = Modifier::PureVirtual;
    static const int virtual_ = Modifier::Virtual;
    static const int override_ = Modifier::Override;
    static const int final_ = Modifier::Final;
    static const int slot_ = Modifier::Slot;
    static const int transient = Modifier::Transient;
    static const int explicit_ = Modifier::Explicit;
    static const int mutable_ = Modifier::Mutable;
    static const int default_ = Modifier::Defaulted;
    static const int delete_ = Modifier::Deleted;

    template<class>
    struct private_;
    template<class>
    struct protected_;
    template<class>
    struct public_;

    template<class _PHNTM_T>
    using typedef_ = ::phantom::reflection::Forward<_PHNTM_T>;
    template<class _PHNTM_T>
    using notypedef = ::phantom::reflection::RemoveForwardT<_PHNTM_T>;
};

struct _PHNTM_GlobalRegistrer;
template<class, class, class, class>
struct TypeBuilderT;
template<class, class, class>
struct ClassTypeBuilderT;
template<class, class>
struct EnumBuilderT;
template<class, class, class>
struct UnionBuilderT;
template<class, class, class>
struct ClassBuilderT;
template<class, class>
struct DefaultUnionBuilderT;
template<class, class>
struct DefaultClassBuilderT;
template<class, class>
struct MemberAnonymousSectionBuilderT;
template<class>
struct ScopeBuilderT;

using TypeInfosGetter = TypeInfos const& (*)();

struct PHANTOM_EXPORT_PHANTOM PhantomBuilderBase
{
    PHANTOM_DECL_ABSTRACT_DELETE_METHOD(PhantomBuilderBase);

public:
    using _PHNTM_Proxy = void;

protected:
    ~PhantomBuilderBase()
    {
        for (PhantomBuilderBase* pType : _PHNTM_SubRegistrers)
        {
            PHANTOM_DELETE_VIRTUAL pType;
        }
    }

    void addSubPhantomBuilderBase(PhantomBuilderBase* a_pSub);

private:
    SmallVector<PhantomBuilderBase*> _PHNTM_SubRegistrers;
};

template<class EndableReg>
struct ScopedIf
{
    ScopedIf(EndableReg& a_Reg) : reg(a_Reg)
    {
    }
    ~ScopedIf()
    {
        reg.end();
    }
    operator bool()
    {
        return true;
    }
    EndableReg& operator()()
    {
        return reg;
    }
    EndableReg& reg;
};

template<class EndableReg>
ScopedIf<EndableReg> makeScopedIf(EndableReg& a_Reg)
{
    return ScopedIf<EndableReg>(a_Reg);
}

struct PHANTOM_EXPORT_PHANTOM SymbolWrapper
{
    explicit SymbolWrapper(Symbol* a_pSymbol) : m_pSymbol(a_pSymbol)
    {
    }
    SymbolWrapper& operator()(MetaDatas&& a_MD);
    SymbolWrapper& operator()(StringView a_Name, Variant&& a_Value);
    SymbolWrapper& operator()(StringView a_Annot);

private:
    Symbol* m_pSymbol;
};

template<class Top, class T, class = std::enable_if_t<std::is_same<T, T*>::value, void>>
void _PHNTM_SelectBuilder()
{
}

template<class Top, class T, class Ret = decltype(_PHNTM_SelectBuilder<Top, T>((T*)0))>
auto& SelectBuilder(T* t)
{
    return _PHNTM_SelectBuilder<Top, T>(t);
}

template<class T>
struct ImplConv
{
    ImplConv(T);
};

union AnonymousUnionProxy {
};
class AnonymousStructProxy
{
};

template<class Top, class T, class = std::enable_if_t<std::is_class<T>::value, void>>
DefaultClassBuilderT<T, Top>* DefaultBuilder(T*, int);
template<class Top, class T,
         class = std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value, void>>
EnumBuilderT<T, Top>* DefaultBuilder(T*, short);
template<class Top, class T, class = std::enable_if_t<std::is_union<T>::value, void>>
DefaultUnionBuilderT<T, Top>* DefaultBuilder(T*, char);

template<class Top, class T>
decltype(phantom::reflection::DefaultBuilder<Top, T>((T*)0, 0)) SelectBuilder(ImplConv<T*>);

#define PHANTOM_BUILDER_TYPE(Top, ...)                                                             \
    std::remove_pointer_t<decltype(                                                                \
    phantom::reflection::SelectBuilder<Top, __VA_ARGS__>((__VA_ARGS__*)0))>
} // namespace reflection
} // namespace phantom

#define PHANTOM_REGISTRATION_STATIC_ASSERT_ENABLED 1

#if PHANTOM_REGISTRATION_STATIC_ASSERT_ENABLED
#    define _PHNTM_REG_STATIC_ASSERT PHANTOM_STATIC_ASSERT
#else
#    define _PHNTM_REG_STATIC_ASSERT(...)
#endif

#define _PHNTM_REG_FRIENDS_NO_GLOBAL                                                               \
    template<class, class, class, class>                                                           \
    friend struct phantom::reflection::TypeBuilderT;                                               \
    template<class, class, class>                                                                  \
    friend struct phantom::reflection::ClassTypeBuilderT;                                          \
    template<class, class>                                                                         \
    friend struct phantom::reflection::EnumBuilderT;                                               \
    template<class, class, class>                                                                  \
    friend struct phantom::reflection::ClassBuilderT;                                              \
    template<class, class, class>                                                                  \
    friend struct phantom::reflection::UnionBuilderT;                                              \
    template<class>                                                                                \
    friend struct phantom::reflection::ScopeBuilderT;                                              \
    template<class, class>                                                                         \
    friend struct phantom::reflection::MemberAnonymousSectionBuilderT;

#define _PHNTM_REG_FRIENDS                                                                         \
    friend struct phantom::reflection::_PHNTM_GlobalRegistrer;                                     \
    _PHNTM_REG_FRIENDS_NO_GLOBAL

// user accessible predefined inside registrers
#define PHANTOM_NOT_TEMPLATE 1
#define PHANTOM_IF_NOT_TEMPLATE_ONLY(...) __VA_ARGS__
#define PHANTOM_REGISTRATION_STEP _PHNTM_RegStep
#define PHANTOM_SCOPE                                                                              \
    phantom::reflection::BuilderProxyTypeT<std::remove_reference_t<decltype(this_())>>

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHTNM_FMT_DEFAULTS(...)                                                               \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHTNM_FMT_DEFAULTS_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),     \
                       (__VA_ARGS__))
#else
#    define _PHTNM_FMT_DEFAULTS(...)                                                               \
        PHANTOM_PP_CAT(_PHTNM_FMT_DEFAULTS_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHTNM_FMT_DEFAULTS_1(a0) [PHANTOM_PP_QUOTE_L5(PHANTOM_PP_REMOVE_PARENS(a0))]
#else
#    define _PHTNM_FMT_DEFAULTS_1(a0) [PHANTOM_PP_QUOTE(PHANTOM_PP_REMOVE_PARENS(a0))]
#endif
#define _PHTNM_FMT_DEFAULTS_2(a0, a1) _PHTNM_FMT_DEFAULTS_1(a0) _PHTNM_FMT_DEFAULTS_1(a1)
#define _PHTNM_FMT_DEFAULTS_3(a0, a1, a2) _PHTNM_FMT_DEFAULTS_2(a0, a1) _PHTNM_FMT_DEFAULTS_1(a2)
#define _PHTNM_FMT_DEFAULTS_4(a0, a1, a2, a3)                                                      \
    _PHTNM_FMT_DEFAULTS_3(a0, a1, a2) _PHTNM_FMT_DEFAULTS_1(a3)
#define _PHTNM_FMT_DEFAULTS_5(a0, a1, a2, a3, a4)                                                  \
    _PHTNM_FMT_DEFAULTS_4(a0, a1, a2, a3) _PHTNM_FMT_DEFAULTS_1(a4)
#define _PHTNM_FMT_DEFAULTS_6(a0, a1, a2, a3, a4, a5)                                              \
    _PHTNM_FMT_DEFAULTS_5(a0, a1, a2, a3, a4) _PHTNM_FMT_DEFAULTS_1(a5)
#define _PHTNM_FMT_DEFAULTS_7(a0, a1, a2, a3, a4, a5, a6)                                          \
    _PHTNM_FMT_DEFAULTS_6(a0, a1, a2, a3, a4, a5) _PHTNM_FMT_DEFAULTS_1(a6)
#define _PHTNM_FMT_DEFAULTS_8(a0, a1, a2, a3, a4, a5, a6, a7)                                      \
    _PHTNM_FMT_DEFAULTS_7(a0, a1, a2, a3, a4, a5, a6) _PHTNM_FMT_DEFAULTS_1(a7)
#define _PHTNM_FMT_DEFAULTS_9(a0, a1, a2, a3, a4, a5, a6, a7, a8)                                  \
    _PHTNM_FMT_DEFAULTS_8(a0, a1, a2, a3, a4, a5, a6, a7) _PHTNM_FMT_DEFAULTS_1(a8)

static const int _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1 = 0;
static const int _PHNTM_TRAILING_METADATA_COUNTER_MINUS_2 = 0;
static const int _PHNTM_MTRAILING_METADATA = 0;

#define _PHNTM_PRE_TRAILING_METADATA static const int PHANTOM_PP_CAT(_PHNTM_dummy, __COUNTER__) =

#define _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1(...)                                              \
    0;                                                                                             \
    PHANTOM_REGISTER(End)                                                                          \
    {                                                                                              \
        phantom::reflection::SymbolWrapper(phantom::detail::symbolRegisteredAt(                    \
        phantom::currentModuleHandle(), __FILE__, __LINE__, __COUNTER__ - 1)) __VA_ARGS__;         \
    }

#define _PHNTM_TRAILING_METADATA_COUNTER_MINUS_2(...)                                              \
    0;                                                                                             \
    PHANTOM_REGISTER(End)                                                                          \
    {                                                                                              \
        phantom::reflection::SymbolWrapper(phantom::detail::symbolRegisteredAt(                    \
        phantom::currentModuleHandle(), __FILE__, __LINE__, __COUNTER__ - 2)) __VA_ARGS__;         \
    }

#define _PHNTM_MTRAILING_METADATA(...) (this_()) __VA_ARGS__
