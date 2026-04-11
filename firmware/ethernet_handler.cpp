#include "ethernet_handler.h"

// Set a MAC address and an IP address for your ESP32
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177); // Change to match your network
unsigned int localPort = 8888;  // The port you will send data to

IPAddress remote_ip(192, 168, 1, 100); // Change this to your computer's IP address!
unsigned int remote_port = 9999;       // The port your computer is listening on

EthernetUDP Udp;

void init_ethernet() {
  // 1. Hardware Reset the W5500 chip (Highly recommended for stability)
  pinMode(PIN_W5500_RST, OUTPUT);
  digitalWrite(PIN_W5500_RST, LOW);
  delay(10);
  digitalWrite(PIN_W5500_RST, HIGH);
  delay(100); // Give it time to wake up

  // 2. Route the ESP32's SPI bus to your custom pins
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_W5500_CS);
  
  // 3. Tell the Ethernet library which pin is Chip Select (CS)
  Ethernet.init(PIN_W5500_CS);

  // 4. Start the Ethernet connection
  Ethernet.begin(mac, ip);
  
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ERROR: W5500 Ethernet shield was not found.");
  } else {
    Serial.print("Ethernet initialized. IP: ");
    Serial.println(Ethernet.localIP());
  }

  // 5. Start listening for UDP packets
  Udp.begin(localPort);
}

bool send_ethernet_data(String data_to_be_sent) {
  // 1. Open a connection to the destination IP and Port
  Udp.beginPacket(remote_ip, remote_port);
  
  // 2. Load the string data into the packet
  Udp.print(data_to_be_sent);
  
  // 3. Fire the packet over the network! 
  // endPacket() returns 1 if it successfully sent, and 0 if it failed.
  if (Udp.endPacket() == 1) {
    return true;  // Sent successfully
  } else {
    return false; // Failed to send
  }
}



bool receive_ethernet_data(int computer_command[]) {
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
    char packetBuffer[128]; // Buffer to hold the incoming string
    
    // Read the packet into the buffer
    int len = Udp.read(packetBuffer, 127);
    if (len > 0) {
      packetBuffer[len] = '\0'; // Add a null-terminator to make it a valid C-string
    }

    // i want to store return the result into computer_command. the  incoming data from the w5500 may be in "CMD, 1322, 1233, 1200, 2300, 2100, 2100" or in "SERVO, 1232". the CMD and SERVO is the header whcih device that shall be actuated. but the problem is CMD has 7 array value, and SERVO has only 2. is that gonna work?
    // im just gonna pass this for now.  
 
    }
    return true;
}