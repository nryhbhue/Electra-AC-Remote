#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "electraAcRemoteEncoder.h"

void initializeStruct (struct airCon* newAcPointer)
{
    int i = 0;
    newAcPointer->fullState = FULL_STATE_MASK;
    newAcPointer->fan = FAN_MASK;
    newAcPointer->mode = MODE_MASK;
    newAcPointer->temp = TEMP_MASK;
    newAcPointer->swing = SWING_MASK;
    for (i = 0;i < TIMINGS_LENGTH;i++){
        (newAcPointer->manchesterTimings)[i] = 0;
    }
}

void updateFan (int value,struct airCon* newAcPointer)
{
    newAcPointer->fan &= (value-1);
    newAcPointer->fan = (newAcPointer->fan) << 28;
    newAcPointer->fullState |= newAcPointer->fan;
}

void updateMode (int value,struct airCon* newAcPointer)
{
    newAcPointer->mode &= (value);
    newAcPointer->mode = (newAcPointer->mode) << 30;
    newAcPointer->fullState |= newAcPointer->mode;
}

void updateTemperature (int value,struct airCon* newAcPointer)
{
    newAcPointer->temp &= (value-15);
    newAcPointer->temp = (newAcPointer->temp) << 19;
    newAcPointer->fullState |= newAcPointer->temp;
}

void updateSwing (int value,struct airCon* newAcPointer)
{
    newAcPointer->swing &= (value);
    newAcPointer->swing = (newAcPointer->swing) << 25;
    newAcPointer->fullState |= newAcPointer->swing;
}

void updateParameter (acParameter parameter,int value,struct airCon* newAcPointer)
{
    switch (parameter) {
    case fan:
        updateFan(value,newAcPointer);
        break;
    case mode:
        updateMode(value,newAcPointer);
        break;
    case temp:
        updateTemperature(value,newAcPointer);
        break;
    case swing:
        updateSwing(value,newAcPointer);
        break;
    }
}

void convertStateToBitStrings (struct airCon* newAcPointer)
{
    int i = 0;
    uint64_t toggleMask = 1,fullStateCopy;

    fullStateCopy = newAcPointer->fullState;

    for (i = DATA_BITS_LENGTH-1;i >= 0;i--){
        sprintf((newAcPointer->bitStrings)[i],"%d",fullStateCopy & toggleMask);
        fullStateCopy = fullStateCopy >> 1;
    }
}

void convertBitStringsToManchesterString (struct airCon* newAcPointer)
{
    int i = 0;
    for (i = 0;i < DATA_BITS_LENGTH;i++){
        if ((newAcPointer->bitStrings)[i][0] == '0'){
            strcpy((newAcPointer->manchesterString) + 2*i,"01");
        }
        else{
            strcpy((newAcPointer->manchesterString) + 2*i,"10");
        }
    }
}

void convertManchesterStringToManchesterTimings (struct airCon* newAcPointer)
{
    int stringPointer = 0;
    int timingPointer = 0;
    int counter = 0;
    int i = 0;

    (newAcPointer->manchesterTimings)[timingPointer] = 3000;
    timingPointer++;

    if ((newAcPointer->manchesterString)[0] == '1'){
        (newAcPointer->manchesterTimings)[timingPointer] = 4000;
        stringPointer++;
    }
    else{
        (newAcPointer->manchesterTimings)[timingPointer] = 3000;
    }
    timingPointer++;

    for (stringPointer;stringPointer < DATA_BITS_LENGTH*2;stringPointer++){

        while ((newAcPointer->manchesterString)[stringPointer] == '0'){
            counter++;
            stringPointer++;
        }
        (newAcPointer->manchesterTimings)[timingPointer] = 1000*counter;
        counter = 0;
        timingPointer++;

        while ((newAcPointer->manchesterString)[stringPointer] == '1'){
            counter++;
            stringPointer++;
        }
        (newAcPointer->manchesterTimings)[timingPointer] = 1000*counter;
        counter = 0;
        timingPointer++;
        stringPointer--;
    }

    (newAcPointer->manchesterTimings)[3*timingPointer] = 4000;

    for (i=0;i<timingPointer;i++){
        (newAcPointer->manchesterTimings)[i+timingPointer] = (newAcPointer->manchesterTimings)[i];
        (newAcPointer->manchesterTimings)[i+timingPointer*2] = (newAcPointer->manchesterTimings)[i];
    }
}

int *getCodes (struct airCon* newAc,int fanV,int modeV,int tempV,int state,int swingV)
{

    initializeStruct(newAc);
    updateParameter(fan,fanV,newAc);
    updateParameter(temp,tempV,newAc);
    updateParameter(mode,modeV,newAc);
    updateParameter(swing,swingV,newAc);

    convertStateToBitStrings(newAc);
    if (state == ON){
        (newAc->bitStrings)[0][0] = '0';
    }
    convertBitStringsToManchesterString(newAc);
    convertManchesterStringToManchesterTimings(newAc);

    return newAc->manchesterTimings;
}
