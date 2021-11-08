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
	void setShapeColor(QRgb);         //����ѡ�ж�������
	void setPenColor(QRgb);			  //����ѡ�ж�����������ɫ
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

	QMenu* m_pRightClickMenu = nullptr;			 //�Ҽ��˵���
	QLineEdit* m_pTextEditWidget = nullptr;
	ShapeType m_currentType;				//�������������
	QList<Shape*> m_shapeList;				//��������
	QList<Shape*> m_clickShapeList;			//�����������
	QList<Shape*> m_copyShapeList;			//������������
	QSvgRenderer* m_pSvgRenderer = nullptr; //ͨ��svgͼƬ������
	QPoint m_transfrom;						//����任��
	QPoint m_lastPoint;						//����ǰһ�������
	QPointF m_shapeLastStartPointF;			//�����ƶ�ǰ�ĵ�ǰ��
	QPointF m_shapeLastEndPointF;			//�����ƶ�ǰ�ĵ�ǰ��
	Shape* m_pCurrentShape = nullptr;		//��������
	Shape* m_pClickShape = nullptr;			//�������
	Shape* m_pClickRect = nullptr;			//������ѡ���
	qreal m_radio = 0.0;					//�Ŵ���С�ı���
	qreal m_penWidth = 1.0;					//�������
	Qt::PenStyle m_penStyle = Qt::SolidLine;//������ʽ
	QRgb m_rgb;
	QString m_text;							//���ֿؼ�������ֵ
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