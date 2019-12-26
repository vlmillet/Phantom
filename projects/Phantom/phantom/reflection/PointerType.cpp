// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "PointerType.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
void PointerType::less(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] < *(byte**)a_pArgs[1];
}

void PointerType::greater(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] > *(byte**)a_pArgs[1];
}

void PointerType::lessEqual(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] <= *(byte**)a_pArgs[1];
}

void PointerType::greaterEqual(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] >= *(byte**)a_pArgs[1];
}

void PointerType::equal(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] == *(byte**)a_pArgs[1];
}

void PointerType::notEqual(void** a_pArgs, void* a_pOutput)
{
    *(bool*)a_pOutput = *(byte**)a_pArgs[0] != *(byte**)a_pArgs[1];
}

void PointerType::assignment(void** a_pArgs, void* a_pOutput)
{
    *(byte**)a_pArgs[0] = *(byte**)a_pArgs[1];
    *(byte***)a_pOutput = (byte**)a_pArgs[0];
}

} // namespace reflection
} // namespace phantom
