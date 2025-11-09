#ifndef AKINATOR_FUNCTIONS_H
#define AKINATOR_FUNCTIONS_H

int guessTheObject (tree_t* tree, dump* dumpInfo);

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]);

int addNewObject (tree_t* tree, node_t* parentNode, dump* dumpInfo);

answers_t getAnswer (void);

void bufferCleaner (void);

void speak (const char* phrase);

int defineTheObject (tree_t* tree, dump* dumpInfo);

int findTheObjectPath (node_t* node, size_t rank, const char* objectName, int objectPath[]);

void printfObjectDefinition (tree_t* tree, const char* objectName, int objectPath[]);

int compareObjects (tree_t* tree, dump* dumpInfo);


void printfComparing (tree_t* tree, const char* firstObjectName, const char* secondObjectName,
                      int firstObjectPath[], int secondObjectPath[]);


#endif
