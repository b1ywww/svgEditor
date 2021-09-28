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

KxSvgCanvas::KxSvgCanvas(QWidget* parent)
	:QGraphicsView(parent)
{
	setObjectName(QStringLiteral("svgGraphicsView"));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条
	//setGeometry(QRect(0, 0, 600, 600));
}

KxSvgCanvas::~KxSvgCanvas()
{

}

void KxSvgCanvas::mousePressEvent(QMouseEvent* event)
{
	//if (event->button() == Qt::LeftButton)
	//{
	//	QPoint poin = event->pos();
	//	QPointF pointScene = mapToScene(poin);
	//	QGraphicsItem* item = nullptr;
	//	qDebug() << "全局坐标" << poin;
	//	qDebug() << "场景坐标" << pointScene;
	//	item = tmppScene->itemAt(pointScene, transform());
	//	if (item == nullptr)
	//		qDebug() << "无图形";
	//	else
	//	{
	//		qDebug() << "item局部坐标" << item->mapFromScene(pointScene);
	//		item->setSelected(true);
	//		scale(1.1, 1.1);
	//		setGeometry(-200, -200, width() * 1.1, height() * 1.1);
	//	}
	//}
	//if (event->button() == Qt::RightButton)
	//{
	//	QPoint poin = event->pos();
	//	QPointF pointScene = mapToScene(poin);
	//	QGraphicsItem* item = nullptr;
	//	item = tmppScene->itemAt(pointScene, transform());
	//	if (item == nullptr)
	//		qDebug() << "无图形";
	//	else
	//	{
	//		item->setSelected(true);
	//		scale(0.9, 0.9);
	//		setGeometry(-200, -200, width() * 0.9, height() * 0.9);
	//	}
	//}
	QGraphicsView::mousePressEvent(event);
}

//void KxSvgCanvas::mouseMoveEvent(QMouseEvent* event)
//{
//	QPoint poin = event->pos();
//	QPointF pointScene = mapToScene(poin);
//	QGraphicsItem* item = nullptr;
//	qDebug() << "全局坐标" << poin;
//	qDebug() << "场景坐标" << pointScene;
//	item = tmppScene->itemAt(pointScene, transform());
//	if (item)
//		item->setPos(pointScene*0.1);
//}

KxLeftToolBarBtn::KxLeftToolBarBtn(QWidget* parent)
	:QRadioButton(parent)
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
	m_imageDir << QString("D:\\icon\\") + objectName() + QString("\\default.svg")
			   << QString("D:\\icon\\") + objectName() + QString("\\enter.svg")
			   << QString("D:\\icon\\") + objectName() + QString("\\click.svg");
}

