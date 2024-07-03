#include "HCSR04.h"
#define TRIG_PIN_F 10
#define ECHO_PIN_F 11
#define TRIG_PIN_R 12
#define ECHO_PIN_R 13
#define TRIG_PIN_L 9
#define ECHO_PIN_L 8

HCSR04 hcF(TRIG_PIN_F, ECHO_PIN_F);
HCSR04 hcL(TRIG_PIN_R, ECHO_PIN_R);
HCSR04 hcR(TRIG_PIN_L, ECHO_PIN_L);

float dF = 0, dL = 0, dR = 0;

void getDistances(){
    dF= hcF.dist();
    dL= hcL.dist();
    dR= hcR.dist();
}