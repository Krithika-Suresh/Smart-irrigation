// INCLUDE NECESSARY LIBRARIES

#include <HTTPClient.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Define a StaticJsonDocument to hold sensor data
StaticJsonDocument<500> doc;

// DEFINING CONFIGURED PINS AND AND MODBUS REGISTERS
#define RS485_TX_PIN 17
#define RS485_RX_PIN 16

#define NPK_SENSOR_SLAVE_ID 1
#define REGISTER_NITROGEN_VALUE 0x0000
#define REGISTER_POTASSIUM_VALUE 0x0001
#define REGISTER_PHOSPHORUS_VALUE 0x0002

ModbusMaster node;

const char* ssid = "********";
const char* password = "************";
const char* serverName = "https://webhooks.mongodb-realm.com/api/client/v2.0/app/XXXXX/service/iotService/incoming_webhook/iotWebhook?secret=XXXXX";




void setup() {
  //INITIALIZE SERIAL COMMUNICATION
  Serial.begin(9600);
  // RS485 communication setup
  //CONFIGURE RS485 COMMUNICATION
  Serial1.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  //INITIALIZE MODBUS NODE WITH SLAVE ID AND SERIAL1
  node.begin(NPK_SENSOR_SLAVE_ID, Serial1);

  // CONNECT TO WIFI NETWORK
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t result;
  uint16_t nitrogenValue, potassiumValue, phosphorusValue;

  // READ NITROGEN VALUE FROM NPK SENSOR
  result = node.readHoldingRegisters(REGISTER_NITROGEN_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    nitrogenValue = node.getResponseBuffer(0);
    Serial.print("Nitrogen Value: ");
    Serial.println(nitrogenValue);
    doc["NPK"]["Nitrogen"] = nitrogenValue;
  } else {
    Serial.println("Failed to read nitrogen value.");
  }

  // READ POTASSIUM VALUE FROM NPK SENSOR
  result = node.readHoldingRegisters(REGISTER_POTASSIUM_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    potassiumValue = node.getResponseBuffer(0);
    Serial.print("Potassium Value: ");
    Serial.println(potassiumValue);
    doc["NPK"]["Potassium"] = potassiumValue;
  } else {
    Serial.println("Failed to read potassium value.");
  }

  // READ PHOSPHORUS VALUE FROM NPK SENSOR
  result = node.readHoldingRegisters(REGISTER_PHOSPHORUS_VALUE, 1);
  if (result == node.ku8MBSuccess) {
    phosphorusValue = node.getResponseBuffer(0);
    Serial.print("Phosphorus Value: ");
    Serial.println(phosphorusValue);
    doc["NPK"]["Phosporous"] = phosphorusValue;
  } else {
    Serial.println("Failed to read phosphorus value.");
  }

  delay(10000); // Adjust delay as needed

  // POST SENSOR DATA TO SERVER
  Serial.println("Posting...");
  POSTData();
  serializeJsonPretty(doc, Serial);
  Serial.println("\nDone.");

}

// FUNCTION TO POST SENSOR DATA TO SERVER
void POSTData()
{
      
      if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // SPECIFY SERVER URL AND HEADERS
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      // SERIALIZE JSON DOCUMENT TO STRING
      String json;
      serializeJson(doc, json);

      // PRINT JSON DATA TO SERIAL MONITOR
      Serial.println(json);

      // SEND HTTP POST REQUEST WITH JSON DATA
      int httpResponseCode = http.POST(json);
      Serial.println(httpResponseCode);

      // End HTTP connection
      http.end();
      }
}
