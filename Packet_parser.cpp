#include "Packet_parser.h"

Packet_parser::Packet_parser(bool echo) 
	: seperator('|'), debug(echo), header_size(2), min_payload_size(0) {
		version[0] = '0';
		version[1] = '.';
		version[2] = '2';
		version[3] = '\0';
	}

void Packet_parser::config() {
    Serial.print("Config data for Packet_parser v");
		Serial.println(version);
		Serial.print("\tmin payload size: ");
		Serial.println(min_payload_size);
		Serial.print("\tPackets monitored: ");
    Serial.println(packets.size());

		for(size_t i = 0; i < packets.size(); ++i)
		{
			char tgt = packets[i].target();
			int ps = packets[i].payload_size();
			Serial.print("\t\ttgt: ");
			Serial.print(tgt);
			Serial.print("\tpayload size: ");
			Serial.println(ps);
		}
}

void Packet_parser::add_packet(int payload_size, char target ){
	packets.push_back(new Packet(payload_size, target));
	if(min_payload_size != 0){	//not default
		if(payload_size < min_payload_size) min_payload_size = payload_size;
	}
}  

bool Packet_parser::listen(){
  //listens until serial available() == 0
	//return true if a valid packet was received
	/*
		TODO consider a timeout if there is a steady stream of serial
	*/

	bool res = false;
  
  //verify there is at least enough data in the serial buffer to make a packet
	//+1 is for the target char
  while ( Serial.available() >= (header_size + min_payload_size +1) ) {
	  
    //check to make sure the first two chars are the packet seperators
    /*
    	TODO consider changing the below if to while.  This might eat up
			more serial buffer per cycle and keep the buffer cleaner.
    */
		if(Serial.read()==seperator && Serial.read()==seperator) {
    
      char test_target = Serial.read();
			
			if(target_is_monitored(test_target)){
				int ps = payload_size(test_target);
				if(debug) {
					Serial.println("Incoming packet detected:");
	        Serial.print("\ttarget is: ");
	        Serial.print(test_target);
					Serial.print("\tpayload size is: ");
					Serial.println(ps);
	      }
				//read the packet's data into the parser's data
				//packet.  The packet object in the parser has a 
				//buffer of payload_size for each type of packet
				for(size_t i = 0; i < packets.size(); ++i)
				{
					if(test_target == packets[i].target()){
						for(int j = 0; j < ps; ++j) {
			        packets[i].elem[j] = Serial.read();
							packets[i].set_delivered(false);
							if(debug) Serial.println(packets[i].elem[j]);
			      }
					}
				}
				res = true;	//packet successfully read
			} //close if(target_is_monitored)
    }
  } //close while
	return res;
}

bool Packet_parser::target_is_monitored(const char target){
	//find out if a target is in the set of monitored packets
	/*
		TODO consider making target is monitored return an int value
		of payload size.  This will prevent double scanning in the
		query function and listen function.
	*/
	bool res = false;
	for(size_t i = 0; i < packets.size(); ++i)
	{
		if(target == packets[i].target()) res = true;
	}
	return res;
}

int Packet_parser::payload_size(const char target){
	int res = 0;
	for(size_t i = 0; i < packets.size(); ++i)
	{
		if(target == packets[i].target()){
			res = packets[i].payload_size();
		}
	}
	return res;
}

void Packet_parser::query(const char target, char* buffer){
	if(target_is_monitored(target)){
		int ps = payload_size(target);
		//find the packet
		for(size_t i = 0; i < packets.size(); ++i) {
			if(target == packets[i].target()){			
				if(packets[i].delivered() == true) return;	//short circuit if already delivered
				//store the packet into the buffer
				for(size_t j = 0; j < ps; ++j){
					buffer[j] = packets[i].elem[j];
				}
				packets[i].set_delivered(true);
				return;
			}
		}
	}
}
