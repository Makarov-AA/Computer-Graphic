#include <QImage>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Filters.h"

int main(int argc, char *argv[])
{
	QImage test_picture;
	std::string path;
	for (int i = 0; i < argc; i++)
		if (!strcmp(argv[i], "-i") && (i + 1 < argc))
			path = argv[i + 1];
	if (test_picture.load(QString(path.c_str())))
		std::cout << "Load success!" << std::endl;
	else
		std::cout << "Load failed:(" << std::endl;
	if (test_picture.save("Results\\New_picture.jpg"))
		std::cout << "Save success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;

	/*InvertFilter* invert = new InvertFilter();
	QImage inverted_picture = invert->calculateNewImagePixMap(test_picture);
	if (inverted_picture.save("Results\\Inverted_picture.jpg"))
		std::cout << "Invertion success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete invert;

	GrayScaleFilter* bw_filter = new GrayScaleFilter();
	QImage bw_picture = bw_filter->calculateNewImagePixMap(test_picture);
	if (bw_picture.save("Results\\BW_picture.jpg"))
		std::cout << "Black and white success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete bw_filter;

	SepiaFilter* sepia_filter = new SepiaFilter();
	QImage sepia_picture = sepia_filter->calculateNewImagePixMap(test_picture);
	if (sepia_picture.save("Results\\Sepia_picture.jpg"))
		std::cout << "Sepia success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete sepia_filter;

	IncreaseIntensityFilter* increase_filter = new IncreaseIntensityFilter();
	QImage increased_picture = increase_filter->calculateNewImagePixMap(test_picture);
	if (increased_picture.save("Results\\Increased_picture.jpg"))
		std::cout << "Increase success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete increase_filter;

	MedianFilter* median_filter = new MedianFilter();
	QImage median_picture = median_filter->calculateNewImagePixMap(test_picture);
	if (median_picture.save("Results\\Median_picture.jpg"))
		std::cout << "Median success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete median_filter;

	Scaling* scale_filter = new Scaling();
	QImage scale_picture = scale_filter->calculateNewImagePixMap(test_picture);
	if (scale_picture.save("Results\\Scaled_picture.jpg"))
		std::cout << "Scale success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete scale_filter;

	Rotation* rotate_filter = new Rotation(-1117.3);
	QImage rotate_picture = rotate_filter->calculateNewImagePixMap(test_picture);
	if (rotate_picture.save("Results\\Rotated_picture.jpg"))
		std::cout << "Rotate success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete rotate_filter;

	BlurFilter* blur = new BlurFilter();
	QImage blured_picture = blur->calculateNewImagePixMap(test_picture);
	if (blured_picture.save("Results\\Blured_picture.jpg"))
		std::cout << "Blur success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete blur;

	GaussianBlurFilter* gaus_filter = new GaussianBlurFilter();
	QImage gaussian_blured_picture = gaus_filter->calculateNewImagePixMap(test_picture);
	if (gaussian_blured_picture.save("Results\\Gaus_blured_picture.jpg"))
		std::cout << "Gaus blur success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete gaus_filter;

	SobelFilter* sobel_filter = new SobelFilter();
	QImage sobel_picture = sobel_filter->calculateNewImagePixMap(test_picture);
	if (sobel_picture.save("Results\\Sobel_picture.jpg"))
		std::cout << "Sobel success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete sobel_filter;

	ContrastIncreaseFilter* contrast_increase_filter = new ContrastIncreaseFilter();
	QImage contrast_increase_picture = contrast_increase_filter->calculateNewImagePixMap(test_picture);
	if (contrast_increase_picture.save("Results\\Contrast_increase_picture.jpg"))
		std::cout << "Contrast increase success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete contrast_increase_filter;

	Mask mask("Resources\\mask.txt");

	Dilation* dilation_filter = new Dilation(mask);
	QImage dilation_picture = dilation_filter->calculateNewImagePixMap(test_picture);
	if (median_picture.save("Results\\Dilation_picture.jpg"))
		std::cout << "Dilation success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete dilation_filter;

	Erosion* erosion_filter = new Erosion(mask);
	QImage erosion_picture = erosion_filter->calculateNewImagePixMap(test_picture);
	if (erosion_picture.save("Results\\Erosion_picture.jpg"))
		std::cout << "Erosion success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete erosion_filter;

	Opening* opening_filter = new Opening(mask);
	QImage opening_picture = opening_filter->calculateNewImagePixMap(test_picture);
	if (opening_picture.save("Results\\Opening_picture.jpg"))
		std::cout << "Opening success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete opening_filter;

	Closing* closing_filter = new Closing(mask);
	QImage closing_picture = closing_filter->calculateNewImagePixMap(test_picture);
	if (closing_picture.save("Results\\Closing_picture.jpg"))
		std::cout << "Closing success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete closing_filter;

	Grad* grad_filter = new Grad(mask);
	QImage grad_picture = grad_filter->calculateNewImagePixMap(test_picture);
	if (grad_picture.save("Results\\Grad_picture.jpg"))
		std::cout << "Grad success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete grad_filter;

	TopHat* top_hat_filter = new TopHat(mask);
	QImage top_hat_picture = top_hat_filter->calculateNewImagePixMap(test_picture);
	if (top_hat_picture.save("Results\\TopHat_picture.jpg"))
		std::cout << "TopHat success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete top_hat_filter;

	BlackHat* black_hat_filter = new BlackHat(mask);
	QImage black_hat_picture = black_hat_filter->calculateNewImagePixMap(test_picture);
	if (black_hat_picture.save("Results\\BlackHat_picture.jpg"))
		std::cout << "BlackHat success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete black_hat_filter;

	MotionBlur* motion_blur_filter = new MotionBlur();
	QImage motion_blur_picture = motion_blur_filter->calculateNewImagePixMap(test_picture);
	if (motion_blur_picture.save("Results\\MotionBlur_picture.jpg"))
		std::cout << "MotionBlur success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete motion_blur_filter;

	Sharpness* sharpness_filter = new Sharpness();
	QImage sharpness_picture = sharpness_filter->calculateNewImagePixMap(test_picture);
	if (sharpness_picture.save("Results\\Sharpness_picture.jpg"))
		std::cout << "Sharpness success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete sharpness_filter;*/

	WaveFilter* wave_filter = new WaveFilter();
	QImage wave_picture = wave_filter->calculateNewImagePixMap(test_picture);
	if (wave_picture.save("Results\\Wave_picture.jpg"))
		std::cout << "Wave success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete wave_filter;
	
	GlassFilter* glass_filter = new GlassFilter();
	QImage glass_picture = glass_filter->calculateNewImagePixMap(test_picture);
	if (glass_picture.save("Results\\Glass_picture.jpg"))
		std::cout << "Glass success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete glass_filter;

	GrayWorld* gray_world_filter = new GrayWorld();
	QImage gray_world_picture = gray_world_filter->calculateNewImagePixMap(test_picture);
	if (gray_world_picture.save("Results\\GrayWorld_picture.jpg"))
		std::cout << "GrayWorld success!" << std::endl;
	else
		std::cout << "Save failed:(" << std::endl;
	delete gray_world_filter;

	return 0;
}
