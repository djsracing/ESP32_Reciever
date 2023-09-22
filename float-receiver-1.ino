#include <CAN.h>
#define TX_GPIO_NUM 21 // Connects to CTX
#define RX_GPIO_NUM 22  // Connects to CRX
//==================================================================================//
void setup() {
 // pinMode(TX_GPIO_NUM,OUTPUT_OPEN_DRAIN);
  Serial.begin (115200);
  while (!Serial);
  delay (1000);

  Serial.println ("CAN Receiver/Receiver");
  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  // start the CAN bus at 500 kbps
  if (!CAN.begin (1000E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
  // CAN.filter(0x12);
  // CAN.filter(0x13);
}
//==================================================================================//
void loop() {
   canReceiver();
}
//==================================================================================//
void filter() {
   unsigned long packetId = CAN.packetId();
    
    // Define a hexadecimal value to compare with
    unsigned long targetId = 0x12; // Replace with your desired packet ID
    
    // Check if the received packet ID matches the target ID
    if (packetId == targetId) {
      // This block will execute if the received packet ID matches the target ID
      Serial.println("Data from Master 0x");
      Serial.println(packetId, HEX);
      // Perform actions based on the matching packet ID
    }
    else {
      // This block will execute if the received packet ID does not match the target ID
      Serial.print("Data from CAN Node 0x");
      Serial.println(packetId, HEX);
      // Optionally, perform other actions for non-matching IDs
    }
}
void canReceiver() {
  // Try to parse a CAN packet
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    Serial.print("Received ");
    
    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }
    
    if (CAN.packetRtr()) {
      Serial.print("RTR ");
    }
    
    filter();

    // Check if the received packet ID matches the expected ID
    if (CAN.packetId() == 0x13) { // Replace with your expected ID
      if (!CAN.packetRtr()) {
        Serial.print(" and length ");
        Serial.println(packetSize);

        // Read the bytes and interpret them as float values
        float receivedValues[8];
        for (int i = 0; i < 8; i++) {
          uint8_t byteValue = CAN.read();
          receivedValues[i] = (float)byteValue / 76;
        }

        // Print the received float values
        for (int i = 0; i < 8; i++) {
          Serial.print("Received Value ");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(receivedValues[i], 2); // Print with 2 decimal places
        }
      }
    } else {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    }

    Serial.println();
  }
}

//==================================================================================//