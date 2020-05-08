#pragma once

#include "Filters.h"



template <class T>
T clamp(T value, T min, T max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;
	return value;
}

QImage InvertFilter::calculateNewImagePixMap(const QImage& picture)
{
	QImage result(picture);

	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor color = picture.pixelColor(i, j);
			color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
			result.setPixelColor(i, j, color);
		}
	return result;
}

QImage GrayScaleFilter::calculateNewImagePixMap(const QImage& picture)
{
	QImage result(picture);
	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor grey_color = picture.pixelColor(i, j);
			float intensity = ((float)grey_color.red() * RED_INTENSITY_SCALE + 
				(float)grey_color.green() * GREEN_INTENSITY_SCALE +
				(float)grey_color.blue() * BLUE_INTENSITY_SCALE);
			grey_color.setRgb((int)intensity, (int)intensity, (int)intensity);
			result.setPixelColor(i, j, grey_color);
		}
	return result;
}

QImage SepiaFilter::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor yellow_color = picture.pixelColor(i, j);
			yellow_color.setRgb(clamp<int>(yellow_color.red() + SEPIA_SCALE * 2, 0, 255),
				clamp<int>(yellow_color.green() + SEPIA_SCALE / 2, 0, 255),
				clamp<int>(yellow_color.blue() - SEPIA_SCALE, 0, 255));
			result.setPixelColor(i, j, yellow_color);
		}
	return result;
}

QImage IncreaseIntensityFilter::calculateNewImagePixMap(const QImage& picture)
{
	QImage result(picture);
	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor increased_color = picture.pixelColor(i, j);
			increased_color.setRgb(clamp<int>(increased_color.red() + INCREASE_INTENSITY_CONST, 0, 255),
				clamp<int>(increased_color.green() + INCREASE_INTENSITY_CONST, 0, 255),
				clamp<int>(increased_color.blue() + INCREASE_INTENSITY_CONST, 0, 255));
			result.setPixelColor(i, j, increased_color);
		}
	return result;
}

QImage MedianFilter::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	int diam = 2 * radius + 1;
	int size = diam * diam;
	float* vector = new float[size];
	int* pos = new int[size];
	QColor* colors = new QColor[size];
	for (int y = 0; y < picture.height(); y++)
		for (int x = 0; x < picture.width(); x++)
		{
			for (int i = -radius; i <= radius; i++)
				for (int j = -radius; j <= radius; j++)
				{
					QColor color = picture.pixelColor(clamp<int>(x + j, 0, picture.width() - 1),
						clamp<int>(y + i, 0, picture.height() - 1));
					float intensity = ((float)color.red() * RED_INTENSITY_SCALE +
						(float)color.green() * GREEN_INTENSITY_SCALE +
						(float)color.blue() * BLUE_INTENSITY_SCALE);
					int idx = (i + radius) * diam + j + radius;
					colors[idx] = color;
					vector[idx] = intensity;
					pos[idx] = idx;
				}
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size - i - 1; j++)
					if (vector[j] > vector[j + 1])
					{
						float tmp = vector[j];
						vector[j] = vector[j + 1];
						vector[j + 1] = tmp;
						int tmp_idx = pos[j];
						pos[j] = pos[j + 1];
						pos[j + 1] = tmp_idx;
					}
			QColor median_color = colors[pos[size / 2]];
			result.setPixelColor(x, y, median_color);
		}
	return result;
}

