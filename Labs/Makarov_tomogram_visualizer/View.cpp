#include "View.h"

template <typename T>
T clamp(T v, T min, T max) {
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

void View::VisualizationQuads() {
	QColor c;
	if (axis == X)
		for (int z = 0; z < (dataDepth - 1); z++)
			for (int y = 0; y < dataHeight - 1; y++)
			{
				glBegin(GL_QUADS);
				c = TransferFunction(data[z * dataWidth * dataHeight + y * dataWidth + layer]);
				qglColor(c);
				glVertex2i(y, z);

				c = TransferFunction(data[(z + 1) * dataWidth * dataHeight + y * dataWidth + layer]);
				qglColor(c);
				glVertex2i(y, (z + 1));

				c = TransferFunction(data[(z + 1) * dataWidth * dataHeight + (y + 1) * dataWidth + layer]);
				qglColor(c);
				glVertex2i((y + 1), (z + 1));

				c = TransferFunction(data[z * dataWidth * dataHeight + (y + 1) * dataWidth + layer]);
				qglColor(c);
				glVertex2i((y + 1), z);
			}
	else if (axis == Y)
		for (int z = 0; z < (dataDepth - 1); z++)
			for (int x = 0; x < dataWidth - 1; x++)
			{
				glBegin(GL_QUADS);
				c = TransferFunction(data[z * dataWidth * dataHeight + layer * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, z);

				c = TransferFunction(data[(z + 1) * dataWidth * dataHeight + layer * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, (z + 1));

				c = TransferFunction(data[(z + 1) * dataWidth * dataHeight + layer * dataWidth + (x + 1)]);
				qglColor(c);
				glVertex2i((x + 1), (z + 1));

				c = TransferFunction(data[z * dataWidth * dataHeight + layer * dataWidth + (x + 1)]);
				qglColor(c);
				glVertex2i((x + 1), z);
			}
	else if (axis == Z)
		for (int y = 0; y < (dataHeight - 1); y++)
			for (int x = 0; x < (dataWidth - 1); x++)
			{
				glBegin(GL_QUADS);
				c = TransferFunction(data[layer * dataWidth * dataHeight + y * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, y);

				c = TransferFunction(data[layer * dataWidth * dataHeight + (y + 1) * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, (y + 1));

				c = TransferFunction(data[layer * dataWidth * dataHeight + (y + 1) * dataWidth + (x + 1)]);
				qglColor(c);
				glVertex2i((x + 1), (y + 1));

				c = TransferFunction(data[layer * dataWidth * dataHeight + y * dataWidth + (x + 1)]);
				qglColor(c);
				glVertex2i((x + 1), y);
				glEnd();
			}
}

void View::VizualizationTexture() {
	glBegin(GL_QUADS);
	if (axis == X) {
		glTexCoord2f(0, 0);
		glVertex2i(0, 0);

		glTexCoord2f(0, 1);
		glVertex2i(0, dataDepth - 1);

		glTexCoord2f(1, 1);
		glVertex2i(dataHeight - 1, dataDepth - 1);

		glTexCoord2f(1, 0);
		glVertex2i(dataHeight - 1, 0);
	}
	else if (axis == Y) {
		glTexCoord2f(0, 0);
		glVertex2i(0, 0);

		glTexCoord2f(0, 1);
		glVertex2i(0, dataDepth - 1);

		glTexCoord2f(1, 1);
		glVertex2i(dataWidth - 1, dataDepth - 1);

		glTexCoord2f(1, 0);
		glVertex2i(dataWidth - 1, 0);
	}
	else if (axis == Z) {
		glTexCoord2f(0, 0);
		glVertex2i(0, 0);

		glTexCoord2f(0, 1);
		glVertex2i(0, dataHeight - 1);

		glTexCoord2f(1, 1);
		glVertex2i(dataWidth - 1, dataHeight - 1);

		glTexCoord2f(1, 0);
		glVertex2i(dataWidth - 1, 0);
	}

	glEnd();
}

void View::VisualizationQuadStrip() {
	QColor c;
	if (axis == X)
		for (int z = 0; z < (dataDepth - 1); z++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int y = 0; y < (dataHeight - 1); y++)
			{

				c = TransferFunction(data[z * dataWidth * dataHeight + y * dataWidth + layer]);
				qglColor(c);
				glVertex2i(y, z);

				c = TransferFunction(data[z * dataWidth * dataHeight + y * dataWidth + layer]);
				qglColor(c);
				glVertex2i(y, (z + 1));
			}
			glEnd();
		}
	else if (axis == Y)
		for (int z = 0; z < (dataDepth - 1); z++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int x = 0; x < (dataWidth - 1); x++)
			{

				c = TransferFunction(data[z * dataWidth * dataHeight + layer * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, z);

				c = TransferFunction(data[z * dataWidth * dataHeight + layer * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, (z + 1));
			}
			glEnd();
		}
	else if (axis == Z)
		for (int y = 0; y < (dataHeight - 1); y++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int x = 0; x < (dataWidth - 1); x++)
			{

				c = TransferFunction(data[layer * dataWidth * dataHeight + y * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, y);

				c = TransferFunction(data[layer * dataWidth * dataHeight + (y + 1) * dataWidth + x]);
				qglColor(c);
				glVertex2i(x, (y + 1));
			}
			glEnd();
		}
}

QColor View::TransferFunction(short value) {
	int c = clamp<int>((value - TFMin) * 255 / (TFMax - TFMin), 0, 255);
	return QColor(c, c, c);
}

void View::Load2DTexture() {
	glBindTexture(GL_TEXTURE_2D, VBOtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
		0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void View::genTextureImage() {
	if (axis == X) {
		textureImage = QImage(dataHeight, dataDepth, QImage::Format_RGB32);
		for (int z = 0; z < dataDepth; z++)
			for (int y = 0; y < dataHeight; y++) {
				QColor c = TransferFunction(data[z * dataWidth * dataHeight + dataWidth * y + layer]);
				textureImage.setPixelColor(y, z, c);
			}
	}
	else if (axis == Y) {
		textureImage = QImage(dataWidth, dataDepth, QImage::Format_RGB32);
		for (int z = 0; z < dataDepth; z++)
			for (int x = 0; x < dataWidth; x++) {
				QColor c = TransferFunction(data[z * dataWidth * dataHeight + dataWidth * layer + x]);
				textureImage.setPixelColor(x, z, c);
			}
	}
	else if (axis == Z) {
		textureImage = QImage(dataWidth, dataHeight, QImage::Format_RGB32);
		for (int y = 0; y < dataHeight; y++)
			for (int x = 0; x < dataWidth; x++){
				QColor c = TransferFunction(data[layer * dataWidth * dataHeight + dataWidth * y + x]);
				textureImage.setPixelColor(x, y, c);
			}
	}
}

void View::initializeGL() {
	qglClearColor(Qt::white);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGenTextures(1, &VBOtexture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void View::resizeGL(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (axis == X)
		glOrtho(0.0f, dataHeight - 1, 0.0f, dataDepth - 1, -1.0f, 1.0f);
	else if (axis == Y)
		glOrtho(0.0f, dataWidth - 1, 0.0f, dataDepth - 1, -1.0f, 1.0f);
	else if (axis == Z)
		glOrtho(0.0f, dataWidth - 1, 0.0f, dataHeight - 1, -1.0f, 1.0f);
	glViewport(0, 0, w, h);
	update();
}

void View::paintGL() {
	qDebug() << "repaint" << visualization_state;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (visualization_state)
	{
	case VISUALIZATION_QUADS:
		VisualizationQuads();
		break;
	case VISUALIZATION_QUADSTRIP:
		VisualizationQuadStrip();
		break;
	case VISUALIZATION_TEXTURE:
		VizualizationTexture();
		break;
	}
}

View::View(QWidget* parent) : QGLWidget(parent) {
	layer = 0;
	axis = Z;
	visualization_state = VISUALIZATION_QUADS;
}

void View::loadData(QString& filename) {
	data.readFile(filename);
	layer = 0;
	dataWidth = data.getWidth();
	dataHeight = data.getHeight();
	dataDepth = data.getDepth();
	TFMax = max = data.getMax();
	TFMin = min = data.getMin();
	/*if (axis == X)
		resize(clamp<int>(dataHeight, 100, QApplication::desktop()->screenGeometry().dataWidth()),
			clamp<int>(dataDepth, 100, QApplication::desktop()->screenGeometry().dataHeight()));
	else if (axis == Y)
		resize(clamp<int>(dataWidth, 100, QApplication::desktop()->screenGeometry().dataWidth()),
			clamp<int>(dataDepth, 100, QApplication::desktop()->screenGeometry().dataHeight()));
	else if (axis == Z)
		resize(clamp<int>(dataWidth, 100, QApplication::desktop()->screenGeometry().dataWidth()),
			clamp<int>(dataHeight, 100, QApplication::desktop()->screenGeometry().dataHeight()));*/
	update();
}

int View::getMin() {
	return min;
}

int View::getMax() {
	return max;
}

int View::getTFMin() {
	return TFMin;
}

int View::getTFMax() {
	return TFMax;
}

int View::getWidth() {
	return dataWidth;
}

int View::getHeight() {
	return dataHeight;
}

int View::getDepth() {
	return dataDepth;
}

Axis View::getAxis() {
	return axis;
}

void View::changeLayer(int newValue) {
	if (axis == X)
		if (newValue >= 0 && newValue < dataWidth) {
			layer = newValue;
			if (visualization_state == VISUALIZATION_TEXTURE) {
				genTextureImage();
				Load2DTexture();
			}
			updateGL();
		}
	if (axis == Y)
		if (newValue >= 0 && newValue < dataHeight) {
			layer = newValue;
			if (visualization_state == VISUALIZATION_TEXTURE) {
				genTextureImage();
				Load2DTexture();
			}
			updateGL();
		}
	if (axis == Z)
		if (newValue >= 0 && newValue < dataDepth) {
			layer = newValue;
			if (visualization_state == VISUALIZATION_TEXTURE) {
				genTextureImage();
				Load2DTexture();
			}
			updateGL();
		}
}

void View::changeVisMod(const QString &text) {
	if (visualization_state == VISUALIZATION_TEXTURE)
		glDisable(GL_TEXTURE_2D);
	if (text == QString("Quads"))
		visualization_state = VISUALIZATION_QUADS;
	else if (text == QString("QuadStrip"))
		visualization_state = VISUALIZATION_QUADSTRIP;
	else if (text == QString("Texture")) {
		visualization_state = VISUALIZATION_TEXTURE;
		glEnable(GL_TEXTURE_2D);
		genTextureImage();
		Load2DTexture();
	}
	update();
}

void View::setTFMaxValue(int newValue) {
	if (newValue > max || newValue < TFMin + 1) return;
	TFMax = newValue;
	if (visualization_state == VISUALIZATION_TEXTURE) {
		genTextureImage();
		Load2DTexture();
	}
	update();
}

void View::setTFMinValue(int newValue) {
	if (newValue > TFMax -1 || newValue < min) return;
	TFMin = newValue;
	if (visualization_state == VISUALIZATION_TEXTURE) {
		genTextureImage();
		Load2DTexture();
	}
	update();
}

void View::setAxis(const QString& text) {
	layer = 0;
	if (text == "X")
		axis = X;
	else if (text == "Y")
		axis = Y;
	else if (text == "Z")
		axis = Z;
	resizeGL(width(), height());
	if (visualization_state == VISUALIZATION_TEXTURE) {
		genTextureImage();
		Load2DTexture();
	}
}