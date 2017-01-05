//
// Created by Andreas on 03.01.16.
//

#include "LEDHandle.h"
/*
 * LEDHandle.cpp
 */

#include "LEDHandle.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

LEDHandle::LEDHandle() {

}

LEDHandle::~LEDHandle() {
}


void LEDHandle::turnOn(int lednumber) {

}

void LEDHandle::turnOff(int lednumber) {

}

void LEDHandle::startup() {

    const int sleeptime=250000;

    turnAllOff();
    usleep(sleeptime);
    turnAllOn();
    usleep(sleeptime);
    turnAllOff();
    usleep(sleeptime);

    switchLEDOn(LEDBrightness1);
    usleep(sleeptime);
    switchLEDOn(LEDBrightness2);
    usleep(sleeptime);
    //switchLEDOn(LEDBrightness3);
    //usleep(sleeptime);
    //switchLEDOn(LEDBrightness4);
    //usleep(sleeptime);
    switchLEDOff(LEDBrightness1);
    usleep(sleeptime);
    switchLEDOff(LEDBrightness2);
    usleep(sleeptime);
    //switchLEDOff(LEDBrightness3);
    //usleep(sleeptime);
    //switchLEDOff(LEDBrightness4);
    //usleep(sleeptime);
}

void LEDHandle::turnAllOn() {
    switchLEDOn(LEDBrightness1);
    switchLEDOn(LEDBrightness2);
    //switchLEDOn(LEDBrightness3);
    //switchLEDOn(LEDBrightness4);
}


void LEDHandle::turnAllOff() {

    switchLEDOff(LEDBrightness1);
    switchLEDOff(LEDBrightness2);
    //switchLEDOff(LEDBrightness3);
    //switchLEDOff(LEDBrightness4);
}


void LEDHandle::switchLEDOn(const char *lednumber){

    FILE *LEDHandle = NULL;
    if((LEDHandle = fopen(lednumber, "r+")) != NULL){
        fwrite("1", sizeof(char), 1, LEDHandle);
        fclose(LEDHandle);
    }

}

void LEDHandle::switchLEDOff(const char *lednumber){
    FILE *LEDHandle = NULL;
    if((LEDHandle = fopen(lednumber, "r+")) != NULL){
        fwrite("0", sizeof(char), 1, LEDHandle);
        fclose(LEDHandle);
    }
}

void LEDHandle::release()
{
    if( theInstance != NULL )
        delete theInstance;
    theInstance = NULL;
}

LEDHandle* LEDHandle::theInstance = NULL;

LEDHandle* LEDHandle::getInstance()
{
    if( theInstance == NULL )
        theInstance = new LEDHandle();
    return theInstance;
}

void LEDHandle::sayHello()
{
    cout << "## LEDHandle started" << endl;
}
