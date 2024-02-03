#include "Arduino.h"
#include "FastLED.h"
#include "AltSoftSerial.h"

#include "globals.h"
#include "lights.h"
#include "comm.h"

AltSoftSerial soft_serial; // transmit 9, receive 8
constexpr unsigned int BAUD_RATE = 19200;

boolean new_data = false;
boolean receiving_from_arduino = false;
const byte num_chars = 32;
char received_chars[num_chars];

LightMode mode = CANDLE;
boolean moving_lights = false;

CRGB leds[NUM_LEDS];
CHSV leds_chsv[NUM_LEDS];
CHSV base_color = CHSV(STANDARD_HUE, 255, 255);
CRGB base_color_rgb = CRGB(base_color);
boolean leds_enabled = true;
int brightness = 255;
int christmas_counter = 0;
int data_received_length = 0;
int packet_target_length = 0;
int no_retries = 0;

unsigned long last_message_time = 0; 
unsigned long message_delay = 1000;

// function declarations
void recvWithStartEndMarkers();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(BAUD_RATE);
  soft_serial.begin(BAUD_RATE);
  Serial.println("Arduino ready - serial.");
  soft_serial.println("Arduino ready - softSerial.");

  FastLED.addLeds<WS2811, PIN_DATA, COLOR_ODER>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, fix(0xFF0000));
  for(int i = 0; i < NUM_LEDS; i++){
    leds_chsv[i] = CHSV(STANDARD_HUE, 255,255);
  }
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 20);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 60); // 60mA / led * 50 leds = 3A, don't plug directly into Arduino 
  FastLED.setBrightness(255);
  FastLED.show();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  recvWithStartEndMarkers();
  if (new_data == true)
  {
    last_message_time = millis();
    String data(received_chars);
    Serial.println("Received new data: " + data);

    new_data = false;
    processData(data);
  }

  if (!receiving_from_arduino) {
    updateLights(); 
  }

}