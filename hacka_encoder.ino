#include <SoftwareSerial.h>
SoftwareSerial MasterSerial(5, 4); // RX, TX
#define CHA 2
#define CHB 3

#include "command.h"

volatile int master_count = 0;
volatile byte INTFLAG1 = 0;

void setup() {
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  Serial.begin(9600);
  MasterSerial.begin(9600);
  attachInterrupt(0, flag, RISING);
}

void loop() {
  if (INTFLAG1)   {
    Serial.println(master_count);
    master_count = master_count / 600;
    UpdatetoMaster(String(setEncoder), String(master_count));
   delay(500);
    INTFLAG1 = 0;
  }
}

void UpdatetoMaster(String Command, String data) {
  Serial.println(String(Start) + Command + data + String(End));
                 MasterSerial.print(String(Start) + Command + data + String(End));
}

void flag() {
  INTFLAG1 = 1;
  if (digitalRead(CHA) && !digitalRead(CHB)) {
    master_count++ ;
  }
  if (digitalRead(CHA) && digitalRead(CHB)) {
    master_count-- ;
  }
}