//*******************************************************************
//*                         Packet_vector
//*******************************************************************

void Packet_vector::push_back(Packet_vector& pv) {
	if(space == 0) reserve(3);		//I don't anticipate lots of packet types
	else if(sz == space) reserve(2*space);	//get more space
	elem[sz] = pv;
	++sz;
}

void Packet_vector::reserve(int newalloc) {
	if(newalloc<=space) return;
	Packet* p = new Packet[newalloc];
	for(int i = 0; i<sz; ++i) p[i] = elem[i];
	delete[] elem;
	elemp = p;
	space = newalloc;
}
