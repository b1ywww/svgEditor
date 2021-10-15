#include "shape.h"
#include <QPainter>
#include <QDebug>
#include <QTextDocument>

Shape::Shape()
{

}

Shape::~Shape()
{

}

QPointF& Shape::getPhysicalStar()
{
	return m_star;
}

QPointF& Shape::getPhysicalEnd()
{
	return m_end;
}

QPointF& Shape::getDrawStar()
{
	return m_drawStar;
}

QPointF& Shape::getDrawEnd()
{
	return m_drawEnd;
}

void Shape::drawPointToPhysicalPoint(qreal ratio)
{
	m_star = m_drawStar / (1 + ratio);
	m_end = m_drawEnd / (1 + ratio);
}

void Shape::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

ShapeType Shape::getShapeType()
{
	return m_type;
}

Line::Line()
{
	m_type = ShapeType::TypeLine;
}

Line::~Line()
{

}

void Line::setDrawStar(QPointF star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Line::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Line::drawShape(QPainter& painter)
{
	if (m_drawStar.isNull() || m_drawEnd.isNull())
		return;
	QPainterPath path;
	path.moveTo(m_drawStar);
	path.lineTo(m_drawEnd);
	painter.drawPath(path);
}

void Line::setDepth(qreal depth)
{
	
}

void Line::scale(qreal width, qreal height)
{
	m_drawStar.setX((m_star.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStar.setY((m_star.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Line::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Line::moveTop(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void Line::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void Line::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveUpperLeft(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
}

void Line::moveUpperRight(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveLowerLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

Square::Square()
{
	m_type = ShapeType::TypeSquare;
}

Square::~Square()
{

}

void Square::setDrawStar(QPointF star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Square::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Square::drawShape(QPainter& painter)
{
	if (m_drawStar.isNull() || m_drawEnd.isNull())
		return;
	QBrush brush(QColor(255, 255, 255));
	painter.setBrush(brush);
	QPainterPath path;
	path.addRect(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);
}

void Square::setDepth(qreal depth)
{

}

void Square::scale(qreal width, qreal height)
{
	m_drawStar.setX((m_star.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStar.setY((m_star.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Square::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Square::moveTop(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void Square::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void Square::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveUpperLeft(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
}

void Square::moveUpperRight(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveLowerLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

Pancil::Pancil()
{
	m_type = ShapeType::TypePencil;
}

Pancil::~Pancil()
{

}

void Pancil::drawShape(QPainter& painter)
{
	if (m_drawPoint.size() < 2)
		return;

	QList<QPointF>::iterator i = m_drawPoint.begin();
	QPainterPath path;
	path.moveTo((*i));
	for (; i != m_drawPoint.end();)
	{
		path.lineTo((*i));
		i++;
	}
	painter.drawPath(path);
}

void Pancil::setDrawStar(QPointF star)
{
	m_drawPoint.append(star);
	m_PhysicalPoint.append(star);
	updateClickRect(star);
}

void Pancil::setDrawEnd(QPointF end)
{
	m_drawPoint.append(end);
	m_PhysicalPoint.append(end);
	updateClickRect(end);
}

void Pancil::updateClickRect(QPointF point)
{
	point.x() < m_Left ? m_Left = point.x() : m_Left;
	point.x() > m_right ? m_right = point.x() : m_right;
	point.y() < m_top ? m_top = point.y() : m_top;
	point.y() > m_bottom ? m_bottom = point.y(): m_bottom;

	m_drawStar.setX(m_Left);
	m_drawStar.setY(m_top);

	m_drawEnd.setX(m_right);
	m_drawEnd.setY(m_bottom);

	m_star = m_drawStar;
	m_end = m_drawEnd;
}

const QList<QPointF>& Pancil::getPhysicalPoint()
{
	return m_PhysicalPoint;
}

void Pancil::setDepth(qreal depth)
{
}

void Pancil::scale(qreal ratioW, qreal ratioH)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	QList<QPointF>::iterator j = m_PhysicalPoint.begin();

	for (; i != m_drawPoint.end() && j != m_PhysicalPoint.end(); )
	{
		(*i).setX((*j).x() * (1 + ratioW));
		(*i).setY((*j).y() * (1 + ratioH));
		i++;
		j++;
	}
	
	m_drawStar.setX((m_star.x()) * (1 + ratioW));
	m_drawEnd.setX((m_end.x()) * (1 + ratioW));

	m_drawStar.setY((m_star.y()) * (1 + ratioH));
	m_drawEnd.setY((m_end.y()) * (1 + ratioH));
}

void Pancil::move(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	for (; i != m_drawPoint.end(); i++)
	{
		*i = *i + offset;
	}
	m_drawStar += offset;
	m_drawEnd += offset;
}

void Pancil::moveTop(QPointF offset)
{
	//QList<QPointF>::iterator i = m_drawPoint.begin();
	//for (; i != m_drawPoint.end(); i++)
	//{
	//	qreal ratio = ((*i).y() - m_bottom) * 1.0 / (m_top - m_bottom);
	//	qreal offsetY = offset.y() * ratio;
	//	(*i).setY((*i).y() + offsetY);
	//	qDebug() << *i;
	//}
	//m_top = m_drawStar.y() + offset.y();
	//m_drawStar.setY(m_top);
}

void Pancil::moveBottom(QPointF offset)
{

}

void Pancil::moveLeft(QPointF offset)
{

}

void Pancil::moveRight(QPointF offset)
{

}

void Pancil::moveUpperLeft(QPointF offset)
{

}

void Pancil::moveUpperRight(QPointF offset)
{

}

void Pancil::moveLowerLeft(QPointF offset)
{

}

void Pancil::moveLowerRight(QPointF offset)
{

}

void Pancil::drawPointToPhysicalPoint(qreal ratio)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	QList<QPointF>::iterator j = m_PhysicalPoint.begin();

	for (; i != m_drawPoint.end() && j != m_PhysicalPoint.end(); )
	{
		(*j) = (*i) / (1 + ratio);
		j++;
		i++;
	}

	Shape::drawPointToPhysicalPoint(ratio);
}

ShapeFactory* ShapeFactory::getShapeFactory()
{
	static ShapeFactory* m_pShapeFactory = nullptr;
	if (m_pShapeFactory == nullptr)
	{
		m_pShapeFactory = new ShapeFactory();
	}

	return m_pShapeFactory;
}

Shape* ShapeFactory::getShape(ShapeType type)
{
	Shape* inShape = nullptr;
	switch (type)
	{
	case ShapeType::TypeSelect:
		break;
	case ShapeType::TypeLine:
		inShape = new Line();
		break;
	case ShapeType::TypeSquare:
		inShape = new Square();
		break;
	case ShapeType::TypePencil:
		inShape = new Pancil();
		break;
	case ShapeType::TypeCircle:
		inShape = new Circle();
		break;
	case ShapeType::TypeHexagon:
		inShape = new Hexagon();
		break;
	case ShapeType::TypeText:
		inShape = new TextEdit();
		break;
	default:
		break;
	}
	return inShape;
}

ShapeFactory::ShapeFactory()
{

}

Circle::Circle()
{
	m_type = ShapeType::TypeCircle;
}

Circle::~Circle()
{
}

void Circle::setDrawStar(QPointF star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Circle::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Circle::drawShape(QPainter& painter)
{
	if (m_drawStar.isNull() || m_drawEnd.isNull())
		return;
	QBrush brush(QColor(255, 255, 255));
	painter.setBrush(brush);
	painter.drawEllipse(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	painter.setBrush(Qt::NoBrush);
}

void Circle::setDepth(qreal depth)
{

}

void Circle::scale(qreal width, qreal height)
{
	m_drawStar.setX((m_star.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStar.setY((m_star.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Circle::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Circle::moveTop(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void Circle::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Circle::moveLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void Circle::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Circle::moveUpperLeft(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
}

void Circle::moveUpperRight(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Circle::moveLowerLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Circle::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

Hexagon::Hexagon()
{
	m_vertex.resize(7);
	m_type = ShapeType::TypeHexagon;
}

Hexagon::~Hexagon()
{
}

void Hexagon::drawShape(QPainter& painter)
{
	QBrush brush(QColor(255, 255, 255));
	painter.setBrush(brush);
	QPolygonF myPolygon(m_vertex);
	QPainterPath path;
	path.addPolygon(myPolygon);
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);
}

void Hexagon::setDrawStar(QPointF star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Hexagon::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
	setVertex();
}

void Hexagon::setDepth(qreal depth)
{

}

void Hexagon::scale(qreal width, qreal height)
{

}

void Hexagon::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
	setVertex();
}

void Hexagon::moveTop(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	setVertex();
}

void Hexagon::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
	setVertex();
}

void Hexagon::moveLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	setVertex();
}

void Hexagon::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
	setVertex();
}

void Hexagon::moveUpperLeft(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	setVertex();
}

void Hexagon::moveUpperRight(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
	setVertex();
}

void Hexagon::moveLowerLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
	setVertex();
}

void Hexagon::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
	setVertex();
}

void Hexagon::setVertex()
{
	m_vertex[0] = QPointF(m_drawStar.x() + (m_drawEnd.x() - m_drawStar.x()) / 5, m_drawStar.y());
	m_vertex[1] = QPointF(m_drawStar.x() + 4 * (m_drawEnd.x() - m_drawStar.x()) / 5, m_drawStar.y());
	m_vertex[2] = QPointF(m_drawEnd.x(), (m_drawStar.y() + m_drawEnd.y()) / 2);
	m_vertex[3] = QPointF(m_drawStar.x() + 4 * (m_drawEnd.x() - m_drawStar.x()) / 5, m_drawEnd.y());
	m_vertex[4] = QPointF(m_drawStar.x() + (m_drawEnd.x() - m_drawStar.x()) / 5, m_drawEnd.y());
	m_vertex[5] = QPointF(m_drawStar.x(), (m_drawStar.y() + m_drawEnd.y()) / 2);
	m_vertex[6] = QPointF(m_drawStar.x() + (m_drawEnd.x() - m_drawStar.x()) / 5, m_drawStar.y());
}

const QVector<QPointF>& Hexagon::getVertex()
{
	return m_vertex;
}

TextEdit::TextEdit()
{
	setText(QStringLiteral("«Î ‰»Î"));
	m_type = ShapeType::TypeText;
}

TextEdit::~TextEdit()
{
}

void TextEdit::drawShape(QPainter& painter)
{
	painter.drawText(QRect(m_drawStar.toPoint(), m_drawEnd.toPoint()), Qt::AlignCenter, m_text);
}

void TextEdit::setDrawStar(QPointF star)
{
	m_drawStar = star;
}

void TextEdit::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
}

void TextEdit::setDepth(qreal depth)
{

}

void TextEdit::scale(qreal width, qreal height)
{
}

void TextEdit::move(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void TextEdit::moveTop(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void TextEdit::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void TextEdit::moveLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void TextEdit::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void TextEdit::moveUpperLeft(QPointF offset)
{
	m_drawStar = m_drawStar + offset;
}

void TextEdit::moveUpperRight(QPointF offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void TextEdit::moveLowerLeft(QPointF offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void TextEdit::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

void TextEdit::setText(QString text)
{
	m_text = text;
}