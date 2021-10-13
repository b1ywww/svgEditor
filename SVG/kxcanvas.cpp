#include "kxcanvas.h"
#include "QPainter"
#include "QDebug"

int KxSvgCanvas::s_offsetStartX = -4;
int KxSvgCanvas::s_offsetStartY = -4;
int KxSvgCanvas::s_offsetWidth = 8;
int KxSvgCanvas::s_offsetHeight = 8;

const int POSITION_DEFAULT = 0;
const int POSITION_LEFT = 1;
const int POSITION_TOP = 2;
const int POSITION_UPPER_LEFT = 3;
const int POSITION_RIGHT = 4;
const int POSITION_UPPER_RIGHT = 6;
const int POSITION_BOTTOM = 8;
const int POSITION_LOWER_LETF = 9;
const int POSITION_LOWER_RIGHT = 12;

KxSvgCanvas::KxSvgCanvas(QWidget* parent)
	:QWidget(parent)
{
	m_currentType = ShapeType::TypeSelect;
	setMouseTracking(true);
	setObjectName(QStringLiteral("svgCanvas"));
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
	setFocusPolicy(Qt::ClickFocus);
	setCanvasSize();
}

KxSvgCanvas::KxSvgCanvas(QWidget* parent, qreal x, qreal y, qreal w, qreal h)
	:QWidget(parent)
{
	setGeometry(x, y, w, h);
}

KxSvgCanvas::~KxSvgCanvas()
{

}

void KxSvgCanvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QTransform transform;
	transform.translate(m_transfrom.x(), m_transfrom.y());
	painter.setTransform(transform);
	for each (Shape * i in m_shapeList)
	{
		if (i != nullptr)
		{
			i->drawShape(painter);
		}
	}

	if (m_pClickShape)
	{
		QPen pen(Qt::DashLine);
		pen.setColor(QColor(255, 192, 203));
		painter.setPen(pen);
		painter.drawRect(m_pClickShape->getDrawStar().x() + KxSvgCanvas::s_offsetStartX
			, m_pClickShape->getDrawStar().y() + KxSvgCanvas::s_offsetStartY
			, m_pClickShape->getDrawEnd().x() - m_pClickShape->getDrawStar().x() + s_offsetWidth
			, m_pClickShape->getDrawEnd().y() - m_pClickShape->getDrawStar().y() + s_offsetHeight);
		painter.setPen(Qt::NoPen);
	}
	else
	{
		setCursor(Qt::ArrowCursor);
		m_positionType = mousePosition::dafeult;
	}

	if (m_pSvgRenderer && m_pSvgRenderer->isValid())
		m_pSvgRenderer->render(&painter);
}

void KxSvgCanvas::mousePressEvent(QMouseEvent* event)
{
	QPoint transfromPoint = event->pos() - m_transfrom;
	if (Qt::LeftButton == event->button())
	{
		Shape* tmpShape = nullptr;
		tmpShape = ShapeFactory::getShapeFactory()->getShape(m_currentType);
		if (tmpShape)
		{
			m_shapeList.append(tmpShape);
			m_pCurrentShape = tmpShape;
			m_pCurrentShape->setDrawStar(transfromPoint / (1 + m_offset));
		}

		if (false == isMove && m_currentType == ShapeType::TypeSelect)
		{
			m_pClickShape = getClickShape(transfromPoint);
			isMove = true;
		}
		else
		{
			isMove = false;
		}
		m_currentPoint = transfromPoint;
	}
}

void KxSvgCanvas::mouseMoveEvent(QMouseEvent* event)
{
	QPoint transformPoint = event->pos() - m_transfrom;
	if (m_pCurrentShape)
	{
		m_pCurrentShape->setDrawEnd(transformPoint / (1 + m_offset));
		m_pCurrentShape->scale(m_offset, m_offset); //更新drawPoint坐标
	}

	if (m_pClickShape && isMove)
	{
		editShape(transformPoint);
		m_currentPoint = transformPoint;
	}
	else if (m_pClickShape && m_currentType == ShapeType::TypeSelect)
	{
		setPositionType(transformPoint);
	}

	update();
}

void KxSvgCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_pCurrentShape)
	{
		if (m_pCurrentShape->getDrawEnd().isNull() && m_currentType != ShapeType::TypePencil)
		{
			delete m_pCurrentShape;
			m_shapeList.removeLast();
			m_pCurrentShape = nullptr;
		}
		else
		{
			m_pClickShape = m_pCurrentShape;
			setOffset(event->pos().x() - m_pClickShape->getDrawStar().x(), event->pos().y() - m_pClickShape->getDrawStar().y());
		}
	}
	updatePhysicalPoint();
	isMove = false;
	m_pCurrentShape = nullptr;
	update();
}

void KxSvgCanvas::setCurrentType(ShapeType type)
{
	m_currentType = type;
}

void KxSvgCanvas::setCanvasSize()
{
	setGeometry(QRect(0, 0, m_canvasWidth, m_canvasHeight));
	m_transfrom.setX(m_canvasWidth / 2);
	m_transfrom.setY(m_canvasHeight / 2);
}

