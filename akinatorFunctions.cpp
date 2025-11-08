#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <TXLib.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"


int objectSearch (tree_t* tree, dump* dumpInfo) {
    assert(tree);

    for(node_t* currentNode = *treeRoot(tree); ; ) {
        assert(currentNode);

        //speak("You can answer only");
        printf("It %s? (You can answer only \"yes\" or \"no\")\n", *nodeObjectDescription(currentNode));

        //speakEnglishFast("You can answer only");



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
                addNewObject (tree, currentNode, dumpInfo);
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
        scanf("%s", answerString);
        bufferCleaner();

        if (strncmp(answerString, "yes\0", 4) == 0)
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

    char nodeDescription[NODE_DESCRIPTION_SIZE] = {};
    char nodesDifference[NODE_DESCRIPTION_SIZE] = {};

    printf("Hmmmm... Surprise me, who was it?\n");
    scanf("%63[^\n]", nodeDescription);
    bufferCleaner();
    nodeDescription[NODE_DESCRIPTION_SIZE - 1] = '\0';

    printf("\"%s\"how interesting!\n", nodeDescription);
    printf("And how is this \"%s\" different from my \"%s\"?\n",
        nodeDescription, *nodeObjectDescription(parentNode));

    getDifference(nodesDifference);

    printf("So, \"%s\" differs from \"%s\" in that it \"%s\".\n",
           nodeDescription, *nodeObjectDescription(parentNode), nodesDifference);

    *nodeRight(parentNode) = treeNodeCtor(*nodeObjectDescription(parentNode));
    *nodeLeft(parentNode) = treeNodeCtor (nodeDescription);
    strncpy (*(nodeObjectDescription(parentNode)), nodesDifference, NODE_DESCRIPTION_SIZE);

    *treeSize(tree) += 2;

    treeDump(tree, dumpInfo, "after adding");

    return 0;
}

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]) {
    printf("It...");
    while(1) {
        scanf("%63[^\n]", nodesDifference);
        bufferCleaner();

        if ((strncmp(nodesDifference, "is not", 6) == 0) ||
            (strncmp(nodesDifference, "does not", 8) == 0) ||
            (strncmp(nodesDifference, "donot", 5) == 0) ||
            (strncmp(nodesDifference, "do not", 6) == 0)) {

            printf("Don't use the negative form. I don't like negativity.\n");
            printf("Try again. It...");

            for (size_t numOfChar = 0; numOfChar < NODE_DESCRIPTION_SIZE; numOfChar++)
                nodesDifference[numOfChar] = '\0';
        }
        else
            break;
    }
}

void bufferCleaner (void) {
    int ch = 0;
    while ((ch = getchar()) != '\n')
        continue;
}

void speak (const char* phrase) {
    assert(phrase);

char command[256];
    sprintf(command, "powershell -Command \"Add-Type -AssemblyName System.Speech; "
        "(New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('%s')\"", phrase);
        system(command);
}

void speakEnglishFast(const char* text) {
    char command[1000];
    sprintf(command,
        "powershell -Command \""
        "(New-Object System.Speech.Synthesis.SpeechSynthesizer -Property @{"
        "Rate=6;"
        "}).Speak('%s');\"",
        text);

    system(command);
}
