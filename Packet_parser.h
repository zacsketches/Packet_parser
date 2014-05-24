/*  Packet based serial communcations class for Arduino
 *  
 *  Author:   Zac Staples
 *  email:    zacstaples@mac.com
 *  license:  If you use this, then buy me a beer at Maker Fair 2015
 *  date:     20 May 2014
 *
 *  About:    This class implements a VERY low overhead packet based Arduino serial
 *            link between a transmitter (XMTR) and reciever (RCVR).  
 *            Comms are 1-way from the XMTR to the RCVR.  My application
 *            was to use the Esplora as a remote control for another 
 *            Arduino based robot.
 * 
 *						Expected implementation relies on push_back of new Packet in the
 * 						add message function.  So the Packet_vector provided in a seperate
 *						library stores pointers to Packets and performs the basic functions
 *						you would expect to find in an STL vector<Packet*>.
 *					
 *						The bool function listen() does the heavy lifting for this class.
 *						This function monitors the incoming serial port for the message header
 *						defined as || (two pipes).  Once a header is received, listen reads 
 *						the next char and tests to see if that 'target' is correlates to
 *						a monitored packet.  If the packet is monitored, then listen reads
 *						the next series of of 
 *
 *  Dependency:  Serial.h
 *
*/

#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include "Arduino.h"
#include "Packet_vector.h"

class Packet_parser {
  
	Packet_vector packets;
	char seperator;
  bool debug;
	char version[4];				//facilitates versioning in the style 1.0, 0.2, etc
	int header_size;
	int min_payload_size;

public:
  Packet_parser(bool echo = false);   //payload size in bytes

  void config();

	void add_packet(int payload_size, char target);
  
  bool listen();
  
	//determine if a topic is monitored
	bool target_is_monitored(const char target);
	
	//find the packet size for a monitored target
	//returns 0 for an unmonitored packet
	int payload_size(const char target);

	//query the latest data pertaining to a target
	void query(const char target, char* buffer);
  
};

#endif /* end of include guard: PACKET_PARSER_H */
