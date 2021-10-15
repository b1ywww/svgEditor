#pragma once
#include <QCoreApplication>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QList>

class Shape;
class SvgWrite
{
public:
	static SvgWrite* svgWrite();

	bool setFile(QString fileDir);

	void writeHead();

	void writeBackGround(int canvasW, int canvasH);

	void writeShape(QList<Shape*> list);

	bool write(QList<Shape*> list, QString fileDir, int canvasW, int canvasH);

private:

	void writeLine(Shape* shape);
	void writeSquare(Shape* shape);
	void writePencil(Shape* shape);
	void writeCircle(Shape* shape);
	void writeHexagon(Shape* shape);
	void writeText(Shape* shape);

	SvgWrite(){}

	QFile m_file;
	QDomDocument m_doc;
	QDomElement m_svg;
	QDomElement m_shapeG;
	QPointF m_transfrom;
};