QImage Scaling::calculateNewImagePixMap(const QImage& picture) {
	int result_width = (int)((float)picture.width() * x_scale);
	int result_height = (int)((float)picture.height() * y_scale);
	QImage result(result_width, result_height, QImage::Format_RGB32);
	for (int y = 0; y < result.height(); y++)
		for (int x = 0; x < result.width(); x++)
		{
			float src_x_value = clamp<float>((float)x / x_scale, 0.0, (float)(picture.width() - 2));
			int int_x_value = (int)src_x_value;
			float fract_x = src_x_value - (float)int_x_value;
			float src_y_value = clamp<float>((float)y / y_scale, 0.0, (float)(picture.height() - 2));
			int int_y_value = (int)src_y_value;
			float fract_y = src_y_value - (float)int_y_value;
			QColor color11 = picture.pixelColor(int_x_value, int_y_value);
			QColor color12 = picture.pixelColor(int_x_value + 1, int_y_value);
			QColor color21 = picture.pixelColor(int_x_value, int_y_value + 1);
			QColor color22 = picture.pixelColor(int_x_value + 1, int_y_value + 1);
			int result_red = (int)((float)color11.red() * (1. - fract_x) * (1. - fract_y) +
				(float)color12.red() * fract_x * (1 - fract_y) +
				(float)color21.red() * (1 - fract_x) * fract_y +
				(float)color12.red() * fract_x * fract_y);
			int result_green = (int)((float)color11.green() * (1. - fract_x) * (1. - fract_y) +
				(float)color12.green() * fract_x * (1 - fract_y) +
				(float)color21.green() * (1 - fract_x) * fract_y +
				(float)color12.green() * fract_x * fract_y);
			int result_blue = (int)((float)color11.blue() * (1. - fract_x) * (1. - fract_y) +
				(float)color12.blue() * fract_x * (1 - fract_y) +
				(float)color21.blue() * (1 - fract_x) * fract_y +
				(float)color12.blue() * fract_x * fract_y);
			QColor color;
			color.setRgb(result_red, result_green, result_blue);
			result.setPixelColor(x, y, color);
		}
	return result;
}

QImage Rotation::calculateNewImagePixMap(const QImage& picture) {
	float top1_new_x = -(float)picture.width() / 2. * cos(angle) + (float)picture.height() / 2. * sin(angle);
	float top1_new_y = -(float)picture.height() / 2. * cos(angle) - (float)picture.width() / 2. * sin(angle);
	float top2_new_x = (float)picture.width() / 2. * cos(angle) + (float)picture.height() / 2. * sin(angle);
	float top2_new_y = -(float)picture.height() / 2. * cos(angle) + (float)picture.width() / 2. * sin(angle);
	int result_width = (abs(top1_new_x) > abs(top2_new_x)) ? (int)abs(top1_new_x * 2.0) : (int)abs(top2_new_x * 2.0);
	int result_height = (abs(top1_new_y) > abs(top2_new_y)) ? (int)abs(top1_new_y * 2.0) : (int)abs(top2_new_y * 2.0);
	QImage result(result_width, result_height, QImage::Format_RGB32);
	for (int y = -result_height / 2; y <= result_height / 2; y++)
		for (int x = -result_width / 2; x <= result_width / 2; x++)
		{
			float old_x = x * cos(angle) + y * sin(angle) + (float)(picture.width() / 2);
			float old_y = -x * sin(angle) + y * cos(angle) + (float)(picture.height() / 2);
			QColor result_color;
			if (old_x < 0. || old_x > (float)picture.width() ||
				old_y < 0. || old_y >(float)picture.height())
				result_color.setRgb(0, 0, 0);
			else
			{
				int int_x_value = (int)old_x;
				float fract_x = old_x - (float)int_x_value;
				int int_y_value = (int)old_y;
				float fract_y = old_y - (float)int_y_value;
				QColor color11 = picture.pixelColor(clamp<int>(int_x_value, 0, picture.width() - 1),
					clamp<int>(int_y_value, 0, picture.height() - 1));
				QColor color12 = picture.pixelColor(clamp<int>(int_x_value + 1, 0, picture.width() - 1),
					clamp<int>(int_y_value, 0, picture.height() - 1));
				QColor color21 = picture.pixelColor(clamp<int>(int_x_value, 0, picture.width() - 1),
					clamp<int>(int_y_value + 1, 0, picture.height() - 1));
				QColor color22 = picture.pixelColor(clamp<int>(int_x_value + 1, 0, picture.width() - 1),
					clamp<int>(int_y_value + 1, 0, picture.height() - 1));
				int result_red = (int)((float)color11.red() * (1. - fract_x) * (1. - fract_y) +
					(float)color12.red() * fract_x * (1. - fract_y) +
					(float)color21.red() * (1. - fract_x) * fract_y +
					(float)color12.red() * fract_x * fract_y);
				int result_green = (int)((float)color11.green() * (1. - fract_x) * (1. - fract_y) +
					(float)color12.green() * fract_x * (1. - fract_y) +
					(float)color21.green() * (1. - fract_x) * fract_y +
					(float)color12.green() * fract_x * fract_y);
				int result_blue = (int)((float)color11.blue() * (1. - fract_x) * (1. - fract_y) +
					(float)color12.blue() * fract_x * (1. - fract_y) +
					(float)color21.blue() * (1. - fract_x) * fract_y +
					(float)color12.blue() * fract_x * fract_y);
				result_color.setRgb(clamp<int>(result_red, 0, 255),
					clamp<int>(result_green, 0, 255),
					clamp<int>(result_blue, 0, 255));
				//result_color = picture.pixelColor(int_x_value, int_y_value);
				result.setPixelColor(clamp<int>(x + (result_width / 2), 0, result.width() - 1),
					clamp<int>(y + (result_height / 2), 0, result.height() - 1), result_color);
			}
		}
	return result;
}

