#ifndef BIT_VECTOR__H
#define BIT_VECTOR__H
#include <vector>


class Bit_vector: public std::vector<bool>{

public:


Bit_vector operator& (Bit_vector input){
	Bit_vector ris;
	for (size_t i=0;i<this->size();++i)
		ris.push_back((*this)[i]&input[i]);
	return ris;
}

Bit_vector operator| (Bit_vector input){
	Bit_vector ris;
	for (size_t i=0;i<this->size();++i)
		ris.push_back((*this)[i]|input[i]);
	return ris;
}


};



#endif

