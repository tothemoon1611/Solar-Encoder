#include <SoftwareSerial.h>
SoftwareSerial MasterSerial(5, 4); // RX, TX
#define CHA 2
#define CHB 3

#include "command.h"

int master_count = 0;
byte INTFLAG1 = 0;
int PanPos = 0;
int panel_resolution = 3000; 

bool SerialRecv_MasterSerial = false;
int serial_counter_MasterSerial = 0;
char cmd_MasterSerial;
String InputString_MasterSerial = "";
boolean StringComplete_MasterSerial = false;

void Get_Serial_Master() ;

void setup() {
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  Serial.begin(9600);
  MasterSerial.begin(9600);
  attachInterrupt(0, flag, RISING || FALLING);
}

void loop() {
  if (INTFLAG1){
    Serial.println(master_count);
    PanPos = ((master_count) / panel_resolution);   
    UpdatetoMaster(String(setEncoder), String(PanPos));   
    delay(500);
    INTFLAG1 = 0;
  }
  Get_Serial_Master() ;
}

void UpdatetoMaster(String Command, String data) 
{
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


void Get_Serial_Master() 
{
  if (MasterSerial.available())
  {
    Serial.println("Data Having") ;
    char inChar_MasterSerial = (char)MasterSerial.read();
    if (inChar_MasterSerial == Start) SerialRecv_MasterSerial = true;
    if (inChar_MasterSerial == End)
    {
      SerialRecv_MasterSerial = false;
      serial_counter_MasterSerial = 0;
      StringComplete_MasterSerial = true;
    }
    if (SerialRecv_MasterSerial)  serial_counter_MasterSerial++;
    if (serial_counter_MasterSerial == 2) cmd_MasterSerial = inChar_MasterSerial;
    if (serial_counter_MasterSerial > 2) InputString_MasterSerial += inChar_MasterSerial;
  }
  if (StringComplete_MasterSerial) 
    {
      Serial.println("Data Rcv") ;
      if(cmd_MasterSerial == ResetEncoder)
        { 
          master_count = 0;
          INTFLAG1 = 0;
          PanPos = 0;
        }
      if(cmd_MasterSerial == NextPanel)
        { 
          master_count = (PanPos + 1)*panel_resolution ;
          INTFLAG1 = 0;
          
        }  
      cmd_MasterSerial = "" ;
      InputString_MasterSerial = "";
      StringComplete_MasterSerial = false; 
    }
}
