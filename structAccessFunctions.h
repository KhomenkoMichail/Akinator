#ifndef STRUCT_ACCESS_FUNCTIONS_H
#define STRUCT_ACCESS_FUNCTIONS_H

char** nodeObjectDescription (node_t* node);

node_t** nodeLeft (node_t* node);

node_t** nodeRight (node_t* node);

node_t** treeRoot (tree_t* tree);

size_t* treeSize (tree_t* tree);

#endif
