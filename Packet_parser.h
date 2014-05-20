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
 *  Dependency:  Serial.h
 *
*/

#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

//#include "Arduino.h"



class Packet_vector {
	// Limited implementation of the c++ standard vector to 
	// allow dyanamic allocation of memory for building a vector of
	// Packets.
	
	int sz;
	Packet* elem;
	int space;

public:
	Packet_vector() : sz(0), elem(0), space(0) {}
	~Packet_vector() { delete[] elem; }
	
	Packet& operator[](int n) { return elem[n]; }
	
	int size() const {return sz;}
	int capacity() const {return space;}
	
	void push_back(Packet_vector& pv);
	void reserve(int newalloc) ;
};

//*******************************************************************
//*                         PACKET_PARSER
//*******************************************************************

class Packet_parser {
  
	Packet_vector packets;
	char seperator;
  bool debug;

public:
  Packet_parser(bool echo = false);   //payload size in bytes
//	~Packet_parser() { delete packets; }

	void add_packet(int payload_size, char target);

  void config();
  
  bool listen();
  
	//determine if a topic is monitored
	bool packet_monitored(const char target);

	//query the latest data pertaining to a target
	const Packet& query(const char target);
  
  char data(const int byt) {
    //return the data element indicated by byt
    return packet.get_data(byt);
  }
  
};

#endif /* end of include guard: PACKET_PARSER_H */
