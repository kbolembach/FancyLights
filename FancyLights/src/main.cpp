#include "Arduino.h"
#include "FastLED.h"
#include "SoftwareSerial.h"

#define NUM_LEDS 50
#define PIN_DATA 6
#define PIN_WIFI 7
#define COLOR_ODER GRB // this doesn't work - a bug in the FastLED library. Left in case they fix it someday

#define TWINKLE_SPEED 4
#define TWINKLE_DENSITY 5

#define STANDARD_HUE 200
#define OFFSET_HUE 5

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];

enum LightMode { plain, candle, christmas };
LightMode mode = plain;

SoftwareSerial softSerial(2, 3);

CRGB leds[NUM_LEDS];

// function declarations
void recvWithStartEndMarkers();
CRGB fix(CRGB color);


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(19200);
  softSerial.begin(19200);
  Serial.println("Arduino ready - serial.");
  softSerial.println("Arduino ready - softSerial.");

  FastLED.addLeds<WS2811, PIN_DATA, COLOR_ODER>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, fix(0xFF0000));
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 20);
  FastLED.setBrightness(255);
  FastLED.show();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  recvWithStartEndMarkers();
  if (newData == true)
  {
    String data(receivedChars);
    Serial.println(data);

    newData = false;
  }

}

void recvWithStartEndMarkers(){
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (softSerial.available() > 0 && newData == false) {
        rc = softSerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

CRGB fix(CRGB color){
  CRGB temp = color;
  color.red = temp.green;
  color.green = temp.red;
  return color;
}