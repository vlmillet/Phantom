// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "phantom/phantom.h"
/* *********************************************** */

namespace phantom
{
PHANTOM_EXPORT_PHANTOM void print(const char*, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(StringView, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(bool, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(char, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(schar, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(uchar, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(short, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(ushort, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(int, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(uint, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(long, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(ulong, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(longlong, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(ulonglong, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(float, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(double, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(longdouble, int channel = 0, int options = 0);
PHANTOM_EXPORT_PHANTOM void print(void*, int channel = 0, int options = 0);

} // namespace phantom
