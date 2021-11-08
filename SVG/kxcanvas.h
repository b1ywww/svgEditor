#pragma once
#pragma execution_character_set("utf-8")

#include "shape.h"
#include "transaction.h"

#include <QtSvg/qsvgrenderer.h>
#include <QtWidgets/QMenuBar>
#include <QLineEdit>
#include <QPixmap>

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
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

	bool deleteShapeList();
	void loadSvgRenderer(QString);
	void unloadSvgRenderer();
	bool isSvgValid();
	int getShapeCount();
	void shapeInClickRect();
	void setShapeColor(QRgb);         //设置选中对象的填充
	void setPenColor(QRgb);			  //设置选中对象线条的颜色
	void copyClickShape();
	void clearCopyList();
	void copyListToShapeList();

	void updatePixmap(Shape* shape);
	void updatePixmap(QList<Shape*>& list);

	const QUndoStack* getUndoStack();
	const qreal getRadio();

	void shapeListRemoveOne(Shape* item);
	void clickListRemoveOne(Shape* item);
	
	void readAddCommand();

public slots:
	void setCurrentType(ShapeType);
	void setCanvasSize();
	void setCanvasWidth(QString);
	void setCanvasHeight(QString);
	void openSvg();
	void saveSvg();
	void init();
	void setCanvasColor(QRgb);
	void changeText(QString);
	void setText();

	void setStrokeWidth(QString width);
	void setStroke();
	void setStrokeStyle(Qt::PenStyle style);

	void shear();
	void copy();
	void paste();
	void deleteShape();

	void shapeToUpper();
	void shapeToLower();
	void shapeToTop();
	void shapeToBottom();

signals:
	void setShapePane(QColor shapeRgb, QColor penRgb, qreal penWidth, Qt::PenStyle);
	void paneIndex(int);
	void setCanvasChooseColor(QRgb);
	void setCanvasChooseSize(int w, int h);

private:
	Shape* getClickShape(QPoint point);
	void setPositionType(QPoint point);
	void setRightClickMenu();

	QPixmap* m_pixmap = nullptr;

	QMenu* m_pRightClickMenu = nullptr;			 //右键菜单栏
	QLineEdit* m_pTextEditWidget = nullptr;
	ShapeType m_currentType;				//新增对象的类型
	QList<Shape*> m_shapeList;				//对象链表
	QList<Shape*> m_clickShapeList;			//点击对象链表
	QList<Shape*> m_copyShapeList;			//拷贝对象链表
	QSvgRenderer* m_pSvgRenderer = nullptr; //通用svg图片加载器
	QPoint m_transfrom;						//坐标变换量
	QPoint m_lastPoint;						//鼠标的前一个坐标点
	QPointF m_shapeLastStartPointF;			//对象移动前的当前点
	QPointF m_shapeLastEndPointF;			//对象移动前的当前点
	Shape* m_pCurrentShape = nullptr;		//新增对象
	Shape* m_pClickShape = nullptr;			//点击对象
	Shape* m_pClickRect = nullptr;			//拖拉的选择框
	qreal m_radio = 0.0;					//放大缩小的比率
	qreal m_penWidth = 1.0;					//线条宽度
	Qt::PenStyle m_penStyle = Qt::SolidLine;//线条样式
	QRgb m_rgb;
	QString m_text;							//文字控件的文字值
	bool isMove = false;
	int m_canvasWidth = 500;
	int m_canvasHeight = 500;
	bool m_isCloseEvent = false;
	bool m_isRepaint = true;
	QUndoStack* m_undoStack = nullptr;

	mousePosition m_positionType = mousePosition::noClick;

	void editShape(QPoint transformPoint);
	QPointF editoffset(QPointF start, QPointF end);
	void updatePhysicalPoint();
};