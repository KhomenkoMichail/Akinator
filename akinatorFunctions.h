#ifndef AKINATOR_FUNCTIONS_H
#define AKINATOR_FUNCTIONS_H

int guessTheObject (tree_t* tree, dump* dumpInfo);

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]);

int addNewObject (tree_t* tree, node_t* parentNode, dump* dumpInfo);

answers_t getAnswer (void);

void bufferCleaner (void);

void printfMenu(void);

void printfWithDelay (const char* str);

void printfComparing (tree_t* tree, const char* firstObjectName, const char* secondObjectName,
                      stack_t* firstObjectRetStack, stack_t* secondObjectRetStack);

int compareObjects (tree_t* tree, dump* dumpInfo);

void runAkinator (tree_t* tree, dump* dumpInfo);

int getModeChoice (void);

void fprintfNode (node_t* node, FILE* file);

node_t* nodeCtorByReadBuffer (char** bufPos, dump* dumpInfo);

void skipSpaces (char** bufPos);

char* copyFileContent (const char* nameOfFile);

unsigned int getSizeOfFile (int fileDescriptor);

int readFileAndCreateTree (tree_t* tree, dump* dumpInfo, const char* nameOfFile);

int defineTheObject (tree_t* tree, dump* dumpInfo);

void printfObjectDefinition (tree_t* tree, const char* objectName, stack_t* stack);

int findTheObjectPath (node_t* node, const char* objectName, stack_t* stack);

void requestToContinue (int* continueFlag);

int saveTreeInFile (tree_t* tree, const char* nameOfSaveFile);

#endif
