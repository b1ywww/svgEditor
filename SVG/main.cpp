#include "kxsvgview.h"
#include <QtWidgets/QApplication>

// 内存泄漏检测工具
// #include "vld.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	SVGMainWIndow w;
	w.show();
	return a.exec();
}