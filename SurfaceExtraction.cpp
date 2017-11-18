//
//  surfaceExtraction.cpp
//  Homework3
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS

#include "SurfaceExtraction.hpp"

#include <iostream>
#include <cstdio>
#include <cstdint>

inline void FillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2)
{
    t1.v1[cInd] = c;
    t2.v1[cInd] = c;
    t1.v2[cInd] = c;
    t2.v2[cInd] = c;
    t1.v3[cInd] = c;
    t2.v3[cInd] = c;
    int aInd = (cInd +1) % 3;
    int bInd = (cInd +2) % 3;
    
    t1.v1[aInd] = a1;
    t1.v2[aInd] = a2;
    t1.v3[aInd] = a2;
    
    t2.v1[aInd] = a1;
    t2.v2[aInd] = a2;
    t2.v3[aInd] = a1;
    
    t1.v1[bInd] = b1;
    t1.v2[bInd] = b1;
    t1.v3[bInd] = b2;
    
    t2.v1[bInd] = b1;
    t2.v2[bInd] = b2;
    t2.v3[bInd] = b2;
    
}

void ExtractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2)
{
    for (int i= 0; i < 3; i++)
	{
        t1.normal[i] = 0;
        t2.normal[i] = 0;
    }
    switch(face){
        case NX:
            t1.normal[0] = -1;
            t2.normal[0] = -1;
            FillPlane(y + 1, y, z, z+1, x, 0, t1, t2);
            break;
        case NY:
            t1.normal[1] = -1;
            t2.normal[1] = -1;
            FillPlane(z + 1, z, x, x+1, y, 1, t1, t2);
            break;
        case NZ:
            t1.normal[2] = -1;
            t2.normal[2] = -1;
            FillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
            break;
        case PX:
            t1.normal[0] = 1;
            t2.normal[0] = 1;
            FillPlane(y, y + 1, z, z +1, x + 1, 0, t1, t2);
            break;
        case PY:
            t1.normal[1] = 1;
            t2.normal[1] = 1;
            FillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
            break;
        case PZ:
            t1.normal[2] = 1;
            t2.normal[2] = 1;
            FillPlane(x, x +1, y, y + 1, z + 1, 2, t1, t2);
            break;
    }
}

void WriteTriangle(FILE* f, const Triangle& t){
    fwrite(t.normal, sizeof(float), 3, f);
    fwrite(t.v1, sizeof(float), 3, f);
    fwrite(t.v2, sizeof(float), 3, f);
    fwrite(t.v3, sizeof(float), 3, f);
    uint16_t zero = 0;
    fwrite(&zero, sizeof(zero), 1, f);
}

void WriteSTL(const VoxelShape &model, const char* filename){
    
    MyArray<Triangle> triangles;
	SharedQueue<Range> workQueue;
	std::mutex mtx;
	
	int nx, ny, nz;
	model.GetSize(nx, ny, nz);
    
	unsigned numThreads = 4;
	//unsigned numThreads = 1;

	std::thread **threads;
	threads = new std::thread*[numThreads];

	for (int x = 0; x < numThreads; x++)
	{
		threads[x] = new std::thread(threadWorker, &workQueue, &triangles, &model, &mtx);
	}

    
	for (int x = 0; x < nx; x++)
	{
        for (int y = 0; y < ny; y++)
		{
			Range *r = new Range{ x, nx, y, ny };
			workQueue.Add(*r);
        }
    }

	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			Range *r = new Range{ -1, -1};
			workQueue.Add(*r);
		}
	}

	for (int x = 0; x < numThreads; x++)
	{
		threads[x]->join();
		delete threads[x];
	}
	delete[] threads;

    FILE* f = fopen(filename, "wb");
    uint8_t header[80] = {0};
    fwrite(header, sizeof(uint8_t), 80, f);
    uint32_t numTris = triangles.Size();
    fwrite(&numTris, sizeof(numTris), 1, f);
    while(triangles.Size() > 0){
        Triangle t = triangles.Back();
        triangles.PopBack();
        WriteTriangle(f, t);
    }
    fclose(f);
}


void extractionLoop(int x, int y, const VoxelShape &model, MyArray<Triangle> *arr , std::mutex *mtx) {
	Triangle t1, t2;

	int nx, ny, nz;
	model.GetSize(nx, ny, nz);
	MyArray<Triangle> tempArray;

	for (int z = 0; z < nz; z++)
	{
		if (model.GetBit(x, y, z))
		{
			if (((x - 1) < 0) || !model.GetBit(x - 1, y, z))
			{
				ExtractFace(x, y, z, NX, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);
			}
			if (((x + 1) >= nx) || !model.GetBit(x + 1, y, z))
			{
				ExtractFace(x, y, z, PX, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);
			}
			if (((y - 1) < 0) || !model.GetBit(x, y - 1, z))
			{
				ExtractFace(x, y, z, NY, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);
			}
			if (((y + 1) >= ny) || !model.GetBit(x, y + 1, z))
			{
				ExtractFace(x, y, z, PY, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);
			}
			if ((z - 1 < 0) || !model.GetBit(x, y, z - 1))
			{

				ExtractFace(x, y, z, NZ, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);

			}
			if ((z + 1 >= nz) || !model.GetBit(x, y, z + 1))
			{
				ExtractFace(x, y, z, PZ, t1, t2);
				tempArray.PushBack(t1);
				tempArray.PushBack(t2);
			}
		}
	}
	mtx->lock();
	for (int i = 0; i < tempArray.Size(); i++) {
		arr->PushBack(tempArray.Get(i));
	}
	mtx->unlock();
}

void threadWorker(SharedQueue<Range> *workQueue, MyArray<Triangle> *results, const VoxelShape *model, std::mutex *mtx)
{
	Range r;
	while (true) {

		while (workQueue->Remove(r) == false)
		{
			std::this_thread::yield();
		}

		if ((r.startX == r.endX) && (r.startY == r.endY))
		{
			break;
		}
		
		extractionLoop(r.startX, r.startY, *model, results , mtx);
	}
}
