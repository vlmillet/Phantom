// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <iosfwd>
#include <phantom/detail/core.h>

// Thansk to Jades Hobo from codeprojects for its console manipulators which served me as reference
// ! http://www.codeproject.com/KB/cpp/AddColorConsole.aspx

namespace phantom
{
namespace console
{
// narrow manipulators
PHANTOM_EXPORT_PHANTOM std::ostream& clr(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& push(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& pop(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_red(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_green(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_blue(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_white(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_cyan(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_magenta(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_yellow(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_black(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& fg_gray(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_red(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_green(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_blue(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_white(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_cyan(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_magenta(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_yellow(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_black(std::ostream& os);
PHANTOM_EXPORT_PHANTOM std::ostream& bg_gray(std::ostream& os);

PHANTOM_EXPORT_PHANTOM String createSpaces(int count);

PHANTOM_EXPORT_PHANTOM void setSize(unsigned short a_uiWidth, unsigned short height);

// wide manipulators
PHANTOM_EXPORT_PHANTOM std::wostream& clr(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& push(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& pop(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_red(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_green(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_blue(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_white(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_cyan(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_magenta(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_yellow(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_black(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& fg_gray(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_red(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_green(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_blue(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_white(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_cyan(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_magenta(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_yellow(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_black(std::wostream& os);
PHANTOM_EXPORT_PHANTOM std::wostream& bg_gray(std::wostream& os);

} // namespace console
} // namespace phantom
