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
}

void Line::setEnd(QPoint end)
{
	m_end = end;
}

void Line::drawShape(QPainter& painter)
{
	if (m_star.isNull() || m_end.isNull())
		return;
	painter.drawLine(m_star, m_end);
}

void Line::setDepth(qreal depth)
{
	
}

void Line::scale(qreal width, qreal height)
{
	m_star.setX(m_star.x() * (1 + width));
	m_end.setX(m_end.x() * (1 + width));

	m_star.setY(m_star.y() * (1 + height));
	m_end.setY(m_end.y() * (1 + height));
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
}

void Square::setEnd(QPoint end)
{
	m_end = end;
}

void Square::drawShape(QPainter& painter)
{
	if (m_star.isNull() || m_end.isNull())
		return;
	QBrush brush(QColor(255, 255, 255));
	painter.setBrush(brush);
	painter.drawRect(m_star.x(), m_star.y(), m_end.x() - m_star.x(), m_end.y() - m_star.y());
	painter.setBrush(Qt::NoBrush);
}

void Square::setDepth(qreal depth)
{

}

void Square::scale(qreal width, qreal height)
{
	m_star.setX(m_star.x() * (1 + width));
	m_end.setX(m_end.x() * (1 + width));

	m_star.setY(m_star.y() * (1 + height));
	m_end.setY(m_end.y() * (1 + height));
}

QPoint& Square::getStar()
{
	return m_star;
}

QPoint& Square::getEnd()
{
	return m_end;
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