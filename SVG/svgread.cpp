#include "svgread.h"
#include "shape.h"
#include <QFontMetrics>

SvgRead* SvgRead::svgRead()
{
	static SvgRead* pSvgRead = nullptr;
	if (nullptr == pSvgRead)
		pSvgRead = new SvgRead();

	return pSvgRead;
}

bool SvgRead::read(QString dir, QList<Shape*>& shapelist, int& width, int& height, QRgb& rgb)
{
	if (dir.isEmpty())
		return false;

	if (shapelist.isEmpty())
		return false;

	QFile file(dir);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QXmlStreamReader reader(&file);
	QXmlStreamAttributes attributes;
	reader.readNext();
	reader.readNext();
	qDebug() << reader.name();
	//判断是否是自己保存的xml
	if (reader.name() == "svg")
	{
		attributes = reader.attributes();
		if (attributes.hasAttribute("type") && attributes.value("type").toString() != QString("wps"))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	//读取画布设置
	reader.readNext();
	reader.readNext();
	reader.readNext();
	reader.readNext();
	qDebug() << reader.name();
	if (reader.name() == "title")
	{
		if (reader.readElementText() != "background")
			return false;
	}
	else
	{
		return false;
	}

	QColor c = readCanvas(reader);
	rgb = qRgb(c.red(), c.green(), c.blue());
	//读取图像
	reader.readNext();
	while (!reader.atEnd())
	{
		attributes = reader.attributes();
		if (attributes.value("type") == "line")
			shapelist << readLine(attributes);

		if (attributes.value("type") == "rect")
			shapelist << readSquare(attributes);

		if (attributes.value("type") == "circle")
			shapelist << readCircle(attributes);

		if (attributes.value("type") == "hexagon")
			shapelist << readHexagon(attributes);

		if (attributes.value("type") == "pencil")
			shapelist << readPencil(attributes);

		if (attributes.value("type") == "text")
			shapelist << readText(attributes, reader);
		reader.readNext();
	}

	width = m_transform.x() * 2;
	height = m_transform.y() * 2;
	return true;
}

Shape* SvgRead::readLine(QXmlStreamAttributes& attributes)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeLine);
	i->setDrawStart(QPointF(attributes.value("x1").toString().toDouble(), attributes.value("y1").toString().toDouble()) - m_transform);
	i->setDrawEnd(QPointF(attributes.value("x2").toString().toDouble(), attributes.value("y2").toString().toDouble()) - m_transform);
	i->drawPointToPhysicalPoint(0);
	i->getPen().setColor(attributes.value("stroke").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setWidthF(attributes.value("stroke-width").toString().toDouble());

	QString strokeStyle = attributes.value("stroke-dasharray").toString();
	if (strokeStyle == "null")
		i->getPen().setStyle(Qt::SolidLine);
	else if (strokeStyle == "2,2")
		i->getPen().setStyle(Qt::DotLine);
	else if (strokeStyle == "5,2,2,2")
		i->getPen().setStyle(Qt::DashDotLine);
	else if (strokeStyle == "5,2,2,2,2,2")
		i->getPen().setStyle(Qt::DashDotDotLine);

	return i;
}

Shape* SvgRead::readSquare(QXmlStreamAttributes& attributes)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeSquare);
	QPointF star = QPointF(attributes.value("x").toString().toDouble(), attributes.value("y").toString().toDouble());
	qreal width = attributes.value("width").toString().toDouble();
	qreal height = attributes.value("height").toString().toDouble();
	i->setDrawStart(star - m_transform);
	i->setDrawEnd(QPointF(star.x() + width, star.y() + height) - m_transform);
	i->drawPointToPhysicalPoint(0);
	i->getBrush().setColor(attributes.value("fill").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setColor(attributes.value("stroke").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setWidthF(attributes.value("stroke-width").toString().toDouble());

	QString strokeStyle = attributes.value("stroke-dasharray").toString();
	if (strokeStyle == "null")
		i->getPen().setStyle(Qt::SolidLine);
	else if (strokeStyle == "2,2")
		i->getPen().setStyle(Qt::DotLine);
	else if (strokeStyle == "5,2,2,2")
		i->getPen().setStyle(Qt::DashDotLine);
	else if (strokeStyle == "5,2,2,2,2,2")
		i->getPen().setStyle(Qt::DashDotDotLine);
	return i;
}

Shape* SvgRead::readPencil(QXmlStreamAttributes& attributes)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypePencil);
	QString d = attributes.value("d").toString();

	QStringList list = d.split(",");
	qDebug() << list;
	QList<QPointF> point;

	qreal top = 10000;
	qreal bottom = -10000;
	qreal left = 10000;
	qreal right = -10000;

	auto k = list.begin();

	qreal x = (*k).split(" ")[0].replace("M", "").toDouble();
	qreal y = (*k).split(" ")[1].toDouble();
	point.append(QPointF(x, y) - m_transform);

	for (k++; k != list.end(); k++)
	{
		qDebug() << *k;
		qDebug() << (*k).split(" ")[0].replace("L", "");
		x = (*k).split(" ")[0].replace("L", "").toDouble();
		y = (*k).split(" ")[1].toDouble();
		point.append(QPointF(x, y) - m_transform);
		if (x < left)
			left = x;
		else if (x > right)
			right = x;

		if (y < top)
			top = y;
		else if (y > bottom)
			bottom = y;
	}
	i->setDrawStart(QPointF(left, top) - m_transform);
	i->setDrawEnd(QPointF(right, bottom) - m_transform);
	Pencil* pencil = dynamic_cast<Pencil*>(i);
	if(pencil)
	{
		pencil->setDrawPoint(point);
		pencil->drawPointToPhysicalPoint(0);
	}

	i->getPen().setColor(attributes.value("stroke").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setWidthF(attributes.value("stroke-width").toString().toDouble());

	QString strokeStyle = attributes.value("stroke-dasharray").toString();
	if (strokeStyle == "null")
		i->getPen().setStyle(Qt::SolidLine);
	else if (strokeStyle == "2,2")
		i->getPen().setStyle(Qt::DotLine);
	else if (strokeStyle == "5,2,2,2")
		i->getPen().setStyle(Qt::DashDotLine);
	else if (strokeStyle == "5,2,2,2,2,2")
		i->getPen().setStyle(Qt::DashDotDotLine);
	return i;
}

