#pragma once

#include "CSG.hpp"
#include "VoxelShape.hpp"

CSG::~CSG()
{
	S.clear();

}

void CSG::RenderToFile(const char * filename, Point3D min, Point3D max, float widthInInches, int voxelsPerInch)
{
	int xVox = widthInInches*voxelsPerInch;
	int yVox = xVox/( max.y - min.y ) * (max.y - min.y);
	int zVox = yVox/ (max.z - min.z) * (max.z - min.z);

	double convertedx = 0;
	double convertedy = 0;
	double convertedz = 0;

	VoxelShape vox = VoxelShape(xVox, yVox, zVox);

	for (auto i = S.begin(); i < S.end(); i++) {

		Shape *p_s = (*i).shape;
		for (int ix = 0; ix < xVox; ix++) {
			convertedx = (((max.x - min.x) / xVox) - min.x) * ix; 

			for (int iy = 0; iy < yVox; iy++) {
				convertedy = (((max.y - min.y) / yVox) - min.y) * iy;

				for (int iz = 0; iz < zVox; iz++) {
					convertedz = (((max.z - min.z) / zVox) - min.z) * iz;

					Point3D a = { convertedx, convertedy, convertedz};
					if (p_s->InsideShape(a)) {

						switch (i->operation)
						{
							case ADD:
								vox.SetBit(ix, iy, iz);
								break;

							case SUBTRACT:
								vox.ClearBit(ix, iy, iz);
								break;

							case MASK:
								//implement
								break;

							case TOGGLE:
								vox.ToggleBit(ix, iy, iz);
								break;

							default:
								break;

						}
					}
				}
			}
		}
	}

	WriteSTL(vox, filename);
}

void CSG::Add(Shape *s)
{
	shapeStruct item;
	item.operation = ADD;
	item.shape = s;
	S.push_back(item);
}

void CSG::Subtract(Shape *s)
{
	shapeStruct item;
	item.operation = SUBTRACT;
	item.shape = s;
	S.push_back(item);
}

void CSG::Mask(Shape *s)
{
	shapeStruct item;
	item.operation = MASK;
	item.shape = s;
	S.push_back(item);
}

void CSG::Toggle(Shape *s)
{
	shapeStruct item;
	item.operation = TOGGLE;
	item.shape = s;
	S.push_back(item);
}

CSG & CSG::operator+=(Shape *s)
{
	this->Add(s);
	return *this;
}

CSG & CSG::operator-=(Shape *s)
{
	this->Subtract(s);
	return *this;
}

CSG & CSG::operator&=(Shape *s)
{
	this->Mask(s);
	return *this;
}

CSG & CSG::operator^=(Shape *s)
{
	this->Toggle(s);
	return *this;
}


