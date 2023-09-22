#include <CAN.h>
#define TX_GPIO_NUM 17 // Connects to CTX
#define RX_GPIO_NUM 16  // Connects to CRX
#define cs 10
#define irq 2
//==================================================================================//
void setup() {
  Serial.begin (115200);
  while (!Serial);
  delay (1000);

  Serial.println ("CAN Sender");
  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  //CAN.setPins(cs, irq);
  // start the CAN bus at 500 kbps
  if (!CAN.begin (1000E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
}
//==================================================================================//
void loop() {
   canSender();
 // canReceiver();
}
//==================================================================================//
void canSender() {
  float values[8] = {1.312, 2.367, 0.69, 2.61, 3.28, 0.085, 3.12, 3.3}; // Array of float values to send
  uint8_t byteValues[8];
  Serial.print("Sending packet ... ");

  CAN.beginPacket(0x13); // Sets the ID and clears the transmit buffer
  // Convert float values to uint8_t and write them to the CAN packet
  for (int i = 0; i < 8; i++) {
    byteValues[i] = (uint8_t)(values[i] * 76);
    CAN.write(byteValues[i]);
  }

  CAN.endPacket();
  Serial.println("done");
}

//==================================================================================//