void KxSvgCanvas::setCanvasWidth(QString width)
{
	m_canvasWidth = width.toInt();
}

void KxSvgCanvas::setCanvasHeight(QString height)
{
	m_canvasHeight = height.toInt();
}

void KxSvgCanvas::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Delete:
	{
		if (m_pClickShape)
		{
			for each (Shape * i in m_shapeList)
			{
				if (i == m_pClickShape)
				{
					m_shapeList.removeOne(i);
					delete m_pClickShape;
					m_pClickShape = nullptr;
				}
			}
		}
	}
	default:
		update();
		break;
	}
}

void KxSvgCanvas::wheelEvent(QWheelEvent* event)
{
	QPoint transfromOffset;
	if (event->delta() > 0)
	{
		m_offset += 0.05;
		setGeometry(0, 0, m_canvasWidth * (1 + m_offset), m_canvasHeight * (1 + m_offset));
		transfromOffset = QPoint((m_canvasWidth * (1 + m_offset)) / 2, (m_canvasHeight * (1 + m_offset)) / 2);
		for each (Shape * i in m_shapeList)
		{
			if (i != nullptr)
			{
				i->scale(m_offset, m_offset);
			}
		}
	}
	else if (event->delta() < 0)
	{
		m_offset -= 0.05;
		if (qAbs(m_offset + 1.0) < 0.1)
			m_offset = -0.95;
		setGeometry(0, 0, m_canvasWidth * (1 + m_offset), m_canvasWidth * (1 + m_offset));
		transfromOffset = QPoint((m_canvasWidth * (1 + m_offset)) / 2, (m_canvasHeight * (1 + m_offset)) / 2);
		for each (Shape * i in m_shapeList)
		{
			if (i != nullptr)
			{
				i->scale(m_offset, m_offset);
			}
		}

	}
	m_transfrom = transfromOffset;
	update();
}

bool KxSvgCanvas::deleteShapeList()
{
	m_pClickShape = nullptr;

	if (false == m_shapeList.isEmpty())
	{
		for each (Shape * i in m_shapeList)
		{
			m_shapeList.removeOne(i);
			delete i;
		}
		return true;
	}

	return false;
}

void KxSvgCanvas::loadSvgRenderer(QString fileDir)
{
	if (nullptr == m_pSvgRenderer)
		m_pSvgRenderer = new QSvgRenderer();
	m_pSvgRenderer->load(fileDir);
}

void KxSvgCanvas::unloadSvgRenderer()
{
	if (m_pSvgRenderer && m_pSvgRenderer->isValid())
	{
		m_pSvgRenderer->load(QString());
	}
}

bool KxSvgCanvas::isSvgValid()
{
	if (m_pSvgRenderer)
	{
		return m_pSvgRenderer->isValid();
	}
	return false;
}

int KxSvgCanvas::getShapeCount()
{
	if (false == m_shapeList.isEmpty())
	{
		return m_shapeList.count();
	}
	return 0;
}

Shape* KxSvgCanvas::getClickShape(QPoint point)
{
	//逆序遍历，深度值越小 在链表的位置越靠前
	for (QList<Shape*>::iterator i = --m_shapeList.end(); i != --m_shapeList.begin(); --i)
	{
		if (isInRect(point, *i))
			return *i;
	}
	return nullptr;
}

bool KxSvgCanvas::isInRect(QPoint point, Shape* shape)
{
	if (shape == nullptr)
		return nullptr;

	qreal x = shape->getDrawEnd().x() - shape->getDrawStar().x() + 0.1; /*加0.1是为了防止出现 分母为0的情况，因为点是为整数的，所以加上0.1 不影响x的符号*/
	qreal y = shape->getDrawEnd().y() - shape->getDrawStar().y() + 0.1;

	qreal point_x = point.x() - shape->getDrawStar().x();
	qreal point_y = point.y() - shape->getDrawStar().y();

	setOffset(x, y);

	if (point_x / x <= 1 &&
		point_x / x >= 0 &&
		point_y / y <= 1 &&
		point_y / y >= 0)
	{
		return true;
	}
	return false;
}

void KxSvgCanvas::setOffset(qreal x, qreal y)
{
	if (x > 0)
	{
		if (s_offsetStartX > 0)
		{
			s_offsetStartX *= -1;
		}
		if (s_offsetWidth < 0)
		{
			s_offsetWidth *= -1;
		}
	}
	else
	{
		if (s_offsetStartX < 0)
		{
			s_offsetStartX *= -1;
		}
		if (s_offsetWidth > 0)
		{
			s_offsetWidth *= -1;
		}
	}

	if (y > 0)
	{
		if (s_offsetStartY > 0)
		{
			s_offsetStartY *= -1;
		}
		if (s_offsetHeight < 0)
		{
			s_offsetHeight *= -1;
		}
	}
	else
	{
		if (s_offsetStartY < 0)
		{
			s_offsetStartY *= -1;
		}
		if (s_offsetHeight > 0)
		{
			s_offsetHeight *= -1;
		}
	}
}

