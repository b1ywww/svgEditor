#include "kxsvgview.h"
#pragma execution_character_set("utf-8")

#include <qgraphicsview.h>
#include <QtWidgets/QToolBar>
#include <QDebug>
#include <QButtonGroup>
#include <qcoreevent.h>
#include <QSvgGenerator>
#include <QPen>
#include <QRect>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QColorDialog>
#include <QPainter>
#include <QListWidget>

const int MAINWINDOW_WIDTH = 1000;
const int MAINWINDOW_HETGHT = 800;

const int DEFAULT_MAX = 16777215;

const int LEFT_TOOLBAR_MIN_WIDTH = 40;
const int LEFT_TOOLBAR_MAX_WIDTH = 50;

const int SETTING_MIN_WIDTH = 100;
const int SETTING_MAX_WIDTH = 150;

const int MOUSENOTCLICK = 0;
const int MOUSEENTER = 1;
const int MOUSECLICK = 2;

const int ICON_MIN_SIZE = 50;

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
	emit setShapeType(m_shapeType);
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

	setFocusPolicy(Qt::ClickFocus);
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

	m_pCanvasScroll = new QScrollArea(m_pCentralWidget);
	m_pCanvasScroll->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	m_pCanvasScroll->setObjectName(QStringLiteral("canvasWidget"));
	m_pCanvasScroll->setStyleSheet(QLatin1String("background-color: rgb(63, 63, 60);\n"
												 "border: 0;"));

	m_pSvgCanvas = new KxSvgCanvas(m_pCanvasScroll);

	m_pMainHoriLayout->addWidget(m_pCanvasScroll);

	m_pCanvasScroll->setWidget(m_pSvgCanvas);
	//设置右边的设置面板
	setSettingPane();

	m_pMainHoriLayout->addWidget(m_pages);
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
	connect(m_pActionNew, SIGNAL(triggered()), m_pSvgCanvas, SLOT(init()));

	m_pActionOpen = new QAction("打开");
	m_pToolBarTop->addAction(m_pActionOpen);
	connect(m_pActionOpen, SIGNAL(triggered()), m_pSvgCanvas, SLOT(openSvg()));

	m_pActionSave = new QAction("保存");
	m_pToolBarTop->addAction(m_pActionSave);
	connect(m_pActionSave, SIGNAL(triggered()), m_pSvgCanvas, SLOT(saveSvg()));

	//设置左边工具栏布局
	setToolBar();
}

SVGMainWIndow::~SVGMainWIndow()
{
	delete m_pActionNew;
	delete m_pActionOpen;
	delete m_pActionSave;
}

void SVGMainWIndow::setCanvasColor()
{
	QColorDialog color(this);
	QColor c = color.getColor();
	if (false == c.isValid())
		return;

	QRgb mRgb = qRgb(c.red(), c.green(), c.blue());
	QString s = QString("background: #%1;border:none").arg(QString::number(mRgb, 16));
	if (m_pCanvasColorChoose)
		m_pCanvasColorChoose->setStyleSheet(s);

	if (m_pSvgCanvas)
		m_pSvgCanvas->setCanvasColor(mRgb);
}

void SVGMainWIndow::setShapeColor()
{
	QColorDialog color;
	QColor c = color.getColor();
	if (false == c.isValid())
		return;
	QRgb mRgb = qRgb(c.red(), c.green(), c.blue());

	setShapeChooseColor(mRgb);
	if (m_pSvgCanvas)
		m_pSvgCanvas->setShapeColor(mRgb);
}

void SVGMainWIndow::setPenColor()
{
	QColorDialog color;
	QColor c = color.getColor();
	if (false == c.isValid())
		return;
	QRgb mRgb = qRgb(c.red(), c.green(), c.blue());

	setPenChooseColor(mRgb);
	if (m_pSvgCanvas)
		m_pSvgCanvas->setPenColor(mRgb);
}

void SVGMainWIndow::setShapePane(QColor shapeRgb, QColor penRgb, qreal penWidth, Qt::PenStyle style)
{
	QRgb rgb = qRgb(shapeRgb.red(), shapeRgb.green(), shapeRgb.blue());
	setShapeChooseColor(rgb);

	rgb = qRgb(penRgb.red(), penRgb.green(), penRgb.blue());
	setPenChooseColor(rgb);

	if(m_pStrokeWidthLineEdit)
		m_pStrokeWidthLineEdit->setText(QString("%1").arg(penWidth));

	if(m_pStrokeStyle)
		m_pStrokeStyle->setPen(style);
}

