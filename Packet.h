#ifndef PACKET_H
#define PACKET_H

struct Packet {
private:
  int ps;    // payload_size, bytes in the payload
  int hs;    // header_size in bytes
  int sz;

	/*
		TODO not sure I need the rcvd data member any more
	*/
  bool rcvd;
		
  char tgt;    //think of this as the port or application for the packet

public:  
  char* data;
	
	Packet() { Packet(1); }
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
    char byt;
    if (elem < ps) {
      byt = data[elem];
    }
    return byt;
  } 
};

#endif /* end of include guard: PACKET_H */
