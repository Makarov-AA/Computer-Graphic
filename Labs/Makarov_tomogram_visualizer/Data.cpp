#include "Data.h"

#include <fstream>

short Data::getMin() {
	return min;
}

short Data::getMax() {
	return max;
}

int Data::getWidth() {
	return x;
}

int Data::getHeight() {
	return y;
}

int Data::getDepth() {
	return z;
}

int Data::readFile(QString& filename) {
	std::ifstream b_file;
	b_file.open(filename.toStdString(), std::ios::binary);
	if (b_file.is_open())
	{
		b_file.read((char*)&x, sizeof(int));
		b_file.read((char*)&y, sizeof(int));
		b_file.read((char*)&z, sizeof(int));
		b_file.read((char*)&xDst, sizeof(float));
		b_file.read((char*)&yDst, sizeof(float));
		b_file.read((char*)&zDst, sizeof(float));
		long size = x * y * z;
		values = new short[size];
		min = SHRT_MAX;
		max = SHRT_MIN;
		for (long i = 0; i < size; i++)
		{
			b_file.read((char*)(values + i), sizeof(short));
			if (values[i] > max) max = values[i];
			if (values[i] < min) min = values[i];
		}
		b_file.close();
	}
	else
	{
		b_file.close();
		throw "Incorrect data path";
	}
}

short Data::operator[](const int idx){
	return values[idx];
}