void SVGMainWIndow::paneIndex(int index /*= 0*/)
{
	index > 1? m_pages->setCurrentIndex(0): m_pages->setCurrentIndex(index);
}

void SVGMainWIndow::setShapeChooseColor(QRgb rgb)
{
	QPushButton* colorChoose = m_pSettingSquareWidget->findChild<QPushButton*>(QString("shapeColorChoose"));

	QString s = QString("background: #%1;border:none").arg(QString::number(rgb, 16));
	if (colorChoose)
		colorChoose->setStyleSheet(s);
}

void SVGMainWIndow::setPenChooseColor(QRgb rgb)
{
	QPushButton* colorChoose = m_pSettingSquareWidget->findChild<QPushButton*>(QString("penColorChoose"));

	QString s = QString("background: #%1;border:none").arg(QString::number(rgb, 16));
	if (colorChoose)
		colorChoose->setStyleSheet(s);
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

	m_pPencilButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypePencil);
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

	m_pCircleButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeCircle);
	m_pCircleButton->setObjectName(QStringLiteral("circle"));
	m_pCircleButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pCircleButton->setImageDir();

	m_pPathButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeText);
	m_pPathButton->setObjectName(QStringLiteral("path"));
	m_pPathButton->setMinimumSize(ICON_MIN_SIZE, ICON_MIN_SIZE);
	m_pPathButton->setImageDir();

	m_pShapeButton = new KxLeftToolBarBtn(m_pToolBarLeftWidget, ShapeType::TypeHexagon);
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
	setSettingCanvas();
	setSettingSquare();
	m_pages = new QStackedWidget();
	m_pages->setObjectName(QStringLiteral("pages"));
	m_pages->setMinimumSize(QSize(SETTING_MIN_WIDTH, 0));
	m_pages->setMaximumSize(QSize(SETTING_MAX_WIDTH, DEFAULT_MAX));
	m_pages->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

	m_pages->addWidget(m_pSettingCanvasWidget);
	m_pages->addWidget(m_pSettingSquareWidget);

	m_pages->setCurrentIndex(0);
}

