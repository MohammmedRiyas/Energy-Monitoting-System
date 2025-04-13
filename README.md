# **Energy Monitoring System - IoT-Based Industrial Monitoring**

🚀 **Energy Monitoring System** is a reliable and scalable solution for real-time monitoring of energy consumption in industrial environments. It utilizes **Modbus RTU (RS-485)** for data acquisition, and transmits critical parameters such as voltage, current, power, and energy usage to a cloud server via **Wi-Fi or Ethernet** for remote monitoring and analysis.

## **📌 Features**
✅ **Modbus RTU (RS-485)** for real-time data collection  
✅ **Wi-Fi & Ethernet support** for cloud connectivity  
✅ **Live dashboard** for remote monitoring  
✅ **Real-time fault detection and diagnostics**  
✅ **Scalable solution** supporting up to 45 energy meters  
✅ **Secure and reliable industrial-grade solution**

---

## **📡 System Overview**
The **Energy Monitoring System** architecture consists of:

1️⃣ **ESP32 Microcontroller** – Handles data acquisition & communication  
2️⃣ **Modbus RTU (RS-485)** – Reads energy parameters from meters  
3️⃣ **Ethernet & Wi-Fi Modules** – Provides internet connectivity  
4️⃣ **Cloud API Server** – Stores & visualizes real-time data  

### **🛠 How It Works**
📊 **Step 1:** **Data Collection** – ESP32 reads energy data using Modbus RTU protocol  
🔄 **Step 2:** **Data Processing** – Formats data for transmission to the cloud  
🌐 **Step 3:** **Data Transmission** – Sends data to the cloud via HTTP GET request  
📈 **Step 4:** **Cloud Visualization** – Logs data for real-time monitoring on a cloud dashboard  

---

## **⚙️ Technologies Used**
### **1️⃣ Modbus RTU (RS-485) Communication**
- **Purpose:** Fetches real-time energy data from energy meters  
- **Why RS-485?**
  ✔ Long-distance communication  
  ✔ Supports multiple devices on a single bus  
  ✔ Reliable and noise-resistant

### **2️⃣ Ethernet Communication (W5500 Module)**
- **Purpose:** Provides stable and high-speed cloud connectivity  
- **Advantage:** More reliable than Wi-Fi in industrial environments

### **3️⃣ Wi-Fi Communication (ESP32 Built-in Wi-Fi)**
- **Fallback option** when Ethernet is unavailable  
- **Security:** WPA2-PSK encryption for secure connectivity

### **4️⃣ HTTP API for Cloud Communication**
- **Data is sent to a REST API endpoint** via an HTTP GET request  
- **Example API Format:**
  ```
  https://serverurl?data=P101,parameter1,parameter2,...
  ```

---

## **📊 Data Acquisition via Modbus**
The **ESP32** reads the following **energy parameters** using **Modbus RTU**:

| **Parameter**              | **Modbus Address** |
|---------------------------|--------------------|
| Power Factor Average       | 43907              |
| Line-to-Line Voltage Avg  | 43909              |
| Line-to-Neutral Voltage Avg| 43911              |
| Energy (Wh)                | 43961              |
| Current Average            | 43913              |
| Watts R Phase              | 40103              |
| Watts Y Phase              | 40105              |
| Watts B Phase              | 40107              |
| Frequency (Hz)             | 43110              |

### **📟 Modbus Read Code Example (ESP32 + ModbusMaster Library)**
```cpp
float modbus_read(int addr, int sm) {
    delay(100);
    int result = node.readHoldingRegisters(addr, sm);
    uint data;
    float flt = 0;
    
    if (result == node.ku8MBSuccess) {
        data = node.getResponseBuffer(0);
        Serial.print(id);
        Serial.print(": ");
        Serial.println("response: " + String(data));
    } else {
        Serial.print(id);
        Serial.print(": Failed, Response Code: ");
        Serial.println(result, HEX);
    }
    return flt;
}
```

---

## **📡 Data Transmission to Cloud (HTTP GET Request)**
### **1️⃣ Formatting Data for Transmission**
The retrieved data is formatted as a comma-separated string before sending:
```cpp
data = String(PFAverage) + "," + String(VLLAverage) + "," + 
       String(VLNAverage) + "," + String(wh) + "," + 
       String(current) + "," + String(wattsrphase) + "," + 
       String(wattsyphase) + "," + String(wattsbphase) + "," + 
       String(frequency);
```

### **2️⃣ Sending Data to Cloud API**
```cpp
bool push_data() {
    String lnk = endpoint + data;
    HTTPClient http;
    http.begin(lnk);
    int httpCode = http.GET();

    if (httpCode > 0) {
        String res = http.getString();
        Serial.println(res);
        return true;
    } else {
        Serial.print("GET err: ");
        Serial.println(httpCode);
        return false;
    }
}
```

---

## **🌍 Network Configuration**
### **📶 Wi-Fi Setup (ESP32)**
```cpp
WiFi.begin(SSID, PASS);
while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
}
Serial.println("Connected to Wi-Fi");
```

### **🌐 Ethernet Setup (W5500)**
```cpp
Ethernet.begin(mac, ip);
if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield not found.");
}
```

---

## **🚨 Error Handling & Fault Detection**
### **1️⃣ Modbus Communication Failure**
If ESP32 fails to read Modbus registers, it logs an error:
```cpp
Serial.print(id);
Serial.print(": Failed, Response Code: ");
Serial.println(result, HEX);
```

### **2️⃣ HTTP GET Request Failure**
If the API request fails, it prints an error message:
```cpp
Serial.print("GET err: ");
Serial.println(httpCode);
```

---

## **📌 Summary & Conclusion**
The **Energy Monitoring System** is an **IoT-based industrial monitoring system** designed to:

✅ **Continuously monitor energy parameters** like voltage, current, and power  
✅ **Detect faults and improve operational efficiency**  
✅ **Enable remote diagnostics & predictive maintenance**  

This system ensures **improved efficiency, reduced downtime, and enhanced reliability** in industrial environments. 🚀

💡 **Contributions & Feedback are Welcome!**

---

📧 **Contact:** mdriyas1607@gmail.com  

---
