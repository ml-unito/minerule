#ifndef BIT_VECTOR__H
#define BIT_VECTOR__H
#include <vector>

namespace minerule {
	class BitVector: public std::vector<bool>{

	public:


		BitVector operator& (BitVector input){
			BitVector ris;
			for (size_t i=0;i<this->size();++i)
				ris.push_back((*this)[i]&input[i]);
			return ris;
		}

		BitVector operator| (BitVector input){
			BitVector ris;
			for (size_t i=0;i<this->size();++i)
				ris.push_back((*this)[i]|input[i]);
			return ris;
		}


	};

}


#endif

