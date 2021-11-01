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

void Shape::drawClickRect(QPainter& painter)
{
	setClickRectOffset(m_drawEnd.x() - m_drawStart.x(), m_drawEnd.y() - m_drawStart.y());
	QPen pen(Qt::DashLine);
	pen.setColor(QColor(255, 192, 203));
	QPainterPath path;

	painter.setPen(pen);
	path.addRect(m_drawStart.x() + m_offsetStartX + 0.5 * m_pen.widthF() * (m_offsetStartX/qAbs(m_offsetStartX))
		, m_drawStart.y() + m_offsetStartY + 0.5 * m_pen.widthF() * (m_offsetStartY / qAbs(m_offsetStartY))
		, m_drawEnd.x() - m_drawStart.x() + m_offsetWidth + m_pen.widthF() * (m_offsetWidth / qAbs(m_offsetWidth))
		, m_drawEnd.y() - m_drawStart.y() + m_offsetHeight + m_pen.widthF() * (m_offsetHeight / qAbs(m_offsetHeight)));

	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
}

QPointF& Shape::getPhysicalStart()
{
	return m_start;
}

QPointF& Shape::getPhysicalEnd()
{
	return m_end;
}

QPointF& Shape::getDrawStart()
{
	return m_drawStart;
}

QPointF& Shape::getDrawEnd()
{
	return m_drawEnd;
}

void Shape::copyDate(Shape* shape)
{
	m_start = shape->m_start + QPointF(10, 10);      //物理坐标
	m_end = shape->m_end + QPointF(10, 10);
	m_drawStart = shape->m_drawStart + QPointF(10, 10);  //逻辑坐标
	m_drawEnd = shape->m_drawEnd + QPointF(10, 10);
	m_depth = shape->m_depth;
	m_pen = shape->m_pen;
	m_brush = shape->m_brush;
	m_type = shape->m_type;
}

void Shape::drawPointToPhysicalPoint(qreal ratio)
{
	m_start = m_drawStart / (1 + ratio);
	m_end = m_drawEnd / (1 + ratio);
}

void Shape::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Shape::setClickRectOffset(qreal x, qreal y)
{
	if (x > 0)
	{
		if (m_offsetStartX > 0)
		{
			m_offsetStartX *= -1;
		}
		if (m_offsetWidth < 0)
		{
			m_offsetWidth *= -1;
		}
	}
	else
	{
		if (m_offsetStartX < 0)
		{
			m_offsetStartX *= -1;
		}
		if (m_offsetWidth > 0)
		{
			m_offsetWidth *= -1;
		}
	}

	if (y > 0)
	{
		if (m_offsetStartY > 0)
		{
			m_offsetStartY *= -1;
		}
		if (m_offsetHeight < 0)
		{
			m_offsetHeight *= -1;
		}
	}
	else
	{
		if (m_offsetStartY < 0)
		{
			m_offsetStartY *= -1;
		}
		if (m_offsetHeight > 0)
		{
			m_offsetHeight *= -1;
		}
	}
}

ShapeType Shape::getShapeType()
{
	return m_type;
}

ShapeType Shape::getShapeType() const
{
	return m_type;
}

bool Shape::getClickState()
{
	return m_isClick;
}

void Shape::setClickState(bool click)
{
	m_isClick = click;
}

QPen& Shape::getPen()
{
	return m_pen;
}

QBrush& Shape::getBrush()
{
	return m_brush;
}

Line::Line()
{
	m_type = ShapeType::TypeLine;
	m_pen.setStyle(Qt::SolidLine);
}

Line::~Line()
{

}

void Line::setDrawStart(QPointF star)
{
	m_drawStart = star;
	m_start = m_drawStart;
}

void Line::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Line::drawShape(QPainter& painter)
{
	if (m_drawStart.isNull() || m_drawEnd.isNull())
		return;
	QPainterPath path;
	path.moveTo(m_drawStart);
	path.lineTo(m_drawEnd);
	painter.setPen(m_pen);
	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
}

void Line::setDepth(qreal depth)
{
	
}

