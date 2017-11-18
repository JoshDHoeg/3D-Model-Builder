#ifndef CSG_hpp
#define CSG_hpp

#include "SurfaceExtraction.hpp"
#include <vector>

struct Point3D {
	Point3D() {}
	Point3D(double x, double y, double z) :x(x), y(y), z(z) {}
	Point3D(float x, float y, float z) :x(x), y(y), z(z) {}
	Point3D(int x, int y, int z) :x(x), y(y), z(z) {}
	float x, y, z;
};

class Shape {
public:
	virtual ~Shape() {}
	virtual bool InsideShape(Point3D p) = 0;
};

class Line : public Shape {
public:
	Line(Point3D p1, Point3D p2, float r);
	bool InsideShape(Point3D p);
private:
	float magnitude(Point3D p);
	Point3D cross(Point3D p1, Point3D p2);
	Point3D p1, p2;
	float r;
};


Line::Line(Point3D p1, Point3D p2, float r)
	:p1(p1), p2(p2), r(r)
{
}

bool Line::InsideShape(Point3D p0)
{
	Point3D a = { p0.x - p1.x, p0.y - p1.y, p0.z - p1.z };
	Point3D b = { p0.x - p2.x, p0.y - p2.y, p0.z - p2.z };
	Point3D c = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	float dist = magnitude(cross(a, b)) / magnitude(c);
	return dist <= r;
}

float Line::magnitude(Point3D p)
{
	return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

Point3D Line::cross(Point3D p1, Point3D p2)
{
	Point3D result;
	result.x = p1.y*p2.z - p1.z*p2.y;
	result.y = p1.z*p2.x - p1.x*p2.z;
	result.z = p1.x*p2.y - p1.y*p2.x;
	return result;
}

enum Operation
{
	ADD,
	SUBTRACT,
	MASK,
	TOGGLE
};

struct shapeStruct {
	Shape * shape;
	Operation operation;
};

class CSG {
public:
	~CSG();
	void RenderToFile(const char *filename, Point3D min, Point3D max, float widthInInches, int voxelsPerInch = 256);
	void Add(Shape *);
	void Subtract(Shape *);
	void Mask(Shape *);
	void Toggle(Shape *);

	CSG &operator+=(Shape *);
	CSG &operator-=(Shape *);
	CSG &operator&=(Shape *);
	CSG &operator^=(Shape *);

private:
	std::vector<shapeStruct> S;
};


#endif // !CSG_hpp