void SVGMainWIndow::setSettingCanvas()
{
	m_pSettingCanvasWidget = new QWidget();
	m_pSettingCanvasWidget->setObjectName(QStringLiteral("settingWidget"));
	m_pSettingCanvasWidget->setMinimumSize(QSize(SETTING_MIN_WIDTH, 0));
	m_pSettingCanvasWidget->setMaximumSize(QSize(SETTING_MAX_WIDTH, DEFAULT_MAX));
	m_pSettingCanvasWidget->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

	m_pSettingCanvasLayout = new QGridLayout(m_pSettingCanvasWidget);
	m_pSettingCanvasLayout->setSpacing(20);
	m_pSettingCanvasLayout->setContentsMargins(20, 10, 10, 10);
	m_pSettingCanvasLayout->setObjectName(QStringLiteral("settingGridLayout"));
	m_pSettingCanvasLayout->setAlignment(Qt::AlignTop);

	QWidget* pEditCanvasWidth = new QWidget(m_pSettingCanvasWidget);
	pEditCanvasWidth->setObjectName(QStringLiteral("widget_4"));
	pEditCanvasWidth->setMinimumSize(QSize(60, 60));
	pEditCanvasWidth->setMaximumSize(QSize(60, 60));
	pEditCanvasWidth->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* widthLabel = new QLabel(pEditCanvasWidth);
	widthLabel->setObjectName(QStringLiteral("widthLabel"));
	widthLabel->setGeometry(QRect(0, 5, 60, 12));
	widthLabel->setMinimumSize(QSize(60, 0));
	widthLabel->setMaximumSize(QSize(60, 16777215));
	widthLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	widthLabel->setAlignment(Qt::AlignCenter);
	widthLabel->setText("宽度");
	//设置输入框
	m_pWidthLineEdit = new QLineEdit(pEditCanvasWidth);
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

	connect(m_pWidthLineEdit, SIGNAL(textChanged(QString)), m_pSvgCanvas, SLOT(setCanvasWidth(QString)));
	connect(m_pWidthLineEdit, SIGNAL(editingFinished()), m_pSvgCanvas, SLOT(setCanvasSize()));

	QWidget* pEditCanvasHeight = new QWidget(m_pSettingCanvasWidget);
	pEditCanvasHeight->setObjectName(QStringLiteral("widget_5"));
	pEditCanvasHeight->setMinimumSize(QSize(60, 60));
	pEditCanvasHeight->setMaximumSize(QSize(60, 60));
	pEditCanvasHeight->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* heightLabel = new QLabel(pEditCanvasHeight);
	heightLabel->setObjectName(QStringLiteral("heightLabel"));
	heightLabel->setGeometry(QRect(0, 5, 60, 12));
	heightLabel->setMinimumSize(QSize(60, 0));
	heightLabel->setMaximumSize(QSize(60, 16777215));
	heightLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	heightLabel->setAlignment(Qt::AlignCenter);
	heightLabel->setText("宽度");
	//设置输入框
	m_pHeightLineEdit = new QLineEdit(pEditCanvasHeight);
	m_pHeightLineEdit->setGeometry(QRect(0, 20, 60, 40));
	m_pHeightLineEdit->setText("500");
	m_pHeightLineEdit->setAlignment(Qt::AlignCenter);
	m_pHeightLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	m_pHeightLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	m_pHeightLineEdit->setFocusPolicy(Qt::ClickFocus);
	//限制只能输入数字
	QValidator* validatorHeight = new QRegExpValidator(regx, m_pHeightLineEdit);
	m_pHeightLineEdit->setValidator(validatorHeight);

	connect(m_pHeightLineEdit, SIGNAL(textChanged(QString)), m_pSvgCanvas, SLOT(setCanvasHeight(QString)));
	connect(m_pHeightLineEdit, SIGNAL(editingFinished()), m_pSvgCanvas, SLOT(setCanvasSize()));

	QWidget* pEditCanvasColor = new QWidget(m_pSettingCanvasWidget);
	pEditCanvasColor->setObjectName(QStringLiteral("widget_5"));
	pEditCanvasColor->setMinimumSize(QSize(60, 60));
	pEditCanvasColor->setMaximumSize(QSize(60, 60));
	pEditCanvasColor->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* colorLabel = new QLabel(pEditCanvasColor);
	colorLabel->setObjectName(QStringLiteral("colorLabel"));
	colorLabel->setGeometry(QRect(0, 5, 60, 12));
	colorLabel->setMinimumSize(QSize(60, 0));
	colorLabel->setMaximumSize(QSize(60, 16777215));
	colorLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	colorLabel->setAlignment(Qt::AlignCenter);
	colorLabel->setText("颜色");
	m_pCanvasColorChoose = new QPushButton(pEditCanvasColor);
	m_pCanvasColorChoose->setObjectName(QStringLiteral("m_pColorChoose"));
	m_pCanvasColorChoose->setGeometry(QRect(5, 30, 50, 25));
	m_pCanvasColorChoose->setMinimumSize(QSize(50, 25));
	m_pCanvasColorChoose->setMaximumSize(QSize(50, 25));
	m_pCanvasColorChoose->setStyleSheet("background: rgb(255,255,255);border:none;");

	connect(m_pCanvasColorChoose, SIGNAL(pressed()), this, SLOT(setCanvasColor()));

	m_pSettingCanvasLayout->addWidget(pEditCanvasWidth, 0, 1, 1, 1);
	m_pSettingCanvasLayout->addWidget(pEditCanvasHeight, 0, 2, 1, 1);
	m_pSettingCanvasLayout->addWidget(pEditCanvasColor, 1, 1, 1, 1);
}

