#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include <QtSvg/qsvgrenderer.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QList>

#include "shape.h"
class Line;
class Shape;
class QLineEdit;
class KxSvgCanvas :public QWidget
{
	Q_OBJECT

public:
	KxSvgCanvas(QWidget* parent);
	KxSvgCanvas(QWidget* parent, qreal x, qreal y, qreal w, qreal h);
	~KxSvgCanvas();

	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	bool deleteShapeList();
	void loadSvgRenderer(QString);
	void unloadSvgRenderer();
	bool isSvgValid();
	int getShapeCount();

public slots:
	void setCurrentType(ShapeType);
	void setCanvasSize();
	void setCanvasWidth(QString);
	void setCanvasHeight(QString);

private:
	Shape* getClickShape(QPoint point);
	bool isInRect(QPoint point, Shape* shape);
	void setOffset(qreal, qreal);
	void setPositionType(QPoint point);

	ShapeType m_currentType;
	QList<Shape*> m_shapeList;
	QSvgRenderer* m_pSvgRenderer = nullptr;
	QPoint m_transfrom;
	QPoint m_currentPoint;
	Shape* m_pCurrentShape = nullptr;
	Shape* m_pClickShape = nullptr;
	qreal m_offset = 0.0;
	bool isMove = false;
	int m_canvasWidth = 500;
	int m_canvasHeight = 500;

	enum class mousePosition
	{
		dafeult = 0,
		top,
		left,
		right,
		bottom,
		upperLeft,
		lowerLeft,
		upperRight,
		lowerRight
	};
	mousePosition m_positionType = mousePosition::dafeult;

	static int s_offsetStartX;
	static int s_offsetStartY;
	static int s_offsetWidth;
	static int s_offsetHeight;
};
	
class KxLeftToolBarBtn :public QRadioButton
{
	Q_OBJECT

public:
	KxLeftToolBarBtn(QWidget* parent, ShapeType type = ShapeType::tmp);
	~KxLeftToolBarBtn();
	void paintEvent(QPaintEvent*) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* e) override;
	
	void setImageDir();

signals:
	void setShapeType(ShapeType type);

private:
	QStringList m_imageDir;
	bool m_isClickIcon = false;
	QSvgRenderer* m_pSvg = nullptr;
	ShapeType m_shapeType;
};

class SVGMainWIndow : public QMainWindow
{
	Q_OBJECT

public:
	SVGMainWIndow(QWidget* parent = Q_NULLPTR);

	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
public slots:
	void newCanvas();
	void openSvg();
	void setColor();

private:
	void setCanvasCenter();
	void setToolBar();
	void setSettingPane();

private:

	QHBoxLayout* m_pMainHoriLayout = nullptr;
	QHBoxLayout* m_pCentralLayout = nullptr;
	QVBoxLayout* m_pToolBarLayout = nullptr;
	QGridLayout* m_pGridLayout = nullptr;

	QWidget* m_pEditCanvasWidth = nullptr;
	QWidget* m_pEditCanvasHeight = nullptr;
	QWidget* m_pEditCanvasColor = nullptr;

	QWidget* m_pCentralWidget = nullptr;
	QWidget* m_pToolBarLeftWidget = nullptr;
	QWidget* m_pCanvasWidget = nullptr;
	QWidget* m_pSettingWidget = nullptr;

	QLineEdit* m_pWidthLineEdit = nullptr;		 //设置面板的编辑工具
	QLineEdit* m_pHeightLineEdit = nullptr;
	QPushButton* m_pColorChoose = nullptr;

	KxLeftToolBarBtn* m_pSelectButton = nullptr; //选择工具
	KxLeftToolBarBtn* m_pPencilButton = nullptr; //画笔工具
	KxLeftToolBarBtn* m_pLineButton = nullptr;   //直线工具
	KxLeftToolBarBtn* m_pRectButton = nullptr;   //矩形工具
	KxLeftToolBarBtn* m_pCircleButton = nullptr; //圆形工具
	KxLeftToolBarBtn* m_pPathButton = nullptr;   //绘制曲线工具
	KxLeftToolBarBtn* m_pShapeButton = nullptr;  //自定义图形工具

	QButtonGroup* m_pToolBarGroup = nullptr;

	QAction* m_pActionNew = nullptr;
	QAction* m_pActionOpen = nullptr;
	QAction* m_pActionSave = nullptr;

	QToolBar* m_pToolBarTop = nullptr;

	QSvgRenderer* m_pSvgRender = nullptr;
	KxSvgCanvas* m_pSvgCanvas = nullptr;
};