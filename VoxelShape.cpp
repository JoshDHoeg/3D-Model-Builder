//
//  VoxelShape.cpp
//  Homework 5
//
//  Created by Nathan Sturtevant on 2/23/17.
//  Copyright © 2017 NS Software. All rights reserved.
//

#include "VoxelShape.hpp"
#include <algorithm>
#include <iostream>

VoxelShape::VoxelShape(int x, int y, int z)
:nx(x), ny(y), nz(z), data(x*y*z)
{}

void VoxelShape::GetSize(int &x, int &y, int &z) const
{
	x = nx;
	y = ny;
	z = nz;
}

void VoxelShape::ClearModel()
{
	data.Clear();
}

void VoxelShape::FillModel()
{
	data.Fill();
}


bool VoxelShape::GetBit(int x, int y, int z) const
{
	return data.Get(GetIndex(x, y, z));
}

void VoxelShape::SetBit(int x, int y, int z)
{
	return data.Set(GetIndex(x, y, z), 1);
}

void VoxelShape::ClearBit(int x, int y, int z)
{
	return data.Set(GetIndex(x, y, z), 0);
}

void VoxelShape::ToggleBit(int x, int y, int z)
{
	data.Toggle(GetIndex(x, y, z));
}


void VoxelShape::AddSphere(float x, float y, float z, float r)
{
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			for (int k = minZ; k < maxZ; k++)
			{
				if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
				{
					SetBit(i, j, k);
				}
			}
		}
	}
}

void VoxelShape::SubtractSphere(float x, float y, float z, float r)
{
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			for (int k = minZ; k < maxZ; k++)
			{
				if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
				{
					ClearBit(i, j, k);
				}
			}
		}
	}
}

void VoxelShape::ToggleSphere(float x, float y, float z, float r)
{
	int minX = std::max(0, static_cast<int>(x - r));
	int maxX = std::min(nx, static_cast<int>(x + r));
	int minY = std::max(0, static_cast<int>(y - r));
	int maxY = std::min(ny, static_cast<int>(y + r));
	int minZ = std::max(0, static_cast<int>(z - r));
	int maxZ = std::min(nz, static_cast<int>(z + r));
	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			for (int k = minZ; k < maxZ; k++)
			{
				if (((x-i)*(x-i) + (y - j)*(y - j) + (z - k)*(z - k)) < r*r)
				{
					ToggleBit(i, j, k);
				}
			}
		}
	}
}


void VoxelShape::GetXYZ(size_t index, int& x, int& y, int& z) const
{
	z = index % nz;
	index /= nz;
	y = index % ny;
	x = index / ny;
}

size_t VoxelShape::GetIndex(int x, int y, int z) const
{
	return x*ny*nz + y*nz + z;
}

