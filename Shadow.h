#pragma once
#include "RobertsAlgorithm.h"
class Shadow : public RobertsAlgorithm
{
public:
	Shadow();

	// paints the shadow
	void paint();
protected:
	// shadow coordinates
	int xShadow[N], yShadow[N], zShadow[N];
private:
	// calculates the intersection between a plane and a line
	void intersection();

	void calculatePlaneCoefficients();

	// light source coordinates
	int xLight, yLight, zLight;

	// plane coefficients used for projection
	int A, B, C, D;
	int m, n, p;
};