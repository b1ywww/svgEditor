#pragma once
#include "kxcanvas.h"
#include "shape.h"

#include <QtWidgets/QMainWindow>
#include <QtSvg/qsvgrenderer.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QStackedWidget>
#include <QList>
#include <QScrollArea>

class Line;
class Shape;
class QLineEdit;
class KxSvgCanvas;

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
	~SVGMainWIndow();
	bool eventFilter(QObject* watched, QEvent* event) override;

public slots:
	void setCanvasColor();
	void setShapeColor();
	void setPenColor();
	void setShapePane(QColor shapeRgb, QColor penRgb);
	void paneIndex(int index = 0);

private:
	void setToolBar();
	void setSettingPane();
	void setSettingCanvas();
	void setSettingSquare(QString x = QString("0"), QString y = QString("0"));

	void setShapeChooseColor(QRgb rgb);
	void setPenChooseColor(QRgb rgb);

private:
	QHBoxLayout* m_pMainHoriLayout = nullptr;
	QHBoxLayout* m_pCentralLayout = nullptr;
	QVBoxLayout* m_pToolBarLayout = nullptr;
	QGridLayout* m_pSettingCanvasLayout = nullptr;
	QGridLayout* m_pSettingSquareLayout = nullptr;

	QStackedWidget* m_pages = nullptr;

	QWidget* m_pEditSquareX = nullptr;			//����X�����������
	QWidget* m_pEditSquareY = nullptr;			//����Y�����������

	QScrollArea* m_pCanvasScroll = nullptr;
	QWidget* m_pCentralWidget = nullptr;
	QWidget* m_pToolBarLeftWidget = nullptr;
	QWidget* m_pSettingCanvasWidget = nullptr;
	QWidget* m_pSettingSquareWidget = nullptr;

	QLineEdit* m_pWidthLineEdit = nullptr;		 //�������ı༭����
	QLineEdit* m_pHeightLineEdit = nullptr;
	QPushButton* m_pCanvasColorChoose = nullptr;
	QPushButton* m_pShapeColorChoose = nullptr;
	QPushButton* m_pPenColorChoose = nullptr;

	KxLeftToolBarBtn* m_pSelectButton = nullptr; //ѡ�񹤾�
	KxLeftToolBarBtn* m_pPencilButton = nullptr; //���ʹ���
	KxLeftToolBarBtn* m_pLineButton = nullptr;   //ֱ�߹���
	KxLeftToolBarBtn* m_pRectButton = nullptr;   //���ι���
	KxLeftToolBarBtn* m_pCircleButton = nullptr; //Բ�ι���
	KxLeftToolBarBtn* m_pPathButton = nullptr;   //�������߹���
	KxLeftToolBarBtn* m_pShapeButton = nullptr;  //�Զ���ͼ�ι���

	QButtonGroup* m_pToolBarGroup = nullptr;

	QAction* m_pActionNew = nullptr;
	QAction* m_pActionOpen = nullptr;
	QAction* m_pActionSave = nullptr;

	QToolBar* m_pToolBarTop = nullptr;

	QSvgRenderer* m_pSvgRender = nullptr;
	KxSvgCanvas* m_pSvgCanvas = nullptr;
};