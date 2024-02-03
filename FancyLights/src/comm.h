#pragma once
#include "AltSoftSerial.h"
#include "Arduino.h"

void processData(String data);
void recvWithStartEndMarkers();
String getValueFromData(String data, String key);
void processLightModes(String data);
void handleInvalidData(String data);