#include <CAN.h>
#define TX_GPIO_NUM 17 // Connects to CTX
#define RX_GPIO_NUM 16  // Connects to CRX
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
  if (!CAN.begin (500E3)) {
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
   // canSender();
   canReceiver();
}
//==================================================================================//
void canSender() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print ("Sending packet ... ");
  CAN.beginPacket (0x12);  //sets the ID and clears the transmit buffer
  // CAN.beginExtendedPacket(0xabcdef);
  
  CAN.write ('4'); //write data to buffer. data is not sent until endPacket() is called.
  CAN.write ('7');
  CAN.write ('2');
  CAN.write ('5');
  CAN.endPacket();
  
  //RTR packet with a requested data length
  CAN.beginPacket (0x12, 3, true);
  CAN.endPacket();
  Serial.println ("done");
  delay (1000);
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
  // try to parse packet
  int packetSize = CAN.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print ("Received ");
    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }
    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }
    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);
    filter();
    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);
      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }
    Serial.println();
  }
}
//==================================================================================//