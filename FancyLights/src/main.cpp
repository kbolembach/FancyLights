#include "Arduino.h"
#include "FastLED.h"
#include "SoftwareSerial.h"

#include "globals.h"
#include "lights.h"

SoftwareSerial soft_serial(2, 3);

boolean new_data = false;
const byte num_chars = 32;
char received_chars[num_chars];

LightMode mode = CHRISTMAS;

CRGB leds[NUM_LEDS];
CHSV leds_chsv[NUM_LEDS];
CHSV base_color = CHSV(STANDARD_HUE, 255, 255);
CRGB base_color_rgb = CRGB(base_color);
int christmas_counter = 0;

// function declarations
void recvWithStartEndMarkers();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(19200);
  soft_serial.begin(19200);
  Serial.println("Arduino ready - serial.");
  soft_serial.println("Arduino ready - softSerial.");

  FastLED.addLeds<WS2811, PIN_DATA, COLOR_ODER>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, fix(0xFF0000));
  for(int i = 0; i < NUM_LEDS; i++){
    leds_chsv[i] = CHSV(STANDARD_HUE, 255,255);
  }
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 60);
  FastLED.setBrightness(255);
  FastLED.show();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  recvWithStartEndMarkers();
  if (new_data == true)
  {
    String data(received_chars);
    Serial.println(data);

    new_data = false;
  }

  updateLights();
}


void recvWithStartEndMarkers(){
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (soft_serial.available() > 0 && new_data == false) {
        rc = soft_serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                received_chars[ndx] = rc;
                ndx++;
                if (ndx >= num_chars) {
                    ndx = num_chars - 1;
                }
            }
            else {
                received_chars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                new_data = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

