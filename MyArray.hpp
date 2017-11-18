//
//  DynamicArray.h
//  Homework6
//
//  Created by Ben Jones on 2/24/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

#pragma once
#include <cassert>
#include <algorithm> //for std::max

template<typename T>
class MyArray{
    
public:
    //make at least a 20 element array
	MyArray(size_t initialSize = 0) {
		if (initialSize <= 20) {
			capacity = 20;
		}
		else if (initialSize > 20) {
			capacity = initialSize;
		}
		size = initialSize;
		data = new T[capacity];
	};

    
    MyArray(const MyArray<T>& other);
    MyArray& operator=(const MyArray<T>& other);
    ~MyArray();
    
    T Get(size_t index) const;
    void Set(T v, size_t index);
    
    size_t Size() const { return size; }

    T Back() const { assert(size > 0); return Get(size -1); }
    void PushBack(T val);
    void PopBack(){ --size; }
    
    void Resize(size_t newSize);
    
private:
    T* data;
    size_t size, capacity;
    
};

template<typename T>
MyArray<T>::MyArray(const MyArray<T>& other){
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for(int i = 0; i < size; i++){ //don't worry about stuff between size and capacity
        data[i] = other.data[i]; //calls operator =, so should make deep copies
    }
}

template<typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray<T>& other){
    if(this == &other){return *this;}
    delete [] data;
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for(int i = 0; i < size; i++){
        data[i] = other.data[i]; //calls operator =, so should make deep copies
    }
    return *this;
}

template<typename T>
MyArray<T>::~MyArray(){
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

template <typename T>
T MyArray<T>::Get(size_t index) const{
    assert(index < size);
    return data[index];
}

template<typename T>
void MyArray<T>::Set(T val, size_t index){
    assert(index < size);
    data[index] = val;
}



template<typename T>
void MyArray<T>::PushBack(T val){
    if(size == capacity){
        size_t oldSize = size;
        Resize(2*capacity);
        size = oldSize; //resize changes the size, but we don't want to do that.
    }
    data[size++] = val; //use the old value of size and then add 1 to it
}


template<typename T>
void MyArray<T>::Resize(size_t newSize){
    T* newData = new T[newSize];
    
    for(int i = 0; i < std::min(size, newSize); i++){
        newData[i] = data[i];
    }

    capacity = newSize;
    size = newSize;
    delete[] data;
    data = newData;

}