QImage WaveFilter::calculateNewImagePixMap(const QImage& picture){
	QImage result(picture);
	for(int i = 0; i < result.height(); i++)
		for (int j = 0; j < result.width(); j++)
		{
			int src_x = clamp<int>((int)((float)j + 20 * sin(2. * M_PI * (float)j / 30.)), 0, picture.width() - 1);
			int src_y = i;
			QColor src_color = picture.pixelColor(src_x, src_y);
			result.setPixelColor(j, i, src_color);
		}
	return result;
}

QImage GlassFilter::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	for (int i = 0; i < result.height(); i++)
		for (int j = 0; j < result.width(); j++)
		{
			int src_x = clamp<int>((int)((float)j + ((float)(std::rand() % 2) - 0.5) * 10.), 0, picture.width() - 1);
			int src_y = clamp<int>((int)((float)i + ((float)(std::rand() % 2) - 0.5) * 10.), 0, picture.height() - 1);
			QColor src_color = picture.pixelColor(src_x, src_y);
			result.setPixelColor(j, i, src_color);
		}
	return result;
}

QImage GrayWorld::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	double sum_red = 0.;
	double sum_green = 0.;
	double sum_blue = 0.;
	for (int i = 0; i < picture.height(); i++)
		for (int j = 0; j < picture.width(); j++)
		{
			QColor src_color = picture.pixelColor(j, i);
			sum_red += (double)src_color.red();
			sum_green += (double)src_color.green();
			sum_blue += (double)src_color.blue();
		}
	double average_red = sum_red / (double)(picture.height() * picture.width());
	double average_green = sum_green / (double)(picture.height() * picture.width());
	double average_blue = sum_blue / (double)(picture.height() * picture.width());
	double average = (average_red + average_green + average_blue) / 3.;
	double red_scale = average / average_red;
	double green_scale = average / average_green;
	double blue_scale = average / average_blue;
	for (int i = 0; i < picture.height(); i++)
		for (int j = 0; j < picture.width(); j++)
		{
			QColor src_color = picture.pixelColor(j, i);
			int new_red = clamp<int>((int)((double)src_color.red() * red_scale), 0, 255);
			int new_green = clamp((int)((double)src_color.green() * green_scale), 0, 255);
			int new_blue = clamp((int)((double)src_color.blue() * blue_scale), 0, 255);
			QColor new_color(new_red, new_green, new_blue);
			result.setPixelColor(j, i, new_color);
		}
	return result;
}

QColor MatrixFilter::calculateNewPixelColor(const QImage& picture, int x, int y) {
	float result_red = 0;
	float result_green = 0;
	float result_blue = 0;
	int diam = m_radius * 2 + 1;
	for (int i = - m_radius; i <= m_radius; i++)
		for (int j = -m_radius; j <= m_radius; j++) {
			int idx = (i + m_radius) * diam + j + m_radius;
			QColor color = picture.pixelColor(clamp<int>(x + j, 0, picture.width() - 1),
				clamp<int>(y + i, 0, picture.height() - 1));
			result_red += (float)color.red() * vector[idx];
			result_green += (float)color.green() * vector[idx];
			result_blue += (float)color.blue() * vector[idx];
		}
	return QColor(clamp<int>((int)result_red, 0, 255),
		clamp<int>((int)result_green, 0, 255),
		clamp<int>((int)result_blue, 0, 255));
}

QImage MatrixFilter::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor color = this->calculateNewPixelColor(picture, i, j);
			result.setPixelColor(i, j, color);
		}
	return result;
}

