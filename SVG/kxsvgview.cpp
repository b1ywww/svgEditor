#include "kxsvgview.h"

#include <QtSvg/QGraphicsSvgItem>
#include <QtWidgets/QToolBar>
#include <QDebug>
#include <QButtonGroup>
#include <qcoreevent.h>
#include <QSvgGenerator>
#include <QGraphicsItem>
#include <QPen>
#include <QRect>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QColorDialog>
#include <QGraphicsOpacityEffect>
#include <QPainter>

#define MAINWINDOW_WIDTH 1000
#define MAINWINDOW_HETGHT 800

#define DEFAULT_MAX 16777215

#define LEFT_TOOLBAR_MIN_WIDTH 40
#define LEFT_TOOLBAR_MAX_WIDTH 50

#define SETTING_MIN_WIDTH 100
#define SETTING_MAX_WIDTH 150

#define MOUSENOTCLICK 0
#define MOUSEENTER 1
#define MOUSECLICK 2

#define ICON_MIN_SIZE 50

#define POSITION_DEFAULT 0
#define POSITION_LEFT 1
#define POSITION_TOP 2
#define POSITION_UPPER_LEFT 3
#define POSITION_RIGHT 4
#define POSITION_UPPER_RIGHT 6
#define POSITION_BOTTOM 8
#define POSITION_LOWER_LETF 9
#define POSITION_LOWER_RIGHT 12

int KxSvgCanvas::s_offsetStartX = -4;
int KxSvgCanvas::s_offsetStartY = -4;
int KxSvgCanvas::s_offsetWidth = 8;
int KxSvgCanvas::s_offsetHeight = 8;

