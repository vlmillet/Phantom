# Phantom
C++ Reflection and Scripting Library

----
## Phantom
----
Phantom is a **C++ reflection library**, feature complete, self-contained (no stl container or other library dependency), highly customizable (allocations, reflection generation) and non intrusive (reflection can be in complete separate files from your source code). 
It has been first developed as a home-made project and then was used inside the game industry as a robust foundation for quick game engine development. 

After years of use inside a professional game studio, it is now made available under a permissive license for whoever want to use it or participate to its development.

It comes with bêta extensions providing scripting features : [Phantom.Code](#phantomcode--soon-on-github-) and  [Phantom.JIT](#phantomjit--soon-on-github-)

----

| [Compilers / Platforms](#compilers--platforms) \| [License / Support / Domations](#license--support--donations) \| [Phantom In a Nutshell](#phantom-in-a-nutshell) \| [HelloWorld](#helloworld) \| [Features](#features) \| [Haunt Reflection Generator](#haunt-reflection-generator-only-available-for-windows-cross-platform-incoming) \| [Integration](#integration) |
:----------------------------------------------------------: |

### Compilers / Platforms 
- Microsoft Visual Studio 2015 / 2017 / 2019 (requires at least C++11)
- Clang version  7 and greater

It has been tested on Windows 7/10, Ubuntu and ORBIS platforms. 
More tests are required as it's scope was limited during development.
<sub>As being mainly used for game engine/editor development, Phantom had a poor maintenance on other platforms than Windows. But things are changing slowly but surely...</sub>

### License / Support / Donations

This project is under a MIT license which is one of the highest permissive distribution license. I receive no salary or royalties for developing it and I won't, by pure believes in sharing stuff for a better world <3. Anyone willing to donate is very welcome to help me maintaining and improving it on my spare time :) 

### Phantom In A Nutshell

- Classical features : **class, method, field, function, enum, constructor, property ...**
- **Template class reflection**
- **Automatic reflection generation** for fast iterations with *Haunt*, a ***Clang* based tool** featuring : 
	- code comment markers
	- dependency resolution (no reflection generated for fields or methods with missing reflected types)
	- highly customizable input/output
	- trivial external libraries reflection and integration 
- **Plugins**, with user **Module/Package/Source** symbols organisation 
- **Custom user allocator** : provide your own allocate/deallocate functions (very few allocations before main for better memory control)
- **Modern C++ with minimal macros use** (only those to avoid unreadable code are used) 
- **On demand registration** : if requested, even if everything is reflected, only accessed class will register members (**fast startup** and **low memory use**, no matter your codebase size)
- **Optimized meta programming compilation time** using *meta-typedef* technique
- **Non intrusive** (you can separate your code from reflection code, even in separate libraries)
- **No third-party library**
- **No RTTI**
- Home made containers with respective *small buffer* and *views* optimizations (rare allocations when passing arguments)
- Automatic namespace detection at registration (ask for the magic trick ;))
- Used in the **game industry** (compiles on ORBIS)
- **Extensions**
	- *Phantom.Code* (soon)
		- **Lite C++ grammar and semantic with import model (CppLite)** for building C++ like code at runtime
		- AST objects for **building your own language above native C++**  
	- *Phantom.Jit* (soon)
		- ***LLVM* based JIT** to run your own language at high performance and debug it inside Visual Studio (on the fly PDB generation)

### HelloWorld

Code:
```cp
#include <iostream>

/// registration includes

#include <phantom/class> 
#include <phantom/method>
#include <phantom/field>
#include <phantom/main>

/// use includes

#include <phantom/lang/Class.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/Application.h>

namespace MyNamespace
{
class MyClass
{
public:
    const char* HelloWorld()
    {
        helloWorldTriggered = true;
        return "HelloWorld !";
    }

protected:
    bool helloWorldTriggered = false;
};

// automatic namespace detection and registration

PHANTOM_CLASS(MyClass) // minimal macro usage
{
    this_()
        .public_()
            .method("HelloWorld", &_::HelloWorld) // or .method<const char*()>(...) to remove possible ambiguities
        .protected_()
            .field<transient>("helloWorldTriggered", &_::helloWorldTriggered)
        ;
}

} // namespace MyNamespace

int main(int argc, char** argv)
{
    using namespace phantom::lang;

    // initializes Phantom, install all pre-main registrations, create the 'main' module (the module related to the .exe)
    Main app(main, "MyApplication", argc, argv);

    // find the class by full qualified name through the Application singleton (which exists only during Main scope)
    auto metaClass = Application::Get()->findCppClass("MyNamespace::MyClass");

    // use RAII + meta class to build a temporary instance on the stack
    auto scopedInstance = metaClass->scopedConstruct(alloca(metaClass->getSize()));

    // get the value of the 'helloWorldTriggered' field
    while (!metaClass->getField("helloWorldTriggered")->get<bool>(scopedInstance))
    {
        // invoke the 'HelloWorld()' method
        const char* helloWorldTxt = metaClass->getMethod("HelloWorld()")->invoke<const char*>(scopedInstance);
        // print the result string
        std::cout << helloWorldTxt << std::endl;
    }
    return 0;
}
```

### Features

#### List of reflected C++ symbols 

Almost all C++ symbols are reflectable by Phantom. 

Here is a list of what is/can be introspected :
- namespace, namespace alias, inline namespace
- types
  - fundamental (int, float, double, ...)
  - typedef / using
  - enum (+anonymous +class)
  - pointer
  - reference (l-value and r-value)
  - array
  - const / volatile qualified
  - class / struct / union (+anonymous)
    - field (static) 
    - constructors
    - methods (static, virtual, pure virtual, cvr qualifiers)
    - typedef / using / friend
    - member types (recursive, except for template inside other templates)
    - properties
  - template (with default template parameters + variadics)
  - template specialization (partial and full specializations)
  - access specifier (public, protected, private)
  - STL compliant containers
- function
- template function instantiation
- global variable
- alias template
- plugin (.dll/.dylib/.so)

<sub>Few exceptions are template functions (only their instances can be) and variadic functions (there is a work done to solve this but it requires ABI knowledges and it takes time).</sub>

Self-introspection and, then, recursive reflection are implemented (Class has a meta-class). It has some power for building self-feeding interpreted languages (such as LUA or Ruby). 

#### Module > Package > Source > Symbol

Phantom reflection is organized by *modules* containing *packages*, containing *sources*, containing *symbols*. This is close to the import/module model you can find in modern languages such as *Python*.  This structure ensure unicity accross different libraries. For example the unique full name of `phantom::lang::Field` is ```Phantom:phantom.lang.Field.Field```. 
- ```Phantom``` is the *module*. 
- ```phantom.lang``` is the *package*. 
- ```Field``` (first) is the *source* (actual 'module' equivalent in C++20 or Python). 
- ```Field``` (second) is the actual class *symbol*.

<sub> NOTE: you can compare a *source* in Phantom to a *module* in Python. </sub>

Here is how you can define your own Package/Source for your registrations
```cp
PHANTOM_PACKAGE("MyPackage0.MyPackage1");
PHANTOM_SOURCE("MySource");
PHANTOM_CLASS("MyClass") { ... }
PHANTOM_END("MySource");
PHANTOM_END("MyPackage0.MyPackage1");
```
or more simply
```cp
PHANTOM_SOURCE("MyPackage0.MyPackage1.MySource");
PHANTOM_CLASS("MyClass") { ... }
PHANTOM_END("MyPackage0.MyPackage1.MySource");
```
<sub>(the module will be automatically detected by the Phantom registration system)</sub>
#### On demand member registration
> **Only keep what you need** <sub>you should</sub>
> 
When you launch your application with Phantom, only the symbols at global or namespace scopes will be registered by default. Every class member will wait for any access in the reflection system to install. This allows to have everything reflected and still have a **very fast startup** and **low memory use** if not everything is used. 


#### Heap allocation control
99% of the time, Phantom will never perform heap allocation before entering ***main*** function ; this might happen if you really have a big module with lot of symbols registered. Once in the ***main*** you can use MemoryTraits in phantom to provide your own allocation functions.

#### Template Class Reflection 

Phantom reflects template classes very easily. Full, partial and variadic template specializations are supported too.

Every template reflection must be isolated into its own file to be used correcly accross multiple translation units. Indeed a template reflection registration is not the same as a class reflection registration (as templates have no real existence in compiled code). 

This is why they must be included wherever an instance of this template is used inside another reflection scope. To do so, template files must be included in between ```#include <phantom/template-only-push>``` and ```#include <phantom/template-only-pop>```  

See below an example of ```EASTL/basic_string.hxx``` used in a game engine for better understanding (it is easier than it look likes).

```cp
#pragma once

#include <EASTL/string.h>

#include <phantom/class>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/meta_type>

#include <phantom/lang/StringClassT.h>

#include <phantom/template-only-push> // < begin template includes here
#include "allocator.hxx"
#include <std/initializer_list.hxx>
#include <phantom/template-only-pop> // < end template includes here

// 'PHANTOM_META_TYPE_T' tells phantom to use phantom::lang::StringClassT<> instead of phantom::lang::ClassT<>
// as the meta type for every reflected instance of basic_string.

PHANTOM_META_TYPE_T((typename, typename), (T, Alloc), eastl::basic_string, phantom::lang::StringClassT);

namespace eastl
{
    PHANTOM_CLASS_T((typename, typename), (T, Alloc), basic_string)
    {
        using CharT = T;
        using AllocatorT = Alloc;
        using ThisType = eastl::basic_string<T, Alloc>;
        using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
        using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
        using size_type = typedef_<PHANTOM_TYPENAME _::size_type>;
        using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;

        this_()
            .public_()
                .PHANTOM_T constant("npos", _::npos)
                .PHANTOM_T typedef_<size_type>("size_type")
                .PHANTOM_T typedef_<value_type>("value_type")
                .PHANTOM_T typedef_<const_iterator>("const_iterator")
                .PHANTOM_T typedef_<iterator>("iterator")
                PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value),
                    .PHANTOM_T constructor<void()>()
                )
                .PHANTOM_T constructor<void(ThisType const&), default_>()
                .PHANTOM_T constructor<void(ThisType&&)>()
                .PHANTOM_T constructor<void(::std::initializer_list<CharT>, const AllocatorT&)>()
                .PHANTOM_T constructor<void(CharT const*, size_t)>()
                .PHANTOM_T constructor<void(CharT const*)>()
                .PHANTOM_T method<ThisType& (ThisType const&), default_>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
                .PHANTOM_T method<ThisType& (ThisType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
                .PHANTOM_T method<ThisType& (CharT const*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
                .PHANTOM_T method<ThisType& (CharT)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
                .PHANTOM_T method<CharT const* () const>("begin", &_::begin)
                .PHANTOM_T method<CharT const* () const>("end", &_::end)
                .PHANTOM_T method<CharT* ()>("begin", &_::begin)
                .PHANTOM_T method<CharT* ()>("end", &_::end)
                .PHANTOM_T method<size_t() const>("size", &_::size)
                .PHANTOM_T method<size_t() const>("length", &_::length)
                .PHANTOM_T method<void(size_t)>("resize", &_::resize)
                .PHANTOM_T method<void(size_t, CharT)>("resize", &_::resize)
                .PHANTOM_T method<size_t() const>("capacity", &_::capacity)
                .PHANTOM_T method<void(size_t)>("reserve", &_::reserve)
                .PHANTOM_T method<void()>("clear", &_::clear)
                .PHANTOM_T method<bool() const>("empty", &_::empty)
                .PHANTOM_T method<CharT const& (size_t) const>("operator[]", &_::operator[])
                .PHANTOM_T method<CharT& (size_t)>("operator[]", &_::operator[])
                .PHANTOM_T method<CharT const& () const>("back", &_::back)
                .PHANTOM_T method<CharT& ()>("back", &_::back)
                .PHANTOM_T method<CharT const& () const>("front", &_::front)
                .PHANTOM_T method<CharT& ()>("front", &_::front)
                .PHANTOM_T method<ThisType& (CharT const*)>("operator+=", &_::operator+=)
                .PHANTOM_T method<ThisType& (ThisType const&)>("operator+=", &_::operator+=)
                .PHANTOM_T method<ThisType& (CharT)>("operator+=", &_::operator+=)
                .PHANTOM_T method<ThisType& (CharT const*)>("append", &_::append)
                .PHANTOM_T method<ThisType& (ThisType const&)>("append", &_::append)
                .PHANTOM_T method<void(CharT)>("push_back", &_::push_back)
                .PHANTOM_T method<ThisType& (CharT const*)>("assign", &_::assign)
                .PHANTOM_T method<ThisType& (CharT const*, size_t)>("assign", &_::assign)
                .PHANTOM_T method<ThisType& (CharT const*, size_t)>("append", &_::append)
                .PHANTOM_T method<ThisType& (const ThisType&, size_t, size_t)>("append", &_::append)
                .PHANTOM_T method<ThisType& (size_t, ThisType const&)>("insert", &_::insert)
                .PHANTOM_T method<ThisType& (size_t, CharT const*, size_t)>("insert", &_::insert)
                .PHANTOM_T method<ThisType& (size_t, size_t)>("erase", &_::erase)
                .PHANTOM_T method<iterator(const_iterator)>("erase", &_::erase)
                .PHANTOM_T method<iterator(const_iterator, const_iterator)>("erase", &_::erase)
                .PHANTOM_T method<void(ThisType&)>("swap", &_::swap)
                .PHANTOM_T method<void()>("pop_back", &_::pop_back)
                .PHANTOM_T method<CharT const* () const>("c_str", &_::c_str)
                .PHANTOM_T method<CharT const* () const>("data", &_::data)
                .PHANTOM_T method<size_t(CharT const*, size_t) const>("find", &_::find)["0"]
                .PHANTOM_T method<size_t(ThisType const&, size_t) const>("find", &_::find)["0"]
                .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_of", &_::find_first_of)["0"]
                .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_of", &_::find_first_of)["0"]
                .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_of", &_::find_last_of)["0"]
                .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_of", &_::find_last_of)["0"]
                .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_not_of", &_::find_first_not_of)["0"]
                .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_not_of", &_::find_first_not_of)["0"]
                .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_not_of", &_::find_last_not_of)["npos"]
                .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_not_of", &_::find_last_not_of)["npos"]
                .PHANTOM_T method<ThisType(size_t, size_t) const>("substr", &_::substr)["npos"]
                .PHANTOM_T method<int(ThisType const&) const>("compare", &_::compare)
                .PHANTOM_T method<int(CharT const*) const>("compare", &_::compare)
                .PHANTOM_T method<int(size_t, size_t, CharT const*) const>("compare", &_::compare)
                .PHANTOM_T method<int(size_t, size_t, ThisType const&) const>("compare", &_::compare)
        ;
    }
}
```

and here is an example where ```EASTL/basic_string.hxx``` is used :

```cp
#pragma once

#include "String.h"

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/function>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <EASTL/basic_string.hxx>

#include <phantom/template-only-pop>

namespace GameEngine {
PHANTOM_PACKAGE("GameEngine.Core")
    PHANTOM_SOURCE("String")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<typedef_<String> >("String"); }
        #endif // PHANTOM_NOT_TEMPLATE
	
    PHANTOM_END("String")
PHANTOM_END("GameEngine.Core")
}

```

#### Custom Meta Types

 You can customize what meta type phantom will use to register your class : 
- for simple classes
```cp
...
#include <phantom/meta_type>
...
PHANTOM_META_TYPE(<your-ns>::<your_type>, <your-metatype>)
namespace <your-ns> 
{
    PHANTOM_CLASS(<your_type>) { ... }
}
``` 
- for template classes (as seen just above in ```EASTL/basic_string.hxx```) with 

```cp
PHANTOM_META_TYPE_T(<tpl-signature>, <your-ns>::<your_type>, <your-metatype>)
namespace <your-ns> 
{
    PHANTOM_CLASS_T(<tpl-signature>, <your_type>) { ... }
}
``` 
these macros are in ```<phantom/meta_type>```

### Haunt Reflection Generator <sub>(only available for Windows, cross platform incoming)</sub>

Phantom comes with a reflection generator based on **Clang** which generates files containing fined control reflection output. This is a **boost for iterations and productivity**. 
**Haunt** is not mandatory like **QT/moc** is. You can create a whole project without. It is just a really handy team mate for hard days of refactoring your other human team mates' code ... <sub>you know what I mean, I know you do</sub>

A pretty cool use of Haunt is to generate reflection for external libraries. 
Libraries that I've already *haunted* will be available in side projects on GitHub when I'll have some time. 

Among them you will find :
-	imgui
-	mathfu
-	vulkan

And more will come.

Quick list of features :
- Driven by command-comment in code (ex: @metadata, @property, @off, @transient ...)
	- or macros (ex: HAUNT_STOP, HAUNT_PAUSE, HAUNT_RESUME  ...)
- Invoked on root-path recursively instead of per-file (easier project management). 
	- Special files can be created for this purpose.
		- **haunt.ignore** file can be used to filter out files.
		- **haunt.select** file can be used to select files to parse one by one.
		- **haunt.bind** file tells Haunt where  to find manual symbol reflection files ; with it you can easily mix manual and auto generated reflection.
		- **haunt.cmd** can be used to provide a full command line.
- Caching symbols and solving dependencies based on include search paths
- Lot of command line options for fine symbol reflection tuning:
	-  **-f\<feature\>** : run *haunt.exe*  for details
	- **-\-symbol-ignore** removed a symbol by full qualified name
	- **-\-symbol-if** adds a compile time static if to remove some template members
  
Example of file generated with Haunt :
```cp
#pragma once

// haunt {

#include "Class.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Class")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(BaseClass)
        {
            this_()
            .constructor<void()>()
            .constructor<void(Class*, size_t, Access)>()["Access::Public"]
            .field("access", &_::access)
            .field("baseClass", &_::baseClass)
            .field("offset", &_::offset)
            .method<Class*() const>("operator Class*", &_::operator notypedef<Class*>)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<BaseClasses>("BaseClasses"); }
        PHANTOM_STRUCT(StructBuilder)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
            .method<StructBuilder&(StringView, size_t)>("begin", &_::begin)["0"]
            .method<StructBuilder&(Type*, StringView, size_t, uint)>("field", &_::field)["0"]["~0u"]
            .method<Class*()>("end", &_::end)
            ;
        }
        PHANTOM_CLASS(Class)
        {
            using AggregateFields = typedef_< phantom::lang::AggregateFields>;
            using Classes = typedef_< phantom::lang::Classes>;
            using ExtraData = typedef_<_::ExtraData>;
            using Methods = typedef_< phantom::lang::Methods>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Signals = typedef_< phantom::lang::Signals>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            using Subroutines = typedef_< phantom::lang::Subroutines>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using ValueMembers = typedef_< phantom::lang::ValueMembers>;
            using Variants = typedef_< phantom::Variants>;
            using VirtualMethodTables = typedef_< phantom::lang::VirtualMethodTables>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ClassType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<Class*()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .struct_<ExtraData>()
                .inherits<::phantom::lang::ClassType::ExtraData>()
                .method<void(), virtual_|override_>("PHANTOM_CUSTOM_VIRTUAL_DELETE", &_::ExtraData::PHANTOM_CUSTOM_VIRTUAL_DELETE)
                .constructor<void()>()
                .field("m_uiFieldMemoryOffset", &_::ExtraData::m_uiFieldMemoryOffset)
                .field("m_uiClassPtrOffset", &_::ExtraData::m_uiClassPtrOffset)
                .field("m_uiStateMachineDataPtrOffset", &_::ExtraData::m_uiStateMachineDataPtrOffset)
                .field("m_bHasVTablePtr", &_::ExtraData::m_bHasVTablePtr)
                .field("m_bHasStateMachineDataPtr", &_::ExtraData::m_bHasStateMachineDataPtr)
            .end()
        
        .public_()
            .constructor<void()>()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
        
        .protected_()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()
        
        .public_()
            .method<Class*(Class*) const>("getCommonBaseClass", &_::getCommonBaseClass)
            .method<void(Methods&) const>("filtersNonOverloadedPureVirtualMethodsCascade", &_::filtersNonOverloadedPureVirtualMethodsCascade)
            .method<void(Methods&) const>("findPureVirtualMethods", &_::findPureVirtualMethods)
            .method<Subroutine*(StringView) const>("getSubroutineCascade", &_::getSubroutineCascade)
            .method<Subroutine*(StringView, TypesView, Modifiers) const>("getSubroutineCascade", &_::getSubroutineCascade)["0"]
            .method<void(ValueMembers&) const>("getValueMembersCascade", &_::getValueMembersCascade)
            .method<void(Method*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)
            .method<void(StringView, Signature*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)
            .method<void(VirtualMethodTable*, Methods*, Methods*) const>("sortBaseAndRootMethods", &_::sortBaseAndRootMethods)
            .method<Method*(StringView) const>("getMethodCascade", &_::getMethodCascade)
            .method<Method*(StringView, TypesView, Modifiers) const>("getMethodCascade", &_::getMethodCascade)["0"]
            .method<Method*(StringView, Type*, Modifiers) const>("getMethodCascade", &_::getMethodCascade)["0"]
            .method<ValueMember*(StringView) const>("getValueMemberCascade", &_::getValueMemberCascade)
            .method<void(Type*, ValueMembers&) const>("findPublicValueMembersPointingValueTypeCascade", &_::findPublicValueMembersPointingValueTypeCascade)
            .method<Field*(StringView) const>("getFieldCascade", &_::getFieldCascade)
            .method<Field*(size_t) const, virtual_|override_>("getFieldByOffset", &_::getFieldByOffset)
            .method<Field*(size_t) const, virtual_|override_>("getFieldAtOffset", &_::getFieldAtOffset)
            .using_("ClassType::getFields")
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)
            .method<Property*(StringView) const>("getPropertyCascade", &_::getPropertyCascade)
            .method<StaticMethod*(StringView, TypesView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)
            .method<StaticMethod*(StringView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)
            .method<StaticField*(StringView) const>("getStaticFieldCascade", &_::getStaticFieldCascade)
            .method<Class*() const, virtual_|override_>("asClass", &_::asClass)
            .method<size_t(size_t) const>("getVirtualMethodTableSize", &_::getVirtualMethodTableSize)
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_|override_>("acceptsSubroutine", &_::acceptsSubroutine)["NULL"]
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_>("acceptsDerivedClassSubroutine", &_::acceptsDerivedClassSubroutine)
            .method<Class*(size_t) const>("getBaseClass", &_::getBaseClass)
            .method<void*(void*, size_t) const>("baseClassCast", &_::baseClassCast)
            .method<const void*(const void*, size_t) const>("baseClassCast", &_::baseClassCast)
            .method<size_t(size_t) const>("getInheritanceLevel", &_::getInheritanceLevel)
            .method<size_t(Class*) const>("getInheritanceLevelFromBase", &_::getInheritanceLevelFromBase)
            .method<bool(Symbol*) const, virtual_|override_>("hasPublicMember", &_::hasPublicMember)
            .method<bool(Symbol*) const, virtual_|override_>("hasProtectedMember", &_::hasProtectedMember)
            .method<bool(Symbol*) const, virtual_|override_>("hasPrivateMember", &_::hasPrivateMember)
            .method<bool() const, virtual_|override_>("isListInitializable", &_::isListInitializable)
            .method<size_t(size_t) const, virtual_>("getVirtualMethodCount", &_::getVirtualMethodCount)
            .method<VirtualMethodTables const&() const>("getVirtualMethodTables", &_::getVirtualMethodTables)
            .method<void(Class*, Access), virtual_>("addBaseClass", &_::addBaseClass)["Access::Public"]
            .method<void(Class*, size_t, Access), virtual_>("addBaseClass", &_::addBaseClass)["Access::Public"]
            .method<const BaseClasses&() const>("getBaseClasses", &_::getBaseClasses)
            .method<Access(Class*) const>("getBaseClassAccess", &_::getBaseClassAccess)
            .method<Access(Class*) const>("getBaseClassAccessCascade", &_::getBaseClassAccessCascade)
            .method<bool(Class*, Access*) const>("hasBaseClass", &_::hasBaseClass)["nullptr"]
            .method<bool(Class*) const>("hasBaseClassCascade", &_::hasBaseClassCascade)
            .method<Class*(size_t) const>("getDerivedClass", &_::getDerivedClass)
            .method<Classes const&() const>("getDerivedClasses", &_::getDerivedClasses)
            .method<void(Classes&) const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)
            .method<Classes const&() const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)
            .method<bool(Class*, Access*) const>("hasDerivedClass", &_::hasDerivedClass)["nullptr"]
            .method<bool(Class*, Access*) const>("hasDerivedClassCascade", &_::hasDerivedClassCascade)["nullptr"]
            .method<bool(LanguageElement*) const>("hasMemberCascade", &_::hasMemberCascade)
            .method<ptrdiff_t(size_t) const>("getBaseClassOffset", &_::getBaseClassOffset)
            .method<void(Class*, ptrdiff_t)>("setBaseClassOffset", &_::setBaseClassOffset)
            .method<ptrdiff_t(Class*) const>("getBaseClassOffset", &_::getBaseClassOffset)
            .method<ptrdiff_t(Class*) const>("getBaseClassOffsetCascade", &_::getBaseClassOffsetCascade)
            .method<ptrdiff_t(Type*) const, virtual_|override_>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            .method<ptrdiff_t(Class*) const>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            .method<bool() const>("isRootClass", &_::isRootClass)
            .method<bool(Class*) const>("isA", &_::isA)
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)
            .method<TypeRelation(Type*) const, virtual_|override_>("getRelationWith", &_::getRelationWith)
            .method<void(Method*), virtual_|override_>("addMethod", &_::addMethod)
            .method<void(Signal*), virtual_>("addSignal", &_::addSignal)
            .method<void(Signal*), virtual_>("removeSignal", &_::removeSignal)
            .method<Signals const&() const>("getSignals", &_::getSignals)
            .method<Signal*(StringView) const>("getSignal", &_::getSignal)
            .method<Signal*(StringView) const>("getSignalCascade", &_::getSignalCascade)
            .method<Method*(StringView) const>("getSlot", &_::getSlot)
            .method<Method*(StringView) const>("getSlotCascade", &_::getSlotCascade)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(ValueMember*, Expression*)>("setOverriddenDefaultExpression", &_::setOverriddenDefaultExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpression", &_::getOverriddenDefaultExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpressionCascade", &_::getOverriddenDefaultExpressionCascade)
            .method<bool() const, virtual_|override_>("isPolymorphic", &_::isPolymorphic)
            .using_("Type::allocate")
            .using_("Type::deallocate")
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void*() const>("getSingleton", &_::getSingleton)
            .method<size_t() const>("getInstanceCount", &_::getInstanceCount)
            .method<void*(Class*, void*) const>("upcast", &_::upcast)
            .method<void const*(Class*, void const*) const>("upcast", &_::upcast)
            .method<void*(Class*, void*) const>("downcast", &_::downcast)
            .method<void const*(Class*, void const*) const>("downcast", &_::downcast)
            .method<void*(Class*, void*) const>("cast", &_::cast)
            .method<void const*(Class*, void const*) const>("cast", &_::cast)
            .method<void*(Type*, void*) const, virtual_|override_>("upcast", &_::upcast)
            .method<void*(Type*, void*) const, virtual_|override_>("downcast", &_::downcast)
            .method<void*(Type*, void*) const, virtual_|final_>("cast", &_::cast)
            .method<void const*(Type*, void const*) const, virtual_|final_>("cast", &_::cast)
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)
            .method<void*(void*) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)
            .method<void*(void*, Constructor*, void**) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)
            .method<void(void*) const, virtual_|override_>("placementDeleteInstance", &_::placementDeleteInstance)
            .method<const Variant&(StringView) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)
            .method<const Variant&(StringWithHash) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)
            .method<void(StringView, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)
            .method<void(StringWithHash, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)
            .method<bool(StringWithHash) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)
            .method<bool(StringView) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<bool(void const*, void const*) const, virtual_|override_>("equal", &_::equal)
            .method<bool() const, virtual_>("hasNewVTable", &_::hasNewVTable)
            .method<bool(Type*) const, virtual_|override_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<ExtraData*() const>("getExtraData", &_::getExtraData)
            .method<void()>("addNewVirtualMethodTable", &_::addNewVirtualMethodTable)
            .method<void(), virtual_|override_>("addImplicitDefaultConstructor", &_::addImplicitDefaultConstructor)
            .method<void(), virtual_|override_>("addImplicitCopyConstructor", &_::addImplicitCopyConstructor)
            .method<void(), virtual_|override_>("addImplicitCopyAssignmentOperator", &_::addImplicitCopyAssignmentOperator)
            .method<void(), virtual_|override_>("addImplicitMoveConstructor", &_::addImplicitMoveConstructor)
            .method<void(), virtual_|override_>("addImplicitMoveAssignmentOperator", &_::addImplicitMoveAssignmentOperator)
            .method<void(), virtual_|override_>("addImplicitDestructor", &_::addImplicitDestructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitDefaultConstructor", &_::canHaveImplicitDefaultConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitCopyConstructor", &_::canHaveImplicitCopyConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitCopyAssignmentOperator", &_::canHaveImplicitCopyAssignmentOperator)
            .method<bool() const, virtual_|override_>("canHaveImplicitMoveConstructor", &_::canHaveImplicitMoveConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitMoveAssignmentOperator", &_::canHaveImplicitMoveAssignmentOperator)
            .method<VirtualMethodTable*() const, virtual_>("createVirtualMethodTable", &_::createVirtualMethodTable)
            .method<VirtualMethodTable*(VirtualMethodTable*) const, virtual_>("deriveVirtualMethodTable", &_::deriveVirtualMethodTable)
            .method<SymbolExtension*(Class*, size_t) const>("getExtensionCascade", &_::getExtensionCascade)["0"]
            .method<void(void*), virtual_>("registerInstance", &_::registerInstance)
            .method<void(void*), virtual_>("unregisterInstance", &_::unregisterInstance)
            .method<InstanceCache*()>("getOrCreateInstanceCache", &_::getOrCreateInstanceCache)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<Strings() const>("getBaseClasseNames", &_::getBaseClasseNames)
            .method<void(Strings), virtual_>("setBaseClasseNames", &_::setBaseClasseNames)
            .method<void(Methods), virtual_>("setMethods", &_::setMethods)
        
        .protected_()
            .method<void()>("_onNativeElementsAccess", &_::_onNativeElementsAccess)
            .method<void()>("_onNativeElementsAccessImpl", &_::_onNativeElementsAccessImpl)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Class")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }

```
Yes this really saves you a lot of time :)
  
### Integration

[Premake](https://premake.github.io/) is used as my cross platform project generator.

<sub>(CMake was not flexible enough for working on exotic console platforms, and honestly, for having used both, really find premake more powerful)</sub>

Just run the *Premake_vs201X.bat* under windows to build a Visual Studio solution. Under linux and other platforms, you will need to use the command line.

## Phantom.Code <sub> (soon on GitHub) </sub>

Provide a scripting language layer upon C++ and Phantom, building a nice and transparent native -> runtime interface for interacting with native C++.

## Phantom.JIT <sub> (soon on GitHub) </sub>

A jit compiler based on **llvm** and **MCJIT** is in development with great features (among them is on-the-fly **PDB** generation and hot reloading)
