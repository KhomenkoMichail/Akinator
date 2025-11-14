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

        HDC nodePhoto =  NULL;

        char speech[COMMAND_LENGTH] = {};
        snprintf(speech, COMMAND_LENGTH, "It is %s? (You can answer only \"yes\" or \"no\")\n", *nodeObjectDescription(currentNode));
        printfWithDelay(speech);

        if((*nodeLeft(currentNode) == NULL) && (*nodeRight(currentNode) == NULL)) {
            char fileName[100] = {};
                sprintf(fileName, "SCREENSHOTS/%s.bmp", *nodeObjectDescription(currentNode));
                nodePhoto = txLoadImage(fileName);
                if(nodePhoto != NULL)
                    txBitBlt(txDC(), 400, 0, 0, 0, nodePhoto, 0, 0);
        }

        if (getAnswer() == yes) {
            if((*nodeLeft(currentNode) == NULL) && (*nodeRight(currentNode) == NULL)) {
                printfWithDelay("I'm not impressed.\n");
                return 1;
            }
            else
                currentNode = *nodeLeft(currentNode);
        }
        else {
            if((*nodeLeft(currentNode) == NULL) && (*nodeRight(currentNode) == NULL)) {
                printfWithDelay("It can not be! Pathetic creature,");
                printfWithDelay("you have guessed at an object that is not in my database.\n");
                addNewObject (tree, currentNode, dumpInfo);
                return 0;
            }
            else
                currentNode = *nodeRight(currentNode);
        }
        if(!nodePhoto)
            txDeleteDC(nodePhoto);

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
            printfWithDelay ("How dare you joke about me!.\n"
            "Use only \"yes\" or \"no\", is it so difficult?\n");
    }
    return no;
}

