#include "Visualizer.h"

Visualizer::Visualizer(QWidget* parent) : QWidget(parent) {
	QHBoxLayout * mainGrid = new QHBoxLayout(this);
	QGridLayout * interafaceGrid = new QGridLayout(this);//Основная сетка
	//Окно вывода
	viewPort = new View(this);
	viewPort->loadData(QString("Resources/testdata.bin"));
	viewPort->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//Слайдер для прохода по слоям
	slider = new QSlider(Qt::Vertical, this);
	slider->setRange(0, viewPort->getDepth() - 1);
	slider->setValue(0);
	slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	connect(slider, &QSlider::valueChanged, viewPort, &View::changeLayer);
	//Диапазон слайдера
	minLayer = new QLabel("0", this);
	//minLayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	maxLayer = new QLabel(QString::number(viewPort->getDepth() - 1), this);
	//maxLayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//Меню выбора метода визуализации
	QComboBox* visMod = new QComboBox(this);
	//visMod->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QStringList mods = { "Quads", "QuadStrip", "Texture" };
	visMod->addItems(mods);
	connect(visMod, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		viewPort, &View::changeVisMod);
	//Кнопка для загрузки новых данных
	QPushButton* openBtn = new QPushButton("Open...", this);
	//openBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(openBtn, &QPushButton::clicked, this, &Visualizer::loadNewData);
	//Кнопка выхода
	QPushButton* quitBtn = new QPushButton("Quit", this);
	//quitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
	
	//Строки для изменения максимума и минимума TF
	QLabel* maxLabel = new QLabel("max", this);
	//maxLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	TFmax = new QSpinBox(this);
	TFmax->setRange(viewPort->getMin() + 1, viewPort->getMax());
	//TFmax->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	TFmax->setSingleStep(100);
	TFmax->setValue(viewPort->getMax());
	connect(TFmax, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Visualizer::changeTFMax);

	QLabel* minLabel = new QLabel("min", this);
	TFmin = new QSpinBox(this);
	//TFmin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	TFmin->setRange(viewPort->getMin(), viewPort->getMax() - 1);
	TFmin->setSingleStep(100);
	TFmin->setValue(viewPort->getMin());
	connect(TFmin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Visualizer::changeTFMin);

	QComboBox* setAxis = new QComboBox(this);
	//setAxis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QStringList axises = { "Z", "Y", "X" };
	setAxis->addItems(axises);
	connect(setAxis, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		this, &Visualizer::axisChange);

	mainGrid->addWidget(viewPort);
	interafaceGrid->addWidget(maxLayer, 0, 0);
	interafaceGrid->addWidget(slider, 1, 0, 3, 1);
	interafaceGrid->addWidget(minLayer, 4, 0);
	interafaceGrid->addWidget(visMod, 0, 1, 1, 2);
	interafaceGrid->addWidget(minLabel, 1, 1);
	interafaceGrid->addWidget(TFmin, 1, 2);
	interafaceGrid->addWidget(maxLabel, 2, 1);
	interafaceGrid->addWidget(TFmax, 2, 2);
	interafaceGrid->addWidget(setAxis, 3, 1, 1, 2);
	interafaceGrid->addWidget(quitBtn, 4, 2);
	interafaceGrid->addWidget(openBtn, 4, 1);

	mainGrid->addLayout(interafaceGrid);

	setLayout(mainGrid);
}

void Visualizer::loadNewData() {
	QString filename = QFileDialog::getOpenFileName(this, "Open file", QString(), "BIN files (*.bin)");
	if (filename != QString()) {
		viewPort->loadData(filename);
		Axis axis = viewPort->getAxis();
		if (axis == X)
		{
			slider->setValue(0);
			slider->setRange(0, viewPort->getWidth() - 1);
			maxLayer->setNum(viewPort->getWidth() - 1);
		}
		else if (axis == Y)
		{
			slider->setValue(0);
			slider->setRange(0, viewPort->getHeight() - 1);
			maxLayer->setNum(viewPort->getHeight() - 1);
		}
		else if (axis == Z)
		{
			slider->setValue(0);
			slider->setRange(0, viewPort->getDepth() - 1);
			maxLayer->setNum(viewPort->getDepth() - 1);
		}
		TFmin->setRange(viewPort->getMin(), viewPort->getMax() - 1);
		TFmin->setValue(viewPort->getMin());
		TFmax->setRange(viewPort->getMin() + 1, viewPort->getMax());
		TFmax->setValue(viewPort->getMax());
	}
}

void Visualizer::changeTFMax(int newValue) {
	viewPort->setTFMaxValue(newValue);
	TFmin->setRange(viewPort->getMin(), viewPort->getTFMax() - 1);
}

void Visualizer::changeTFMin(int newValue) {
	viewPort->setTFMinValue(newValue);
	TFmax->setRange(viewPort->getTFMin() + 1, viewPort->getMax());
}

void Visualizer::axisChange(const QString& text) {
	viewPort->setAxis(text);
	Axis axis = viewPort->getAxis();
	if (axis == X)
	{
		slider->setRange(0, viewPort->getWidth() - 1);
		slider->setValue(0);
		maxLayer->setNum(viewPort->getWidth() - 1);
	}
	else if (axis == Y)
	{
		slider->setRange(0, viewPort->getHeight() - 1);
		slider->setValue(0);
		maxLayer->setNum(viewPort->getHeight() - 1);
	}
	else if (axis == Z)
	{
		slider->setRange(0, viewPort->getDepth() - 1);
		slider->setValue(0);
		maxLayer->setNum(viewPort->getDepth() - 1);
	}
}