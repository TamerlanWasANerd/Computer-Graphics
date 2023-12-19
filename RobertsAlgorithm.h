#pragma once
#include "Figures.h"
class RobertsAlgorithm : public Figures
{
public:
	RobertsAlgorithm(int x, int y, int z);

	// runs Roberts algorithm for all edges
	void algorithm();
protected:
	void fillBlock(int i1, int i2, int coordinates1[][4], COLORREF color);
private:
	// checks whether a plane, defined by three points, is visible
	bool visible(int point1, int point2, int point3);
	// draws the visible part of a plane
	void drawVisiblePlane(int num, int point1, int point2, int point3, int point4 = 0);
	void paintXY(int x, int y, COLORREF color);

	COLORREF colors[9];

	// observer point coordinates, must only be set through the constructor
	int xObserver, yObserver, zObserver;
};