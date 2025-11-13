#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <sys/stat.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "stackStructsAndEnums.h"
#include "stackFunctions.h"
#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"


int guessTheObject (tree_t* tree, dump* dumpInfo) {
    assert(tree);

    for(node_t* currentNode = *treeRoot(tree); ; ) {
        assert(currentNode);

        printf("It %s? (You can answer only \"yes\" or \"no\")\n", *nodeObjectDescription(currentNode));


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

    //*nodeParent(*nodeRight(parentNode)) = parentNode;
    //*nodeParent(*nodeLeft(parentNode)) = parentNode;

    *treeSize(tree) += 2;

    treeDump(tree, dumpInfo, "after adding new node");

    return 0;
}

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]) {
    printf("It...");
    while(1) {
        scanf("%63[^\n]", nodesDifference);
        bufferCleaner();

        if ((strncmp(nodesDifference, "is not", 6) == 0) ||
            (strncmp(nodesDifference, "does not", 8) == 0) ||
            (strncmp(nodesDifference, "not", 3) == 0) ||
            (strncmp(nodesDifference, "do not", 6) == 0)) {

            printf("Don't use the negative form. I don't like negativity.\n");
            printf("Try again. It is...");

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

int findTheObjectPath (node_t* node, const char* objectName, stack_t* stack) {
    assert(node);
    assert(objectName);
    assert(stack);

    if((*nodeLeft(node) == NULL) && (*nodeRight(node) == NULL)) {
        if(strcmp(*nodeObjectDescription(node), objectName) == 0) {
            stackPush(stack, findObject);
            return 1;
        }
        else
            return 0;
    }

    if(findTheObjectPath(*nodeLeft(node), objectName, stack)) {
        stackPush(stack, yes);
        return 1;
    }

    if(findTheObjectPath(*nodeRight(node), objectName, stack)) {
        stackPush(stack, no);
        return 1;
    }

    return 0;
}

void printfObjectDefinition (tree_t* tree, const char* objectName, stack_t* stack) {
    assert(tree);
    assert(objectName);
    assert(stack);

    int nodeInfo = 0;

    printf("%s ", objectName);

    for (node_t* currentNode = *treeRoot(tree); ; ) {
        stackPop(stack, &nodeInfo);
        if (nodeInfo == yes) {
            printf ("is %s ", *nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
            continue;
        }

        if (nodeInfo == no) {
            printf ("is not %s ", *nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
            continue;
        }

        if (nodeInfo == findObject) {
            break;
        }
    }
    printf("\n");
}

int defineTheObject(tree_t* tree, dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);

    char objectName[NODE_DESCRIPTION_SIZE] = {};

    printf("Definition of what object you want to get?\n");
    scanf("%63[^\n]", objectName);
    bufferCleaner();

    struct stack retStack = {};
    struct info stackInfo = {};
    STACK_CTOR(retStack, stackInfo, 25);

    if (findTheObjectPath(*treeRoot(tree), objectName, &retStack))
        printfObjectDefinition(tree, objectName, &retStack);
    else
        printf("There is no such object in my database.\n");

    stackDtor (&retStack);

    return 0;
}

int compareObjects(tree_t* tree, dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);

    char firstObjectName[NODE_DESCRIPTION_SIZE] = {};
    char secondObjectName[NODE_DESCRIPTION_SIZE] = {};

    struct stack firstObjectRetStack = {};
    struct info firstStackInfo = {};
    STACK_CTOR(firstObjectRetStack, firstStackInfo, 25);

    struct stack secondObjectRetStack = {};
    struct info secondStackInfo = {};
    STACK_CTOR(secondObjectRetStack, secondStackInfo, 25);

    printf("Enter the first object to be compared...");
    scanf("%63[^\n]", firstObjectName);
    bufferCleaner();

    printf("Enter the second object to be compared...");
    scanf("%63[^\n]", secondObjectName);
    bufferCleaner();

    if (!findTheObjectPath(*treeRoot(tree), firstObjectName, &firstObjectRetStack)) {
        printf("There is no object \"%s\" in my database.\n", firstObjectName);
        return 0;
    }

    if (!findTheObjectPath(*treeRoot(tree), secondObjectName, &secondObjectRetStack)) {
        printf("There is no object \"%s\" in my database.\n", secondObjectName);
        return 0;
    }

    printfComparing (tree, firstObjectName, secondObjectName, &firstObjectRetStack, &secondObjectRetStack);

    stackDtor (&firstObjectRetStack);
    stackDtor (&secondObjectRetStack);
    return 0;
}

void printfComparing(tree_t* tree, const char* firstObjectName, const char* secondObjectName,
                      stack_t* firstObjectRetStack, stack_t* secondObjectRetStack) {
    assert(tree);
    assert(firstObjectName);
    assert(secondObjectName);
    assert(firstObjectRetStack);
    assert(secondObjectRetStack);

    int firstObjectStep = 0;
    int secondObjectStep = 0;

    stackPop(firstObjectRetStack, &firstObjectStep);
    stackPop(secondObjectRetStack, &secondObjectStep);

    if (firstObjectStep == secondObjectStep)
        printf("\"%s\" and \"%s\" are similar in that they both are",
               firstObjectName, secondObjectName);

    node_t* currentNode = *treeRoot(tree);
    for( ; firstObjectStep == secondObjectStep; ) {

        if (firstObjectStep == yes) {
            printf (" %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }
        if (firstObjectStep == no) {
            printf (" not %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
        }
        if (firstObjectStep == findObject)
            break;

        stackPop(firstObjectRetStack, &firstObjectStep);
        stackPop(secondObjectRetStack, &secondObjectStep);
    }
    node_t* branchingNode = currentNode;
    printf("\n");

    printf("\"%s\" and \"%s\" differ in that", firstObjectName, secondObjectName);

    if (firstObjectStep != findObject)
        printf("\n\"%s\"", firstObjectName);
    for( ; firstObjectStep != findObject; ) {
        if (firstObjectStep == yes) {
            printf (" %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }
        if (firstObjectStep == no) {
            printf (" not %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
        }
        if (firstObjectStep == findObject)
            break;

        stackPop(firstObjectRetStack, &firstObjectStep);
    }

    if (secondObjectStep != findObject)
        printf("\n\"%s\"", secondObjectName);

    currentNode = branchingNode;
    for( ; secondObjectStep != findObject; ) {
        if (secondObjectStep == yes) {
            printf (" %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }

        if (secondObjectStep == no) {
            printf (" not %s", *nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
        }

        if (secondObjectStep == findObject) {
            break;
        }

        stackPop(secondObjectRetStack, &secondObjectStep);
    }
    printf("\n");
}


void printfMenu(void) {
    printfWithDelay("As usual, you have tree wishes:\n");
    printfWithDelay("-- 1) Read your mind.\n");
    printfWithDelay("-- 2) Give a definition of the object.\n");
    printfWithDelay("-- 3) Compare two objects.\n");
    printfWithDelay("Choose one...(answer only 1, 2 or 3)\n");
}

void printfWithDelay(const char* str) {
    assert(str);
    char command[COMMAND_LENGTH] = {};

    snprintf(command, COMMAND_LENGTH, "start /min PowerShell -Command \"Add-Type –AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('%s');\"", str);
    for(size_t i = 0; command[i]; i++)
        if (command[i] == '\n') command[i] = ' ';
    system(command);


    for(size_t numOfChar = 0; str[numOfChar] != '\0'; numOfChar++) {
        printf("%c", str[numOfChar]);
        Sleep(35);
    }
}

void runAkinator(tree_t* tree, dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);

    printfMenu();
    int modeChoice = getModeChoice();

    printf("modeChoice == %d\n", modeChoice);

    switch (modeChoice) {
        case guessObject:
            guessTheObject (tree, dumpInfo);
            break;

        case defineObject:
            defineTheObject (tree, dumpInfo);
            break;

        case objectsComparing:
            compareObjects (tree, dumpInfo);
            break;

        default:
            printf("Error! Bad modeChoice == %d\n", modeChoice);
            break;
    }
}

int getModeChoice(void) {
    int ch = 0;
    int modeChoice = 0;
    char ch1 = '\0';
    while ((scanf ("%d%c", &modeChoice, &ch1) != 2) || (ch1 != '\n')
                                || (modeChoice > 3) || (modeChoice < 1)) {

        putchar(ch1);
        while ((ch = getchar()) != '\n')
            putchar (ch);

        printfWithDelay("You can choose only 1, 2 or 3.\n Try again.\n");
    }
    return modeChoice;
}

void fprintfNode(node_t* node, FILE* file) {
    assert(node);
    assert(file);

    fprintf(file, "(");
    fprintf(file, "\"%s\"", *nodeObjectDescription(node));

    if(*nodeLeft(node) != NULL)
        fprintfNode(*nodeLeft(node), file);
    else
        fprintf(file, "nil ");

    if(*nodeRight(node) != NULL)
        fprintfNode(*nodeRight(node), file);
    else
        fprintf(file, "nil ");

    fprintf(file, ")");
}

node_t* nodeCtorByReadBuffer(char** bufPos, dump* dumpInfo) {
    assert(bufPos);
    assert(dumpInfo);

    skipSpaces(bufPos);//
    if(**bufPos == '(') {
        (*bufPos)++;
        skipSpaces(bufPos);//

        //node_t* newNode = (node_t*)calloc(1, sizeof(node_t));
        //*nodeObjectDescription(newNode) = (*bufPos) + 1;


        int lenOfName = 0;

        char nodeName[NODE_DESCRIPTION_SIZE] = {};
        sscanf(*bufPos, "\"%[^\"]\"%n", name, &lenOfName);

        node_t* newNode = treeNodeCtor(name);

        //char* nextQuotes = strchr((*bufPos) + 1, '"');
        //*nextQuotes = '\0';

        (*bufPos) += lenOfName;

        *nodeLeft(newNode) = nodeCtorByReadBuffer(bufPos, dumpInfo);
        *nodeRight(newNode) = nodeCtorByReadBuffer(bufPos, dumpInfo);

        skipSpaces(bufPos);//
        (*bufPos)++;

        return newNode;
    }

    if (strncmp(*bufPos, "nil", 3) == 0) {
        (*bufPos) += 4;
        printf("bufpos == %s, from string %d\n", *bufPos, __LINE__);
        return NULL;
    }

    return NULL;
}

void skipSpaces(char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    while(**bufPos == ' ')
        (*bufPos)++;
}

int readFileAndCreateTree (tree_t* tree, dump* dumpInfo, const char* nameOfFile) {
    assert(tree);
    assert(dumpInfo);
    assert(nameOfFile);

    char* bufferStart = copyFileContent(nameOfFile);
    if (bufferStart == NULL) {
        printf("Error of copying tree from file\n");
        return 1;
    }
    else
        *treeRoot(tree) = nodeCtorByReadBuffer(&bufferStart, dumpInfo);

    treeDump(tree, dumpInfo, "after Creating a tree");

    return 0;
}

char* copyFileContent (const char* nameOfFile) {
    assert(nameOfFile);

    int fileDescriptor = open(nameOfFile, O_RDONLY, 0);
    if (fileDescriptor == -1) {

        fprintf(stderr, "Error of opening file \"%s\"", nameOfFile);
        perror("");
        return NULL;
    }

    unsigned int sizeOfFile = getSizeOfFile(fileDescriptor);
    if (sizeOfFile == 0) {
        close(fileDescriptor);
        return NULL;
    }

    char* fileCopyBuffer = (char*)calloc(sizeOfFile + 1, sizeof(char));

    size_t numOfReadSymbols = read(fileDescriptor, fileCopyBuffer, sizeOfFile);
    fileCopyBuffer[numOfReadSymbols] = '\0';

    if(close(fileDescriptor) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfFile);
        perror("");
        return NULL;
    }

    return fileCopyBuffer;
}

unsigned int getSizeOfFile (int fileDescriptor) {
    struct stat fileInfo = {};

    if (fstat(fileDescriptor, &fileInfo) == 0)
        return fileInfo.st_size;

    perror("Error of getting the size of the file");
    return 0;
}

void requestToContinue (int* continueFlag) {
    assert (continueFlag);
    int ch = 0;

    printf ("Enter any character to continue or click [Enter] to end program.\n");

    if ((ch = getchar()) == '\n')
        *continueFlag = 0;
    else bufferCleaner();
}

int saveTreeInFile (tree_t* tree, const char* nameOfSaveFile) {
    assert(tree);
    assert(nameOfSaveFile);

    FILE* saveFile = fopen(nameOfSaveFile, "w");

    if (saveFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfSaveFile);
        perror("");
        return 1;
    }

    fprintfNode(*treeRoot(tree), saveFile);

    if (fclose(saveFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfSaveFile);
        perror("");
        return 1;
    }

    return 0;
}
