#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "stackStructsAndEnums.h"
#include <assert.h>

#define STACK_CTOR(stackName, stackInfoName, capacity)\
    stackInfoName.nameOfFile = __FILE__;\
    stackInfoName.nameOfFunct = __func__;\
    stackInfoName.numOfLine = __LINE__;\
    stackCtor(&stackName, capacity, #stackName, stackInfoName);\

    int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info creationInfo);

    int stackPush (stack_t* stack, stackElement_t value);

    int stackPop (stack_t* stack, stackElement_t* ptrToVariable);

    int stackDtor (stack_t* stack);

#endif
