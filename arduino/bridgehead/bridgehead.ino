// fancontrol.ino
//
// Intentionally left simple and stupid so that all intelligent code can reside on the host side.

#include <SoftPWM.h>

// pins to which the fans are connected physically
// connector index  0  1  2  3   4   5   6   7
int PINS_PWM[]   = {2, 6, 9, 4, A3, 15, 10, A1};
int PINS_PULSE[] = {3, 7, 8, 5, A2, 14, 16, A0};

void setup() {
  // initialize serial connection
  Serial.begin(9600);
  
  // init pwm
  SoftPWMBegin();
  for (int i = 0; i < 8; i++) {      
    pinMode(PINS_PULSE[i], INPUT_PULLUP);
    
    SoftPWMSetFadeTime(PINS_PWM[i], 0, 0);
    SoftPWMSet(PINS_PWM[i], 0);
  }

  // set values to a safe and quiet default
  //int PWMS[] = {255, 255, 255, 16, 100, 100, 255, 100}; // summer
  //int PWMS[] = {255, 255, 255, 32, 120, 120, 255, 120}; // gaming
  //int PWMS[] = {255, 255, 255, 255, 120, 120, 255, 120}; // testing
  //int PWMS[] = {255, 255, 255, 56, 145, 145, 255, 145}; // streaming
  int PWMS[] = {169, 255, 255, 255, 167, 181, 196, 176}; // desktop
  for (int i = 0; i < 8; i++) {
    SoftPWMSet(PINS_PWM[i], PWMS[i]);
  }
}

void loop() {
  // get message from serial console
  String message = "";
  while (Serial.available())
    message.concat((char) Serial.read());
  if (message == "")
    return;

  // iterate fans, collect pulse durations
  String pulseDurations = ":";
  for (int i = 0; i < 8; i++) {
    // parse value left of a ';' 
    int value = message.substring(0, message.indexOf(';')).toInt();
    
    SoftPWMSet(PINS_PWM[i], value);
    
    // strip part left of a ';' from message
    message = message.substring(message.indexOf(';') + 1);

    // respond with pulse durations
    pulseDurations = pulseDurations + String(pulseIn(PINS_PULSE[i], LOW)) + ";";
    
    // if there is no further ';' in the message, parse and set the rest of it
    if (message.indexOf(';') == -1)
      SoftPWMSet(PINS_PWM[i], message.toInt());
  }
  Serial.println(pulseDurations);
}

