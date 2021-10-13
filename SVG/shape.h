#pragma once
#include <QGraphicsItem>

enum class ShapeType
{
	TypeSelect = 0,
	TypeLine,
	TypeSquare,
	tmp
};

class Shape
{
public:
	Shape();
	~Shape();

	virtual void drawShape(QPainter& painter) = 0;
	virtual void setDrawStar(QPoint star) = 0;
	virtual void setDrawEnd(QPoint end) = 0;
	virtual void setDepth(qreal depth) = 0;
	virtual void scale(qreal ratioW, qreal ratioH) = 0;
	QPoint& getPhysicalStar();
	QPoint& getPhysicalEnd();
	QPoint& getDrawStar();
	QPoint& getDrawEnd();
	void drawPointToPhysicalPoint(qreal ratio);

	void move(QPoint offset);
protected:
	QPoint m_star;      //物理坐标
	QPoint m_end;
	QPoint m_drawStar;  //逻辑坐标
	QPoint m_drawEnd;
	qreal m_depth;
};

class Line : public Shape
{
public:
	Line();
	Line(QPoint star, QPoint end);
	~Line();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPoint star) override;
	void setDrawEnd(QPoint end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;
};

class Square : public Shape
{
public:
	Square();
	Square(QPoint star, QPoint end);
	~Square();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPoint star) override;
	void setDrawEnd(QPoint end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

};

class ShapeFactory
{
public:
	static ShapeFactory* getShapeFactory();
	Shape* getShape(ShapeType);
		
protected:
	ShapeFactory();
};