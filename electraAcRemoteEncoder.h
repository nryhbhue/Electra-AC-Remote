#ifndef ELECTRA_AC_REMOTE_ENCODER_H
#define ELECTRA_AC_REMOTE_ENCODER_H

#define FULL_STATE_MASK 0x200000002
#define ON_STATE_MASK   0x1
#define FAN_MASK        0x3
#define SWING_MASK      0x3
#define MODE_MASK       0x7
#define TEMP_MASK       0xF
#define COOL            0x1
#define HEAT            0x2
#define SWING_OFF       0x0
#define SWING_ON	0x1
#define SWING_SINGLE    0x2
#define OFF             0x0
#define ON              0x1

#define TIMINGS_LENGTH  200
#define DATA_BITS_LENGTH 34

typedef enum acParameters {fan,mode,temp,swing} acParameter;

typedef struct airCon {
    uint64_t fullState;
    uint64_t fan;
    uint64_t mode;
    uint64_t temp;
    uint64_t swing;
    char bitStrings[DATA_BITS_LENGTH][2];
    char manchesterString[DATA_BITS_LENGTH*2];
    int manchesterTimings[TIMINGS_LENGTH]; //64 time frames per packet x 3 + 1 for ending trail
} ac;

int *getCodes (struct airCon* newAc,int fanV,int modeV,int tempV,int state,int swingV);

#endif
