// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/macros.h>
/* *********************************************** */

//==========================================================
// Macro for export / import
//==========================================================

#if !defined(PHANTOM_STATIC_LINK_PHANTOM)
#    if defined(PHANTOM_LIB_PHANTOM)
#        define PHANTOM_EXPORT_PHANTOM PHANTOM_VISIBILITY_EXPORT
#    else
#        define PHANTOM_EXPORT_PHANTOM PHANTOM_VISIBILITY_IMPORT
#    endif

#else // defined(PHANTOM_STATIC_LINK_PHANTOM)
#    define PHANTOM_EXPORT_PHANTOM

#endif // !defined(PHANTOM_STATIC_LINK_PHANTOM)

//==========================================================
// Deactivation of warning 4251
// "T1 needs a DLL interface to be used by T2 class clients"
//==========================================================

#ifdef _MSC_VER
#    pragma warning(disable : 4251)
#endif
