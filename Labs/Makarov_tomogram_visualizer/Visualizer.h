#pragma once

#include "View.h"

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>

class Visualizer : public QWidget{
	Q_OBJECT

private:
	QSlider* slider;
	View* viewPort;
	QLabel* minLayer;
	QLabel* maxLayer;
	QSpinBox* TFmin;
	QSpinBox* TFmax;

private slots:
	void loadNewData();
	void changeTFMax(int newValue);
	void changeTFMin(int newValue);
	void axisChange(const QString& text);

public:
	Visualizer(QWidget* parent);
};