void Line::scale(qreal width, qreal height)
{
	m_drawStart.setX((m_start.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStart.setY((m_start.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Line::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Line::moveTop(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
}

void Line::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
}

void Line::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveUpperLeft(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
}

void Line::moveUpperRight(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Line::moveLowerLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Line::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

void Line::copyDate(Shape* shape)
{
	Shape::copyDate(shape);
}

Square::Square()
{
	m_type = ShapeType::TypeSquare;
	m_brush.setColor(QColor(255, 255, 255));
	m_brush.setStyle(Qt::SolidPattern);
	m_pen.setStyle(Qt::SolidLine);
}

Square::~Square()
{

}

void Square::setDrawStart(QPointF star)
{
	m_drawStart = star;
	m_start = m_drawStart;
}

void Square::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Square::drawShape(QPainter& painter)
{
	if (m_drawStart.isNull() || m_drawEnd.isNull())
		return;

	painter.setPen(m_pen);
	painter.setBrush(m_brush);
	QPainterPath path;
	path.addRect(m_drawStart.x(), m_drawStart.y(), m_drawEnd.x() - m_drawStart.x(), m_drawEnd.y() - m_drawStart.y());
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
}

void Square::setDepth(qreal depth)
{

}

void Square::scale(qreal width, qreal height)
{
	m_drawStart.setX((m_start.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStart.setY((m_start.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Square::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Square::moveTop(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
}

void Square::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
}

void Square::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveUpperLeft(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
}

void Square::moveUpperRight(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Square::moveLowerLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Square::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

void Square::copyDate(Shape* shape)
{
	Shape::copyDate(shape);
}

Pencil::Pencil()
{
	m_type = ShapeType::TypePencil;
	m_pen.setStyle(Qt::SolidLine);
}

Pencil::~Pencil()
{
	m_drawPoint.clear();
	m_PhysicalPoint.clear();
}

void Pencil::drawShape(QPainter& painter)
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
	painter.setPen(m_pen);
	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
}

void Pencil::setDrawStart(QPointF star)
{
	m_drawPoint.append(star);
	m_PhysicalPoint.append(star);
	updateClickRect(star);
}

void Pencil::setDrawEnd(QPointF end)
{
	m_drawPoint.append(end);
	m_PhysicalPoint.append(end);
	updateClickRect(end);
}

void Pencil::updateClickRect(QPointF point)
{
	point.x() < m_Left ? m_Left = point.x() : m_Left;
	point.x() > m_right ? m_right = point.x() : m_right;
	point.y() < m_top ? m_top = point.y() : m_top;
	point.y() > m_bottom ? m_bottom = point.y(): m_bottom;

	m_drawStart.setX(m_Left);
	m_drawStart.setY(m_top);

	m_drawEnd.setX(m_right);
	m_drawEnd.setY(m_bottom);

	m_start = m_drawStart;
	m_end = m_drawEnd;
}

void Pencil::updateEdgeValue()
{
	m_top = m_drawStart.y();
	m_bottom = m_drawEnd.y();
	m_Left = m_drawStart.x();
	m_right = m_drawEnd.x();
}

const QList<QPointF>& Pencil::getPhysicalPoint()
{
	return m_PhysicalPoint;
}

void Pencil::setDrawPoint(QList<QPointF>& list)
{
	m_drawPoint = std::move(list);
	m_PhysicalPoint.clear();
	for (auto i : m_drawPoint)
	{
		m_PhysicalPoint.append(i);
	}
}

void Pencil::copyDate(Shape* shape)
{
	for (auto i : dynamic_cast<Pencil*>(shape)->m_drawPoint)
	{
		m_drawPoint.append(i + QPointF(10, 10));
	}

	for (auto i : dynamic_cast<Pencil*>(shape)->m_PhysicalPoint)
	{
		m_PhysicalPoint.append(i + QPointF(10, 10));
	}
	Shape::copyDate(shape);
}

void Pencil::setDepth(qreal depth)
{
}

void Pencil::scale(qreal ratioW, qreal ratioH)
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
	
	m_drawStart.setX((m_start.x()) * (1 + ratioW));
	m_drawEnd.setX((m_end.x()) * (1 + ratioW));

	m_drawStart.setY((m_start.y()) * (1 + ratioH));
	m_drawEnd.setY((m_end.y()) * (1 + ratioH));
	updateEdgeValue();
}

void Pencil::move(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	QList<QPointF>::iterator j = m_PhysicalPoint.begin();
	for (; i != m_drawPoint.end(); i++,j++)
	{
		*i = *i + offset;
	}
	m_drawStart += offset;
	m_drawEnd += offset;

	updateEdgeValue();
}

void Pencil::moveTop(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	//防止线重合加一个判断
	if (qAbs(m_top + offset.y() - m_bottom) < 0.0001)
		return;

	for (; i != m_drawPoint.end(); i++)
	{
		qreal ratio = ((*i).y() - m_bottom) / (m_top - m_bottom);
		qreal offsetY = offset.y() * ratio;
		(*i).setY((*i).y() + offsetY);
	}
	m_top = m_drawStart.y() + offset.y();
	m_drawStart.setY(m_top);
}

void Pencil::moveBottom(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	//防止线重合加一个判断
	if (qAbs(m_bottom + offset.y() - m_top) < 0.0001)
		return;

	for (; i != m_drawPoint.end(); i++)
	{
		qreal ratio = ((*i).y() - m_top) / (m_bottom - m_top);
		qreal offsetY = offset.y() * ratio;
		(*i).setY((*i).y() + offsetY);
	}
	m_bottom = m_drawEnd.y() + offset.y();
	m_drawEnd.setY(m_bottom);
}

void Pencil::moveLeft(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	//防止线重合加一个判断
	if (qAbs(m_Left + offset.x() - m_right) < 0.0001)
		return;

	for (; i != m_drawPoint.end(); i++)
	{
		qreal ratio = ((*i).x() - m_right) / (m_Left - m_right);
		qreal offsetX = offset.x() * ratio;
		(*i).setX((*i).x() + offsetX);
	}
	m_Left = m_drawStart.x() + offset.x();
	m_drawStart.setX(m_Left);
}

void Pencil::moveRight(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	//防止线重合加一个判断
	if (qAbs(m_right + offset.x() - m_Left) < 0.0001)
		return;

	for (; i != m_drawPoint.end(); i++)
	{
		qreal ratio = ((*i).x() - m_Left) / (m_right - m_Left);
		qreal offsetX = offset.x() * ratio;
		(*i).setX((*i).x() + offsetX);
	}
	m_right = m_drawEnd.x() + offset.x();
	m_drawEnd.setX(m_right);
}

void Pencil::moveUpperLeft(QPointF offset)
{
	moveLeft(offset);
	moveTop(offset);
}

void Pencil::moveUpperRight(QPointF offset)
{
	moveRight(offset);
	moveTop(offset);
}

void Pencil::moveLowerLeft(QPointF offset)
{
	moveLeft(offset);
	moveBottom(offset);
}

void Pencil::moveLowerRight(QPointF offset)
{
	moveRight(offset);
	moveBottom(offset);
}

void Pencil::drawPointToPhysicalPoint(qreal ratio)
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

Circle::Circle()
{
	m_type = ShapeType::TypeCircle;
	m_brush.setColor(QColor(255, 255, 255));
	m_brush.setStyle(Qt::SolidPattern);
	m_pen.setStyle(Qt::SolidLine);
}

Circle::~Circle()
{
}

void Circle::setDrawStart(QPointF star)
{
	m_drawStart = star;
	m_start = m_drawStart;
}

void Circle::setDrawEnd(QPointF end)
{
	m_drawEnd = end;
	m_end = m_drawEnd;
}

void Circle::drawShape(QPainter& painter)
{
	if (m_drawStart.isNull() || m_drawEnd.isNull())
		return;
	painter.setBrush(m_brush);
	painter.setPen(m_pen);
	painter.drawEllipse(m_drawStart.x(), m_drawStart.y(), m_drawEnd.x() - m_drawStart.x(), m_drawEnd.y() - m_drawStart.y());
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
}

void Circle::setDepth(qreal depth)
{

}

void Circle::scale(qreal width, qreal height)
{
	m_drawStart.setX((m_start.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStart.setY((m_start.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));
}

void Circle::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;
}

void Circle::moveTop(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
}

void Circle::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Circle::moveLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
}

void Circle::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Circle::moveUpperLeft(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
}

void Circle::moveUpperRight(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void Circle::moveLowerLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void Circle::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
}

void Circle::copyDate(Shape* shape)
{
	Shape::copyDate(shape);
}

Hexagon::Hexagon()
{
	m_vertex.resize(7);
	m_type = ShapeType::TypeHexagon;
	m_brush.setColor(QColor(255, 255, 255));
	m_brush.setStyle(Qt::SolidPattern);
	m_pen.setStyle(Qt::SolidLine);
}

Hexagon::~Hexagon()
{
}

void Hexagon::drawShape(QPainter& painter)
{
	painter.setBrush(m_brush);
	painter.setPen(m_pen);
	QPolygonF myPolygon(m_vertex);
	QPainterPath path;
	path.addPolygon(myPolygon);
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
}

void Hexagon::setDrawStart(QPointF star)
{
	m_drawStart = star;
	m_start = m_drawStart;
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
	m_drawStart.setX((m_start.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStart.setY((m_start.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));

	setVertex();
}

void Hexagon::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;
	setVertex();
}

void Hexagon::moveTop(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
	setVertex();
}

void Hexagon::moveBottom(QPointF offset)
{
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
	setVertex();
}

void Hexagon::moveLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
	setVertex();
}

void Hexagon::moveRight(QPointF offset)
{
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
	setVertex();
}

void Hexagon::moveUpperLeft(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	setVertex();
}

void Hexagon::moveUpperRight(QPointF offset)
{
	m_drawStart.setY(m_drawStart.y() + offset.y());
	m_drawEnd.setX(m_drawEnd.x() + offset.x());
	setVertex();
}

void Hexagon::moveLowerLeft(QPointF offset)
{
	m_drawStart.setX(m_drawStart.x() + offset.x());
	m_drawEnd.setY(m_drawEnd.y() + offset.y());
	setVertex();
}

void Hexagon::moveLowerRight(QPointF offset)
{
	m_drawEnd = m_drawEnd + offset;
	setVertex();
}

void Hexagon::setVertex(QVector<QPointF>& vector)
{
	if(vector.isEmpty())
	{
		m_vertex[0] = QPointF(m_drawStart.x() + (m_drawEnd.x() - m_drawStart.x()) / 5, m_drawStart.y());
		m_vertex[1] = QPointF(m_drawStart.x() + 4 * (m_drawEnd.x() - m_drawStart.x()) / 5, m_drawStart.y());
		m_vertex[2] = QPointF(m_drawEnd.x(), (m_drawStart.y() + m_drawEnd.y()) / 2);
		m_vertex[3] = QPointF(m_drawStart.x() + 4 * (m_drawEnd.x() - m_drawStart.x()) / 5, m_drawEnd.y());
		m_vertex[4] = QPointF(m_drawStart.x() + (m_drawEnd.x() - m_drawStart.x()) / 5, m_drawEnd.y());
		m_vertex[5] = QPointF(m_drawStart.x(), (m_drawStart.y() + m_drawEnd.y()) / 2);
		m_vertex[6] = QPointF(m_drawStart.x() + (m_drawEnd.x() - m_drawStart.x()) / 5, m_drawStart.y());
	}
	else
	{
		m_vertex = std::move(vector);
	}
}

const QVector<QPointF>& Hexagon::getVertex()
{
	return m_vertex;
}

void Hexagon::copyDate(Shape* shape)
{
	Shape::copyDate(shape);
	setVertex();
}

TextEdit::TextEdit()
{
	m_type = ShapeType::TypeText;
	m_pen.setStyle(Qt::SolidLine);
	m_brush.setColor(Qt::black);
}

TextEdit::~TextEdit()
{
}

void TextEdit::drawShape(QPainter& painter)
{
	painter.setPen(m_pen);
	//painter.setFont(QFont("Microsoft YaHei", 20));

	//painter.drawText(QRect(m_drawStart.toPoint(), m_drawEnd.toPoint()), m_text);
	painter.fillPath(m_path, m_brush.color());
	painter.drawPath(m_path);
	painter.setPen(Qt::NoPen);
}

void TextEdit::setDrawStart(QPointF star)
{
	m_drawStart = star;
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
	m_drawStart.setX((m_start.x()) * (1 + width));
	m_drawEnd.setX((m_end.x()) * (1 + width));

	m_drawStart.setY((m_start.y()) * (1 + height));
	m_drawEnd.setY((m_end.y()) * (1 + height));

	if (m_text.isEmpty())
		return;

	//简陋版本的文字放大缩小
	while (true)
	{
		QFontMetricsF textLength(QFont("Microsoft YaHei", m_fontSize));
		qreal widthText = textLength.width(m_text);
		if(widthText > m_drawEnd.x() - m_drawStart.x())
			break;

		m_fontSize++;
		textLength;
	}

	while (true)
	{
		if(m_drawEnd.x() - m_drawStart.x() == 0)
			break;

		QFontMetricsF textLength(QFont("Microsoft YaHei", m_fontSize));
		qreal widthText = textLength.width(m_text);
		if (widthText < m_drawEnd.x() - m_drawStart.x())
			break;

		m_fontSize--;
		textLength;
	}

	drawPointToPhysicalPoint(width);
}

void TextEdit::move(QPointF offset)
{
	m_drawStart = m_drawStart + offset;
	m_drawEnd = m_drawEnd + offset;

	m_path.translate(offset);
	updateClickRect();
}

void TextEdit::moveTop(QPointF offset)
{
	qDebug() << m_path.controlPointRect();

	if (qAbs(m_top + offset.y() - m_bottom) < 0.0001)
		return;

	int length = m_path.elementCount();
	for (int i = 0; i < length; i++)
	{
		qreal ratio = (m_path.elementAt(i).y - m_bottom) / (m_top - m_bottom);
		qreal offsetY = offset.y() * ratio;
		m_path.setElementPositionAt(i, m_path.elementAt(i).x, m_path.elementAt(i).y + offsetY);
	}

	m_top = m_drawStart.y() + offset.y();
	m_drawStart.setY(m_top);
}

void TextEdit::moveBottom(QPointF offset)
{
	if (qAbs(m_bottom + offset.y() - m_top) < 0.0001)
		return;

	int length = m_path.elementCount();
	for (int i = 0; i < length; i++)
	{
		qreal ratio = (m_path.elementAt(i).y - m_top) / (m_bottom - m_top);
		qreal offsetY = offset.y() * ratio;
		m_path.setElementPositionAt(i, m_path.elementAt(i).x, m_path.elementAt(i).y + offsetY);
	}
	m_bottom = m_drawEnd.y() + offset.y();
	m_drawEnd.setY(m_bottom);
}

void TextEdit::moveLeft(QPointF offset)
{
	if (qAbs(m_Left + offset.x() - m_right) < 0.0001)
		return;

	int length = m_path.elementCount();
	for (int i = 0; i < length; i++)
	{
		qreal ratio = (m_path.elementAt(i).x - m_right) / (m_Left - m_right);
		qreal offsetX = offset.x() * ratio;
		m_path.setElementPositionAt(i, m_path.elementAt(i).x + offsetX, m_path.elementAt(i).y);
	}
	m_Left = m_drawStart.x() + offset.x();
	m_drawStart.setX(m_Left);
}

void TextEdit::moveRight(QPointF offset)
{
	if (qAbs(m_right + offset.x() - m_Left) < 0.0001)
		return;

	int length = m_path.elementCount();
	for (int i = 0; i < length; i++)
	{
		qreal ratio = (m_path.elementAt(i).x - m_Left) / (m_right - m_Left);
		qreal offsetX = offset.x() * ratio;
		m_path.setElementPositionAt(i, m_path.elementAt(i).x + offsetX, m_path.elementAt(i).y);
	}
	m_right = m_drawEnd.x() + offset.x();
	m_drawEnd.setX(m_right);
}

void TextEdit::moveUpperLeft(QPointF offset)
{
	moveLeft(offset);
	moveTop(offset);
}

void TextEdit::moveUpperRight(QPointF offset)
{
	moveRight(offset);
	moveTop(offset);
}

void TextEdit::moveLowerLeft(QPointF offset)
{
	moveLeft(offset);
	moveBottom(offset);
}

void TextEdit::moveLowerRight(QPointF offset)
{
	moveRight(offset);
	moveBottom(offset);
}

void TextEdit::setText(QString text)
{
	m_text = text;
	m_path.addText(m_drawStart.x(), m_drawEnd.y(), QFont("Microsoft YaHei", 30), m_text);
	updateClickRect();
}

const QString TextEdit::getText()
{
	return m_text;
}

const int TextEdit::getFontSize()
{
	return m_fontSize;
}

void TextEdit::updateClickRect()
{
	QRectF rect = m_path.controlPointRect();

	qDebug() << rect;

	m_drawStart = QPointF(rect.x(), rect.y());
	m_drawEnd = m_drawStart + QPointF(rect.width(), rect.height());

	m_start = m_drawStart;
	m_end = m_drawEnd;

	m_top = m_drawStart.y();
	m_bottom = m_drawEnd.y();
	m_Left = m_drawStart.x();
	m_right = m_drawEnd.x();
}

void TextEdit::copyDate(Shape* shape)
{
	m_text = dynamic_cast<TextEdit*>(shape)->m_text;
	
	Shape::copyDate(shape);
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
		inShape = new Pencil();
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

void ShapeFactory::deleteShapeFactory()
{
	delete getShapeFactory();
}

ShapeFactory::ShapeFactory()
{

}