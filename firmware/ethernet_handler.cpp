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

bool receive_ethernet_data(int ethernet_data[DATA_LENGTH]) { 
  // 1. Check if any UDP packet has arrived
  int packetSize = Udp.parsePacket();
  
  if (packetSize > 0) {
    // 2. Create a character buffer to hold the incoming data.
    // NOTE: Ensure 256 is large enough for your maximum expected string length. 
    // If your string is longer, increase this number.
    char packetBuffer[256]; 
    
    // 3. Read the packet data into the buffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0'; // Null-terminate the array so it acts as a valid C-string
    }

    // 4. Parse the comma-separated string
    int index = 0;
    
    // strtok looks for the first delimiter (",") and grabs the chunk of text before it
    char* token = strtok(packetBuffer, ","); 
    
    // Loop through the string until we run out of tokens or hit our array limit
    while (token != NULL && index < DATA_LENGTH) {
      // atoi() converts the text token (e.g., " 124 ") into an actual integer (124)
      ethernet_data[index] = atoi(token); 
      
      index++;
      
      // Passing NULL tells strtok to continue from where it left off in the same string
      token = strtok(NULL, ","); 
    }
    
    return true; // Packet successfully received and parsed
  }
  
  return false; // No packet available to read during this loop cycle
}