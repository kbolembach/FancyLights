#pragma once
#include "SoftwareSerial.h"
#include "Arduino.h"
#include "FastLED.h"

constexpr int NUM_LEDS = 50;
constexpr int PIN_DATA = 6;
constexpr int PIN_WIFI = 7;
constexpr EOrder COLOR_ODER = GRB; // this doesn't work - a bug in the FastLED library. Left in case they fix it someday

constexpr int TWINKLE_SPEED = 4;
constexpr int TWINKLE_DENSITY = 5;

constexpr int STANDARD_HUE = 32;
constexpr int OFFSET_HUE = 5;

constexpr int CHRISTMAS_HUES[4] = { 0, 32, 96, 160 }; // red, orange, green, blue
extern int christmas_counter;

extern SoftwareSerial soft_serial;

extern boolean new_data;
extern const byte num_chars;
extern char received_chars[];

enum LightMode { PLAIN, CANDLE, CHRISTMAS };
extern LightMode mode;

extern CRGB leds[];
extern CHSV leds_chsv[];
extern CHSV base_color;
extern CRGB base_color_rgb;