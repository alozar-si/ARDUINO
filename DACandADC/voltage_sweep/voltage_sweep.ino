/**************************************************************************/
/*!
    @file     sinewave.pde
    @author   Adafruit Industries
    @license  BSD (see license.txt)

    This example will generate a sine wave with the MCP4725 DAC.

    This is an example sketch for the Adafruit MCP4725 breakout board
    ----> http://www.adafruit.com/products/935

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!
*/
/**************************************************************************/
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>

Adafruit_MCP4725 dac;
Adafruit_ADS1115 ads(0x48);  // ADC object at I2C address 0x48 for addr pin = GND

int16_t adc0;  // variables to hold ADC readings
float multiplier = 0.1875F;               // ADS1115  @ +/- 6.144V gain = 0.1875mV/step
//float multiplier = 0.125F;               // ADS1115  @ +/- 2.048V  gain = 0.125mV/step
//float multiplier = 0.0625F;               // ADS1115  @ +/- 1.024V  gain = 0.0625mV/step

float adcScale   = 4.8828F;               // Arduino 10 bit @ 5 volts   = 4.88mV/step

// Define states
const int idleState = 0;
const int measureState = 1;
const int delayState = 2;
const int setVoltageState = 3;
const int switchDacState = 4;
int currentState = idleState;

// Status variables
int DAC_status;

//Serial command
String command;

// Variables
int VSET = 0;
int Vlast = 0;
int index0, index1;

//Debug
int debug = 0;

void setup(void) {
  Serial.begin(115200);
  if(debug) Serial.println("Serial initialized.");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  // Voltage values: 0-4095
  
  dac.begin(0x60);
  
  if(debug) Serial.println("DAC connected (0x60).");
  delay(1);
  dac.setVoltage(0, false);
  DAC_status = 0;
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1115
  //                                                                -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
  //ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV
  //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  ads.begin();  // init ADS1115 ADC
  
}

void loop(void) {    
    String command_0 = "";
    String arg = "";

    for(int i = 0; i <= 4095; i++){
      dac.setVoltage(i, false);
      delay(5);
      adc0 = ads.readADC_SingleEnded(0);        // read single AIN0
      Serial.println(adc0 * multiplier/1000, 5);
    }
      
}
