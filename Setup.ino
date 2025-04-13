#include <WiFi.h>
#include <HTTPClient.h>

#include "mb_getdata.h"
#include "push_data_int.h"

// Wi-Fi credentials
String SSID = "test";
String PASS = "12345678";

// Device initialization with unique ID and API endpoint
// SERVER_URL should be defined in your code or header file, e.g.:
// #define SERVER_URL 

modb dev1(1, SERVER_URL + "101,");
modb dev2(2, SERVER_URL + "102,");
modb dev3(3, SERVER_URL + "103,");
modb dev4(4, SERVER_URL + "104,");
modb dev5(5, SERVER_URL + "105,");
modb dev6(6, SERVER_URL + "106,");
modb dev7(7, SERVER_URL + "107,");
modb dev8(8, SERVER_URL + "108,");
modb dev9(9, SERVER_URL + "109,");
modb dev10(10, SERVER_URL + "110,");
modb dev11(11, SERVER_URL + "111,");
modb dev12(12, SERVER_URL + "112,");
modb dev13(13, SERVER_URL + "113,");
modb dev14(14, SERVER_URL + "114,");
modb dev15(15, SERVER_URL + "115,");
modb dev16(16, SERVER_URL + "116,");
modb dev17(17, SERVER_URL + "117,");
modb dev18(18, SERVER_URL + "118,");
modb dev19(19, SERVER_URL + "119,");
modb dev20(20, SERVER_URL + "120,");
modb dev21(21, SERVER_URL + "121,");
modb dev22(22, SERVER_URL + "122,");
modb dev23(23, SERVER_URL + "123,");
modb dev24(24, SERVER_URL + "124,");
modb dev25(25, SERVER_URL + "125,");
modb dev26(26, SERVER_URL + "126,");
modb dev27(27, SERVER_URL + "127,");
modb dev28(28, SERVER_URL + "128,");
modb dev29(29, SERVER_URL + "129,");
modb dev30(30, SERVER_URL + "130,");
modb dev31(31, SERVER_URL + "131,");
modb dev32(32, SERVER_URL + "132,");
modb dev33(33, SERVER_URL + "133,");
modb dev34(34, SERVER_URL + "134,");
modb dev35(35, SERVER_URL + "135,");
modb dev36(36, SERVER_URL + "136,");
modb dev37(37, SERVER_URL + "137,");
modb dev38(38, SERVER_URL + "138,");
modb dev39(39, SERVER_URL + "139,");
modb dev40(40, SERVER_URL + "140,");
modb dev41(41, SERVER_URL + "141,");
modb dev42(42, SERVER_URL + "142,");
modb dev43(43, SERVER_URL + "143,");
modb dev44(44, SERVER_URL + "144,");
modb dev45(45, SERVER_URL + "145,");

// Store all devices in an array for easy looping
modb devices[] = {
  dev1, dev2, dev3, dev4, dev5,
  dev6, dev7, dev8, dev9, dev10,
  dev11, dev12, dev13, dev14, dev15,
  dev16, dev17, dev18, dev19, dev20,
  dev21, dev22, dev23, dev24, dev25,
  dev26, dev27, dev28, dev29, dev30,
  dev31, dev32, dev33, dev34, dev35,
  dev36, dev37, dev38, dev39, dev40,
  dev41, dev42, dev43, dev44, dev45
};

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi network
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Iterate over each device to read and push data
  for (int i = 0; i < 45; i++) {

    // Read Modbus data from energy meter
    devices[i].load_data();

    // Reconnect if WiFi connection drops during loop
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Reconnecting to WiFi...");
      WiFi.begin(SSID, PASS);
    }
    Serial.println("WiFi Connected");

    // Push data to the server
    devices[i].push_data();

    delay(200); // Small delay between devices to avoid flooding the network/server
  }
}