KxSvgCanvas::KxSvgCanvas(QWidget* parent)
	:QWidget(parent)
{
	m_currentType = ShapeType::TypeSelect;
	setMouseTracking(true);
	setObjectName(QStringLiteral("svgCanvas"));
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
	setGeometry(QRect(0, 0, 500, 500));
	setFocusPolicy(Qt::ClickFocus);

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
	for each (Shape* i in m_shapeList)
	{
		if (i != nullptr)
		{
			i->drawShape(painter);
			qDebug() << "draw";
		}
	}

	if (m_pClickShape)
	{
		QPen pen(Qt::DashLine);
		pen.setColor(QColor(255, 192, 203));
		painter.setPen(pen);
		painter.drawRect( m_pClickShape->getStar().x() + KxSvgCanvas::s_offsetStartX
						 ,m_pClickShape->getStar().y() + KxSvgCanvas::s_offsetStartY
						 ,m_pClickShape->getEnd().x() - m_pClickShape->getStar().x() + s_offsetWidth
						 ,m_pClickShape->getEnd().y() - m_pClickShape->getStar().y() + s_offsetHeight);
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
	if (Qt::LeftButton == event->button())
	{	
		Shape* tmpShape = nullptr;
		tmpShape = ShapeFactory::getShapeFactory()->getShape(m_currentType);
		if(tmpShape)
		{
			m_shapeList.append(tmpShape);
			m_pCurrentShape = tmpShape;
			m_pCurrentShape->setStar(event->pos());
			qDebug() << "event->pos()" << event->pos();
		}

		if (false == isMove && m_currentType == ShapeType::TypeSelect)
		{
			m_pClickShape = getClickShape(event->pos());
			isMove = true;
		}
		else
		{
			isMove = false;
		}
		m_currentPoint = event->pos();
	}
}

void KxSvgCanvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_pCurrentShape)
	{
		qDebug() << "mouseMove";
		m_pCurrentShape->setEnd(event->pos());
		qDebug() << m_pCurrentShape->getStar();
	}

	if (m_pClickShape && isMove)
	{
		switch (m_positionType)
		{
		case KxSvgCanvas::mousePosition::dafeult:
		{
			m_pClickShape->move(event->pos() - m_currentPoint);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::top:
		{
			QPoint point = m_pClickShape->getStar();
			point.setY(point.y() + event->pos().y() - m_currentPoint.y());
			m_pClickShape->setStar(point);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::left:
		{
			QPoint point = m_pClickShape->getStar();
			point.setX(point.x() + event->pos().x() - m_currentPoint.x());
			m_pClickShape->setStar(point);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::right:
		{
			QPoint point = m_pClickShape->getEnd();
			point.setX(point.x() + event->pos().x() - m_currentPoint.x());
			m_pClickShape->setEnd(point);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::bottom:
		{
			QPoint point = m_pClickShape->getEnd();
			point.setY(point.y() + event->pos().y() - m_currentPoint.y());
			m_pClickShape->setEnd(point);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::upperLeft:
		{
			QPoint point = m_pClickShape->getStar();
			point = point + event->pos() - m_currentPoint;
			m_pClickShape->setStar(point);
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::lowerLeft:
		{
			QPoint pointStart = m_pClickShape->getStar();
			QPoint pointEnd = m_pClickShape->getEnd();
			m_pClickShape->setStar(QPoint(pointStart.x() + event->pos().x() - m_currentPoint.x(), pointStart.y()));
			m_pClickShape->setEnd(QPoint(pointEnd.x(), pointEnd.y() + event->pos().y() - m_currentPoint.y()));
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::upperRight:
		{
			QPoint pointStart = m_pClickShape->getStar();
			QPoint pointEnd = m_pClickShape->getEnd();

			m_pClickShape->setStar(QPoint(pointStart.x(), pointStart.y() + event->pos().y() - m_currentPoint.y()));
			m_pClickShape->setEnd(QPoint(pointEnd.x() + event->pos().x() - m_currentPoint.x(), pointEnd.y()));
			m_currentPoint = event->pos();
			break;
		}
		case KxSvgCanvas::mousePosition::lowerRight:
		{
			QPoint point = m_pClickShape->getEnd();
			point = point + event->pos() - m_currentPoint;
			m_pClickShape->setEnd(point);
			m_currentPoint = event->pos();
			break;
		}
		default:
			break;
		}
	}
	else if (m_pClickShape && m_currentType == ShapeType::TypeSelect)
	{
		setPositionType(event->pos());
	}
	update();
}

void KxSvgCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_pCurrentShape)
	{
		if(m_pCurrentShape->getEnd().isNull())
		{
			delete m_pCurrentShape;
			m_shapeList.removeLast();
		}
		else
		{
			m_pClickShape = m_pCurrentShape;
			setOffset(event->pos().x() - m_pClickShape->getStar().x(), event->pos().y() - m_pClickShape->getStar().y());
		}
	}
	isMove = false;
	m_pCurrentShape = nullptr;
	update();
}

void KxSvgCanvas::setCurrentType(ShapeType type)
{
	m_currentType = type;
}

void KxSvgCanvas::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Delete:
	{
		if (m_pClickShape)
		{
			for each (Shape* i in m_shapeList)
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
	static bool isEnlarge = false;
	qDebug() << event->delta();
	if (event->delta() > 0)
	{
		m_offset += 0.05;
		setGeometry(0, 0, 500 * (1 + m_offset) , 500 * (1 + m_offset));
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
		setGeometry(0, 0, 500 * (1 + m_offset), 500 * (1 + m_offset));
		for each (Shape * i in m_shapeList)
		{
			if (i != nullptr)
			{
				i->scale(m_offset, m_offset);
			}
		}
	}
	update();
}

bool KxSvgCanvas::deleteShapeList()
{
	m_pClickShape = nullptr;

	if (false == m_shapeList.isEmpty())
	{
		for each (Shape* i in m_shapeList)
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
	if(nullptr == m_pSvgRenderer)
		m_pSvgRenderer = new QSvgRenderer();
	m_pSvgRenderer->load(fileDir);
}

void KxSvgCanvas::unloadSvgRenderer()
{
	if (m_pSvgRenderer&&m_pSvgRenderer->isValid())
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

	qreal x = shape->getEnd().x() - shape->getStar().x();
	qreal y = shape->getEnd().y() - shape->getStar().y();

	qreal point_x = point.x() - shape->getStar().x();
	qreal point_y = point.y() - shape->getStar().y();

	setOffset(x, y);

	if(point_x / x <= 1 &&
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

	if (qAbs(point.x() - m_pClickShape->getStar().x()) < 5)
	{
		flag = flag ^ 1;
	}
	if (qAbs(point.y() - m_pClickShape->getStar().y()) < 5)
	{
		flag = flag ^ 2;
	}
	if (qAbs(point.x() - m_pClickShape->getEnd().x()) < 5)
	{
		flag = flag ^ 4;
	}
	if (qAbs(point.y() - m_pClickShape->getEnd().y()) < 5)
	{
		flag = flag ^ 8;
	}

	switch (flag)
	{
	case POSITION_DEFAULT	  :setCursor(Qt::ArrowCursor); m_positionType = mousePosition::dafeult; break;
	case POSITION_LEFT		  :setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::left; break;
	case POSITION_TOP         :setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::top; break;
	case POSITION_RIGHT		  :setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::right; break;
	case POSITION_BOTTOM	  :setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::bottom; break;
	default:
		break;
	}

	/*下面这么写的原因是对角线存在两种情况，加个if else 来区分，没有其他好点的办法QAQ*/
	if ((m_pClickShape->getEnd().x() - m_pClickShape->getStar().x()) * (m_pClickShape->getEnd().y() - m_pClickShape->getStar().y()) >= 0)
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

KxLeftToolBarBtn::KxLeftToolBarBtn(QWidget* parent, ShapeType type /*= shapeType::tmp*/)
	:QRadioButton(parent), m_shapeType(type)
{
	setGeometry(0, 0, 50, 50);
	if (nullptr == m_pSvg)
	{
		m_pSvg = new QSvgRenderer(this);
	}
}

KxLeftToolBarBtn::~KxLeftToolBarBtn()
{

}

void KxLeftToolBarBtn::enterEvent(QEvent* event)
{
	m_isClickIcon = true;
	update();
}

void KxLeftToolBarBtn::leaveEvent(QEvent* event)
{
	m_isClickIcon = false;
}

void KxLeftToolBarBtn::mousePressEvent(QMouseEvent* event)
{
	setChecked(true);
	setShapeType(m_shapeType);
	update();
}

void KxLeftToolBarBtn::paintEvent(QPaintEvent* event)
{
	QPainter pain(this);
	if (isChecked())
		m_pSvg->load(m_imageDir[MOUSECLICK]);
	else
		m_pSvg->load(m_imageDir[MOUSENOTCLICK]);

	if (m_isClickIcon && !isChecked())
		m_pSvg->load(m_imageDir[MOUSEENTER]);

	if (m_pSvg)
	{
		m_pSvg->render(&pain, QRect(0, 0, 32, 32));
	}

	
}

void KxLeftToolBarBtn::setImageDir()
{
	m_imageDir << QString("icon\\") + objectName() + QString("\\default.svg")
			   << QString("icon\\") + objectName() + QString("\\enter.svg")
			   << QString("icon\\") + objectName() + QString("\\click.svg");
}

SVGMainWIndow::SVGMainWIndow(QWidget* parent)
	: QMainWindow(parent)
{
	if (objectName().isEmpty())
		setObjectName(QStringLiteral("SVGClass"));
	resize(MAINWINDOW_WIDTH, MAINWINDOW_HETGHT);

	installEventFilter(this);

	//设置布局
	m_pCentralWidget = new QWidget(this);
	m_pCentralWidget->setObjectName(QStringLiteral("centralWidget"));
	m_pCentralWidget->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

	m_pCentralLayout = new QHBoxLayout(m_pCentralWidget);
	m_pCentralLayout->setContentsMargins(11, 11, 11, 11);
	m_pCentralLayout->setObjectName(QStringLiteral("centralLayout"));

	//设置左边工具栏
	m_pToolBarLeftWidget = new QWidget(m_pCentralWidget);
	m_pToolBarLeftWidget->setObjectName(QStringLiteral("toolBarWidget"));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_pToolBarLeftWidget->sizePolicy().hasHeightForWidth());
	m_pToolBarLeftWidget->setSizePolicy(sizePolicy);
	m_pToolBarLeftWidget->setMinimumSize(QSize(LEFT_TOOLBAR_MIN_WIDTH, 0));
	m_pToolBarLeftWidget->setMaximumSize(QSize(LEFT_TOOLBAR_MAX_WIDTH, DEFAULT_MAX));
	m_pToolBarLeftWidget->setStyleSheet(QLatin1String("background-color: rgb(47, 47, 44);\n" "border-color: rgb(255, 255, 255);"));

	m_pToolBarLayout = new QVBoxLayout(m_pToolBarLeftWidget);
	m_pToolBarLayout->setContentsMargins(11, 11, 11, 11);
	m_pToolBarLayout->setObjectName(QStringLiteral("toolBarLayout"));
	m_pToolBarLayout->setAlignment(Qt::AlignTop);

	m_pToolBarLeftWidget->setLayout(m_pToolBarLayout);
	m_pCentralLayout->addWidget(m_pToolBarLeftWidget);

	m_pMainHoriLayout = new QHBoxLayout();
	m_pMainHoriLayout->setObjectName(QStringLiteral("mainHoriLayout"));

	//设置画布

	m_pCanvasWidget = new QWidget(m_pCentralWidget);
	m_pCanvasWidget->setObjectName(QStringLiteral("canvasWidget"));
	m_pCanvasWidget->setStyleSheet(QLatin1String("background-color: rgb(63, 63, 60);\n"
												"border-color: rgb(255, 255, 255);\n"));

	m_pSvgCanvas = new KxSvgCanvas(m_pCanvasWidget);

	m_pMainHoriLayout->addWidget(m_pCanvasWidget);

	//设置右边的设置面板
	m_pSettingWidget = new QWidget();
	m_pSettingWidget->setObjectName(QStringLiteral("settingWidget"));
	m_pSettingWidget->setMinimumSize(QSize(SETTING_MIN_WIDTH, 0));
	m_pSettingWidget->setMaximumSize(QSize(SETTING_MAX_WIDTH, DEFAULT_MAX));
	m_pSettingWidget->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

	setSettingPane();

	m_pMainHoriLayout->addWidget(m_pSettingWidget);
	m_pCentralLayout->addLayout(m_pMainHoriLayout);

	this->setCentralWidget(m_pCentralWidget);

	//设置上边工具栏
	m_pToolBarTop = new QToolBar(this);
	m_pToolBarTop->setObjectName(QStringLiteral("toolBartop"));
	m_pToolBarTop->setMovable(false);
	addToolBar(Qt::TopToolBarArea, m_pToolBarTop);
	m_pToolBarTop->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);color: rgb(255, 255, 255);"));
	m_pToolBarTop->layout()->setSpacing(10);

	m_pActionNew = new QAction("新建");
	m_pToolBarTop->addAction(m_pActionNew);
	connect(m_pActionNew, SIGNAL(triggered()), this, SLOT(newCanvas()));

	m_pActionOpen = new QAction("打开");
	m_pToolBarTop->addAction(m_pActionOpen);
	connect(m_pActionOpen, SIGNAL(triggered()), this, SLOT(openSvg()));

	m_pActionSave = new QAction("保存");
	m_pToolBarTop->addAction(m_pActionSave);

	//设置左边工具栏布局
	setToolBar();

}

void SVGMainWIndow::paintEvent(QPaintEvent* event)
{
	setCanvasCenter();
}

void SVGMainWIndow::resizeEvent(QResizeEvent* event)
{
	setCanvasCenter();
}

void SVGMainWIndow::newCanvas()
{
	qDebug() << "点击";

	if(m_pSvgCanvas->getShapeCount() > 0 || m_pSvgCanvas->isSvgValid())
	{
		QMessageBox msg(this);
		msg.setText("是否保存画布");
		msg.setIcon(QMessageBox::Warning);
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);

		int res = msg.exec();
		switch (res)
		{
		case QMessageBox::Ok:
		{
			m_pSvgCanvas->deleteShapeList();
			m_pSvgCanvas->unloadSvgRenderer();
			break;
		}
		case QMessageBox::No:
		{
			m_pSvgCanvas->deleteShapeList();
			m_pSvgCanvas->unloadSvgRenderer();
			break;
		}
		default:
			update();
			break;
		}
	}
}

void SVGMainWIndow::openSvg()
{
	QString file_path = QFileDialog::getOpenFileName(this, tr("打开文件"),"./", tr("Exe files(*.svg);;All files(*.*)"));
	if (file_path.isEmpty())
	{
		return;
	}
	newCanvas();
	m_pSvgCanvas->loadSvgRenderer(file_path);
}

void SVGMainWIndow::setCanvasSize()
{
	if (m_pSvgCanvas)
	{
		m_pSvgCanvas->setGeometry(QRect(0, 0, m_canvasWidth, m_canvasHeight));
	}
}

void SVGMainWIndow::setCanvasWidth(QString width)
{
	m_canvasWidth = width.toInt();
}

void SVGMainWIndow::setCanvasHeight(QString height)
{
	m_canvasHeight = height.toInt();
}

void SVGMainWIndow::setColor()
{
	QColorDialog color;
	QColor c = color.getRgba();
	
	QString s = QString("background: rgb(%1, %2, %3);border:none").arg(c.red()).arg(c.green()).arg(c.blue());
	if(m_pColorChoose)
		m_pColorChoose->setStyleSheet(s);
	
	s = QString("background: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue());
	if(m_pSvgCanvas)
		m_pSvgCanvas->setStyleSheet(s);
}

void SVGMainWIndow::setCanvasCenter()
{
	int w = m_pCanvasWidget->width();
	int h = m_pCanvasWidget->height();

	m_pSvgCanvas->move((w - m_pSvgCanvas->width()) / 2, (h - m_pSvgCanvas->height()) / 2);
	m_pSvgCanvas->show();
}

void SVGMainWIndow::setToolBar()
{
	if (nullptr == m_pToolBarLeftWidget)
		return;

	if (nullptr == m_pToolBarLayout)
		return;

	m_pToolBarGroup = new QButtonGroup(m_pToolBarLeftWidget);
	m_pToolBarGroup->setObjectName(QStringLiteral("toolBarGroup"));
	m_pToolBarGroup->setExclusive(true);

	m_pSelectButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeSelect);
	m_pSelectButton->setObjectName(QStringLiteral("select"));
	m_pSelectButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pSelectButton->setImageDir();

	m_pPencilButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pPencilButton->setObjectName(QStringLiteral("pencil"));
	m_pPencilButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pPencilButton->setImageDir();

	m_pLineButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeLine);
	m_pLineButton->setObjectName(QStringLiteral("Line"));
	m_pLineButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pLineButton->setImageDir();

	m_pRectButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeSquare);
	m_pRectButton->setObjectName(QStringLiteral("rect"));
	m_pRectButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pRectButton->setImageDir();

	m_pCircleButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pCircleButton->setObjectName(QStringLiteral("circle"));
	m_pCircleButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pCircleButton->setImageDir();

	m_pPathButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pPathButton->setObjectName(QStringLiteral("path"));
	m_pPathButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pPathButton->setImageDir();

	m_pShapeButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pShapeButton->setObjectName(QStringLiteral("shape"));
	m_pShapeButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pShapeButton->setImageDir();

	m_pToolBarGroup->addButton(m_pSelectButton);
	m_pToolBarGroup->addButton(m_pPencilButton);
	m_pToolBarGroup->addButton(m_pLineButton);
	m_pToolBarGroup->addButton(m_pRectButton);
	m_pToolBarGroup->addButton(m_pCircleButton);
	m_pToolBarGroup->addButton(m_pPathButton);
	m_pToolBarGroup->addButton(m_pShapeButton);

	m_pToolBarLayout->addWidget(m_pSelectButton);
	m_pToolBarLayout->addWidget(m_pPencilButton);
	m_pToolBarLayout->addWidget(m_pLineButton);
	m_pToolBarLayout->addWidget(m_pRectButton);
	m_pToolBarLayout->addWidget(m_pCircleButton);
	m_pToolBarLayout->addWidget(m_pPathButton);
	m_pToolBarLayout->addWidget(m_pShapeButton);

	connect(m_pSelectButton, SIGNAL(setShapeType(ShapeType)),m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pPencilButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pLineButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pRectButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pCircleButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pPathButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
	connect(m_pShapeButton, SIGNAL(setShapeType(ShapeType)), m_pSvgCanvas, SLOT(setCurrentType(ShapeType)));
}

void SVGMainWIndow::setSettingPane()
{
	m_pGridLayout = new QGridLayout(m_pSettingWidget);
	m_pGridLayout->setSpacing(20);
	m_pGridLayout->setContentsMargins(20, 10, 10, 10);
	m_pGridLayout->setObjectName(QStringLiteral("settingGridLayout"));
	m_pGridLayout->setAlignment(Qt::AlignTop);

	m_pEditCanvasWidth = new QWidget(m_pSettingWidget);
	m_pEditCanvasWidth->setObjectName(QStringLiteral("widget_4"));
	m_pEditCanvasWidth->setMinimumSize(QSize(60, 60));
	m_pEditCanvasWidth->setMaximumSize(QSize(60, 60));
	m_pEditCanvasWidth->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* widthLabel = new QLabel(m_pEditCanvasWidth);
	widthLabel->setObjectName(QStringLiteral("widthLabel"));
	widthLabel->setGeometry(QRect(0, 5, 60, 12));
	widthLabel->setMinimumSize(QSize(60, 0));
	widthLabel->setMaximumSize(QSize(60, 16777215));
	widthLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	widthLabel->setAlignment(Qt::AlignCenter);
	widthLabel->setText("宽度");
	//设置输入框
	m_pWidthLineEdit = new QLineEdit(m_pEditCanvasWidth);
	m_pWidthLineEdit->setGeometry(QRect(0, 20, 60, 40));
	m_pWidthLineEdit->setText("500");
	m_pWidthLineEdit->setAlignment(Qt::AlignCenter);
	m_pWidthLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	m_pWidthLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	m_pWidthLineEdit->setMaxLength(4);
	m_pWidthLineEdit->setFocusPolicy(Qt::ClickFocus);
	//限制只能输入数字
	QRegExp regx("[1-9][0-9]+$");
	QValidator* validatorWidth = new QRegExpValidator(regx, m_pWidthLineEdit);
	m_pWidthLineEdit->setValidator(validatorWidth);

	connect(m_pWidthLineEdit, SIGNAL(editingFinished()), this, SLOT(setCanvasSize()));
	connect(m_pWidthLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setCanvasWidth(QString)));

	m_pEditCanvasHeight = new QWidget(m_pSettingWidget);
	m_pEditCanvasHeight->setObjectName(QStringLiteral("widget_5"));
	m_pEditCanvasHeight->setMinimumSize(QSize(60, 60));
	m_pEditCanvasHeight->setMaximumSize(QSize(60, 60));
	m_pEditCanvasHeight->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* heightLabel = new QLabel(m_pEditCanvasHeight);
	heightLabel->setObjectName(QStringLiteral("heightLabel"));
	heightLabel->setGeometry(QRect(0, 5, 60, 12));
	heightLabel->setMinimumSize(QSize(60, 0));
	heightLabel->setMaximumSize(QSize(60, 16777215));
	heightLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	heightLabel->setAlignment(Qt::AlignCenter);
	heightLabel->setText("宽度");
	//设置输入框
	m_pHeightLineEdit = new QLineEdit(m_pEditCanvasHeight);
	m_pHeightLineEdit->setGeometry(QRect(0, 20, 60, 40));
	m_pHeightLineEdit->setText("500");
	m_pHeightLineEdit->setAlignment(Qt::AlignCenter);
	m_pHeightLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	m_pHeightLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	m_pHeightLineEdit->setFocusPolicy(Qt::ClickFocus);
	//限制只能输入数字
	QValidator* validatorHeight = new QRegExpValidator(regx, m_pHeightLineEdit);
	m_pHeightLineEdit->setValidator(validatorHeight);

	connect(m_pHeightLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setCanvasHeight(QString)));
	connect(m_pHeightLineEdit, SIGNAL(editingFinished()), this, SLOT(setCanvasSize()));

	m_pEditCanvasColor = new QWidget(m_pSettingWidget);
	m_pEditCanvasColor->setObjectName(QStringLiteral("widget_5"));
	m_pEditCanvasColor->setMinimumSize(QSize(60, 60));
	m_pEditCanvasColor->setMaximumSize(QSize(60, 60));
	m_pEditCanvasColor->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* colorLabel = new QLabel(m_pEditCanvasColor);
	colorLabel->setObjectName(QStringLiteral("colorLabel"));
	colorLabel->setGeometry(QRect(0, 5, 60, 12));
	colorLabel->setMinimumSize(QSize(60, 0));
	colorLabel->setMaximumSize(QSize(60, 16777215));
	colorLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	colorLabel->setAlignment(Qt::AlignCenter);
	colorLabel->setText("颜色");
	m_pColorChoose = new QPushButton(m_pEditCanvasColor);
	m_pColorChoose->setObjectName(QStringLiteral("m_pColorChoose"));
	m_pColorChoose->setGeometry(QRect(5, 30, 50, 25));
	m_pColorChoose->setMinimumSize(QSize(50, 25));
	m_pColorChoose->setMaximumSize(QSize(50, 25));
	m_pColorChoose->setStyleSheet("background: rgb(255,255,255);border:none;");

	connect(m_pColorChoose, SIGNAL(pressed()), this, SLOT(setColor()));

	m_pGridLayout->addWidget(m_pEditCanvasWidth, 0, 1, 1, 1);
	m_pGridLayout->addWidget(m_pEditCanvasHeight, 0, 2, 1, 1);
	m_pGridLayout->addWidget(m_pEditCanvasColor, 1, 1, 1, 1);
}

bool SVGMainWIndow::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() != QEvent::MouseButtonPress)
		return false;



	if (m_pHeightLineEdit && watched != m_pHeightLineEdit)
	{
		m_pHeightLineEdit->clearFocus();
		setFocus();
	}

	if (m_pWidthLineEdit && watched != m_pWidthLineEdit)
	{
		m_pWidthLineEdit->clearFocus();
		setFocus();
	}

	return false;
}