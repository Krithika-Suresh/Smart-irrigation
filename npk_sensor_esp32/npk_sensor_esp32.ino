#include <ModbusMaster.h>

#define RS485_TX_PIN 17
#define RS485_RX_PIN 16

#define NPK_SENSOR_SLAVE_ID 1
#define REGISTER_NITROGEN_VALUE 0x0000
#define REGISTER_POTASSIUM_VALUE 0x0001
#define REGISTER_PHOSPHORUS_VALUE 0x0002

ModbusMaster node;

void setup() {
  Serial.begin(9600);
  // RS485 communication setup
  Serial1.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  node.begin(NPK_SENSOR_SLAVE_ID, Serial1);
}

void loop() {
  uint8_t result;
  uint16_t nitrogenValue, potassiumValue, phosphorusValue;

  // Read nitrogen value from NPK sensor
  result = node.readHoldingRegisters(REGISTER_NITROGEN_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    nitrogenValue = node.getResponseBuffer(0);
    Serial.print("Nitrogen Value: ");
    Serial.println(nitrogenValue);
  } else {
    Serial.println("Failed to read nitrogen value.");
  }

  // Read potassium value from NPK sensor
  result = node.readHoldingRegisters(REGISTER_POTASSIUM_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    potassiumValue = node.getResponseBuffer(0);
    Serial.print("Potassium Value: ");
    Serial.println(potassiumValue);
  } else {
    Serial.println("Failed to read potassium value.");
  }

  // Read phosphorus value from NPK sensor
  result = node.readHoldingRegisters(REGISTER_PHOSPHORUS_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    phosphorusValue = node.getResponseBuffer(0);
    Serial.print("Phosphorus Value: ");
    Serial.println(phosphorusValue);
  } else {
    Serial.println("Failed to read phosphorus value.");
  }

  delay(1000); // Adjust delay as needed
}
