#pragma once

// This macro, declared inside any namespace, allows to use 'PHANTOM_PRIVATE_ACCESS' inside any class of this namespace.
// A good practice is to put this macro in your top forwarding header (ex: forward.h or prerequisistes.h) so that you
// don't have to worry about it later (you can also use directly macro content to avoid any phantom direct includes)

#define PHANTOM_FORWARD_DECL_NAMESPACE                                                                                 \
    template<class>                                                                                                    \
    struct _PHNTM_Registrer

// This allows phantom to access private members of your class/struct/union and register them to reflection
// Indeed by default, Phantom cannot access any private member, which is normal considering what a 'private' is.
// (Phantom can access your protected members thanks to an inheritance hack though..)
// If you get a compilation error 'undefined _PHNTM_Registrer' symbol here you should use the macro above
// 'PHANTOM_FORWARD_DECL_NAMESPACE' inside the namespace where your class with privates is.

#define PHANTOM_PRIVATE_ACCESS                                                                                         \
    template<class>                                                                                                    \
    friend struct _PHNTM_Registrer
