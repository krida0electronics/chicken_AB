
#include <TimerOne.h>
#include "OneButton.h"

#define RLY_LEFT  3
#define RLY_RIGHT 2
#define LED_POWER 4
#define LED_FORWARD 5
#define LED_REVERSE 6
#define LIGHT_SENSOR 0
#define SENSIVITY 1
#define TIMER_1 2
#define TIMER_2 3
#define BUTTON_A 8
#define BUTTON_B 7

#define TIMER_X 30        // coefficient that sets the maximum delay in seconds

#define HYSTERESIS 10

#define DEBUGGING 0

OneButton button_A(8, true, false);         
OneButton button_B(7, true, false);   


byte Mode = 0;     // 0 - auto, 1 - manual
byte NewEvent = 0;
byte Function = 0;
unsigned int light_value, sensivity_value, timer1_value, timer2_value, delay_X1, delay_X2, led_count;

void setup() {

pinMode(RLY_LEFT, OUTPUT);       // RELAY LEFT OUTPUT
pinMode(RLY_RIGHT, OUTPUT);      // RELAY RIGHT OUTPUT
pinMode(LED_POWER, OUTPUT);      // LED "POWER" OUTPUT
pinMode(LED_FORWARD, OUTPUT);    // LED "FORWARD" OUTPUT
pinMode(LED_REVERSE, OUTPUT);    // LED "REVERSE" OUTPUT

digitalWrite(LED_POWER, 1);      // Turn ON led "POWER"

button_A.attachClick(single_click_1);
button_B.attachClick(single_click_2);
  
button_A.attachDoubleClick(double_click_1);
button_B.attachDoubleClick(double_click_2);

Serial.begin(115200);

Timer1.attachInterrupt(timerIsr);
Timer1.initialize(5000);              // us

}

void timerIsr() {

button_A.tick();
button_B.tick();

light_value = analogRead(LIGHT_SENSOR);
sensivity_value = analogRead(SENSIVITY);
timer1_value = analogRead(TIMER_1);
timer2_value = analogRead(TIMER_2);

if (light_value > (sensivity_value+HYSTERESIS)) { NewEvent = 1; }    // High sunlight level, gates open
if (light_value < (sensivity_value-HYSTERESIS)) { NewEvent = 2; }    // Low sunlight level, gates closed

// ........low level...<-.*.->...high level...........

//////////////////////////////////////////

if (DEBUGGING) { 
                Serial.print("|   ");
                Serial.print(light_value);     Serial.print("   |   ");
                Serial.print(sensivity_value); Serial.print("   |   ");
                Serial.print(timer1_value);    Serial.print("   |   ");
                Serial.print(timer2_value);  Serial.println("   |   ");
               }

if (Mode == 1) {
                led_count++;
                if (led_count == 50) { led_count = 0;
                                         if (digitalRead(4)==1) {digitalWrite(LED_POWER, 0);}
                                          else {digitalWrite(LED_POWER, 1);}
                                       }  
 
               }

}


void single_click_1() {
                       //Serial.println("click button A");
                      }
              

void single_click_2() { 
                       //Serial.println("click button B"); 
                      }



void double_click_1() {
                       if (Mode == 0) { Mode = 1; }
                        else {Mode = 0; digitalWrite(LED_POWER, 1);}
                      }

void double_click_2() {
                       if (Mode == 0) { Mode = 1; }
                        else {Mode = 0; digitalWrite(LED_POWER, 1);}
                      }                    


void loop() {

if (Mode == 0) {
                switch(NewEvent) { 
                                  case 1: if(Function == 0) 
                                          {
                                           if (DEBUGGING) {Serial.println("|   GATE OPEN   |");}
                           
                                           delay_X1 = timer1_value * TIMER_X;
                                           digitalWrite(RLY_RIGHT, 1);   
                                           digitalWrite(LED_FORWARD, 1); delay(delay_X1); digitalWrite(LED_FORWARD, 0); 
                                           digitalWrite(RLY_RIGHT, 0);   
                                           Function = 1;
                                          } 
                                          break;

                                  case 2: if(Function == 1) 
                                          {
                                           if (DEBUGGING) {Serial.println("|   GATE CLOSE   |");}
                           
                                           delay_X2 = timer2_value * TIMER_X;
                                           digitalWrite(RLY_LEFT, 1);   
                                           digitalWrite(LED_REVERSE, 1); delay(delay_X2); digitalWrite(LED_REVERSE, 0); 
                                           digitalWrite(RLY_LEFT, 0); 
                                           Function = 0;
                                          } 
                                          break;                
                                 }
                }

if (Mode == 1) {
                 if (digitalRead(8)==0) {digitalWrite(LED_FORWARD, 1); digitalWrite(RLY_RIGHT, 1);}
                  else {digitalWrite(LED_FORWARD, 0); digitalWrite(RLY_RIGHT, 0);}

                 if (digitalRead(7)==0) {digitalWrite(LED_REVERSE, 1); digitalWrite(RLY_LEFT, 1);}
                  else {digitalWrite(LED_REVERSE, 0); digitalWrite(RLY_LEFT, 0);} 
               }
              
      
}
