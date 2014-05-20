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

#include "Arduino.h"

struct Packet {
private:
  int ps;    // payload_size, bytes in the payload
  int hs;    // header_size in bytes
  int sz;
  boolean rcvd;
  char tgt;    //think of this as the port or application for the packet

public:  
  char* data;

  Packet(int payload_size) 
    :hs(2) { ps = payload_size; data = new char[ps]; sz = ps + hs; }
  
  ~Packet() { delete data; }  
  
  short size() { return sz; }
  int payload_size() { return ps; }
  
  bool received() { return rcvd; }
  void successfully_received() { rcvd = true; }
  void not_received() { rcvd = false; }
  
  char target() { return tgt; }
  void set_target(char target) { tgt = target; }
  
  char get_data(const int elem) {
    //return data elem if elem is in range
    //otherwise return NULL
    char byt = NULL;
    if (elem < ps) {
      byt = data[elem];
    }
    return byt;
  }
  
};

struct Packet_vector() {
	
	/*
		TODO dynamically allocated list of packets.
		Copy stroustrups basic vector implementation
	*/
	
	Packet* data;
	int sz;

	void push_back(Packet& new_packet);
	
	int size() {return sz;}

	Packet& at(int index);
}

//*******************************************************************
//*                         PACKET_PARSER
//*******************************************************************

class Packet_parser {
  
	Packet_list packets;
	char seperator;
  boolean debug;

public:
  Packet_parser(boolean echo = false);   //payload size in bytes
	~Packet_parser() { delete messages; }

	void add_packet(int payload_size, char target);

  void config();
  
  boolean listen();
  
  boolean ready(const char target) { 
    bool res = false;
    if ( packet.received() &&  target==packet.target() ) 
      res = true;
    return res;
  }
  
  char data(const int byt) {
    //return the data element indicated by byt
    return packet.get_data(byt);
  }
  
};

#endif /* end of include guard: PACKET_PARSER_H */