void KxSvgCanvas::setPositionType(QPoint point)
{
	int flag = 0;
	if (false == isInRect(point, m_pClickShape))
	{
		setCursor(Qt::ArrowCursor);
		m_positionType = mousePosition::dafeult;
		return;
	}

	if (qAbs(point.x() - m_pClickShape->getDrawStar().x()) < 5)
	{
		flag = flag ^ 1;
	}
	if (qAbs(point.y() - m_pClickShape->getDrawStar().y()) < 5)
	{
		flag = flag ^ 2;
	}
	if (qAbs(point.x() - m_pClickShape->getDrawEnd().x()) < 5)
	{
		flag = flag ^ 4;
	}
	if (qAbs(point.y() - m_pClickShape->getDrawEnd().y()) < 5)
	{
		flag = flag ^ 8;
	}

	switch (flag)
	{
	case POSITION_DEFAULT:setCursor(Qt::ArrowCursor); m_positionType = mousePosition::dafeult; break;
	case POSITION_LEFT:setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::left; break;
	case POSITION_TOP:setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::top; break;
	case POSITION_RIGHT:setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::right; break;
	case POSITION_BOTTOM:setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::bottom; break;
	default:
		break;
	}

	/*下面这么写的原因是对角线存在两种情况，加个if else 来区分，没有其他好点的办法QAQ*/
	if ((m_pClickShape->getDrawEnd().x() - m_pClickShape->getDrawStar().x()) * (m_pClickShape->getDrawEnd().y() - m_pClickShape->getDrawStar().y()) >= 0)
	{
		switch (flag)
		{
		case POSITION_UPPER_RIGHT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::upperRight; break;
		case POSITION_UPPER_LEFT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::upperLeft; break;
		case POSITION_LOWER_LETF:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::lowerLeft; break;
		case POSITION_LOWER_RIGHT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::lowerRight; break;
		}
	}
	else
	{
		switch (flag)
		{
		case POSITION_UPPER_RIGHT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::upperRight; break;
		case POSITION_UPPER_LEFT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::upperLeft; break;
		case POSITION_LOWER_LETF:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::lowerLeft; break;
		case POSITION_LOWER_RIGHT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::lowerRight; break;
		}
	}
}

void KxSvgCanvas::editShape(QPoint transformPoint)
{
	switch (m_positionType)
	{
	case KxSvgCanvas::mousePosition::dafeult:
	{
		m_pClickShape->move((transformPoint - m_currentPoint));
		break;
	}
	case KxSvgCanvas::mousePosition::top:
	{
		m_pClickShape->moveTop(QPoint(0, transformPoint.y() - m_currentPoint.y()));
		break;
	}
	case KxSvgCanvas::mousePosition::left:
	{
		QPoint point = m_pClickShape->getDrawStar();
		point.setX(point.x() + (transformPoint.x() - m_currentPoint.x()));
		m_pClickShape->setDrawStar(point);
		break;
	}
	case KxSvgCanvas::mousePosition::right:
	{
		QPoint point = m_pClickShape->getDrawEnd();
		point.setX(point.x() + transformPoint.x() - m_currentPoint.x());
		m_pClickShape->setDrawEnd(point);
		break;
	}
	case KxSvgCanvas::mousePosition::bottom:
	{
		QPoint point = m_pClickShape->getDrawEnd();
		point.setY(point.y() + transformPoint.y() - m_currentPoint.y());
		m_pClickShape->setDrawEnd(point);
		break;
	}
	case KxSvgCanvas::mousePosition::upperLeft:
	{
		QPoint point = m_pClickShape->getDrawStar();
		point = point + transformPoint - m_currentPoint;
		m_pClickShape->setDrawStar(point);
		break;
	}
	case KxSvgCanvas::mousePosition::lowerLeft:
	{
		QPoint pointStart = m_pClickShape->getDrawStar();
		QPoint pointEnd = m_pClickShape->getDrawEnd();
		m_pClickShape->setDrawStar(QPoint(pointStart.x() + transformPoint.x() - m_currentPoint.x(), pointStart.y()));
		m_pClickShape->setDrawEnd(QPoint(pointEnd.x(), pointEnd.y() + transformPoint.y() - m_currentPoint.y()));
		break;
	}
	case KxSvgCanvas::mousePosition::upperRight:
	{
		QPoint pointStart = m_pClickShape->getDrawStar();
		QPoint pointEnd = m_pClickShape->getDrawEnd();

		m_pClickShape->setDrawStar(QPoint(pointStart.x(), pointStart.y() + transformPoint.y() - m_currentPoint.y()));
		m_pClickShape->setDrawEnd(QPoint(pointEnd.x() + transformPoint.x() - m_currentPoint.x(), pointEnd.y()));
		break;
	}
	case KxSvgCanvas::mousePosition::lowerRight:
	{
		QPoint point = m_pClickShape->getDrawEnd();
		point = point + (transformPoint - m_currentPoint);
		m_pClickShape->setDrawEnd(point);
		break;
	}
	default:
		break;
	}
}

void KxSvgCanvas::updatePhysicalPoint()
{
	if (m_pClickShape)
	{
		m_pClickShape->drawPointToPhysicalPoint(m_offset);
	}
}