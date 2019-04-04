#include <SoftwareSerial.h>

SoftwareSerial Monitor (5, 4);

void setup(){
  Monitor.begin(9600);
  pinMode (4, OUTPUT);
  pinMode (5, INPUT);
}

void loop(){
  Monitor.println("teste");
}

