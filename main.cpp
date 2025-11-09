#include <stdio.h>
#include <TXLib.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"

int main (void) {

    tree_t tree = {};
    struct dump dumpInfo = {};
    dumpInfo.nameOfGraphFile = "treeGraph.txt";
    dumpInfo.nameOfDumpFile = "dump.html";

    akinatorTreeCtor(&tree, &dumpInfo);

    node_t* nodePolt = treeNodeCtor("Poltorashka");
    node_t* nodeVedetMatan = treeNodeCtor("Teaches mathan?");
    node_t* nodePetrovich = treeNodeCtor("Petrovich?");
    node_t* nodeDivari = treeNodeCtor("Divari");

    node_t* root = *treeRoot(&tree);
    *nodeLeft(root) = nodePolt;
    *nodeRight(root) = nodeVedetMatan;
    *nodeLeft(nodeVedetMatan) = nodePetrovich;
    *nodeRight(nodeVedetMatan) = nodeDivari;

    tree.size = 5;
    //for (int i = 0; i<5; i++)
    //    guessTheObject(&tree, &dumpInfo);
    //treeDump(&tree, &dumpInfo, "review");
    defineTheObject(&tree, &dumpInfo);

    return 0;
}
