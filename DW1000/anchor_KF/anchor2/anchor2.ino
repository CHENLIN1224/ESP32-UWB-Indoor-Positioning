#include <SPI.h>
#include "DW1000Ranging.h"

#define ANCHOR_ADD "83:17:5B:D5:A9:9A:E2:9C"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin

// // Antenna delay calibration parameters
// float antennaDelay = 16600; 

// // Kalman filter dependent variable
// float Q = 0.01; // Process noise covariance
// float R = 0.1;   // Measure noise covariance
// float P = 1;   // Estimated error covariance
// float X = 0;   // State variable (estimated distance)
// float K = 0;   // Kalman gain

void setup()
{
    // Set antenna delay calibration
    // DW1000.setAntennaDelay(antennaDelay);

    Serial.begin(115200);
    delay(1000);
    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
    //define the sketch as anchor. It will be great to dynamically change the type of module
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachBlinkDevice(newBlink);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    //Enable the filter to smooth the distance
    //DW1000Ranging.useRangeFilter(true);

    //start the module as an anchor
    DW1000Ranging.startAsAnchor(ANCHOR_ADD, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);

    // Kalman filter initialization
    // X = 0; 
    // P = 1; 
}

void loop()
{
    DW1000Ranging.loop();
}

void newRange()
{
    // float range = DW1000Ranging.getDistantDevice()->getRange();

    // // Kalman filter update
    // P = P + Q; 
    // K = P / (P + R); 
    // X = X + K * (range - X); 
    // P = (1 - K) * P; 

    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    // Serial.print(X); // The filtered distance of the output
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.println(" dBm");
}

void newBlink(DW1000Device *device)
{
    Serial.print("blink; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
}
