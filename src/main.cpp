#include <TXLib.h>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "../include/stackStructsAndEnums.h"
#include "../include/stackFunctions.h"
#include "../include/structsAndConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/treeFunctions.h"
#include "../include/akinatorFunctions.h"

int main (void) {

    tree_t tree = {};
    struct dump dumpInfo = {};

    dumpInfo.nameOfGraphFile = "DUMPS/treeGraph.txt";
    dumpInfo.nameOfDumpFile = "DUMPS/dump.html";

    if (readFileAndCreateTree(&tree, &dumpInfo, "treeSave.txt"))
        return 1;

    int runProgram = 1;

    createWindow();
    printfWithDelay("Hi, my name is akinator\n");
    while (runProgram) {

    if (runAkinator(&tree, &dumpInfo))
        break;
    requestToContinue (&runProgram);
    }

    saveTreeInFile (&tree, "treeSave.txt");
    deleteTree(&tree);

    return 0;
}
