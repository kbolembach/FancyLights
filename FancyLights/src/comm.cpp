#include "globals.h"
#include "comm.h"

constexpr char* ERR_VAL = "Error! No value found.";

void processData(String data){
    String new_color;
    String new_brightness;

    data_received_length++;

    switch (data[0]){
        case 'e':
        receiving_from_arduino = false;
        Serial.println("Data received length: " + String(data_received_length) + ".");
        Serial.println();
        break;

        case 's':
        packet_target_length = data.substring(1, data.length()).toInt(); // TODO: make this safer
        Serial.println("Packet target length: " + String(packet_target_length) + ".");
        receiving_from_arduino = true;
        break;

        case 'm': //light Mode
        processLightModes(data);
        break;

        case 'c': //Color
        new_color = "0x" + data.substring(2, 8);
        if (new_color != ERR_VAL){
            Serial.println("Color: " + new_color);
            auto c = strtoul(new_color.c_str(), NULL, 0);
            base_color_rgb = CRGB(c);
            base_color = rgb2hsv_approximate(base_color_rgb);
        }
        else handleInvalidData(data);
        break;

        case 'b': //Brightness
        new_brightness = data.substring(1, data.length());
        if (new_brightness != ERR_VAL){
            Serial.println("Brightness: " + new_brightness);
            brightness = new_brightness.toInt();
        }
        else handleInvalidData(data);
        break;

        default:
        handleInvalidData(data);
        break; 
    }
}

void processLightModes(String data){
    switch (data[1]){
        case '0': // turn leds on/off
        if (data.length() < 3) {
            handleInvalidData(data);
            break;
        }
        if (data[2] == '0') {
            leds_enabled = false;
            Serial.println("LEDs disabled.");
        }
        else if (data[2] == '1') {
            leds_enabled = true;
            Serial.println("LEDs enabled.");
        }
        else handleInvalidData(data);
        break;

        case '1': // plain light mode
        Serial.println("Plain color mode.");
        mode = PLAIN;
        break;

        case '2': // candle flicker mode
        Serial.println("Candle mode.");
        mode = CANDLE;
        break;

        case '3': // christmas lights mode
        if (data.length() < 3){
            handleInvalidData(data);
            break;
        }
        if (data[2] == '0') {
            moving_lights = false;
            Serial.println("Christmas lights mode, moving = false.");
        }
        else if (data[2] == '1') {
            moving_lights = true;
            Serial.println("Christmas lights mode, moving = true.");
        }
        else {
            handleInvalidData(data);
            break;
        }
        mode = CHRISTMAS;
        break;
    }
}

void handleInvalidData(String data){
    Serial.println("Invalid data: " + data);
}

// void processData(String data){
//     // Serial.println("Incoming data: " + data);
//     String new_color;
//     String new_brightness;
//     switch (data[0]){
//         case '0':
//             if (data.length() < 3) {
//                 Serial.println("Invalid data: " + data);
//                 break;
//             }
//             if (data[2] == '0') leds_enabled = false;
//             else if (data[2] == '1') leds_enabled = true;
//             else Serial.println("Invalid data: " + data);
//             break;
            
//         case '1':
//             Serial.println("Plain color mode.");
//             new_color = "0x" + getValueFromData(data, "CO").substring(1, 7);
//             new_brightness = getValueFromData(data, "BR");
//             Serial.println("Color: " + new_color);
//             // Serial.println("Brightness: " + new_brightness);
//             if (new_color != ERR_VAL && new_brightness != ERR_VAL){
//                 auto c = strtoul(new_color.c_str(), NULL, 0);
//                 base_color_rgb = CRGB(c);
//                 base_color = rgb2hsv_approximate(base_color_rgb);
//                 brightness = new_brightness.toInt();
//             }
//             mode = PLAIN;
//             break;

//         case '2':
//             Serial.println("Candle mode.");
//             mode = CANDLE;
//             break;

//         case '3':
//             Serial.println("Christmas lights mode.");
//             new_brightness = getValueFromData(data, "BR");
//             Serial.println("Brightness: " + new_brightness);
//             if (new_brightness != ERR_VAL){
//                 brightness = new_brightness.toInt();
//             }
//             mode = CHRISTMAS;
//             break;

//         default:
//             Serial.println("Invalid data: " + data);
//             break;
//      }
// }

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

String getValueFromData(String data, String key){
    unsigned int starting_index = data.indexOf(key) + key.length() + 1;
    unsigned int ending_index = starting_index;
    // Serial.println("Data: " + data + " | starting index: " + starting_index);
    for (; ending_index < data.length(); ending_index++){
        if (data[ending_index] == ';' || data[ending_index] == '>'){
            // Serial.println("Ending index found: " + ending_index);
            return data.substring(starting_index, ending_index);
        }
    }
    return ERR_VAL;
}