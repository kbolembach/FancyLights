#include "globals.h"
#include "lights.h"

void updateLights(){
    switch(mode){
        case PLAIN:
            fill_solid(leds, NUM_LEDS, fix(base_color_rgb));
        break;
        case CANDLE:
            candleFlicker();
        break;
        case CHRISTMAS:
            christmasLights(true);
        break;
        default:
            fill_solid(leds, NUM_LEDS, fix(0xFFFFFF));
            FastLED.setBrightness(255);
        break;
    }
  if (leds_enabled) FastLED.setBrightness(brightness);\
  else FastLED.setBrightness(0);
  FastLED.show();
}

void christmasLights(bool moving){
    if (moving){
        for (int i = 0; i < NUM_LEDS; i++){
            leds_chsv[i].h = CHRISTMAS_HUES[(i + christmas_counter) % 4];
            leds[i] = fix(leds_chsv[i]);
        }
        christmas_counter++;
        christmas_counter %= 4;
    }
    else{ 
        for (int i = 0; i < NUM_LEDS; i++){
            leds_chsv[i].h = CHRISTMAS_HUES[i % 4];
            leds[i] = fix(leds_chsv[i]);
        }
    }
    delay(2500);
}

void candleFlicker(){
    bounce();
    varyHue();
    delay(30);
    fadeOut(7, 2, 10, 0); // feather
    fadeOut(2, 4, 15, 0); // light
    fadeOut(3, 10, 500, 40); // strong
    if (random(1,15) == 3){
        fadeOut(3, 10, 200, 220);
    }
}

void varyHue() {
  random16_set_seed(535);  
  for(int i = 0; i<NUM_LEDS; i++)
  {
    if (random(1,10) == 2)
    {
      if (leds_chsv[i].h < STANDARD_HUE - OFFSET_HUE)
      {
        leds_chsv[i].h += 2 * random(1,2);
      }
      else if (leds_chsv[i].h > STANDARD_HUE + OFFSET_HUE){
        leds_chsv[i].h -= 2 * random(1,2);
      }
      else
      {
        leds_chsv[i].h += 2 * random(-2,2);
      }
      leds[i] = fix(leds_chsv[i]);
    }
  }
  random16_set_seed(millis());  
}

void fadeOut(int delta, int multiplier, int chance, int offset){
  random16_set_seed(millis());   
  for(int i = 0; i<NUM_LEDS; i++)
  {
    if (random(1, chance) == 2)
    {
      leds_chsv[i].v -= offset + multiplier * random(1, delta);
      leds[i] = fix(leds_chsv[i]);
    }
  }
}

void bounce(){
  random16_set_seed(535);  
  for(int i = 0; i<NUM_LEDS; i++)
  {
    if (leds_chsv[i].v < 255)
    {
      leds_chsv[i].v += 20;
      if (leds_chsv[i].v < 25)
      {
        leds_chsv[i].v = 255;
      }
      leds[i] = fix(leds_chsv[i]);
    }
  }
  random16_set_seed(millis());
}

void basicFadeIn() {
  random16_set_seed(535);                      // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.
  for (int i = 0; i<NUM_LEDS; i++) 
  {
    uint8_t fader =  sin8(millis()/random8(10,20)) *1/2;              // The random number for each 'i' will be the same every time.
    leds[i] = fix(CRGB(CHSV(35,255 - fader *2/5 , 255 - fader)));     // Now, let's assign to CHSV values.
  }
  random16_set_seed(millis());                                        // Re-randomizing the random number seed for other routines.
}

CRGB fix(CRGB color){
  CRGB temp = color;
  color.red = temp.green;
  color.green = temp.red;
  return color;
}

