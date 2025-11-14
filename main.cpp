#include <TXLib.h>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "stackStructsAndEnums.h"
#include "stackFunctions.h"
#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "treeFunctions.h"
#include "akinatorFunctions.h"
#include "animation.h"

int main (void) {

    //startAnimation();

    tree_t tree = {};
    struct dump dumpInfo = {};

    dumpInfo.nameOfGraphFile = "treeGraph.txt";
    dumpInfo.nameOfDumpFile = "dump.html";

    if (readFileAndCreateTree(&tree, &dumpInfo, "treeSave.txt"))
        return 1;

    int runProgram = 1;

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
