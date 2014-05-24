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

struct Packet {
private:
  int ps;    // payload_size, bytes in the payload
  int hs;    // header_size in bytes. Typical value is '||'
  int sz;
  char tgt;    //think of this as the port or application for the packet
	bool delv;	//has the newest data been queried delivered
	
public:  
  char* elem;
	
	Packet() {}
  Packet(int payload_size, char target) 
    : ps(payload_size), hs(2), sz(ps + hs), tgt(target), elem(new char[ps]),
			delv(true) {}
  
  ~Packet() {
		delete[] elem;
	}  
  
	//copy assignment
	Packet& operator=(const Packet* p) {
		ps = p->ps;
		hs = p->hs;
		sz = p->sz;
		tgt = p->tgt;
		elem = p->elem;
	}

  int size() { return sz; }
	int payload_size() { return ps; }
  
  char target() { return tgt; }
  void set_target(const char target) { tgt = target; }
  
	bool delivered() {return delv;}
	void set_delivered(const bool state) { delv = state; }

  char data(const int index) {
    //return data elem if elem is in range
    //otherwise return NULL
    char byt;
    if (index < ps) {
      byt = elem[index];
    }
    return byt;
  } 
};	//end packet


class Packet_vector {
	// Limited implementation of the c++ standard vector to 
	// allow dyanamic allocation of memory for building a vector of
	// Packet*.  A vector of pointers is necessary to allow the primary
	// user of the vector, Packet_parser, to push_back new objects
	// allocated on the free store.
	
	int sz;
	Packet* elem;
	int space;

public:
	Packet_vector() : sz(0), elem(0), space(0) {}
	~Packet_vector() { 
		delete[] elem;
	}
	
	Packet& operator[](int n) { return elem[n]; }
	
	int size() const {return sz;}
	int capacity() const {return space;}
	
	void push_back(Packet* p);
	void reserve(int newalloc) ;
};	//end Packet_vector

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
  
};	//end Packet_parser

#endif /* end of include guard: PACKET_PARSER_H */
