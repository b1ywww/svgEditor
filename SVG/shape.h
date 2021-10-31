#pragma once
#include <QList>
#include <QTextDocument>
#include <QPen>
#include <QBrush>

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
	virtual ~Shape();

	virtual void drawShape(QPainter& painter) = 0;
	virtual void drawClickRect(QPainter& painter);
	virtual void setDrawStart(QPointF star) = 0;
	virtual void setDrawEnd(QPointF end) = 0;
	virtual void setDepth(qreal depth) = 0;
	virtual void scale(qreal ratioW, qreal ratioH) = 0;
	virtual QPointF& getPhysicalStart();
	virtual QPointF& getPhysicalEnd();
	virtual QPointF& getDrawStart();
	virtual QPointF& getDrawEnd();
	virtual void copyDate(Shape* shape);
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

	void setClickRectOffset(qreal x, qreal y);
	void setClickRect();
	const QRectF getClickRect();
	ShapeType getShapeType();
	bool getClickState();
	void setClickState(bool click);
	ShapeType getShapeType() const;
	QPen& getPen();
	QBrush& getBrush();

	void setIsPaint(bool isPaint);
	bool isPaint();

protected:
	QPointF m_start;      //物理坐标
	QPointF m_end;
	QPointF m_drawStart;  //逻辑坐标
	QPointF m_drawEnd;
	qreal m_depth;
	QPen m_pen;
	QBrush m_brush;
	ShapeType m_type;

private:
	int m_offsetStartX = -4;
	int m_offsetStartY = -4;
	int m_offsetWidth = 8; 
	int m_offsetHeight = 8;
	QRectF m_clickRect;
	bool m_isClick = false;
	bool m_isPaint = true;
};

class Line : public Shape
{
public:
	Line();
	Line(QPointF star, QPointF end);
	~Line();

	void drawShape(QPainter& painter) override;
	void setDrawStart(QPointF star) override;
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

	void copyDate(Shape* shape);
};

class Square : public Shape
{
public:
	Square();
	Square(QPointF star, QPointF end);
	~Square();

	void drawShape(QPainter& painter) override;
	void setDrawStart(QPointF star) override;
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

	void copyDate(Shape* shape);
};

class Pencil : public Shape
{
public:
	Pencil();
	~Pencil();

	void drawShape(QPainter& painter) override;
	void setDrawStart(QPointF star) override;
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
	void updateEdgeValue();
	const QList<QPointF>& getPhysicalPoint();
	void setDrawPoint(QList<QPointF>& list);
	void setEndPoint();

	void copyDate(Shape* shape);

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
	void setDrawStart(QPointF star) override;
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

	void copyDate(Shape* shape);
};

class Hexagon : public Shape
{
public:
	Hexagon();
	~Hexagon();

	void drawShape(QPainter& painter) override;
	void setDrawStart(QPointF star) override;
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

	void setVertex(QVector<QPointF>& vector = QVector<QPointF>());
	const QVector<QPointF>& getVertex();

	void copyDate(Shape* shape);

private:
	QVector<QPointF> m_vertex;
};

class TextEdit : public Shape
{
public:
	TextEdit();
	~TextEdit();

	void drawShape(QPainter& painter) override;
	void setDrawStart(QPointF star) override;
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
	const QString getText();
	const int getFontSize();

	void copyDate(Shape* shape);

private:
	QString m_text;
	int m_fontSize = 20;

};

class ShapeFactory
{
public:
	static ShapeFactory* getShapeFactory();
	Shape* getShape(ShapeType);
	void deleteShapeFactory();
protected:
	ShapeFactory();
};