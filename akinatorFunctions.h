#ifndef AKINATOR_FUNCTIONS_H
#define AKINATOR_FUNCTIONS_H

int guessTheObject (tree_t* tree, dump* dumpInfo);

void getDifference (char nodesDifference[NODE_DESCRIPTION_SIZE]);

int addNewObject (tree_t* tree, node_t* parentNode, dump* dumpInfo);

answers_t getAnswer (void);

void bufferCleaner (void);

void speak (const char* phrase);

void speakEnglishFast(const char* text);

int defineTheObject (tree_t* tree, dump* dumpInfo);

int findTheObjectPath (node_t* node, size_t rank, const char* objectName, int objectPath[]);

void printfObjectDefinition (tree_t* tree, const char* objectName, int objectPath[]);

#endif
