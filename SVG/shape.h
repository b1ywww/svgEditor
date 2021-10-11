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
	virtual void setStar(QPoint star) = 0;
	virtual void setEnd(QPoint end) = 0;
	virtual void setDepth(qreal depth) = 0;
	virtual void scale(qreal width, qreal height) = 0;
	virtual QPoint& getStar() = 0;
	virtual QPoint& getEnd() = 0;

	void move(QPoint offset);
protected:
	QPoint m_star;
	QPoint m_end;
	QPoint m_drawStar;
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
	void setStar(QPoint star) override;
	void setEnd(QPoint end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;
	QPoint& getStar() override;
	QPoint& getEnd() override;
};

class Square : public Shape
{
public:
	Square();
	Square(QPoint star, QPoint end);
	~Square();

	void drawShape(QPainter& painter) override;
	void setStar(QPoint star) override;
	void setEnd(QPoint end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	QPoint& getStar() override;
	QPoint& getEnd() override;
};

class ShapeFactory
{
public:
	static ShapeFactory* getShapeFactory();
	Shape* getShape(ShapeType);
		
protected:
	ShapeFactory();
};