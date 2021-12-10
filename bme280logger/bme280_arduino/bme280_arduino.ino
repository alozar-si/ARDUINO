/**************************************************************************/
/*!
    @file     bme280_monitor.ino
    @author   Andrej Lozar, IJS
    

    This example operates bme280 sensor.
    Control is made with commands over serial communication.

    Last mod: 06/08/2021
    History:
    06/08/2021: initialised
    
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

// Define states
int newstate;
const int idleState = 0;
const int measure = 1;
const int delayState = 2;

int currentState = idleState;

//Debug
int debug = 0;

// BME variables
unsigned long delayTime;
Adafruit_BME280 bme; // I2C

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  
 
  Serial.begin(115200);
  while(!Serial);    // time to get serial running
    unsigned status;
    
    // default settings
    //status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    status = bme.begin(0x76);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    delayTime = 1000;

}

void loop(void) {    
    //Serial command
    String command;
    
    // Variables
    int index0, index1;
    
    String command_0 = "";
    String arg = "";
    
    
    while(1){
      switch(currentState)
      {
        case idleState :
          if(Serial.available()){
            command = Serial.readStringUntil('\n');
            command_0 = ""; // reset string
            arg = ""; // reset string
            
            index0 = command.indexOf("(");
            index1 = command.indexOf(")");
            
            if((index0 == -1) or (index1 == -1)){
              if(debug) Serial.print("Expected '(' or ')': ");
              Serial.print("-1: ");Serial.println(command);
              break;
            }
            
            for(int i = 0; i < index0; i++){
              //Serial.print(command[i]);
              command_0.concat(command[i]);
            }
            for(int i = index0+1; i < index1; i++){
              //Serial.print(command[i]);
              arg.concat(command[i]);
            }
            
            if(debug){
              Serial.print("Index: ");
              Serial.print(index0);
              Serial.print(", ");
              Serial.println(index1);
              Serial.print("Command: ");
              Serial.println(command_0);
              Serial.print("Argument: ");
              Serial.println(arg);
            }
            
            
            if(command.equals("help()")){
              Serial.println("This software is used for controling bme280 sensor. Source: bme280_monitor.ino"); 
              Serial.println("Commands:");
              Serial.println("- help()    : Displays this help");
              Serial.println("- get()     : Prints temperature, humidity and pressure. Format: X, Y, Z;");
              Serial.println("- debug()   : Enable/disable debug mode");

            }
            
            else if(command_0.equals("get")){
              currentState = measure; // change state
            }
            else if(command.equals("debug()")){
              switch(debug){
                case 0:
                  debug = 1;
                  break;
                
                case 1:
                  debug = 0;
                  break;
              }
            }
            else{
              Serial.print("?: "); Serial.println(command); 
            }
            
          }
          break;
          
        case measure :
          newstate = arg.toInt();
          if(debug) {Serial.println("Printig sensor values to serial");}

          Serial.print(bme.readTemperature());
          Serial.print(", "); 
          Serial.print(bme.readPressure() / 100.0F);
          Serial.print(", ");
          Serial.print(bme.readHumidity());
          Serial.println(";");
          
          currentState = idleState; // change state to idle state
          break;
          
        case delayState :
          delay(1);
          currentState = idleState;
          break;
          
        default :
          //undefined state
          break;
   
      }
    }
}