void SVGMainWIndow::setSettingSquare(QString x, QString y)
{
	m_pSettingSquareWidget = new QWidget();
	m_pSettingSquareWidget->setObjectName(QStringLiteral("settingSquareWidget"));
	m_pSettingSquareWidget->setMinimumSize(QSize(SETTING_MIN_WIDTH, 0));
	m_pSettingSquareWidget->setMaximumSize(QSize(SETTING_MAX_WIDTH, DEFAULT_MAX));
	m_pSettingSquareWidget->setStyleSheet(QStringLiteral("background-color: rgb(47, 47, 44);"));

	m_pSettingSquareLayout = new QGridLayout(m_pSettingSquareWidget);
	m_pSettingSquareLayout->setSpacing(20);
	m_pSettingSquareLayout->setContentsMargins(20, 10, 10, 10);
	m_pSettingSquareLayout->setObjectName(QStringLiteral("settingGridLayout"));
	m_pSettingSquareLayout->setAlignment(Qt::AlignTop);

	//m_pEditSquareX = new QWidget(m_pSettingSquareWidget);
	//m_pEditSquareX->setObjectName(QStringLiteral("SqureX"));
	//m_pEditSquareX->setMinimumSize(QSize(60, 60));
	//m_pEditSquareX->setMaximumSize(QSize(60, 60));
	//m_pEditSquareX->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	//QLabel* xLabel = new QLabel(m_pEditSquareX);
	//xLabel->setFont(QFont("Microsoft YaHei", 10));
	//xLabel->setObjectName(QStringLiteral("widthLabel"));
	//xLabel->setGeometry(QRect(0, 5, 60, 12));
	//xLabel->setMinimumSize(QSize(60, 0));
	//xLabel->setMaximumSize(QSize(60, 16777215));
	//xLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	//xLabel->setAlignment(Qt::AlignCenter);
	//xLabel->setText("X");
	////设置输入框
	//QLineEdit* pSquareXLineEdit = new QLineEdit(m_pEditSquareX);
	//pSquareXLineEdit->setGeometry(QRect(0, 20, 60, 40));
	//pSquareXLineEdit->setText(x);
	//pSquareXLineEdit->setAlignment(Qt::AlignCenter);
	//pSquareXLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	//pSquareXLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	//pSquareXLineEdit->setMaxLength(4);
	//pSquareXLineEdit->setFocusPolicy(Qt::ClickFocus);
	////限制只能输入数字
	//QRegExp regx("[1-9][0-9]+$");
	//QValidator* validatorX = new QRegExpValidator(regx, pSquareXLineEdit);
	//pSquareXLineEdit->setValidator(validatorX);

	//m_pEditSquareY = new QWidget(m_pSettingSquareWidget);
	//m_pEditSquareY->setObjectName(QStringLiteral("SqureX"));
	//m_pEditSquareY->setMinimumSize(QSize(60, 60));
	//m_pEditSquareY->setMaximumSize(QSize(60, 60));
	//m_pEditSquareY->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	//QLabel* YLabel = new QLabel(m_pEditSquareY);
	//YLabel->setFont(QFont("Microsoft YaHei", 10));
	//YLabel->setObjectName(QStringLiteral("widthLabel"));
	//YLabel->setGeometry(QRect(0, 5, 60, 12));
	//YLabel->setMinimumSize(QSize(60, 0));
	//YLabel->setMaximumSize(QSize(60, 16777215));
	//YLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	//YLabel->setAlignment(Qt::AlignCenter);
	//YLabel->setText("Y");
	////设置输入框
	//QLineEdit* pSquareYLineEdit = new QLineEdit(m_pEditSquareY);
	//pSquareYLineEdit->setGeometry(QRect(0, 20, 60, 40));
	//pSquareYLineEdit->setText(y);
	//pSquareYLineEdit->setAlignment(Qt::AlignCenter);
	//pSquareYLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	//pSquareYLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	//pSquareYLineEdit->setMaxLength(4);
	//pSquareYLineEdit->setFocusPolicy(Qt::ClickFocus);
	////限制只能输入数字
	//QRegExp regY("[1-9][0-9]+$");
	//QValidator* validatorY = new QRegExpValidator(regY, pSquareYLineEdit);
	//pSquareYLineEdit->setValidator(validatorY);

	//m_pSettingSquareLayout->addWidget(m_pEditSquareX, 0, 1, 1, 1);
	//m_pSettingSquareLayout->addWidget(m_pEditSquareY, 0, 2, 1, 1);

	//暂时用来充当一下对象颜色 线条颜色的面板
	QWidget* pEditShapeColor = new QWidget(m_pSettingSquareWidget);
	pEditShapeColor->setObjectName(QStringLiteral("editShapeColor"));
	pEditShapeColor->setMinimumSize(QSize(60, 60));
	pEditShapeColor->setMaximumSize(QSize(60, 60));
	pEditShapeColor->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* pShapeColorLabel = new QLabel(pEditShapeColor);
	pShapeColorLabel->setObjectName(QStringLiteral("colorLabel"));
	pShapeColorLabel->setGeometry(QRect(0, 5, 60, 12));
	pShapeColorLabel->setMinimumSize(QSize(60, 0));
	pShapeColorLabel->setMaximumSize(QSize(60, 16777215));
	pShapeColorLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	pShapeColorLabel->setAlignment(Qt::AlignCenter);
	pShapeColorLabel->setText("颜色填充");
	QPushButton* pShapeColorChoose = new QPushButton(pEditShapeColor);
	pShapeColorChoose->setObjectName(QStringLiteral("shapeColorChoose"));
	pShapeColorChoose->setGeometry(QRect(5, 30, 50, 25));
	pShapeColorChoose->setMinimumSize(QSize(50, 25));
	pShapeColorChoose->setMaximumSize(QSize(50, 25));
	pShapeColorChoose->setStyleSheet("background: rgb(255,255,255);border:none;");

	connect(pShapeColorChoose, SIGNAL(pressed()), this, SLOT(setShapeColor()));

	QWidget* pEditPenColor = new QWidget(m_pSettingSquareWidget);
	pEditPenColor->setObjectName(QStringLiteral("editPenColor"));
	pEditPenColor->setMinimumSize(QSize(60, 60));
	pEditPenColor->setMaximumSize(QSize(60, 60));
	pEditPenColor->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* pPencolorLabel = new QLabel(pEditPenColor);
	pPencolorLabel->setObjectName(QStringLiteral("colorLabel"));
	pPencolorLabel->setGeometry(QRect(0, 5, 60, 12));
	pPencolorLabel->setMinimumSize(QSize(60, 0));
	pPencolorLabel->setMaximumSize(QSize(60, 16777215));
	pPencolorLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	pPencolorLabel->setAlignment(Qt::AlignCenter);
	pPencolorLabel->setText("线条颜色");
	QPushButton* pPenColorChoose = new QPushButton(pEditPenColor);
	pPenColorChoose->setObjectName(QStringLiteral("penColorChoose"));
	pPenColorChoose->setGeometry(QRect(5, 30, 50, 25));
	pPenColorChoose->setMinimumSize(QSize(50, 25));
	pPenColorChoose->setMaximumSize(QSize(50, 25));
	pPenColorChoose->setStyleSheet("background: rgb(255,255,255);border:none;");

	connect(pPenColorChoose, SIGNAL(pressed()), this, SLOT(setPenColor()));

	QWidget* pEditStrokeWidth = new QWidget(m_pSettingSquareWidget);
	pEditStrokeWidth->setObjectName(QStringLiteral("widget_4"));
	pEditStrokeWidth->setMinimumSize(QSize(60, 60));
	pEditStrokeWidth->setMaximumSize(QSize(60, 60));
	pEditStrokeWidth->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* pStrokeWidthLabel = new QLabel(pEditStrokeWidth);
	pStrokeWidthLabel->setObjectName(QStringLiteral("widthLabel"));
	pStrokeWidthLabel->setGeometry(QRect(0, 5, 60, 12));
	pStrokeWidthLabel->setMinimumSize(QSize(60, 0));
	pStrokeWidthLabel->setMaximumSize(QSize(60, 16777215));
	pStrokeWidthLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	pStrokeWidthLabel->setAlignment(Qt::AlignCenter);
	pStrokeWidthLabel->setText("线条宽度");
	//设置输入框
	m_pStrokeWidthLineEdit = new QLineEdit(pEditStrokeWidth);
	m_pStrokeWidthLineEdit->setGeometry(QRect(0, 20, 60, 40));
	m_pStrokeWidthLineEdit->setText("500");
	m_pStrokeWidthLineEdit->setAlignment(Qt::AlignCenter);
	m_pStrokeWidthLineEdit->setStyleSheet(QStringLiteral("color:rgb(79, 128, 255);border-width:0;border-style:outset"));
	m_pStrokeWidthLineEdit->setFont(QFont("宋体", 18, QFont::Bold));
	m_pStrokeWidthLineEdit->setMaxLength(4);
	m_pStrokeWidthLineEdit->setFocusPolicy(Qt::ClickFocus);
	//限制只能输入数字
	QRegExp regx("[0-9].*[0-9]*");
	QValidator* validatorWidth = new QRegExpValidator(regx, pStrokeWidthLabel);
	m_pWidthLineEdit->setValidator(validatorWidth);

	connect(m_pStrokeWidthLineEdit, SIGNAL(textChanged(QString)), m_pSvgCanvas, SLOT(setStrokeWidth(QString)));
	connect(m_pStrokeWidthLineEdit, SIGNAL(editingFinished()), m_pSvgCanvas, SLOT(setStroke()));

	QWidget* pEditStrokeStyle = new QWidget(m_pSettingSquareWidget);
	pEditStrokeStyle->setObjectName(QStringLiteral("widget_4"));
	pEditStrokeStyle->setMinimumSize(QSize(60, 60));
	pEditStrokeStyle->setMaximumSize(QSize(60, 60));
	pEditStrokeStyle->setStyleSheet(QStringLiteral("background-color: rgb(63, 63, 60);"));
	QLabel* pStrokeStyleLabel = new QLabel(pEditStrokeStyle);
	pStrokeStyleLabel->setObjectName(QStringLiteral("strokeStyle"));
	pStrokeStyleLabel->setGeometry(QRect(0, 5, 60, 12));
	pStrokeStyleLabel->setMinimumSize(QSize(60, 0));
	pStrokeStyleLabel->setMaximumSize(QSize(60, 16777215));
	pStrokeStyleLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));
	pStrokeStyleLabel->setAlignment(Qt::AlignCenter);
	pStrokeStyleLabel->setText("线条样式");
	m_pStrokeStyle = new KxDropDownButton(pEditStrokeStyle);
	m_pStrokeStyle->setGeometry(QRect(0, 20, 60, 40));

	connect(m_pStrokeStyle, SIGNAL(setShapePen(Qt::PenStyle)), m_pSvgCanvas, SLOT(setStrokeStyle(Qt::PenStyle)));

	m_pSettingSquareLayout->addWidget(pEditShapeColor, 0, 1, 1, 1);
	m_pSettingSquareLayout->addWidget(pEditPenColor, 0, 2, 1, 1);
	m_pSettingSquareLayout->addWidget(pEditStrokeWidth, 1, 1, 1, 1);
	m_pSettingSquareLayout->addWidget(pEditStrokeStyle, 1, 2, 1, 1);

	connect(m_pSvgCanvas, SIGNAL(setShapePane(QColor, QColor, qreal, Qt::PenStyle)), this, SLOT(setShapePane(QColor, QColor, qreal, Qt::PenStyle)));
	connect(m_pSvgCanvas, SIGNAL(paneIndex(int)), this, SLOT(paneIndex(int)));
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

