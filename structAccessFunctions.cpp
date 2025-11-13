#include <assert.h>
#include <stdio.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"

char** nodeObjectDescription (node_t* node) {
    assert (node);

    return &(node->objectDescription);
}

node_t** nodeLeft (node_t* node) {
    assert(node);

    return &(node->left);
}

node_t** nodeRight (node_t* node) {
    assert(node);

    return &(node->right);
}

//node_t** nodeParent (node_t* node) {
//    assert(node);
//
//    return &(node->parent);
//}

node_t** treeRoot (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->rootNode);
}

size_t* treeSize (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->size);
}
