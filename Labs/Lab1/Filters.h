#pragma once

#define _USE_MATH_DEFINES
#define RED_INTENSITY_SCALE 0.36
#define GREEN_INTENSITY_SCALE 0.53
#define BLUE_INTENSITY_SCALE 0.11
#define SEPIA_SCALE 60
#define INCREASE_INTENSITY_CONST 30

#include <QImage>
#include <math.h>
#include <iostream>
#include <fstream>



class Filter {
public:
	Filter() {}
	~Filter() {}

	virtual QImage calculateNewImagePixMap(const QImage& picture) = 0;
};

class InvertFilter : public Filter {
protected:
	int filter_radius;
public:
	InvertFilter(int radius = 1) : filter_radius(radius) {}
	~InvertFilter() {}

	QImage calculateNewImagePixMap(const QImage& picture);
};

class GrayScaleFilter : public Filter {
public:
	GrayScaleFilter() {};
	~GrayScaleFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class SepiaFilter : public Filter {
public:
	SepiaFilter() {};
	~SepiaFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class IncreaseIntensityFilter : public Filter {
public:
	IncreaseIntensityFilter() {};
	~IncreaseIntensityFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class MedianFilter : public Filter {
protected:
	int radius;
	float* vector;
public:
	MedianFilter(int i_radius = 1) : radius(i_radius) {};
	~MedianFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Scaling : public Filter {
protected:
	float x_scale;
	float y_scale;
public:
	Scaling(float i_x_scale = 2.0, float i_y_scale = 2.0) : x_scale(i_x_scale), y_scale(i_y_scale) {};
	~Scaling() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Rotation : public Filter
{
protected:
	float angle;
public:
	Rotation(float degrees = 90.) {
		angle = degrees / 180 * M_PI;
	};
	~Rotation() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class WaveFilter : public Filter {
public:
	WaveFilter() {};
	~WaveFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class GlassFilter : public Filter {
public:
	GlassFilter() {};
	~GlassFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class GrayWorld : public Filter {
public:
	GrayWorld() {};
	~GrayWorld() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class MatrixFilter : public Filter {
protected:
	float* vector;
	int m_radius;
	QColor calculateNewPixelColor(const QImage& picture, int x, int y);
public:
	MatrixFilter(int radius = 1, float* vector = nullptr) : m_radius(radius) {}
	~MatrixFilter() { 
		if (vector != nullptr) delete[] vector;
	}
	QImage calculateNewImagePixMap(const QImage& picture);
};

class BlurFilter : public MatrixFilter {
public:
	BlurFilter(int radius = 1) : MatrixFilter(radius) {
		int size = 2 * m_radius + 1;
		vector = new float[size * size];
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				vector[i * size + j] = 1.0f / (float)(size * size);
	}
	~BlurFilter() {};
};

class GaussianBlurFilter : public MatrixFilter {
protected:
	int sigma;
public:
	GaussianBlurFilter(int radius = 3, int gaus_sigma = 2) : MatrixFilter(radius), sigma(gaus_sigma) {
		createGaussianVector();
	}
	void createGaussianVector();
};

class SobelFilter : public MatrixFilter {
protected:
	QColor calculateNewPixelColor(const QImage& picture, int x, int y);
public:
	SobelFilter() : MatrixFilter(1) {
		vector = new float[9];
		vector[0] = -1.0; vector[1] = -2.0; vector[2] = -1.0;
		vector[3] = 0.0; vector[4] = 0.0; vector[5] = 0.0;
		vector[6] = 1.0; vector[7] = 2.0; vector[8] = 1.0;
	};
	~SobelFilter() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class ContrastIncreaseFilter : public MatrixFilter {
public:
	ContrastIncreaseFilter() : MatrixFilter(1) {
		vector = new float[9];
		vector[0] = 0.; vector[1] = -1.; vector[2] = 0.;
		vector[3] = -1.; vector[4] = 5.; vector[5] = -1.;
		vector[6] = 0.; vector[7] = -1.; vector[8] = 0.;
	}
	~ContrastIncreaseFilter() {};
};

class MotionBlur : public MatrixFilter {
public:
	MotionBlur(int radius = 1) : MatrixFilter(radius) {
		int diam = radius * 2 + 1;
		vector = new float[diam * diam];
		for (int i = 0; i < diam; i++)
			for (int j = 0; j < diam; j++)
				if (i == j)
					vector[i * diam + j] = 1. / (float)diam;
				else
					vector[i * diam + j] = 0.;
	}
	~MotionBlur() { };
};

class Sharpness : public MatrixFilter {
public:
	Sharpness() : MatrixFilter(1) {
		vector = new float[9];
		vector[0] = -1.; vector[1] = -1.; vector[2] = -1.;
		vector[3] = -1.; vector[4] = 9.; vector[5] = -1.;
		vector[6] = -1.; vector[7] = -1.; vector[8] = -1.;
	}
	~Sharpness() {};
};

class Mask {
public:
	int* vector;
	int mask_width;
	int mask_height;
	Mask(const std::string& filename);
	Mask(const Mask& copy);
	~Mask() {
		delete[] vector;
	}
};

class MorphOperation : public Filter {
protected:
	Mask mask;
public:
	MorphOperation(Mask i_mask) : mask(i_mask) {};
	~MorphOperation(){};
};

class Dilation : public MorphOperation {
public:
	Dilation(Mask i_mask) : MorphOperation(i_mask){};
	~Dilation() { };

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Erosion : public MorphOperation {
public:
	Erosion(Mask i_mask) : MorphOperation(i_mask){};
	~Erosion() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Opening : public MorphOperation {
public:
	Opening(Mask i_mask) : MorphOperation(i_mask) {};
	~Opening() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Closing : public MorphOperation {
public:
	Closing(Mask i_mask) : MorphOperation(i_mask) {};
	~Closing() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class Grad : public MorphOperation {
public:
	Grad(Mask i_mask) : MorphOperation(i_mask) {};
	~Grad() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class TopHat : public MorphOperation {
public:
	TopHat(Mask i_mask) : MorphOperation(i_mask) {};
	~TopHat() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

class BlackHat : public MorphOperation {
public:
	BlackHat(Mask i_mask) : MorphOperation(i_mask) {};
	~BlackHat() {};

	QImage calculateNewImagePixMap(const QImage& picture);
};