KxDropDownButton::KxDropDownButton(QWidget* parent)
	:QComboBox(parent)
{
	setStyleSheet("QComboBox QAbstractItemView{border: 0px;outline:0px;selection-background-color: blue;height:50px;background: rgb(255,255,255);font:15px;}");
	QStringList items;
	items << "——" << "·······" << "-·-·-·" << "-··-··-";

	QListWidget* listWidget = new QListWidget(this);
	for (int i = 0; i < items.count(); i++)
	{
		QListWidgetItem* item = new QListWidgetItem(items.at(i));
		item->setTextAlignment(Qt::AlignCenter);
		listWidget->addItem(item);
	}

	setModel(listWidget->model());
	setView(listWidget);

	m_pen.setColor(QColor(200, 200, 200));
	m_pen.setWidthF(2.0);

	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(getPenStyle(int)));
}

KxDropDownButton::~KxDropDownButton()
{
}

void KxDropDownButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(m_pen);
	painter.drawLine(QPoint(15, 20), QPoint(45, 20));
}

void KxDropDownButton::getPenStyle(int index)
{
	switch (index)
	{
	case 0:
		setPen(Qt::SolidLine);
		emit setShapePen(Qt::SolidLine);
		break;
	case 1:
		setPen(Qt::DotLine);
		emit setShapePen(Qt::DotLine);
		break;
	case 2:
		setPen(Qt::DashDotLine);
		emit setShapePen(Qt::DashDotLine);
		break;
	case 3:
		setPen(Qt::DashDotDotLine);
		emit setShapePen(Qt::DashDotDotLine);
	default:
		break;
	}
}

void KxDropDownButton::setPen(Qt::PenStyle style)
{
	m_pen.setStyle(style);
	update();
}