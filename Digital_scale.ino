#include "HX711.h"
#include <SoftwareSerial.h>

HX711 scale; // scale name
SoftwareSerial BTserial(7, 6); // RX | TX
uint8_t dataPin = 3; //bt data pin
uint8_t clockPin = 2; //bt clock pin
float w1, w2, previous = 0; 

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600); //initialise BT module
  scale.begin(dataPin, clockPin); //initialise HX711
  scale.set_scale(3048);
  scale.tare();
}

void loop() {
// read until stable
  w1 = scale.get_units(10);
  delay(50);
  w2 = scale.get_units();
  while (abs(w1 - w2) > 10){
     w1 = w2;
     w2 = scale.get_units();
     delay(50);
  }  
  //Serial.print("Grams: ");
  if (w1 < 0.5 && w1 > -0.5){w1 = 0;}
  BTserial.print(w1);
  BTserial.print("\n");
  if (w1 == 0){Serial.println();}
  else{
    previous = w1;
  }
  delay(10);

  // Keep reading from HC-06 and send to Arduino Serial Monitor
  while (BTserial.available()){
    //Serial.write(BTserial.read());
    delay(3);  
    String c = BTserial.readString();
    c.trim();
    if(c == "Tare") {
      scale.tare();
    }    
  }
 
  // Keep reading from Arduino Serial Monitor and send to HC-06
  while (Serial.available()){
    BTserial.write(Serial.read());
  }
}