SVGMainWIndow::SVGMainWIndow(QWidget* parent)
	: QMainWindow(parent)
{
	if (objectName().isEmpty())
		setObjectName(QStringLiteral("SVGClass"));
	resize(MAINWINDOW_WIDTH, MAINWINDOW_HETGHT);
	/*
	设置布局
	*/
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

	//设置左边工具栏布局
	setToolBar();

	m_pToolBarLeftWidget->setLayout(m_pToolBarLayout);
	m_pCentralLayout->addWidget(m_pToolBarLeftWidget);

	m_pMainHoriLayout = new QHBoxLayout();
	m_pMainHoriLayout->setObjectName(QStringLiteral("mainHoriLayout"));

	//设置画布
	m_pCanvasWidget = new QWidget(m_pCentralWidget);
	m_pCanvasWidget->setObjectName(QStringLiteral("canvasWidget"));
	m_pCanvasWidget->setStyleSheet(QLatin1String("background-color: rgb(63,63,60);\n"
												"border-color: rgb(255, 255, 255);\n"));

	m_pSvgCanvas = new KxSvgCanvas(m_pCanvasWidget);
	m_pSvgCanvas->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

	m_pMainHoriLayout->addWidget(m_pCanvasWidget);

	//设置右边的设置面板
	m_pSettingWidget = new QWidget();
	m_pSettingWidget->setObjectName(QStringLiteral("settingWidget"));
	m_pSettingWidget->setMinimumSize(QSize(SETTING_MIN_WIDTH, 0));
	m_pSettingWidget->setMaximumSize(QSize(SETTING_MAX_WIDTH, DEFAULT_MAX));
	m_pSettingWidget->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

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

	m_pActionOpen = new QAction("打开");
	m_pToolBarTop->addAction(m_pActionOpen);

	m_pActionSave = new QAction("保存");
	m_pToolBarTop->addAction(m_pActionSave);

	//读取svg图片测试版本，直接在构造函数读取了
	//QString svg_dir = "D:\\icon\\click.svg";
	//QGraphicsScene* pScene = new QGraphicsScene;
	//QScopedPointer<QGraphicsSvgItem> svgItem(new QGraphicsSvgItem(svg_dir));

	//m_pSvgCanvas->resetTransform();
	//auto svg = svgItem.take();
	//svg->setFlag(QGraphicsItem::ItemClipsToShape);
	//svg->setCacheMode(QGraphicsItem::NoCache);
	//svg->setZValue(0);
	//pScene->addItem(svg);
	//m_pSvgCanvas->setScene(pScene);
	//m_pSvgCanvas->show();

	m_pSvgCanvas->tmppScene = new QGraphicsScene(QRectF(-200, -200, 400, 400));
	m_pSvgCanvas->setScene(m_pSvgCanvas->tmppScene);
	//pScene->setSceneRect(0, 0, 500, 500);
	//pScene->setBackgroundBrush(Qt::blue);
	m_pSvgCanvas->rectItem = new QGraphicsRectItem(QRectF(-200, -200, 400, 400));

	QPen pen = m_pSvgCanvas->rectItem->pen();
	pen.setWidth(2);
	pen.setColor(QColor(111, 111, 111));
	m_pSvgCanvas->rectItem->setPen(pen);
	m_pSvgCanvas->rectItem->setPos(0, 0);
	//m_pSvgCanvas->rectItem->setBrush(QColor(111, 111, 111, 100));
	//m_pSvgCanvas->tmppScene->addItem(m_pSvgCanvas->rectItem);

	m_pSvgCanvas->rectItem1 = new QGraphicsRectItem(50, 50, 200, 100);
	m_pSvgCanvas->rectItem1->setBrush(Qt::blue);
	m_pSvgCanvas->tmppScene->addItem(m_pSvgCanvas->rectItem1);

	m_pSvgCanvas->rectItem2= new QGraphicsRectItem(50, 50, 100, 50);
	m_pSvgCanvas->rectItem2->setBrush(Qt::red);
	m_pSvgCanvas->rectItem2->setPos(0, 0);
	m_pSvgCanvas->tmppScene->addItem(m_pSvgCanvas->rectItem2);

	//m_pSvgCanvas->rectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
	m_pSvgCanvas->rectItem1->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
	m_pSvgCanvas->rectItem2->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);

	qDebug() << m_pSvgCanvas->tmppScene->width()<< m_pSvgCanvas->tmppScene->height();

	setCanvasCenter();
}

void SVGMainWIndow::paintEvent(QPaintEvent* event)
{
	setCanvasCenter();
}

void SVGMainWIndow::resizeEvent(QResizeEvent* event)
{
	setCanvasCenter();
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

	m_pSelectButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pSelectButton->setObjectName(QStringLiteral("select"));
	m_pSelectButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pSelectButton->setImageDir();

	m_pPencilButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pPencilButton->setObjectName(QStringLiteral("pencil"));
	m_pPencilButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pPencilButton->setImageDir();

	m_pLineButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
	m_pLineButton->setObjectName(QStringLiteral("line"));
	m_pLineButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pLineButton->setImageDir();

	m_pRectButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget);
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

}