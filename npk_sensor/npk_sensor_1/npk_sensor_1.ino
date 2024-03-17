//===================================================================================//

#include <Arduino.h>
#include <CSE_ArduinoRS485.h>

//===================================================================================//

#define PIN_RS485_RX        16
#define PIN_RS485_TX        17

#define PORT_USB            Serial
#define PORT_RS485          Serial2

//===================================================================================//
// Globals

// Create a new RS-485 port with (serial port, DE pin, RE pin, TX pin)
RS485Class RS485 (PORT_RS485, -1, -1, PIN_RS485_TX);
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
byte values[11];
//===================================================================================//
// Forward declaration

void setup();
void loop();
void readRS485();

//===================================================================================//

void setup() {
  // Initialize default serial port
  PORT_USB.begin (115200);
  delay (1000);

  // Initialize the RS485 port manually
  PORT_RS485.begin (9600, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);

  // Initialize the RS485 object.
  RS485.begin();
  delay (1000);

  PORT_USB.println ("--- RS485 Test ---");
  delay (1000);
}

//===================================================================================//

void loop() {
  // readRS485();

  // Get data from the USB port and send it to the RS485 port.
  if (PORT_USB.available()) {
    PORT_USB.print ("TX: ");
    String msg = PORT_USB.readStringUntil ('\n');
    PORT_USB.print (msg);
    // PORT_USB.print(nitro);
    // RS485.print (msg);
  }
  RS485.write(nitro, sizeof(nitro)==8);
  delay (5);
  readRS485();

}

//===================================================================================//
/**
 * @brief Read data from the RS485 port and print it to the USB port.
 * 
 */
void readRS485() {
  if (RS485.available()) {
    // Read incoming message from the RS-485 port
    String msg = RS485.readString();

    // Print the message as a string
    PORT_USB.print ("RX String: ");
    PORT_USB.println (msg);
    
    // Print the message as decimal values
    PORT_USB.print ("RX Dec: ");
    for (int i = 0; i < msg.length(); i++) {
      PORT_USB.print (msg [i], DEC);
      PORT_USB.print (" ");
    }
    PORT_USB.println();

    // Print the message as hex values
    PORT_USB.print ("RX Hex: ");
    for (int i = 0; i < msg.length(); i++) {
      if (msg [i] < 0x10) PORT_USB.print ("0");
      PORT_USB.print (msg [i], HEX);
      PORT_USB.print (" ");
    }
    PORT_USB.println();
    PORT_USB.println();
  }
}

//===================================================================================//