/**************************************************************************/
/*!
    @file     16ch_relay.ino
    @author   Andrej Lozar, IJS
    

    This example operates 16ch realy module over PCA9655 with I2C.
    Control is made with commands over serial communication.

    Last mod: 8/9/2020
    
*/
/**************************************************************************/
// https://github.com/sumotoy/gpio_expander
#include <Wire.h>
#include <pca9655.h>   // import library


#define MAX_MODULES 1
pca9655 *module[MAX_MODULES];//instance

// Define states
const int idleState = 0;
const int setState = 1;
const int delayState = 2;
int currentState = idleState;

//Debug
int debug = 0;

int chStates[16];

void turnAllOff();
void setChannel();

void setup(){
  
  pinMode(13, OUTPUT); // to show we are working
  Serial.begin(38400);
  delay(100);
  for (int j=0;j<MAX_MODULES;j++){
    module[j]= new pca9655(0x20+j);
    module[j]->begin();//x.begin(true) will override automatic SPI initialization
    module[j]->gpioPinMode(OUTPUT);
    if(debug) Serial.print("Initialise module: ");
    //Serial.println(j);
    
    for(int i = 0; i<16; i++){
      module[j]->gpioDigitalWriteFast(i, HIGH); // write in the pin register
      chStates[i] = 0;
    }
    if(debug) Serial.println("Update all pins"); //Send whole register to the chip and update pins
    //module[j]->gpioDigitalWriteFast(0, LOW); // write in the pin register
    module[j]->gpioPortUpdate();
    module[j]->gpioDigitalWrite(0, HIGH); // use this function to "force" update all channels from register
  }

}

void loop(){
  int j = 0;
  int ch = 0;
  //Serial command
  String command;

  // Variables
  int VSET = 0;
  int index0, index1;
  
  String command_0 = "";
  String arg = "";

  while(1){
    switch(currentState){
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
              Serial.println("This is relay state machine. ATM on idle. Source: 16ch_relay.ino"); 
              Serial.println("Commands:");
              Serial.println("- set(X)    : Set ch X HIGH or LOW");
              Serial.println("- help()   : display help");
            }
            
            else if(command_0.equals("set")){
              currentState = setState; // change state to set channel state
              setChannel(arg.toInt());
            }

            else if(command.equals("debug()")){
              
              switch(debug){
                case 0:
                  Serial.println("Turn debug on.");
                  debug = 1;
                  break;
                
                case 1:
                  Serial.println("Turn debug off.");
                  debug = 0;
                  break;
              }
            }

            else if(command.equals("off()")){
              turnAllOff();
            }
            else if(command_0.equals("mux")){
              multiplex(arg.toInt());
            }
            
            else{
              Serial.print("?: "); Serial.println(command); 
            }
            
          }
          break;

          /*case setState:
            ch = arg.toInt();
            Serial.print("Change channel: "); Serial.println(ch);
            switch(chStates[ch]){
              case 0:
                //HIGH->LOW
                //turn on
                module[j]->gpioDigitalWrite(ch, LOW);
                digitalWrite(13, 1);
                digitalWrite(13, 0); 
                chStates[ch] = 1;
                break;

              case 1:
                //LOW->HIGH
                //turn off
                module[j]->gpioDigitalWrite(ch, HIGH);
                digitalWrite(13, 1);
                digitalWrite(13, 0); 
                chStates[ch] = 0;
                break;
              }    
            currentState = idleState;
            break;*/
    }

    
  }
}

void turnAllOff(){
  int j=0;
  Serial.println("All off.");
  for(int i = 0; i<16; i++){
      module[j]->gpioDigitalWriteFast(i, HIGH); // write in the pin register
      chStates[i] = 0;
    }
    if(debug) Serial.println("Turn off all channels."); //Send whole register to the chip and update pins
    //module[j]->gpioDigitalWriteFast(0, LOW); // write in the pin register
    module[j]->gpioPortUpdate();
    module[j]->gpioDigitalWrite(0, HIGH); // use this function to "force" update all channels from register
   
}

void setChannel(int x){
  Serial.print("Change channel: "); Serial.print(x);
  switch(chStates[x]){
    case 0:
      //HIGH->LOW
      //turn on
      module[0]->gpioDigitalWrite(x, LOW);
      digitalWrite(13, 1);
      digitalWrite(13, 0); 
      chStates[x] = 1;
      Serial.println(" ON.");
      break;

    case 1:
      //LOW->HIGH
      //turn off
      module[0]->gpioDigitalWrite(x, HIGH);
      digitalWrite(13, 1);
      digitalWrite(13, 0); 
      chStates[x] = 0;
      Serial.println(" OFF.");
      break;
    }    
  currentState = idleState;
}

void multiplex(int diode){
  int R1 = 8;
  int R2 = 0;
  int R3 = 9;
  int R4 = 1;
  int R5 = 10;

  int R6 = 2;
  int R7 = 11;
  int R8 = 3;
  
  turnAllOff();
  switch(diode){
    case 1:
      /* if using only 3 channels <- old version
      if(chStates[R1]==1) setChannel(R1);
      if(chStates[R3]==0) setChannel(R3);
      
      if(chStates[R5]!=0) setChannel(R5);
      if(chStates[R4]!=1) setChannel(R4);*/

      if(chStates[R2]!=1) setChannel(R2);
      if(chStates[R1]!=0) setChannel(R1);
      if(chStates[R3]!=1) setChannel(R3);

      if(chStates[R6]!=1) setChannel(R6);
      if(chStates[R5]!=0) setChannel(R5);
      if(chStates[R4]!=1) setChannel(R4);
      break;
   
    case 2:
      if(chStates[R1]==0) setChannel(R1);
      if(chStates[R3]==0) setChannel(R3);
      
      if(chStates[R5]!=1) setChannel(R5);
      if(chStates[R4]!=1) setChannel(R4);
      break;
      
    case 3:
      if(chStates[R1]==1) setChannel(R1);
      if(chStates[R3]==1) setChannel(R3);
      
      if(chStates[R5]!=0) setChannel(R5);
      if(chStates[R4]!=0) setChannel(R4);
      break;

    case 4:
      if(chStates[R2]!=0) setChannel(R2);
      if(chStates[R1]!=0) setChannel(R1);
      if(chStates[R3]!=1) setChannel(R3);

      if(chStates[R6]!=0) setChannel(R6);
      if(chStates[R5]!=0) setChannel(R5);
      if(chStates[R4]!=1) setChannel(R4);
      break;
  }
    
  
}
