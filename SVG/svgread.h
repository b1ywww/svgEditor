#pragma once
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QRgb>

class Shape;
class SvgRead
{
public:
	static SvgRead* svgRead();

	bool read(QString dir, QList<Shape*>& shapelist, int& width, int& height, QRgb& rgb);

	Shape* readLine(QXmlStreamAttributes& attributes);

	Shape* readSquare(QXmlStreamAttributes& attributes);

	Shape* readPencil(QXmlStreamAttributes& attributes);

	Shape* readCircle(QXmlStreamAttributes& attributes);

	Shape* readHexagon(QXmlStreamAttributes& attributes);

	Shape* readText(QXmlStreamAttributes& attributes, QXmlStreamReader& reader);

	QColor readCanvas(QXmlStreamReader& reader);
private:
	SvgRead();
	~SvgRead();

	QPointF m_transform;
};