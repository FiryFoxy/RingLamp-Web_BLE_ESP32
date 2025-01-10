#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Adafruit_NeoPixel.h>

BLEServer* pServer = NULL;
BLECharacteristic* pLedCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define LED_CHARACTERISTIC_UUID "19b10002-e8f2-537e-4f6c-d104768a1214"

#define LED_PIN1 15
#define LED_PIN2 16
#define LED_COUNT 12
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

bool isOn = false;
uint32_t currentColor1 = strip1.Color(255, 255, 255);
uint32_t currentColor2 = strip2.Color(255, 255, 255);

void updateLEDs() {
  Serial.println(isOn ? "Updating LEDs: ON" : "Updating LEDs: OFF");
  if (isOn) {
    for (int i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, currentColor1);
    }
    for (int i = 0; i < strip2.numPixels(); i++) {
    strip2.setPixelColor(i, currentColor2);
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

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Client connected");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Client disconnected");
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string rawValue = pCharacteristic->getValue();
    if (rawValue.length() > 0) {
      uint8_t command = rawValue[0];
      if (command == 0) { // Turn off
        isOn = false;
        updateLEDs();
        Serial.println("LEDs turned off");
      } else if (command == 1) { // Turn on
        isOn = true;
        updateLEDs();
        Serial.println("LEDs turned on");
      } else if (command == 'C') { // Change color
        if (rawValue.length() == 4) { // Expect 'C' + R + G + B
          uint8_t r = rawValue[1];
          uint8_t g = rawValue[2];
          uint8_t b = rawValue[3];
          currentColor1 = strip1.Color(r, g, b);
          currentColor2 = strip2.Color(r, g, b);
          updateLEDs();
          Serial.printf("Color changed to R:%d G:%d B:%d\n", r, g, b);
        }
      }
    }
  }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing BLE...");

    strip1.begin();
    strip1.show();
    strip1.setBrightness(50);

    strip2.begin();
    strip2.show();
    strip2.setBrightness(50);

    BLEDevice::init("ESP32-Lamp");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService* pService = pServer->createService(SERVICE_UUID);

    pLedCharacteristic = pService->createCharacteristic(
        LED_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );

    pLedCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    pLedCharacteristic->addDescriptor(new BLE2902());

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE initialized. Waiting for connections...");
}

void loop() {
    if (deviceConnected && !oldDeviceConnected) {
        Serial.println("Device connected for the first time");
        oldDeviceConnected = deviceConnected;
    } else if (!deviceConnected && oldDeviceConnected) {
        Serial.println("Device disconnected, restarting advertising");
        oldDeviceConnected = deviceConnected;
        delay(500);
        pServer->startAdvertising();
    }
}
