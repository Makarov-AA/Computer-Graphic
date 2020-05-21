#pragma once
#include <QtOpengl>
#include <QGLWidget>
#include <QImage>
#include <algorithm>
#include <qgl.h>

#include "Data.h"

#define MIN_WINDOW_WIDTH 200
#define MIN_WINDOW_HEIGHT 200

enum Visualisation { VISUALIZATION_QUADS, VISUALIZATION_QUADSTRIP, VISUALIZATION_TEXTURE };

enum Axis {X, Y, Z};

class View : public QGLWidget {
	Q_OBJECT

private:
	Visualisation visualization_state;
	int layer;
	GLuint VBOtexture;
	QImage textureImage;
	Data data;

	int min, max;
	int TFMin, TFMax;
	int dataWidth, dataHeight, dataDepth;

	Axis axis;

	void VisualizationQuads();
	void VizualizationTexture();
	void VisualizationQuadStrip();

	QColor TransferFunction(short value);

	void Load2DTexture();
	void genTextureImage();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	//void Up();
	//void Down();

public slots:
	void changeLayer(int newValue);
	void changeVisMod(const QString &text);
	void setTFMaxValue(int newValue);
	void setTFMinValue(int newValue);
	void setAxis(const QString& text);

public:
	View(QWidget* parent = nullptr);

	void loadData(QString& filename);

	int getMin();
	int getMax();
	int getTFMin();
	int getTFMax();
	int getWidth();
	int getHeight();
	int getDepth();
	Axis getAxis();

//public slots: 
	//void View::keyPressEvent(QKeyEvent* event) override;
};
