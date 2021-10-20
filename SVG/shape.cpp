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
	setClickRectOffset(m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	QPen pen(Qt::DashLine);
	pen.setColor(QColor(255, 192, 203));
	QPainterPath path;

	painter.setPen(pen);
	path.addRect(m_drawStar.x() + m_offsetStartX
		, m_drawStar.y() + m_offsetStartY
		, m_drawEnd.x() - m_drawStar.x() + m_offsetWidth
		, m_drawEnd.y() - m_drawStar.y() + m_offsetHeight);

	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
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
	painter.setPen(m_pen);
	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
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
	m_brush.setColor(QColor(255, 255, 255));
	m_brush.setStyle(Qt::SolidPattern);
	m_pen.setStyle(Qt::SolidLine);
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

	painter.setPen(m_pen);
	painter.setBrush(m_brush);
	QPainterPath path;
	path.addRect(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
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
	m_pen.setStyle(Qt::SolidLine);
}

Pancil::~Pancil()
{
	m_drawPoint.clear();
	m_PhysicalPoint.clear();
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
	painter.setPen(m_pen);
	painter.drawPath(path);
	painter.setPen(Qt::NoPen);
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

void Pancil::updateEdgeValue()
{
	m_top = m_drawStar.y();
	m_bottom = m_drawEnd.y();
	m_Left = m_drawStar.x();
	m_right = m_drawEnd.x();
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
	m_drawEnd.setY((m_end.y()) * (1 + ratioH));\
	updateEdgeValue();
}

void Pancil::move(QPointF offset)
{
	QList<QPointF>::iterator i = m_drawPoint.begin();
	QList<QPointF>::iterator j = m_PhysicalPoint.begin();
	for (; i != m_drawPoint.end(); i++,j++)
	{
		*i = *i + offset;
	}
	m_drawStar += offset;
	m_drawEnd += offset;

	updateEdgeValue();
}

void Pancil::moveTop(QPointF offset)
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
	m_top = m_drawStar.y() + offset.y();
	m_drawStar.setY(m_top);
}

void Pancil::moveBottom(QPointF offset)
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

void Pancil::moveLeft(QPointF offset)
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
	m_Left = m_drawStar.x() + offset.x();
	m_drawStar.setX(m_Left);
}

void Pancil::moveRight(QPointF offset)
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

void Pancil::moveUpperLeft(QPointF offset)
{
	moveLeft(offset);
	moveTop(offset);
}

void Pancil::moveUpperRight(QPointF offset)
{
	moveRight(offset);
	moveTop(offset);
}

void Pancil::moveLowerLeft(QPointF offset)
{
	moveLeft(offset);
	moveBottom(offset);
}

void Pancil::moveLowerRight(QPointF offset)
{
	moveRight(offset);
	moveBottom(offset);
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
	painter.setBrush(m_brush);
	painter.setPen(m_pen);
	painter.drawEllipse(m_drawStar.x(), m_drawStar.y(), m_drawEnd.x() - m_drawStar.x(), m_drawEnd.y() - m_drawStar.y());
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
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
	m_type = ShapeType::TypeText;
	m_pen.setStyle(Qt::SolidLine);
}

TextEdit::~TextEdit()
{
}

void TextEdit::drawShape(QPainter& painter)
{

	//QPen pen;

	//QFont font("Microsoft YaHei", 20);//这里设置字体大小

	//static int x =300;
	//font.setStretch(x);
	////font.setPointSize(x);
	//painter.setFont(font);
	//painter.drawText(QRect(m_drawStar.toPoint(), m_drawEnd.toPoint()), Qt::AlignCenter, m_text);

	painter.setPen(m_pen);
	painter.setFont(QFont("Microsoft YaHei", 20));

	painter.drawText(QRect(m_drawStar.toPoint(), m_drawEnd.toPoint()), m_text);
	painter.setPen(Qt::NoPen);
	//static QSize size(qAbs(m_drawEnd.toPoint().x() - m_drawStar.toPoint().x()), qAbs(m_drawEnd.toPoint().y() - m_drawStar.toPoint().y())); //指定图片大小;
	//QImage image(size, QImage::Format_ARGB32);
	////以ARGB32格式构造一个QImage
	////image.fill(qRgba(0,0,0,100));//填充图片背景,120/250为透明度
	//QPainter painterI(&image); //为这个QImage构造一个QPainter
	//painterI.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	////设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
	////改变组合模式和上面的填充方式可以画出透明的图片。

	////改变画笔和字体
	//QPen pen = painterI.pen();
	//pen.setColor(Qt::red);
	//QFont font = painterI.font();
	//font.setBold(true);//加粗
	//font.setPixelSize(50);//改变字体大小

	//painterI.setPen(pen);
	//painterI.setFont(font);

	//painterI.drawText(image.rect(), Qt::AlignCenter, QStringLiteral("你好"));
	////将Hello写在Image的中心

	//painter.drawImage(QRect(m_drawStar.toPoint(), m_drawEnd.toPoint()),image);
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
	//m_drawStar.setY(m_drawStar.y() + offset.y());
}

void TextEdit::moveBottom(QPointF offset)
{
	//m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void TextEdit::moveLeft(QPointF offset)
{
	//m_drawStar.setX(m_drawStar.x() + offset.x());
}

void TextEdit::moveRight(QPointF offset)
{
	//m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void TextEdit::moveUpperLeft(QPointF offset)
{
	//m_drawStar = m_drawStar + offset;
}

void TextEdit::moveUpperRight(QPointF offset)
{
	//m_drawStar.setY(m_drawStar.y() + offset.y());
	//m_drawEnd.setX(m_drawEnd.x() + offset.x());
}

void TextEdit::moveLowerLeft(QPointF offset)
{
	//m_drawStar.setX(m_drawStar.x() + offset.x());
	//m_drawEnd.setY(m_drawEnd.y() + offset.y());
}

void TextEdit::moveLowerRight(QPointF offset)
{
	//m_drawEnd = m_drawEnd + offset;
}

void TextEdit::setText(QString text)
{
	m_text = text;
}

QString TextEdit::getText()
{
	return m_text;
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

void ShapeFactory::deleteShapeFactory()
{
	delete getShapeFactory();
}

ShapeFactory::ShapeFactory()
{

}