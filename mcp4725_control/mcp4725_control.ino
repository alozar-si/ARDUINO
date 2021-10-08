/**************************************************************************/
/*!
    @file     DACandADC.ino
    @author   Andrej Lozar, IJS
    

    This example operates MCP4725 DAC I2C.
    Control is made with commands over serial communication.
    Library used: AdafruitMCP4725 2.0.0
    Last mod: 8/10/2021
    History:
    
*/
/**************************************************************************/
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

// Define states
const int idleState = 0;
const int measureState = 1;
const int delayState = 2;
const int setVoltageState = 3;
const int switchDacState = 4;
const int operateCurrent = 5;
const int pulseModeState = 6;

int currentState = idleState;

// Status variables
int DAC_status;
int current_status;

//Debug
int debug = 1;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  if(debug) Serial.println("Serial initialized.");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  // Voltage values: 0-4095
  
  if(dac.begin(0x60)== 0){
    Serial.println("DAC initialization failed!");
  }else{
    if(debug) Serial.println("DAC connected (0x60).");
  }
  
  
  delay(1);
  dac.setVoltage(0, false);
  DAC_status = 0;

}

void loop(void) {    
    //Serial command
    String command;
    
    // Variables
    int VSET = 0;
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
              Serial.println("This is DAC state machine. ATM on idle. Source: mcp4725_control.ino"); 
              Serial.println("Commands:");
              Serial.println("- set(X)    : Set DAC voltage to X, where X is an integer between 0 (0V) and 4095 (Vcc)");
              Serial.println("- on(X)     : Turn on DAC voltage (X=1), turn off voltage (X=0)");
              Serial.println("- debug()   : Enable/disable debug mode");
            }
            
            else if(command_0.equals("set")){
              currentState = setVoltageState; // change state to set voltage state
            }
        
            else if(command_0.equals("on")){
              currentState = switchDacState; // change state to switch DAC state
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
       
        case setVoltageState :
          // set voltage, if DAC on, force change it
          VSET = arg.toInt();
          
          if((0 > VSET) or (VSET > 4095)){
            Serial.println("DAC off rng");
            Serial.println("set to 0");
            VSET=0;
          }
          
          if(debug) {
            Serial.print("Set voltage to ");
            Serial.println(VSET);
          }
          
          if(DAC_status) dac.setVoltage(VSET, false);
          
          currentState = idleState; // change state to idle state
          break;
          
        case switchDacState:
          DAC_status = arg.toInt();
          if(debug) Serial.print("Switching DAC "); 
          switch(DAC_status){
            case 1:
              digitalWrite(LED_BUILTIN, HIGH);
              dac.setVoltage(VSET, false);
              break;
            case 0:
              digitalWrite(LED_BUILTIN, LOW);
              dac.setVoltage(0, false);
              break;
          }
          if(debug) Serial.println(DAC_status);
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
