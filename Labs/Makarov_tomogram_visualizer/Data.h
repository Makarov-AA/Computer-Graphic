#pragma once
#include <string>
#include <QString>

class Data {
private:
	int x, y, z; //sizes
	float xDst, yDst, zDst;
	short* values;
	short min, max;

public:
	short getMin();
	short getMax();
	
	int getWidth();
	int getHeight();
	int getDepth();

	int readFile(QString& filename);
	short operator[](const int idx);
};