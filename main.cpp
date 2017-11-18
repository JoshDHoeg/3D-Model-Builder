//
//  main.cpp
//  Homework2
//
//  Created by Ben Jones on 1/4/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

#include <iostream>

#include <random>
#include <cassert>
#include "CSG.hpp"
#include "SurfaceExtraction.hpp"

void TestBitVector();

int main(int argc, const char * argv[])
{
	TestBitVector();
	
	CSG csg;

	Point3D a = { 30, 30, 30};
	Point3D b = { 10, 10, 10};
	Line *l = new Line(a, b, 0.1);


	csg.Add(l);

	Point3D max = { 100, 100, 100 };
	Point3D min = { 0, 0, 0 };



 //   VoxelShape m(150, 150, 150);

	////body
	//m.AddSphere(30, 80, 80, 30);
	//m.AddSphere(70, 80, 80, 25);
	//m.AddSphere(100, 80, 80, 20);

	////eyes
	//m.AddSphere(110, 97, 75, 4);
	//m.AddSphere(110, 97, 85, 4);

	Timer t;
	t.StartTimer();

	csg.RenderToFile("model.stl", min, max, 1, 1);
    /*WriteSTL(m, "model.stl");*/

	t.EndTimer();

	//std::cout << "this code with 1 thread took: " << t.GetElapsedTime() << std::endl;
	std::cout << "this code with 4 thread took: " << t.GetElapsedTime() << std::endl;
}

void TestBitVector()
{
	BitVector b(999);
	BitVector b2(20);
	
	for (int x = 0; x < 999; x+=2)
		b.Set(x, 1);

	b2 = b;

	assert(b.Size() == b2.Size());
	for (int x = 0; x < 999; x++)
		assert(b.Get(x) == b2.Get(x));

	b.Resize(0);
	b.Resize(999);
	for (int x = 0; x < 999; x++)
	{
		assert(b.Get(x) == 0);
		b.Set(x, 1);
	}
	b.Resize(1002);
	for (int x = 999; x < b.Size(); x++)
	{
		assert(b.Get(x) == 0);
	}
	
    
    BitVector bv(1);
    bv.Fill();
    bv.Resize(64);
    assert(bv.Get(0));
    for(int i = 1; i < 64; i++){
        assert(!bv.Get(i));
    }

    
	printf("BitVector passed tests\n");
}
