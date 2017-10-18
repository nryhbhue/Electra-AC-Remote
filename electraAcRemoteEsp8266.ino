#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <electraAcRemoteEncoder.h> /* Add the electraAcRemoteEncoder header and source code files to your library in the Arduino IDE.
                                       You will probably need to change the extension of the source file to ".cpp" for it to compile */

#define FREQ_HZ 38000
#define TIMINGS_LENGTH 200

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = ""; //Insert your network's SSID here
char pass[] = ""; //Insert your network's password here
char auth[] = ""; //Insert your Blynk project's authentication token here
char localIp[] = ""; //Insert your local server's ip address if you're using one

////// AC GLOBALS //////
int acState = 0;
int fanStrength = 1;
int temperature = 16;
int acMode = 1;
int acSwing = 0;
char acCommand[256];
////////////////////////

int outputPin = 3; /* 3 is the RX pin. Since the output pin is connected to the base of your transistor it'll be low at boot time which imposes no problem
                      as far as the RX pin is involved, gpio 2 on the other hand must be high/floating at boot time or else the esp module won't start */

void mark (int time)
{
    int i = 0;
    int cycles = 0;
    cycles = time / 28; /*The carrier wave is 38kHz so each period is 26 micro seconds. After checking the output with an IR receiver, 28 seemed to be better
                          as the hardware isn't as precise as you would hope */
    if (cycles == 0){
        cycles = 1;
    }
    for (i = 0;i < cycles;i++){
        digitalWrite(outputPin, HIGH);
        delayMicroseconds(13);
        digitalWrite(outputPin, LOW);
        delayMicroseconds(13);
    }
}

void space (int time)
{
    digitalWrite(outputPin, LOW);
    if (time > 0){
        delayMicroseconds(time);
    }
}

void irRemoteSendRaw (int *codes, int len)
{
    int i;

    for (i = 0;i < len;i++){
        if (i%2 == 0){
            mark(codes[i]);
        }
        else{
            space(codes[i]);
        }
    }
}

void sendCodesTask(int state)
{
    struct airCon newAc;
    int *codes;

    codes = getCodes(&newAc,fanStrength,acMode,temperature,state,acSwing);
    irRemoteSendRaw(codes,TIMINGS_LENGTH);
}

BLYNK_WRITE(V1) //Temperature: step widget
{
    temperature = param.asInt();
    Blynk.virtualWrite(V7,temperature);
    if (acState){
        sendCodesTask(acState);
    }
}

BLYNK_WRITE(V2) //Fan: step widget
{
    fanStrength = param.asInt();
    if (acState){
        sendCodesTask(acState);
    }
}

BLYNK_WRITE(V3) //Swing: slider widget
{
    acSwing = param.asInt();
    if (acState){
        sendCodesTask(acState);
    }
}

BLYNK_WRITE(V4) //Mode selection: table widget
{
    char hotColor[] = "#b73838";
    char coldColor[] = "#4d9aff";
    char *color = NULL;

    if (param.asInt() == 1){ //COLD
        acMode = 1;
        color = coldColor;
    }
    if (param.asInt() == 2){ //HOT
        acMode = 2;
        color = hotColor;
    }
    Blynk.setProperty(V1,"color",color); // Temperature step
    Blynk.setProperty(V5,"color",color); // ON/OFF Button
    Blynk.setProperty(V7,"color",color); // Temperature gauge
    Blynk.setProperty(V6,"color","#2DC72D"); // LED - green
    Blynk.setProperty(V4,"color",color); // MODE

    if (acState){
        sendCodesTask(acState);
    }
}

BLYNK_WRITE(V5) // ON/OFF: button widget
{
    char ledOn[] = "255";
    char ledOff[] = "0";
    char *ledState = NULL;

    sendCodesTask(0);
    acState ^= 1;
    if (acState){
        ledState = ledOn;
    }
    else{
        ledState = ledOff;
    }
    Blynk.virtualWrite(V6,ledState);
}

void setup()
{
    pinMode(outputPin,OUTPUT);
    Blynk.begin(auth,ssid,pass); //add a 4th argument if you're using a local server i.e. "(auth,ssid,pass,localIp)"
}

void loop()
{
    Blynk.run();
}
