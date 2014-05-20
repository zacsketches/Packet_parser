#include "Arduino.h"
#include "Packet_parser.h"

Packet_parser::Packet_parser(int payload_size, boolean echo) 
  : packet(payload_size) { 
  seperator = '|'; 
  debug = echo; 
  
}

void Packet_parser::config() {
    Serial.println("Config data: ");
    Serial.print("\tpacket payload size: ");
    Serial.print(packet.payload_size());
    Serial.println();
}
  

boolean Packet_parser::listen(){
  //returns true on successful receipt of a packet.
  packet.not_received();  
  
  //verify there is at least enough data in the serial buffer to make a packet
  while (Serial.available() >= packet.size()) {
    
    //check to make sure the first two chars are the packet seperators
    if(Serial.read()==seperator && Serial.read()==seperator) {
    
      if(debug) {
        Serial.println("PACKET ECHO:");
        Serial.print("\tpacket size is: ");
        Serial.println(packet.payload_size());
      }
      
      char tmp = Serial.read();
      packet.set_target(tmp);
      if(debug) {
        Serial.print("\ttarget is: ");
        Serial.print(tmp);
      }
      
      //read the packet's data
      for(int i = 0; i < packet.payload_size(); ++i) {
        packet.data[i] = Serial.read();
      }
      
      if(debug) {
        //print the data packet
        Serial.print("\tdata: ");
        for(int i = 0; i < packet.payload_size(); ++i) {
          Serial.print(packet.data[i]);
          Serial.print('\t');
        }
        Serial.println();
      }
      
      packet.successfully_received();
    }
  }
    
  return packet.received();
}