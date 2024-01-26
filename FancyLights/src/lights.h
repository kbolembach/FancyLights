#pragma once
#include "SoftwareSerial.h"
#include "Arduino.h"
#include "FastLED.h"

void updateLights();
void christmasLights(bool moving);
CRGB fix(CRGB color);
void candleFlicker();
void bounce();
void fadeOut(int delta, int multiplier, int chance, int offset);
void basicFadeIn();
void varyHue();