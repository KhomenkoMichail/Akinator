#include <stdio.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"

int main (void) {

    //SetConsoleOutputCP(CP_UTF8);
    //SetConsoleCP(CP_UTF8);

    tree_t tree = {};
    struct dump dumpInfo = {};
    dumpInfo.nameOfGraphFile = "treeGraph.txt";
    dumpInfo.nameOfDumpFile = "dump.html";

    akinatorTreeCtor(&tree, &dumpInfo);

    node_t* nodePolt = treeNodeCtor("Poltorashka");
    node_t* nodeVedetMatan = treeNodeCtor("Teaches mathan?");
    node_t* nodePetrovich = treeNodeCtor("Petrovich?");
    node_t* nodeDivari = treeNodeCtor("Divari");

    printf("%s\n", *nodeObjectDescription(nodeVedetMatan));

    node_t* root = *treeRoot(&tree);
    *nodeLeft(root) = nodePolt;
    *nodeRight(root) = nodeVedetMatan;
    *nodeLeft(nodeVedetMatan) = nodePetrovich;
    *nodeRight(nodeVedetMatan) = nodeDivari;
    printf("%p\n", *nodeLeft(root));
    printf("%p\n", *nodeRight(root));

    tree.size = 5;
    objectSearch(&tree);
    treeDump(&tree, &dumpInfo, "review");

    return 0;
}
