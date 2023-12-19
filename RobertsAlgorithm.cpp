#include "RobertsAlgorithm.h"
#include <stack>

RobertsAlgorithm::RobertsAlgorithm(int x, int y, int z) : Figures(), xObserver(x), yObserver(y), zObserver(z)
{
	short random[27];

	srand(8);

	for (int i = 0; i < 27; ++i)
		random[i] = rand() % 256;

	for (int i = 0; i < 9; ++i)
		colors[i] = RGB(random[i], random[i + 9], random[i + 18]);
}

void RobertsAlgorithm::algorithm()
{
	SelectObject(memDeviceContext, GetStockObject(DC_BRUSH));
	SetDCBrushColor(memDeviceContext, RGB(150, 150, 150));
	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, RGB(150, 150, 150));
	Rectangle(memDeviceContext, 0, 0, 1200, 900);
	SelectObject(memDeviceContext, GetStockObject(DC_BRUSH));
	SetDCBrushColor(memDeviceContext, RGB(255, 255, 255));
	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, RGB(255, 255, 255));

	if (visible(0, 2, 4))
	{
		center(0, 6);
		if (visible(0, 1, 5))
			drawVisiblePlane(0, 0, 1, 5);
		if (visible(3, 4, 2))
			drawVisiblePlane(1, 3, 4, 2);
		if (visible(1, 0, 4))
			drawVisiblePlane(2, 1, 0, 4, 3);
		if (visible(0, 4, 2))
			drawVisiblePlane(3, 0, 4, 2, 5);
		if (visible(1, 5, 2))
			drawVisiblePlane(4, 1, 5, 2, 3);
		center(6, 10);
		if (visible(9, 8, 7))
			drawVisiblePlane(5, 9, 8, 7);
		if (visible(7, 8, 11))
			drawVisiblePlane(6, 7, 8, 11, 10);
		if (visible(7, 9, 12))
			drawVisiblePlane(7, 7, 9, 12, 10);
		if (visible(8, 9, 12))
			drawVisiblePlane(8, 8, 9, 12, 11);
	}
	else
	{
		center(6, 10);
		if (visible(9, 8, 7))
			drawVisiblePlane(5, 9, 8, 7);
		if (visible(7, 8, 11))
			drawVisiblePlane(6, 7, 8, 11, 10);
		if (visible(7, 9, 12))
			drawVisiblePlane(7, 7, 9, 12, 10);
		if (visible(8, 9, 12))
			drawVisiblePlane(8, 8, 9, 12, 11);
		center(0, 6);
		if (visible(0, 1, 5))
			drawVisiblePlane(0, 0, 1, 5);
		if (visible(3, 4, 2))
			drawVisiblePlane(1, 3, 4, 2);
		if (visible(1, 0, 4))
			drawVisiblePlane(2, 1, 0, 4, 3);
		if (visible(0, 4, 2))
			drawVisiblePlane(3, 0, 4, 2, 5);
		if (visible(1, 5, 2))
			drawVisiblePlane(4, 1, 5, 2, 3);
	}
}
bool RobertsAlgorithm::visible(int point1, int point2, int point3)
{
	int	delta12[3] =
	{
		x[point1] - x[point2],
		y[point1] - y[point2],
		z[point1] - z[point2]
	},
		delta32[3] =
	{
		x[point3] - x[point2],
		y[point3] - y[point2],
		z[point3] - z[point2]
	};

	int A = delta12[1] * delta32[2] - delta32[1] * delta12[2];
	int B = delta12[2] * delta32[0] - delta32[2] * delta12[0];
	int C = delta12[0] * delta32[1] - delta32[0] * delta12[1];
	int D = -(A * x[point2] + B * y[point2] + C * z[point2]);

	int sign = (A * xCenter + B * yCenter + C * zCenter + D < 0) ? -1 : 1;

	A *= sign;
	B *= sign;
	C *= sign;
	D *= sign;

	return (A * xObserver + B * yObserver + C * zObserver + D > 0);
}

