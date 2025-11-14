#ifndef STRUCTS_AND_CONSTS_H
#define STRUCTS_AND_CONSTS_H

struct node_t {
    char* objectDescription;
    int ownsMemory;

    node_t* left;
    node_t* right;
};

struct tree_t {
    node_t* rootNode;
    size_t size;

    int errorCode;
};

struct dump {
    const char* nameOfFile;
    const char* nameOfFunc;
    unsigned int numOfLine;

    const char* nameOfDumpFile;
    const char* nameOfGraphFile;
    int dumpFileWasOpened;
};

enum treeErr_t {
    noErrors = 0x00,
    badLeft = 0x02,
    badRight = 0x04,
    tooManyRecursiveCalls = 0x08,
};

enum answers_t {
    no = 7,
    yes = 8,
};

enum akinModes {
    guessObject = 1,
    defineObject = 2,
    objectsComparing = 3,
};

const size_t NODE_DESCRIPTION_SIZE = 64;

const size_t STR_SIZE = 64;

const size_t MAX_NODE_RANK = 500;

const int findObject = 333;

const size_t COMMAND_LENGTH = 512;

#endif
