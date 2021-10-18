#pragma once
#pragma execution_character_set("utf-8")
#include <QtSvg/qsvgrenderer.h>
#include <QtWidgets/QMenuBar>
#include "shape.h"

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
	void inputMethodEvent(QInputMethodEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	bool deleteShapeList();
	void loadSvgRenderer(QString);
	void unloadSvgRenderer();
	bool isSvgValid();
	int getShapeCount();
	void shapeInClickRect();

public slots:
	void setCurrentType(ShapeType);
	void setCanvasSize();
	void setCanvasWidth(QString);
	void setCanvasHeight(QString);
	void openSvg();
	void saveSvg();
	void init();
	void setColor(QRgb);

private:
	Shape* getClickShape(QPoint point);
	bool isInRect(QPointF point, Shape* shape);
	void setPositionType(QPoint point);

	ShapeType m_currentType;				//新增对象的类型
	QList<Shape*> m_shapeList;				//对象链表
	QList<Shape*> m_clickShapeList;			//点击对象链表
	QSvgRenderer* m_pSvgRenderer = nullptr; //通用svg图片加载器
	QPoint m_transfrom;						//坐标变换量
	QPoint m_lastPoint;						//鼠标的前一个坐标点
	Shape* m_pCurrentShape = nullptr;		//新增对象
	Shape* m_pClickShape = nullptr;			//点击对象
	Shape* m_pClickRect = nullptr;			//拖拉的选择框
	qreal m_offset = 0.0;					//放大缩小的比率
	QRgb m_rgb;
	bool isMove = false;
	int m_canvasWidth = 500;
	int m_canvasHeight = 500;

	enum class mousePosition
	{
		move = 0,
		top,
		left,
		right,
		bottom,
		upperLeft,
		lowerLeft,
		upperRight,
		lowerRight,
		noClick
	};
	mousePosition m_positionType = mousePosition::noClick;

	void editShape(QPoint transformPoint);
	void updatePhysicalPoint();
};