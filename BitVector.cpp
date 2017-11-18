//
//  BitVector.cpp
//  Homework 5
//

#include "BitVector.hpp"

BitVector::BitVector(size_t elements)
: numElements(elements),
storageSize(GetStorageSize(numElements)),
data(storageSize)
{
    Clear();
}


void BitVector::Fill()
{
	uint64_t fill = -1;
	for (int x = 0; x < storageSize; x++)
		data.Set(fill, x);
}

void BitVector::Clear()
{
	// This sets 64 bits at a time
	for (int x = 0; x < storageSize; x++)
        data.Set(0, x);
}

bool BitVector::Get(size_t whichBit) const
{
	size_t element;
	int offset;
	GetElementAndOffset(whichBit, element, offset);
	return (data.Get(element)>>offset)&0x1;
}

void BitVector::Set(size_t whichBit, bool value)
{
	const uint64_t one = 1;
	size_t element;
	int offset;
	GetElementAndOffset(whichBit, element, offset);
	if (value)
	{
        uint64_t old = data.Get(element);
        old |= (one<<offset);

        data.Set(old, element);
    } else {
        uint64_t old = data.Get(element);
		old &= (~(one<<offset));
        data.Set(old, element);
	}
}

void BitVector::Toggle(size_t whichBit)
{
	const uint64_t one = 1;
	size_t element;
	int offset;
	GetElementAndOffset(whichBit, element, offset);
    uint64_t old = data.Get(element);
    
	old ^= (one<<offset);
    data.Set(old, element);
}

size_t BitVector::Size() const
{
	return numElements;
}

void BitVector::Resize(size_t newSize)
{
    
    data.Resize(GetStorageSize(newSize));
    for(size_t i = numElements; i < sizeof(uint64_t)*8*GetStorageSize(newSize); i++){
        Set(i, 0); //set new bits to be 0;
    }
    numElements = newSize;
    storageSize = GetStorageSize(newSize);
}

size_t BitVector::GetStorageSize(size_t numElements) const
{
	size_t result = numElements>>6;
	if (numElements&0x3F)
		return result+1;
	return result;
}

void BitVector::GetElementAndOffset(size_t index, size_t &element, int &offset) const
{
	element = index>>6;
	offset = index&0x3F;
}
