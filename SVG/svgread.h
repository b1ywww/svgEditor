#pragma once
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

class Shape;
class SvgRead
{
public:
	static SvgRead* svgRead();

	bool read(QString dir, QList<Shape*>& shapelist, int& width, int& height);

	Shape* readLine(QXmlStreamAttributes& attributes);

	Shape* readSquare(QXmlStreamAttributes& attributes);

	Shape* readPencil(QXmlStreamAttributes& attributes);

	Shape* readCircle(QXmlStreamAttributes& attributes);

	Shape* readHexagon(QXmlStreamAttributes& attributes);

	Shape* readText(QXmlStreamAttributes& attributes, QXmlStreamReader& reader);

	void readCanvas(QXmlStreamReader& reader);
private:
	SvgRead();
	~SvgRead();

	QPointF m_transform;
};