Shape* SvgRead::readCircle(QXmlStreamAttributes& attributes)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeCircle);
	qreal cx = attributes.value("cx").toString().toDouble() - m_transform.x();
	qreal cy = attributes.value("cy").toString().toDouble() - m_transform.y();
	qreal width = attributes.value("rx").toString().toDouble();
	qreal height = attributes.value("ry").toString().toDouble();
	
	i->setDrawStart(QPointF(cx - width, cy - height));
	i->setDrawEnd(QPointF(cx + width, cy + height));
	i->drawPointToPhysicalPoint(0);

	i->getBrush().setColor(attributes.value("fill").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setColor(attributes.value("stroke").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setWidthF(attributes.value("stroke-width").toString().toDouble());

	QString strokeStyle = attributes.value("stroke-dasharray").toString();
	if (strokeStyle == "null")
		i->getPen().setStyle(Qt::SolidLine);
	else if (strokeStyle == "2,2")
		i->getPen().setStyle(Qt::DotLine);
	else if (strokeStyle == "5,2,2,2")
		i->getPen().setStyle(Qt::DashDotLine);
	else if (strokeStyle == "5,2,2,2,2,2")
		i->getPen().setStyle(Qt::DashDotDotLine);
	return i;
}

Shape* SvgRead::readHexagon(QXmlStreamAttributes& attributes)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeHexagon);
	QString d = attributes.value("d").toString();

	QStringList list = d.split(",");
	qDebug() << list;
	QVector<QPointF> point;

	qreal top = 10000;
	qreal bottom = -10000;
	qreal left = 10000;
	qreal right = -10000;

	auto k = list.begin();

	qreal x = (*k).split(" ")[0].replace("M", "").toDouble();
	qreal y = (*k).split(" ")[1].toDouble();
	point.append(QPointF(x, y) - m_transform);

	for (k++; k != list.end(); k++)
	{
		qDebug() << *k;
		qDebug() << (*k).split(" ")[0].replace("L", "");
		x = (*k).split(" ")[0].replace("L", "").toDouble();
		y = (*k).split(" ")[1].toDouble();
		point.append(QPointF(x, y) - m_transform);
		if (x < left)
			left = x;
		else if (x > right)
			right = x;

		if (y < top)
			top = y;
		else if (y > bottom)
			bottom = y;
	}
	i->setDrawStart(QPointF(left, top) - m_transform);
	i->setDrawEnd(QPointF(right, bottom) - m_transform);
	i->drawPointToPhysicalPoint(0);
	Hexagon* hexagon = dynamic_cast<Hexagon*>(i);
	if(hexagon)
		hexagon->setVertex(point);

	i->getBrush().setColor(attributes.value("fill").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setColor(attributes.value("stroke").toString().replace("#", "").toInt(NULL, 16));
	i->getPen().setWidthF(attributes.value("stroke-width").toString().toDouble());

	QString strokeStyle = attributes.value("stroke-dasharray").toString();
	if (strokeStyle == "null")
		i->getPen().setStyle(Qt::SolidLine);
	else if (strokeStyle == "2,2")
		i->getPen().setStyle(Qt::DotLine);
	else if (strokeStyle == "5,2,2,2")
		i->getPen().setStyle(Qt::DashDotLine);
	else if (strokeStyle == "5,2,2,2,2,2")
		i->getPen().setStyle(Qt::DashDotDotLine);

	return i;
}

Shape* SvgRead::readText(QXmlStreamAttributes& attributes, QXmlStreamReader& reader)
{
	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeText);
	QString text = reader.readElementText();
	QFontMetrics textLength(QFont("Microsoft YaHei", 20));
	int length = textLength.width(text);
	i->setDrawStart(QPointF(attributes.value("x").toString().toDouble(), attributes.value("y").toString().toDouble()) - m_transform);
	i->setDrawEnd(QPointF(length, 40) + i->getDrawStart());
	TextEdit* textEdit = dynamic_cast<TextEdit*>(i);
	if(textEdit)
		textEdit->setText(text);

	return i;
}

QColor SvgRead::readCanvas(QXmlStreamReader& reader)
{
	reader.readNext();
	reader.readNext();
	QXmlStreamAttributes attributes = reader.attributes();
	m_transform = QPointF(attributes.value("width").toString().toDouble(), attributes.value("height").toString().toDouble()) / 2;
	return attributes.value("fill").toString().replace("#", "").toInt(NULL, 16);
}

SvgRead::SvgRead()
{

}

SvgRead::~SvgRead()
{

}