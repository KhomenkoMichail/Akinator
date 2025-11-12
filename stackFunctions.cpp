#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <typeinfo>
#include <errno.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "stackFunctions.h"
#include "stackStructsAndEnums.h"

const stackElement_t POISON = 0xBADBABE;
const int MAX_CAPACITY = 100000000;

#ifdef CANARY_PROTECTION
    const int CANARY = 0xBEEFFACE;
#endif

int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info creationInfo) {
    assert(stack);
    assert(nameOfStack);

    stack->size = 0;
    stack->capacity = capacity;
    stack->nameOfStack = nameOfStack;
    stack->errorCode = 0;
    stack->nameOfType = txDemangle(typeid(stackElement_t).name()).c_str();

    stack->stackInfo.nameOfFunct = creationInfo.nameOfFunct;
    stack->stackInfo.nameOfFile = creationInfo.nameOfFile;
    stack->stackInfo.numOfLine = creationInfo.numOfLine;

    if ((capacity <= 0) || (capacity > MAX_CAPACITY)) {
        stack->errorCode |= badCapacity;
        return stack->errorCode;
    }


    stack->data = (stackElement_t*)calloc(capacity, sizeof(stackElement_t));
    for(ssize_t numOfElement = 0; numOfElement < capacity; numOfElement++)
        stack->data[numOfElement] = POISON;


    return stack->errorCode;
}

int stackPush (stack_t* stack, stackElement_t value) {
    assert(stack);

    stack->data[stack->size] = value;
    stack->size++;

    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->data = (stackElement_t*)realloc(stack->data, stack->capacity);

        if (stack->data == NULL)
            stack->errorCode = badRealloc;

        else {
            for(ssize_t numOfElement = stack->size; numOfElement < stack->capacity; numOfElement++)
                stack->data[numOfElement] = POISON;
        }
    }

    return stack->errorCode;
}

int stackPop (stack_t* stack, stackElement_t* ptrToVariable) {
    assert(ptrToVariable);
    assert(stack);

    if (stack->size == 0) {
        stack->errorCode |= noElementsForPop;
        return stack->errorCode;
    }

    stack->size--;

    *ptrToVariable = stack->data[stack->size];
    stack->data[stack->size] = POISON;

    return stack->errorCode;
}


int stackDtor (stack_t* stack) {
    assert(stack);

    free(stack->data);

    return(stack->errorCode);
}
