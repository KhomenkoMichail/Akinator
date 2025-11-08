#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "akinatorFunctions.h"


int objectSearch (tree_t* tree) {
    assert(tree);

    for(node_t* currentNode = *treeRoot(tree); ; ) {
        assert(currentNode);

        printf("It (is) %s (You can answer only \"yes\" or \"no\")\n", *nodeObjectDescription(currentNode));
        if (getAnswer() == yes) {
            if((*nodeLeft(currentNode) == NULL) && (*nodeRight(currentNode) == NULL)) {
                printf("I'm not impressed.\n");
                return 1;
            }
            else
                currentNode = *nodeLeft(currentNode);
        }
        else {
            if((*nodeLeft(currentNode) == NULL) && (*nodeRight(currentNode) == NULL)) {
                printf("It can't be! Pathetic creature, you've guessed at an object that isn't in my database.\n");
                return 0;
            }
            else
                currentNode = *nodeRight(currentNode);
        }
    }

    return 0;
}


answers_t getAnswer (void) {
    char answerString[STR_SIZE] = {};

    while (1) {
        if (scanf ("%s", answerString) != 1)
            continue;
        else if (strncmp(answerString, "yes\0", 4) == 0)
            return yes;
        else if (strncmp(answerString, "no\0", 3) == 0)
            return no;
        else
            printf ("How dare you joke about me! You have no right to answer \"%s\".\n"
                    "\"yes\" or \"no\", is it so difficult?\n", answerString);
    }
    return no;
}

int addNewObject (tree_t* tree, node_t* parentNode, dump* dumpInfo) {
    assert(tree);
    assert(parentNode);
    assert(dumpInfo);
    return 0;
}
