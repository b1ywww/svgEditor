#include "shape.h"
#include <QPainter>
Shape::Shape()
{

}

Shape::~Shape()
{

}

void Shape::move(QPoint offset)
{
	m_star = m_star + offset;
	m_end = m_end + offset;

	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

Line::Line()
{

}

Line::~Line()
{

}

void Line::setStar(QPoint star)
{
	m_star = star;
	m_drawStar = m_star;
}

void Line::setEnd(QPoint end)
{
	m_end = end;
	m_drawEnd = m_end;
}

void Line::drawShape(QPainter& painter)
{
	if (m_drawStar.isNull() || m_drawEnd.isNull())
		return;
	painter.drawLine(m_drawStar, m_drawEnd);
}

void Line::setDepth(qreal depth)
{
	
}

void Line::scale(qreal width, qreal height)
{
	m_drawStar.setX(m_star.x() + width);
	m_drawEnd.setX(m_end.x() + width);

	m_drawStar.setY(m_star.y() + height);
	m_drawEnd.setY(m_end.y() + height);
}

QPoint& Line::getStar()
{
	return m_star;
}

QPoint& Line::getEnd()
{
	return m_end;
}

Square::Square()
{

}

Square::~Square()
{

}

void Square::setStar(QPoint star)
{
	m_star = star;
	m_drawStar = m_star;
}

void Square::setEnd(QPoint end)
{
	m_end = end;
	m_drawEnd = m_end;
}

void Square::drawShape(QPainter& painter)
{
	if (m_star.isNull() || m_end.isNull())
		return;
	QBrush brush(QColor(255, 255, 255));
	painter.setBrush(brush);
	painter.drawRect(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	painter.setBrush(Qt::NoBrush);
}

void Square::setDepth(qreal depth)
{

}

void Square::scale(qreal width, qreal height)
{
	m_drawStar.setX(m_drawStar.x() + width);
	m_drawEnd.setX(m_drawEnd.x() + width);

	m_drawStar.setY(m_drawStar.y() + height);
	m_drawEnd.setY(m_drawEnd.y() + height);
}

QPoint& Square::getStar()
{
	return m_drawStar;
}

QPoint& Square::getEnd()
{
	return m_drawEnd;
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
	case ShapeType::tmp:
		break;
	default:
		break;
	}
	return inShape;
}

ShapeFactory::ShapeFactory()
{

}