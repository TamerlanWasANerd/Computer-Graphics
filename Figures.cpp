#include <Windows.h>
#include "figures.h"
Figures::Figures()
{
	x[0] = 550; y[0] = 250; z[0] = -35;
	x[1] = 630; y[1] = 140; z[1] = 50;
	x[2] = 510; y[2] = 180; z[2] = 120;
	x[3] = 590; y[3] = 180; z[3] = 150;
	x[4] = 510; y[4] = 280; z[4] = 80;
	x[5] = 540; y[5] = 140; z[5] = 20;
	x[6] = 570; y[6] = 210; z[6] = 70;
	x[7] = 510; y[7] = 240; z[7] = -50;
	x[8] = 500; y[8] = 140; z[8] = 10;
	x[9] = 470; y[9] = 290; z[9] = 60;
	intersection(0, 2, 4, 6, 7, 10);
	intersection(0, 2, 4, 6, 8, 11);
	intersection(0, 2, 4, 6, 9, 12);
	clearTransformationMatrix();
	center();

	HWND handle = GetConsoleWindow();
	HDC deviceContext = GetDC(handle);
	memDeviceContext = CreateCompatibleDC(deviceContext);
	memBitmap = CreateCompatibleBitmap(deviceContext, 1200, 900);
	SelectObject(memDeviceContext, memBitmap);
}

Figures::~Figures()
{
	DeleteDC(memDeviceContext);
	DeleteObject(memBitmap);
}

void Figures::transformationMatrixMultiplication()
{
	for (int i = 0; i < N; i++)
	{
		float buf1[4] = { 0.0f };

		float buf[4] = { (float)x[i], (float)y[i], (float)z[i], 1 };

		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				buf1[j] += buf[k] * transformationMatrix[k][j];

		x[i] = (int)buf1[0];
		y[i] = (int)buf1[1];
		z[i] = (int)buf1[2];
	}
}

void Figures::center()
{
	xCenter = 0;
	yCenter = 0;
	zCenter = 0;

	for (int i = 0; i < N; i++)
	{
		xCenter += x[i];
		yCenter += y[i];
		zCenter += z[i];
	}

	xCenter /= N;
	yCenter /= N;
	zCenter /= N;
}

void Figures::center(int index1, int index2)
{
	xCenter = 0;
	yCenter = 0;
	zCenter = 0;

	for (int i = index1; i < index2; i++)
	{
		xCenter += x[i];
		yCenter += y[i];
		zCenter += z[i];
	}

	xCenter /= (index2 - index1);
	yCenter /= (index2 - index1);
	zCenter /= (index2 - index1);
}

void Figures::clearTransformationMatrix()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			transformationMatrix[i][j] = (i == j) ? 1 : 0;
}

void Figures::intersection(int planePoint1, int planePoint2, int planePoint3,
	int linePoint1, int linePoint2, int resultIndex)
{
	int delta21[3]=
	{
		x[planePoint2] - x[planePoint1],
		y[planePoint2] - y[planePoint1],
		z[planePoint2] - z[planePoint1]
	}, 
		delta31[3] = 
	{
		x[planePoint3] - x[planePoint1],
		y[planePoint3] - y[planePoint1],
		z[planePoint3] - z[planePoint1]
	};


	int A = delta21[1] * delta31[2] - delta31[1] * delta21[2];
	int B = delta21[2] * delta31[0] - delta31[2] * delta21[0];
	int C = delta21[0] * delta31[1] - delta31[0] * delta21[1];
	int D = -(A * x[planePoint1] + B * y[planePoint1] + C * z[planePoint1]);

	int m = x[linePoint2] - x[linePoint1];
	int n = y[linePoint2] - y[linePoint1];
	int p = z[linePoint2] - z[linePoint1];

	float t = (float)(-A * x[linePoint1] - B * y[linePoint1] - C * z[linePoint1] - D) / (A * m + B * n + C * p);

	x[resultIndex] = m * t + x[linePoint1];
	y[resultIndex] = n * t + y[linePoint1];
	z[resultIndex] = p * t + z[linePoint1];
}

void Figures::move(arguments coordinate, arguments sign)
{
	clearTransformationMatrix();

	float increment = (sign == PLUS) ? 25 : -25;

	transformationMatrix[3][coordinate] = increment;

	transformationMatrixMultiplication();
}

void Figures::scale(arguments sign)
{
	clearTransformationMatrix();

	center();

	float scaleCoefficient = (sign == PLUS) ? 1.1f : 0.9f;

	transformationMatrix[0][0] = transformationMatrix[1][1] = transformationMatrix[2][2] = scaleCoefficient;

	transformationMatrix[3][0] = xCenter * (1 - scaleCoefficient);
	transformationMatrix[3][1] = yCenter * (1 - scaleCoefficient);
	transformationMatrix[3][2] = zCenter * (1 - scaleCoefficient);

	transformationMatrixMultiplication();
}

void Figures::rotate(arguments coordinate, arguments sign)
{
	clearTransformationMatrix();
	center();
	int fi = (sign == PLUS) ? 20 : -20;

	switch (coordinate)
	case X:
	{
		transformationMatrix[1][1] = transformationMatrix[2][2] = (float)cos(fi * M_PI / 180);
		transformationMatrix[1][2] = (float)sin(fi * M_PI / 180);
		transformationMatrix[2][1] = (float)-sin(fi * M_PI / 180);
		transformationMatrix[3][1] = yCenter * (1 - (float)cos(fi * M_PI / 180)) + zCenter * (float)sin(fi * M_PI / 180);
		transformationMatrix[3][2] = zCenter * (1 - (float)cos(fi * M_PI / 180)) - yCenter * (float)sin(fi * M_PI / 180);

		break;
	case Y:
		transformationMatrix[0][0] = transformationMatrix[2][2] = (float)cos(fi * M_PI / 180);
		transformationMatrix[0][2] = (float)-sin(fi * M_PI / 180);
		transformationMatrix[2][0] = (float)sin(fi * M_PI / 180);
		transformationMatrix[3][0] = xCenter * (1 - (float)cos(fi * M_PI / 180)) - zCenter * (float)sin(fi * M_PI / 180);
		transformationMatrix[3][2] = zCenter * (1 - (float)cos(fi * M_PI / 180)) + xCenter * (float)sin(fi * M_PI / 180);

		break;
	case Z:
		transformationMatrix[0][0] = transformationMatrix[1][1] = (float)cos(fi * M_PI / 180);
		transformationMatrix[0][1] = (float)sin(fi * M_PI / 180);
		transformationMatrix[1][0] = (float)-sin(fi * M_PI / 180);
		transformationMatrix[3][0] = xCenter * (1 - (float)cos(fi * M_PI / 180)) + yCenter * (float)sin(fi * M_PI / 180);
		transformationMatrix[3][1] = yCenter * (1 - (float)cos(fi * M_PI / 180)) - xCenter * (float)sin(fi * M_PI / 180);

		break;
	}

	transformationMatrixMultiplication();
}
