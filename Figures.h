#pragma once
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <iostream>
#include <math.h>

#define N 13
enum arguments { X, Y, Z, PLUS, MINUS };

class Figures
{
public:
	Figures();
	~Figures();

	void move(arguments coordinate, arguments sign);

	void scale(arguments sign);

	void rotate(arguments coordinate, arguments sign);
protected:
	HDC memDeviceContext;
	HBITMAP memBitmap;
	int x[N], y[N], z[N];
	int xCenter, yCenter, zCenter;
	float transformationMatrix[4][4];

	/*	calculates the central point of one of two figures
		(index1	=	0, index2	=	8	for parallelepiped)
		(index1	=	8, index2	=	12	for pyramid)*/
	void center(int index1, int index2);

	// calculates the central point of the combined two figures
	void center();
private:
	// multiplies x[], y[], z[] by the transformation matrix
	void transformationMatrixMultiplication();

	void clearTransformationMatrix();

	/*	calculates the intersection point	of a plane	defined by planePoint1, planePoint2, planePoint3,
		and									of a line	defined by linePoint1, linePoint2,
		then records the result in x[resultIndex], y[resultIndex],z [resultIndex]*/
	void intersection(int planePoint1, int planePoint2, int planePoint3,
		int linePoint1, int linePoint2, int resultIndex);
};