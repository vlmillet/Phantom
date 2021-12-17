// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include <iostream>

/// registration includes

#include <phantom/class>
#include <phantom/field>
#include <phantom/main>
#include <phantom/method>

/// use includes

#include <phantom/lang/Application.h>
#include <phantom/lang/Class.h>
#include <phantom/lang/Field.h>
#include <phantom/lang/Method.h>

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
    .field<transient>("helloWorldTriggered", &_::helloWorldTriggered);
}

} // namespace MyNamespace

int main(int argc, char** argv)
{
    using namespace phantom::lang;

    // <- at this point no heap allocation have been made,
    // this is where you can configure allocations with MemoryTraits class
    // before Phantom starts the big registration work

    // initializes Phantom, install all pre-main registrations, create the 'main' module (the module related to the
    // .exe)
    Main app(main, "HelloWorld", argc, argv);

    // find the class by full qualified name through the Application singleton (which exists only during Main scope)
    auto metaClass = Application::Get()->findCppClass("MyNamespace::MyClass");

    PHANTOM_CLASSOF(Type::AlignmentComputer)->getMethod("bidule");

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
