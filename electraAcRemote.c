#include <stdio.h>
#include <string.h>
#include "irslinger.h"
#include "electraAcRemoteEncoder.h"

int main(int argc,char *argv[])
{
    int i = 0,acState,acMode;
    struct airCon newAc;;
    uint32_t outPin = 20;           // The Broadcom pin number the signal will be sent on
    int frequency = 38000;          // The frequency of the IR signal in Hz
    double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                            // the LED will turn on for half the cycle time, and off the other half
    int *codes = NULL;

    if (argc!=5){
        printf("Invalid arguments\n");
        printf("usage: Fan Strength [1-4] (4=auto), Mode [COOL/HEAT], Temp [16-30], State [ON,OFF]\n(ON - if the AC is already running)\n");
        printf("\nFor example: ./electraAcRemote 1 COOL 25 ON\n");
        return 1;
    }
    if(!strcmp(argv[2],"COOL")){
        acMode = COOL;
    }
    else{
        printf("Invalid mode argument!\n");
        printf("usage: Fan Strength [1-4] (4=Auto), Mode [COOL/HEAT], Temp [16-30], State [ON,OFF]\n(ON - if the AC is already running)\n");
        return 1;
    }

    if(!strcmp(argv[4],"ON")){
        acState = ON;
    }
    if(!strcmp(argv[4],"OFF")){
        acState = OFF;
    }

    codes = getCodes(&newAc,atoi(argv[1]),acMode,atoi(argv[3]),acState);

    int result = irSlingRaw(outPin,frequency,dutyCycle,codes,TIMINGS_LENGTH*sizeof(int)/sizeof(int));

    return result;
}
