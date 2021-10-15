#pragma once
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

	void init();
public slots:
	void setCurrentType(ShapeType);
	void setCanvasSize();
	void setCanvasWidth(QString);
	void setCanvasHeight(QString);
	void opensvg();
	void saveSvg();

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

	static int s_offsetStartX;
	static int s_offsetStartY;
	static int s_offsetWidth;
	static int s_offsetHeight;

	void editShape(QPoint transformPoint);
	void updatePhysicalPoint();
};