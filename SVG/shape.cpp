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

void Line::move(QPoint offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Line::moveTop(QPoint offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void Line::moveBottom(QPoint offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLeft(QPoint offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void Line::moveRight(QPoint offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveUpperLeft(QPoint offset)
{
	m_drawStar = m_drawStar + offset;
}

void Line::moveUpperRight(QPoint offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveLowerLeft(QPoint offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLowerRight(QPoint offset)
{
	m_drawEnd = m_drawEnd + offset;
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

void Square::move(QPoint offset)
{
	m_drawStar = m_drawStar + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Square::moveTop(QPoint offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
}

void Square::moveBottom(QPoint offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLeft(QPoint offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
}

void Square::moveRight(QPoint offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveUpperLeft(QPoint offset)
{
	m_drawStar = m_drawStar + offset;
}

void Square::moveUpperRight(QPoint offset)
{
	m_drawStar.setY(m_drawStar.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveLowerLeft(QPoint offset)
{
	m_drawStar.setX(m_drawStar.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLowerRight(QPoint offset)
{
	m_drawEnd = m_drawEnd + offset;
}

Pancil::Pancil()
{

}

Pancil::~Pancil()
{

}

void Pancil::drawShape(QPainter& painter)
{
	if (m_drawPoint.size() < 2)
		return;

	QList<QPoint>::iterator i = m_drawPoint.begin();
	QList<QPoint>::iterator j = i + 1;
	for (; j != m_drawPoint.end();)
	{
		painter.drawLine(*i, *j);
		i++;
		j++;
	}
}

void Pancil::setDrawStar(QPoint star)
{
	m_drawPoint.append(star);
	m_PhysicalPoint.append(star);
	updateClickRect(star);
}

void Pancil::setDrawEnd(QPoint end)
{
	m_drawPoint.append(end);
	m_PhysicalPoint.append(end);
	updateClickRect(end);
}

void Pancil::updateClickRect(QPoint point)
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

void Pancil::setDepth(qreal depth)
{
}

void Pancil::scale(qreal ratioW, qreal ratioH)
{
	QList<QPoint>::iterator i = m_drawPoint.begin();
	QList<QPoint>::iterator j = m_PhysicalPoint.begin();

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

void Pancil::move(QPoint offset)
{
	QList<QPoint>::iterator i = m_drawPoint.begin();
	for (; i != m_drawPoint.end(); i++)
	{
		*i = *i + offset;
	}
	m_drawStar += offset;
	m_drawEnd += offset;
}

void Pancil::moveTop(QPoint offset)
{

}

void Pancil::moveBottom(QPoint offset)
{

}

void Pancil::moveLeft(QPoint offset)
{

}

void Pancil::moveRight(QPoint offset)
{

}

void Pancil::moveUpperLeft(QPoint offset)
{

}

void Pancil::moveUpperRight(QPoint offset)
{

}

void Pancil::moveLowerLeft(QPoint offset)
{

}

void Pancil::moveLowerRight(QPoint offset)
{

}

void Pancil::drawPointToPhysicalPoint(qreal ratio)
{
	QList<QPoint>::iterator i = m_drawPoint.begin();
	QList<QPoint>::iterator j = m_PhysicalPoint.begin();

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
