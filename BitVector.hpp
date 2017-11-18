//
//  BitVector.hpp
//  Homework 5
//

#ifndef BitVector_hpp
#define BitVector_hpp

#include <stdio.h>
#include <cstdint>
#include "MyArray.hpp"

class BitVector {
public:
	BitVector(size_t elements);
	
	void Fill();
	void Clear();
	bool Get(size_t whichBit) const;
	void Set(size_t whichBit, bool value);
	void Toggle(size_t whichBit);
	size_t Size() const;
	void Resize(size_t newSize);
private:
	size_t GetStorageSize(size_t numElements) const;
	void GetElementAndOffset(size_t index, size_t &element, int &offset) const;
	
	size_t numElements;
	size_t storageSize;
    MyArray<uint64_t> data;
};

#endif /* BitVector_hpp */
