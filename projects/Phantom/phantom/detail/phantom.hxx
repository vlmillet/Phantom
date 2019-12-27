#pragma once

// haunt {

#include "phantom.h"

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
#include <phantom/enum_class>
#include <phantom/function>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <std/initializer_list.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<size_t(void const*)>("_dllModuleHandleFromAddress", _dllModuleHandleFromAddress);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("phantom")
PHANTOM_END("phantom")
namespace  {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

    PHANTOM_END("phantom")
PHANTOM_END("phantom")
}
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void(StringView, StringBuffer&, ::phantom::reflection::LanguageElement *)>("conversionOperatorNameNormalizer", conversionOperatorNameNormalizer);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("phantom")
PHANTOM_END("phantom")
}
namespace phantom {
namespace detail {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

        PHANTOM_CLASS_T((class), (t_Ty), TypeRegistrer)
        {
            this_()
            ;
        }
        PHANTOM_CLASS_T((class, class, bool), (t_Target, t_Source, t_SourceHasEmbeddedRtti), asH)
        {
            this_()
            ;
        }
    PHANTOM_END("phantom")
PHANTOM_END("phantom")
}
}
namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<RegistrationStep>().values({
            {"_None",RegistrationStep::_None},
            {"_Reserved",RegistrationStep::_Reserved},
            {"Start",RegistrationStep::Start},
            {"Namespaces",RegistrationStep::Namespaces},
            {"Enums",RegistrationStep::Enums},
            {"ClassTypes",RegistrationStep::ClassTypes},
            {"PostClassTypes",RegistrationStep::PostClassTypes},
            {"Typedefs",RegistrationStep::Typedefs},
            {"PostTypedefs",RegistrationStep::PostTypedefs},
            {"TemplateSignatures",RegistrationStep::TemplateSignatures},
            {"PostTypes",RegistrationStep::PostTypes},
            {"Variables",RegistrationStep::Variables},
            {"PostVariables",RegistrationStep::PostVariables},
            {"Functions",RegistrationStep::Functions},
            {"End",RegistrationStep::End}});
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<RegistrationSteps>("RegistrationSteps"); }
        PHANTOM_REGISTER(Enums) { this_().enum_<TypeInstallationStep>().values({
            {"Uninstalled",TypeInstallationStep::Uninstalled},
            {"TemplateSignature",TypeInstallationStep::TemplateSignature},
            {"TemplateDefaultArgs",TypeInstallationStep::TemplateDefaultArgs},
            {"Inheritance",TypeInstallationStep::Inheritance},
            {"Members",TypeInstallationStep::Members},
            {"Installed",TypeInstallationStep::Installed}});
        }
        /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<TypeInstallationDelegate>("TypeInstallationDelegate"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("phantom")
PHANTOM_END("phantom")
}
namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("phantom")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<MessageType>().values({
            {"Undefined",MessageType::Undefined},
            {"Information",MessageType::Information},
            {"Success",MessageType::Success},
            {"Warning",MessageType::Warning},
            {"Error",MessageType::Error}});
        }
        /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<MessageReportFunc>("MessageReportFunc"); }
        /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<LogFunc>("LogFunc"); }
        /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<PrintDelegate>("PrintDelegate"); }
        /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<void(MessageReportFunc)>("setAssertFunc", setAssertFunc);}
        /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<void(MessageReportFunc)>("setErrorFunc", setErrorFunc);}
        /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<void(LogFunc)>("setLogFunc", setLogFunc);}
        /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<void(MessageReportFunc)>("setWarningFunc", setWarningFunc);}
        /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Functions) { this_().function<void(int, const PrintDelegate&)>("setPrintDelegate", setPrintDelegate);}
        PHANTOM_REGISTER(Functions) { this_().function<bool()>("isMainThread", isMainThread);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("phantom")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
