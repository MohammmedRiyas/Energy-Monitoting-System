#include <ModbusMaster.h>  // Include ModbusMaster library for Modbus communication

// Define RS-485 control pins
#define MAX485_DE 5
#define MAX485_RE_NEG 4

// Define Serial1 RX and TX pins
#define RX_PIN 16
#define TX_PIN 17

bool flag = 0;  // Flag to ensure one-time initialization of Serial and RS485 pins
const int X = 40001;  // Modbus address offset

// Initialize Serial1 and RS485 transceiver
void init_whole() {
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Begin Serial1 with RX/TX pins
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);  // Set transceiver to receive mode
  digitalWrite(MAX485_DE, 0);
  flag = 1;  // Set flag to prevent re-initialization
}

// Modbus device class
class modb {
public:
  // Constructor: takes device ID and server endpoint URL
  modb(int id_, String endpoint_) {
    id = id_;
    endpoint = endpoint_;
    if (!flag) {
      init_whole();  // Initialize only once
    }
    node.begin(id, Serial1);  // Start Modbus communication with given ID
    node.preTransmission(preTransmission);  // Setup pre-transmission hook
    node.postTransmission(postTransmission);  // Setup post-transmission hook
  }

  // Read Modbus registers and format the data string
  void load_data() {
    float PFAverage = modbus_read((43907 - X), 2);       // Power factor average
    float VLLAverage = modbus_read((43909 - X), 2);      // Line-to-line voltage average
    float VLNAverage = modbus_read((43911 - X), 2);      // Line-to-neutral voltage average
    double wh = modbus_read((43961 - X), 2);             // Energy in watt-hours
    float current = modbus_read((43913 - X), 2);         // Current average
    float wattsrphase = modbus_read((40103 - X), 2);     // Watts R phase
    float wattsyphase = modbus_read((40105 - X), 2);     // Watts Y phase
    float wattsbphase = modbus_read((40107 - X), 2);     // Watts B phase
    float frequency = modbus_read((43110 - X), 2);       // Frequency
    // Combine all values into a comma-separated string
    data = String(current) + "," + String(PFAverage) + "," + String(VLLAverage) + "," + String(VLNAverage) + "," + String(wh) + "," + String(wattsrphase) + "," + String(wattsyphase) + "," + String(wattsbphase)+ "," + String(frequency);
  }

  // Push data to cloud using HTTP GET
  bool push_data() {
    // Ensure WiFi is connected before calling this method
    String lnk = endpoint + data;  // Full URL to push data
    HTTPClient http;
    http.begin(lnk);  // Start HTTP connection
    int httpCode = http.GET();  // Send GET request

    if (httpCode > 0) {
      String res = http.getString();  // Read response
      Serial.println(res);  // Print response
      return true;
    } else {
      // Error handling
      Serial.print("GET err: ");
      Serial.println(httpCode);
      return false;
    }
  }

private:
  ModbusMaster node;      // Instance of ModbusMaster
  int id;                 // Modbus ID of the device
  String data = "";       // String to hold the collected data
  String endpoint = "";   // URL endpoint to push data

  // Modbus read helper function
  float modbus_read(int addr, int sm) {
    int result = node.readHoldingRegisters(addr, sm);  // Read Modbus registers
    int data[10];  // Temporary storage
    float flt = 0;

    if (result == node.ku8MBSuccess) {
      // If successful, read response buffer
      for (int j = 0; j < 2; j++) {
        data[j] = node.getResponseBuffer(j);
      }

      // Convert 2-register response to float
      unsigned long temp = (unsigned long)data[0] + (unsigned long)data[1] * 65536;
      flt = *((float*)&temp);  // Interpret as float using pointer casting

      Serial.print(id);
      Serial.print(": ");
      Serial.println("response: " + String(flt));
      delay(100);  // Small delay for reliability
    } else {
      // Print error code on failure
      Serial.print(id);
      Serial.print(": ");
      Serial.print("Failed, Response Code: ");
      Serial.print(result, HEX);
      Serial.println(" ");
      delay(100);
    }

    return flt;  // Return the float value
  }

  // RS-485: Set transceiver to transmit mode
  static void preTransmission() {
    digitalWrite(MAX485_RE_NEG, 1);
    digitalWrite(MAX485_DE, 1);
  }

  // RS-485: Set transceiver back to receive mode
  static void postTransmission() {
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);
  }
};
