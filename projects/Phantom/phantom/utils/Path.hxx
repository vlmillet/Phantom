#pragma once

// haunt {

// clang-format off

#include "Path.h"

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
#include <phantom/struct>
#include <phantom/enum_class>
#include <phantom/static_constant>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallString.hxx"
#include "SmallVector.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Path")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<DirectoryEntries>("DirectoryEntries"); }
        PHANTOM_STRUCT(Path)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            /// missing symbol(s) reflection (, std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, DirectoryEntries&, ::std::error_code &, bool)>("ListDirectoryEntries", &_::ListDirectoryEntries)["false"]
            .staticMethod<bool(const Path&, DirectoryEntries&, bool)>("ListDirectoryEntries", &_::ListDirectoryEntries)["false"]
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("CreateDirectories", &_::CreateDirectories)
            .staticMethod<bool(const Path&)>("CreateDirectories", &_::CreateDirectories)
            .staticMethod<Path()>("TempFolder", &_::TempFolder)
            .staticMethod<bool(const Path&)>("Exists", &_::Exists)
            .staticMethod<bool(const Path&)>("IsAbsolute", &_::IsAbsolute)
            .staticMethod<bool(const Path&)>("IsGenericAbsolute", &_::IsGenericAbsolute)
            .staticMethod<bool(const Path&)>("IsWindowsAbsolute", &_::IsWindowsAbsolute)
            .staticMethod<bool(const Path&)>("IsUnixAbsolute", &_::IsUnixAbsolute)
            .staticMethod<bool(const Path&)>("IsDirectory", &_::IsDirectory)
            .staticMethod<bool(const Path&)>("IsRegularFile", &_::IsRegularFile)
            .staticMethod<bool(const Path&)>("IsSymLink", &_::IsSymLink)
            .staticMethod<Path(const Path&)>("Absolute", &_::Absolute)
            .staticMethod<bool(const Path&, const Path&)>("Equivalent", &_::Equivalent)
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, const Path&, ::std::error_code &)>("Copy", &_::Copy)
            .staticMethod<bool(const Path&, const Path&)>("Copy", &_::Copy)
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, const Path&, ::std::error_code &)>("Rename", &_::Rename)
            .staticMethod<bool(const Path&, const Path&)>("Rename", &_::Rename)
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("Remove", &_::Remove)
            .staticMethod<bool(const Path&)>("Remove", &_::Remove)
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("RemoveAll", &_::RemoveAll)
            .staticMethod<bool(const Path&)>("RemoveAll", &_::RemoveAll)
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, size_t, ::std::error_code &)>("ResizeFile", &_::ResizeFile)
            .staticMethod<bool(const Path&, size_t)>("ResizeFile", &_::ResizeFile)
            .staticMethod<Path()>("GetCurrentWorkingDir", &_::GetCurrentWorkingDir)
            .staticMethod<void(const Path&)>("SetCurrentWorkingDir", &_::SetCurrentWorkingDir)
            .enum_<Type>().values({
                {"unknown",_::Type::unknown},
                {"directory",_::Type::directory},
                {"regular",_::Type::regular},
                {"symlink",_::Type::symlink}})
            .end()
            .constant("separator", _::separator)
            .constant("npos", _::npos)
            .constructor<void()>()
            .constructor<void(StringView)>()
            .constructor<void(String const&)>()
            .constructor<void(StringView, const char)>()
            .constructor<void(const char*)>()
            .method<Path&(const StringView)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<String&()>("front", &_::front)
            .method<StringView() const>("front", &_::front)
            .method<String&()>("back", &_::back)
            .method<StringView() const>("back", &_::back)
            .method<void()>("popBack", &_::popBack)
            .method<void(const ::phantom::Path &)>("append", &_::append)
            .method<void()>("removeExtension", &_::removeExtension)
            .method<String&(size_t)>("operator[]", &_::operator[])
            .method<StringView(size_t) const>("operator[]", &_::operator[])
            .method<Strings::const_iterator() const>("begin", &_::begin)
            .method<Strings::const_iterator() const>("end", &_::end)
            .method<String() const>("platformString", &_::platformString)
            .method<String() const>("genericString", &_::genericString)
            .method<String(StringView) const>("customString", &_::customString)
            .method<size_t(const Path&) const>("find", &_::find)
            .method<size_t(const Path&) const>("rfind", &_::rfind)
            .method<bool() const>("isAbsolute", &_::isAbsolute)
            .method<bool() const>("isGenericAbsolute", &_::isGenericAbsolute)
            .method<bool() const>("isWindowsAbsolute", &_::isWindowsAbsolute)
            .method<bool() const>("isUnixAbsolute", &_::isUnixAbsolute)
            .method<bool() const>("exists", &_::exists)
            .method<void()>("clear", &_::clear)
            .method<Type() const>("getType", &_::getType)
            .method<bool() const>("isDirectory", &_::isDirectory)
            .method<bool() const>("isRegularFile", &_::isRegularFile)
            .method<bool() const>("isSymLink", &_::isSymLink)
            .method<Path() const>("parentPath", &_::parentPath)
            .method<Path(const Path&) const>("childPath", &_::childPath)
            .method<bool() const>("empty", &_::empty)
            .method<bool() const>("isRoot", &_::isRoot)
            .method<StringView() const>("filename", &_::filename)
            .method<StringView() const>("stem", &_::stem)
            .method<StringView() const>("extension", &_::extension)
            .method<Path() const>("absolute", &_::absolute)
            .method<bool(const Path&) const>("operator==", &_::operator==)
            .method<bool(const Path&) const>("operator!=", &_::operator!=)
            .method<bool(const Path&) const>("operator<", &_::operator<)
            .method<size_t() const>("size", &_::size)
            .method<Path(const Path&) const>("relative", &_::relative)
            .method<bool(const Path&) const>("hasChildPath", &_::hasChildPath)
            .method<Path() const>("canonical", &_::canonical)
            .method<Path() const>("subPath", &_::subPath)
            .method<Path(size_t) const>("subPath", &_::subPath)
            ;
        }
        PHANTOM_STRUCT(DirectoryEntry)
        {
            this_()
            .constructor<void()>()
            .constructor<void(const Path&, ::phantom::Path::Type)>()
            .method<const Path&() const>("path", &_::path)
            .method<bool() const>("isRegularFile", &_::isRegularFile)
            .method<bool() const>("isDirectory", &_::isDirectory)
            .method<bool() const>("isSymLink", &_::isSymLink)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Path")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
