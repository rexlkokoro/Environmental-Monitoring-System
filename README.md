**ESP32 EcoStation: Real-Time Environmental & Location Monitor**

A portable, IoT-enabled environmental monitoring system built with the **ESP32**. This project integrates air quality, climate, and GPS data into a mobile-responsive web dashboard served directly from the microcontroller via a **Captive Portal**.

##  Features
*   **Live Web Dashboard:** A clean, modern UI to monitor sensors in real-time.
*   **Captive Portal:** Automatically redirects users to the dashboard upon connecting to the ESP32’s WiFi—no IP typing required.
*   **Multi-Sensor Integration:**
    *   **Air Quality:** Real-time gas concentration monitoring (CO2, Ammonia, Benzene) using the **MQ135**.
    *   **Climate Tracking:** High-accuracy temperature and humidity data via **DHT11**.
    *   **Geolocation:** Precise GPS coordinates using the **Neo-6M** module.
*   **Google Maps Integration:** One-click button to view the device's exact location on Google Maps.

##  Hardware Components
*   **Microcontroller:** ESP32 (NodeMCU)
*   **Gas Sensor:** MQ135 (Air Quality)
*   **Climate Sensor:** DHT11 (Temp/Humidity)
*   **GPS Module:** Neo-6M
*   **Power:** 5V via USB (utilizing VIN for sensor heaters)

## Connection Mapping

| Component | ESP32 Pin | Logic |
| :--- | :--- | :--- |
| **DHT11** | GPIO 4 | Digital |
| **MQ135** | GPIO 34 | Analog (ADC1) |
| **Neo-6M TX** | GPIO 16 | UART (RX2) |
| **Neo-6M RX** | GPIO 17 | UART (TX2) |

##  Software Stack
*   **Framework:** Arduino Core for ESP32
*   **Libraries:** `WiFi.h`, `DNSServer.h`, `WebServer.h`, `DHT.h`, `TinyGPS++.h`
*   **Frontend:** HTML5, CSS3 (embedded directly in C++ strings for offline use)

##  Setup & Installation
1.  Clone the Repo: 
    ```bash
    git clone [https://github.com/yourusername/ESP32-EcoStation.git](https://github.com/yourusername/ESP32-EcoStation.git)
    ```
2.  **Install Libraries:** Use the Arduino Library Manager to install `DHT Sensor Library` and `TinyGPS++`.
3.  **Flash the Code:** Upload the provided `.ino` file to your ESP32.
4.  **Connect:** Look for the WiFi network **"ESP32_Environment_Monitor"** on your phone or PC.

---

### Author
**Manas Kumar Singh**  
*B.Tech Computer Science and Engineering*
<img width="716" height="1600" alt="image" src="https://github.com/user-attachments/assets/6e36d46f-9d72-4218-847d-9dc335644fd7" />
<img width="1600" height="1524" alt="image" src="https://github.com/user-attachments/assets/a05cc4d3-df64-4eca-b318-98e5a07d3812" />

