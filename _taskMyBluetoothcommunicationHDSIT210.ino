#include <Wire.h>
#include <BH1750.h>
#include <ArduinoBLE.h>

BLEService proximityService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Define the BLE service
BLEByteCharacteristic proximityChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);  // Define the BLE characteristic

BH1750 lightMeter;  

void setup() {
  Serial.begin(9600); 
  
  Serial.println("Initializing BLE...");
  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);  
  }

  Serial.println("BLE initialization successful.");

  BLE.setLocalName("LightSensor");                     
  BLE.setAdvertisedService(proximityService);          
  proximityService.addCharacteristic(proximityChar);  
  BLE.addService(proximityService);                    
  BLE.advertise();                                     

  Serial.println("BLE advertising started");

  // Initialize the BH1750 light sensor
  Wire.begin();
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 not found!");
    while (1);  
  }

  Serial.println("BH1750 initialized.");
}

void loop() {
  BLEDevice central = BLE.central();  // Check if a device connects

  if (central) {
    Serial.println("Connected to central");

    // Read the light level from the BH1750 sensor
    uint16_t lux = lightMeter.readLightLevel();

    if (lux == 0) {
      Serial.println("Error reading light level");
    } else {
      Serial.print("Light level: ");
      Serial.print(lux);
      Serial.println(" lux");

      
      proximityChar.writeValue(lux);
    }

    delay(1000);  
  } else {
    
    Serial.println("Waiting for central device to connect...");
    delay(1000);
  }
}