int addNewObject (tree_t* tree, node_t* parentNode, dump* dumpInfo) {
    assert(tree);
    assert(parentNode);
    assert(dumpInfo);

    char nodeDescription[NODE_DESCRIPTION_SIZE] = {};
    char nodesDifference[NODE_DESCRIPTION_SIZE] = {};

    printfWithDelay("Hmmmm... Surprise me, who was it?\n");
    scanf("%63[^\n]", nodeDescription);
    bufferCleaner();
    nodeDescription[NODE_DESCRIPTION_SIZE - 1] = '\0';

    printfWithDelay(nodeDescription);
    printfWithDelay(" how interesting!\n");

    char speech[COMMAND_LENGTH] = {};
    snprintf(speech, COMMAND_LENGTH, "And how is this \"%s\" different from my \"%s\"?\n",
        nodeDescription, *nodeObjectDescription(parentNode));
    printfWithDelay(speech);

    getDifference(nodesDifference);

    printfWithDelay("I will remember...\n");

    *nodeRight(parentNode) = treeNodeCtor(*nodeObjectDescription(parentNode));
    *nodeLeft(parentNode) = treeNodeCtor (nodeDescription);
    //strncpy (*(nodeObjectDescription(parentNode)), nodesDifference, NODE_DESCRIPTION_SIZE);
    if(parentNode->ownsMemory)
        free(*(nodeObjectDescription(parentNode)));
    *(nodeObjectDescription(parentNode)) = strdup(nodesDifference);
    parentNode->ownsMemory = 1;

    *treeSize(tree) += 2;

    treeDump(tree, dumpInfo, "after adding new node");

    return 0;
}

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]) {
    printfWithDelay("It is...");
    while(1) {
        scanf("%63[^\n]", nodesDifference);
        bufferCleaner();

        if ((strncmp(nodesDifference, "is not", 6) == 0) ||
            (strncmp(nodesDifference, "does not", 8) == 0) ||
            (strncmp(nodesDifference, "not", 3) == 0) ||
            (strncmp(nodesDifference, "do not", 6) == 0)) {

            printfWithDelay("Don't use the negative form. I don't like negativity.\n");
            printfWithDelay("Try again. It is...");

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

    printfWithDelay(objectName);

    for (node_t* currentNode = *treeRoot(tree); ; ) {
        stackPop(stack, &nodeInfo);
        if (nodeInfo == yes) {
            printfWithDelay(" is ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
            continue;
        }

        if (nodeInfo == no) {
            printfWithDelay(" is not ");
            printfWithDelay(*nodeObjectDescription(currentNode));
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

    printfWithDelay("Definition of what object you want to get?\n");
    scanf("%63[^\n]", objectName);
    bufferCleaner();

    struct stack retStack = {};
    struct info stackInfo = {};
    STACK_CTOR(retStack, stackInfo, 25);

    if (findTheObjectPath(*treeRoot(tree), objectName, &retStack))
        printfObjectDefinition(tree, objectName, &retStack);
    else
        printfWithDelay("There is no such object in my database.\n");

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

    printfWithDelay("Enter the first object to be compared...");
    scanf("%63[^\n]", firstObjectName);
    bufferCleaner();

    printfWithDelay("Enter the second object to be compared...");
    scanf("%63[^\n]", secondObjectName);
    bufferCleaner();

    if (!findTheObjectPath(*treeRoot(tree), firstObjectName, &firstObjectRetStack)) {
        char speech[COMMAND_LENGTH] = {};
        snprintf(speech, COMMAND_LENGTH, "There is no object \"%s\" in my database.\n", firstObjectName);
        printfWithDelay(speech);
        return 0;
    }

    if (!findTheObjectPath(*treeRoot(tree), secondObjectName, &secondObjectRetStack)) {
        char speech[COMMAND_LENGTH] = {};
        snprintf(speech, COMMAND_LENGTH, "There is no object \"%s\" in my database.\n", secondObjectName);
        printfWithDelay(speech);
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

    if (firstObjectStep == secondObjectStep) {
        char speech[COMMAND_LENGTH] = {};
        snprintf(speech, COMMAND_LENGTH, "\"%s\" and \"%s\" are similar in that they both",firstObjectName, secondObjectName);
        printfWithDelay(speech);
    }

    node_t* currentNode = *treeRoot(tree);
    for( ; firstObjectStep == secondObjectStep; ) {

        if (firstObjectStep == yes) {
            printfWithDelay(" are ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }
        if (firstObjectStep == no) {
            printfWithDelay(" are not ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
        }
        if (firstObjectStep == findObject)
            break;

        stackPop(firstObjectRetStack, &firstObjectStep);
        stackPop(secondObjectRetStack, &secondObjectStep);
    }
    node_t* branchingNode = currentNode;
    printf("\n");

    char speech[COMMAND_LENGTH] = {};
        snprintf(speech, COMMAND_LENGTH, "\"%s\" and \"%s\" differ in that", firstObjectName, secondObjectName);
        printfWithDelay(speech);

    if (firstObjectStep != findObject) {
        printf("\n");
        printfWithDelay(firstObjectName);
    }

    for( ; firstObjectStep != findObject; ) {
        if (firstObjectStep == yes) {
            printfWithDelay(" is ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }
        if (firstObjectStep == no) {
            printfWithDelay(" is not ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeRight(currentNode);
        }
        if (firstObjectStep == findObject)
            break;

        stackPop(firstObjectRetStack, &firstObjectStep);
    }

    if (secondObjectStep != findObject) {
        printf("\n");
        printfWithDelay(secondObjectName);
    }

    currentNode = branchingNode;
    for( ; secondObjectStep != findObject; ) {
        if (secondObjectStep == yes) {
            printfWithDelay(" is ");
            printfWithDelay(*nodeObjectDescription(currentNode));
            currentNode = *nodeLeft(currentNode);
        }

        if (secondObjectStep == no) {
            printfWithDelay(" is not ");
            printfWithDelay(*nodeObjectDescription(currentNode));
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
    printfWithDelay("As usual, you have 3 wishes:\n");
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

    HDC screenShotsArr[NUM_OF_SCREENSHOTS];
    char fileName[100];

    int screenCounter = 0;

    for (int numOfScreenShot = 0; numOfScreenShot < NUM_OF_SCREENSHOTS; numOfScreenShot++) {
        sprintf(fileName, "SCREENSHOTS/screen%d.bmp", numOfScreenShot);

        screenShotsArr[numOfScreenShot] = txLoadImage(fileName);
        if (screenShotsArr[numOfScreenShot] == NULL)
            break;
        screenCounter++;
    }

    static int curScreen = 0;

    for(size_t numOfChar = 0; str[numOfChar] != '\0'; numOfChar++) {
        printf("%c", str[numOfChar]);
        Sleep(100);

        if (screenShotsArr[curScreen] != NULL) {
            txBitBlt(txDC(), 0, 0, 1280, 666, screenShotsArr[curScreen], 0, 0);
        }

        if(numOfChar % 5 == 0)
            curScreen = (curScreen + 1) % screenCounter;
    }




    for (int numOfScreenShot = 0; numOfScreenShot < screenCounter; numOfScreenShot++) {
        if (screenShotsArr[numOfScreenShot] != NULL) {
            txDeleteDC(screenShotsArr[numOfScreenShot]);
        }
    }

}

int runAkinator(tree_t* tree, dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);

    if (treeVerifier(tree)) {
        treeDump (tree, dumpInfo, "before running akinator");
        return 1;
    }


    printfMenu();
    int modeChoice = getModeChoice();


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

    if (treeVerifier(tree)) {
        treeDump (tree, dumpInfo, "after running akinator");
        return 1;
    }

    return 0;
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

    fprintf(file, "( ");
    fprintf(file, "\"%s\" ", *nodeObjectDescription(node));

    if(*nodeLeft(node) != NULL)
        fprintfNode(*nodeLeft(node), file);
    else
        fprintf(file, "nil ");

    if(*nodeRight(node) != NULL)
        fprintfNode(*nodeRight(node), file);
    else
        fprintf(file, "nil ");

    fprintf(file, ") ");
}

node_t* nodeCtorByReadBuffer(char** bufPos, tree_t* tree, dump* dumpInfo, FILE* dumpFile) {
    assert(bufPos);
    assert(dumpInfo);
    assert(tree);
    assert(dumpFile);

    DUMP_MESSAGE(dumpFile, "Зашла в функцию создания узла\n", *bufPos);
    skipSpaces(bufPos);
    if(**bufPos == '(') {
        *treeSize(tree) += 1;
        (*bufPos)++;
        skipSpaces(bufPos);

        DUMP_MESSAGE(dumpFile, "Прочитала (\n", *bufPos);

        node_t* newNode = (node_t*)calloc(1, sizeof(node_t));
        *nodeObjectDescription(newNode) = (*bufPos) + 1;


        int lenOfName = 0;

        sscanf(*bufPos, "\"%*[^\"]\"%n", &lenOfName);

        char* nextQuotes = strchr((*bufPos) + 1, '"');
        *nextQuotes = '\0';

        (*bufPos) += lenOfName;

        DUMP_MESSAGE(dumpFile, "Прочитала имя узла.\n", *bufPos);

        DUMP_MESSAGE(dumpFile, "<h3>Сейчас зайду в левое поддерево.\n</font></h3>", *bufPos);
        *nodeLeft(newNode) = nodeCtorByReadBuffer(bufPos, tree, dumpInfo, dumpFile);

        if(*nodeLeft(newNode) != NULL) {
            fclose(dumpFile);
            SUBTREE_DUMP((*nodeLeft(newNode)), dumpInfo, "Вот созданное ЛЕВОЕ поддерево.\n");
            dumpFile = fopen(dumpInfo->nameOfDumpFile, "a");
        }

        DUMP_MESSAGE(dumpFile, "<h3>Сейчас зайду в правое поддерево.\n</font></h3>", *bufPos);
        *nodeRight(newNode) = nodeCtorByReadBuffer(bufPos, tree, dumpInfo, dumpFile);

        if(*nodeRight(newNode) != NULL) {
            fclose(dumpFile);
            SUBTREE_DUMP((*nodeRight(newNode)), dumpInfo, "Вот созданное ПРАВОЕ поддерево.\n");
            dumpFile = fopen(dumpInfo->nameOfDumpFile, "a");
        }

        skipSpaces(bufPos);
        (*bufPos)++;

        DUMP_MESSAGE(dumpFile, "Прочитала ).\n", *bufPos);

        return newNode;
    }

    if (strncmp(*bufPos, "nil", 3) == 0) {
        DUMP_MESSAGE(dumpFile, "Нашла nil.\n", *bufPos);
        (*bufPos) += 4;
        DUMP_MESSAGE(dumpFile, "Прочитала nil.\n", *bufPos);
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

    FILE* dumpFile = 0;
    if(dumpInfo->dumpFileWasOpened)
        dumpFile = fopen(dumpInfo->nameOfDumpFile, "a");
    else {
        dumpFile = fopen(dumpInfo->nameOfDumpFile, "w");
        dumpInfo->dumpFileWasOpened = 1;
    }

    if (dumpFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", dumpInfo->nameOfDumpFile);
        perror("");
        return 1;
    }

    char* bufferStart = copyFileContent(nameOfFile);
    if (bufferStart == NULL) {
        printf("Error of copying tree from file\n");
        return 1;
    }
    else
        *treeRoot(tree) = nodeCtorByReadBuffer(&bufferStart, tree, dumpInfo, dumpFile);

    if (fclose(dumpFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", dumpInfo->nameOfDumpFile);
        perror("");
        return 1;
    }

    treeDump(tree, dumpInfo, "Вот созданное финальное дерево:");

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

    printfWithDelay("Enter any character to continue or click [Enter] to end program.\n");

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

void createWindow() {
    txCreateWindow(1280, 666);
}
