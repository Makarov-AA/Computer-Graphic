#include <QApplication>

#include "Visualizer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Visualizer visualizer(nullptr);
	visualizer.resize(1000, 700);
	visualizer.show();
	return a.exec();
}
