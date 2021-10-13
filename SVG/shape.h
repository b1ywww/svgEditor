#pragma once
#include <QList>

enum class ShapeType
{
	TypeSelect = 0,
	TypeLine,
	TypeSquare,
	TypePencil,
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
	virtual QPoint& getPhysicalStar();
	virtual QPoint& getPhysicalEnd();
	virtual QPoint& getDrawStar();
	virtual QPoint& getDrawEnd();
	virtual void drawPointToPhysicalPoint(qreal ratio);

	virtual void move(QPoint offset);
	virtual void moveTop(QPoint offset) = 0;
	virtual void moveBottom(QPoint offset) = 0;
	virtual void moveLeft(QPoint offset) = 0;
	virtual void moveRight(QPoint offset) = 0;
	virtual void moveUpperLeft(QPoint offset) = 0;
	virtual void moveUpperRight(QPoint offset) = 0;
	virtual void moveLowerLeft(QPoint offset) = 0;
	virtual void moveLowerRight(QPoint offset) = 0;

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

	void move(QPoint offset) override;
	void moveTop(QPoint offset) override;
	void moveBottom(QPoint offset) override;
	void moveLeft(QPoint offset) override;
	void moveRight(QPoint offset) override;
	void moveUpperLeft(QPoint offset) override;
	void moveUpperRight(QPoint offset) override;
	void moveLowerLeft(QPoint offset) override;
	void moveLowerRight(QPoint offset) override;
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

	void move(QPoint offset) override;
	void moveTop(QPoint offset) override;
	void moveBottom(QPoint offset) override;
	void moveLeft(QPoint offset) override;
	void moveRight(QPoint offset) override;
	void moveUpperLeft(QPoint offset) override;
	void moveUpperRight(QPoint offset) override;
	void moveLowerLeft(QPoint offset) override;
	void moveLowerRight(QPoint offset) override;

};

class Pancil : public Shape
{
public:
	Pancil();
	~Pancil();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPoint star) override;
	void setDrawEnd(QPoint end) override;
	void setDepth(qreal depth) override;
	void scale(qreal ratioW, qreal ratioH) override;
	void drawPointToPhysicalPoint(qreal ratio) override;

	void move(QPoint offset) override;
	void moveTop(QPoint offset) override;
	void moveBottom(QPoint offset) override;
	void moveLeft(QPoint offset) override;
	void moveRight(QPoint offset) override;
	void moveUpperLeft(QPoint offset) override;
	void moveUpperRight(QPoint offset) override;
	void moveLowerLeft(QPoint offset) override;
	void moveLowerRight(QPoint offset) override;

	void updateClickRect(QPoint point);

private:
	QList<QPoint> m_drawPoint;
	QList<QPoint> m_PhysicalPoint;
	int m_Left = 10000;  //让画笔抽象出一个矩形，这个矩形包含了整个画笔内容
	int m_right = -10000;
	int m_top = 10000;
	int m_bottom = -10000;
};

class ShapeFactory
{
public:
	static ShapeFactory* getShapeFactory();
	Shape* getShape(ShapeType);
		
protected:
	ShapeFactory();
};