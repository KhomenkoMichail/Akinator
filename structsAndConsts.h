#ifndef STRUCTS_AND_CONSTS_H
#define STRUCTS_AND_CONSTS_H

struct node_t {
    char* objectDescription;

    node_t* parent;

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
    no = 0,
    yes = 1,
};

const size_t NODE_DESCRIPTION_SIZE = 64;

const size_t STR_SIZE = 64;

#endif
