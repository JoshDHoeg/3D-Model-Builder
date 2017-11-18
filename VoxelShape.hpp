//
//  VoxelShape.hpp
//  Homework 5
//
//  Created by Nathan Sturtevant on 2/23/17.
//  Copyright © 2017 NS Software. All rights reserved.
//

#ifndef VoxelShape_hpp
#define VoxelShape_hpp

#include <stdio.h>
#include "BitVector.hpp"

class VoxelShape
{
public:
	VoxelShape(int x, int y, int z);
	void GetSize(int &x, int &y, int &z) const;
	void ClearModel();
	void FillModel();
	
	bool GetBit(int x, int y, int z) const;
	
	void SetBit(int x, int y, int z);
	void ClearBit(int x, int y, int z);
	void ToggleBit(int x, int y, int z);
	
	void AddSphere(float x, float y, float z, float r);
	void SubtractSphere(float x, float y, float z, float r);
	void ToggleSphere(float x, float y, float z, float r);
private:
	void GetXYZ(size_t index, int& x, int& y, int& z) const;
	size_t GetIndex(int x, int y, int z) const;

	int nx, ny, nz;
	BitVector data;
};

#endif /* VoxelShape_hpp */