void RobertsAlgorithm::drawVisiblePlane(int num, int point1, int point2, int point3, int point4)
{
	int coordinates1[N][4] =
	{
		{x[point1], y[point1], z[point1], 1},
		{x[point2], y[point2], z[point2], 1},
		{x[point3], y[point3], z[point3], 1},
		{x[point4], y[point4], z[point4], 1}
	};

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			transformationMatrix[i][j] = 0;

	/* косоугольная */
	int a = 45;
	transformationMatrix[0][0] = 1;
	transformationMatrix[1][1] = 1;
	transformationMatrix[3][3] = 1;
	transformationMatrix[0][2] = (float)-cos(a * M_PI / 180);
	transformationMatrix[1][2] = (float)-sin(a * M_PI / 180);

	float buf[4], buf1[4];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			buf1[j] = 0;

		std::copy_n(coordinates1[i], 4, buf);

		for (int k = 0; k < 4; k++)
			for (int t = 0; t < 4; t++)
				buf1[k] += buf[t] * transformationMatrix[t][k];

		coordinates1[i][0] = (int)buf1[0];
		coordinates1[i][1] = (int)buf1[1];
	}

	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, colors[num]);

	int x = 0, y = 0, count = 0;

	MoveToEx(memDeviceContext, coordinates1[0][0], coordinates1[0][1], NULL);

	x += coordinates1[0][0]; y += coordinates1[0][1]; count++;
	LineTo(memDeviceContext, coordinates1[1][0], coordinates1[1][1]);
	x += coordinates1[1][0]; y += coordinates1[1][1]; count++;
	LineTo(memDeviceContext, coordinates1[2][0], coordinates1[2][1]);
	x += coordinates1[2][0]; y += coordinates1[2][1]; count++;

	if (point4)
	{
		LineTo(memDeviceContext, coordinates1[3][0], coordinates1[3][1]);
		x += coordinates1[3][0]; y += coordinates1[3][1]; count++;
	}

	LineTo(memDeviceContext, coordinates1[0][0], coordinates1[0][1]);

	x /= count; y /= count;

	paintXY(x, y, colors[num]);
	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, RGB(0, 0, 0));
	MoveToEx(memDeviceContext, coordinates1[0][0], coordinates1[0][1], NULL);

	LineTo(memDeviceContext, coordinates1[1][0], coordinates1[1][1]);
	LineTo(memDeviceContext, coordinates1[2][0], coordinates1[2][1]);
	if (point4)	LineTo(memDeviceContext, coordinates1[3][0], coordinates1[3][1]);
	LineTo(memDeviceContext, coordinates1[0][0], coordinates1[0][1]);
}

void RobertsAlgorithm::paintXY(int x, int y, COLORREF color)
{
	std::stack<std::pair<int, int>> nodes;

	nodes.push({ x,y });

	while (nodes.size() > 0)
	{
		std::pair<int, int> currentNode = nodes.top();

		nodes.pop();

		if (GetPixel(memDeviceContext, currentNode.first, currentNode.second) != color
			&& currentNode.first < 1000 && currentNode.second < 500
			&& currentNode.first > 0 && currentNode.second > 0)
		{
			SetPixel(memDeviceContext, currentNode.first, currentNode.second, color);
			nodes.push({ currentNode.first + 1, currentNode.second });
			nodes.push({ currentNode.first - 1, currentNode.second });
			nodes.push({ currentNode.first, currentNode.second + 1 });
			nodes.push({ currentNode.first, currentNode.second - 1 });
		}
	}

	return;
}

void RobertsAlgorithm::fillBlock(int i1, int i2, int coordinates1[][4], COLORREF color)
{
	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, color);

	int xMax = coordinates1[i1][0], xMin = coordinates1[i1][0];
	int yMax = coordinates1[i1][1], yMin = coordinates1[i1][1];

	for (int i = i1; i < i2; i++)
	{
		xMax = max(xMax, coordinates1[i][0]);

		xMin = min(xMin, coordinates1[i][0]);

		yMax = max(yMax, coordinates1[i][1]);

		yMin = min(yMin, coordinates1[i][1]);
	}

	bool flag1 = false, flag2 = false;

	for (int y = yMin; y <= yMax; y++)
	{
		int x1 = xMin;
		int x2 = xMax;

		while (!flag1 || !flag2)
		{
			if (!flag1)
				if (GetPixel(memDeviceContext, x1, y) == RGB(255, 255, 255))
					flag1 = true;
				else
					x1++;

			if (!flag2)
				if (GetPixel(memDeviceContext, x2, y) == RGB(255, 255, 255))
					flag2 = true;
				else
					x2--;

			if (x1 == x2 || x1 > xMax || x2 < xMin)
				break;
		}

		MoveToEx(memDeviceContext, x1, y, NULL);
		LineTo(memDeviceContext, x2 + 1, y);
		flag1 = false;
		flag2 = false;
	}

	SelectObject(memDeviceContext, GetStockObject(DC_PEN));
	SetDCPenColor(memDeviceContext, RGB(255, 255, 255));
}