void GaussianBlurFilter::createGaussianVector() {
	const unsigned int size = 2 * m_radius + 1;
	vector = new float[size * size];
	float norm = 0;

	for (int i = -m_radius; i <= m_radius; i++)
		for (int j = -m_radius; j <= m_radius; j++) {
			int idx = (i + m_radius) * size + (j + m_radius);
			vector[idx] = exp(-(float)(i * i + j * j) / (float)(2 * sigma * sigma));
			norm += vector[idx];
		}
	for (int i = -m_radius; i <= m_radius; i++)
		for (int j = -m_radius; j <= m_radius; j++)
		{
			int idx = (i + m_radius) * size + (j + m_radius);
			vector[idx] /= norm;
		}
	return;
}

QColor SobelFilter::calculateNewPixelColor(const QImage& picture, int x, int y)
{
	float x_result_red = 0;
	float x_result_green = 0;
	float x_result_blue = 0;
	int diam = m_radius * 2 + 1;
	for (int i = -m_radius; i <= m_radius; i++)
		for (int j = -m_radius; j <= m_radius; j++) {
			int idx = (i + m_radius) * diam + j + m_radius;
			QColor color = picture.pixelColor(clamp<int>(x + j, 0, picture.width() - 1),
				clamp<int>(y + i, 0, picture.height() - 1));
			x_result_red += (float)color.red() * vector[idx];
			x_result_green += (float)color.green() * vector[idx];
			x_result_blue += (float)color.blue() * vector[idx];
		}
	float y_result_red = 0;
	float y_result_green = 0;
	float y_result_blue = 0;
	for (int i = -m_radius; i <= m_radius; i++)
		for (int j = -m_radius; j <= m_radius; j++) {
			int idx = (j + m_radius) * diam + i + m_radius;
			QColor color = picture.pixelColor(clamp<int>(x + i, 0, picture.width() - 1),
				clamp<int>(y + j, 0, picture.height() - 1));
			y_result_red += (float)color.red() * vector[idx];
			y_result_green += (float)color.green() * vector[idx];
			y_result_blue += (float)color.blue() * vector[idx];
		}
	int result_red = (int)sqrt(x_result_red * x_result_red + y_result_red * y_result_red);
	int result_green = (int)sqrt(x_result_green * x_result_green + y_result_green * y_result_green);
	int result_blue = (int)sqrt(x_result_blue * x_result_blue + y_result_blue * y_result_blue);
	QColor gradient(clamp<int>(result_red, 0, 255),
		clamp<int>(result_green, 0, 255),
		clamp<int>(result_blue, 0, 255));
	return gradient;
}

QImage SobelFilter::calculateNewImagePixMap(const QImage& picture){
	QImage result(picture);
	for (int i = 0; i < picture.width(); i++)
		for (int j = 0; j < picture.height(); j++)
		{
			QColor color = calculateNewPixelColor(picture, i, j);
			result.setPixelColor(i, j, color);
		}
	return result;
}

Mask::Mask(const std::string& filename){
	std::ifstream F;
	F.open(filename);
	if (F)
	{
		F >> mask_width >> mask_height;
		vector = new int[mask_width * mask_height];
		for (int i = 0; i < mask_width * mask_height; i++)
		{
			if (F.eof()) throw "Incorrect file";
			F >> vector[i];
		}
		F.close();
	}
	else
		throw "There is no such a file";
}

Mask::Mask(const Mask& copy){
	mask_width = copy.mask_width;
	mask_height = copy.mask_height;
	vector = new int[mask_width * mask_height];
	for (int i = 0; i < mask_width * mask_height; i++)
		vector[i] = copy.vector[i];
}

QImage Dilation::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	int size = mask.mask_height * mask.mask_width;
	for (int y = 0; y < picture.height(); y++)
		for (int x = 0; x < picture.width(); x++)
		{
			QColor max_color(0, 0, 0);
			float max_intensity = 0.;
			for (int i = 0; i < mask.mask_height; i++)
				for (int j = 0; j < mask.mask_width; j++)
				{
					if (mask.vector[i * mask.mask_width + j])
					{
						QColor color = picture.pixelColor(clamp<int>(x + (j - mask.mask_width / 2), 0, picture.width() - 1),
							clamp<int>(y + (i - mask.mask_height / 2), 0, picture.height() - 1));
						float intensity = ((float)color.red() * RED_INTENSITY_SCALE +
							(float)color.green() * GREEN_INTENSITY_SCALE +
							(float)color.blue() * BLUE_INTENSITY_SCALE);
						if (intensity > max_intensity)
						{
							max_color = color;
							max_intensity = intensity;
						}
					}
				}
			result.setPixelColor(x, y, max_color);
		}
	return result;
}

