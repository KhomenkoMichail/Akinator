#include <TXLib.h>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"

int main (void) {

    tree_t tree = {};
    struct dump dumpInfo = {};

    dumpInfo.nameOfGraphFile = "treeGraph.txt";
    dumpInfo.nameOfDumpFile = "dump.html";

    readFileAndCreateTree (&tree, &dumpInfo, "treeSave.txt");
    treeDump (&tree, &dumpInfo, "after ctoring tree");
/*
    int runProgram = 1;

    printfWithDelay("Привет, я акинатор\n");
    while (runProgram) {

    runAkinator(&tree, &dumpInfo);
    break;
    }
*/


/*
    tree_t tree = {};
    struct dump dumpInfo = {};
    dumpInfo.nameOfGraphFile = "treeGraph.txt";
    dumpInfo.nameOfDumpFile = "dump.html";

    akinatorTreeCtor(&tree, &dumpInfo);

    node_t* nodePolt = treeNodeCtor("Poltorashka");
    node_t* nodeVedetMatan = treeNodeCtor("Teaches mathan");
    node_t* nodePetrovich = treeNodeCtor("Petrovich");
    node_t* nodeDivari = treeNodeCtor("Divari");



    node_t* root = *treeRoot(&tree);
    *nodeLeft(root) = nodePolt;
    *nodeRight(root) = nodeVedetMatan;
    *nodeLeft(nodeVedetMatan) = nodePetrovich;
    *nodeRight(nodeVedetMatan) = nodeDivari;

    FILE* file = fopen("treeSave.txt", "w");
    fprintfNode(*treeRoot(&tree), file);
    fclose(file);
*/

    return 0;
}
