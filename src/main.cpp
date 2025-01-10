#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN1 15
#define LED_PIN2 16
#define LED_COUNT 12

Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

BLEServer* pServer = NULL;
BLECharacteristic* pLedControlCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// UUIDs for BLE Service and Characteristics
#define SERVICE_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define LED_CONTROL_CHARACTERISTIC_UUID "19b10002-e8f2-537e-4f6c-d104768a1214"

// Global variables for LED control
bool isOn = false;
uint32_t currentColor = strip1.Color(255, 255, 255); // Default color: white

void updateLEDs() {
  if (isOn) {
    for (int i = 0; i < LED_COUNT; i++) {
      strip1.setPixelColor(i, currentColor);
      strip2.setPixelColor(i, currentColor);
    }
    strip1.show();
    strip2.show();
  } else {
    strip1.clear();
    strip2.clear();
    strip1.show();
    strip2.show();
  }
}

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string rawValue = pCharacteristic->getValue();
    if (rawValue.length() > 0) {
      char command = rawValue[0];
      if (command == '0') { // Turn off
        isOn = false;
        updateLEDs();
        Serial.println("LEDs turned off");
      } else if (command == '1') { // Turn on
        isOn = true;
        updateLEDs();
        Serial.println("LEDs turned on");
      } else if (command == 'C') { // Change color
        if (rawValue.length() == 4) { // Expect 'C' + R + G + B
          uint8_t r = rawValue[1];
          uint8_t g = rawValue[2];
          uint8_t b = rawValue[3];
          currentColor = strip1.Color(r, g, b);
          updateLEDs();
          Serial.printf("Color changed to R:%d G:%d B:%d\n", r, g, b);
        }
      }
    }
  }
};

void setup() {
  Serial.begin(115200);

  // Initialize LED strips
  strip1.begin();
  strip1.show();
  strip1.setBrightness(50);

  strip2.begin();
  strip2.show();
  strip2.setBrightness(50);

  // Create the BLE Device
  BLEDevice::init("ESP32 Lamp");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create the LED Control Characteristic
  pLedControlCharacteristic = pService->createCharacteristic(
    LED_CONTROL_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  pLedControlCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pLedControlCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection...");
}

void loop() {
  // Handle connection state changes
  if (!deviceConnected && oldDeviceConnected) {
    Serial.println("Device disconnected.");
    delay(500);
    pServer->startAdvertising();
    Serial.println("Restart advertising.");
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    Serial.println("Device connected.");
    oldDeviceConnected = deviceConnected;
  }
}
