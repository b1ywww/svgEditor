#pragma once
#include <QList>
#include <QTextDocument>

enum class ShapeType
{
	TypeSelect = 0,
	TypeLine,
	TypeSquare,
	TypePencil,
	TypeCircle,
	TypeHexagon,
	TypeText,
	tmp
};

class Shape
{
public:
	Shape();
	~Shape();

	virtual void drawShape(QPainter& painter) = 0;
	virtual void setDrawStar(QPointF star) = 0;
	virtual void setDrawEnd(QPointF end) = 0;
	virtual void setDepth(qreal depth) = 0;
	virtual void scale(qreal ratioW, qreal ratioH) = 0;
	virtual QPointF& getPhysicalStar();
	virtual QPointF& getPhysicalEnd();
	virtual QPointF& getDrawStar();
	virtual QPointF& getDrawEnd();
	virtual void drawPointToPhysicalPoint(qreal ratio);

	virtual void move(QPointF offset);
	virtual void moveTop(QPointF offset) = 0;
	virtual void moveBottom(QPointF offset) = 0;
	virtual void moveLeft(QPointF offset) = 0;
	virtual void moveRight(QPointF offset) = 0;
	virtual void moveUpperLeft(QPointF offset) = 0;
	virtual void moveUpperRight(QPointF offset) = 0;
	virtual void moveLowerLeft(QPointF offset) = 0;
	virtual void moveLowerRight(QPointF offset) = 0;

	ShapeType getShapeType();
protected:
	QPointF m_star;      //物理坐标
	QPointF m_end;
	QPointF m_drawStar;  //逻辑坐标
	QPointF m_drawEnd;
	qreal m_depth;
	ShapeType m_type;
};

class Line : public Shape
{
public:
	Line();
	Line(QPointF star, QPointF end);
	~Line();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;
};

class Square : public Shape
{
public:
	Square();
	Square(QPointF star, QPointF end);
	~Square();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;

};

class Pancil : public Shape
{
public:
	Pancil();
	~Pancil();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal ratioW, qreal ratioH) override;
	void drawPointToPhysicalPoint(qreal ratio) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;

	void updateClickRect(QPointF point);
	const QList<QPointF>& getPhysicalPoint();

private:
	QList<QPointF> m_drawPoint;
	QList<QPointF> m_PhysicalPoint;
	qreal m_Left = 10000.0;  //让画笔抽象出一个矩形，这个矩形包含了整个画笔内容
	qreal m_right = -10000.0;
	qreal m_top = 10000.0;
	qreal m_bottom = -10000.0;
};

class Circle : public Shape
{
public:
	Circle();
	~Circle();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;
};

class Hexagon : public Shape
{
public:
	Hexagon();
	~Hexagon();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;

	void setVertex();
	const QVector<QPointF>& getVertex();
private:
	QVector<QPointF> m_vertex;
};

class TextEdit : public Shape
{
public:
	TextEdit();
	~TextEdit();

	void drawShape(QPainter& painter) override;
	void setDrawStar(QPointF star) override;
	void setDrawEnd(QPointF end) override;
	void setDepth(qreal depth) override;
	void scale(qreal width, qreal height) override;

	void move(QPointF offset) override;
	void moveTop(QPointF offset) override;
	void moveBottom(QPointF offset) override;
	void moveLeft(QPointF offset) override;
	void moveRight(QPointF offset) override;
	void moveUpperLeft(QPointF offset) override;
	void moveUpperRight(QPointF offset) override;
	void moveLowerLeft(QPointF offset) override;
	void moveLowerRight(QPointF offset) override;

	void setText(QString text);

private:
	QString m_text;

};

class ShapeFactory
{
public:
	static ShapeFactory* getShapeFactory();
	Shape* getShape(ShapeType);
		
protected:
	ShapeFactory();
};