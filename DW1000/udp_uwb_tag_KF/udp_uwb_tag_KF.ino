#include <SPI.h>
#include <DW1000Ranging.h>
#include <WiFi.h>
#include "link.h"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define PIN_RST 27
#define PIN_IRQ 34

#define TAG_ADD "7D:00:22:EA:82:60:3B:9C"

const char *ssid = "AAAAA";
const char *password = "19820119";
// const char *host = "172.20.10.7";
const char *host = "172.20.10.4";
WiFiClient client;

struct MyLink *uwb_data;
int index_num = 0;
long runtime = 0;
String all_json = "";

// // Antenna delay calibration parameters
// float antennaDelay = 16600; 

// // Kalman filter dependent variable
// float Q = 0.01; 
// float R = 0.1;   
// float P = 1;   
// float X = 0;  
// float K = 0;  

void setup()
{
    // DW1000.setAntennaDelay(antennaDelay);
    
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());

    if (client.connect(host, 8085))
    {
        Serial.println("Success");
        client.print(String("GET /") + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n" +
                     "\r\n");
    }

    delay(1000);

    // Kalman filter initialization
    // X = 0; 
    // P = 1;

    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, DW_CS, PIN_IRQ);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);

    //start the module as a tag
    DW1000Ranging.startAsTag(TAG_ADD, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);

    uwb_data = init_link();
}

void loop()
{
    DW1000Ranging.loop();
    if ((millis() - runtime) > 1000)
    {
        make_link_json(uwb_data, &all_json);
        send_udp(&all_json);
        runtime = millis();
    }
}

void newRange()
{
    char c[30];

    // float range = DW1000Ranging.getDistantDevice()->getRange();

    // Kalman filter update
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
    fresh_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
}

void newDevice(DW1000Device *device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);

    add_link(uwb_data, device->getShortAddress());
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);

    delete_link(uwb_data, device->getShortAddress());
}

void send_udp(String *msg_json)
{
    if (client.connected())
    {
        client.print(*msg_json);
        Serial.println("UDP send");
    }
}
