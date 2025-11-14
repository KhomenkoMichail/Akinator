#include <TXLib.h>
#include <windows.h>

#include "animation.h"

#include <TXLib.h>

int startAnimation(void) {
    txCreateWindow(1280, 666);

    HDC screenShotsArr[25];
    char fileName[100];

    int screenCounter = 0;

    for (int numOfScreenShot = 0; numOfScreenShot < 25; numOfScreenShot++) {
        sprintf(fileName, "SCREENSHOTS/screen%d.bmp", numOfScreenShot);

        screenShotsArr[numOfScreenShot] = txLoadImage(fileName);
        if (screenShotsArr[numOfScreenShot] == NULL)
            break;
        screenCounter++;
    }

    int curScreen = 0;
    while (!txGetAsyncKeyState(VK_ESCAPE)) {
        txBegin();
        txClear();

        if (screenShotsArr[curScreen] != NULL) {
            txBitBlt(txDC(), 0, 0, 1280, 666, screenShotsArr[curScreen], 0, 0);
        }

        txEnd();

        curScreen = (curScreen + 1) % screenCounter;

        txSleep(100);
    }

    for (int i = 0; i < screenCounter; i++) {
        if (screenShotsArr[i] != NULL) {
            txDeleteDC(screenShotsArr[i]);
        }
    }

    return 0;
}
