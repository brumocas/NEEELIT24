#include <Arduino.h>
#include <stdlib.h>
#include "Motor.h"
#include "wifi.h"
#include "distance.h"


typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

// Our finite state machines
fsm_t fsm1, fsm2;

unsigned long interval, last_cycle;
unsigned long loop_micros;
uint16_t blink_period;

// Set new state
void set_state(fsm_t& fsm, int new_state)
{
  if (fsm.state != new_state) {  // if the state chnanged tis is reset
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
  }
}

void setup() 
{  
  // Initialize the LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Start the serial port with 115200 baudrate
  Serial.begin(115200);

  // Init Motors
  init_motors();
  init_pose();

  // Init Wifi
  initWifi();

  // Set 1 state
  set_state(fsm1, 0);
  set_state(fsm2, 0);
  interval = 30;
}

void loop() 
{
   
    // To measure the time between loop() calls
    unsigned long last_loop_micros = loop_micros; 

    // Do this only every "interval" miliseconds 
    // It helps to clear the switches bounce effect
    unsigned long now = millis();
    if (now - last_cycle > interval) {
      loop_micros = micros();
      last_cycle = now;

      // Inputs
      client_request();
      getDistances();
      
      // FSM 
      if ((fsm1.state == 0 || fsm1.state == 2 || fsm1.state == 3 || fsm1.state == 4 || fsm1.state == 5 || fsm1.state == 6) && mode == "w") {
        fsm1.new_state = 1;
      } else if ((fsm1.state == 0 || fsm1.state == 1 || fsm1.state == 3 || fsm1.state == 4 || fsm1.state == 5 || fsm1.state == 6) && mode == "s"){
        fsm1.new_state = 2;
      } else if ((fsm1.state == 0 || fsm1.state == 2 || fsm1.state == 1 || fsm1.state == 4 || fsm1.state == 5 || fsm1.state == 6) && mode == "b"){
        fsm1.new_state = 3;
      } else if ((fsm1.state == 0 || fsm1.state == 2 || fsm1.state == 3 || fsm1.state == 1 || fsm1.state == 5 || fsm1.state == 6) && mode == "r"){
        fsm1.new_state = 4;
      } else if ((fsm1.state == 0 || fsm1.state == 2 || fsm1.state == 3 || fsm1.state == 4 || fsm1.state == 1 || fsm1.state == 6) && mode == "l"){
        fsm1.new_state = 5;
      } else if ((fsm1.state == 0 || fsm1.state == 2 || fsm1.state == 3 || fsm1.state == 4 || fsm1.state == 5 || fsm1.state == 1) && mode == "a"){
        fsm1.new_state = 6;
      }

      
      if ((fsm1.state == 6 && fsm2.state == 0)) {
        fsm2.new_state = 1;
      } else if  (fsm2.state == 1 && dF < 13){
          if (dL < 13){
          fsm2.new_state = 2;
          } else if (dR < 13){
          fsm2.new_state = 3;
          }
          else {
            int randomDirection = random(2);
            // If randomDirection is 0, select left, else select right
            if (randomDirection == 0) {
              fsm2.new_state = 2; // Left direction
            } else {
              fsm2.new_state = 3; // Right direction
            }
          }
      } else if (fsm1.state != 6){
        fsm2.new_state = 0;
      }

      // NEW STATE
      set_state(fsm1, fsm1.new_state);
      set_state(fsm2, fsm2.new_state);

      // OUTPUTS 
      if (fsm1.state == 0 || fsm1.state == 2){
        // Start with LED off 
        digitalWrite(LED_BUILTIN, LOW);
        Stop();
      } else if (fsm1.state == 1){
        // Start with LED on
        digitalWrite(LED_BUILTIN, HIGH);
        MoveF();
      } else if (fsm1.state == 3){
        // Start with LED on
        digitalWrite(LED_BUILTIN, HIGH);
        MoveB();
      }else if (fsm1.state == 4){
        // Start with LED on
        digitalWrite(LED_BUILTIN, HIGH);
        MoveR();
        fsm1.new_state = 2;
        mode = 's';
      } else if (fsm1.state == 5){
        // Start with LED on
        digitalWrite(LED_BUILTIN, HIGH);
        MoveL();
        fsm1.new_state = 2;
        mode = 's';
      } else if (fsm1.state == 6){
        // Start with LED on
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }

    if (fsm2.state == 1){
        MoveF();
      } else if (fsm2.state == 2 ){
        MoveR();
        fsm2.state = 0;
      } else if (fsm2.state == 3 ){
        MoveL();
        fsm2.state = 0;
      }


    // DEBUG
    Serial.print(" IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" FSM1: ");
    Serial.print(fsm1.state);
    Serial.print(" FSM2: ");
    Serial.print(fsm2.state);
    Serial.print(" DF: ");
    Serial.print(dF);
    Serial.print(" DR: ");
    Serial.print(dR);
    Serial.print(" DL: ");
    Serial.print(dL);
    Serial.println();

}



