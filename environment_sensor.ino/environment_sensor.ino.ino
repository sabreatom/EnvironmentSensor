#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>

//constants:
#define SENSOR_UPDATE_PERIOD_MS           1000 //1 second

//GATT realeted:
BLEService EnvironmentSensorService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
BLEFloatCharacteristic TemperatureCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead);
BLEFloatCharacteristic HumidityCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Environment sensor");
  BLE.setDeviceName("Environment sensor");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(EnvironmentSensorService);

  // add the characteristic to the service
  EnvironmentSensorService.addCharacteristic(TemperatureCharacteristic);
  EnvironmentSensorService.addCharacteristic(HumidityCharacteristic);

  // add service
  BLE.addService(EnvironmentSensorService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  // poll for BLE events
  BLE.poll();

  unsigned long _sensors_last_update = 0;
  unsigned long _uptime = millis();

  if ((_uptime - _sensors_last_update) >= SENSOR_UPDATE_PERIOD_MS) {
    //store update time:
    _sensors_last_update = _uptime;
    
    // read all the sensor values
    float temperature = HTS.readTemperature();
    float humidity    = HTS.readHumidity();
  
    // print each of the sensor values
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");
  
    Serial.print("Humidity    = ");
    Serial.print(humidity);
    Serial.println(" %");
  
    // print an empty line
    Serial.println();

    //update GATT table:
    TemperatureCharacteristic.writeValue(temperature);
    HumidityCharacteristic.writeValue(humidity);
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}
