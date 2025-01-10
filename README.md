# ESP32 Ring Lamp with BLE Server

This project implements a **Ring Lamp** controlled via Bluetooth Low Energy (BLE) using an **ESP32** microcontroller. The lamp can be controlled remotely through a mobile device or computer that supports BLE.

## Features

- **BLE Server:** The ESP32 acts as a BLE server to manage communication with BLE-enabled devices.
- **Color Control:** Change the ring lamp's color remotely via BLE.
- **Brightness Control:** Adjust the brightness of the ring lamp.
- **Power Control:** Toggle the lamp on and off.

## Hardware Requirements

- **ESP32 Development Board** (e.g., ESP32 DevKitC)
- **LED Ring** (Addressable LEDs like WS2812 or similar)
- **Power Supply** for the LED ring
- **Jump wires** for connections

## Software Requirements

- **Arduino IDE** (or PlatformIO)
- **ESP32 Board Support** (install via the Arduino Board Manager)
- **Adafruit NeoPixel Library** (or any other compatible library for addressable LEDs)
- **BLE Library** (Built-in ESP32 BLE libraries)

## Installation

### 1. Install Arduino IDE

If you haven't installed Arduino IDE, download and install it from [here](https://www.arduino.cc/en/software).

### 2. Install ESP32 Board

1. Open Arduino IDE.
2. Go to **File > Preferences**.
3. In the **Additional Boards Manager URLs** field, add the following URL:  
   `https://dl.espressif.com/dl/package_esp32_index.json`
4. Go to **Tools > Board > Board Manager**.
5. Search for `ESP32` and install the board package.

### 3. Install Libraries

- Open the Arduino IDE and go to **Sketch > Include Library > Manage Libraries**.
- Install **Adafruit NeoPixel** and **ESP32 BLE Arduino** libraries.

### 4. Upload the Code

1. Connect your ESP32 to your computer using a USB cable.
2. Open the `ESP32_Ring_Lamp_BLE_Server.ino` file in Arduino IDE.
3. Select your ESP32 board under **Tools > Board**.
4. Select the correct port under **Tools > Port**.
5. Click **Upload** to flash the code to the ESP32.

### 5. Wiring the Hardware

- **ESP32 GPIO Pins** should be connected to the data input pin of your LED ring.
- Ensure your **LED ring** is powered using a suitable power supply (e.g., 5V).
- Make connections for **Ground** between the ESP32, LED ring, and power supply.

## How It Works

- The ESP32 initializes as a BLE server.
- It creates characteristics to control the **color**, **brightness**, and **power** of the ring lamp.
- A mobile app or BLE-enabled device can connect to the ESP32 and send commands to control the lamp.

## Example Mobile App

You can use any BLE scanner app to test the connection, but for full functionality, you can create your own app using platforms like **React Native**, **Flutter**, or native Android/iOS development, which connects to the ESP32 and sends commands to control the lamp's settings.

## Code Explanation

- The ESP32 sets up BLE services and characteristics for **color**, **brightness**, and **power control**.
- The `Adafruit_NeoPixel` library is used to control the LED ring.
- BLE write commands are mapped to actions such as changing color and brightness.

### BLE Characteristics

1. **Color Control:** Control the color of the LEDs via RGB values (e.g., red, green, blue).
2. **Brightness Control:** Set the brightness of the LEDs (0-255).
3. **Power Control:** Turn the lamp on or off.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgements

- ESP32: [Espressif Systems](https://www.espressif.com/)
- Adafruit NeoPixel: [Adafruit Industries](https://www.adafruit.com/)
- BLE Libraries: [ESP32 BLE Arduino](https://github.com/nkolban/ESP32_BLE_Arduino)
