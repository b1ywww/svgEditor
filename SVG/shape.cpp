#include "shape.h"
#include <QPainter>
#include <QDebug>
Shape::Shape()
{

}

Shape::~Shape()
{

}

QPoint& Shape::getPhysicalStar()
{
	return m_star;
}

QPoint& Shape::getPhysicalEnd()
{
	return m_end;
}

QPoint& Shape::getDrawStar()
{
	return m_drawStar;
}

QPoint& Shape::getDrawEnd()
{
	return m_drawEnd;
}

void Shape::drawPointToPhysicalPoint(qreal ratio)
{
	m_star = m_drawStar / (1 + ratio);
	m_end = m_drawEnd / (1 + ratio);
}

void Shape::move(QPoint offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

Line::Line()
{

}

Line::~Line()
{

}

void Line::setDrawStar(QPoint star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Line::setDrawEnd(QPoint end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
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
	m_drawStar.setX((m_star.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStar.setY((m_star.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

Square::Square()
{

}

Square::~Square()
{

}

void Square::setDrawStar(QPoint star)
{
	m_drawStar = star;
	m_star = m_drawStar;
}

void Square::setDrawEnd(QPoint end)
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
	painter.drawRect(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
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