//
//  surfaceExtraction.hpp
//  Homework3
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

#pragma once
#include <cstdint>
#include "VoxelShape.hpp"
#include "MyArray.hpp"
#include "SharedQueue.hpp"
#include "Timer.hpp"


enum FaceType {
	NX,
	NY,
	NZ,
	PX,
	PY,
	PZ
};

struct Triangle {
	float normal[3];
	float v1[3];
	float v2[3];
	float v3[3];
};

struct Range{
	int startX, endX, startY, endY;
};

void WriteSTL(const VoxelShape &model, const char* filename);
void extractionLoop(int x, int y, const VoxelShape &model, MyArray<Triangle> *arr, std::mutex *mtx);
void threadWorker(SharedQueue<Range> *workQueue, MyArray<Triangle> *results, const VoxelShape *model, std::mutex *mtx);
