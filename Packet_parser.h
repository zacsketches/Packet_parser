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
 * 						add message function.  So the Packet_vector must take pointers to
 *						Packets.
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

public:
  Packet_parser(bool echo = false);   //payload size in bytes

	void add_packet(int payload_size, char target);

  void config();
  
  bool listen();
  
	//determine if a topic is monitored
	bool target_is_monitored(const char target);

	//query the latest data pertaining to a target
	const Packet& query(const char target);
  
};

#endif /* end of include guard: PACKET_PARSER_H */
