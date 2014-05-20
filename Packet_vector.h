#ifndef PACKET_VECTOR_H
#define PACKET_VECTOR_H

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


#endif /* end of include guard: PACKET_VECTOR_H */