QImage Erosion::calculateNewImagePixMap(const QImage& picture) {
	QImage result(picture);
	int size = mask.mask_height * mask.mask_width;
	for (int y = 0; y < picture.height(); y++)
		for (int x = 0; x < picture.width(); x++)
		{
			QColor min_color(255, 255, 255);
			float min_intensity = 255.;
			for (int i = 0; i < mask.mask_height; i++)
				for (int j = 0; j < mask.mask_width; j++)
				{
					if (mask.vector[i * mask.mask_width + j])
					{
						QColor color = picture.pixelColor(clamp<int>(x + (j - mask.mask_width / 2), 0, picture.width() - 1),
							clamp<int>(y + (i - mask.mask_height / 2), 0, picture.height() - 1));
						float intensity = ((float)color.red() * RED_INTENSITY_SCALE +
							(float)color.green() * GREEN_INTENSITY_SCALE +
							(float)color.blue() * BLUE_INTENSITY_SCALE);
						if (intensity < min_intensity)
						{
							min_color = color;
							min_intensity = intensity;
						}
					}
				}
			result.setPixelColor(x, y, min_color);
		}
	return result;
}

QImage Opening::calculateNewImagePixMap(const QImage& picture) {
	Erosion erosion_filter(mask);
	QImage result = erosion_filter.calculateNewImagePixMap(picture);
	Dilation dilation_filter(mask);
	return dilation_filter.calculateNewImagePixMap(result);
}

QImage Closing::calculateNewImagePixMap(const QImage& picture){
	Dilation dilation_filter(mask);
	QImage result = dilation_filter.calculateNewImagePixMap(picture);
	Erosion erosion_filter(mask);
	return erosion_filter.calculateNewImagePixMap(result);
}

QImage Grad::calculateNewImagePixMap(const QImage& picture) {
	Dilation dilation_filter(mask);
	QImage dilated_picture = dilation_filter.calculateNewImagePixMap(picture);
	Erosion erosion_filter(mask);
	QImage erosion_picture = erosion_filter.calculateNewImagePixMap(picture);
	QImage result(picture);
	for (int i = 0; i < picture.height(); i++)
		for (int j = 0; j < picture.width(); j++)
		{
			QColor dilated_color = dilated_picture.pixelColor(j, i);
			QColor erosion_color = erosion_picture.pixelColor(j, i);
			int new_red = clamp<int>(dilated_color.red() - erosion_color.red(), 0, 255);
			int new_green = clamp<int>(dilated_color.green() - erosion_color.green(), 0, 255);
			int new_blue = clamp<int>(dilated_color.blue() - erosion_color.blue(), 0, 255);
			QColor new_color(new_red, new_green, new_blue);
			result.setPixelColor(j, i, new_color);
		}
	return result;
}

QImage TopHat::calculateNewImagePixMap(const QImage& picture){
	Opening opening(mask);
	QImage opened_picture = opening.calculateNewImagePixMap(picture);
	QImage result(picture);
	for (int i = 0; i < picture.height(); i++)
		for (int j = 0; j < picture.width(); j++)
		{
			QColor src_color = picture.pixelColor(j, i);
			QColor opened_color = opened_picture.pixelColor(j, i);
			int new_red = clamp<int>(src_color.red() - opened_color.red(), 0, 255);
			int new_green = clamp<int>(src_color.green() - opened_color.green(), 0, 255);
			int new_blue = clamp<int>(src_color.blue() - opened_color.blue(), 0, 255);
			QColor new_color(new_red, new_green, new_blue);
			result.setPixelColor(j, i, new_color);
		}
	return result;
}

QImage BlackHat::calculateNewImagePixMap(const QImage& picture) {
	Closing closing(mask);
	QImage closed_picture = closing.calculateNewImagePixMap(picture);
	QImage result(picture);
	for (int i = 0; i < picture.height(); i++)
		for (int j = 0; j < picture.width(); j++)
		{
			QColor src_color = picture.pixelColor(j, i);
			QColor closed_color = closed_picture.pixelColor(j, i);
			int new_red = clamp<int>(closed_color.red() - src_color.red(), 0, 255);
			int new_green = clamp<int>(closed_color.green() - src_color.green(), 0, 255);
			int new_blue = clamp<int>(closed_color.blue() - src_color.blue(), 0, 255);
			QColor new_color(new_red, new_green, new_blue);
			result.setPixelColor(j, i, new_color);
		}
	return result;
}
