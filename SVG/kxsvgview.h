#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QGraphicsView>
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

public slots:
	void setCurrentType(ShapeType);

private:
	ShapeType m_currentType;
	QList<Shape*> m_shapeList;
	Shape* m_pCurrentShape = nullptr;

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

private:
	void setCanvasCenter();
	void setToolBar();

private:

	QHBoxLayout* m_pMainHoriLayout = nullptr;
	QHBoxLayout* m_pCentralLayout = nullptr;
	QVBoxLayout* m_pToolBarLayout = nullptr;
	
	QWidget* m_pCentralWidget = nullptr;
	QWidget* m_pToolBarLeftWidget = nullptr;
	QWidget* m_pCanvasWidget = nullptr;
	QWidget* m_pSettingWidget = nullptr;

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

